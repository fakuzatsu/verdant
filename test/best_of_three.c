#include "global.h"
#include "battle.h"
#include "battle_script_commands.h"
#include "battle_setup.h"
#include "best_of_three_controller.h"
#include "data.h"
#include "event_data.h"
#include "event_scripts.h"
#include "money.h"
#include "party_menu.h"
#include "pokemon.h"
#include "script.h"
#include "string_util.h"
#include "test/test.h"
#include "constants/best_of_three.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/pokemon.h"
#include "constants/species.h"

extern const u8 RustboroCity_Gym_Text_RoxanneIntro[];
extern const u8 RustboroCity_Gym_Text_RoxanneDefeat[];
extern const u8 RustboroCity_Gym_EventScript_RoxanneDefeated[];
extern const u8 RustboroCity_Gym_Text_RoxannePreRematch[];
extern const u8 RustboroCity_Gym_Text_RoxanneRematchDefeat[];
extern const u8 RustboroCity_Gym_EventScript_RoxanneRematchDefeated[];
extern const u8 SootopolisCity_Gym_1F_Text_WallaceIntro[];
extern const u8 SootopolisCity_Gym_1F_Text_WallaceDefeat[];
extern const u8 SootopolisCity_Gym_1F_EventScript_WallaceOrJuanDefeated[];
extern const u8 SootopolisCity_Gym_1F_Text_JuanPreRematch[];
extern const u8 SootopolisCity_Gym_1F_Text_JuanRematchDefeat[];
extern const u8 SootopolisCity_Gym_1F_EventScript_JuanRematchDefeated[];
extern const u8 EverGrandeCity_ChampionsRoom_Text_WallaceIntroSpeech[];
extern const u8 EverGrandeCity_ChampionsRoom_Text_WallaceDefeat[];
extern const u8 EverGrandeCity_ChampionsRoom_EventScript_Defeated[];

static const u8 *InitializeRoxanne(bool32 bestOfThree)
{
    return BestOfThree_TryInitialize(TRAINER_ROXANNE_1,
                                     RustboroCity_Gym_Text_RoxanneIntro,
                                     RustboroCity_Gym_Text_RoxanneDefeat,
                                     RustboroCity_Gym_EventScript_RoxanneDefeated,
                                     bestOfThree);
}

static const u8 *InitializeWallaceChampion(bool32 bestOfThree)
{
    return BestOfThree_TryInitialize(TRAINER_WALLACE_CHAMPION,
                                     EverGrandeCity_ChampionsRoom_Text_WallaceIntroSpeech,
                                     EverGrandeCity_ChampionsRoom_Text_WallaceDefeat,
                                     EverGrandeCity_ChampionsRoom_EventScript_Defeated,
                                     bestOfThree);
}

TEST("BO3: Supported trainer lookup and eligibility use full trainer IDs")
{
    static const u16 rematchTrainers[] =
    {
        TRAINER_ROXANNE_2, TRAINER_ROXANNE_3, TRAINER_ROXANNE_4, TRAINER_ROXANNE_5,
        TRAINER_BRAWLY_2, TRAINER_BRAWLY_3, TRAINER_BRAWLY_4, TRAINER_BRAWLY_5,
        TRAINER_WATTSON_2, TRAINER_WATTSON_3, TRAINER_WATTSON_4, TRAINER_WATTSON_5,
        TRAINER_FLANNERY_2, TRAINER_FLANNERY_3, TRAINER_FLANNERY_4, TRAINER_FLANNERY_5,
        TRAINER_NORMAN_2, TRAINER_NORMAN_3, TRAINER_NORMAN_4, TRAINER_NORMAN_5,
        TRAINER_WINONA_2, TRAINER_WINONA_3, TRAINER_WINONA_4, TRAINER_WINONA_5,
        TRAINER_TATE_AND_LIZA_2, TRAINER_TATE_AND_LIZA_3, TRAINER_TATE_AND_LIZA_4, TRAINER_TATE_AND_LIZA_5,
        TRAINER_JUAN_2, TRAINER_JUAN_3, TRAINER_JUAN_4, TRAINER_JUAN_5,
    };
    u32 i;

    ClearTrainerFlag(TRAINER_ROXANNE_1);
    ClearTrainerFlag(TRAINER_WALLACE_CHAMPION);

    EXPECT(BestOfThree_IsSupportedTrainer(TRAINER_WALLACE_CHAMPION));
    EXPECT(TRAINER_WALLACE_CHAMPION > 255);
    EXPECT(!BestOfThree_IsSupportedTrainer(TRAINER_WALLACE_CHAMPION + 1));
    EXPECT(BestOfThree_IsTrainerEligible(TRAINER_ROXANNE_1));

    SetTrainerFlag(TRAINER_ROXANNE_1);
    SetTrainerFlag(TRAINER_WALLACE_CHAMPION);
    EXPECT(!BestOfThree_IsTrainerEligible(TRAINER_ROXANNE_1));
    EXPECT(BestOfThree_IsTrainerEligible(TRAINER_WALLACE_CHAMPION));

    for (i = 0; i < ARRAY_COUNT(rematchTrainers); i++)
    {
        SetTrainerFlag(rematchTrainers[i]);
        EXPECT(BestOfThree_IsSupportedTrainer(rematchTrainers[i]));
        EXPECT(BestOfThree_IsTrainerEligible(rematchTrainers[i]));
        ClearTrainerFlag(rematchTrainers[i]);
    }

    ClearTrainerFlag(TRAINER_ROXANNE_1);
    ClearTrainerFlag(TRAINER_WALLACE_CHAMPION);
}

