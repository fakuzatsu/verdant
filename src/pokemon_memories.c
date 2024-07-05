#include "global.h"
#include "dynamic_placeholder_text_util.h"
#include "event_data.h"
#include "malloc.h"
#include "pokemon.h"
#include "region_map.h"
#include "script.h"
#include "string_util.h"

static const u8 sText_MetAtYZ[] = _("{STR_VAR_1} was met at {LV_2}{DYNAMIC 0}{DYNAMIC 3}{DYNAMIC 1},\n{DYNAMIC 0}{DYNAMIC 4}{DYNAMIC 1}.");
static const u8 sText_HatchedAtYZ[] = _("{STR_VAR_1} hatched at {LV_2}{DYNAMIC 0}{DYNAMIC 3}{DYNAMIC 1},\n{DYNAMIC 0}{DYNAMIC 4}{DYNAMIC 1}.");
static const u8 sText_ObtainedInTrade[] = _("{STR_VAR_1} was obtained in a trade.");
static const u8 sText_FatefulEncounter[] = _("{STR_VAR_1} was obtained in a fateful\nencounter at {LV_2}{DYNAMIC 0}{DYNAMIC 3}{DYNAMIC 1}.");
static const u8 sText_MetSomewhereAt[] = _("{STR_VAR_1} was met somewhere at {LV_2}\n{DYNAMIC 0}{DYNAMIC 3}{DYNAMIC 1}.");
static const u8 sText_HatchedSomewhereAt[] = _("{STR_VAR_1} hatched somewhere at\n{LV_2}{DYNAMIC 0}{DYNAMIC 3}{DYNAMIC 1}.");

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
extern const u8 MemoryStrings_WithX[];
extern const u8 MemoryStrings_GivenByX[];
extern const u8 MemoryStrings_ByX[];

static const u8 MemoryStrings_FullStop[] = _(".");

static const u8 *const sMemoryStrings[TOTAL_MEMORIES][2] = {
    {MemoryStrings_NoMemory, MemoryStrings_NoMemory},
    {MemoryStrings_TransferedOverWonderTrade, MemoryStrings_ByX},
    {MemoryStrings_TransferedOverLinkTrade, MemoryStrings_ByX},
    {MemoryStrings_WonACoolContest, MemoryStrings_WithX},
    {MemoryStrings_WonABeautyContest, MemoryStrings_WithX},
    {MemoryStrings_WonACuteContest, MemoryStrings_WithX},
    {MemoryStrings_WonACleverContest, MemoryStrings_WithX},
    {MemoryStrings_WonAToughContest, MemoryStrings_WithX},
    {MemoryStrings_AteACoolPokeblock, MemoryStrings_GivenByX},
    {MemoryStrings_AteABeautyPokeblock, MemoryStrings_GivenByX},
    {MemoryStrings_AteACutePokeblock, MemoryStrings_GivenByX},
    {MemoryStrings_AteACleverPokeblock, MemoryStrings_GivenByX},
    {MemoryStrings_AteAToughPokeblock, MemoryStrings_GivenByX},
    {MemoryStrings_AteADislikedPokeblock, MemoryStrings_GivenByX},
    {MemoryStrings_AteALovedPokeblock, MemoryStrings_GivenByX},
    {MemoryStrings_MaxedOutSheen, MemoryStrings_GivenByX},
    {MemoryStrings_FoughtRoxanneGym, MemoryStrings_WithX},
    {MemoryStrings_FoughtBrawlyGym, MemoryStrings_WithX},
    {MemoryStrings_FoughtWattsonGym, MemoryStrings_WithX},
    {MemoryStrings_FoughtFlanneryGym, MemoryStrings_WithX},
    {MemoryStrings_FoughtNormanGym, MemoryStrings_WithX},
    {MemoryStrings_FoughtWinonaGym, MemoryStrings_WithX},
    {MemoryStrings_FoughtTateAndLizaGym, MemoryStrings_WithX},
    {MemoryStrings_FoughtWallaceGym, MemoryStrings_WithX},
    {MemoryStrings_FoughtTheEliteFour, MemoryStrings_WithX},
    {MemoryStrings_FoughtASignificant, MemoryStrings_WithX},
    {MemoryStrings_EarnedSilverFrontier, MemoryStrings_WithX},
    {MemoryStrings_CaughtALegendary, MemoryStrings_WithX},
    {MemoryStrings_DefendedChampionTitle, MemoryStrings_WithX},
    {MemoryStrings_WonAMasterContest, MemoryStrings_WithX},
    {MemoryStrings_SavedTheWorld, MemoryStrings_WithX},
    {MemoryStrings_EarnedGoldFrontier, MemoryStrings_WithX},
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
        if (memorySlot == MON_DATA_MEMORY_OLD || memorySlot == MON_DATA_MEMORY_NEW) {
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
            u8 *metLevelString = Alloc(32);
            u8 *metLocationString = Alloc(32);
            u8 metLoc = GetMonData(&gPlayerParty[partyIndex], MON_DATA_MET_LOCATION, NULL);
            u8 metLevel = GetMonData(&gPlayerParty[partyIndex], MON_DATA_MET_LEVEL, NULL);

            if (metLevel == 0)
                metLevel = (P_EGG_HATCH_LEVEL >= GEN_4) ? 1 : 5;
            ConvertIntToDecimalStringN(metLevelString, metLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
            DynamicPlaceholderTextUtil_SetPlaceholderPtr(3, metLevelString);

            if (metLoc < MAPSEC_NONE)
            {
                GetMapNameHandleAquaHideout(metLocationString, metLoc);
                DynamicPlaceholderTextUtil_SetPlaceholderPtr(4, metLocationString);
            }

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
            else
            {
                text = sText_ObtainedInTrade;
            }

        DynamicPlaceholderTextUtil_ExpandPlaceholders(gStringVar4, text);

        Free(metLevelString);
        Free(metLocationString);
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

    if (memory != newestMem)
    {
        if (IsMemorySpecial(memory))
        {
            if (!IsMemorySpecial(oldestMem) || memory > oldestMem)
                SetMonData(slot, MON_DATA_MEMORY_OLD, &newestMem);
            if (!IsMemorySpecial(newestMem) || memory > newestMem)
                SetMonData(slot, MON_DATA_MEMORY_NEW, &memory);
        } 
        else 
        {
            if (!IsMemorySpecial(oldestMem))
                SetMonData(slot, MON_DATA_MEMORY_OLD, &newestMem);
            if (!IsMemorySpecial(newestMem) || !IsMemorySpecial(oldestMem))
                SetMonData(slot, MON_DATA_MEMORY_NEW, &memory);
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