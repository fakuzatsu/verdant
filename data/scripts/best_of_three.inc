EventScript_RoxanneBestOfThreeStart::
    goto_if_ge VAR_MATCHES_PLAYED, 1, EventScript_RoxanneReturnToContinueQ
    msgbox RustboroCity_Gym_Text_RoxanneIntro, MSGBOX_DEFAULT
EventScript_RoxanneBestOfThreeContinue:
    setvar VAR_0x8005, 3
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
    trainerbattle_no_intro_no_whiteout TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxanneMatchDefeat
    goto EventScript_RoxanneAfterBattle
    end

EventScript_RoxanneAfterBattle:
    special LoadPlayerParty
    call EventScript_CountMatchesWinsAndLosses
    setvar VAR_0x8001, 250
    goto_if_ge VAR_MATCHES_WON, 2, EventScript_DefeatedRoxanne
    goto_if_ge VAR_MATCHES_LOST, 2, EventScript_PlayerDefeated
    goto_if_ge VAR_MATCHES_PLAYED, 3, EventScript_AbortBattleFallback
    special HealPlayerParty
EventScript_RoxanneReturnToContinueQ:
    msgbox RustboroCity_Gym_Text_RoxanneContinue, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_AbortChallenge
    goto EventScript_RoxanneBestOfThreeContinue
    end

EventScript_DefeatedRoxanne:
    msgbox RustboroCity_Gym_Text_RoxanneDefeat, MSGBOX_DEFAULT
    goto RustboroCity_Gym_EventScript_RoxanneDefeated
    end

EventScript_BrawlyBestOfThreeStart::
    goto_if_ge VAR_MATCHES_PLAYED, 1, EventScript_BrawlyReturnToContinueQ
    msgbox DewfordTown_Gym_Text_BrawlyIntro, MSGBOX_DEFAULT
EventScript_BrawlyBestOfThreeContinue:
    setvar VAR_0x8005, 3
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
    trainerbattle_no_intro_no_whiteout TRAINER_BRAWLY_1, DewfordTown_Gym_Text_BrawlyMatchDefeat
    goto EventScript_BrawlyAfterBattle
    end

EventScript_BrawlyAfterBattle:
    special LoadPlayerParty
    call EventScript_CountMatchesWinsAndLosses
    setvar VAR_0x8001, 250
    goto_if_ge VAR_MATCHES_WON, 2, EventScript_DefeatedBrawly
    goto_if_ge VAR_MATCHES_LOST, 2, EventScript_PlayerDefeated
    goto_if_ge VAR_MATCHES_PLAYED, 3, EventScript_AbortBattleFallback
    special HealPlayerParty
EventScript_BrawlyReturnToContinueQ:
    msgbox DewfordTown_Gym_Text_BrawlyContinue, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_AbortChallenge
    goto EventScript_BrawlyBestOfThreeContinue
    end

EventScript_DefeatedBrawly:
    msgbox DewfordTown_Gym_Text_BrawlyDefeat, MSGBOX_DEFAULT
    goto DewfordTown_Gym_EventScript_BrawlyDefeated
    end

EventScript_WattsonBestOfThreeStart::
    goto_if_ge VAR_MATCHES_PLAYED, 1, EventScript_WattsonReturnToContinueQ
    msgbox MauvilleCity_Gym_Text_WattsonIntro, MSGBOX_DEFAULT
EventScript_WattsonBestOfThreeContinue:
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
    trainerbattle_no_intro_no_whiteout TRAINER_WATTSON_1, MauvilleCity_Gym_Text_WattsonMatchDefeat
    goto EventScript_WattsonAfterBattle
    end

EventScript_WattsonAfterBattle:
    special LoadPlayerParty
    call EventScript_CountMatchesWinsAndLosses
    setvar VAR_0x8001, 250
    goto_if_ge VAR_MATCHES_WON, 2, EventScript_DefeatedWattson
    goto_if_ge VAR_MATCHES_LOST, 2, EventScript_PlayerDefeated
    goto_if_ge VAR_MATCHES_PLAYED, 3, EventScript_AbortBattleFallback
    special HealPlayerParty
