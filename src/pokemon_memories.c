#include "global.h"
#include "event_data.h"
#include "malloc.h"
#include "pokemon.h"
#include "region_map.h"
#include "script.h"
#include "string_util.h"
#include "tv.h"

static const u8 sText_MetAtYZ[] = _("{STR_VAR_1} was caught by {STR_VAR_3} at\n{STR_VAR_2}.");
static const u8 sText_HatchedAtYZ[] = _("{STR_VAR_1} was hatched by {STR_VAR_3} at\n{STR_VAR_2}.");
static const u8 sText_ObtainedInTrade[] = _("{STR_VAR_1} was obtained in a trade\nfrom {STR_VAR_3}.");
static const u8 sText_FatefulEncounter[] = _("{STR_VAR_1} was obtained in a fateful encounter\nat {LV_2}{STR_VAR_2}.");
static const u8 sText_MetSomewhereAt[] = _("{STR_VAR_1} was caught somewhere at {LV_2}{STR_VAR_2}.");
static const u8 sText_HatchedSomewhereAt[] = _("{STR_VAR_1} hatched from an egg somewhere,\nat {LV_2}{STR_VAR_2}.");
static const u8 gText_ProbablyMetAt[] = _("{STR_VAR_1} seems to have met {STR_VAR_3}\nat {STR_VAR_2}");
static const u8 sText_KobesSeedot[] = _("DOTS was caught by KOBE at\n{LV_2}{STR_VAR_2}, ROUTE 102");
static const u8 sText_RomansPlusle[] = _("PLUSES was caught by CHESTER at\n{LV_2}{STR_VAR_2}, ROUTE 110");
static const u8 sText_SkylarsHorsea[] = _("SEASOR was hatched by SKYLAR at\n{LV_2}{STR_VAR_2}, PACIFIDLOG TOWN");
static const u8 sText_IsissMeowth[] = _("MEOWOW was caught by ISIS at\n{LV_2}{STR_VAR_2}, ROUTE 38");
static const u8 sText_AtLevelLocation[] = _("{LV_2}{STR_VAR_2}, at {STR_VAR_3}");

extern const u8 MemoryStrings_NoMemory[];
extern const u8 MemoryStrings_TransferedOverWonderTrade[];
extern const u8 MemoryStrings_TransferedOverLinkTrade[];
extern const u8 MemoryStrings_WonACoolContest[];
extern const u8 MemoryStrings_WonABeautyContest[];
extern const u8 MemoryStrings_WonACuteContest[];
extern const u8 MemoryStrings_WonACleverContest[];
extern const u8 MemoryStrings_WonAToughContest[];
extern const u8 MemoryStrings_AteACoolPokeblock[];
extern const u8 MemoryStrings_AteABeautyPokeblock[];
extern const u8 MemoryStrings_AteACutePokeblock[];
extern const u8 MemoryStrings_AteACleverPokeblock[];
extern const u8 MemoryStrings_AteAToughPokeblock[];
extern const u8 MemoryStrings_AteADislikedPokeblock[];
extern const u8 MemoryStrings_AteALovedPokeblock[];
extern const u8 MemoryStrings_MaxedOutSheen[];
extern const u8 MemoryStrings_FoughtRoxanneGym[];
extern const u8 MemoryStrings_FoughtBrawlyGym[];
extern const u8 MemoryStrings_FoughtWattsonGym[];
extern const u8 MemoryStrings_FoughtFlanneryGym[];
extern const u8 MemoryStrings_FoughtNormanGym[];
extern const u8 MemoryStrings_FoughtWinonaGym[];
extern const u8 MemoryStrings_FoughtTateAndLizaGym[];
extern const u8 MemoryStrings_FoughtWallaceGym[];
extern const u8 MemoryStrings_FoughtTheEliteFour[];
extern const u8 MemoryStrings_FoughtASignificant[];
extern const u8 MemoryStrings_EarnedSilverFrontier[];
extern const u8 MemoryStrings_CaughtALegendary[];
extern const u8 MemoryStrings_DefendedChampionTitle[];
extern const u8 MemoryStrings_WonAMasterContest[];
extern const u8 MemoryStrings_SavedTheWorld[];
extern const u8 MemoryStrings_EarnedGoldFrontier[];

static const u8 MemoryStrings_FullStop[] = _(".");
static const u8 MemoryStrings_ByX[] = _(" by {STR_VAR_2}.");
static const u8 MemoryStrings_WithX[] = _(" with {STR_VAR_2}.");
static const u8 MemoryStrings_GivenByX[] = _(" given by {STR_VAR_2}.");

