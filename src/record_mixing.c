#include "global.h"
#include "malloc.h"
#include "random.h"
#include "constants/items.h"
#include "text.h"
#include "item.h"
#include "task.h"
#include "save.h"
#include "load_save.h"
#include "pokemon.h"
#include "cable_club.h"
#include "link.h"
#include "link_rfu.h"
#include "tv.h"
#include "battle_tower.h"
#include "window.h"
#include "secret_base.h"
#include "mauville_old_man.h"
#include "sound.h"
#include "constants/songs.h"
#include "menu.h"
#include "overworld.h"
#include "field_screen_effect.h"
#include "fldeff_misc.h"
#include "script.h"
#include "event_data.h"
#include "lilycove_lady.h"
#include "strings.h"
#include "string_util.h"
#include "record_mixing.h"
#include "new_game.h"
#include "daycare.h"
#include "international_string_util.h"
#include "constants/battle_frontier.h"
#include "constants/lilycove_lady.h"
#include "constants/mauville_old_man.h"
#include "dewford_trend.h"
#include "easy_chat.h"
#include "util.h"
#include "constants/apprentice.h"
#include "constants/decorations.h"
#include "constants/game_stat.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/secret_bases.h"
#include "constants/species.h"
#include "constants/trainers.h"

// Number of bytes of the record transferred at a time
#define BUFFER_CHUNK_SIZE 200

#define NUM_SWAP_COMBOS 3

// Used by several tasks in this file
#define tState        data[0]

struct RecordMixingHallRecords
{
    struct RankingHall1P hallRecords1P[HALL_FACILITIES_COUNT][FRONTIER_LVL_MODE_COUNT][HALL_RECORDS_COUNT * 2];
    struct RankingHall2P hallRecords2P[FRONTIER_LVL_MODE_COUNT][HALL_RECORDS_COUNT * 2];
};

struct PlayerRecordRS
{
    struct SecretBase secretBases[SECRET_BASES_COUNT];
    TVShow tvShows[TV_SHOWS_COUNT];
    PokeNews pokeNews[POKE_NEWS_COUNT];
    OldMan oldMan;
    struct DewfordTrend dewfordTrends[SAVED_TRENDS_COUNT];
    struct RecordMixingDaycareMail daycareMail;
    struct RSBattleTowerRecord battleTowerRecord;
    u16 giftItem;
    u16 filler[50];
};

union PlayerRecord
{
    struct PlayerRecordRS ruby;
    struct PlayerRecordEmerald emerald;
};

static bool8 sReadyToReceive;
static struct SecretBase *sSecretBasesSave;
static TVShow *sTvShowsSave;
static PokeNews *sPokeNewsSave;
static OldMan *sOldManSave;
static struct DewfordTrend *sDewfordTrendsSave;
static struct RecordMixingDaycareMail *sRecordMixMailSave;
static void *sBattleTowerSave;
static LilycoveLady *sLilycoveLadySave;
static void *sApprenticesSave;
static void *sBattleTowerSave_Duplicate;
static u32 sRecordStructSize;
static u8 sDaycareMailRandSum;
#if FREE_RECORD_MIXING_HALL_RECORDS == FALSE
static struct PlayerHallRecords *sPartnerHallRecords[HALL_RECORDS_COUNT];
#endif //FREE_RECORD_MIXING_HALL_RECORDS

static EWRAM_DATA struct RecordMixingDaycareMail sRecordMixMail = {0};
static EWRAM_DATA union PlayerRecord *sReceivedRecords = NULL;
static EWRAM_DATA union PlayerRecord *sSentRecord = NULL;

static int CalcRecordMixingGiftChecksum(void)
{
    u32 i;
    int sum = 0;
    const u8 *data = (const u8 *)&gSaveBlock1Ptr->recordMixingGift.data;

    for (i = 0; i < sizeof(gSaveBlock1Ptr->recordMixingGift.data); i++)
        sum += data[i];
    return sum;
}

static void ClearRecordMixingGift(void)
{
    CpuFill16(0, &gSaveBlock1Ptr->recordMixingGift, sizeof(gSaveBlock1Ptr->recordMixingGift));
}

static bool32 IsRecordMixingGiftValid(void)
{
    const struct RecordMixingGiftData *data = &gSaveBlock1Ptr->recordMixingGift.data;
    int checksum = CalcRecordMixingGiftChecksum();

    return data->unk0 != 0
        && data->quantity != 0
        && data->itemId != ITEM_NONE
        && checksum != 0
        && checksum == gSaveBlock1Ptr->recordMixingGift.checksum;
}

u16 GetRecordMixingGift(void)
{
    struct RecordMixingGiftData *data = &gSaveBlock1Ptr->recordMixingGift.data;
    u16 itemId;

    if (!IsRecordMixingGiftValid())
    {
        ClearRecordMixingGift();
        return ITEM_NONE;
    }

    itemId = data->itemId;
    if (--data->quantity == 0)
        ClearRecordMixingGift();
    else
        gSaveBlock1Ptr->recordMixingGift.checksum = CalcRecordMixingGiftChecksum();
    return itemId;
}

static void Task_RecordMixing_Main(u8);
static void Task_MixingRecordsRecv(u8);
static void Task_SendPacket(u8);
static void Task_CopyReceiveBuffer(u8);
static void Task_SendPacket_SwitchToReceive(u8);
static void *LoadPtrFromTaskData(const u16 *);
static void StorePtrInTaskData(void *, u16 *);
static u8 GetMultiplayerId_(void);
static void *GetPlayerRecvBuffer(u8);
static void ReceiveOldManData(OldMan *, size_t, u8);
static void ReceiveBattleTowerData(void *, size_t, u8);
static void ReceiveLilycoveLadyData(LilycoveLady *, size_t, u8);
static void CalculateDaycareMailRandSum(const u8 *);
static void ReceiveDaycareMailData(struct RecordMixingDaycareMail *, size_t, u8);
static void ReceiveGiftItem(u16 *, u8 );
static void Task_DoRecordMixing(u8);
static void GetSavedApprentices(struct Apprentice *, struct Apprentice *);
static void ReceiveApprenticeData(struct Apprentice *, size_t, u32);
static void ReceiveRankingHallRecords(struct PlayerHallRecords *, size_t, u32);
static void GetRecordMixingDaycareMail(struct RecordMixingDaycareMail *);
static void SanitizeDaycareMailForRuby(struct RecordMixingDaycareMail *);
static void SanitizeEmeraldBattleTowerRecord(struct EmeraldBattleTowerRecord *);
static void SanitizeRubyBattleTowerRecord(struct RSBattleTowerRecord *);

#define INTERNET_RECORD_MIX_VERSION 1

enum
{
    INTERNET_RECORD_MIX_OFFSET_VERSION = 4,
    INTERNET_RECORD_MIX_OFFSET_GAME_VERSION,
    INTERNET_RECORD_MIX_OFFSET_LANGUAGE,
    INTERNET_RECORD_MIX_OFFSET_RESERVED,
    INTERNET_RECORD_MIX_OFFSET_TRAINER_ID,
    INTERNET_RECORD_MIX_OFFSET_PLAYER_NAME = 12,
    INTERNET_RECORD_MIX_OFFSET_PAYLOAD_SIZE = 22,
    INTERNET_RECORD_MIX_OFFSET_PAYLOAD_CRC = 24,
    INTERNET_RECORD_MIX_OFFSET_RESERVED_2 = 26,
};

static const u8 sInternetRecordMixMagic[] = {'P', 'M', 'R', 'M'};

static const u8 sPlayerIdxOrders_2Player[] = {1, 0};

static const u8 sPlayerIdxOrders_3Player[][3] =
{
    {1, 2, 0},
    {2, 0, 1},
};

static const u8 sPlayerIdxOrders_4Player[][4] =
{
    {1, 0, 3, 2},
    {3, 0, 1, 2},
    {2, 0, 3, 1},
    {1, 3, 0, 2},
    {2, 3, 0, 1},
    {3, 2, 0, 1},
    {1, 2, 3, 0},
    {2, 3, 1, 0},
    {3, 2, 1, 0},
};

// When 3 players can swap mail 2 players are randomly selected and the 3rd is left out
static const u8 sDaycareMailSwapIds_3Player[NUM_SWAP_COMBOS][2] =
{
    {0, 1},
    {1, 2},
    {2, 0},
};

static const u8 sDaycareMailSwapIds_4Player[NUM_SWAP_COMBOS][4] =
{
    {0, 1,   2, 3}, // 0 swaps with 1, 2 swaps with 3
    {0, 2,   1, 3},
    {0, 3,   2, 1},
};

void RecordMixingPlayerSpotTriggered(void)
{
    CreateTask_EnterCableClubSeat(Task_RecordMixing_Main);
}

// these variables were const in R/S, but had to become changeable because of saveblocks changing RAM position
static void SetSrcLookupPointers(void)
{
    sSecretBasesSave = gSaveBlock1Ptr->secretBases;
    sTvShowsSave = gSaveBlock1Ptr->tvShows;
    sPokeNewsSave = gSaveBlock1Ptr->pokeNews;
    sOldManSave = &gSaveBlock1Ptr->oldMan;
    sDewfordTrendsSave = gSaveBlock1Ptr->dewfordTrends;
    sRecordMixMailSave = &sRecordMixMail;
    sBattleTowerSave = &gSaveBlock2Ptr->frontier.towerPlayer;
    sLilycoveLadySave = &gSaveBlock1Ptr->lilycoveLady;
    sApprenticesSave = gSaveBlock2Ptr->apprentices;
    sBattleTowerSave_Duplicate = &gSaveBlock2Ptr->frontier.towerPlayer;
}

static void PrepareUnknownExchangePacket(struct PlayerRecordRS *dest)
{
    memcpy(dest->secretBases, sSecretBasesSave, sizeof(dest->secretBases));
    memcpy(dest->tvShows, sTvShowsSave, sizeof(dest->tvShows));
    SanitizeTVShowLocationsForRuby(dest->tvShows);
    memcpy(dest->pokeNews, sPokeNewsSave, sizeof(dest->pokeNews));
    memcpy(&dest->oldMan, sOldManSave, sizeof(dest->oldMan));
    memcpy(dest->dewfordTrends, sDewfordTrendsSave, sizeof(dest->dewfordTrends));
    GetRecordMixingDaycareMail(&dest->daycareMail);
    EmeraldBattleTowerRecordToRuby(sBattleTowerSave, &dest->battleTowerRecord);

    if (GetMultiplayerId() == 0)
        dest->giftItem = GetRecordMixingGift();
}

