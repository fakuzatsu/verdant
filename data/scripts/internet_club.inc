InternetClub_OnLoad::
    goto_if_set FLAG_SYS_INTERNET_OPTIONS_ENABLE, InternetClub_EventScript_DisableTutorial
    end

InternetClub_EventScript_DisableTutorial::
    setvar VAR_TEMP_1, 1
    end

InternetClub_OnFrame::
	map_script_2 VAR_TEMP_1, 0, InternetClub_EventScript_CheckTutorial
	.2byte 0

InternetClub_EventScript_CheckTutorial::
	lockall
	setvar VAR_TEMP_1, 1
    goto_if_unset FLAG_SYS_INTERNET_OPTIONS_ENABLE, InternetClub_EventScript_Tutorial
    release
    end

InternetClub_EventScript_Tutorial::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox InternetClub_Text_FirstTimeRightThisWay, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, InternetClub_Movement_PlayerApproachCounter
	waitmovement 0
	delay 30
	msgbox InternetClub_Text_ExplainWirelessClubFirstTime, MSGBOX_DEFAULT
	setflag FLAG_SYS_INTERNET_OPTIONS_ENABLE
	releaseall
	end

InternetClub_EventScript_InternetClubAttendant::
    lock
    faceplayer
    msgbox InternetClub_Text_AccessFromMainMenu, MSGBOX_DEFAULT
    release
    return

InternetClub_Text_FirstTimeRightThisWay::
	.string "Hello!\n"
	.string "My name is Winola.\p"
	.string "This must be your first time\n"
	.string "down here.\p"
	.string "I'll show you how the Internet\n"
	.string "Communication System works.\p"
	.string "Right this way, please.$"

InternetClub_Text_ExplainWirelessClubFirstTime::
	.string "To connect, you will need a\n"
    .string "Mobile Adapter.\p"
    .string "You can find more information\n"
    .string "about this from the Main Menu.\p"
    .string "From there you can Mix Records\n"
    .string "with trainers online…\p"
    .string "as well as claim Mystery Gifts!\p"
    .string "This room is where you can\n"
    .string "communicate with other Trainers\l"
    .string "who use the Internet…\p"
    .string "and check out the Globe to see all\n"
    .string "the places you've connected with.\p"
    .string "…Just one moment…\p"
    .string "I have now approved you for\n"
    .string "online features.\p"
	.string "Once you save your game, you can\n"
	.string "access the Internet Menu.$"

InternetClub_Text_AccessFromMainMenu::
    .string "To use Internet Features, you must\n"
    .string "access them from the Main Menu\p"
    .string "Thank you for your patronage.$"

InternetClub_Movement_PlayerApproachCounter:
	walk_up
	step_end