static const u8 *const sMemoryStrings[MEMORY_COUNT][2] = {
    [MEMORY_NONE]               = {MemoryStrings_NoMemory,                  MemoryStrings_NoMemory},
    [MEMORY_WONDER_TRADE]       = {MemoryStrings_TransferedOverWonderTrade, MemoryStrings_ByX},
    [MEMORY_LINK_TRADE]         = {MemoryStrings_TransferedOverLinkTrade,   MemoryStrings_ByX},
    [MEMORY_COOL_CONTEST]       = {MemoryStrings_WonACoolContest,           MemoryStrings_WithX},
    [MEMORY_BEAUTY_CONTEST]     = {MemoryStrings_WonABeautyContest,         MemoryStrings_WithX},
    [MEMORY_CUTE_CONTEST]       = {MemoryStrings_WonACuteContest,           MemoryStrings_WithX},
    [MEMORY_CLEVER_CONTEST]     = {MemoryStrings_WonACleverContest,         MemoryStrings_WithX},
    [MEMORY_TOUGH_CONTEST]      = {MemoryStrings_WonAToughContest,          MemoryStrings_WithX},
    [MEMORY_COOL_POKEBLOCK]     = {MemoryStrings_AteACoolPokeblock,         MemoryStrings_GivenByX},
    [MEMORY_BEAUTY_POKEBLOCK]   = {MemoryStrings_AteABeautyPokeblock,       MemoryStrings_GivenByX},
    [MEMORY_CUTE_POKEBLOCK]     = {MemoryStrings_AteACutePokeblock,         MemoryStrings_GivenByX},
    [MEMORY_CLEVER_POKEBLOCK]   = {MemoryStrings_AteACleverPokeblock,       MemoryStrings_GivenByX},
    [MEMORY_TOUGH_POKEBLOCK]    = {MemoryStrings_AteAToughPokeblock,        MemoryStrings_GivenByX},
    [MEMORY_DISLIKED_POKEBLOCK] = {MemoryStrings_AteADislikedPokeblock,     MemoryStrings_GivenByX},
    [MEMORY_LOVED_POKEBLOCK]    = {MemoryStrings_AteALovedPokeblock,        MemoryStrings_GivenByX},
    [MEMORY_MAXED_OUT_SHEEN]    = {MemoryStrings_MaxedOutSheen,             MemoryStrings_GivenByX},
    [MEMORY_ROXANNE_GYM]        = {MemoryStrings_FoughtRoxanneGym,          MemoryStrings_WithX},
    [MEMORY_BRAWLY_GYM]         = {MemoryStrings_FoughtBrawlyGym,           MemoryStrings_WithX},
    [MEMORY_WATTSON_GYM]        = {MemoryStrings_FoughtWattsonGym,          MemoryStrings_WithX},
    [MEMORY_FLANNERY_GYM]       = {MemoryStrings_FoughtFlanneryGym,         MemoryStrings_WithX},
    [MEMORY_NORMAN_GYM]         = {MemoryStrings_FoughtNormanGym,           MemoryStrings_WithX},
    [MEMORY_WINONA_GYM]         = {MemoryStrings_FoughtWinonaGym,           MemoryStrings_WithX},
    [MEMORY_TATE_LIZA_GYM]      = {MemoryStrings_FoughtTateAndLizaGym,      MemoryStrings_WithX},
    [MEMORY_WALLACE_GYM]        = {MemoryStrings_FoughtWallaceGym,          MemoryStrings_WithX},
    [MEMORY_ELITE_FOUR]         = {MemoryStrings_FoughtTheEliteFour,        MemoryStrings_WithX},
    [MEMORY_SIG_TRAINER]        = {MemoryStrings_FoughtASignificant,        MemoryStrings_WithX},
    [MEMORY_SILVER_FRONTIER]    = {MemoryStrings_EarnedSilverFrontier,      MemoryStrings_WithX},
    [MEMORY_LEGENDARY_CATCH]    = {MemoryStrings_CaughtALegendary,          MemoryStrings_WithX},
    [MEMORY_CHAMPION_TITLE_DEF] = {MemoryStrings_DefendedChampionTitle,     MemoryStrings_WithX},
    [MEMORY_MASTER_CONTEST]     = {MemoryStrings_WonAMasterContest,         MemoryStrings_WithX},
    [MEMORY_SAVED_THE_WORLD]    = {MemoryStrings_SavedTheWorld,             MemoryStrings_WithX},
    [MEMORY_GOLD_FRONTIER]      = {MemoryStrings_EarnedGoldFrontier,        MemoryStrings_WithX},
};

