#include "global.h"
#include "characters.h"
#include "event_data.h"
#include "item.h"
#include "internet_protocol.h"
#include "internet_options_menu.h"
#include "load_save.h"
#include "main.h"
#include "mobile_adapter.h"
#include "mystery_gift.h"
#include "pokemon.h"
#include "record_mixing.h"
#include "save.h"
#include "string_util.h"
#include "test/test.h"
#include "util.h"
#include "constants/easy_chat.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/tv.h"

EWRAM_DATA static u8 sRecordPacket[INTERNET_RECORD_MIX_MAX_PACKET_SIZE + 1] = {0};
EWRAM_DATA static u8 sUploadedRecordPacket[INTERNET_RECORD_MIX_MAX_PACKET_SIZE] = {0};

#define TEST_INTERNET_API_URL(path) "http://127.0.0.1/" path "?gameidentifier=" INTERNET_GAME_IDENTIFIER

static void WriteU16(u8 *dest, u16 value)
{
    dest[0] = value;
    dest[1] = value >> 8;
}

static void WriteU32(u8 *dest, u32 value)
{
    dest[0] = value;
    dest[1] = value >> 8;
    dest[2] = value >> 16;
    dest[3] = value >> 24;
}

static u16 ReadU16(const u8 *src)
{
    return src[0] | (src[1] << 8);
}

static u32 ReadU32(const u8 *src)
{
    return (u32)src[0]
         | (u32)src[1] << 8
         | (u32)src[2] << 16
         | (u32)src[3] << 24;
}

static enum InternetRecordMixResult ReceiveRecordPayload(const struct PlayerRecordEmerald *record, u16 blockMask)
{
    u8 *payload = &sRecordPacket[INTERNET_RECORD_MIX_HEADER_SIZE];

    memset(sRecordPacket, 0, sizeof(sRecordPacket));
    memcpy(sRecordPacket, "PMRM", 4);
    sRecordPacket[4] = 1;
    sRecordPacket[5] = VERSION_EMERALD;
    sRecordPacket[6] = LANGUAGE_ENGLISH;
    WriteU32(&sRecordPacket[8], 0x44332211);
    sRecordPacket[12] = EOS;
    WriteU16(&sRecordPacket[INTERNET_RECORD_MIX_BLOCK_MASK_OFFSET], blockMask);
    WriteU16(&sRecordPacket[22], sizeof(*record));
    memcpy(payload, record, sizeof(*record));
    WriteU16(&sRecordPacket[24], CalcCRC16WithTable(payload, sizeof(*record)));

    return ReceiveInternetRecordMix(sRecordPacket, INTERNET_RECORD_MIX_MAX_PACKET_SIZE, NULL);
}

static void InitGiftHeader(u8 *packet, u8 type, u16 payloadSize)
{
    packet[0] = 'P';
    packet[1] = 'M';
    packet[2] = 'G';
    packet[3] = 'F';
    packet[4] = 1;
    packet[5] = type;
    WriteU16(&packet[6], payloadSize);
    WriteU16(&packet[8], CalcCRC16WithTable(&packet[INTERNET_MYSTERY_GIFT_HEADER_SIZE], payloadSize));
    WriteU16(&packet[10], 0);
}

static bool32 StringsEqual(const char *left, const char *right)
{
    while (*left == *right)
    {
        if (*left == '\0')
            return TRUE;
        left++;
        right++;
    }

    return FALSE;
}

static bool32 BytesEqual(const u8 *left, const u8 *right, u32 size)
{
    u32 i;

    for (i = 0; i < size; i++)
    {
        if (left[i] != right[i])
            return FALSE;
    }

    return TRUE;
}

static bool32 GameStringsEqual(const u8 *left, const u8 *right)
{
    while (*left == *right)
    {
        if (*left == EOS)
            return TRUE;
        left++;
        right++;
    }

    return FALSE;
}

static bool32 AppendSessionToUrl(char *url, u32 capacity, const u8 *sessionToken)
{
    static const char parameter[] = "&session=";
    char tokenText[INTERNET_SESSION_TOKEN_TEXT_SIZE];
    u32 length = strlen(url);

    if (length + sizeof(parameter) - 1 + sizeof(tokenText) > capacity)
        return FALSE;
    FormatInternetSessionToken(sessionToken, tokenText);
    memcpy(&url[length], parameter, sizeof(parameter) - 1);
    memcpy(&url[length + sizeof(parameter) - 1], tokenText, sizeof(tokenText));
    return TRUE;
}

static bool32 BuildSessionUrl(char *url, u32 capacity, const char *baseUrl, const u8 *sessionToken)
{
    u32 length = strlen(baseUrl);

    if (length + 1 > capacity)
        return FALSE;
    memcpy(url, baseUrl, length + 1);
    return AppendSessionToUrl(url, capacity, sessionToken);
}

static bool32 BuildUploadedRecordUrl(char *url, u32 capacity, const u8 *code, u16 codeSize, const u8 *sessionToken)
{
    static const char prefix[] = TEST_INTERNET_API_URL("Record") "&code=";
    u32 i;

    if (codeSize == 0 || codeSize > INTERNET_RECORD_CODE_LENGTH)
        return FALSE;

    memcpy(url, prefix, sizeof(prefix) - 1);
    for (i = 0; i < codeSize; i++)
    {
        if (!((code[i] >= 'A' && code[i] <= 'Z')
           || (code[i] >= 'a' && code[i] <= 'z')
           || (code[i] >= '0' && code[i] <= '9')))
            return FALSE;
        url[sizeof(prefix) - 1 + i] = code[i];
    }
    url[sizeof(prefix) - 1 + codeSize] = '\0';
    return AppendSessionToUrl(url, capacity, sessionToken);
}