TEST("BO3: Only best-of-three battle flags suppress regular EXP")
{
    u32 savedBattleTypeFlags = gBattleTypeFlags;

    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    EXPECT(BattleTypeAllowsExpForTest());
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BEST_OF_THREE;
    EXPECT(!BattleTypeAllowsExpForTest());

    gBattleTypeFlags = savedBattleTypeFlags;
}

TEST("BO3: All meaningful score sequences settle on the second result")
{
    static const struct
    {
        u8 outcomes[3];
        u8 count;
        u8 result;
        u8 wins;
        u8 losses;
    } cases[] =
    {
        {{B_OUTCOME_WON,  B_OUTCOME_WON},                    2, BO3_SERIES_WON,  2, 0},
        {{B_OUTCOME_WON,  B_OUTCOME_LOST, B_OUTCOME_WON},    3, BO3_SERIES_WON,  2, 1},
        {{B_OUTCOME_LOST, B_OUTCOME_WON,  B_OUTCOME_WON},    3, BO3_SERIES_WON,  2, 1},
        {{B_OUTCOME_LOST, B_OUTCOME_LOST},                   2, BO3_SERIES_LOST, 0, 2},
        {{B_OUTCOME_WON,  B_OUTCOME_LOST, B_OUTCOME_LOST},   3, BO3_SERIES_LOST, 1, 2},
        {{B_OUTCOME_LOST, B_OUTCOME_WON,  B_OUTCOME_LOST},   3, BO3_SERIES_LOST, 1, 2},
    };
    u32 i;

    for (i = 0; i < ARRAY_COUNT(cases); i++)
    {
        u32 round;
        u8 result = BO3_ROUND_ABORTED;

        EXPECT(InitializeWallaceChampion(TRUE) != NULL);
        for (round = 0; round < cases[i].count; round++)
        {
            result = BestOfThree_RecordOutcomeForTest(cases[i].outcomes[round]);
            if (round + 1 < cases[i].count)
                EXPECT_EQ(result, BO3_ROUND_CONTINUE);
        }
        EXPECT_EQ(result, cases[i].result);
        EXPECT_EQ(BestOfThree_GetWins(), cases[i].wins);
        EXPECT_EQ(BestOfThree_GetLosses(), cases[i].losses);
        BestOfThree_Finish();
    }
}