static void PrepareExchangePacketForRubySapphire(struct PlayerRecordRS *dest)
{
    memcpy(dest->secretBases, sSecretBasesSave, sizeof(dest->secretBases));
    ClearJapaneseSecretBases(dest->secretBases);
    memcpy(dest->tvShows, sTvShowsSave, sizeof(dest->tvShows));
    SanitizeTVShowsForRuby(dest->tvShows);
    memcpy(dest->pokeNews, sPokeNewsSave, sizeof(dest->pokeNews));
    memcpy(&dest->oldMan, sOldManSave, sizeof(dest->oldMan));
    SanitizeMauvilleOldManForRuby(&dest->oldMan);
    memcpy(dest->dewfordTrends, sDewfordTrendsSave, sizeof(dest->dewfordTrends));
    GetRecordMixingDaycareMail(&dest->daycareMail);
    SanitizeDaycareMailForRuby(&dest->daycareMail);
    EmeraldBattleTowerRecordToRuby(sBattleTowerSave, &dest->battleTowerRecord);
    SanitizeRubyBattleTowerRecord(&dest->battleTowerRecord);

    if (GetMultiplayerId() == 0)
        dest->giftItem = GetRecordMixingGift();
}

static void PrepareEmeraldExchangePacket(struct PlayerRecordEmerald *dest, bool32 includeGift)
{
    memcpy(dest->secretBases, sSecretBasesSave, sizeof(dest->secretBases));
    memcpy(dest->tvShows, sTvShowsSave, sizeof(dest->tvShows));
    memcpy(dest->pokeNews, sPokeNewsSave, sizeof(dest->pokeNews));
    memcpy(&dest->oldMan, sOldManSave, sizeof(dest->oldMan));
    memcpy(&dest->lilycoveLady, sLilycoveLadySave, sizeof(dest->lilycoveLady));
    memcpy(dest->dewfordTrends, sDewfordTrendsSave, sizeof(dest->dewfordTrends));
    GetRecordMixingDaycareMail(&dest->daycareMail);
    memcpy(&dest->battleTowerRecord, sBattleTowerSave, sizeof(dest->battleTowerRecord));
    SanitizeEmeraldBattleTowerRecord(&dest->battleTowerRecord);

    if (includeGift)
        dest->giftItem = GetRecordMixingGift();

    GetSavedApprentices(dest->apprentices, sApprenticesSave);
    GetPlayerHallRecords(&dest->hallRecords);
}

static void PrepareExchangePacket(void)
{
    SetPlayerSecretBaseParty();
    DeactivateAllNormalTVShows();
    SetSrcLookupPointers();

    if (Link_AnyPartnersPlayingRubyOrSapphire())
    {
        if (LinkDummy_Return2() == 0)
            PrepareUnknownExchangePacket(&sSentRecord->ruby);
        else
            PrepareExchangePacketForRubySapphire(&sSentRecord->ruby);
    }
    else
    {
        PrepareEmeraldExchangePacket(&sSentRecord->emerald, GetMultiplayerId() == 0);
    }
}

static void ReceiveExchangePacket(u32 multiplayerId)
{
    if (Link_AnyPartnersPlayingRubyOrSapphire())
    {
        // Ruby/Sapphire
        CalculateDaycareMailRandSum((void *)sReceivedRecords->ruby.tvShows);
        ReceiveSecretBasesData(sReceivedRecords->ruby.secretBases, sizeof(sReceivedRecords->ruby), multiplayerId);
        ReceiveDaycareMailData(&sReceivedRecords->ruby.daycareMail, sizeof(sReceivedRecords->ruby), multiplayerId);
        ReceiveBattleTowerData(&sReceivedRecords->ruby.battleTowerRecord, sizeof(sReceivedRecords->ruby), multiplayerId);
        ReceiveTvShowsData(sReceivedRecords->ruby.tvShows, sizeof(sReceivedRecords->ruby), multiplayerId);
        ReceivePokeNewsData(sReceivedRecords->ruby.pokeNews, sizeof(sReceivedRecords->ruby), multiplayerId);
        ReceiveOldManData(&sReceivedRecords->ruby.oldMan, sizeof(sReceivedRecords->ruby), multiplayerId);
        ReceiveDewfordTrendData(sReceivedRecords->ruby.dewfordTrends, sizeof(sReceivedRecords->ruby), multiplayerId);
        ReceiveGiftItem(&sReceivedRecords->ruby.giftItem, multiplayerId);
    }
    else
    {
        // Emerald
        CalculateDaycareMailRandSum((void *)sReceivedRecords->emerald.tvShows);
        ReceiveSecretBasesData(sReceivedRecords->emerald.secretBases, sizeof(sReceivedRecords->emerald), multiplayerId);
        ReceiveTvShowsData(sReceivedRecords->emerald.tvShows, sizeof(sReceivedRecords->emerald), multiplayerId);
        ReceivePokeNewsData(sReceivedRecords->emerald.pokeNews, sizeof(sReceivedRecords->emerald), multiplayerId);
        ReceiveOldManData(&sReceivedRecords->emerald.oldMan, sizeof(sReceivedRecords->emerald), multiplayerId);
        ReceiveDewfordTrendData(sReceivedRecords->emerald.dewfordTrends, sizeof(sReceivedRecords->emerald), multiplayerId);
        ReceiveDaycareMailData(&sReceivedRecords->emerald.daycareMail, sizeof(sReceivedRecords->emerald), multiplayerId);
        ReceiveBattleTowerData(&sReceivedRecords->emerald.battleTowerRecord, sizeof(sReceivedRecords->emerald), multiplayerId);
        ReceiveGiftItem(&sReceivedRecords->emerald.giftItem, multiplayerId);
        ReceiveLilycoveLadyData(&sReceivedRecords->emerald.lilycoveLady, sizeof(sReceivedRecords->emerald), multiplayerId);
        ReceiveApprenticeData(sReceivedRecords->emerald.apprentices, sizeof(sReceivedRecords->emerald), (u8)multiplayerId);
        ReceiveRankingHallRecords(&sReceivedRecords->emerald.hallRecords, sizeof(sReceivedRecords->emerald), (u8)multiplayerId);
    }
}

static u16 ReadInternetRecordU16(const u8 *data)
{
    return data[0] | (data[1] << 8);
}

static u32 ReadInternetRecordU32(const u8 *data)
{
    return (u32)data[0]
         | (u32)data[1] << 8
         | (u32)data[2] << 16
         | (u32)data[3] << 24;
}

static void WriteInternetRecordU16(u8 *data, u16 value)
{
    data[0] = value;
    data[1] = value >> 8;
}

static void WriteInternetRecordU32(u8 *data, u32 value)
{
    data[0] = value;
    data[1] = value >> 8;
    data[2] = value >> 16;
    data[3] = value >> 24;
}

u16 BuildInternetRecordMixPacket(u8 *packet, u16 capacity)
{
    struct PlayerRecordEmerald *record;

    if (packet == NULL || capacity < INTERNET_RECORD_MIX_MAX_PACKET_SIZE)
        return 0;

    memset(packet, 0, INTERNET_RECORD_MIX_MAX_PACKET_SIZE);
    record = (struct PlayerRecordEmerald *)&packet[INTERNET_RECORD_MIX_HEADER_SIZE];

    memcpy(packet, sInternetRecordMixMagic, sizeof(sInternetRecordMixMagic));
    packet[INTERNET_RECORD_MIX_OFFSET_VERSION] = INTERNET_RECORD_MIX_VERSION;
    packet[INTERNET_RECORD_MIX_OFFSET_GAME_VERSION] = VERSION_EMERALD;
    packet[INTERNET_RECORD_MIX_OFFSET_LANGUAGE] = GAME_LANGUAGE;
    WriteInternetRecordU32(&packet[INTERNET_RECORD_MIX_OFFSET_TRAINER_ID], GetTrainerId(gSaveBlock2Ptr->playerTrainerId));
    memcpy(&packet[INTERNET_RECORD_MIX_OFFSET_PLAYER_NAME], gSaveBlock2Ptr->playerName, PLAYER_NAME_LENGTH + 1);

    SetPlayerSecretBaseParty();
    SetSrcLookupPointers();
    PrepareEmeraldExchangePacket(record, TRUE);
    DeactivateNormalTVShows(record->tvShows);

    WriteInternetRecordU16(&packet[INTERNET_RECORD_MIX_BLOCK_MASK_OFFSET], INTERNET_RECORD_MIX_ALL_BLOCKS);
    WriteInternetRecordU16(&packet[INTERNET_RECORD_MIX_OFFSET_PAYLOAD_SIZE], sizeof(*record));
    WriteInternetRecordU16(&packet[INTERNET_RECORD_MIX_OFFSET_PAYLOAD_CRC], CalcCRC16WithTable((u8 *)record, sizeof(*record)));
    return INTERNET_RECORD_MIX_MAX_PACKET_SIZE;
}

bool32 DecodeInternetRecordMixCode(const u8 *response, u16 responseSize, u8 *code)
{
    u32 i;

    if (response == NULL || code == NULL || responseSize == 0 || responseSize > INTERNET_RECORD_CODE_LENGTH)
        return FALSE;

    for (i = 0; i < responseSize; i++)
    {
        if (!((response[i] >= 'A' && response[i] <= 'Z')
           || (response[i] >= 'a' && response[i] <= 'z')
           || (response[i] >= '0' && response[i] <= '9')))
            return FALSE;
    }

    memcpy(code, response, responseSize);
    code[responseSize] = 0;
    ASCIIToPkmnStr(code, code);
    return TRUE;
}

static bool32 IsValidInternetSecretBaseId(u8 secretBaseId)
{
    u8 group;
    u8 position;

    if (secretBaseId == 0)
        return TRUE;

    group = secretBaseId / 10;
    position = secretBaseId % 10;
    if (group >= NUM_SECRET_BASE_GROUPS || position == 0 || position > 4)
        return FALSE;

    return position <= 3 || group == 16 || group == 17 || group == 20;
}