static bool32 HasPokeNews(u8 kind)
{
    u32 i;

    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pokeNews[i].kind == kind)
            return TRUE;
    }
    return FALSE;
}

static struct SecretBase *FindSecretBaseById(u8 secretBaseId)
{
    u32 i;

    for (i = 1; i < SECRET_BASES_COUNT; i++)
    {
        if (gSaveBlock1Ptr->secretBases[i].secretBaseId == secretBaseId)
            return &gSaveBlock1Ptr->secretBases[i];
    }
    return NULL;
}

static TVShow *FindTvShowByKind(u8 kind)
{
    u32 i;

    for (i = 0; i < TV_SHOWS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->tvShows[i].common.kind == kind)
            return &gSaveBlock1Ptr->tvShows[i];
    }
    return NULL;
}

static void SetTvShowTrainerIds(TVShow *show)
{
    show->common.trainerIdLo = 0x11;
    show->common.trainerIdHi = 0x22;
    show->common.srcTrainerIdLo = 0x33;
    show->common.srcTrainerIdHi = 0x44;
    show->common.srcTrainerId2Lo = 0x55;
    show->common.srcTrainerId2Hi = 0x66;
}

static void InitValidTrendWatcher(TVShow *show)
{
    show->trendWatcher.kind = TVSHOW_TREND_WATCHER;
    show->trendWatcher.words[0] = EC_EMPTY_WORD;
    show->trendWatcher.words[1] = EC_EMPTY_WORD;
    show->trendWatcher.playerName[0] = EOS;
    show->trendWatcher.language = LANGUAGE_ENGLISH;
    SetTvShowTrainerIds(show);
}

static u8 CalculateDaycareSeed(const TVShow *shows)
{
    const u8 *bytes = (const u8 *)shows;
    u8 sum = 0;
    u32 i;

    for (i = 0; i < 256; i++)
        sum += bytes[i];
    return sum;
}

TEST("Record Mix upload builder emits an exact native packet")
{
    static const u8 expectedName[] = _("UPLOAD");
    u8 *packet = sUploadedRecordPacket;
    const u8 *payload = &packet[INTERNET_RECORD_MIX_HEADER_SIZE];
    const struct PlayerRecordEmerald *record = (const void *)payload;
    u32 trainerId = 0x44332211;
    u16 packetSize;

    memcpy(gSaveBlock2Ptr->playerTrainerId, &trainerId, sizeof(trainerId));
    StringCopy(gSaveBlock2Ptr->playerName, COMPOUND_STRING("UPLOAD"));
    gSaveBlock1Ptr->tvShows[0].common.kind = TVSHOW_FAN_CLUB_LETTER;
    gSaveBlock1Ptr->tvShows[0].common.active = TRUE;

    packetSize = BuildInternetRecordMixPacket(packet, INTERNET_RECORD_MIX_MAX_PACKET_SIZE);

    EXPECT_EQ(packetSize, INTERNET_RECORD_MIX_MAX_PACKET_SIZE);
    EXPECT(BytesEqual(packet, (const u8 *)"PMRM", 4));
    EXPECT_EQ(packet[4], 1);
    EXPECT_EQ(packet[5], VERSION_EMERALD);
    EXPECT_EQ(packet[6], GAME_LANGUAGE);
    EXPECT_EQ(packet[7], 0);
    EXPECT_EQ(ReadU32(&packet[8]), trainerId);
    EXPECT(GameStringsEqual(&packet[12], expectedName));
    EXPECT_EQ(ReadU16(&packet[INTERNET_RECORD_MIX_BLOCK_MASK_OFFSET]), INTERNET_RECORD_MIX_ALL_BLOCKS);
    EXPECT_EQ(ReadU16(&packet[22]), sizeof(struct PlayerRecordEmerald));
    EXPECT_EQ(ReadU16(&packet[24]), CalcCRC16WithTable(payload, sizeof(struct PlayerRecordEmerald)));
    EXPECT_EQ(ReadU16(&packet[26]), 0);
    EXPECT(!record->tvShows[0].common.active);
    EXPECT(gSaveBlock1Ptr->tvShows[0].common.active);
    EXPECT_EQ(BuildInternetRecordMixPacket(packet, INTERNET_RECORD_MIX_MAX_PACKET_SIZE - 1), 0);
    EXPECT_EQ(BuildInternetRecordMixPacket(NULL, INTERNET_RECORD_MIX_MAX_PACKET_SIZE), 0);
}

TEST("Internet session authentication matches the shared HMAC vector")
{
    static const u8 expected[INTERNET_SESSION_AUTH_PACKET_SIZE] = {
        0x50, 0x4D, 0x53, 0x41, 0x01, 0x00, 0x00, 0x01,
        0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
        0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x97, 0x21,
        0x6C, 0x88, 0x12, 0x49, 0xDA, 0x86, 0xDF, 0x81,
    };
    u8 nonce[INTERNET_SESSION_NONCE_SIZE];
    u8 packet[INTERNET_SESSION_AUTH_PACKET_SIZE];
    u32 i;

    for (i = 0; i < ARRAY_COUNT(nonce); i++)
        nonce[i] = i;

    EXPECT(BuildInternetSessionAuthPacket(nonce, packet));
    EXPECT(BytesEqual(packet, expected, sizeof(expected)));
}

