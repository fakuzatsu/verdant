EventScript_RoxanneDraftStart::
    setvar VAR_0x8005, 3
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
	trainerbattle_single TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxanneIntro, RustboroCity_Gym_Text_RoxanneDefeat, EventScript_RoxanneDraftDefeated, NO_MUSIC
	end

EventScript_RoxanneDraftDefeated::
    special LoadPlayerParty
    goto RustboroCity_Gym_EventScript_RoxanneDefeated
    end

EventScript_BrawlyDraftStart::
    setvar VAR_0x8005, 3
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
	trainerbattle_single TRAINER_BRAWLY_1, DewfordTown_Gym_Text_BrawlyIntro, DewfordTown_Gym_Text_BrawlyDefeat, EventScript_BrawlyDraftDefeated, NO_MUSIC
    end

EventScript_BrawlyDraftDefeated::
    special LoadPlayerParty
    goto DewfordTown_Gym_EventScript_BrawlyDefeated
    end

EventScript_WattsonDraftStart::
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
	trainerbattle_single TRAINER_WATTSON_1, MauvilleCity_Gym_Text_WattsonIntro, MauvilleCity_Gym_Text_WattsonDefeat, EventScript_WattsonDraftDefeated, NO_MUSIC
    end

EventScript_WattsonDraftDefeated::
    special LoadPlayerParty
    goto MauvilleCity_Gym_EventScript_WattsonDefeated
    end

EventScript_FlanneryDraftStart::
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
	trainerbattle_single TRAINER_FLANNERY_1, LavaridgeTown_Gym_1F_Text_FlanneryIntro, LavaridgeTown_Gym_1F_Text_FlanneryDefeat, EventScript_FlanneryDraftDefeated, NO_MUSIC
    end

EventScript_FlanneryDraftDefeated::
    special LoadPlayerParty
    goto LavaridgeTown_Gym_1F_EventScript_FlanneryDefeated
    end

EventScript_NormanDraftStart::
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
	trainerbattle_no_intro TRAINER_NORMAN_1, PetalburgCity_Gym_Text_NormanDefeat
    special LoadPlayerParty
    goto PetalburgCity_Gym_EventScript_NormanDefeated
    end

EventScript_WinonaDraftStart::
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
	trainerbattle_single TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaIntro, FortreeCity_Gym_Text_WinonaDefeat, EventScript_WinonaDraftDefeated, NO_MUSIC
    end

EventScript_WinonaDraftDefeated::
    special LoadPlayerParty
    goto FortreeCity_Gym_EventScript_WinonaDefeated
    end

EventScript_TateAndLizaDraftStart::
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
	trainerbattle_double TRAINER_TATE_AND_LIZA_1, MossdeepCity_Gym_Text_TateAndLizaIntro, MossdeepCity_Gym_Text_TateAndLizaDefeat, MossdeepCity_Gym_Text_TateAndLizaNeedTwoMons, EventScript_TateAndLizaDraftDefeated, NO_MUSIC
    end

EventScript_TateAndLizaDraftDefeated::
    special LoadPlayerParty
    goto MossdeepCity_Gym_EventScript_TateAndLizaDefeated
    end

EventScript_JuanDraftStart::
    setvar VAR_0x8005, 4
    special ChoosePartyForDraft
    waitstate
    goto_if_eq VAR_RESULT, 0, EventScript_AbortChallenge
    special SavePlayerParty
    special ReducePlayerPartyToSelectedMons
	trainerbattle_single TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanIntro, SootopolisCity_Gym_1F_Text_JuanDefeat, EventScript_JuanDraftDefeated, NO_MUSIC
    end

EventScript_JuanDraftDefeated::
    special LoadPlayerParty
    goto SootopolisCity_Gym_1F_EventScript_JuanDefeated
    end