EventScript_WattsonReturnToContinueQ:
    msgbox MauvilleCity_Gym_Text_WattsonContinue, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_AbortChallenge
    goto EventScript_WattsonBestOfThreeContinue
    end

EventScript_DefeatedWattson:
    msgbox MauvilleCity_Gym_Text_WattsonDefeat, MSGBOX_DEFAULT
    goto MauvilleCity_Gym_EventScript_WattsonDefeated
    end

EventScript_FlanneryBestOfThreeStart::
    goto_if_ge VAR_MATCHES_PLAYED, 1, EventScript_FlanneryReturnToContinueQ
    msgbox LavaridgeTown_Gym_1F_Text_FlanneryIntro, MSGBOX_DEFAULT
EventScript_FlanneryBestOfThreeContinue:
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
    trainerbattle_no_intro_no_whiteout TRAINER_FLANNERY_1, LavaridgeTown_Gym_1F_Text_FlanneryMatchDefeat
    goto EventScript_FlanneryAfterBattle
    end

EventScript_FlanneryAfterBattle:
    special LoadPlayerParty
    call EventScript_CountMatchesWinsAndLosses
    setvar VAR_0x8001, 250
    goto_if_ge VAR_MATCHES_WON, 2, EventScript_DefeatedFlannery
    goto_if_ge VAR_MATCHES_LOST, 2, EventScript_PlayerDefeated
    goto_if_ge VAR_MATCHES_PLAYED, 3, EventScript_AbortBattleFallback
    special HealPlayerParty
EventScript_FlanneryReturnToContinueQ:
    msgbox LavaridgeTown_Gym_1F_Text_FlanneryContinue, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_AbortChallenge
    goto EventScript_FlanneryBestOfThreeContinue
    end

EventScript_DefeatedFlannery:
    msgbox LavaridgeTown_Gym_1F_Text_FlanneryDefeat, MSGBOX_DEFAULT
    goto LavaridgeTown_Gym_1F_EventScript_FlanneryDefeated
    end

EventScript_NormanBestOfThreeStart::
    goto_if_ge VAR_MATCHES_PLAYED, 1, EventScript_NormanReturnToContinueQ
    msgbox PetalburgCity_Gym_Text_NormanIntro, MSGBOX_DEFAULT
EventScript_NormanBestOfThreeContinue:
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
    trainerbattle_no_intro_no_whiteout TRAINER_NORMAN_1, PetalburgCity_Gym_Text_NormanMatchDefeat
    goto EventScript_NormanAfterBattle
    end

EventScript_NormanAfterBattle:
    special LoadPlayerParty
    call EventScript_CountMatchesWinsAndLosses
    setvar VAR_0x8001, 250
    goto_if_ge VAR_MATCHES_WON, 2, EventScript_DefeatedNorman
    goto_if_ge VAR_MATCHES_LOST, 2, EventScript_PlayerDefeated
    goto_if_ge VAR_MATCHES_PLAYED, 3, EventScript_AbortBattleFallback
    special HealPlayerParty
EventScript_NormanReturnToContinueQ:
    msgbox PetalburgCity_Gym_Text_NormanContinue, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_AbortChallenge
    goto EventScript_NormanBestOfThreeContinue
    end

EventScript_DefeatedNorman:
    msgbox PetalburgCity_Gym_Text_NormanDefeat, MSGBOX_DEFAULT
    goto PetalburgCity_Gym_EventScript_NormanDefeated
    end

EventScript_WinonaBestOfThreeStart::
    goto_if_ge VAR_MATCHES_PLAYED, 1, EventScript_WinonaReturnToContinueQ
    msgbox FortreeCity_Gym_Text_WinonaIntro, MSGBOX_DEFAULT
EventScript_WinonaBestOfThreeContinue:
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
    trainerbattle_no_intro_no_whiteout TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaMatchDefeat
    goto EventScript_WinonaAfterBattle
    end