TEST("Internet control packet decoders require exact shape and CRC")
{
    u8 challenge[INTERNET_SESSION_CHALLENGE_PACKET_SIZE] = {0};
    u8 response[INTERNET_SESSION_RESPONSE_PACKET_SIZE] = {0};
    u8 identity[INTERNET_IDENTITY_RESPONSE_PACKET_SIZE] = {0};
    u8 nonce[INTERNET_SESSION_NONCE_SIZE] = {0};
    u8 sessionToken[INTERNET_SESSION_TOKEN_SIZE] = {0};
    u8 identityToken[INTERNET_IDENTITY_TOKEN_SIZE] = {0};
    u32 pid = 0;

    memcpy(challenge, "PMSC", 4);
    challenge[4] = INTERNET_PROTOCOL_VERSION;
    WriteU16(&challenge[sizeof(challenge) - 2], CalcCRC16WithTable(challenge, sizeof(challenge) - 2));
    EXPECT(DecodeInternetSessionChallenge(challenge, sizeof(challenge), nonce));
    EXPECT(!DecodeInternetSessionChallenge(challenge, sizeof(challenge) - 1, nonce));
    challenge[5] = 1;
    EXPECT(!DecodeInternetSessionChallenge(challenge, sizeof(challenge), nonce));

    memcpy(response, "PMSS", 4);
    response[4] = INTERNET_PROTOCOL_VERSION;
    WriteU16(&response[sizeof(response) - 2], CalcCRC16WithTable(response, sizeof(response) - 2));
    EXPECT(DecodeInternetSessionResponse(response, sizeof(response), sessionToken));
    response[15] ^= 1;
    EXPECT(!DecodeInternetSessionResponse(response, sizeof(response), sessionToken));

    memcpy(identity, "PMID", 4);
    identity[4] = INTERNET_PROTOCOL_VERSION;
    identity[5] = 1;
    WriteU32(&identity[8], 0x12345678);
    WriteU16(&identity[sizeof(identity) - 2], CalcCRC16WithTable(identity, sizeof(identity) - 2));
    EXPECT(DecodeInternetIdentityResponse(identity, sizeof(identity), &pid, identityToken));
    EXPECT_EQ(pid, 0x12345678);
    EXPECT_EQ(identityToken[0], 1);
}

