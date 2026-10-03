#include "global.h"
#include "battle.h"
#include "battle_script_commands.h"
#include "battle_setup.h"
#include "best_of_three_controller.h"
#include "data.h"
#include "event_data.h"
#include "event_scripts.h"
#include "gym_leader_rematch.h"
#include "level_caps.h"
#include "money.h"
#include "party_menu.h"
#include "pokemon.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "string_util.h"
#include "strings.h"
#include "constants/battle.h"
#include "constants/best_of_three.h"
#include "constants/flags.h"
#include "constants/opponents.h"

enum BestOfThreeConfigFlags
{
    BO3_CONFIG_LEAGUE   = (1 << 0),
    BO3_CONFIG_CHAMPION = (1 << 1),
};

struct BestOfThreeConfig
{
    u16 trainerId;
    u8 draftSize;
    u8 flags;
    const u8 *continueText;
    const u8 *roundWonText;
    const u8 *roundLostText;
};

struct BestOfThreeState
{
    const struct BestOfThreeConfig *config;
    const u8 *introText;
    const u8 *seriesWonText;
    const u8 *victoryScript;
    struct Pokemon fullParty[PARTY_SIZE];
    u32 terminalExpPool;
    u16 trainerId;
    u8 selectedOrder[MAX_FRONTIER_PARTY_SIZE];
    u8 fullPartyCount;
    u8 wins;
    u8 losses;
    u8 activeRound;
    u8 terminalResult;
    bool8 active;
    bool8 draftBattle;
    bool8 isRematch;
    bool8 snapshotValid;
    bool8 settlementApplied;
};

#define BO3_CONFIG(trainer, size, configFlags, cont, prefix)                                 \
    {                                                                                       \
        .trainerId = (trainer),                                                              \
        .draftSize = (size),                                                                 \
        .flags = (configFlags),                                                              \
        .continueText = (cont),                                                              \
        .roundWonText = prefix##Bo3RoundWon,                                                 \
        .roundLostText = prefix##Bo3RoundLost,                                               \
    }

static const struct BestOfThreeConfig sBestOfThreeConfigs[] =
{
    BO3_CONFIG(TRAINER_ROXANNE_1, 3, 0,
               RustboroCity_Gym_Text_RoxanneContinue, RustboroCity_Gym_Text_Roxanne),
    BO3_CONFIG(TRAINER_BRAWLY_1, 3, 0,
               DewfordTown_Gym_Text_BrawlyContinue, DewfordTown_Gym_Text_Brawly),
    BO3_CONFIG(TRAINER_WATTSON_1, 4, 0,
               MauvilleCity_Gym_Text_WattsonContinue, MauvilleCity_Gym_Text_Wattson),
    BO3_CONFIG(TRAINER_FLANNERY_1, 4, 0,
               LavaridgeTown_Gym_1F_Text_FlanneryContinue, LavaridgeTown_Gym_1F_Text_Flannery),
    BO3_CONFIG(TRAINER_NORMAN_1, 4, 0,
               PetalburgCity_Gym_Text_NormanContinue, PetalburgCity_Gym_Text_Norman),
    BO3_CONFIG(TRAINER_WINONA_1, 4, 0,
               FortreeCity_Gym_Text_WinonaContinue, FortreeCity_Gym_Text_Winona),
    BO3_CONFIG(TRAINER_TATE_AND_LIZA_1, 4, 0,
               MossdeepCity_Gym_Text_TateAndLizaContinue, MossdeepCity_Gym_Text_TateAndLiza),
    BO3_CONFIG(TRAINER_WALLACE, 4, 0,
               SootopolisCity_Gym_1F_Text_WallaceContinue, SootopolisCity_Gym_1F_Text_Wallace),
    BO3_CONFIG(TRAINER_SIDNEY, 4, BO3_CONFIG_LEAGUE,
               EverGrandeCity_SidneysRoom_Text_Bo3Continue, EverGrandeCity_SidneysRoom_Text_),
    BO3_CONFIG(TRAINER_PHOEBE, 4, BO3_CONFIG_LEAGUE,
               EverGrandeCity_PhoebesRoom_Text_Bo3Continue, EverGrandeCity_PhoebesRoom_Text_),
    BO3_CONFIG(TRAINER_GLACIA, 4, BO3_CONFIG_LEAGUE,
               EverGrandeCity_GlaciasRoom_Text_Bo3Continue, EverGrandeCity_GlaciasRoom_Text_),
    BO3_CONFIG(TRAINER_DRAKE, 4, BO3_CONFIG_LEAGUE,
               EverGrandeCity_DrakesRoom_Text_Bo3Continue, EverGrandeCity_DrakesRoom_Text_),
    BO3_CONFIG(TRAINER_STEVEN_CHAMPION, 4, BO3_CONFIG_LEAGUE | BO3_CONFIG_CHAMPION,
               EverGrandeCity_ChampionsRoom_Text_StevenBo3Continue, EverGrandeCity_ChampionsRoom_Text_Steven),
    BO3_CONFIG(TRAINER_WALLACE_CHAMPION, 4, BO3_CONFIG_LEAGUE | BO3_CONFIG_CHAMPION,
               EverGrandeCity_ChampionsRoom_Text_WallaceBo3Continue, EverGrandeCity_ChampionsRoom_Text_Wallace),
};

