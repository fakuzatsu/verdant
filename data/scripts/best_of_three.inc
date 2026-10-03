EventScript_BestOfThreeStart::
	special BestOfThree_BufferIntroText
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto EventScript_BestOfThreeChooseParty

EventScript_BestOfThreeChooseParty:
	special BestOfThree_SetDraftSize
	special ChoosePartyForDraft
	waitstate
	goto_if_eq VAR_RESULT, FALSE, EventScript_BestOfThreePartyCancelled
	special BestOfThree_PrepareParty
	special BestOfThree_StartBattle
	waitstate
	special BestOfThree_CompleteRound
	goto_if_eq VAR_RESULT, BO3_ROUND_CONTINUE, EventScript_BestOfThreeNextRound
	goto_if_eq VAR_RESULT, BO3_SERIES_WON, EventScript_BestOfThreeWon
	goto_if_eq VAR_RESULT, BO3_SERIES_LOST, EventScript_BestOfThreeLost
	release
	end

EventScript_BestOfThreeNextRound:
	special HealPlayerParty
EventScript_BestOfThreeContinuePrompt:
	special BestOfThree_BufferContinueText
	msgbox gStringVar4, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_BestOfThreeContinueDeclined
	goto EventScript_BestOfThreeChooseParty

EventScript_BestOfThreeContinueDeclined:
	special BestOfThree_GetCancelPolicy
	goto_if_eq VAR_RESULT, BO3_CANCEL_WHITEOUT, EventScript_BestOfThreeLost
	release
	end

EventScript_BestOfThreePartyCancelled:
	special BestOfThree_GetCancelPolicy
	goto_if_eq VAR_RESULT, BO3_CANCEL_WHITEOUT, EventScript_BestOfThreeLost
	release
	end

EventScript_BestOfThreeWon:
	special BestOfThree_Finish
	gotobeatenscript
	releaseall
	end

EventScript_BestOfThreeLost:
	special BestOfThree_Finish
	special SetCB2WhiteOut
	waitstate
	end

EventScript_DraftBattleStart::
	special BestOfThree_BufferIntroText
	msgbox gStringVar4, MSGBOX_DEFAULT
EventScript_DraftBattleChooseParty:
	special BestOfThree_SetDraftSize
	special ChoosePartyForDraft
	waitstate
	goto_if_eq VAR_RESULT, FALSE, EventScript_DraftBattlePartyCancelled
	special BestOfThree_PrepareParty
	special BestOfThree_StartBattle
	waitstate
	goto_if_ne VAR_RESULT, B_OUTCOME_WON, EventScript_DraftBattleAborted
	special BestOfThree_Finish
	gotobeatenscript
	releaseall
	end

EventScript_DraftBattleAborted:
	special BestOfThree_Abort
	releaseall
	end

EventScript_DraftBattlePartyCancelled:
	special BestOfThree_GetCancelPolicy
	goto_if_eq VAR_RESULT, BO3_CANCEL_WHITEOUT, EventScript_BestOfThreeLost
	release
	end
