#include "global.h"
#include "internet_options_menu.h"
#include "internet_protocol.h"
#include "util.h"

struct Sha1Context
{
    u32 state[5];
    u64 byteCount;
    u8 block[64];
    u32 blockSize;
};

STATIC_ASSERT(sizeof(INTERNET_HMAC_SALT) - 1 == INTERNET_HMAC_SALT_SIZE, InternetHmacSaltSize);

static u32 RotateLeft(u32 value, u32 amount)
{
    return (value << amount) | (value >> (32 - amount));
}

static u32 ReadU32BigEndian(const u8 *data)
{
    return (u32)data[0] << 24
         | (u32)data[1] << 16
         | (u32)data[2] << 8
         | data[3];
}

static u16 ReadU16(const u8 *data)
{
    return data[0] | (data[1] << 8);
}

static u32 ReadU32(const u8 *data)
{
    return (u32)data[0]
         | (u32)data[1] << 8
         | (u32)data[2] << 16
         | (u32)data[3] << 24;
}

static void WriteU16(u8 *data, u16 value)
{
    data[0] = value;
    data[1] = value >> 8;
}

static void WriteU32(u8 *data, u32 value)
{
    data[0] = value;
    data[1] = value >> 8;
    data[2] = value >> 16;
    data[3] = value >> 24;
}