static bool32 ValidateInternetSecretBases(const struct SecretBase *secretBases)
{
    u32 baseId;
    u32 decorationId;
    u32 partyId;
    u32 moveId;

    for (baseId = 0; baseId < SECRET_BASES_COUNT; baseId++)
    {
        const struct SecretBase *base = &secretBases[baseId];

        if (!IsValidInternetSecretBaseId(base->secretBaseId))
            return FALSE;
        if (base->secretBaseId == 0)
            continue;
        if (!IsValidGameLanguage(base->language) || base->toRegister > TRUE)
            return FALSE;

        for (decorationId = 0; decorationId < DECOR_MAX_SECRET_BASE; decorationId++)
        {
            if (base->decorations[decorationId] > NUM_DECORATIONS)
                return FALSE;
        }

        for (partyId = 0; partyId < PARTY_SIZE; partyId++)
        {
            if (base->party.species[partyId] >= NUM_SPECIES
             || base->party.heldItems[partyId] >= ITEMS_COUNT
             || base->party.levels[partyId] > MAX_LEVEL)
                return FALSE;
            if ((base->party.species[partyId] == SPECIES_NONE) != (base->party.levels[partyId] == 0))
                return FALSE;

            for (moveId = 0; moveId < MAX_MON_MOVES; moveId++)
            {
                if (base->party.moves[partyId * MAX_MON_MOVES + moveId] >= MOVES_COUNT)
                    return FALSE;
            }
        }
    }

    return TRUE;
}

static bool32 ValidateInternetPokeNews(const PokeNews *pokeNews)
{
    u32 i;

    for (i = 0; i < POKE_NEWS_COUNT; i++)
    {
        if (pokeNews[i].kind > POKENEWS_BLENDMASTER
         || pokeNews[i].state > POKENEWS_STATE_ACTIVE)
            return FALSE;
    }
    return TRUE;
}

static bool32 ValidateInternetOldMan(const OldMan *oldMan)
{
    u32 i;

    switch (oldMan->common.id)
    {
    case MAUVILLE_MAN_BARD:
        return AreEasyChatWordsValid(oldMan->bard.songLyrics, ARRAY_COUNT(oldMan->bard.songLyrics))
            && AreEasyChatWordsValid(oldMan->bard.temporaryLyrics, ARRAY_COUNT(oldMan->bard.temporaryLyrics))
            && IsStringTerminated(oldMan->bard.playerName, ARRAY_COUNT(oldMan->bard.playerName))
            && IsValidGameLanguage(oldMan->bard.language);
    case MAUVILLE_MAN_HIPSTER:
        return oldMan->hipster.taughtWord <= TRUE
            && IsValidGameLanguage(oldMan->hipster.language);
    case MAUVILLE_MAN_TRADER:
        if (oldMan->trader.alreadyTraded > TRUE)
            return FALSE;
        for (i = 0; i < NUM_TRADER_ITEMS; i++)
        {
            if (oldMan->trader.decorations[i] > NUM_DECORATIONS
             || !IsStringTerminated(oldMan->trader.playerNames[i], ARRAY_COUNT(oldMan->trader.playerNames[i]))
             || !IsValidGameLanguage(oldMan->trader.language[i]))
                return FALSE;
        }
        return TRUE;
    case MAUVILLE_MAN_STORYTELLER:
        if (oldMan->storyteller.alreadyRecorded > TRUE)
            return FALSE;
        for (i = 0; i < NUM_STORYTELLER_TALES; i++)
        {
            if (oldMan->storyteller.gameStatIDs[i] >= NUM_GAME_STATS
             || !IsValidGameLanguage(oldMan->storyteller.language[i]))
                return FALSE;
        }
        return TRUE;
    case MAUVILLE_MAN_GIDDY:
        if (oldMan->giddy.taleCounter > GIDDY_MAX_TALES
         || oldMan->giddy.questionNum > GIDDY_MAX_QUESTIONS
         || !AreEasyChatWordsValid(oldMan->giddy.randomWords, ARRAY_COUNT(oldMan->giddy.randomWords))
         || !IsValidGameLanguage(oldMan->giddy.language))
            return FALSE;
        for (i = 0; i < ARRAY_COUNT(oldMan->giddy.questionList); i++)
        {
            if (oldMan->giddy.questionList[i] >= GIDDY_MAX_QUESTIONS)
                return FALSE;
        }
        return TRUE;
    default:
        return FALSE;
    }
}

static bool32 ValidateInternetDewfordTrends(const struct DewfordTrend *trends)
{
    u32 i;

    for (i = 0; i < SAVED_TRENDS_COUNT; i++)
    {
        if (!AreEasyChatWordsValid(trends[i].words, ARRAY_COUNT(trends[i].words)))
            return FALSE;
    }
    return TRUE;
}

static bool32 ValidateInternetDaycareMail(const struct RecordMixingDaycareMail *daycareMail)
{
    u32 i;
    if (daycareMail->numDaycareMons > DAYCARE_MON_COUNT)
        return FALSE;
    for (i = 0; i < daycareMail->numDaycareMons; i++)
    {
        const struct DaycareMail *mail = &daycareMail->mail[i];

        if (daycareMail->cantHoldItem[i] > TRUE
         || mail->message.itemId >= ITEMS_COUNT
         || mail->message.species >= NUM_SPECIES
         || !IsStringTerminated(mail->message.playerName, ARRAY_COUNT(mail->message.playerName))
         || !IsStringTerminated(mail->otName, ARRAY_COUNT(mail->otName))
         || !IsStringTerminated(mail->monName, ARRAY_COUNT(mail->monName))
         || !IsValidGameLanguage(mail->gameLanguage)
         || !IsValidGameLanguage(mail->monLanguage)
         || !AreEasyChatWordsValid(mail->message.words, ARRAY_COUNT(mail->message.words)))
            return FALSE;
    }
    return TRUE;
}

static bool32 ValidateInternetBattleTowerRecord(const struct EmeraldBattleTowerRecord *towerRecord)
{
    u32 i;
    u32 move;

    if (towerRecord->winStreak == 0)
        return TRUE;
    if (towerRecord->lvlMode >= FRONTIER_LVL_MODE_COUNT
     || towerRecord->facilityClass >= FACILITY_CLASSES_COUNT
     || !IsStringTerminated(towerRecord->name, ARRAY_COUNT(towerRecord->name))
     || !IsValidGameLanguage(towerRecord->language)
     || !AreEasyChatWordsValid(towerRecord->greeting, ARRAY_COUNT(towerRecord->greeting))
     || !AreEasyChatWordsValid(towerRecord->speechWon, ARRAY_COUNT(towerRecord->speechWon))
     || !AreEasyChatWordsValid(towerRecord->speechLost, ARRAY_COUNT(towerRecord->speechLost)))
        return FALSE;

    for (i = 0; i < ARRAY_COUNT(towerRecord->party); i++)
    {
        const struct BattleTowerPokemon *mon = &towerRecord->party[i];

        if (mon->species == SPECIES_NONE)
            continue;
        if (mon->species >= NUM_SPECIES || mon->heldItem >= ITEMS_COUNT
         || mon->level == 0 || mon->level > MAX_LEVEL
         || !IsStringTerminated(mon->nickname, ARRAY_COUNT(mon->nickname)))
            return FALSE;
        for (move = 0; move < ARRAY_COUNT(mon->moves); move++)
        {
            if (mon->moves[move] >= MOVES_COUNT)
                return FALSE;
        }
    }
    return TRUE;
}

static u32 CalculateInternetApprenticeChecksum(const struct Apprentice *apprentice)
{
    const u32 *data = (const u32 *)apprentice;
    u32 checksum = 0;
    u32 i;

    for (i = 0; i < offsetof(struct Apprentice, checksum) / sizeof(u32); i++)
        checksum += data[i];
    return checksum;
}

static bool32 ValidateInternetApprentices(const struct Apprentice *apprentices)
{
    u32 i;
    u32 mon;
    u32 move;

    for (i = 0; i < 2; i++)
    {
        const struct Apprentice *apprentice = &apprentices[i];

        if (apprentice->playerName[0] == EOS)
            continue;
        if (apprentice->id >= NUM_APPRENTICES
         || apprentice->lvlMode == 0 || apprentice->lvlMode > FRONTIER_LVL_MODE_COUNT
         || apprentice->numQuestions > APPRENTICE_MAX_QUESTIONS
         || !IsValidGameLanguage(apprentice->language)
         || !AreEasyChatWordsValid(apprentice->speechWon, ARRAY_COUNT(apprentice->speechWon))
         || apprentice->checksum != CalculateInternetApprenticeChecksum(apprentice))
            return FALSE;
        for (mon = 0; mon < ARRAY_COUNT(apprentice->party); mon++)
        {
            if (apprentice->party[mon].species == SPECIES_NONE
             || apprentice->party[mon].species >= NUM_SPECIES
             || apprentice->party[mon].item >= ITEMS_COUNT)
                return FALSE;
            for (move = 0; move < ARRAY_COUNT(apprentice->party[mon].moves); move++)
            {
                if (apprentice->party[mon].moves[move] >= MOVES_COUNT)
                    return FALSE;
            }
        }
    }
    return TRUE;
}

static bool32 ValidateInternetHallRecords(const struct PlayerHallRecords *hallRecords)
{
    u32 facility;
    u32 mode;

    for (facility = 0; facility < HALL_FACILITIES_COUNT; facility++)
    {
        for (mode = 0; mode < FRONTIER_LVL_MODE_COUNT; mode++)
        {
            const struct RankingHall1P *record = &hallRecords->onePlayer[facility][mode];

            if (record->winStreak != 0
             && (!IsStringTerminated(record->name, ARRAY_COUNT(record->name))
              || !IsValidGameLanguage(record->language)))
                return FALSE;
        }
    }
    for (mode = 0; mode < FRONTIER_LVL_MODE_COUNT; mode++)
    {
        const struct RankingHall2P *record = &hallRecords->twoPlayers[mode];

        if (record->winStreak != 0
         && (!IsStringTerminated(record->name1, ARRAY_COUNT(record->name1))
          || !IsStringTerminated(record->name2, ARRAY_COUNT(record->name2))
          || !IsValidGameLanguage(record->language)))
            return FALSE;
    }
    return TRUE;
}