TEST("BO3: Battle dialogue follows the pre-round score and outcome")
{
    ClearTrainerFlag(TRAINER_ROXANNE_1);
    EXPECT(InitializeRoxanne(TRUE) != NULL);

    EXPECT(BestOfThree_GetBattleText(TRUE) == RustboroCity_Gym_Text_RoxanneBo3RoundWon);
    EXPECT(BestOfThree_GetBattleText(FALSE) == RustboroCity_Gym_Text_RoxanneBo3RoundLost);
    EXPECT_EQ(BestOfThree_RecordOutcomeForTest(B_OUTCOME_WON), BO3_ROUND_CONTINUE);
    EXPECT(BestOfThree_GetBattleText(TRUE) == RustboroCity_Gym_Text_RoxanneDefeat);
    EXPECT(BestOfThree_GetVictoryScriptForTest() == RustboroCity_Gym_EventScript_RoxanneDefeated);
    EXPECT(BestOfThree_GetBattleText(FALSE) == RustboroCity_Gym_Text_RoxanneBo3RoundLost);
    EXPECT_EQ(BestOfThree_RecordOutcomeForTest(B_OUTCOME_LOST), BO3_ROUND_CONTINUE);
    EXPECT(BestOfThree_GetBattleText(TRUE) == RustboroCity_Gym_Text_RoxanneDefeat);
    EXPECT(BestOfThree_GetBattleText(FALSE) == RustboroCity_Gym_Text_RoxanneBo3RoundLost);

    BestOfThree_Finish();

    EXPECT(InitializeRoxanne(TRUE) != NULL);
    EXPECT_EQ(BestOfThree_RecordOutcomeForTest(B_OUTCOME_LOST), BO3_ROUND_CONTINUE);
    EXPECT_EQ(BestOfThree_RecordOutcomeForTest(B_OUTCOME_WON), BO3_ROUND_CONTINUE);
    EXPECT(BestOfThree_GetBattleText(TRUE) == RustboroCity_Gym_Text_RoxanneDefeat);
    EXPECT(BestOfThree_GetBattleText(FALSE) == RustboroCity_Gym_Text_RoxanneBo3RoundLost);
    BestOfThree_Finish();
}

TEST("BO3: Juan rematches use Juan round dialogue while Wallace's story battle does not")
{
    ClearTrainerFlag(TRAINER_WALLACE);
    EXPECT(BestOfThree_TryInitialize(TRAINER_WALLACE,
                                     SootopolisCity_Gym_1F_Text_WallaceIntro,
                                     SootopolisCity_Gym_1F_Text_WallaceDefeat,
                                     SootopolisCity_Gym_1F_EventScript_WallaceOrJuanDefeated,
                                     TRUE) != NULL);
    EXPECT(BestOfThree_GetBattleText(TRUE) == SootopolisCity_Gym_1F_Text_WallaceBo3RoundWon);
    EXPECT(BestOfThree_GetBattleText(FALSE) == SootopolisCity_Gym_1F_Text_WallaceBo3RoundLost);
    BestOfThree_Finish();

    EXPECT(BestOfThree_TryInitializeRematch(TRAINER_JUAN_2,
                                            SootopolisCity_Gym_1F_Text_JuanPreRematch,
                                            SootopolisCity_Gym_1F_Text_JuanRematchDefeat,
                                            SootopolisCity_Gym_1F_EventScript_JuanRematchDefeated,
                                            TRUE) != NULL);
    EXPECT(BestOfThree_GetBattleText(TRUE) == SootopolisCity_Gym_1F_Text_JuanBo3RoundWon);
    EXPECT(BestOfThree_GetBattleText(FALSE) == SootopolisCity_Gym_1F_Text_JuanBo3RoundLost);
    BestOfThree_BufferContinueText();
    EXPECT_EQ(StringCompare(gStringVar4, SootopolisCity_Gym_1F_Text_JuanContinue), 0);
    BestOfThree_Finish();
}