TEST("Mobile Adapter exchanges data with the PokeMobile API")
{
    static const u8 request[] = {'P', 'I', 'N', 'G'};
    static const u8 expectedResponse[] = {'P', 'O', 'N', 'G'};
    MA_TELDATA telephone = {0};
    char userId[33] = {0};
    char responseHeaders[256] = {0};
    u8 response[sizeof(expectedResponse) + 1] = {0};
    u8 challengePacket[INTERNET_SESSION_CHALLENGE_PACKET_SIZE + 1] = {0};
    u8 authPacket[INTERNET_SESSION_AUTH_PACKET_SIZE] = {0};
    u8 sessionPacket[INTERNET_SESSION_RESPONSE_PACKET_SIZE + 1] = {0};
    u8 identityRequest[INTERNET_IDENTITY_REQUEST_PACKET_SIZE] = {0};
    u8 identityPacket[INTERNET_IDENTITY_RESPONSE_PACKET_SIZE + 1] = {0};
    u8 credentialPacket[INTERNET_CREDENTIAL_PACKET_SIZE] = {0};
    u8 ackPacket[INTERNET_ACK_PACKET_SIZE + 1] = {0};
    u8 nonce[INTERNET_SESSION_NONCE_SIZE] = {0};
    u8 sessionToken[INTERNET_SESSION_TOKEN_SIZE] = {0};
    u8 identityToken[INTERNET_IDENTITY_TOKEN_SIZE] = {0};
    u8 giftPacket[INTERNET_MYSTERY_GIFT_MAX_PACKET_SIZE + 1] = {0};
    static const u8 expectedRecordSource[] = _("Zatsu");
    static const u8 expectedBaseOwner[] = _("Zatsu");
    u8 recordSource[PLAYER_NAME_LENGTH + 1] = {0};
    u8 uploadedCode[INTERNET_RECORD_CODE_LENGTH + 1] = {0};
    u8 displayedCode[INTERNET_RECORD_CODE_LENGTH + 1] = {0};
    char operationUrl[128] = {0};
    char uploadedRecordUrl[128] = {0};
    struct InternetMysteryGift gift = {0};
    struct InternetMysteryGift claimedGift = {0};
    u16 responseSize = 0;
    u16 challengePacketSize = 0;
    u16 sessionPacketSize = 0;
    u16 identityPacketSize = 0;
    u16 ackPacketSize = 0;
    u16 giftPacketSize = 0;
    u16 firstGiftPacketSize = 0;
    u16 recordPacketSize = 0;
    u16 uploadedCodeSize = 0;
    u16 uploadedPacketSize = 0;
    u16 roundTripPacketSize = 0;
    int initResult;
    int connectionDataResult = -1;
    int connectResult = -1;
    int uploadResult = -1;
    int challengeResult = -1;
    int sessionResult = -1;
    int identityResult = -1;
    int downloadResult = -1;
    int ackResult = -1;
    int secondGiftResult = -1;
    int recordDownloadResult = -1;
    int recordUploadResult = -1;
    int roundTripDownloadResult = -1;
    int disconnectResult = -1;
    u16 originalTrendWord = 0x1234;
    enum InternetMysteryGiftResult giftResult = INTERNET_MYSTERY_GIFT_INVALID_PACKET;
    enum InternetMysteryGiftResult claimedGiftResult = INTERNET_MYSTERY_GIFT_INVALID_PACKET;
    enum InternetRecordMixResult maskedRecordResult = INTERNET_RECORD_MIX_INVALID_PACKET;
    enum InternetRecordMixResult recordResult = INTERNET_RECORD_MIX_INVALID_PACKET;
    bool32 uploadedCodeValid = FALSE;
    bool32 sessionValid = FALSE;
    bool32 identityValid = FALSE;
    bool32 ackValid = FALSE;
    struct SecretBase *receivedBase;
    u32 pid = NO_PID;
    u32 i;

    initResult = maInitLibrary();
    if (initResult == 0)
        connectionDataResult = maGetConnectionData(&telephone, userId);
    if (connectionDataResult == 0)
        connectResult = maConnectServer(&telephone, userId, "password1");
    if (connectResult == 0)
        uploadResult = maUpload("http://127.0.0.1/Debug", responseHeaders, sizeof(responseHeaders),
                                request, sizeof(request), response, sizeof(response), &responseSize, "", "");
    if (uploadResult == 0)
        challengeResult = maDownload(TEST_INTERNET_API_URL("Session/Challenge"),
                                     responseHeaders, sizeof(responseHeaders), challengePacket, sizeof(challengePacket),
                                     &challengePacketSize, "", "");
    if (challengeResult == 0
     && DecodeInternetSessionChallenge(challengePacket, challengePacketSize, nonce)
     && BuildInternetSessionAuthPacket(nonce, authPacket))
    {
        sessionResult = maUpload(TEST_INTERNET_API_URL("Session"),
                                 responseHeaders, sizeof(responseHeaders), authPacket, sizeof(authPacket),
                                 sessionPacket, sizeof(sessionPacket), &sessionPacketSize, "", "");
    }
    if (sessionResult == 0)
        sessionValid = DecodeInternetSessionResponse(sessionPacket, sessionPacketSize, sessionToken);
    if (sessionValid
     && BuildSessionUrl(operationUrl, sizeof(operationUrl),
                        TEST_INTERNET_API_URL("Identity"), sessionToken))
    {
        BuildInternetIdentityRequest(identityRequest);
        identityResult = maUpload(operationUrl, responseHeaders, sizeof(responseHeaders),
                                  identityRequest, sizeof(identityRequest), identityPacket, sizeof(identityPacket),
                                  &identityPacketSize, "", "");
    }
    if (identityResult == 0)
        identityValid = DecodeInternetIdentityResponse(identityPacket, identityPacketSize, &pid, identityToken);
    if (identityValid
     && BuildSessionUrl(operationUrl, sizeof(operationUrl),
                        TEST_INTERNET_API_URL("Gift"), sessionToken))
    {
        BuildInternetCredentialPacket("PMGR", pid, identityToken, credentialPacket);
        downloadResult = maUpload(operationUrl, responseHeaders, sizeof(responseHeaders),
                                  credentialPacket, sizeof(credentialPacket), giftPacket, sizeof(giftPacket),
                                  &giftPacketSize, "", "");
    }
    if (downloadResult == 0)
    {
        firstGiftPacketSize = giftPacketSize;
        giftResult = ReceiveInternetMysteryGift(giftPacket, giftPacketSize, &gift);
    }
    if (giftResult == INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PARTY
     && BuildSessionUrl(operationUrl, sizeof(operationUrl),
                        TEST_INTERNET_API_URL("Gift/Acknowledge"), sessionToken))
    {
        BuildInternetCredentialPacket("PMGA", pid, identityToken, credentialPacket);
        ackResult = maUpload(operationUrl, responseHeaders, sizeof(responseHeaders),
                             credentialPacket, sizeof(credentialPacket), ackPacket, sizeof(ackPacket),
                             &ackPacketSize, "", "");
    }
    if (ackResult == 0)
        ackValid = DecodeInternetAcknowledgement(ackPacket, ackPacketSize);
    if (ackValid
     && BuildSessionUrl(operationUrl, sizeof(operationUrl),
                        TEST_INTERNET_API_URL("Gift"), sessionToken))
    {
        BuildInternetCredentialPacket("PMGR", pid, identityToken, credentialPacket);
        secondGiftResult = maUpload(operationUrl, responseHeaders, sizeof(responseHeaders),
                                    credentialPacket, sizeof(credentialPacket), giftPacket, sizeof(giftPacket),
                                    &giftPacketSize, "", "");
    }
    if (secondGiftResult == 0)
        claimedGiftResult = ReceiveInternetMysteryGift(giftPacket, giftPacketSize, &claimedGift);
    if (claimedGiftResult == INTERNET_MYSTERY_GIFT_ALREADY_RECEIVED
     && BuildSessionUrl(operationUrl, sizeof(operationUrl),
                        TEST_INTERNET_API_URL("Record") "&code=Zatsu", sessionToken))
    {
        recordDownloadResult = maDownload(operationUrl, responseHeaders, sizeof(responseHeaders),
                                          sRecordPacket, sizeof(sRecordPacket), &recordPacketSize, "", "");
    }
    if (recordDownloadResult == 0)
    {
        gSaveBlock1Ptr->dewfordTrends[0].words[0] = originalTrendWord;
        EXPECT_EQ(gSaveBlock1Ptr->dewfordTrends[0].words[0], originalTrendWord);
        sRecordPacket[INTERNET_RECORD_MIX_BLOCK_MASK_OFFSET] = INTERNET_RECORD_MIX_BLOCK_GIFT_ITEM;
        sRecordPacket[INTERNET_RECORD_MIX_BLOCK_MASK_OFFSET + 1] = 0;
        maskedRecordResult = ReceiveInternetRecordMix(sRecordPacket, recordPacketSize, recordSource);
        EXPECT_EQ(gSaveBlock1Ptr->dewfordTrends[0].words[0], originalTrendWord);
    }
    if (maskedRecordResult == INTERNET_RECORD_MIX_RECEIVED)
    {
        sRecordPacket[INTERNET_RECORD_MIX_BLOCK_MASK_OFFSET] = INTERNET_RECORD_MIX_BLOCK_POKE_NEWS
                                                            | INTERNET_RECORD_MIX_BLOCK_DEWFORD
                                                            | INTERNET_RECORD_MIX_BLOCK_SECRET_BASES;
        FlagSet(FLAG_RECEIVED_SECRET_POWER);
        recordResult = ReceiveInternetRecordMix(sRecordPacket, recordPacketSize, recordSource);
    }
    if (recordResult == INTERNET_RECORD_MIX_RECEIVED)
    {
        StringCopy(gSaveBlock2Ptr->playerName, COMPOUND_STRING("LOCAL"));
        uploadedPacketSize = BuildInternetRecordMixPacket(sUploadedRecordPacket, sizeof(sUploadedRecordPacket));
        if (BuildSessionUrl(operationUrl, sizeof(operationUrl),
                            TEST_INTERNET_API_URL("Record"), sessionToken))
        {
            recordUploadResult = maUpload(operationUrl, responseHeaders, sizeof(responseHeaders),
                                          sUploadedRecordPacket, uploadedPacketSize,
                                          uploadedCode, sizeof(uploadedCode), &uploadedCodeSize, "", "");
        }
    }
    if (recordUploadResult == 0)
    {
        uploadedCodeValid = DecodeInternetRecordMixCode(uploadedCode, uploadedCodeSize, displayedCode)
                         && BuildUploadedRecordUrl(uploadedRecordUrl, sizeof(uploadedRecordUrl), uploadedCode,
                                                   uploadedCodeSize, sessionToken);
    }
    if (uploadedCodeValid)
    {
        roundTripDownloadResult = maDownload(uploadedRecordUrl, responseHeaders, sizeof(responseHeaders),
                                             sRecordPacket, sizeof(sRecordPacket), &roundTripPacketSize, "", "");
    }
    if (connectResult == 0)
        disconnectResult = maDisconnect();
    maEnd();

    EXPECT_EQ(initResult, 0);
    EXPECT_EQ(connectionDataResult, 0);
    EXPECT(StringsEqual(telephone.telNo, "#9677"));
    EXPECT(StringsEqual(telephone.comment, "mGBA test"));
    EXPECT(StringsEqual(userId, "test-user"));
    EXPECT_EQ(connectResult, 0);
    EXPECT_EQ(uploadResult, 0);
    EXPECT_EQ(responseSize, sizeof(expectedResponse));
    EXPECT(BytesEqual(response, expectedResponse, sizeof(expectedResponse)));
    EXPECT_EQ(challengeResult, 0);
    EXPECT_EQ(challengePacketSize, INTERNET_SESSION_CHALLENGE_PACKET_SIZE);
    EXPECT_EQ(sessionResult, 0);
    EXPECT(sessionValid);
    EXPECT_EQ(sessionPacketSize, INTERNET_SESSION_RESPONSE_PACKET_SIZE);
    EXPECT_EQ(identityResult, 0);
    EXPECT(identityValid);
    EXPECT(pid != 0 && pid != NO_PID);
    EXPECT_EQ(downloadResult, 0);
    EXPECT_EQ(firstGiftPacketSize, INTERNET_MYSTERY_GIFT_MAX_PACKET_SIZE);
    EXPECT_EQ(giftResult, INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PARTY);
    EXPECT_EQ(gift.type, INTERNET_MYSTERY_GIFT_POKEMON);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_PERSONALITY), 0x12345678);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_SPECIES), SPECIES_PIKACHU);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_OT_ID), 4002541612);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_MARKINGS), 5);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_TERA_TYPE), TYPE_ELECTRIC);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_HELD_ITEM), ITEM_LIGHT_BALL);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_EXP), 12345);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_FRIENDSHIP), 70);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_MOVE1), MOVE_THUNDERBOLT);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_PP1), 15);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_HP_EV), 12);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_ATK_EV), 23);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_MET_LOCATION), 55);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_MET_LEVEL), 20);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_MET_GAME), VERSION_EMERALD);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_DYNAMAX_LEVEL), 5);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_OT_GENDER), 1);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_HP_IV), 28);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_ABILITY_NUM), 2);
    EXPECT_EQ(GetMonData(&gift.data.pokemon, MON_DATA_MODERN_FATEFUL_ENCOUNTER), 1);
    EXPECT_EQ(ackResult, 0);
    EXPECT(ackValid);
    EXPECT_EQ(ackPacketSize, INTERNET_ACK_PACKET_SIZE);
    EXPECT_EQ(secondGiftResult, 0);
    EXPECT_EQ(giftPacketSize, INTERNET_MYSTERY_GIFT_HEADER_SIZE);
    EXPECT_EQ(claimedGiftResult, INTERNET_MYSTERY_GIFT_ALREADY_RECEIVED);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_SPECIES), SPECIES_PIKACHU);
    EXPECT_EQ(recordDownloadResult, 0);
    EXPECT_EQ(recordPacketSize, INTERNET_RECORD_MIX_MAX_PACKET_SIZE);
    EXPECT_EQ(maskedRecordResult, INTERNET_RECORD_MIX_RECEIVED);
    EXPECT_EQ(recordResult, INTERNET_RECORD_MIX_RECEIVED);
    EXPECT_EQ(uploadedPacketSize, INTERNET_RECORD_MIX_MAX_PACKET_SIZE);
    EXPECT_EQ(recordUploadResult, 0);
    EXPECT(uploadedCodeValid);
    EXPECT_EQ(roundTripDownloadResult, 0);
    EXPECT_EQ(roundTripPacketSize, uploadedPacketSize);
    EXPECT(BytesEqual(sRecordPacket, sUploadedRecordPacket, uploadedPacketSize));
    EXPECT(GameStringsEqual(recordSource, expectedRecordSource));
    EXPECT_EQ(gSaveBlock1Ptr->dewfordTrends[0].words[0], EC_WORD_EXCELLENT);
    EXPECT_EQ(gSaveBlock1Ptr->dewfordTrends[0].words[1], EC_WORD_SOFTWARE);
    EXPECT(HasPokeNews(POKENEWS_BLENDMASTER));
    receivedBase = FindSecretBaseById(164);
    EXPECT(receivedBase != NULL);
    if (receivedBase != NULL)
    {
        EXPECT(GameStringsEqual(receivedBase->trainerName, expectedBaseOwner));
        EXPECT(receivedBase->gender == MALE);
        EXPECT_EQ(receivedBase->party.species[0], SPECIES_LURANTIS);
        EXPECT_EQ(receivedBase->party.species[1], SPECIES_KOMMO_O);
        EXPECT_EQ(receivedBase->party.levels[0], 80);
        EXPECT_EQ(receivedBase->party.levels[1], 80);
        for (i = 0; i < PARTY_SIZE; i++)
        {
            EXPECT_EQ(receivedBase->party.heldItems[i], ITEM_NONE);
            EXPECT_EQ(receivedBase->party.EVs[i], 0);
        }
        for (i = 0; i < DECOR_MAX_SECRET_BASE; i++)
            EXPECT_EQ(receivedBase->decorations[i], 0);
    }
    EXPECT_EQ(disconnectResult, 0);
}