static bool32 ValidateInternetRecordMixPayload(const struct PlayerRecordEmerald *record, u16 blockMask)
{
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_SECRET_BASES)
     && !ValidateInternetSecretBases(record->secretBases))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_TV_SHOWS)
     && !ValidateInternetTvShows(record->tvShows))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_POKE_NEWS)
     && !ValidateInternetPokeNews(record->pokeNews))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_OLD_MAN)
     && !ValidateInternetOldMan(&record->oldMan))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_DEWFORD)
     && !ValidateInternetDewfordTrends(record->dewfordTrends))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_DAYCARE_MAIL)
     && !ValidateInternetDaycareMail(&record->daycareMail))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_BATTLE_TOWER)
     && !ValidateInternetBattleTowerRecord(&record->battleTowerRecord))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_GIFT_ITEM)
     && record->giftItem != ITEM_NONE
     && (record->giftItem >= ITEMS_COUNT || GetPocketByItemId(record->giftItem) != POCKET_KEY_ITEMS))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_LILYCOVE_LADY)
     && !ValidateInternetLilycoveLady(&record->lilycoveLady))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_APPRENTICES)
     && !ValidateInternetApprentices(record->apprentices))
        return FALSE;
    if ((blockMask & INTERNET_RECORD_MIX_BLOCK_HALL_RECORDS)
     && !ValidateInternetHallRecords(&record->hallRecords))
        return FALSE;

    return TRUE;
}

static void PrepareLocalInternetRecord(struct PlayerRecordEmerald *record, u16 blockMask)
{
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_DEWFORD)
        memcpy(record->dewfordTrends, gSaveBlock1Ptr->dewfordTrends, sizeof(record->dewfordTrends));
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_DAYCARE_MAIL)
        GetRecordMixingDaycareMail(&record->daycareMail);
}

static void SetInternetRecordLinkContext(const u8 *packet)
{
    memset(gLinkPlayers, 0, sizeof(gLinkPlayers));

    gWirelessCommType = FALSE;
    gLinkStatus = 2 << LINK_STAT_PLAYER_COUNT_SHIFT;

    gLinkPlayers[0].version = packet[INTERNET_RECORD_MIX_OFFSET_GAME_VERSION];
    gLinkPlayers[0].trainerId = ReadInternetRecordU32(&packet[INTERNET_RECORD_MIX_OFFSET_TRAINER_ID]);
    memcpy(gLinkPlayers[0].name, &packet[INTERNET_RECORD_MIX_OFFSET_PLAYER_NAME], sizeof(gLinkPlayers[0].name));
    gLinkPlayers[0].language = packet[INTERNET_RECORD_MIX_OFFSET_LANGUAGE];

    gLinkPlayers[1].version = VERSION_EMERALD;
    gLinkPlayers[1].trainerId = GetTrainerId(gSaveBlock2Ptr->playerTrainerId);
    memcpy(gLinkPlayers[1].name, gSaveBlock2Ptr->playerName, sizeof(gLinkPlayers[1].name));
    gLinkPlayers[1].language = GAME_LANGUAGE;
}

enum InternetRecordMixResult ReceiveInternetRecordMix(const u8 *packet, u16 packetSize, u8 *sourceName)
{
    struct PlayerRecordEmerald *records;
    struct LinkPlayer savedLinkPlayers[MAX_RFU_PLAYERS];
    const u8 *payload;
    u32 savedLinkStatus;
    u16 blockMask;
    u16 payloadSize;
    u16 payloadCrc;
    u8 savedWirelessCommType;

    if (packet == NULL || packetSize != INTERNET_RECORD_MIX_MAX_PACKET_SIZE)
        return INTERNET_RECORD_MIX_INVALID_PACKET;
    if (memcmp(packet, sInternetRecordMixMagic, sizeof(sInternetRecordMixMagic)) != 0)
        return INTERNET_RECORD_MIX_INVALID_PACKET;
    if (packet[INTERNET_RECORD_MIX_OFFSET_VERSION] != INTERNET_RECORD_MIX_VERSION
     || packet[INTERNET_RECORD_MIX_OFFSET_GAME_VERSION] != VERSION_EMERALD
     || !IsValidGameLanguage(packet[INTERNET_RECORD_MIX_OFFSET_LANGUAGE])
     || packet[INTERNET_RECORD_MIX_OFFSET_RESERVED] != 0
     || ReadInternetRecordU16(&packet[INTERNET_RECORD_MIX_OFFSET_RESERVED_2]) != 0
     || !IsStringTerminated(&packet[INTERNET_RECORD_MIX_OFFSET_PLAYER_NAME], PLAYER_NAME_LENGTH + 1))
        return INTERNET_RECORD_MIX_INVALID_PACKET;

    blockMask = ReadInternetRecordU16(&packet[INTERNET_RECORD_MIX_BLOCK_MASK_OFFSET]);
    payloadSize = ReadInternetRecordU16(&packet[INTERNET_RECORD_MIX_OFFSET_PAYLOAD_SIZE]);
    payloadCrc = ReadInternetRecordU16(&packet[INTERNET_RECORD_MIX_OFFSET_PAYLOAD_CRC]);
    payload = &packet[INTERNET_RECORD_MIX_HEADER_SIZE];
    if (blockMask == 0 || (blockMask & ~INTERNET_RECORD_MIX_ALL_BLOCKS) != 0
     || payloadSize != sizeof(struct PlayerRecordEmerald)
     || payloadCrc != CalcCRC16WithTable(payload, payloadSize))
        return INTERNET_RECORD_MIX_INVALID_PACKET;

    records = AllocZeroed(sizeof(*records) * MAX_LINK_PLAYERS);
    if (records == NULL)
        return INTERNET_RECORD_MIX_OUT_OF_MEMORY;
    memcpy(&records[0], payload, sizeof(records[0]));
    if (!ValidateInternetRecordMixPayload(&records[0], blockMask))
    {
        Free(records);
        return INTERNET_RECORD_MIX_INVALID_PACKET;
    }

    savedWirelessCommType = gWirelessCommType;
    savedLinkStatus = gLinkStatus;
    memcpy(savedLinkPlayers, gLinkPlayers, sizeof(savedLinkPlayers));
    SetInternetRecordLinkContext(packet);
    SetSrcLookupPointers();
    PrepareLocalInternetRecord(&records[1], blockMask);

    if (blockMask & INTERNET_RECORD_MIX_BLOCK_DAYCARE_MAIL)
    {
        if (blockMask & INTERNET_RECORD_MIX_BLOCK_TV_SHOWS)
            CalculateDaycareMailRandSum((void *)records[0].tvShows);
        else
            sDaycareMailRandSum = 0;
    }

    // TV mixing is deliberately one-way here. The original live-link processor
    // moves both players' pending shows; an Internet receive must not discard
    // local shows merely because there is no server-side player to receive them.
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_TV_SHOWS)
        ReceiveTvShowsDataFromInternet(records[0].tvShows);

    if (blockMask & INTERNET_RECORD_MIX_BLOCK_SECRET_BASES)
        ReceiveSecretBasesData(records[0].secretBases, sizeof(records[0]), 1);
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_POKE_NEWS)
        ReceivePokeNewsDataFromInternet(records[0].pokeNews);
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_OLD_MAN)
        ReceiveOldManData(&records[0].oldMan, sizeof(records[0]), 1);
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_DEWFORD)
        ReceiveDewfordTrendData(records[0].dewfordTrends, sizeof(records[0]), 1);
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_DAYCARE_MAIL)
        ReceiveDaycareMailData(&records[0].daycareMail, sizeof(records[0]), 1);
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_BATTLE_TOWER)
        ReceiveBattleTowerData(&records[0].battleTowerRecord, sizeof(records[0]), 1);
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_GIFT_ITEM)
        ReceiveGiftItem(&records[0].giftItem, 1);
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_LILYCOVE_LADY)
        ReceiveLilycoveLadyData(&records[0].lilycoveLady, sizeof(records[0]), 1);
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_APPRENTICES)
        ReceiveApprenticeData(records[0].apprentices, sizeof(records[0]), 1);
    if (blockMask & INTERNET_RECORD_MIX_BLOCK_HALL_RECORDS)
        ReceiveRankingHallRecords(&records[0].hallRecords, sizeof(records[0]), 1);

    if (sourceName != NULL)
        memcpy(sourceName, &packet[INTERNET_RECORD_MIX_OFFSET_PLAYER_NAME], PLAYER_NAME_LENGTH + 1);

    gWirelessCommType = savedWirelessCommType;
    gLinkStatus = savedLinkStatus;
    memcpy(gLinkPlayers, savedLinkPlayers, sizeof(savedLinkPlayers));
    Free(records);
    return INTERNET_RECORD_MIX_RECEIVED;
}

static void PrintTextOnRecordMixing(const u8 *src)
{
    DrawDialogueFrame(0, FALSE);
    AddTextPrinterParameterized(0, FONT_NORMAL, src, 0, 1, 0, NULL);
    CopyWindowToVram(0, COPYWIN_FULL);
}

#define tCounter data[0]

static void Task_RecordMixing_SoundEffect(u8 taskId)
{
    if (++gTasks[taskId].tCounter == 50)
    {
        PlaySE(SE_M_ATTRACT);
        gTasks[taskId].tCounter = 0;
    }
}

#undef tCounter

#define tTimer       data[8]
#define tLinkTaskId  data[10]
#define tSoundTaskId data[15]