TEST("Draft battle: Post-battle state is merged into the originally selected party slots")
{
    static const u16 species[PARTY_SIZE] =
    {
        SPECIES_WOBBUFFET,
        SPECIES_CATERPIE,
        SPECIES_WURMPLE,
        SPECIES_ZIGZAGOON,
        SPECIES_POOCHYENA,
        SPECIES_RALTS,
    };
    static const u8 selectedSlots[] = {4, 1, 3};
    struct Pokemon originalParty[PARTY_SIZE];
    struct Pokemon battledParty[ARRAY_COUNT(selectedSlots)];
    u32 value;
    u32 i;

    BestOfThree_Finish();
    ClearTrainerFlag(TRAINER_ROXANNE_1);
    memset(gPlayerParty, 0, sizeof(gPlayerParty));
    memset(gSelectedOrderFromParty, 0, sizeof(gSelectedOrderFromParty));
    for (i = 0; i < PARTY_SIZE; i++)
        CreateMon(&gPlayerParty[i], species[i], 10 + i, 0, FALSE, 0, OT_ID_PRESET, 0);
    gPlayerPartyCount = PARTY_SIZE;
    memcpy(originalParty, gPlayerParty, sizeof(originalParty));

    EXPECT(InitializeRoxanne(FALSE) != NULL);
    for (i = 0; i < ARRAY_COUNT(selectedSlots); i++)
        gSelectedOrderFromParty[i] = selectedSlots[i] + 1;
    BestOfThree_PrepareParty();

    value = GetMonData(&gPlayerParty[0], MON_DATA_EXP) + 25;
    SetMonData(&gPlayerParty[0], MON_DATA_EXP, &value);
    value = ITEM_ORAN_BERRY;
    SetMonData(&gPlayerParty[1], MON_DATA_HELD_ITEM, &value);
    value = STATUS1_POISON;
    SetMonData(&gPlayerParty[2], MON_DATA_STATUS, &value);
    memcpy(battledParty, gPlayerParty, sizeof(battledParty));
    memset(gSelectedOrderFromParty, 0, sizeof(gSelectedOrderFromParty));

    EXPECT(BestOfThree_ReconcileDraftParty());
    EXPECT_EQ(gPlayerPartyCount, PARTY_SIZE);
    for (i = 0; i < ARRAY_COUNT(selectedSlots); i++)
        EXPECT(memcmp(&gPlayerParty[selectedSlots[i]], &battledParty[i], sizeof(struct Pokemon)) == 0);
    EXPECT(memcmp(&gPlayerParty[0], &originalParty[0], sizeof(struct Pokemon)) == 0);
    EXPECT(memcmp(&gPlayerParty[2], &originalParty[2], sizeof(struct Pokemon)) == 0);
    EXPECT(memcmp(&gPlayerParty[5], &originalParty[5], sizeof(struct Pokemon)) == 0);

    BestOfThree_Finish();
}

TEST("Draft battle: Invalid selected-slot mappings restore the untouched party")
{
    struct Pokemon originalParty[PARTY_SIZE];
    u32 item = ITEM_ORAN_BERRY;
    u32 i;

    BestOfThree_Finish();
    ClearTrainerFlag(TRAINER_ROXANNE_1);
    memset(gPlayerParty, 0, sizeof(gPlayerParty));
    memset(gSelectedOrderFromParty, 0, sizeof(gSelectedOrderFromParty));
    CreateMon(&gPlayerParty[0], SPECIES_CATERPIE, 10, 0, FALSE, 0, OT_ID_PRESET, 0);
    CreateMon(&gPlayerParty[1], SPECIES_WURMPLE, 10, 0, FALSE, 0, OT_ID_PRESET, 0);
    CreateMon(&gPlayerParty[2], SPECIES_POOCHYENA, 10, 0, FALSE, 0, OT_ID_PRESET, 0);
    gPlayerPartyCount = 3;
    memcpy(originalParty, gPlayerParty, sizeof(originalParty));

    EXPECT(InitializeRoxanne(FALSE) != NULL);
    gSelectedOrderFromParty[0] = 1;
    gSelectedOrderFromParty[1] = 1;
    gSelectedOrderFromParty[2] = 2;
    BestOfThree_PrepareParty();
    SetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM, &item);

    EXPECT(!BestOfThree_ReconcileDraftParty());
    EXPECT_EQ(gPlayerPartyCount, 3);
    for (i = 0; i < PARTY_SIZE; i++)
        EXPECT(memcmp(&gPlayerParty[i], &originalParty[i], sizeof(struct Pokemon)) == 0);

    BestOfThree_Finish();
}

