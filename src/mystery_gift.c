#include "global.h"
#include "event_data.h"
#include "item.h"
#include "mystery_gift.h"
#include "pokedex.h"
#include "pokemon.h"
#include "strings.h"
#include "util.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokedex.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/vars.h"
#include "constants/wild_encounter.h"

#define INTERNET_MYSTERY_GIFT_VERSION 1
#define INTERNET_MYSTERY_GIFT_ITEM_PAYLOAD_SIZE 4
#define INTERNET_MYSTERY_GIFT_EVENT_PAYLOAD_SIZE 4

static const u8 sInternetMysteryGiftMagic[] = {'P', 'M', 'G', 'F'};

STATIC_ASSERT(sizeof(struct BoxPokemon) == 80, InternetMysteryGift_BoxPokemonSchemaMustBeUpdated);

struct InternetMysteryGiftEventHandler
{
    u16 eventId;
    enum InternetMysteryGiftResult (*apply)(u16 value);
    const u8 *message;
};

static enum InternetMysteryGiftResult ApplyAuroraTicketEvent(u16 value);
static enum InternetMysteryGiftResult ApplyOldSeaMapEvent(u16 value);
static enum InternetMysteryGiftResult ApplyAlteringCaveEvent(u16 value);

static const struct InternetMysteryGiftEventHandler sInternetMysteryGiftEventHandlers[] =
{
    {INTERNET_MYSTERY_GIFT_EVENT_AURORA_TICKET, ApplyAuroraTicketEvent, gText_InternetGiftAuroraTicket},
    {INTERNET_MYSTERY_GIFT_EVENT_OLD_SEA_MAP, ApplyOldSeaMapEvent, gText_InternetGiftOldSeaMap},
    {INTERNET_MYSTERY_GIFT_EVENT_ALTERING_CAVE, ApplyAlteringCaveEvent, gText_InternetGiftAlteringCave},
};

static u16 ReadInternetMysteryGiftU16(const u8 *data)
{
    return data[0] | (data[1] << 8);
}

static const struct InternetMysteryGiftEventHandler *FindInternetMysteryGiftEventHandler(u16 eventId)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sInternetMysteryGiftEventHandlers); i++)
    {
        if (sInternetMysteryGiftEventHandlers[i].eventId == eventId)
            return &sInternetMysteryGiftEventHandlers[i];
    }
    return NULL;
}

static enum InternetMysteryGiftResult ApplyAuroraTicketEvent(u16 value)
{
    (void)value;
    if (!AddBagItem(ITEM_AURORA_TICKET, 1))
        return INTERNET_MYSTERY_GIFT_NO_SPACE;
    FlagSet(FLAG_ENABLE_SHIP_BIRTH_ISLAND);
    return INTERNET_MYSTERY_GIFT_RECEIVED_EVENT;
}

static enum InternetMysteryGiftResult ApplyOldSeaMapEvent(u16 value)
{
    (void)value;
    if (!AddBagItem(ITEM_OLD_SEA_MAP, 1))
        return INTERNET_MYSTERY_GIFT_NO_SPACE;
    FlagSet(FLAG_ENABLE_SHIP_FARAWAY_ISLAND);
    return INTERNET_MYSTERY_GIFT_RECEIVED_EVENT;
}

static enum InternetMysteryGiftResult ApplyAlteringCaveEvent(u16 value)
{
    if (value >= NUM_ALTERING_CAVE_TABLES)
        return INTERNET_MYSTERY_GIFT_INVALID_PACKET;
    VarSet(VAR_ALTERING_CAVE_WILD_SET, value);
    return INTERNET_MYSTERY_GIFT_RECEIVED_EVENT;
}

const u8 *GetInternetMysteryGiftEventMessage(u16 eventId)
{
    const struct InternetMysteryGiftEventHandler *handler = FindInternetMysteryGiftEventHandler(eventId);

    return handler == NULL ? NULL : handler->message;
}