// Note: gSpecialVar_0x8005 here contains the player's spot id.
static void Task_RecordMixing_Main(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0: // init
        sSentRecord = Alloc(sizeof(*sSentRecord));
        sReceivedRecords = Alloc(sizeof(*sReceivedRecords) * MAX_LINK_PLAYERS);
        SetLocalLinkPlayerId(gSpecialVar_0x8005);
        VarSet(VAR_TEMP_MIXED_RECORDS, 1);
        sReadyToReceive = FALSE;
        PrepareExchangePacket();
        CreateRecordMixingLights();
        tState = 1;
        tLinkTaskId = CreateTask(Task_MixingRecordsRecv, 80);
        tSoundTaskId = CreateTask(Task_RecordMixing_SoundEffect, 81);
        break;
    case 1: // wait for Task_MixingRecordsRecv
        if (!gTasks[tLinkTaskId].isActive)
        {
            tState = 2;
            FlagSet(FLAG_SYS_MIX_RECORD);
            DestroyRecordMixingLights();
            DestroyTask(tSoundTaskId);
        }
        break;
    case 2:
        tLinkTaskId = CreateTask(Task_DoRecordMixing, 10);
        tState = 3;
        PlaySE(SE_M_BATON_PASS);
        break;
    case 3: // wait for Task_DoRecordMixing
        if (!gTasks[tLinkTaskId].isActive)
        {
            tState = 4;
            if (gWirelessCommType == 0)
                tLinkTaskId = CreateTask_ReestablishCableClubLink();

            PrintTextOnRecordMixing(gText_RecordMixingComplete);
            tTimer = 0;
        }
        break;
    case 4: // wait 60 frames
        if (++tTimer > 60)
            tState = 5;
        break;
    case 5: // Wait for the task created by CreateTask_ReestablishCableClubLink
        if (!gTasks[tLinkTaskId].isActive)
        {
            Free(sReceivedRecords);
            Free(sSentRecord);
            SetLinkWaitingForScript();
            if (gWirelessCommType != 0)
                CreateTask(Task_ReturnToFieldRecordMixing, 10);
            ClearDialogWindowAndFrame(0, TRUE);
            DestroyTask(taskId);
            ScriptContext_Enable();
        }
        break;
    }
}

#undef tTimer
#undef tLinkTaskId
#undef tSoundTaskId

// Task data for Task_MixingRecordsRecv and subsequent tasks
#define tSentRecord    data[2] // Used to store a ptr, so data[2] and data[3]
#define tNumChunksSent data[4]
#define tMultiplayerId data[5]
#define tCopyTaskId    data[10]

// Task data for Task_CopyReceiveBuffer
#define tParentTaskId     data[0]
#define tNumChunksRecv(i) data[1 + (i)] // Number of chunks of the record received per player
#define tRecvRecords      data[5] // Used to store a ptr, so data[5] and data[6]

static void Task_MixingRecordsRecv(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
    case 0:
        PrintTextOnRecordMixing(gText_MixingRecords);
        task->data[8] = 0x708;
        task->tState = 400;
        ClearLinkCallback_2();
        break;
    case 100: // wait 20 frames
        if (++task->data[12] > 20)
        {
            task->data[12] = 0;
            task->tState = 101;
        }
        break;
    case 101:
        {
            u8 players = GetLinkPlayerCount_2();
            if (IsLinkMaster() == TRUE)
            {
                if (players == GetSavedPlayerCount())
                {
                    PlaySE(SE_PIN);
                    task->tState = 201;
                    task->data[12] = 0;
                }
            }
            else
            {
                PlaySE(SE_BOO);
                task->tState = 301;
            }
        }
        break;
    case 201:
        // We're the link master. Delay for 30 frames per connected player.
        if (GetSavedPlayerCount() == GetLinkPlayerCount_2() && ++task->data[12] > (GetLinkPlayerCount_2() * 30))
        {
            CheckShouldAdvanceLinkState();
            task->tState = 1;
        }
        break;
    case 301:
        if (GetSavedPlayerCount() == GetLinkPlayerCount_2())
            task->tState = 1;
        break;
    case 400: // wait 20 frames
        if (++task->data[12] > 20)
        {
            task->tState = 1;
            task->data[12] = 0;
        }
        break;
    case 1: // wait for handshake
        if (gReceivedRemoteLinkPlayers)
        {
            ConvertIntToDecimalStringN(gStringVar1, GetMultiplayerId_(), STR_CONV_MODE_LEADING_ZEROS, 2);
            task->tState = 5;
        }
        break;
    case 2:
        {
            u8 subTaskId;

            task->data[6] = GetLinkPlayerCount_2();
            task->tState = 0;
            task->tMultiplayerId = GetMultiplayerId_();
            task->func = Task_SendPacket;
            if (Link_AnyPartnersPlayingRubyOrSapphire())
            {
                StorePtrInTaskData(sSentRecord, (u16*) &task->tSentRecord);
                subTaskId = CreateTask(Task_CopyReceiveBuffer, 80);
                task->tCopyTaskId = subTaskId;
                gTasks[subTaskId].tParentTaskId = taskId;
                StorePtrInTaskData(sReceivedRecords, (u16*) &gTasks[subTaskId].tRecvRecords);
                sRecordStructSize = sizeof(struct PlayerRecordRS);
            }
            else
            {
                StorePtrInTaskData(sSentRecord, (u16*)  &task->tSentRecord);
                subTaskId = CreateTask(Task_CopyReceiveBuffer, 80);
                task->tCopyTaskId = subTaskId;
                gTasks[subTaskId].tParentTaskId = taskId;
                StorePtrInTaskData(sReceivedRecords,(u16*) &gTasks[subTaskId].tRecvRecords);
                sRecordStructSize = sizeof(struct PlayerRecordEmerald);
            }
        }
        break;
    case 5: // wait 60 frames
        if (++task->data[10] > 60)
        {
            task->data[10] = 0;
            task->tState = 2;
        }
        break;
    }
}

static void Task_SendPacket(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    switch (task->tState)
    {
    case 0: // Copy record data chunk to send buffer
        {
            void *recordData = LoadPtrFromTaskData((u16*)&task->tSentRecord) + task->tNumChunksSent * BUFFER_CHUNK_SIZE;

            memcpy(gBlockSendBuffer, recordData, BUFFER_CHUNK_SIZE);
            task->tState++;
        }
        break;
    case 1:
        if (GetMultiplayerId() == 0)
            SendBlockRequest(BLOCK_REQ_SIZE_200);
        task->tState++;
        break;
    case 2:
        break;
    case 3:
        // If sent final chunk of record, move on to next state.
        // Otherwise return to first state and send next chunk.
        task->tNumChunksSent++;
        if (task->tNumChunksSent == sRecordStructSize / BUFFER_CHUNK_SIZE + 1)
            task->tState++;
        else
            task->tState = 0;
        break;
    case 4:
        if (!gTasks[task->tCopyTaskId].isActive)
            task->func = Task_SendPacket_SwitchToReceive;
        break;
    }
}

static void Task_CopyReceiveBuffer(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 status = GetBlockReceivedStatus();
    u8 handledPlayers = 0;

    if (status == GetLinkPlayerCountAsBitFlags())
    {
        u8 i;
        for (i = 0; i < GetLinkPlayerCount(); i++)
        {
            if ((status >> i) & 1)
            {
                void *dest = LoadPtrFromTaskData((u16*) &task->tRecvRecords) + task->tNumChunksRecv(i) * BUFFER_CHUNK_SIZE + sRecordStructSize * i;
                void *src = GetPlayerRecvBuffer(i);
                if ((task->tNumChunksRecv(i) + 1) * BUFFER_CHUNK_SIZE > sRecordStructSize)
                    memcpy(dest, src, sRecordStructSize - task->tNumChunksRecv(i) * BUFFER_CHUNK_SIZE);
                else
                    memcpy(dest, src, BUFFER_CHUNK_SIZE);
                ResetBlockReceivedFlag(i);
                task->tNumChunksRecv(i)++;
                if (task->tNumChunksRecv(i) == sRecordStructSize / BUFFER_CHUNK_SIZE + 1)
                    handledPlayers++;
            }
        }
        gTasks[task->tParentTaskId].tState++;
    }

    if (handledPlayers == GetLinkPlayerCount())
        DestroyTask(taskId);
}

static void Task_WaitReceivePacket(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    // Wait for Task_CopyReceiveBuffer to finish
    if (!gTasks[task->tCopyTaskId].isActive)
        DestroyTask(taskId);
}

static void Task_ReceivePacket(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->func = Task_WaitReceivePacket;
    if (sReadyToReceive == TRUE)
        ReceiveExchangePacket(task->tMultiplayerId);
}

static void Task_SendPacket_SwitchToReceive(u8 taskId)
{
    gTasks[taskId].func = Task_ReceivePacket;
    sReadyToReceive = TRUE;
}

static void *LoadPtrFromTaskData(const u16 *asShort)
{
    return (void *)(asShort[0] | (asShort[1] << 16));
}

static void StorePtrInTaskData(void *records, u16 *asShort)
{
    asShort[0] = (u32)records;
    asShort[1] = ((u32)records >> 16);
}

static u8 GetMultiplayerId_(void)
{
    return GetMultiplayerId();
}

static void *GetPlayerRecvBuffer(u8 id)
{
    return gBlockRecvBuffer[id];
}

static void ShufflePlayerIndices(u32 *data)
{
    u32 i;
    u32 linkTrainerId;
    u32 players = GetLinkPlayerCount();

    switch (players)
    {
    case 2:
        for (i = 0; i < ARRAY_COUNT(sPlayerIdxOrders_2Player); i++)
            data[i] = sPlayerIdxOrders_2Player[i];
        break;
    case 3:
        linkTrainerId = GetLinkPlayerTrainerId(0) % ARRAY_COUNT(sPlayerIdxOrders_3Player);
        for (i = 0; i < ARRAY_COUNT(sPlayerIdxOrders_3Player[0]); i++)
            data[i] = sPlayerIdxOrders_3Player[linkTrainerId][i];
        break;
    case 4:
        linkTrainerId = GetLinkPlayerTrainerId(0) % ARRAY_COUNT(sPlayerIdxOrders_4Player);
        for (i = 0; i < ARRAY_COUNT(sPlayerIdxOrders_4Player[0]); i++)
            data[i] = sPlayerIdxOrders_4Player[linkTrainerId][i];
        break;
    }
}

static void ReceiveOldManData(OldMan *records, size_t recordSize, u8 multiplayerId)
{
    u8 version;
    u16 language;
    OldMan *oldMan;
    u32 mixIndices[MAX_LINK_PLAYERS];

    ShufflePlayerIndices(mixIndices);
    oldMan = (void *)records + recordSize * mixIndices[multiplayerId];
    version = gLinkPlayers[mixIndices[multiplayerId]].version;
    language = gLinkPlayers[mixIndices[multiplayerId]].language;

    if (Link_AnyPartnersPlayingRubyOrSapphire())
        SanitizeReceivedRubyOldMan(oldMan, version, language);
    else
        SanitizeReceivedEmeraldOldMan(oldMan, version, language);

    memcpy(sOldManSave, (void *)records + recordSize * mixIndices[multiplayerId], sizeof(OldMan));
    ResetMauvilleOldManFlag();
}

