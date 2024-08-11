.set VAR_LEFTRIGHT, VAR_TEMP_6
.set VAR_TOPBOTTOM, VAR_TEMP_7
.set VAR_CLOCKWISE, VAR_TEMP_8
.set VAR_ANTICLOCKWISE, VAR_TEMP_9

Grotto09_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Grotto09_MapScript_OnTransition
	.byte 0

Grotto09_MapScript_OnTransition::
	setflag FLAG_TEMP_5
	end

Grotto09_EventScript_BottomLeftVariation0::
	setmetatile 8, 16, METATILE_General_TallGrass, FALSE
	setmetatile 8, 17, METATILE_General_TallGrass, FALSE
	setmetatile 6, 18, METATILE_General_Grass, FALSE
	setmetatile 7, 18, METATILE_General_Grass, FALSE
	setmetatile 7, 19, METATILE_General_Grass, FALSE
	setmetatile 5, 19, METATILE_General_TallGrass, FALSE
	setmetatile 9, 20, METATILE_General_TallGrass, FALSE
	special DrawWholeMapView
	releaseall
	end

Grotto09_EventScript_BottomLeftVariation1::
	setmetatile 8, 16, METATILE_General_Grass, FALSE
	setmetatile 8, 17, METATILE_General_TallGrass, FALSE
	setmetatile 6, 18, METATILE_General_Grass, FALSE
	setmetatile 7, 18, METATILE_General_TallGrass, FALSE
	setmetatile 7, 19, METATILE_General_TallGrass, FALSE
	setmetatile 5, 19, METATILE_General_TallGrass, FALSE
	setmetatile 9, 20, METATILE_General_Grass, FALSE
	special DrawWholeMapView
	releaseall
	end

Grotto09_EventScript_BottomLeftVariation2::
	setmetatile 8, 16, METATILE_General_Grass, FALSE
	setmetatile 8, 17, METATILE_General_Grass, FALSE
	setmetatile 6, 18, METATILE_General_TallGrass, FALSE
	setmetatile 7, 18, METATILE_General_TallGrass, FALSE
	setmetatile 7, 19, METATILE_General_TallGrass, FALSE
	setmetatile 5, 19, METATILE_General_Grass, FALSE
	setmetatile 9, 20, METATILE_General_Grass, FALSE
	special DrawWholeMapView
	releaseall
	end

Grotto09_EventScript_TopRightVariation0::
	setmetatile 18, 8, METATILE_General_TallGrass, FALSE
	setmetatile 18, 9, METATILE_General_TallGrass, FALSE
	setmetatile 19, 8, METATILE_General_TallGrass, FALSE
	setmetatile 22, 6, METATILE_General_Grass, FALSE
	setmetatile 23, 6, METATILE_General_Grass, FALSE
	setmetatile 22, 7, METATILE_General_TallGrass, FALSE
	setmetatile 25, 7, METATILE_General_TallGrass, FALSE
	special DrawWholeMapView
	releaseall
	end

Grotto09_EventScript_TopRightVariation1::
	setmetatile 18, 8, METATILE_General_TallGrass, FALSE
	setmetatile 18, 9, METATILE_General_TallGrass, FALSE
	setmetatile 19, 8, METATILE_General_Grass, FALSE
	setmetatile 22, 6, METATILE_General_Grass, FALSE
	setmetatile 23, 6, METATILE_General_TallGrass, FALSE
	setmetatile 22, 7, METATILE_General_TallGrass, FALSE
	setmetatile 25, 7, METATILE_General_Grass, FALSE
	special DrawWholeMapView
	releaseall
	end

Grotto09_EventScript_TopRightVariation2::
	setmetatile 18, 8, METATILE_General_Grass, FALSE
	setmetatile 18, 9, METATILE_General_Grass, FALSE
	setmetatile 19, 8, METATILE_General_TallGrass, FALSE
	setmetatile 22, 6, METATILE_General_TallGrass, FALSE
	setmetatile 23, 6, METATILE_General_TallGrass, FALSE
	setmetatile 22, 7, METATILE_General_Grass, FALSE
	setmetatile 25, 7, METATILE_General_Grass, FALSE
	clearflag FLAG_TEMP_5
	special DrawWholeMapView
	releaseall
	end

Grotto09_EventScript_NorthTriggerInner::
	call_if_eq VAR_LEFTRIGHT, 0, Grotto09_EventScript_SetTopBottomOne
	switch VAR_ANTICLOCKWISE
	case 0, Grotto09_EventScript_BottomLeftVariation0
	case 1, Grotto09_EventScript_BottomLeftVariation1
	case 2, Grotto09_EventScript_BottomLeftVariation1
	case 3, Grotto09_EventScript_BottomLeftVariation2
	case 4, Grotto09_EventScript_BottomLeftVariation2
	releaseall
	end

Grotto09_EventScript_NorthTriggerOuter::
	setvar VAR_TOPBOTTOM, 0
	call_if_eq VAR_LEFTRIGHT, 1, Grotto09_EventScript_TravelledClockwise
	releaseall
	end

