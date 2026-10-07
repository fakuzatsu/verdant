#!/usr/bin/env bash

set -euo pipefail

if [[ $# -ne 4 ]]; then
    echo "usage: $0 ROM_TEST_HYDRA MOBILE_ROM_TEST OBJCOPY TEST_ELF" >&2
    exit 2
fi

repo_root="$(cd "$(dirname "$0")/.." && pwd)"
api_port="${MOBILE_API_PORT:-18080}"
api_log="$(mktemp -t verdant-mobile-api.XXXXXX)"
api_pid=""

cleanup() {
    if [[ -n "$api_pid" ]] && kill -0 "$api_pid" 2>/dev/null; then
        kill "$api_pid" 2>/dev/null || true
        wait "$api_pid" 2>/dev/null || true
    fi
    rm -f "$api_log"
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

cd "$repo_root"
BIND_ADDRESS="127.0.0.1" PORT="$api_port" node pokemobile/app.js >"$api_log" 2>&1 &
api_pid=$!

api_ready=false
for _ in {1..100}; do
    if ! kill -0 "$api_pid" 2>/dev/null; then
        echo "PokeMobile API exited before becoming ready:" >&2
        cat "$api_log" >&2
        exit 1
    fi
    if curl --silent --fail --output /dev/null "http://127.0.0.1:${api_port}/Health"; then
        api_ready=true
        break
    fi
    sleep 0.1
done

if [[ "$api_ready" != true ]]; then
    echo "PokeMobile API did not become ready on port ${api_port}:" >&2
    cat "$api_log" >&2
    exit 1
fi

if ! MGBA_MOBILE_HTTP_PORT="$api_port" "$1" "$2" "$3" "$4"; then
    echo "PokeMobile API log:" >&2
    cat "$api_log" >&2
    exit 1
fi

if ! grep --quiet --fixed-strings "Mobile Adapter payload received: PING" "$api_log"; then
    echo "The ROM test passed without the API recording the expected payload:" >&2
    cat "$api_log" >&2
    exit 1
fi

if ! grep --quiet --fixed-strings "Record code ZATSU was downloaded" "$api_log"; then
    echo "The ROM test passed without the API recording the expected Record Mix download:" >&2
    cat "$api_log" >&2
    exit 1
fi

if ! grep --quiet --fixed-strings "was uploaded" "$api_log"; then
    echo "The ROM test passed without the API recording the expected Record Mix upload:" >&2
    cat "$api_log" >&2
    exit 1
fi

echo "PokeMobile API confirmed the debug exchange and Record Mix download/upload round trip."
