.set LOCALID_MOLTRES, 1
.set LOCALID_NINJA_1, 2
.set LOCALID_NINJA_2, 3

MtChimney_North_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, MtChimney_North_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, MtChimney_North_OnTransition
	.byte 0

MtChimney_North_OnResume:
	setstepcallback STEP_CB_ASH
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, MtChimney_North_EventScript_TryRemoveMoltres
	end

MtChimney_North_OnTransition:
	goto_if_set FLAG_CAUGHT_MOLTRES, MtChimney_North_EventScript_HideMoltres
	goto_if_set FLAG_DEFEATED_MOLTRES, MtChimney_North_EventScript_HideMoltres
	clearflag FLAG_TEMP_9
	end

MtChimney_North_EventScript_HideMoltres::
	setflag FLAG_TEMP_9
	end

MtChimney_North_EventScript_TryRemoveMoltres::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_MOLTRES
	removeobject LOCALID_NINJA_1
	removeobject LOCALID_NINJA_2
	return

MtChimney_North_EventScript_Moltres::
	lock
	faceplayer
	message MtChimney_North_Text_Moltres
	playmoncry SPECIES_MOLTRES, CRY_MODE_ENCOUNTER
	waitmoncry
	closemessage
	seteventmon SPECIES_MOLTRES, 60
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtChimney_North_EventScript_MoltresDefeated
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtChimney_North_EventScript_PlayerOrMoltresRan
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtChimney_North_EventScript_PlayerOrMoltresRan
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_TELEPORTED, MtChimney_North_EventScript_PlayerOrMoltresRan
	setflag FLAG_CAUGHT_MOLTRES
	release
	end

MtChimney_North_EventScript_MoltresDefeated::
	setflag FLAG_DEFEATED_MOLTRES
	setvar VAR_0x8004, SPECIES_MOLTRES
	goto Common_EventScript_LegendaryFlewAway
	end

MtChimney_North_EventScript_PlayerOrMoltresRan::
	setflag FLAG_TEMP_9
	setvar VAR_0x8004, SPECIES_MOLTRES
	goto Common_EventScript_LegendaryFlewAway
	end

MtChimney_North_Text_Moltres:
	.string "Kyyuuu$"