static bool32 IsInternetGiftPokemonValid(struct BoxPokemon *boxMon)
{
    u16 species = GetBoxMonData(boxMon, MON_DATA_SPECIES);
    u16 heldItem = GetBoxMonData(boxMon, MON_DATA_HELD_ITEM);
    u32 experience = GetBoxMonData(boxMon, MON_DATA_EXP);
    u32 totalEvs = 0;
    u32 i;

    if (boxMon->isBadEgg || !boxMon->hasSpecies)
        return FALSE;
    if (species == SPECIES_NONE || species >= NUM_SPECIES)
        return FALSE;
    if (heldItem >= ITEMS_COUNT
     || (heldItem != ITEM_NONE && ItemId_GetPocket(heldItem) == POCKET_NONE))
        return FALSE;
    if (experience > gExperienceTables[gSpeciesInfo[species].growthRate][MAX_LEVEL])
        return FALSE;
    if (boxMon->checksum != CalculateBoxMonChecksum(boxMon))
        return FALSE;
    if (GetBoxMonData(boxMon, MON_DATA_TERA_TYPE) >= NUMBER_OF_MON_TYPES)
        return FALSE;
    if (GetBoxMonData(boxMon, MON_DATA_POKEBALL) < FIRST_BALL
     || GetBoxMonData(boxMon, MON_DATA_POKEBALL) > LAST_BALL)
        return FALSE;
    if (GetBoxMonData(boxMon, MON_DATA_ABILITY_NUM) >= NUM_ABILITY_SLOTS)
        return FALSE;
    if (GetBoxMonData(boxMon, MON_DATA_DYNAMAX_LEVEL) > MAX_DYNAMAX_LEVEL)
        return FALSE;
    if (GetBoxMonData(boxMon, MON_DATA_HIDDEN_NATURE) >= NUM_NATURES)
        return FALSE;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (GetBoxMonData(boxMon, MON_DATA_MOVE1 + i) >= MOVES_COUNT)
            return FALSE;
    }

    for (i = 0; i < NUM_STATS; i++)
        totalEvs += GetBoxMonData(boxMon, MON_DATA_HP_EV + i);
    return totalEvs <= MAX_TOTAL_EVS;
}