static void ReceiveBattleTowerData(void *records, size_t recordSize, u8 multiplayerId)
{
    struct EmeraldBattleTowerRecord *battleTowerRecord;
    struct BattleTowerPokemon *btPokemon;
    u32 mixIndices[MAX_LINK_PLAYERS];
    s32 i;

    ShufflePlayerIndices(mixIndices);
    if (Link_AnyPartnersPlayingRubyOrSapphire())
    {
        if (RubyBattleTowerRecordToEmerald((void *)records + recordSize * mixIndices[multiplayerId], (void *)records + recordSize * multiplayerId) == TRUE)
        {
            battleTowerRecord = (void *)records + recordSize * multiplayerId;
            battleTowerRecord->language = gLinkPlayers[mixIndices[multiplayerId]].language;
            CalcEmeraldBattleTowerChecksum(battleTowerRecord);
        }
    }
    else
    {
        memcpy((void *)records + recordSize * multiplayerId, (void *)records + recordSize * mixIndices[multiplayerId], sizeof(struct EmeraldBattleTowerRecord));
        battleTowerRecord = (void *)records + recordSize * multiplayerId;
        for (i = 0; i < MAX_FRONTIER_PARTY_SIZE; i++)
        {
            btPokemon = &battleTowerRecord->party[i];
            if (btPokemon->species != SPECIES_NONE && IsStringJapanese(btPokemon->nickname))
                ConvertInternationalString(btPokemon->nickname, LANGUAGE_JAPANESE);
        }
        CalcEmeraldBattleTowerChecksum(battleTowerRecord);
    }
    PutNewBattleTowerRecord((void *)records + recordSize * multiplayerId);
}

static void ReceiveLilycoveLadyData(LilycoveLady *records, size_t recordSize, u8 multiplayerId)
{
    LilycoveLady *lilycoveLady;
    u32 mixIndices[MAX_LINK_PLAYERS];

    ShufflePlayerIndices(mixIndices);
    memcpy((void *)records + recordSize * multiplayerId, sLilycoveLadySave, sizeof(LilycoveLady));

    if (GetLilycoveLadyId() == 0)
    {
        lilycoveLady = Alloc(sizeof(*lilycoveLady));
        if (lilycoveLady == NULL)
            return;

        memcpy(lilycoveLady, sLilycoveLadySave, sizeof(LilycoveLady));
    }
    else
    {
        lilycoveLady = NULL;
    }

    memcpy(sLilycoveLadySave, (void *)records + recordSize * mixIndices[multiplayerId], sizeof(LilycoveLady));
    ResetLilycoveLadyForRecordMix();
    if (lilycoveLady != NULL)
    {
        QuizLadyClearQuestionForRecordMix(lilycoveLady);
        Free(lilycoveLady);
    }
}

static u8 GetDaycareMailItemId(struct DaycareMail *mail)
{
    return mail->message.itemId;
}

// Indexes for a 2 element array used to store the multiplayer id and daycare
// slot that correspond to a daycare Pokémon that can hold an item.
enum {
    MULTIPLAYER_ID,
    DAYCARE_SLOT,
};

static void SwapDaycareMail(struct RecordMixingDaycareMail *records, size_t recordSize, u8 (*idxs)[2], u8 playerSlot1, u8 playerSlot2)
{
    struct DaycareMail temp;
    struct RecordMixingDaycareMail *mixMail1, *mixMail2;

    // 1st player's daycare mail --> temp
    mixMail1 = (void *)records + recordSize * idxs[playerSlot1][MULTIPLAYER_ID];
    memcpy(&temp, &mixMail1->mail[idxs[playerSlot1][DAYCARE_SLOT]], sizeof(struct DaycareMail));

    // 2nd player's daycare mail --> 1st player's daycare mail
    mixMail2 = (void *)records + recordSize * idxs[playerSlot2][MULTIPLAYER_ID];
    memcpy(&mixMail1->mail[idxs[playerSlot1][DAYCARE_SLOT]], &mixMail2->mail[idxs[playerSlot2][DAYCARE_SLOT]], sizeof(struct DaycareMail));

    // temp --> 2nd player's daycare mail
    memcpy(&mixMail2->mail[idxs[playerSlot2][DAYCARE_SLOT]], &temp, sizeof(struct DaycareMail));
}

// This sum is used to determine which players will swap daycare mail if there are more than 2 players who can.
// The TV show data is used to calculate this sum.
static void CalculateDaycareMailRandSum(const u8 *src)
{
    u8 sum;
    s32 i;

    sum = 0;
    for (i = 0; i < 256; i++)
        sum += src[i];

    sDaycareMailRandSum = sum;
}

#if TESTING
u8 GetDaycareMailRandSum(void)
#else
static u8 GetDaycareMailRandSum(void)
#endif
{
    return sDaycareMailRandSum;
}

static void ReceiveDaycareMailData(struct RecordMixingDaycareMail *records, size_t recordSize, u8 multiplayerId)
{
    u16 i, j;
    u8 linkPlayerCount;
    u8 tableId;
    struct RecordMixingDaycareMail *mixMail;
    u8 playerSlot1, playerSlot2;
    void *ptr;
    bool8 canHoldItem[MAX_LINK_PLAYERS][DAYCARE_MON_COUNT];
    u8 idxs[MAX_LINK_PLAYERS][2];
    u8 numDaycareCanHold;
    u16 oldSeed;
    bool32 anyRS;

    // Seed RNG to the first player's trainer id so that
    // every player has the same random swap occur
    // (see the other use of Random2 in this function)
    oldSeed = Random2();
    SeedRng2(gLinkPlayers[0].trainerId);
    linkPlayerCount = GetLinkPlayerCount();
    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        canHoldItem[i][0] = FALSE;
        canHoldItem[i][1] = FALSE;
    }

    // Handle language differences if RS / Japanese players are present
    anyRS = Link_AnyPartnersPlayingRubyOrSapphire();
    for (i = 0; i < GetLinkPlayerCount(); i++)
    {
        u32 language, version;

        mixMail = (void *)records + i * recordSize;
        language = gLinkPlayers[i].language;
        version = gLinkPlayers[i].version & 0xFF;

        for (j = 0; j < mixMail->numDaycareMons; j++)
        {
            u16 otNameLanguage, nicknameLanguage;
            struct DaycareMail *daycareMail = &mixMail->mail[j];

            if (daycareMail->message.itemId == ITEM_NONE)
                continue;

            if (anyRS)
            {
                // Handle OT name language
                if (StringLength(daycareMail->otName) <= 5)
                {
                    otNameLanguage = LANGUAGE_JAPANESE;
                }
                else
                {
                    StripExtCtrlCodes(daycareMail->otName);
                    otNameLanguage = language;
                }

                // Handle nickname langugae
                if (daycareMail->monName[0] == EXT_CTRL_CODE_BEGIN && daycareMail->monName[1] == EXT_CTRL_CODE_JPN)
                {
                    StripExtCtrlCodes(daycareMail->monName);
                    nicknameLanguage = LANGUAGE_JAPANESE;
                }
                else
                {
                    nicknameLanguage = language;
                }

                // Set languages
                if (version == VERSION_RUBY || version == VERSION_SAPPHIRE)
                {
                    daycareMail->gameLanguage = otNameLanguage;
                    daycareMail->monLanguage = nicknameLanguage;
                }
            }
            else if (language == LANGUAGE_JAPANESE)
            {
                if (IsStringJapanese(daycareMail->otName))
                    daycareMail->gameLanguage = LANGUAGE_JAPANESE;
                else
                    daycareMail->gameLanguage = GAME_LANGUAGE;

                if (IsStringJapanese(daycareMail->monName))
                    daycareMail->monLanguage = LANGUAGE_JAPANESE;
                else
                    daycareMail->monLanguage = GAME_LANGUAGE;
            }
        }
    }

    // For each player, get which of their daycare Pokémon can hold items
    // (can't hold items if already holding one, or if daycare slot is empty).
    // Note that when deposited in the daycare, Pokémon have their mail taken
    // from them and returned upon withdrawal, which means daycare Pokémon that
    // have associated mail do not have a held item.
    // Because not holding an item is the only determination for a swap, this also
    // means that a "swap" can occur even if neither Pokémon has associated mail.
    numDaycareCanHold = 0;
    for (i = 0; i < linkPlayerCount; i++)
    {
        mixMail = (void *)records + i * recordSize;
        if (mixMail->numDaycareMons == 0)
            continue;

        for (j = 0; j < mixMail->numDaycareMons; j++)
        {
            if (!mixMail->cantHoldItem[j])
                canHoldItem[i][j] = TRUE;
        }
    }

    // Fill the idxs array with data about which players
    // and which daycare slots should swap mail.
    j = 0;
    for (i = 0; i < linkPlayerCount; i++)
    {
        mixMail = (void *)records + i * recordSize;

        // Count number of players that have at least
        // one daycare Pokémon with no held item
        if (canHoldItem[i][0] == TRUE || canHoldItem[i][1] == TRUE)
            numDaycareCanHold++;

        if (canHoldItem[i][0] == TRUE && canHoldItem[i][1] == FALSE)
        {
            // Only daycare slot 0 can hold an item for this player, record it
            idxs[j][MULTIPLAYER_ID] = i;
            idxs[j][DAYCARE_SLOT] = 0;
            j++;
        }
        else if (canHoldItem[i][0] == FALSE && canHoldItem[i][1] == TRUE)
        {
            // Only daycare slot 1 can hold an item for this player, record it
            idxs[j][MULTIPLAYER_ID] = i;
            idxs[j][DAYCARE_SLOT] = 1;
            j++;
        }
        else if (canHoldItem[i][0] == TRUE && canHoldItem[i][1] == TRUE)
        {
            // Both daycare slots can hold an item, choose which one to use.
            // If either one is the only one to have associated mail, use that one.
            // If both do or don't have associated mail, choose one randomly.
            u32 itemId1, itemId2;
            idxs[j][MULTIPLAYER_ID] = i;
            itemId1 = GetDaycareMailItemId(&mixMail->mail[0]);
            itemId2 = GetDaycareMailItemId(&mixMail->mail[1]);

            if ((!itemId1 && !itemId2) || (itemId1 && itemId2))
                idxs[j][DAYCARE_SLOT] = Random2() % 2;
            else if (itemId1 && !itemId2)
                idxs[j][DAYCARE_SLOT] = 0;
            else if (!itemId1 && itemId2)
                 idxs[j][DAYCARE_SLOT] = 1;

            j++;
        }
    }

    // Copy the player's record mix mail 4 times to an array that's never read.
    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        mixMail = &records[multiplayerId * recordSize];
    }

    // Choose a random table id to determine who will
    // swap if there are more than 2 candidate players.
    tableId = GetDaycareMailRandSum() % NUM_SWAP_COMBOS;
    switch (numDaycareCanHold)
    {
    case 2:
        // 2 players can swap, just perform swap.
        SwapDaycareMail(records, recordSize, idxs, 0, 1);
        break;
    case 3:
        // 3 players can swap, select 2 and leave the 3rd out
        playerSlot1 = sDaycareMailSwapIds_3Player[tableId][0];
        playerSlot2 = sDaycareMailSwapIds_3Player[tableId][1];
        SwapDaycareMail(records, recordSize, idxs, playerSlot1, playerSlot2);
        break;
    case 4:
        // 4 players can swap, select which 2 pairings will swap
        ptr = idxs;

        // Swap pair 1
        playerSlot1 = sDaycareMailSwapIds_4Player[tableId][0];
        playerSlot2 = sDaycareMailSwapIds_4Player[tableId][1];
        SwapDaycareMail(records, recordSize, ptr, playerSlot1, playerSlot2);

        // Swap pair 2
        playerSlot1 = sDaycareMailSwapIds_4Player[tableId][2];
        playerSlot2 = sDaycareMailSwapIds_4Player[tableId][3];
        SwapDaycareMail(records, recordSize, ptr, playerSlot1, playerSlot2);
        break;
    }

    // Save player's record mixed mail to the daycare (in case it has changed)
    mixMail = (void *)records + multiplayerId * recordSize;
    memcpy(&gSaveBlock1Ptr->daycare.mons[0].mail, &mixMail->mail[0], sizeof(struct DaycareMail));
    memcpy(&gSaveBlock1Ptr->daycare.mons[1].mail, &mixMail->mail[1], sizeof(struct DaycareMail));
    SeedRng(oldSeed);
}


