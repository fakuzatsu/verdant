EventScript_RoxanneBestOfThreeStart::
    goto_if_ge VAR_MATCHES_PLAYED, 1, EventScript_RoxanneReturnToContinueQ
    msgbox RustboroCity_Gym_Text_RoxanneIntro, MSGBOX_DEFAULT
EventScript_RoxanneBestOfThreeContinue:
    trainerbattle_no_intro_no_whiteout TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxanneMatchDefeat
    goto EventScript_RoxanneAfterBattle
    end

EventScript_RoxanneAfterBattle:
    special HealPlayerParty
    call EventScript_CountMatchesWinsAndLosses
    setvar VAR_0x8001, 250
    goto_if_ge VAR_MATCHES_WON, 2, EventScript_DefeatedRoxanne
    goto_if_ge VAR_MATCHES_LOST, 2, EventScript_PlayerDefeated
    goto_if_ge VAR_MATCHES_PLAYED, 3, EventScript_AbortBattleFallback
EventScript_RoxanneReturnToContinueQ:
    msgbox RustboroCity_Gym_Text_RoxanneContinue, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_AbortChallenge
    goto EventScript_RoxanneBestOfThreeContinue
    end

EventScript_DefeatedRoxanne:
    msgbox RustboroCity_Gym_Text_RoxanneDefeat, MSGBOX_DEFAULT
    goto RustboroCity_Gym_EventScript_RoxanneDefeated
    end

EventScript_PlayerDefeated:
    buffernumberstring STR_VAR_1, VAR_0x8001
    msgbox Text_PaidOutMoney, MSGBOX_DEFAULT
EventScript_AbortBattleFallback:
    setvar VAR_MATCHES_PLAYED, 0
    setvar VAR_MATCHES_WON, 0
    setvar VAR_MATCHES_LOST, 0
EventScript_AbortChallenge:
    release
    end

EventScript_CountMatchesWinsAndLosses:
    call EventScript_IncrementMatches
    specialvar VAR_RESULT, GetBattleOutcome
	call_if_eq VAR_RESULT, B_OUTCOME_WON, EventScript_IncrementWins
    call_if_ne VAR_RESULT, B_OUTCOME_WON, EventScript_IncrementLosses
    return

EventScript_IncrementMatches:
    call_if_eq VAR_MATCHES_PLAYED, 2, EventScript_SetMatchesThree
    call_if_eq VAR_MATCHES_PLAYED, 1, EventScript_SetMatchesTwo
    call_if_eq VAR_MATCHES_PLAYED, 0, EventScript_SetMatchesOne
    return

EventScript_SetMatchesThree:
    setvar VAR_MATCHES_PLAYED, 3
    return

EventScript_SetMatchesTwo:
    setvar VAR_MATCHES_PLAYED, 2
    return

EventScript_SetMatchesOne:
    setvar VAR_MATCHES_PLAYED, 1
    return

EventScript_IncrementWins:
    call_if_eq VAR_MATCHES_WON, 1, EventScript_SetMatchesWonTwo
    call_if_eq VAR_MATCHES_WON, 0, EventScript_SetMatchesWonOne
    return

EventScript_SetMatchesWonTwo:
    setvar VAR_MATCHES_WON, 2
    return

EventScript_SetMatchesWonOne:
    setvar VAR_MATCHES_WON, 1
    return

EventScript_IncrementLosses:
    call_if_eq VAR_MATCHES_LOST, 1, EventScript_SetMatchesLostTwo
    call_if_eq VAR_MATCHES_LOST, 0, EventScript_SetMatchesLostOne
    return

EventScript_SetMatchesLostTwo:
    setvar VAR_MATCHES_LOST, 2
    return

EventScript_SetMatchesLostOne:
    setvar VAR_MATCHES_LOST, 1
    return

EventScript_OnTransition_ClearBestOfThreeVars::
	setvar VAR_MATCHES_PLAYED, 0
	setvar VAR_MATCHES_WON, 0
	setvar VAR_MATCHES_LOST, 0
	end

Text_PaidOutMoney:
    .string "{PLAYER} paid ¥{STR_VAR_1} as prize money…$" 