TEST("Mystery Gift packet distinguishes and receives an item")
{
    u8 itemPacket[INTERNET_MYSTERY_GIFT_HEADER_SIZE + 4] = {0};
    struct InternetMysteryGift gift = {0};
    enum InternetMysteryGiftResult result;

    WriteU16(&itemPacket[INTERNET_MYSTERY_GIFT_HEADER_SIZE], ITEM_POTION);
    WriteU16(&itemPacket[INTERNET_MYSTERY_GIFT_HEADER_SIZE + 2], 3);
    InitGiftHeader(itemPacket, INTERNET_MYSTERY_GIFT_ITEM, 4);
    SetBagItemsPointers();
    result = ReceiveInternetMysteryGift(itemPacket, sizeof(itemPacket), &gift);

    EXPECT_EQ(result, INTERNET_MYSTERY_GIFT_RECEIVED_ITEM);
    EXPECT_EQ(gift.type, INTERNET_MYSTERY_GIFT_ITEM);
    EXPECT_EQ(gift.data.item.itemId, ITEM_POTION);
    EXPECT_EQ(gift.data.item.quantity, 3);
    EXPECT(CheckBagHasItem(ITEM_POTION, 3));
}

TEST("Mystery Gift receiver rejects malformed sizes and CRC")
{
    u8 itemPacket[INTERNET_MYSTERY_GIFT_HEADER_SIZE + 5] = {0};
    struct InternetMysteryGift gift = {0};
    u16 potionCount;

    WriteU16(&itemPacket[INTERNET_MYSTERY_GIFT_HEADER_SIZE], ITEM_POTION);
    WriteU16(&itemPacket[INTERNET_MYSTERY_GIFT_HEADER_SIZE + 2], 1);
    InitGiftHeader(itemPacket, INTERNET_MYSTERY_GIFT_ITEM, 4);
    SetBagItemsPointers();
    potionCount = CountTotalItemQuantityInBag(ITEM_POTION);

    EXPECT_EQ(ReceiveInternetMysteryGift(itemPacket, sizeof(itemPacket), &gift), INTERNET_MYSTERY_GIFT_INVALID_PACKET);

    itemPacket[8] ^= 1;
    EXPECT_EQ(ReceiveInternetMysteryGift(itemPacket, sizeof(itemPacket) - 1, &gift), INTERNET_MYSTERY_GIFT_INVALID_PACKET);
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POTION), potionCount);
}

