Underwater_Route128_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Underwater_Route128_OnLoad
	.byte 0

Underwater_Route128_OnLoad:
	call_if_unset FLAG_MANAPHY_DOOR_OPEN, Underwater_Route128_CloseManaphyDoor
	end

Underwater_Route128_CloseManaphyDoor::
    setmetatile 77, 17, METATILE_Underwater_RockWall, TRUE
    setmetatile 77, 18, METATILE_Underwater_RockWall, TRUE
    end