static void ReceiveGiftItem(u16 *item, u8 multiplayerId)
{
    if (multiplayerId != 0 && *item != ITEM_NONE && GetPocketByItemId(*item) == POCKET_KEY_ITEMS)
    {
        if (!CheckBagHasItem(*item, 1) && !CheckPCHasItem(*item, 1) && AddBagItem(*item, 1))
        {
            VarSet(VAR_TEMP_RECORD_MIX_GIFT_ITEM, *item);
            StringCopy(gStringVar1, gLinkPlayers[0].name);
            if (*item == ITEM_EON_TICKET)
                FlagSet(FLAG_ENABLE_SHIP_SOUTHERN_ISLAND);
        }
        else
        {
            VarSet(VAR_TEMP_RECORD_MIX_GIFT_ITEM, ITEM_NONE);
        }
    }
}

static void Task_DoRecordMixing(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
    case 0:
        task->tState++;
        break;
    case 1:
        if (Link_AnyPartnersPlayingRubyOrSapphire())
            task->tState++;
        else
            task->tState = 6;
        break;
    case 2:
        // Mixing Ruby/Sapphire records.
        SetContinueGameWarpStatusToDynamicWarp();
        WriteSaveBlock2();
        task->tState++;
        break;
    case 3:
        if (WriteSaveBlock1Sector())
        {
            ClearContinueGameWarpStatus2();
            task->tState = 4;
            task->data[1] = 0;
        }
        break;
    case 4: // Wait 10 frames
        if (++task->data[1] > 10)
        {
            SetCloseLinkCallback();
            task->tState++;
        }
        break;
    case 5:
        // Finish mixing Ruby/Sapphire records
        if (gReceivedRemoteLinkPlayers == FALSE)
            DestroyTask(taskId);
        break;

    // Mixing Emerald records.
    case 6:
        if (!Rfu_SetLinkRecovery(FALSE))
        {
            CreateTask(Task_LinkFullSave, 5);
            task->tState++;
        }
        break;
    case 7: // wait for Task_LinkFullSave to finish.
        if (!FuncIsActiveTask(Task_LinkFullSave))
        {
            if (gWirelessCommType)
            {
                Rfu_SetLinkRecovery(TRUE);
                task->tState = 8;
            }
            else
            {
                task->tState = 4;
            }
        }
        break;
    case 8:
        SetLinkStandbyCallback();
        task->tState++;
        break;
    case 9:
        if (IsLinkTaskFinished())
            DestroyTask(taskId);
        break;
    }
}

static void GetSavedApprentices(struct Apprentice *dst, struct Apprentice *src)
{
    s32 i, id;
    s32 apprenticeSaveId, oldPlayerApprenticeSaveId;
    s32 numOldPlayerApprentices, numMixApprentices;

    dst[0].playerName[0] = EOS;
    dst[1].playerName[0] = EOS;

    dst[0] = src[0];

    oldPlayerApprenticeSaveId = 0;
    numOldPlayerApprentices = 0;
    apprenticeSaveId = 0;
    numMixApprentices = 0;
    for (i = 0; i < 2; i++)
    {
        id = (i + gSaveBlock2Ptr->playerApprentice.saveId) % (APPRENTICE_COUNT - 1) + 1;
        if (src[id].playerName[0] != EOS)
        {
            if (GetTrainerId(src[id].playerId) != GetTrainerId(gSaveBlock2Ptr->playerTrainerId))
            {
                numMixApprentices++;
                apprenticeSaveId = id;
            }
            if (GetTrainerId(src[id].playerId) == GetTrainerId(gSaveBlock2Ptr->playerTrainerId))
            {
                numOldPlayerApprentices++;
                oldPlayerApprenticeSaveId = id;
            }
        }
    }

    // Prefer passing on other mixed Apprentices rather than old player's Apprentices
    if (numMixApprentices == 0 && numOldPlayerApprentices != 0)
    {
        numMixApprentices = numOldPlayerApprentices;
        apprenticeSaveId = oldPlayerApprenticeSaveId;
    }

    switch (numMixApprentices)
    {
    case 1:
        dst[1] = src[apprenticeSaveId];
        break;
    case 2:
        if (Random2() > 0x3333)
            dst[1] = src[gSaveBlock2Ptr->playerApprentice.saveId + 1];
        else
            dst[1] = src[((gSaveBlock2Ptr->playerApprentice.saveId + 1) % (APPRENTICE_COUNT - 1) + 1)];
        break;
    }
}

void GetPlayerHallRecords(struct PlayerHallRecords *dst)
{
    s32 i, j;

    for (i = 0; i < HALL_FACILITIES_COUNT; i++)
    {
        for (j = 0; j < FRONTIER_LVL_MODE_COUNT; j++)
        {
            CopyTrainerId(dst->onePlayer[i][j].id, gSaveBlock2Ptr->playerTrainerId);
            dst->onePlayer[i][j].language = GAME_LANGUAGE;
            StringCopy_PlayerName(dst->onePlayer[i][j].name, gSaveBlock2Ptr->playerName);
        }
    }

    for (j = 0; j < FRONTIER_LVL_MODE_COUNT; j++)
    {
        dst->twoPlayers[j].language = GAME_LANGUAGE;
        CopyTrainerId(dst->twoPlayers[j].id1, gSaveBlock2Ptr->playerTrainerId);
        CopyTrainerId(dst->twoPlayers[j].id2, gSaveBlock2Ptr->frontier.opponentTrainerIds[j]);
        StringCopy_PlayerName(dst->twoPlayers[j].name1, gSaveBlock2Ptr->playerName);
        StringCopy_PlayerName(dst->twoPlayers[j].name2, gSaveBlock2Ptr->frontier.opponentNames[j]);
    }

    for (i = 0; i < FRONTIER_LVL_MODE_COUNT; i++)
    {
        dst->onePlayer[RANKING_HALL_TOWER_SINGLES][i].winStreak = gSaveBlock2Ptr->frontier.towerRecordWinStreaks[FRONTIER_MODE_SINGLES][i];
        dst->onePlayer[RANKING_HALL_TOWER_DOUBLES][i].winStreak = gSaveBlock2Ptr->frontier.towerRecordWinStreaks[FRONTIER_MODE_DOUBLES][i];
        dst->onePlayer[RANKING_HALL_TOWER_MULTIS][i].winStreak = gSaveBlock2Ptr->frontier.towerRecordWinStreaks[FRONTIER_MODE_MULTIS][i];
        dst->onePlayer[RANKING_HALL_DOME][i].winStreak = gSaveBlock2Ptr->frontier.domeRecordWinStreaks[FRONTIER_MODE_SINGLES][i];
        dst->onePlayer[RANKING_HALL_PALACE][i].winStreak = gSaveBlock2Ptr->frontier.palaceRecordWinStreaks[FRONTIER_MODE_SINGLES][i];
        dst->onePlayer[RANKING_HALL_ARENA][i].winStreak = gSaveBlock2Ptr->frontier.arenaRecordStreaks[i];
        dst->onePlayer[RANKING_HALL_FACTORY][i].winStreak = gSaveBlock2Ptr->frontier.factoryRecordWinStreaks[FRONTIER_MODE_SINGLES][i];
        dst->onePlayer[RANKING_HALL_PIKE][i].winStreak = gSaveBlock2Ptr->frontier.pikeRecordStreaks[i];
        dst->onePlayer[RANKING_HALL_PYRAMID][i].winStreak = gSaveBlock2Ptr->frontier.pyramidRecordStreaks[i];

        dst->twoPlayers[i].winStreak = gSaveBlock2Ptr->frontier.towerRecordWinStreaks[FRONTIER_MODE_LINK_MULTIS][i];
    }
}

static bool32 IsApprenticeAlreadySaved(struct Apprentice *mixApprentice, struct Apprentice *apprentices)
{
    s32 i;

    for (i = 0; i < APPRENTICE_COUNT; i++)
    {
        if (GetTrainerId(mixApprentice->playerId) == GetTrainerId(apprentices[i].playerId)
            && mixApprentice->number == apprentices[i].number)
            return TRUE;
    }

    return FALSE;
}