TEST("Mystery Gift receiver rejects an out-of-range hidden nature")
{
    u8 packet[INTERNET_MYSTERY_GIFT_MAX_PACKET_SIZE] = {0};
    struct BoxPokemon boxMon = {0};
    struct InternetMysteryGift gift = {0};
    u32 hiddenNature = NUM_NATURES;
    u8 partyCount = gPlayerPartyCount;

    CreateBoxMon(&boxMon, SPECIES_PIKACHU, 5, 0, FALSE, 0, OT_ID_PLAYER_ID, 0);
    SetBoxMonData(&boxMon, MON_DATA_HIDDEN_NATURE, &hiddenNature);
    memcpy(&packet[INTERNET_MYSTERY_GIFT_HEADER_SIZE], &boxMon, sizeof(boxMon));
    InitGiftHeader(packet, INTERNET_MYSTERY_GIFT_POKEMON, sizeof(boxMon));

    EXPECT_EQ(ReceiveInternetMysteryGift(packet, sizeof(packet), &gift), INTERNET_MYSTERY_GIFT_INVALID_PACKET);
    EXPECT_EQ(gPlayerPartyCount, partyCount);
}

TEST("Internet Record Mix decoder requires exact sizes and payload CRC")
{
    u8 *packet = sRecordPacket;
    u8 *payload = &packet[INTERNET_RECORD_MIX_HEADER_SIZE];
    u16 payloadSize = sizeof(struct PlayerRecordEmerald);

    memset(packet, 0, INTERNET_RECORD_MIX_MAX_PACKET_SIZE);
    memcpy(packet, "PMRM", 4);
    packet[4] = 1;
    packet[5] = VERSION_EMERALD;
    packet[6] = LANGUAGE_ENGLISH;
    packet[12] = EOS;
    WriteU16(&packet[INTERNET_RECORD_MIX_BLOCK_MASK_OFFSET], INTERNET_RECORD_MIX_BLOCK_GIFT_ITEM);
    WriteU16(&packet[22], payloadSize);
    WriteU16(&packet[24], CalcCRC16WithTable(payload, payloadSize));

    EXPECT_EQ(ReceiveInternetRecordMix(packet, INTERNET_RECORD_MIX_MAX_PACKET_SIZE, NULL), INTERNET_RECORD_MIX_RECEIVED);
    EXPECT_EQ(ReceiveInternetRecordMix(packet, INTERNET_RECORD_MIX_MAX_PACKET_SIZE - 1, NULL), INTERNET_RECORD_MIX_INVALID_PACKET);

    WriteU16(&packet[22], payloadSize - 1);
    EXPECT_EQ(ReceiveInternetRecordMix(packet, INTERNET_RECORD_MIX_MAX_PACKET_SIZE, NULL), INTERNET_RECORD_MIX_INVALID_PACKET);
    WriteU16(&packet[22], payloadSize);

    payload[payloadSize - 1] ^= 1;
    EXPECT_EQ(ReceiveInternetRecordMix(packet, INTERNET_RECORD_MIX_MAX_PACKET_SIZE, NULL), INTERNET_RECORD_MIX_INVALID_PACKET);
}