static const struct BestOfThreeConfig sJuanRematchConfig =
    BO3_CONFIG(TRAINER_WALLACE, 4, 0,
               SootopolisCity_Gym_1F_Text_JuanContinue, SootopolisCity_Gym_1F_Text_Juan);

#undef BO3_CONFIG

static EWRAM_DATA struct BestOfThreeState sBestOfThreeState = {0};

static const struct BestOfThreeConfig *FindBestOfThreeConfig(u16 trainerId)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sBestOfThreeConfigs); i++)
    {
        if (sBestOfThreeConfigs[i].trainerId == trainerId)
            return &sBestOfThreeConfigs[i];
    }

    return NULL;
}

static const struct BestOfThreeConfig *FindBestOfThreeRematchConfig(u16 trainerId)
{
    s32 rematchId = TrainerIdToRematchTableId(gRematchTable, trainerId);

    if (rematchId < 0 || gRematchTable[rematchId].trainerIds[0] == trainerId)
        return NULL;
    if (rematchId == REMATCH_JUAN)
        return &sJuanRematchConfig;

    return FindBestOfThreeConfig(gRematchTable[rematchId].trainerIds[0]);
}

static void RestoreFullParty(void)
{
    u32 i;

    if (!sBestOfThreeState.snapshotValid)
        return;

    gPlayerPartyCount = sBestOfThreeState.fullPartyCount;
    for (i = 0; i < PARTY_SIZE; i++)
        gPlayerParty[i] = sBestOfThreeState.fullParty[i];
    sBestOfThreeState.snapshotValid = FALSE;
}

static u32 GetGeneratedEnemyExpPool(void)
{
    u32 i;
    u32 pool = 0;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u32 species = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES);
        u32 level = GetMonData(&gEnemyParty[i], MON_DATA_LEVEL);
        u32 exp;

        if (species == SPECIES_NONE || species == SPECIES_EGG)
            continue;

        exp = gSpeciesInfo[species].expYield * level;
        if (B_SCALED_EXP >= GEN_5 && B_SCALED_EXP != GEN_6)
            exp /= 5;
        else
            exp /= 7;

        if (B_TRAINER_EXP_MULTIPLIER <= GEN_7)
            exp = (exp * 150) / 100;

        pool += exp;
    }

    return pool;
}