static bool32 DecodeInternetMysteryGiftPacket(const u8 *packet, u16 packetSize, struct InternetMysteryGift *gift, u16 *status)
{
    const u8 *payload;
    u16 payloadSize;
    u16 payloadCrc;

    if (gift == NULL || status == NULL)
        return FALSE;
    memset(gift, 0, sizeof(*gift));

    if (packet == NULL || packetSize < INTERNET_MYSTERY_GIFT_HEADER_SIZE)
        return FALSE;
    if (memcmp(packet, sInternetMysteryGiftMagic, sizeof(sInternetMysteryGiftMagic)) != 0)
        return FALSE;
    if (packet[4] != INTERNET_MYSTERY_GIFT_VERSION)
        return FALSE;

    payloadSize = ReadInternetMysteryGiftU16(&packet[6]);
    payloadCrc = ReadInternetMysteryGiftU16(&packet[8]);
    *status = ReadInternetMysteryGiftU16(&packet[10]);
    if (payloadSize != packetSize - INTERNET_MYSTERY_GIFT_HEADER_SIZE)
        return FALSE;

    payload = &packet[INTERNET_MYSTERY_GIFT_HEADER_SIZE];
    if (payloadCrc != CalcCRC16WithTable(payload, payloadSize))
        return FALSE;
    gift->type = packet[5];

    if (*status != INTERNET_MYSTERY_GIFT_STATUS_OK)
    {
        return (*status == INTERNET_MYSTERY_GIFT_STATUS_ALREADY_RECEIVED
             || *status == INTERNET_MYSTERY_GIFT_STATUS_UNKNOWN_IDENTITY)
            && gift->type == INTERNET_MYSTERY_GIFT_NONE
            && payloadSize == 0;
    }

    switch (gift->type)
    {
    case INTERNET_MYSTERY_GIFT_POKEMON:
        if (payloadSize != sizeof(struct BoxPokemon))
            return FALSE;
        memcpy(&gift->data.pokemon.box, payload, sizeof(struct BoxPokemon));
        if (!IsInternetGiftPokemonValid(&gift->data.pokemon.box))
            return FALSE;
        if (gift->data.pokemon.box.isEgg != GetBoxMonData(&gift->data.pokemon.box, MON_DATA_IS_EGG))
            return FALSE;
        BoxMonToMon(&gift->data.pokemon.box, &gift->data.pokemon);
        break;
    case INTERNET_MYSTERY_GIFT_ITEM:
        if (payloadSize != INTERNET_MYSTERY_GIFT_ITEM_PAYLOAD_SIZE)
            return FALSE;
        gift->data.item.itemId = ReadInternetMysteryGiftU16(payload);
        gift->data.item.quantity = ReadInternetMysteryGiftU16(&payload[2]);
        if (gift->data.item.itemId == ITEM_NONE
         || gift->data.item.itemId >= ITEMS_COUNT
         || ItemId_GetPocket(gift->data.item.itemId) == POCKET_NONE
         || gift->data.item.quantity == 0
         || gift->data.item.quantity > MAX_BAG_ITEM_CAPACITY)
            return FALSE;
        break;
    case INTERNET_MYSTERY_GIFT_EVENT:
        if (payloadSize != INTERNET_MYSTERY_GIFT_EVENT_PAYLOAD_SIZE)
            return FALSE;
        gift->data.event.eventId = ReadInternetMysteryGiftU16(payload);
        gift->data.event.value = ReadInternetMysteryGiftU16(&payload[2]);
        if (FindInternetMysteryGiftEventHandler(gift->data.event.eventId) == NULL)
            return FALSE;
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

static enum InternetMysteryGiftResult ApplyInternetMysteryGift(const struct InternetMysteryGift *gift)
{
    struct Pokemon pokemon;
    u16 species;
    u32 i;

    switch (gift->type)
    {
    case INTERNET_MYSTERY_GIFT_ITEM:
        if (!AddBagItem(gift->data.item.itemId, gift->data.item.quantity))
            return INTERNET_MYSTERY_GIFT_NO_SPACE;
        return INTERNET_MYSTERY_GIFT_RECEIVED_ITEM;
    case INTERNET_MYSTERY_GIFT_POKEMON:
        pokemon = gift->data.pokemon;
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_NONE)
                break;
        }

        if (i < PARTY_SIZE)
        {
            CopyMon(&gPlayerParty[i], &pokemon, sizeof(struct Pokemon));
            CalculatePlayerPartyCount();
        }
        else if (CopyMonToPC(&pokemon) == MON_CANT_GIVE)
        {
            return INTERNET_MYSTERY_GIFT_NO_SPACE;
        }

        species = GetMonData(&pokemon, MON_DATA_SPECIES_OR_EGG);
        if (species != SPECIES_EGG)
        {
            species = GetMonData(&pokemon, MON_DATA_SPECIES);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_SEEN);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT);
        }
        return i < PARTY_SIZE
             ? INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PARTY
             : INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PC;
    case INTERNET_MYSTERY_GIFT_EVENT:
    {
        const struct InternetMysteryGiftEventHandler *handler = FindInternetMysteryGiftEventHandler(gift->data.event.eventId);

        if (handler == NULL)
            return INTERNET_MYSTERY_GIFT_INVALID_PACKET;
        return handler->apply(gift->data.event.value);
    }
    default:
        return INTERNET_MYSTERY_GIFT_INVALID_PACKET;
    }
}

enum InternetMysteryGiftResult ReceiveInternetMysteryGift(const u8 *packet, u16 packetSize, struct InternetMysteryGift *gift)
{
    u16 status;

    if (!DecodeInternetMysteryGiftPacket(packet, packetSize, gift, &status))
        return INTERNET_MYSTERY_GIFT_INVALID_PACKET;
    if (status == INTERNET_MYSTERY_GIFT_STATUS_ALREADY_RECEIVED)
        return INTERNET_MYSTERY_GIFT_ALREADY_RECEIVED;
    if (status == INTERNET_MYSTERY_GIFT_STATUS_UNKNOWN_IDENTITY)
        return INTERNET_MYSTERY_GIFT_UNKNOWN_IDENTITY;
    return ApplyInternetMysteryGift(gift);
}
