# mGBA

The binaries in this folder are built from `mGBA`, an emulator for running Game Boy Advance games. The source code is available here: <https://github.com/mgba-emu/mgba>.
The source code for these specific builds is available from:

 - Windows: <https://github.com/mgba-emu/mgba/tree/7ee2be6c96222dca12a9a579b747fe5ff1829def>
 - Linux: <https://github.com/mgba-emu/mgba/tree/dbffb46c4e7d2e7a2cbed7c3488cece4c2176d4c>
 - Mac: <https://github.com/mgba-emu/mgba/tree/daf01b03d5316dac966acd4b05318a225cab12f5>

`mgba-mobile-rom-test-mac` is built from a separately maintained mGBA
repository based on the Mobile Adapter fork at
<https://github.com/Wit-MKW/mgba/tree/717fb3fd063985b5b0cd167c116f4ea3674744c8>.
The local development checkout is kept alongside this repository as `mgba/`.
It adds a dedicated headless target that attaches the GBA Mobile Adapter, uses
a deterministic valid EEPROM configuration by default, and accepts a 512-byte
override with `--mobile-config FILE` or the `MGBA_MOBILE_CONFIG` environment
variable.

The `check-mobile` target also expects a separately managed PokeMobile checkout
at `pokemobile/`; that server repository is not part of this repository.

The built-in configuration uses `#9677`, `test-user`, `test@example.com`, and
the game-supplied password `password1`. `make check-mobile` verifies that this
configuration can be read, PPP can be established, and an HTTP `PING`/`PONG`
exchange can be completed against `pokemobile`'s `/Debug` endpoint. The test
orchestration lives alongside the ROM test in `test/`; this directory keeps the
prebuilt custom runner. The API is started on `MOBILE_API_PORT` (port 18080 by
default), and the runner redirects the adapter's fixed HTTP port 80 to that
local port. Missing npm dependencies are installed automatically.