void BestOfThree_ApplyTerminalExp(void)
{
    u32 i;
    u32 eligibleCount = 0;
    u32 scaledPool;
    u32 share;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u32 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);
        u32 level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);

        if (species != SPECIES_NONE && species != SPECIES_EGG && level < MAX_LEVEL)
            eligibleCount++;
    }

    if (eligibleCount == 0 || sBestOfThreeState.terminalExpPool == 0)
        return;

    scaledPool = sBestOfThreeState.terminalExpPool
               * (sBestOfThreeState.terminalResult == BO3_SERIES_WON ? 10 : 5)
               / 100;
    share = scaledPool / eligibleCount;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u32 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);
        u32 level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        u32 currentExp;
        u32 nextLevelExp;
        u32 expToGive;
        u32 growthRate;

        if (species == SPECIES_NONE || species == SPECIES_EGG || level >= MAX_LEVEL)
            continue;

        currentExp = GetMonData(&gPlayerParty[i], MON_DATA_EXP);
        growthRate = gSpeciesInfo[species].growthRate;
        nextLevelExp = gExperienceTables[growthRate][level + 1];
        expToGive = GetSoftLevelCapExpValue(level, share);

        if (currentExp >= nextLevelExp - 1)
            continue;
        if (expToGive > nextLevelExp - 1 - currentExp)
            expToGive = nextLevelExp - 1 - currentExp;

        currentExp += expToGive;
        SetMonData(&gPlayerParty[i], MON_DATA_EXP, &currentExp);
    }
}

static u8 RecordRoundOutcome(u32 battleOutcome)
{
    sBestOfThreeState.activeRound++;

    if (battleOutcome == B_OUTCOME_WON)
        sBestOfThreeState.wins++;
    else if (battleOutcome == B_OUTCOME_LOST || battleOutcome == B_OUTCOME_DREW)
        sBestOfThreeState.losses++;
    else
        return BO3_ROUND_ABORTED;

    if (sBestOfThreeState.wins == 2)
        return sBestOfThreeState.terminalResult = BO3_SERIES_WON;
    if (sBestOfThreeState.losses == 2)
        return sBestOfThreeState.terminalResult = BO3_SERIES_LOST;

    return BO3_ROUND_CONTINUE;
}

bool32 BestOfThree_IsSupportedTrainer(u16 trainerId)
{
    return FindBestOfThreeConfig(trainerId) != NULL
        || FindBestOfThreeRematchConfig(trainerId) != NULL;
}

bool32 BestOfThree_IsTrainerEligible(u16 trainerId)
{
    const struct BestOfThreeConfig *config = FindBestOfThreeConfig(trainerId);

    if (config != NULL)
    {
        if (config->flags & BO3_CONFIG_LEAGUE)
            return TRUE;

        return !HasTrainerBeenFought(trainerId);
    }

    if (FindBestOfThreeRematchConfig(trainerId) != NULL)
        return TRUE;

    return FALSE;
}

static const u8 *Initialize(const struct BestOfThreeConfig *config, u16 trainerId,
                            const u8 *introText, const u8 *seriesWonText,
                            const u8 *victoryScript, bool32 isRematch,
                            bool32 bestOfThree)
{
    memset(&sBestOfThreeState, 0, sizeof(sBestOfThreeState));
    sBestOfThreeState.config = config;
    sBestOfThreeState.introText = introText;
    sBestOfThreeState.seriesWonText = seriesWonText;
    sBestOfThreeState.victoryScript = victoryScript;
    sBestOfThreeState.trainerId = trainerId;
    sBestOfThreeState.active = TRUE;
    sBestOfThreeState.draftBattle = !bestOfThree;
    sBestOfThreeState.isRematch = isRematch;

    return bestOfThree ? EventScript_BestOfThreeStart : EventScript_DraftBattleStart;
}

const u8 *BestOfThree_TryInitialize(u16 trainerId, const u8 *introText,
                                    const u8 *seriesWonText, const u8 *victoryScript,
                                    bool32 bestOfThree)
{
    const struct BestOfThreeConfig *config = FindBestOfThreeConfig(trainerId);

    if (config == NULL || !BestOfThree_IsTrainerEligible(trainerId))
        return NULL;

    return Initialize(config, trainerId, introText, seriesWonText, victoryScript,
                      FALSE, bestOfThree);
}

const u8 *BestOfThree_TryInitializeRematch(u16 trainerId, const u8 *introText,
                                           const u8 *seriesWonText, const u8 *victoryScript,
                                           bool32 bestOfThree)
{
    const struct BestOfThreeConfig *config = FindBestOfThreeRematchConfig(trainerId);

    if (config == NULL)
        return NULL;

    return Initialize(config, trainerId, introText, seriesWonText, victoryScript, TRUE, bestOfThree);
}