static void Sha1Transform(struct Sha1Context *context, const u8 *block)
{
    u32 words[16];
    u32 a = context->state[0];
    u32 b = context->state[1];
    u32 c = context->state[2];
    u32 d = context->state[3];
    u32 e = context->state[4];
    u32 f;
    u32 k;
    u32 temp;
    u32 i;

    for (i = 0; i < 16; i++)
        words[i] = ReadU32BigEndian(&block[i * 4]);

    for (i = 0; i < 80; i++)
    {
        u32 word;

        if (i >= 16)
        {
            words[i & 15] = RotateLeft(words[(i - 3) & 15]
                                     ^ words[(i - 8) & 15]
                                     ^ words[(i - 14) & 15]
                                     ^ words[i & 15], 1);
        }
        word = words[i & 15];

        if (i < 20)
        {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        else if (i < 40)
        {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if (i < 60)
        {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        else
        {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }

        temp = RotateLeft(a, 5) + f + e + k + word;
        e = d;
        d = c;
        c = RotateLeft(b, 30);
        b = a;
        a = temp;
    }

    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;
    context->state[4] += e;
}

static void Sha1Init(struct Sha1Context *context)
{
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->byteCount = 0;
    context->blockSize = 0;
}

static void Sha1Update(struct Sha1Context *context, const u8 *data, u32 size)
{
    u32 copySize;

    context->byteCount += size;
    while (size != 0)
    {
        copySize = min(size, sizeof(context->block) - context->blockSize);
        memcpy(&context->block[context->blockSize], data, copySize);
        context->blockSize += copySize;
        data += copySize;
        size -= copySize;

        if (context->blockSize == sizeof(context->block))
        {
            Sha1Transform(context, context->block);
            context->blockSize = 0;
        }
    }
}

static void Sha1Finish(struct Sha1Context *context, u8 *digest)
{
    u64 bitCount = context->byteCount * 8;
    u32 i;

    context->block[context->blockSize++] = 0x80;
    if (context->blockSize > 56)
    {
        memset(&context->block[context->blockSize], 0, sizeof(context->block) - context->blockSize);
        Sha1Transform(context, context->block);
        context->blockSize = 0;
    }
    memset(&context->block[context->blockSize], 0, 56 - context->blockSize);
    for (i = 0; i < 8; i++)
        context->block[63 - i] = bitCount >> (i * 8);
    Sha1Transform(context, context->block);

    for (i = 0; i < ARRAY_COUNT(context->state); i++)
    {
        digest[i * 4] = context->state[i] >> 24;
        digest[i * 4 + 1] = context->state[i] >> 16;
        digest[i * 4 + 2] = context->state[i] >> 8;
        digest[i * 4 + 3] = context->state[i];
    }
}

static void HmacSha1(const u8 *data, u32 size, u8 *digest)
{
    static const u8 salt[] = INTERNET_HMAC_SALT;
    struct Sha1Context context;
    u8 pad[64];
    u8 innerDigest[20];
    u32 i;

    memset(pad, 0x36, sizeof(pad));
    for (i = 0; i < INTERNET_HMAC_SALT_SIZE; i++)
        pad[i] ^= salt[i];
    Sha1Init(&context);
    Sha1Update(&context, pad, sizeof(pad));
    Sha1Update(&context, data, size);
    Sha1Finish(&context, innerDigest);

    memset(pad, 0x5C, sizeof(pad));
    for (i = 0; i < INTERNET_HMAC_SALT_SIZE; i++)
        pad[i] ^= salt[i];
    Sha1Init(&context);
    Sha1Update(&context, pad, sizeof(pad));
    Sha1Update(&context, innerDigest, sizeof(innerDigest));
    Sha1Finish(&context, digest);
}

static bool32 ValidatePacket(const u8 *packet, u16 packetSize, const char *magic, const u8 *reserved, u32 reservedCount)
{
    u32 i;

    if (packet == NULL || packetSize < INTERNET_ACK_PACKET_SIZE)
        return FALSE;
    if (memcmp(packet, magic, 4) != 0 || packet[4] != INTERNET_PROTOCOL_VERSION)
        return FALSE;
    for (i = 0; i < reservedCount; i++)
    {
        if (packet[reserved[i]] != 0)
            return FALSE;
    }
    return ReadU16(&packet[packetSize - 2]) == CalcCRC16WithTable(packet, packetSize - 2);
}

static void FinishPacket(u8 *packet, u16 packetSize)
{
    WriteU16(&packet[packetSize - 2], CalcCRC16WithTable(packet, packetSize - 2));
}

bool32 DecodeInternetSessionChallenge(const u8 *packet, u16 packetSize, u8 *nonce)
{
    static const u8 reserved[] = {5};

    if (nonce == NULL
     || packetSize != INTERNET_SESSION_CHALLENGE_PACKET_SIZE
     || !ValidatePacket(packet, packetSize, "PMSC", reserved, ARRAY_COUNT(reserved)))
        return FALSE;
    memcpy(nonce, &packet[6], INTERNET_SESSION_NONCE_SIZE);
    return TRUE;
}

bool32 BuildInternetSessionAuthPacket(const u8 *nonce, u8 *packet)
{
    u8 digest[20];

    if (nonce == NULL || packet == NULL)
        return FALSE;
    memset(packet, 0, INTERNET_SESSION_AUTH_PACKET_SIZE);
    memcpy(packet, "PMSA", 4);
    packet[4] = INTERNET_PROTOCOL_VERSION;
    memcpy(&packet[6], nonce, INTERNET_SESSION_NONCE_SIZE);
    HmacSha1(packet, 22, digest);
    memcpy(&packet[22], digest, 8);
    FinishPacket(packet, INTERNET_SESSION_AUTH_PACKET_SIZE);
    return TRUE;
}

bool32 DecodeInternetSessionResponse(const u8 *packet, u16 packetSize, u8 *token)
{
    static const u8 reserved[] = {5};

    if (token == NULL
     || packetSize != INTERNET_SESSION_RESPONSE_PACKET_SIZE
     || !ValidatePacket(packet, packetSize, "PMSS", reserved, ARRAY_COUNT(reserved)))
        return FALSE;
    memcpy(token, &packet[6], INTERNET_SESSION_TOKEN_SIZE);
    return TRUE;
}

void FormatInternetSessionToken(const u8 *token, char *text)
{
    static const char hex[] = "0123456789abcdef";
    u32 i;

    for (i = 0; i < INTERNET_SESSION_TOKEN_SIZE; i++)
    {
        text[i * 2] = hex[token[i] >> 4];
        text[i * 2 + 1] = hex[token[i] & 0xF];
    }
    text[INTERNET_SESSION_TOKEN_SIZE * 2] = '\0';
}

void BuildInternetIdentityRequest(u8 *packet)
{
    memset(packet, 0, INTERNET_IDENTITY_REQUEST_PACKET_SIZE);
    memcpy(packet, "PMIR", 4);
    packet[4] = INTERNET_PROTOCOL_VERSION;
    FinishPacket(packet, INTERNET_IDENTITY_REQUEST_PACKET_SIZE);
}

bool32 DecodeInternetIdentityResponse(const u8 *packet, u16 packetSize, u32 *pid, u8 *token)
{
    static const u8 reserved[] = {12, 13};
    u32 decodedPid;

    if (pid == NULL || token == NULL
     || packetSize != INTERNET_IDENTITY_RESPONSE_PACKET_SIZE
     || !ValidatePacket(packet, packetSize, "PMID", reserved, ARRAY_COUNT(reserved)))
        return FALSE;

    decodedPid = ReadU32(&packet[8]);
    if (decodedPid == 0 || decodedPid == NO_PID)
        return FALSE;
    *pid = decodedPid;
    memcpy(token, &packet[5], INTERNET_IDENTITY_TOKEN_SIZE);
    return TRUE;
}

void BuildInternetCredentialPacket(const char *magic, u32 pid, const u8 *token, u8 *packet)
{
    memset(packet, 0, INTERNET_CREDENTIAL_PACKET_SIZE);
    memcpy(packet, magic, 4);
    packet[4] = INTERNET_PROTOCOL_VERSION;
    memcpy(&packet[5], token, INTERNET_IDENTITY_TOKEN_SIZE);
    WriteU32(&packet[8], pid);
    FinishPacket(packet, INTERNET_CREDENTIAL_PACKET_SIZE);
}

bool32 DecodeInternetAcknowledgement(const u8 *packet, u16 packetSize)
{
    static const u8 reserved[] = {5};

    return packetSize == INTERNET_ACK_PACKET_SIZE
        && ValidatePacket(packet, packetSize, "PMAK", reserved, ARRAY_COUNT(reserved));
}