static void ReceiveApprenticeData(struct Apprentice *records, size_t recordSize, u32 multiplayerId)
{
    s32 i, numApprentices, apprenticeId;
    struct Apprentice *mixApprentice;
    u32 mixIndices[MAX_LINK_PLAYERS];
    u32 apprenticeSaveId;

    ShufflePlayerIndices(mixIndices);
    mixApprentice = (void *)records + (recordSize * mixIndices[multiplayerId]);
    numApprentices = 0;
    apprenticeId = 0;
    for (i = 0; i < 2; i++)
    {
        if (mixApprentice[i].playerName[0] != EOS && !IsApprenticeAlreadySaved(&mixApprentice[i], &gSaveBlock2Ptr->apprentices[0]))
        {
            numApprentices++;
            apprenticeId = i;
        }
    }

    switch (numApprentices)
    {
    case 1:
        apprenticeSaveId = gSaveBlock2Ptr->playerApprentice.saveId + 1;
        gSaveBlock2Ptr->apprentices[apprenticeSaveId] = mixApprentice[apprenticeId];
        gSaveBlock2Ptr->playerApprentice.saveId = (gSaveBlock2Ptr->playerApprentice.saveId + 1) % (APPRENTICE_COUNT - 1);
        break;
    case 2:
        for (i = 0; i < 2; i++)
        {
            apprenticeSaveId = ((i ^ 1) + gSaveBlock2Ptr->playerApprentice.saveId) % (APPRENTICE_COUNT - 1) + 1;
            gSaveBlock2Ptr->apprentices[apprenticeSaveId] = mixApprentice[i];
        }
        gSaveBlock2Ptr->playerApprentice.saveId = (gSaveBlock2Ptr->playerApprentice.saveId + 2) % (APPRENTICE_COUNT - 1);
        break;
    }
}

#if FREE_RECORD_MIXING_HALL_RECORDS == FALSE
static void GetNewHallRecords(struct RecordMixingHallRecords *dst, void *records, size_t recordSize, u32 multiplayerId, s32 linkPlayerCount)
{
    s32 i, j, k, l;
    s32 repeatTrainers;

    // Load sPartnerHallRecords with link partners' hall records
    k = 0;
    for (i = 0; i < linkPlayerCount; i++)
    {
        if (i != multiplayerId)
            sPartnerHallRecords[k++] = records;
        if (k == HALL_RECORDS_COUNT)
            break;
        records += recordSize;
    }

    // Get improved 1P hall records
    for (i = 0; i < HALL_FACILITIES_COUNT; i++)
    {
        for (j = 0; j < FRONTIER_LVL_MODE_COUNT; j++)
        {
            // First get the existing saved records
            for (k = 0; k < HALL_RECORDS_COUNT; k++)
                dst->hallRecords1P[i][j][k] = gSaveBlock2Ptr->hallRecords1P[i][j][k];

            // Then read the new mixed records
            for (k = 0; k < linkPlayerCount - 1; k++)
            {
                repeatTrainers = 0;
                for (l = 0; l < HALL_RECORDS_COUNT; l++)
                {
                    // If the new trainer is already in the existing saved records, only
                    // use the new one if the win streak is better
                    if (GetTrainerId(dst->hallRecords1P[i][j][l].id) == GetTrainerId(sPartnerHallRecords[k]->onePlayer[i][j].id))
                    {
                        repeatTrainers++;
                        if (dst->hallRecords1P[i][j][l].winStreak < sPartnerHallRecords[k]->onePlayer[i][j].winStreak)
                            dst->hallRecords1P[i][j][l] = sPartnerHallRecords[k]->onePlayer[i][j];
                    }
                }

                // If all of the mixed records are new trainers, just save them
                if (repeatTrainers == 0)
                    dst->hallRecords1P[i][j][k + HALL_RECORDS_COUNT] = sPartnerHallRecords[k]->onePlayer[i][j];
            }
        }
    }

    // Get improved 2P hall records
    for (j = 0; j < FRONTIER_LVL_MODE_COUNT; j++)
    {
        // First get the existing saved records
        for (k = 0; k < HALL_RECORDS_COUNT; k++)
            dst->hallRecords2P[j][k] = gSaveBlock2Ptr->hallRecords2P[j][k];

        // Then read the new mixed records
        for (k = 0; k < linkPlayerCount - 1; k++)
        {
            repeatTrainers = 0;
            for (l = 0; l < HALL_RECORDS_COUNT; l++)
            {
                // If the new trainer pair is already in the existing saved records, only
                // use the new pair if the win streak is better
                if (GetTrainerId(dst->hallRecords2P[j][l].id1) == GetTrainerId(sPartnerHallRecords[k]->twoPlayers[j].id1)
                 && GetTrainerId(dst->hallRecords2P[j][l].id2) == GetTrainerId(sPartnerHallRecords[k]->twoPlayers[j].id2))
                {
                    repeatTrainers++;
                    if (dst->hallRecords2P[j][l].winStreak < sPartnerHallRecords[k]->twoPlayers[j].winStreak)
                        dst->hallRecords2P[j][l] = sPartnerHallRecords[k]->twoPlayers[j];
                }
            }

            // If all of the mixed records are new trainer pairs, just save them
            if (repeatTrainers == 0)
                dst->hallRecords2P[j][k + HALL_RECORDS_COUNT] = sPartnerHallRecords[k]->twoPlayers[j];
        }
    }
}

static void FillWinStreakRecords1P(struct RankingHall1P *playerRecords, struct RankingHall1P *mixRecords)
{
    s32 i, j;

    // Fill the player's 1P records with the highest win streaks from the mixed records
    for (i = 0; i < HALL_RECORDS_COUNT; i++)
    {
        // Get the highest remaining win streak in the mixed hall records
        s32 highestWinStreak = 0;
        s32 highestId = -1;
        for (j = 0; j < HALL_RECORDS_COUNT * 2; j++)
        {
            if (mixRecords[j].winStreak > highestWinStreak)
            {
                highestId = j;
                highestWinStreak = mixRecords[j].winStreak;
            }
        }

        // Save the win streak to the player's records, then clear it from the mixed records
        if (highestId >= 0)
        {
            playerRecords[i] = mixRecords[highestId];
            mixRecords[highestId].winStreak = 0;
        }
    }
}

static void FillWinStreakRecords2P(struct RankingHall2P *playerRecords, struct RankingHall2P *mixRecords)
{
    s32 i, j;

    // Fill the player's 2P records with the highest win streaks from the mixed records
    for (i = 0; i < HALL_RECORDS_COUNT; i++)
    {
        // Get the highest remaining win streak in the mixed hall records
        s32 highestWinStreak = 0;
        s32 highestId = -1;
        for (j = 0; j < HALL_RECORDS_COUNT * 2; j++)
        {
            if (mixRecords[j].winStreak > highestWinStreak)
            {
                highestId = j;
                highestWinStreak = mixRecords[j].winStreak;
            }
        }

        // Save the win streak to the player's records, then clear it from the mixed records
        if (highestId >= 0)
        {
            playerRecords[i] = mixRecords[highestId];
            mixRecords[highestId].winStreak = 0;
        }
    }
}

static void SaveHighestWinStreakRecords(struct RecordMixingHallRecords *mixHallRecords)
{
    s32 i, j;

    for (i = 0; i < HALL_FACILITIES_COUNT; i++)
    {
        for (j = 0; j < FRONTIER_LVL_MODE_COUNT; j++)
            FillWinStreakRecords1P(gSaveBlock2Ptr->hallRecords1P[i][j], mixHallRecords->hallRecords1P[i][j]);
    }

    for (j = 0; j < FRONTIER_LVL_MODE_COUNT; j++)
        FillWinStreakRecords2P(gSaveBlock2Ptr->hallRecords2P[j], mixHallRecords->hallRecords2P[j]);
}
#endif //FREE_RECORD_MIXING_HALL_RECORDS

static void ReceiveRankingHallRecords(struct PlayerHallRecords *records, size_t recordSize, u32 multiplayerId)
{
#if FREE_RECORD_MIXING_HALL_RECORDS == FALSE
    u8 linkPlayerCount = GetLinkPlayerCount();
    struct RecordMixingHallRecords *mixHallRecords = AllocZeroed(sizeof(*mixHallRecords));

    if (mixHallRecords == NULL)
        return;

    GetNewHallRecords(mixHallRecords, records, recordSize, multiplayerId, linkPlayerCount);
    SaveHighestWinStreakRecords(mixHallRecords);

    Free(mixHallRecords);
#endif //FREE_RECORD_MIXING_HALL_RECORDS
}

static void GetRecordMixingDaycareMail(struct RecordMixingDaycareMail *dst)
{
    sRecordMixMail.mail[0] = gSaveBlock1Ptr->daycare.mons[0].mail;
    sRecordMixMail.mail[1] = gSaveBlock1Ptr->daycare.mons[1].mail;
    InitDaycareMailRecordMixing(&gSaveBlock1Ptr->daycare, &sRecordMixMail);
    *dst = *sRecordMixMailSave;
}

static void SanitizeDaycareMailForRuby(struct RecordMixingDaycareMail *src)
{
    s32 i;

    for (i = 0; i < src->numDaycareMons; i++)
    {
        struct DaycareMail *mail = &src->mail[i];
        if (mail->message.itemId != ITEM_NONE)
        {
            if (mail->gameLanguage != LANGUAGE_JAPANESE)
                PadNameString(mail->otName, EXT_CTRL_CODE_BEGIN);

            ConvertInternationalString(mail->monName, mail->monLanguage);
        }
    }
}

static void SanitizeRubyBattleTowerRecord(struct RSBattleTowerRecord *src)
{

}

static void SanitizeEmeraldBattleTowerRecord(struct EmeraldBattleTowerRecord *dst)
{
    s32 i;

    for (i = 0; i < MAX_FRONTIER_PARTY_SIZE; i++)
    {
        struct BattleTowerPokemon *towerMon = &dst->party[i];
        if (towerMon->species != SPECIES_NONE)
            StripExtCtrlCodes(towerMon->nickname);
    }

    CalcEmeraldBattleTowerChecksum(dst);
}