bool32 BestOfThree_IsActive(void)
{
    return sBestOfThreeState.active && !sBestOfThreeState.draftBattle;
}

bool32 BestOfThree_IsDraftBattle(void)
{
    return sBestOfThreeState.active && sBestOfThreeState.draftBattle;
}

bool32 BestOfThree_IsChampion(void)
{
    return sBestOfThreeState.active
        && (sBestOfThreeState.config->flags & BO3_CONFIG_CHAMPION);
}

bool32 BestOfThree_IsTerminalOutcome(u32 battleOutcome)
{
    if (!BestOfThree_IsActive())
        return FALSE;
    if (battleOutcome == B_OUTCOME_WON)
        return sBestOfThreeState.wins == 1;
    if (battleOutcome == B_OUTCOME_LOST || battleOutcome == B_OUTCOME_DREW)
        return sBestOfThreeState.losses == 1;

    return FALSE;
}

const u8 *BestOfThree_GetBattleText(bool32 playerWon)
{
    if (!BestOfThree_IsActive())
        return gText_EmptyString2;

    if (playerWon)
    {
        if (sBestOfThreeState.wins == 1)
            return sBestOfThreeState.seriesWonText;
        return sBestOfThreeState.config->roundWonText;
    }

    return sBestOfThreeState.config->roundLostText;
}

u32 BestOfThree_ApplyMoneySettlement(u32 battleOutcome, u32 moneyMultiplier)
{
    static const u16 sWhiteOutBadgeMoney[9] = {8, 16, 24, 36, 48, 64, 80, 100, 120};
    u32 amount = 0;

    if (!BestOfThree_IsTerminalOutcome(battleOutcome)
     || sBestOfThreeState.settlementApplied)
        return 0;

    sBestOfThreeState.terminalExpPool = GetGeneratedEnemyExpPool();
    sBestOfThreeState.settlementApplied = TRUE;

    if (battleOutcome == B_OUTCOME_WON)
    {
        const struct TrainerMon *party = GetTrainerPartyFromId(sBestOfThreeState.trainerId);
        u32 partySize = GetTrainerPartySizeFromId(sBestOfThreeState.trainerId);
        u32 trainerMoney = gTrainerClasses[GetTrainerClassFromId(sBestOfThreeState.trainerId)].money;

        if (party != NULL && partySize != 0)
        {
            amount = 4 * party[partySize - 1].lvl * moneyMultiplier * trainerMoney;
            if (IsTrainerDoubleBattle(sBestOfThreeState.trainerId))
                amount *= 2;
        }
        AddMoney(&gSaveBlock1Ptr->money, amount);
    }
    else
    {
#if B_WHITEOUT_MONEY >= GEN_4
        u32 i;
        u32 badgeCount = 0;
        u32 highestLevel = 1;

        for (i = 0; i < NUM_BADGES; i++)
        {
            if (FlagGet(FLAG_BADGE01_GET + i))
                badgeCount++;
        }
        for (i = 0; i < PARTY_SIZE; i++)
        {
            u32 species = GetMonData(&sBestOfThreeState.fullParty[i], MON_DATA_SPECIES_OR_EGG);
            u32 level = GetMonData(&sBestOfThreeState.fullParty[i], MON_DATA_LEVEL);

            if (species != SPECIES_NONE && species != SPECIES_EGG && level > highestLevel)
                highestLevel = level;
        }
        amount = sWhiteOutBadgeMoney[badgeCount] * highestLevel;
#else
        amount = GetMoney(&gSaveBlock1Ptr->money) / 2;
#endif
#if B_WHITEOUT_MONEY >= GEN_4
        RemoveMoney(&gSaveBlock1Ptr->money, amount);
#endif
    }

    return amount;
}

void BestOfThree_BufferIntroText(void)
{
    StringExpandPlaceholders(gStringVar4, sBestOfThreeState.introText);
}

void BestOfThree_BufferContinueText(void)
{
    StringExpandPlaceholders(gStringVar4, sBestOfThreeState.config->continueText);
}

void BestOfThree_SetDraftSize(void)
{
    gSpecialVar_0x8005 = sBestOfThreeState.config->draftSize;
}

