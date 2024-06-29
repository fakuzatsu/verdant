#include "global.h"
#include "event_data.h"
#include "pokemon.h"
#include "script.h"
#include "string_util.h"

extern const u8 MemoryStrings_NoMemory[];
//extern const u8 MemoryStrings_
//extern const u8 MemoryStrings_
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
//extern const u8 MemoryStrings_
extern const u8 MemoryStrings_EarnedGoldFrontier[];

static const u8 MemoryStrings_FullStop[] = _(".");
static const u8 MemoryStrings_[] = _(".");

extern const u8 MemoryStrings_WithX[];
extern const u8 MemoryStrings_GivenByX[];

static const u8 *const sMemoryStrings[TOTAL_MEMORIES][2] = {
    {MemoryStrings_NoMemory, MemoryStrings_NoMemory},
    {MemoryStrings_, MemoryStrings_},
    {MemoryStrings_, MemoryStrings_},
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
    {MemoryStrings_, MemoryStrings_},
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
    u8 otId = 0;
    u8 memory = 0;

    if (partyIndex < PARTY_SIZE)
    {
        GetMonData(&gPlayerParty[partyIndex], MON_DATA_OT_NAME, otName);
        otId = GetMonData(&gPlayerParty[partyIndex], MON_DATA_OT_ID, 0);
        memory = GetMonData(&gPlayerParty[partyIndex], (memorySlot == MON_MEMORY_OLD) ? MON_DATA_MEMORY_OLD : MON_DATA_MEMORY_NEW);

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