TEST("BO3: Champion cancellation aborts and requests a League-ending whiteout")
{
    ClearTrainerFlag(TRAINER_ROXANNE_1);

    EXPECT(InitializeWallaceChampion(TRUE) != NULL);
    BestOfThree_RecordOutcomeForTest(B_OUTCOME_WON);
    BestOfThree_GetCancelPolicy();
    EXPECT_EQ(gSpecialVar_Result, BO3_CANCEL_WHITEOUT);
    EXPECT(!BestOfThree_IsActive());
    EXPECT_EQ(BestOfThree_GetWins(), 0);

    EXPECT(InitializeRoxanne(TRUE) != NULL);
    BestOfThree_RecordOutcomeForTest(B_OUTCOME_WON);
    BestOfThree_GetCancelPolicy();
    EXPECT_EQ(gSpecialVar_Result, BO3_CANCEL_ABORT);
    EXPECT(!BestOfThree_IsActive());
}

TEST("BO3: A ready gym rematch uses its script-authored result data and settles on victory")
{
    u16 rematchTrainer = 0;

    SetTrainerFlag(TRAINER_ROXANNE_1);
    ClearTrainerFlag(TRAINER_ROXANNE_2);
    gSaveBlock1Ptr->trainerRematches[REMATCH_ROXANNE] = 1;

    EXPECT(BattleSetup_GetReadyRematchTrainerId(TRAINER_ROXANNE_1, &rematchTrainer));
    EXPECT_EQ(rematchTrainer, TRAINER_ROXANNE_2);
    EXPECT(BestOfThree_TryInitializeRematch(rematchTrainer,
                                            RustboroCity_Gym_Text_RoxannePreRematch,
                                            RustboroCity_Gym_Text_RoxanneRematchDefeat,
                                            RustboroCity_Gym_EventScript_RoxanneRematchDefeated,
                                            TRUE) != NULL);
    EXPECT(BestOfThree_GetVictoryScriptForTest() == RustboroCity_Gym_EventScript_RoxanneRematchDefeated);
    EXPECT_EQ(BestOfThree_RecordOutcomeForTest(B_OUTCOME_WON), BO3_ROUND_CONTINUE);
    EXPECT(BestOfThree_GetBattleText(TRUE) == RustboroCity_Gym_Text_RoxanneRematchDefeat);
    gBattleOutcome = B_OUTCOME_WON;
    BestOfThree_CompleteRound();

    EXPECT_EQ(gSpecialVar_Result, BO3_SERIES_WON);
    EXPECT(HasTrainerBeenFought(TRAINER_ROXANNE_2));
    EXPECT_EQ(gSaveBlock1Ptr->trainerRematches[REMATCH_ROXANNE], 0);

    ClearTrainerFlag(TRAINER_ROXANNE_1);
    ClearTrainerFlag(TRAINER_ROXANNE_2);
    BestOfThree_Finish();
}

TEST("BO3: Terminal victory applies the exact prize, full-team EXP pool, and trainer flag once")
{
    const struct TrainerMon *trainerParty = GetTrainerPartyFromId(TRAINER_ROXANNE_1);
    u32 trainerPartySize = GetTrainerPartySizeFromId(TRAINER_ROXANNE_1);
    u32 expectedMoney = 4
                      * trainerParty[trainerPartySize - 1].lvl
                      * 2
                      * gTrainerClasses[GetTrainerClassFromId(TRAINER_ROXANNE_1)].money;
    u32 expectedPool = 0;
    u32 originalExp;
    u32 moneyBefore = 1000;
    u32 species;
    u32 level;
    u32 exp;
    u32 i;

    ClearTrainerFlag(TRAINER_ROXANNE_1);
    memset(gPlayerParty, 0, sizeof(gPlayerParty));
    memset(gEnemyParty, 0, sizeof(gEnemyParty));
    memset(gSelectedOrderFromParty, 0, sizeof(gSelectedOrderFromParty));
    gSaveBlock2Ptr->optionsLevelCap = OPTIONS_LEVEL_CAPS_OFF;
    CreateMon(&gPlayerParty[0], SPECIES_CATERPIE, 10, 0, FALSE, 0, OT_ID_PRESET, 0);
    gPlayerPartyCount = 1;
    originalExp = GetMonData(&gPlayerParty[0], MON_DATA_EXP);
    SetMoney(&gSaveBlock1Ptr->money, moneyBefore);

    EXPECT(InitializeRoxanne(TRUE) != NULL);
    gSelectedOrderFromParty[0] = 1;
    BestOfThree_PrepareParty();
    EXPECT_EQ(BestOfThree_ApplyMoneySettlement(B_OUTCOME_WON, 2), 0);
    gBattleOutcome = B_OUTCOME_WON;
    BestOfThree_CompleteRound();
    EXPECT_EQ(gSpecialVar_Result, BO3_ROUND_CONTINUE);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), moneyBefore);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_EXP), originalExp);
    EXPECT(!HasTrainerBeenFought(TRAINER_ROXANNE_1));

    memset(gSelectedOrderFromParty, 0, sizeof(gSelectedOrderFromParty));
    gSelectedOrderFromParty[0] = 1;
    BestOfThree_PrepareParty();
    CreateMon(&gEnemyParty[0], SPECIES_CATERPIE, 5, 0, FALSE, 0, OT_ID_PRESET, 0);
    CreateMon(&gEnemyParty[1], SPECIES_WURMPLE, 7, 0, FALSE, 0, OT_ID_PRESET, 0);
    for (i = 0; i < 2; i++)
    {
        species = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES);
        level = GetMonData(&gEnemyParty[i], MON_DATA_LEVEL);
        exp = gSpeciesInfo[species].expYield * level;