TEST("Internet Record Mix reuses one-way legacy TV mixing")
{
    struct PlayerRecordEmerald record;
    TVShow *normalShow;
    TVShow *recordMixShow;
    TVShow *outbreakShow;
    u32 i;

    memset(&record, 0, sizeof(record));
    memset(gSaveBlock1Ptr->tvShows, 0, sizeof(gSaveBlock1Ptr->tvShows));
    gSaveBlock2Ptr->playerTrainerId[0] = 0xAA;
    gSaveBlock2Ptr->playerTrainerId[1] = 0xBB;

    gSaveBlock1Ptr->tvShows[NUM_NORMAL_TVSHOW_SLOTS].safariFanClub.kind = TVSHOW_SAFARI_FAN_CLUB;
    gSaveBlock1Ptr->tvShows[NUM_NORMAL_TVSHOW_SLOTS].safariFanClub.playerName[0] = EOS;
    gSaveBlock1Ptr->tvShows[NUM_NORMAL_TVSHOW_SLOTS].safariFanClub.language = LANGUAGE_ENGLISH;

    record.tvShows[0].recentHappenings.kind = TVSHOW_RECENT_HAPPENINGS;
    record.tvShows[0].recentHappenings.species = SPECIES_PIKACHU;
    for (i = 0; i < ARRAY_COUNT(record.tvShows[0].recentHappenings.words); i++)
        record.tvShows[0].recentHappenings.words[i] = EC_EMPTY_WORD;
    record.tvShows[0].recentHappenings.playerName[0] = EOS;
    record.tvShows[0].recentHappenings.language = LANGUAGE_ENGLISH;
    SetTvShowTrainerIds(&record.tvShows[0]);

    InitValidTrendWatcher(&record.tvShows[1]);

    record.tvShows[2].massOutbreak.kind = TVSHOW_MASS_OUTBREAK;
    record.tvShows[2].massOutbreak.species = SPECIES_PIKACHU;
    record.tvShows[2].massOutbreak.level = 5;
    record.tvShows[2].massOutbreak.language = LANGUAGE_ENGLISH;
    SetTvShowTrainerIds(&record.tvShows[2]);

    EXPECT_EQ(ReceiveRecordPayload(&record, INTERNET_RECORD_MIX_BLOCK_TV_SHOWS), INTERNET_RECORD_MIX_RECEIVED);

    normalShow = FindTvShowByKind(TVSHOW_RECENT_HAPPENINGS);
    recordMixShow = FindTvShowByKind(TVSHOW_TREND_WATCHER);
    outbreakShow = FindTvShowByKind(TVSHOW_MASS_OUTBREAK);
    EXPECT(normalShow != NULL);
    EXPECT(recordMixShow != NULL);
    EXPECT(outbreakShow != NULL);
    EXPECT(FindTvShowByKind(TVSHOW_SAFARI_FAN_CLUB) != NULL);
    if (normalShow != NULL)
    {
        EXPECT_EQ(normalShow->common.trainerIdLo, 0x33);
        EXPECT_EQ(normalShow->common.trainerIdHi, 0x44);
        EXPECT_EQ(normalShow->common.srcTrainerIdLo, 0xAA);
        EXPECT_EQ(normalShow->common.srcTrainerIdHi, 0xBB);
    }
    if (recordMixShow != NULL)
    {
        EXPECT_EQ(recordMixShow->common.srcTrainerIdLo, 0x55);
        EXPECT_EQ(recordMixShow->common.srcTrainerIdHi, 0x66);
        EXPECT_EQ(recordMixShow->common.srcTrainerId2Lo, 0xAA);
        EXPECT_EQ(recordMixShow->common.srcTrainerId2Hi, 0xBB);
    }
    if (outbreakShow != NULL)
    {
        EXPECT_EQ(outbreakShow->common.trainerIdLo, 0x33);
        EXPECT_EQ(outbreakShow->common.trainerIdHi, 0x44);
        EXPECT_EQ(outbreakShow->common.srcTrainerIdLo, 0xAA);
        EXPECT_EQ(outbreakShow->common.srcTrainerIdHi, 0xBB);
        EXPECT_EQ(outbreakShow->massOutbreak.daysLeft, 1);
    }
}