Grotto09_EventScript_EastTriggerInner::
	call_if_eq VAR_TOPBOTTOM, 0, Grotto09_EventScript_SetLeftRightOne
	switch VAR_CLOCKWISE
	case 0, Grotto09_EventScript_BottomLeftVariation0
	case 1, Grotto09_EventScript_BottomLeftVariation1
	case 2, Grotto09_EventScript_BottomLeftVariation1
	case 3, Grotto09_EventScript_BottomLeftVariation2
	case 4, Grotto09_EventScript_BottomLeftVariation2
	releaseall
	end

Grotto09_EventScript_EastTriggerOuter::
	setvar VAR_LEFTRIGHT, 0
	call_if_eq VAR_TOPBOTTOM, 1, Grotto09_EventScript_TravelledAntiClockwise
	releaseall
	end

Grotto09_EventScript_SouthTriggerInner::
	call_if_eq VAR_LEFTRIGHT, 0, Grotto09_EventScript_SetTopBottomOne
	switch VAR_ANTICLOCKWISE
	case 0, Grotto09_EventScript_TopRightVariation0
	case 1, Grotto09_EventScript_TopRightVariation1
	case 2, Grotto09_EventScript_TopRightVariation1
	case 3, Grotto09_EventScript_TopRightVariation2
	case 4, Grotto09_EventScript_TopRightVariation2
	releaseall
	end

Grotto09_EventScript_SouthTriggerOuter::
	call_if_eq VAR_LEFTRIGHT, 1, Grotto09_EventScript_TravelledClockwise
	setvar VAR_TOPBOTTOM, 0
	releaseall
	end

Grotto09_EventScript_WestTriggerInner::
	call_if_eq VAR_TOPBOTTOM, 0, Grotto09_EventScript_SetLeftRightOne
	switch VAR_CLOCKWISE
	case 0, Grotto09_EventScript_TopRightVariation0
	case 1, Grotto09_EventScript_TopRightVariation1
	case 2, Grotto09_EventScript_TopRightVariation1
	case 3, Grotto09_EventScript_TopRightVariation2
	case 4, Grotto09_EventScript_TopRightVariation2
	releaseall
	end

Grotto09_EventScript_WestTriggerOuter::
	call_if_eq VAR_TOPBOTTOM, 1, Grotto09_EventScript_TravelledAntiClockwise
	setvar VAR_LEFTRIGHT, 0
	releaseall
	end

Grotto09_EventScript_SetLeftRightOne::
	setvar VAR_LEFTRIGHT, 1
	return

Grotto09_EventScript_SetTopBottomOne::
	setvar VAR_TOPBOTTOM, 1
	return

Grotto09_EventScript_TravelledClockwise::
	call_if_eq VAR_CLOCKWISE, 3, Grotto09_EventScript_SetClockwiseFour
	call_if_eq VAR_CLOCKWISE, 2, Grotto09_EventScript_SetClockwiseThree
	call_if_eq VAR_CLOCKWISE, 1, Grotto09_EventScript_SetClockwiseTwo
	call_if_eq VAR_CLOCKWISE, 0, Grotto09_EventScript_SetClockwiseOne
	setvar VAR_ANTICLOCKWISE, 0
	setvar VAR_LEFTRIGHT, 0
	return

Grotto09_EventScript_SetClockwiseOne::
	setvar VAR_CLOCKWISE, 1
	return

Grotto09_EventScript_SetClockwiseTwo::
	setvar VAR_CLOCKWISE, 2
	return

Grotto09_EventScript_SetClockwiseThree::
	setvar VAR_CLOCKWISE, 3
	return

Grotto09_EventScript_SetClockwiseFour::
	setvar VAR_CLOCKWISE, 4
	return

Grotto09_EventScript_TravelledAntiClockwise::
	call_if_eq VAR_ANTICLOCKWISE, 3, Grotto09_EventScript_SetAntiClockwiseFour
	call_if_eq VAR_ANTICLOCKWISE, 2, Grotto09_EventScript_SetAntiClockwiseThree
	call_if_eq VAR_ANTICLOCKWISE, 1, Grotto09_EventScript_SetAntiClockwiseTwo
	call_if_eq VAR_ANTICLOCKWISE, 0, Grotto09_EventScript_SetAntiClockwiseOne
	setvar VAR_CLOCKWISE, 0
	setvar VAR_TOPBOTTOM, 0
	return
	end

Grotto09_EventScript_SetAntiClockwiseOne::
	setvar VAR_ANTICLOCKWISE, 1
	return

Grotto09_EventScript_SetAntiClockwiseTwo::
	setvar VAR_ANTICLOCKWISE, 2
	return

Grotto09_EventScript_SetAntiClockwiseThree::
	setvar VAR_ANTICLOCKWISE, 3
	return

Grotto09_EventScript_SetAntiClockwiseFour::
	setvar VAR_ANTICLOCKWISE, 4
	return
