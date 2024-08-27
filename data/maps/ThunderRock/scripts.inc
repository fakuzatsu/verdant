ThunderRock_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, ThunderRock_OnResume
	map_script MAP_SCRIPT_ON_LOAD, ThunderRock_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, ThunderRock_OnTransition
	.byte 0

ThunderRock_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, ThunderRock_EventScript_TryRemoveRegieleki
	end

ThunderRock_EventScript_TryRemoveRegieleki::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

ThunderRock_OnLoad:
	call_if_unset FLAG_SYS_BRAILLE_REGIELEKI_COMPLETED, ThunderRock_EventScript_HideRegiEntrance
	end

ThunderRock_EventScript_HideRegiEntrance::
	setmetatile 7, 19, METATILE_Cave_EntranceCover, TRUE
	setmetatile 8, 19, METATILE_Cave_EntranceCover, TRUE
	setmetatile 9, 19, METATILE_Cave_EntranceCover, TRUE
	setmetatile 7, 20, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 8, 20, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	setmetatile 9, 20, METATILE_Cave_SealedChamberBraille_Mid, TRUE
	return

ThunderRock_OnTransition:
	setflag FLAG_LANDMARK_THUNDER_ROCK
	call ThunderRock_EventScript_ClearSteps
	goto_if_set FLAG_CAUGHT_REGIELEKI, ThunderRock_EventScript_HideRegieleki
	goto_if_set FLAG_DEFEATED_REGIELEKI, ThunderRock_EventScript_HideRegieleki
	clearflag FLAG_TEMP_9
	end

ThunderRock_EventScript_HideRegieleki::
	clearflag FLAG_TEMP_9
	end

ThunderRock_EventScript_OpenRegiEntrance::
	setmetatile 7, 19, METATILE_Cave_SealedChamberEntrance_TopLeft, TRUE
	setmetatile 8, 19, METATILE_Cave_SealedChamberEntrance_TopMid, TRUE
	setmetatile 9, 19, METATILE_Cave_SealedChamberEntrance_TopRight, TRUE
	setmetatile 7, 20, METATILE_Cave_SealedChamberEntrance_BottomLeft, TRUE
	setmetatile 8, 20, METATILE_Cave_SealedChamberEntrance_BottomMid, FALSE
	setmetatile 9, 20, METATILE_Cave_SealedChamberEntrance_BottomRight, TRUE
	special DrawWholeMapView
	playse SE_BANG
	setflag FLAG_SYS_BRAILLE_REGIELEKI_COMPLETED
	end

ThunderRock_EventScript_CaveEntranceMiddle::
	lockall
	call_if_set FLAG_TEMP_REGIELEKI_PUZZLE_FAILED, ThunderRock_EventScript_ClearSteps
	goto_if_set FLAG_SYS_BRAILLE_REGIELEKI_COMPLETED, ThunderRock_EventScript_BigHoleInWall
	braillemessage ThunderRock_Braille_DownLeftOver
	setflag FLAG_TEMP_REGIELEKI_PUZZLE_STARTED
	special ShouldDoBrailleRegielekiPuzzle
	goto ThunderRock_EventScript_CloseBrailleMsg
	end

ThunderRock_EventScript_BigHoleInWall::
	msgbox gText_BigHoleInTheWall, MSGBOX_DEFAULT
	releaseall
	end

ThunderRock_EventScript_CaveEntranceSide::
	lockall
	call_if_set FLAG_TEMP_REGIELEKI_PUZZLE_FAILED, ThunderRock_EventScript_ClearSteps
	braillemessage ThunderRock_Braille_DownLeftOver
	goto_if_set FLAG_SYS_BRAILLE_REGIELEKI_COMPLETED, ThunderRock_EventScript_CloseBrailleMsg
	setflag FLAG_TEMP_REGIELEKI_PUZZLE_STARTED
	special ShouldDoBrailleRegielekiPuzzle
	goto ThunderRock_EventScript_CloseBrailleMsg
	end

ThunderRock_EventScript_CloseBrailleMsg::
	waitbuttonpress
	closebraillemessage
	releaseall
	end

ThunderRock_EventScript_ClearSteps::
	setvar VAR_REGIELEKI_STEPS, 0
	clearflag FLAG_TEMP_REGIELEKI_PUZZLE_FAILED
	return

ThunderRock_EventScript_Regieleki::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_REGIELEKI, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_REGIELEKI, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special StartRegiBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, ThunderRock_EventScript_DefeatedRegieleki
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, ThunderRock_EventScript_RanFromRegieleki
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, ThunderRock_EventScript_RanFromRegieleki
	setflag FLAG_DEFEATED_REGIELEKI
	release
	end

ThunderRock_EventScript_DefeatedRegieleki::
	setflag FLAG_DEFEATED_REGIELEKI
	goto Common_EventScript_RemoveStaticPokemon
	end

ThunderRock_EventScript_RanFromRegieleki::
	setflag FLAG_TEMP_9
	setvar VAR_0x8004, SPECIES_REGIELEKI
	goto Common_EventScript_LegendaryFlewAway
	end