TEST("Internet daycare seed uses only selected original TV data")
{
    struct PlayerRecordEmerald record;
    u8 expectedSeed;

    memset(&record, 0, sizeof(record));
    InitValidTrendWatcher(&record.tvShows[0]);
    expectedSeed = CalculateDaycareSeed(record.tvShows);
    EXPECT_EQ(ReceiveRecordPayload(&record, INTERNET_RECORD_MIX_BLOCK_TV_SHOWS | INTERNET_RECORD_MIX_BLOCK_DAYCARE_MAIL),
              INTERNET_RECORD_MIX_RECEIVED);
    EXPECT_EQ(GetDaycareMailRandSum(), expectedSeed);

    memset(&record, 0, sizeof(record));
    record.tvShows[0].common.data[0] = 0x7F;
    EXPECT_EQ(ReceiveRecordPayload(&record, INTERNET_RECORD_MIX_BLOCK_DAYCARE_MAIL), INTERNET_RECORD_MIX_RECEIVED);
    EXPECT_EQ(GetDaycareMailRandSum(), 0);
}

TEST("Mobile Adapter reports an oversized HTTP response")
{
    MA_TELDATA telephone = {0};
    char userId[33] = {0};
    u8 response[5] = {0};
    u16 responseSize = 0;
    int result = maInitLibrary();

    if (result == MA_RESULT_OK)
        result = maGetConnectionData(&telephone, userId);
    if (result == MA_RESULT_OK)
        result = maConnectServer(&telephone, userId, "password1");
    if (result == MA_RESULT_OK)
        result = maDownload("http://127.0.0.1/Debug/Oversized", NULL, 0,
                            response, sizeof(response), &responseSize, "", "");

    EXPECT_EQ(result, MA_RESULT_BUFFER_FULL);
    EXPECT_EQ(responseSize, sizeof(response));
    maKill();
}

TEST("Internet Record Mix rejects unsafe fields in every native block")
{
    struct PlayerRecordEmerald *record = (void *)sUploadedRecordPacket;
    u16 originalTrendWord = gSaveBlock1Ptr->dewfordTrends[0].words[0];
    u32 i;

    gSaveBlock1Ptr->dewfordTrends[0].words[0] = 0x1234;

    memset(record, 0, sizeof(*record));
    record->secretBases[0].secretBaseId = 0xFF;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_SECRET_BASES), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    record->tvShows[0].common.kind = 0xFF;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_TV_SHOWS), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    record->pokeNews[0].kind = 0xFF;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_POKE_NEWS), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    record->oldMan.common.id = 0xFF;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_OLD_MAN), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    for (i = 0; i < SAVED_TRENDS_COUNT; i++)
    {
        record->dewfordTrends[i].words[0] = EC_EMPTY_WORD;
        record->dewfordTrends[i].words[1] = EC_EMPTY_WORD;
    }
    record->dewfordTrends[0].words[0] = 0xFFFE;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_DEWFORD), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    record->daycareMail.numDaycareMons = DAYCARE_MON_COUNT + 1;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_DAYCARE_MAIL), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    record->battleTowerRecord.winStreak = 1;
    record->battleTowerRecord.facilityClass = 0xFF;
    record->battleTowerRecord.name[0] = EOS;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_BATTLE_TOWER), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    record->giftItem = ITEMS_COUNT;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_GIFT_ITEM), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    record->lilycoveLady.id = 0xFF;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_LILYCOVE_LADY), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    record->apprentices[0].playerName[0] = CHAR_A;
    record->apprentices[0].id = 0x1F;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_APPRENTICES), INTERNET_RECORD_MIX_INVALID_PACKET);

    memset(record, 0, sizeof(*record));
    record->hallRecords.onePlayer[0][0].winStreak = 1;
    record->hallRecords.onePlayer[0][0].language = LANGUAGE_ENGLISH;
    for (i = 0; i < ARRAY_COUNT(record->hallRecords.onePlayer[0][0].name); i++)
        record->hallRecords.onePlayer[0][0].name[i] = CHAR_A;
    EXPECT_EQ(ReceiveRecordPayload(record, INTERNET_RECORD_MIX_BLOCK_HALL_RECORDS), INTERNET_RECORD_MIX_INVALID_PACKET);

    EXPECT_EQ(gSaveBlock1Ptr->dewfordTrends[0].words[0], 0x1234);
    gSaveBlock1Ptr->dewfordTrends[0].words[0] = originalTrendWord;
}