#if B_SCALED_EXP >= GEN_5 && B_SCALED_EXP != GEN_6
        exp /= 5;
#else
        exp /= 7;
#endif
#if B_TRAINER_EXP_MULTIPLIER <= GEN_7
        exp = exp * 150 / 100;
#endif
        expectedPool += exp;
    }

    EXPECT_EQ(BestOfThree_ApplyMoneySettlement(B_OUTCOME_WON, 2), expectedMoney);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), moneyBefore + expectedMoney);
    EXPECT_EQ(BestOfThree_ApplyMoneySettlement(B_OUTCOME_WON, 2), 0);
    gBattleOutcome = B_OUTCOME_WON;
    BestOfThree_CompleteRound();
    EXPECT_EQ(gSpecialVar_Result, BO3_SERIES_WON);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), moneyBefore + expectedMoney);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_EXP), originalExp + expectedPool * 10 / 100);
    EXPECT(HasTrainerBeenFought(TRAINER_ROXANNE_1));

    ClearTrainerFlag(TRAINER_ROXANNE_1);
    BestOfThree_Finish();
}

TEST("BO3: Trainer flags are registered only by a terminal series victory")
{
    u32 moneyBefore = 1000;
    u32 reward;

    ClearTrainerFlag(TRAINER_ROXANNE_1);
    memset(gPlayerParty, 0, sizeof(gPlayerParty));
    memset(gEnemyParty, 0, sizeof(gEnemyParty));
    gPlayerPartyCount = 0;
    EXPECT(InitializeRoxanne(TRUE) != NULL);
    SetMoney(&gSaveBlock1Ptr->money, moneyBefore);

    EXPECT_EQ(BestOfThree_ApplyMoneySettlement(B_OUTCOME_WON, 1), 0);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), moneyBefore);
    EXPECT_EQ(BestOfThree_RecordOutcomeForTest(B_OUTCOME_WON), BO3_ROUND_CONTINUE);
    EXPECT(!HasTrainerBeenFought(TRAINER_ROXANNE_1));
    reward = BestOfThree_ApplyMoneySettlement(B_OUTCOME_WON, 1);
    EXPECT_GT(reward, 0);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), moneyBefore + reward);
    EXPECT_EQ(BestOfThree_ApplyMoneySettlement(B_OUTCOME_WON, 1), 0);
    gBattleOutcome = B_OUTCOME_WON;
    BestOfThree_CompleteRound();
    EXPECT_EQ(gSpecialVar_Result, BO3_SERIES_WON);
    EXPECT(HasTrainerBeenFought(TRAINER_ROXANNE_1));

    ClearTrainerFlag(TRAINER_ROXANNE_1);
    BestOfThree_Finish();
}

