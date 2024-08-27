Route119_North_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Route119_North_OnLoad
	.byte 0

Route119_North_OnLoad:
	call_if_unset FLAG_REGI_DOORS_OPENED, Route119_North_EventScript_CloseThunderRock
	end

Route119_North_EventScript_CloseThunderRock::
	setmetatile 7, 5, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 7, 6, METATILE_General_RockWall_GrassBase, TRUE
	return