void BestOfThree_PrepareParty(void)
{
    u32 i;

    sBestOfThreeState.fullPartyCount = gPlayerPartyCount;
    for (i = 0; i < PARTY_SIZE; i++)
        sBestOfThreeState.fullParty[i] = gPlayerParty[i];
    for (i = 0; i < MAX_FRONTIER_PARTY_SIZE; i++)
        sBestOfThreeState.selectedOrder[i] = gSelectedOrderFromParty[i];
    sBestOfThreeState.snapshotValid = TRUE;
    ReducePlayerPartyToSelectedMons();
}

void BestOfThree_StartBattle(void)
{
    BattleSetup_StartConfiguredTrainerBattle(sBestOfThreeState.trainerId,
                                              sBestOfThreeState.draftBattle
                                                ? sBestOfThreeState.seriesWonText
                                                : sBestOfThreeState.config->roundWonText,
                                              sBestOfThreeState.victoryScript,
                                              !sBestOfThreeState.draftBattle);
}

void BestOfThree_CompleteRound(void)
{
    u8 result;

    RestoreFullParty();
    result = RecordRoundOutcome(gBattleOutcome);

    if (result == BO3_SERIES_WON || result == BO3_SERIES_LOST)
        BestOfThree_ApplyTerminalExp();
    if (result == BO3_SERIES_WON)
    {
        TryGivePickupItemsToParty();
        BestOfThree_RegisterVictory();
    }
    if (result == BO3_ROUND_ABORTED)
        BestOfThree_Abort();

    gSpecialVar_Result = result;
}

bool32 BestOfThree_ReconcileDraftParty(void)
{
    u32 i;
    u32 j;
    u32 draftSize;

    if (!BestOfThree_IsDraftBattle()
     || !sBestOfThreeState.snapshotValid
     || sBestOfThreeState.config == NULL)
        return FALSE;

    draftSize = sBestOfThreeState.config->draftSize;
    if (draftSize > MAX_FRONTIER_PARTY_SIZE)
    {
        RestoreFullParty();
        return FALSE;
    }

    for (i = 0; i < draftSize; i++)
    {
        u32 selectedSlot = sBestOfThreeState.selectedOrder[i];

        if (selectedSlot == 0
         || selectedSlot > sBestOfThreeState.fullPartyCount
         || selectedSlot > PARTY_SIZE)
        {
            RestoreFullParty();
            return FALSE;
        }

        for (j = 0; j < i; j++)
        {
            if (selectedSlot == sBestOfThreeState.selectedOrder[j])
            {
                RestoreFullParty();
                return FALSE;
            }
        }
    }

    for (i = 0; i < draftSize; i++)
        sBestOfThreeState.fullParty[sBestOfThreeState.selectedOrder[i] - 1] = gPlayerParty[i];

    RestoreFullParty();
    return TRUE;
}

void BestOfThree_RegisterVictory(void)
{
    BattleSetup_RegisterBestOfThreeVictory(sBestOfThreeState.trainerId,
                                           sBestOfThreeState.isRematch);
}

void BestOfThree_Abort(void)
{
    RestoreFullParty();
    memset(&sBestOfThreeState, 0, sizeof(sBestOfThreeState));
}

void BestOfThree_Finish(void)
{
    memset(&sBestOfThreeState, 0, sizeof(sBestOfThreeState));
}

void BestOfThree_GetCancelPolicy(void)
{
    bool32 terminateLeague = BestOfThree_IsChampion();

    BestOfThree_Abort();
    gSpecialVar_Result = terminateLeague ? BO3_CANCEL_WHITEOUT : BO3_CANCEL_ABORT;
}

#if TESTING
u8 BestOfThree_GetWins(void)
{
    return sBestOfThreeState.wins;
}

u8 BestOfThree_GetLosses(void)
{
    return sBestOfThreeState.losses;
}

u8 BestOfThree_RecordOutcomeForTest(u32 battleOutcome)
{
    return RecordRoundOutcome(battleOutcome);
}

const u8 *BestOfThree_GetVictoryScriptForTest(void)
{
    return sBestOfThreeState.victoryScript;
}
#endif
