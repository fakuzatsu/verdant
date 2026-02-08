SealedChamber_InnerRoom_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SealedChamber_InnerRoom_OnLoad
	.byte 0

SealedChamber_InnerRoom_OnLoad::
    goto_if_unset FLAG_REGIGIGAS_DOOR_OPENED, SealedChamber_InnerRoom_EventScript_SetSolidWall
    end

SealedChamber_InnerRoom_EventScript_BrailleBackWall::
	lockall
    goto_if_set FLAG_REGIGIGAS_DOOR_OPENED, SealedChamber_InnerRoom_EventScript_CollapsedWall
	braillemsgbox SealedChamber_InnerRoom_Braille_FirstWailordLastRelicanth
	goto_if_set FLAG_REGI_DOORS_OPENED, SealedChamber_InnerRoom_EventScript_CheckForRegis
	specialvar VAR_RESULT, CheckRelicanthWailord
	goto_if_eq VAR_RESULT, FALSE, SealedChamber_InnerRoom_EventScript_NoEffect
	fadeoutbgm 0
	playse SE_TRUCK_MOVE
	special DoSealedChamberShakingEffect_Long
	waitstate
	delay 40
	special DoSealedChamberShakingEffect_Short
	waitstate
	playse SE_DOOR
	delay 40
	special DoSealedChamberShakingEffect_Short
	waitstate
	playse SE_DOOR
	delay 40
	special DoSealedChamberShakingEffect_Short
	waitstate
	playse SE_DOOR
	delay 40
	special DoSealedChamberShakingEffect_Short
	waitstate
	playse SE_DOOR
	delay 40
	special DoSealedChamberShakingEffect_Short
	waitstate
	playse SE_DOOR
	delay 40
	msgbox gText_DoorOpenedFarAway, MSGBOX_DEFAULT
	closemessage
	fadeinbgm 0
	setflag FLAG_REGI_DOORS_OPENED
SealedChamber_InnerRoom_EventScript_NoEffect::
	releaseall
	end

SealedChamber_InnerRoom_EventScript_CheckForRegis::
    specialvar VAR_RESULT, CheckAllRegis
    goto_if_eq VAR_RESULT, FALSE, SealedChamber_InnerRoom_EventScript_NoEffect
	fadeoutbgm 0
	playse SE_TRUCK_MOVE
	special DoSealedChamberShakingEffect_Long
	waitstate
	delay 40
    call SealedChamber_InnerRoom_EventScript_SetCollapsedWall
    special DrawWholeMapView
    playse SE_BANG
    removeobject LOCALID_BRAILLE_CRUMBLED_WALL
    setflag FLAG_REGIGIGAS_DOOR_OPENED
	releaseall
	end

SealedChamber_InnerRoom_EventScript_SetSolidWall::
    setmetatile 9, 3, METATILE_Cave_SealedChamberInterior_Wall, TRUE
    setmetatile 9, 4, METATILE_Cave_SealedChamberInterior_TextWall, TRUE
    setmetatile 10, 3, METATILE_Cave_SealedChamberInterior_Wall, TRUE
    setmetatile 10, 4, METATILE_Cave_SealedChamberInterior_TextWall, TRUE
    setmetatile 11, 3, METATILE_Cave_SealedChamberInterior_Wall, TRUE
    setmetatile 11, 4, METATILE_Cave_SealedChamberInterior_TextWall, TRUE
    return

SealedChamber_InnerRoom_EventScript_SetCollapsedWall::
    setmetatile 9, 3, METATILE_Cave_SealedChamberInterior_TopLeft, TRUE
    setmetatile 9, 4, METATILE_Cave_SealedChamberInterior_BottomLeft, TRUE
    setmetatile 10, 3, METATILE_Cave_SealedChamberInterior_TopMid, TRUE
    setmetatile 10, 4, METATILE_Cave_SealedChamberInterior_BottomMid, FALSE
    setmetatile 11, 3, METATILE_Cave_SealedChamberInterior_TopRight, TRUE
    setmetatile 11, 4, METATILE_Cave_SealedChamberInterior_BottomRight, TRUE
    return

SealedChamber_InnerRoom_EventScript_CollapsedWall::
    braillemsgbox SealedChamber_InnerRoom_Braille_LastLord
	releaseall
	end

SealedChamber_InnerRoom_EventScript_BrailleStoryPart1::
	lockall
	braillemsgbox SealedChamber_InnerRoom_Braille_InThisCaveWeHaveLived
	releaseall
	end

SealedChamber_InnerRoom_EventScript_BrailleStoryPart2::
	lockall
	braillemsgbox SealedChamber_InnerRoom_Braille_WeOweAllToThePokemon
	releaseall
	end

SealedChamber_InnerRoom_EventScript_BrailleStoryPart3::
	lockall
	braillemsgbox SealedChamber_InnerRoom_Braille_ButWeSealedThePokemonAway
	releaseall
	end

SealedChamber_InnerRoom_EventScript_BrailleStoryPart4::
	lockall
	braillemsgbox SealedChamber_InnerRoom_Braille_WeFearedIt
	releaseall
	end

SealedChamber_InnerRoom_EventScript_BrailleStoryPart5::
	lockall
	braillemsgbox SealedChamber_InnerRoom_Braille_ThoseWithCourageHope
	releaseall
	end

SealedChamber_InnerRoom_EventScript_BrailleStoryPart6::
	lockall
	braillemsgbox SealedChamber_InnerRoom_Braille_OpenDoorEternalPokemonWaits
	releaseall
	end