TEST("BO3: Terminal loss restores the party, settles once, and grants capped silent EXP")
{
    u32 originalHp;
    u32 originalPp;
    u32 originalStatus;
    u32 originalExp;
    u32 maxLevelExp;
    u32 expectedPool;
    u32 expectedShare;
    u32 item = ITEM_ORAN_BERRY;
    u32 move = MOVE_TACKLE;
    u32 changedMove = MOVE_GROWL;
    u32 pp = 7;
    u32 hp = 3;
    u32 status = STATUS1_POISON;
    u32 zero = 0;
    u32 moneyBefore = 100000;
    u32 moneyLost;
    u32 i;

    BestOfThree_Finish();
    memset(gPlayerParty, 0, sizeof(gPlayerParty));
    memset(gEnemyParty, 0, sizeof(gEnemyParty));
    memset(gSelectedOrderFromParty, 0, sizeof(gSelectedOrderFromParty));
    gSaveBlock2Ptr->optionsLevelCap = OPTIONS_LEVEL_CAPS_OFF;
    for (i = 0; i < NUM_BADGES; i++)
        FlagClear(FLAG_BADGE01_GET + i);

    CreateMon(&gPlayerParty[0], SPECIES_CATERPIE, 10, 0, FALSE, 0, OT_ID_PRESET, 0);
    CreateMon(&gPlayerParty[1], SPECIES_WOBBUFFET, MAX_LEVEL, 0, FALSE, 0, OT_ID_PRESET, 0);
    SetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM, &item);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE1, &move);
    SetMonData(&gPlayerParty[0], MON_DATA_PP1, &pp);
    SetMonData(&gPlayerParty[0], MON_DATA_HP, &hp);
    SetMonData(&gPlayerParty[0], MON_DATA_STATUS, &status);
    gPlayerPartyCount = 2;
    originalHp = GetMonData(&gPlayerParty[0], MON_DATA_HP);
    originalPp = GetMonData(&gPlayerParty[0], MON_DATA_PP1);
    originalStatus = GetMonData(&gPlayerParty[0], MON_DATA_STATUS);
    originalExp = GetMonData(&gPlayerParty[0], MON_DATA_EXP);
    maxLevelExp = GetMonData(&gPlayerParty[1], MON_DATA_EXP);

    EXPECT(InitializeWallaceChampion(TRUE) != NULL);
    gSelectedOrderFromParty[0] = 1;
    gSelectedOrderFromParty[1] = 2;
    BestOfThree_PrepareParty();

    SetMonData(&gPlayerParty[0], MON_DATA_HP, &zero);
    SetMonData(&gPlayerParty[0], MON_DATA_PP1, &zero);
    SetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM, &zero);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE1, &changedMove);
    SetMonData(&gPlayerParty[0], MON_DATA_STATUS, &zero);
    SetMonData(&gPlayerParty[0], MON_DATA_EXP, &zero);

    CreateMon(&gEnemyParty[0], SPECIES_CATERPIE, 50, 0, FALSE, 0, OT_ID_PRESET, 0);
    EXPECT_EQ(BestOfThree_RecordOutcomeForTest(B_OUTCOME_LOST), BO3_ROUND_CONTINUE);
    SetMoney(&gSaveBlock1Ptr->money, moneyBefore);
    moneyLost = BestOfThree_ApplyMoneySettlement(B_OUTCOME_LOST, 1);
    EXPECT_EQ(moneyLost, 8 * MAX_LEVEL);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), moneyBefore - moneyLost);
    EXPECT_EQ(BestOfThree_ApplyMoneySettlement(B_OUTCOME_LOST, 1), 0);

    gBattleOutcome = B_OUTCOME_LOST;
    BestOfThree_CompleteRound();
    EXPECT_EQ(gSpecialVar_Result, BO3_SERIES_LOST);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_HP), originalHp);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_PP1), originalPp);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_STATUS), originalStatus);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM), ITEM_ORAN_BERRY);
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_MOVE1), MOVE_TACKLE);

    expectedPool = gSpeciesInfo[SPECIES_CATERPIE].expYield * 50 / 5;
#if B_TRAINER_EXP_MULTIPLIER <= GEN_7
    expectedPool = expectedPool * 150 / 100;
#endif
    expectedShare = expectedPool * 5 / 100;
    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_EXP), originalExp + expectedShare);
    EXPECT_LT(GetMonData(&gPlayerParty[0], MON_DATA_EXP),
              gExperienceTables[gSpeciesInfo[SPECIES_CATERPIE].growthRate][11]);
    EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_EXP), maxLevelExp);

    BestOfThree_Finish();
}