void GetMemory(struct ScriptContext *ctx)
{
    u32 partyIndex = VarGet(ScriptReadHalfword(ctx));
    u32 memorySlot = ScriptReadByte(ctx);

    gSpecialVar_Result = FALSE;

    if (partyIndex < PARTY_SIZE)
        gSpecialVar_Result = GetMonData(&gPlayerParty[partyIndex], (memorySlot == MON_MEMORY_OLD) ? MON_DATA_MEMORY_OLD : MON_DATA_MEMORY_NEW);
}

void BufferMemoryMessage(struct ScriptContext *ctx)
{
    u32 partyIndex = VarGet(ScriptReadHalfword(ctx));
    u32 memorySlot = ScriptReadByte(ctx);
    u8 otName[PLAYER_NAME_LENGTH + 1];
    u32 otId = GetMonData(&gPlayerParty[partyIndex], MON_DATA_OT_ID);
    GetMonData(&gPlayerParty[partyIndex], MON_DATA_OT_NAME, otName);

    if (partyIndex < PARTY_SIZE)
    {
        if (memorySlot == MON_MEMORY_OLD || memorySlot == MON_MEMORY_NEW) {
            u8 memory = GetMonData(&gPlayerParty[partyIndex], (memorySlot == MON_MEMORY_OLD) ? MON_DATA_MEMORY_OLD : MON_DATA_MEMORY_NEW);

            if (memorySlot == MON_MEMORY_OLD && IsOtherTrainer(otId, otName))
            {
                StringCopy(gStringVar2, otName);
                StringExpandPlaceholders(gStringVar3, sMemoryStrings[memory][1]);
            }
            else
            {
                StringCopy(gStringVar3, MemoryStrings_FullStop);
            }

            StringExpandPlaceholders(gStringVar4, sMemoryStrings[memory][0]);
        }
        else 
        {
            const u8 *text;
            u8 metLoc = GetMonData(&gPlayerParty[partyIndex], MON_DATA_MET_LOCATION);
            u8 metLevel = GetMonData(&gPlayerParty[partyIndex], MON_DATA_MET_LEVEL);
            u8 metGame = GetMonData(&gPlayerParty[partyIndex], MON_DATA_MET_GAME);

            if (metLevel == 0)
                ConvertIntToDecimalStringN(gStringVar2, (P_EGG_HATCH_LEVEL >= GEN_4) ? 1 : 5, STR_CONV_MODE_LEFT_ALIGN, 3);
            else
                ConvertIntToDecimalStringN(gStringVar2, metLevel, STR_CONV_MODE_LEFT_ALIGN, 3);

            if (metLoc < MAPSEC_NONE && metLoc != METLOC_FATEFUL_ENCOUNTER)
            {
                GetMapNameHandleAquaHideout(gStringVar3, metLoc);
                StringExpandPlaceholders(gStringVar4, sText_AtLevelLocation);
                StringCopy(gStringVar2, gStringVar4);
            }

            StringCopy(gStringVar3, otName);

            if (!IsOtherTrainer(otId, otName))
            {
                if (metLevel == 0)
                    text = (metLoc >= MAPSEC_NONE) ? sText_HatchedSomewhereAt : sText_HatchedAtYZ;
                else
                    text = (metLoc >= MAPSEC_NONE) ? sText_MetSomewhereAt : sText_MetAtYZ;
            }
            else if (metLoc == METLOC_FATEFUL_ENCOUNTER)
            {
                text = sText_FatefulEncounter;
            }
            else if (metLoc != METLOC_IN_GAME_TRADE && metGame > 0 && metGame <= VERSION_LEAF_GREEN)
            {
                text = (metLoc >= MAPSEC_NONE) ? sText_ObtainedInTrade : gText_ProbablyMetAt;
            }
            else if (otId == 38726)
            {
                text = sText_KobesSeedot;
            }
            else if (otId == 73996)
            {
                text = sText_RomansPlusle;
            }
            else if (otId == 46285)
            {
                text = sText_SkylarsHorsea;
            }
            else if (otId == 91481)
            {
                text = sText_IsissMeowth;
            }
            else
            {
                text = sText_ObtainedInTrade;
            }

        StringExpandPlaceholders(gStringVar4, text);
        }
    }
}

static bool8 IsMemorySpecial(u8 memory)
{
    if (memory >= SPECIAL_MEMORY_START)
        return TRUE;
    return FALSE;
}

