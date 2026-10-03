#ifndef GUARD_BEST_OF_THREE_CONTROLLER_H
#define GUARD_BEST_OF_THREE_CONTROLLER_H

#include "global.h"

struct ScriptContext;

const u8 *BestOfThree_TryInitialize(u16 trainerId, const u8 *introText,
                                    const u8 *seriesWonText, const u8 *victoryScript,
                                    bool32 bestOfThree);
const u8 *BestOfThree_TryInitializeRematch(u16 trainerId, const u8 *introText,
                                           const u8 *seriesWonText, const u8 *victoryScript,
                                           bool32 bestOfThree);
bool32 BestOfThree_IsActive(void);
bool32 BestOfThree_IsDraftBattle(void);
bool32 BestOfThree_IsSupportedTrainer(u16 trainerId);
bool32 BestOfThree_IsTrainerEligible(u16 trainerId);
bool32 BestOfThree_IsChampion(void);
bool32 BestOfThree_IsTerminalOutcome(u32 battleOutcome);
const u8 *BestOfThree_GetBattleText(bool32 playerWon);
u32 BestOfThree_ApplyMoneySettlement(u32 battleOutcome, u32 moneyMultiplier);
void BestOfThree_ApplyTerminalExp(void);
bool32 BestOfThree_ReconcileDraftParty(void);
void BestOfThree_RegisterVictory(void);

// Field-script specials used by the common BO3 and draft shells.
void BestOfThree_BufferIntroText(void);
void BestOfThree_BufferContinueText(void);
void BestOfThree_SetDraftSize(void);
void BestOfThree_PrepareParty(void);
void BestOfThree_StartBattle(void);
void BestOfThree_CompleteRound(void);
void BestOfThree_Abort(void);
void BestOfThree_Finish(void);
void BestOfThree_GetCancelPolicy(void);

#if TESTING
u8 BestOfThree_GetWins(void);
u8 BestOfThree_GetLosses(void);
u8 BestOfThree_RecordOutcomeForTest(u32 battleOutcome);
const u8 *BestOfThree_GetVictoryScriptForTest(void);
#endif

#endif // GUARD_BEST_OF_THREE_CONTROLLER_H
