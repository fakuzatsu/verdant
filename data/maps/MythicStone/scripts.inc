MythicStone_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, MythicStone_OnResume
	map_script MAP_SCRIPT_ON_LOAD, MythicStone_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, MythicStone_OnTransition
	.byte 0

MythicStone_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, MythicStone_EventScript_TryRemoveRegidrago
	end

MythicStone_EventScript_TryRemoveRegidrago::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

MythicStone_OnLoad:
	call_if_unset FLAG_SYS_REGIDRAGO_PUZZLE_COMPLETED, MythicStone_EventScript_HideRegiEntrance
	end

MythicStone_EventScript_HideRegiEntrance::
	setmetatile 7, 19, METATILE_Cave_EntranceCover, TRUE
	setmetatile 8, 19, METATILE_Cave_EntranceCover, TRUE
	setmetatile 9, 19, METATILE_Cave_EntranceCover, TRUE
	setmetatile 7, 20, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 8, 20, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 9, 20, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	return

MythicStone_OnTransition:
	setflag FLAG_LANDMARK_MYTHIC_STONE
	goto_if_set FLAG_CAUGHT_REGIDRAGO, MythicStone_EventScript_HideRegidrago
	goto_if_set FLAG_DEFEATED_REGIDRAGO, MythicStone_EventScript_HideRegidrago
	clearflag FLAG_TEMP_9
	end

MythicStone_EventScript_HideRegidrago::
	clearflag FLAG_TEMP_9
	end

MythicStone_EventScript_CaveEntranceMiddle::
	lockall
	goto_if_set FLAG_SYS_REGIDRAGO_PUZZLE_COMPLETED, MythicStone_EventScript_BigHoleInWall
	braillemsgbox MythicStone_Braille_FierceStrength
	end

MythicStone_EventScript_BigHoleInWall::
	msgbox gText_BigHoleInTheWall, MSGBOX_DEFAULT
	releaseall
	end

MythicStone_EventScript_CaveEntranceSide::
	lockall
	braillemsgbox MythicStone_Braille_FierceStrength
	end

MythicStone_EventScript_Regidrago::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_REGIDRAGO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_REGIDRAGO, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special StartRegiBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MythicStone_EventScript_DefeatedRegidrago
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MythicStone_EventScript_RanFromRegidrago
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MythicStone_EventScript_RanFromRegidrago
	setflag FLAG_DEFEATED_REGIDRAGO
	release
	end

MythicStone_EventScript_DefeatedRegidrago::
	setflag FLAG_DEFEATED_REGIDRAGO
	goto Common_EventScript_RemoveStaticPokemon
	end

MythicStone_EventScript_RanFromRegidrago::
	setflag FLAG_TEMP_9
	setvar VAR_0x8004, SPECIES_REGIDRAGO
	goto Common_EventScript_LegendaryFlewAway
	end
