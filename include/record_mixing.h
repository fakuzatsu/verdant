#ifndef GUARD_RECORD_MIXING_H
#define GUARD_RECORD_MIXING_H

#include "daycare.h"

struct PlayerHallRecords
{
    struct RankingHall1P onePlayer[HALL_FACILITIES_COUNT][FRONTIER_LVL_MODE_COUNT];
    struct RankingHall2P twoPlayers[FRONTIER_LVL_MODE_COUNT];
};

// The Emerald record-mixing payload. Internet packets wrap this native payload
// in a versioned header and use a block mask so a server can publish only the
// records it intends to change. Bytes in blocks absent from the mask are ignored.
struct PlayerRecordEmerald
{
    /* 0x0000 */ struct SecretBase secretBases[SECRET_BASES_COUNT];
    /* 0x0640 */ TVShow tvShows[TV_SHOWS_COUNT];
    /* 0x09C4 */ PokeNews pokeNews[POKE_NEWS_COUNT];
    /* 0x0A04 */ OldMan oldMan;
    /* 0x0A44 */ struct DewfordTrend dewfordTrends[SAVED_TRENDS_COUNT];
    /* 0x0A6C */ struct RecordMixingDaycareMail daycareMail;
    /* 0x0AE4 */ struct EmeraldBattleTowerRecord battleTowerRecord;
    /* 0x0BD0 */ u16 giftItem;
    /* 0x0BD4 */ LilycoveLady lilycoveLady;
    /* 0x0C14 */ struct Apprentice apprentices[2];
    /* 0x0C9C */ struct PlayerHallRecords hallRecords;
    /* 0x0DF4 */ u8 filler_1434[16];
}; // 0x0E04

#define INTERNET_RECORD_MIX_HEADER_SIZE 28
#define INTERNET_RECORD_MIX_BLOCK_MASK_OFFSET 20
#define INTERNET_RECORD_MIX_MAX_PACKET_SIZE (INTERNET_RECORD_MIX_HEADER_SIZE + sizeof(struct PlayerRecordEmerald))
#define INTERNET_RECORD_CODE_LENGTH 12

enum InternetRecordMixBlock
{
    INTERNET_RECORD_MIX_BLOCK_SECRET_BASES  = (1 << 0),
    INTERNET_RECORD_MIX_BLOCK_TV_SHOWS      = (1 << 1),
    INTERNET_RECORD_MIX_BLOCK_POKE_NEWS     = (1 << 2),
    INTERNET_RECORD_MIX_BLOCK_OLD_MAN       = (1 << 3),
    INTERNET_RECORD_MIX_BLOCK_DEWFORD       = (1 << 4),
    INTERNET_RECORD_MIX_BLOCK_DAYCARE_MAIL  = (1 << 5),
    INTERNET_RECORD_MIX_BLOCK_BATTLE_TOWER  = (1 << 6),
    INTERNET_RECORD_MIX_BLOCK_GIFT_ITEM     = (1 << 7),
    INTERNET_RECORD_MIX_BLOCK_LILYCOVE_LADY = (1 << 8),
    INTERNET_RECORD_MIX_BLOCK_APPRENTICES   = (1 << 9),
    INTERNET_RECORD_MIX_BLOCK_HALL_RECORDS  = (1 << 10),
    INTERNET_RECORD_MIX_ALL_BLOCKS           = (1 << 11) - 1,
};

enum InternetRecordMixResult
{
    INTERNET_RECORD_MIX_INVALID_PACKET,
    INTERNET_RECORD_MIX_RECEIVED,
    INTERNET_RECORD_MIX_OUT_OF_MEMORY,
};

void RecordMixingPlayerSpotTriggered(void);
u16 GetRecordMixingGift(void);
void GetPlayerHallRecords(struct PlayerHallRecords *dst);
u16 BuildInternetRecordMixPacket(u8 *packet, u16 capacity);
bool32 DecodeInternetRecordMixCode(const u8 *response, u16 responseSize, u8 *code);
enum InternetRecordMixResult ReceiveInternetRecordMix(const u8 *packet, u16 packetSize, u8 *sourceName);
#if TESTING
u8 GetDaycareMailRandSum(void);
#endif

#endif //GUARD_RECORD_MIXING_H
