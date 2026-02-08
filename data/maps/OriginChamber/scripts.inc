OriginChamber_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OriginChamber_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, OriginChamber_OnResume
	.byte 0

.set LOCALID_REGIGIGAS, 1

OriginChamber_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, OriginChamber_EventScript_TryRemoveRegigigas
	end

OriginChamber_EventScript_TryRemoveRegigigas::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_REGIGIGAS
	removeobject LOCALID_NINJA_1
	removeobject LOCALID_NINJA_2
	return

OriginChamber_OnTransition:
	goto_if_set FLAG_CAUGHT_REGIGIGAS, OriginChamber_EventScript_HideRegigigas
	goto_if_set FLAG_DEFEATED_REGIGIGAS, OriginChamber_EventScript_HideRegigigas
	clearflag FLAG_TEMP_9
	end

OriginChamber_EventScript_HideRegigigas::
	setflag FLAG_TEMP_9
	end

OriginChamber_EventScript_Regigigas::
	lock
	faceplayer
	message OriginChamber_Text_Regigigas
	playmoncry SPECIES_REGIGIGAS, CRY_MODE_ENCOUNTER
	waitmoncry
	closemessage
	seteventmon SPECIES_REGIGIGAS, 60
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, OriginChamber_EventScript_RegigigasDefeated
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, OriginChamber_EventScript_PlayerOrRegigigasRan
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, OriginChamber_EventScript_PlayerOrRegigigasRan
	goto_if_eq VAR_RESULT, B_OUTCOME_MON_TELEPORTED, OriginChamber_EventScript_PlayerOrRegigigasRan
	setflag FLAG_CAUGHT_REGIGIGAS
	release
	end

OriginChamber_EventScript_RegigigasDefeated::
	setflag FLAG_DEFEATED_REGIGIGAS
	setvar VAR_0x8004, SPECIES_REGIGIGAS
	goto Common_EventScript_LegendaryFlewAway
	end

OriginChamber_EventScript_PlayerOrRegigigasRan::
	setflag FLAG_TEMP_9
	setvar VAR_0x8004, SPECIES_REGIGIGAS
	goto Common_EventScript_LegendaryFlewAway
	end

OriginChamber_Text_Regigigas:
	.string "Giigaas!$"

