#ifndef GUARD_MYSTERY_GIFT_H
#define GUARD_MYSTERY_GIFT_H

#include "pokemon.h"

// Wire header: "PMGF", protocol version, gift type, little-endian payload size,
// payload CRC16, and a status word.
#define INTERNET_MYSTERY_GIFT_HEADER_SIZE 12
#define INTERNET_MYSTERY_GIFT_MAX_PACKET_SIZE (INTERNET_MYSTERY_GIFT_HEADER_SIZE + sizeof(struct BoxPokemon))

enum InternetMysteryGiftType
{
    INTERNET_MYSTERY_GIFT_NONE,
    INTERNET_MYSTERY_GIFT_POKEMON,
    INTERNET_MYSTERY_GIFT_ITEM,
    INTERNET_MYSTERY_GIFT_EVENT,
};

enum InternetMysteryGiftEventId
{
    INTERNET_MYSTERY_GIFT_EVENT_NONE,
    INTERNET_MYSTERY_GIFT_EVENT_AURORA_TICKET,
    INTERNET_MYSTERY_GIFT_EVENT_OLD_SEA_MAP,
    INTERNET_MYSTERY_GIFT_EVENT_ALTERING_CAVE,
};

enum InternetMysteryGiftResult
{
    INTERNET_MYSTERY_GIFT_INVALID_PACKET,
    INTERNET_MYSTERY_GIFT_ALREADY_RECEIVED,
    INTERNET_MYSTERY_GIFT_UNKNOWN_IDENTITY,
    INTERNET_MYSTERY_GIFT_RECEIVED_ITEM,
    INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PARTY,
    INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PC,
    INTERNET_MYSTERY_GIFT_RECEIVED_EVENT,
    INTERNET_MYSTERY_GIFT_NO_SPACE,
};

enum InternetMysteryGiftStatus
{
    INTERNET_MYSTERY_GIFT_STATUS_OK,
    INTERNET_MYSTERY_GIFT_STATUS_ALREADY_RECEIVED,
    INTERNET_MYSTERY_GIFT_STATUS_UNKNOWN_IDENTITY,
};

struct InternetMysteryGiftItem
{
    u16 itemId;
    u16 quantity;
};

struct InternetMysteryGiftEvent
{
    u16 eventId;
    u16 value;
};

struct InternetMysteryGift
{
    u8 type;
    union
    {
        struct Pokemon pokemon;
        struct InternetMysteryGiftItem item;
        struct InternetMysteryGiftEvent event;
    } data;
};

enum InternetMysteryGiftResult ReceiveInternetMysteryGift(const u8 *packet, u16 packetSize, struct InternetMysteryGift *gift);
const u8 *GetInternetMysteryGiftEventMessage(u16 eventId);

#endif // GUARD_MYSTERY_GIFT_H