EventScript_WinonaAfterBattle:
    special LoadPlayerParty
    call EventScript_CountMatchesWinsAndLosses
    setvar VAR_0x8001, 250
    goto_if_ge VAR_MATCHES_WON, 2, EventScript_DefeatedWinona
    goto_if_ge VAR_MATCHES_LOST, 2, EventScript_PlayerDefeated
    goto_if_ge VAR_MATCHES_PLAYED, 3, EventScript_AbortBattleFallback
    special HealPlayerParty
EventScript_WinonaReturnToContinueQ:
    msgbox FortreeCity_Gym_Text_WinonaContinue, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_AbortChallenge
    goto EventScript_WinonaBestOfThreeContinue
    end

EventScript_DefeatedWinona:
    msgbox FortreeCity_Gym_Text_WinonaDefeat, MSGBOX_DEFAULT
    goto FortreeCity_Gym_EventScript_WinonaDefeated
    end

EventScript_TateAndLizaBestOfThreeStart::
    goto_if_ge VAR_MATCHES_PLAYED, 1, EventScript_TateAndLizaReturnToContinueQ
    msgbox MossdeepCity_Gym_Text_TateAndLizaIntro, MSGBOX_DEFAULT
EventScript_TateAndLizaBestOfThreeContinue:
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
    trainerbattle_no_intro_no_whiteout TRAINER_TATE_AND_LIZA_1, MossdeepCity_Gym_Text_TateAndLizaMatchDefeat
    goto EventScript_TateAndLizaAfterBattle
    end

EventScript_TateAndLizaAfterBattle:
    special LoadPlayerParty
    call EventScript_CountMatchesWinsAndLosses
    setvar VAR_0x8001, 250
    goto_if_ge VAR_MATCHES_WON, 2, EventScript_DefeatedTateAndLiza
    goto_if_ge VAR_MATCHES_LOST, 2, EventScript_PlayerDefeated
    goto_if_ge VAR_MATCHES_PLAYED, 3, EventScript_AbortBattleFallback
    special HealPlayerParty
EventScript_TateAndLizaReturnToContinueQ:
    msgbox MossdeepCity_Gym_Text_TateAndLizaContinue, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_AbortChallenge
    goto EventScript_TateAndLizaBestOfThreeContinue
    end

EventScript_DefeatedTateAndLiza:
    msgbox MossdeepCity_Gym_Text_TateAndLizaDefeat, MSGBOX_DEFAULT
    goto MossdeepCity_Gym_EventScript_TateAndLizaDefeated
    end

EventScript_JuanBestOfThreeStart::
    goto_if_ge VAR_MATCHES_PLAYED, 1, EventScript_JuanReturnToContinueQ
    msgbox SootopolisCity_Gym_1F_Text_JuanIntro, MSGBOX_DEFAULT
EventScript_JuanBestOfThreeContinue:
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
    trainerbattle_no_intro_no_whiteout TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanMatchDefeat
    goto EventScript_JuanAfterBattle
    end

EventScript_JuanAfterBattle:
    special LoadPlayerParty
    call EventScript_CountMatchesWinsAndLosses
    setvar VAR_0x8001, 250
    goto_if_ge VAR_MATCHES_WON, 2, EventScript_DefeatedJuan
    goto_if_ge VAR_MATCHES_LOST, 2, EventScript_PlayerDefeated
    goto_if_ge VAR_MATCHES_PLAYED, 3, EventScript_AbortBattleFallback
    special HealPlayerParty
EventScript_JuanReturnToContinueQ:
    msgbox SootopolisCity_Gym_1F_Text_JuanContinue, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_AbortChallenge
    goto EventScript_JuanBestOfThreeContinue
    end

EventScript_DefeatedJuan:
    msgbox SootopolisCity_Gym_1F_Text_JuanDefeat, MSGBOX_DEFAULT
    goto SootopolisCity_Gym_1F_EventScript_JuanDefeated
    end

EventScript_PlayerDefeated:
    buffernumberstring STR_VAR_1, VAR_0x8001
    msgbox Text_PaidOutMoney, MSGBOX_DEFAULT
EventScript_AbortBattleFallback:
    setvar VAR_MATCHES_PLAYED, 0
    setvar VAR_MATCHES_WON, 0
    setvar VAR_MATCHES_LOST, 0
EventScript_AbortChallenge::
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
