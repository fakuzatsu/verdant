.set LOCALID_RECEPTIONIST, 1
.set LOCALID_LINK_RECEPTIONIST, 2

SlateportCity_ContestLobby_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SlateportCity_ContestLobby_SetContestAsVerdanturf
	.byte 0

SlateportCity_ContestLobby_SetContestAsVerdanturf:
	setvar VAR_CONTEST_LOCATION, SLATEPORT_CONTEST
	end

SlateportCity_ContestLobby_EventScript_ChangingRoomGuard::
	lock
	faceplayer
	msgbox SlateportCity_ContestLobby_Text_CantLetYouInHere, MSGBOX_DEFAULT
	release
	end

SlateportCity_ContestLobby_EventScript_BlendingPokefan::
	lock
	faceplayer
	msgbox SlateportCity_ContestLobby_Text_FanaticalAboutPokeBlocks, MSGBOX_DEFAULT
	release
	end

SlateportCity_ContestLobby_EventScript_BlendingSailor1::
	lock
	faceplayer
	msgbox SlateportCity_ContestLobby_Text_NiceToBeOnLand, MSGBOX_DEFAULT
	release
	end

SlateportCity_ContestLobby_EventScript_BlendingSailor2::
	lock
	faceplayer
	msgbox SlateportCity_ContestLobby_Text_OtherSailorsArePreparingShip, MSGBOX_DEFAULT
	release
	end

SlateportCity_ContestLobby_Text_CantLetYouInHere:
	.string "Sorry, this room is reserved for elite\n"
	.string "contest participants only.$"

SlateportCity_ContestLobby_Text_FanaticalAboutPokeBlocks:
	.string "{POKEBLOCK}S {POKEBLOCK}S {POKEBLOCK}S!\p"
	.string "I just can't get enough of them!$"

SlateportCity_ContestLobby_Text_NiceToBeOnLand:
	.string "It's nice to be on land for a while.$"

SlateportCity_ContestLobby_Text_OtherSailorsArePreparingShip:
	.string "The other sailors are preparing our\n"
	.string "boat right now.$"