void SetMemoryWithRules(struct Pokemon *slot, u8 memory)
{
    u8 oldestMem = GetMonData(slot, MON_DATA_MEMORY_OLD);
    u8 newestMem = GetMonData(slot, MON_DATA_MEMORY_NEW);

    bool8 isOtherTrainer;
    u8 otName[PLAYER_NAME_LENGTH + 1];
    u32 otId = GetMonData(slot, MON_DATA_OT_ID);
    GetMonData(slot, MON_DATA_OT_NAME, otName);

    isOtherTrainer = IsOtherTrainer(otId, otName);

    if (memory != newestMem)
    {
        if (IsMemorySpecial(memory))
        {
            if (!isOtherTrainer)
            {
                if (!IsMemorySpecial(oldestMem) || (newestMem > oldestMem && memory > newestMem) || memory == oldestMem)
                {
                    SetMonData(slot, MON_DATA_MEMORY_OLD, &newestMem);
                    SetMonData(slot, MON_DATA_MEMORY_NEW, &memory);
                }
                else if (memory > newestMem || !IsMemorySpecial(newestMem))
                {
                    SetMonData(slot, MON_DATA_MEMORY_NEW, &memory);
                }
            }
            else
            {
                if (memory > newestMem || !IsMemorySpecial(newestMem))
                {
                    SetMonData(slot, MON_DATA_MEMORY_NEW, &memory);
                }
            }
        } 
        else 
        {
            if (!isOtherTrainer)
            {
                if (!IsMemorySpecial(oldestMem) || memory == oldestMem)
                {
                    SetMonData(slot, MON_DATA_MEMORY_OLD, &newestMem);
                    SetMonData(slot, MON_DATA_MEMORY_NEW, &memory);
                }
                else if (!IsMemorySpecial(newestMem))
                {
                    SetMonData(slot, MON_DATA_MEMORY_NEW, &memory);
                }
            }
            else if (!IsMemorySpecial(newestMem))
            {
                SetMonData(slot, MON_DATA_MEMORY_NEW, &memory);
            }
        }
    }
}

void SetMemory(struct ScriptContext *ctx)
{
    u8 memory = ScriptReadByte(ctx);
    u32 partyIndex = VarGet(ScriptReadHalfword(ctx));

    if (partyIndex < PARTY_SIZE)
        SetMemoryWithRules(&gPlayerParty[partyIndex], memory);
}

void SetMemoryAll(struct ScriptContext *ctx)
{
    u8 memory = ScriptReadByte(ctx);
    u32 i = 0;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG) != SPECIES_NONE
        && GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG) != SPECIES_EGG)
            SetMemoryWithRules(&gPlayerParty[i], memory);
    }
}

void ResolveMemoriesAfterTrade(u8 partyIdx)
{
    struct Pokemon *mon = &gPlayerParty[partyIdx];
    u8 oldestMem = GetMonData(mon, MON_DATA_MEMORY_OLD);
    u8 newestMem = GetMonData(mon, MON_DATA_MEMORY_NEW);
    u8 clearMem = 0;

    if (!GetMonData(mon, MON_DATA_IS_EGG))
    {
        if (newestMem > oldestMem)
            SetMonData(mon, MON_DATA_MEMORY_OLD, &newestMem);
        SetMonData(mon, MON_DATA_MEMORY_NEW, &clearMem);
    }
}

bool8 GiveMonTravellerRibbon(void)
{
    u8 hasTravellerRibbon;

    hasTravellerRibbon = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_TRAVELLER_RIBBON);
    if (!hasTravellerRibbon)
    {
        hasTravellerRibbon = 1;
        SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_TRAVELLER_RIBBON, &hasTravellerRibbon);
        if (GetRibbonCount(&gPlayerParty[gSpecialVar_0x8004]) > NUM_CUTIES_RIBBONS)
            TryPutSpotTheCutiesOnAir(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_TRAVELLER_RIBBON);
        FlagSet(FLAG_SYS_RIBBON_GET);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

bool8 GiveMonHistoricRibbon(void)
{
    u8 hasHistoricRibbon;

    hasHistoricRibbon = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HISTORIC_RIBBON);
    if (!hasHistoricRibbon && IsMemorySpecial(GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_MEMORY_OLD))
    && IsMemorySpecial(GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_MEMORY_NEW)))
    {
        hasHistoricRibbon = 1;
        SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HISTORIC_RIBBON, &hasHistoricRibbon);
        if (GetRibbonCount(&gPlayerParty[gSpecialVar_0x8004]) > NUM_CUTIES_RIBBONS)
            TryPutSpotTheCutiesOnAir(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_HISTORIC_RIBBON);
        FlagSet(FLAG_SYS_RIBBON_GET);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
