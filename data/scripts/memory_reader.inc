MemoryGirl_EventScript_BeginConversation::
	lock
	faceplayer
	msgbox MemoryGirl_Text_PleasedToReadMonMemory, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MemoryGirl_EventScript_ChooseMonToRead
	goto_if_eq VAR_RESULT, NO, MemoryGirl_EventScript_DeclineNameRead
	end

MemoryGirl_EventScript_ChooseMonToRead::
	msgbox MemoryGirl_Text_CritiqueWhichMonMemory, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	goto_if_ne VAR_0x8004, PARTY_NOTHING_CHOSEN, MemoryGirl_EventScript_ReadMonMemory
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, MemoryGirl_EventScript_DeclineNameRead
	end

MemoryGirl_EventScript_DeclineNameRead::
	msgbox MemoryGirl_Text_DoVisitAgain, MSGBOX_DEFAULT
	release
	end

MemoryGirl_EventScript_ReadMonMemory::
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, MemoryGirl_EventScript_CantReadEgg
	special BufferMonNickname
	special IsMonOTIDNotPlayers
	goto_if_eq VAR_RESULT, TRUE, MemoryGirl_EventScript_PlayerNotMonsOT
	specialvar VAR_RESULT, MonOTNameNotPlayer
	special BufferMonNickname
	goto_if_eq VAR_RESULT, TRUE, MemoryGirl_EventScript_PlayerNotMonsOT
	msgbox MemoryGirl_Text_FineNameSuggestBetterOne, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MemoryGirl_EventScript_ChangeMemory
	goto_if_eq VAR_RESULT, NO, MemoryGirl_EventScript_DeclineNameRead
	end

MemoryGirl_EventScript_CantReadEgg::
	msgbox MemoryGirl_Text_ThatIsMerelyAnEgg, MSGBOX_DEFAULT
	release
	end

MemoryGirl_EventScript_PlayerNotMonsOT::
	msgbox MemoryGirl_Text_MagnificentName, MSGBOX_DEFAULT
	release
	end

MemoryGirl_EventScript_ChangeMemory::
	msgbox MemoryGirl_Text_WhatShallNewNameBe, MSGBOX_DEFAULT
	call Common_EventScript_NameReceivedPartyMon
	special BufferMonNickname
	goto_if_eq VAR_RESULT, TRUE, MemoryGirl_EventScript_NewNameDifferent
	msgbox MemoryGirl_Text_NameNoDifferentYetSuperior, MSGBOX_DEFAULT
	release
	end

MemoryGirl_EventScript_NewNameDifferent::
	msgbox MemoryGirl_Text_MonShallBeKnownAsName, MSGBOX_DEFAULT
	release
	end

MemoryGirl_Text_PleasedToReadMonMemory:
	.string "Hi, hi! I'm the NAME ReadR!\n"
	.string "I'm the fortune-teller of names!\p"
	.string "I shall be pleased to Read your\n"
	.string "POKéMON's Memory.$"

MemoryGirl_Text_CritiqueWhichMonMemory:
	.string "Which POKéMON's Memory should\n"
	.string "I critique?$"

MemoryGirl_Text_FineNameSuggestBetterOne:
	.string "Hmmm… {STR_VAR_1}, is it? That is\n"
	.string "quite a fine name you bestowed.\p"
	.string "But! What say you, if I were to\n"
	.string "suggest a slightly better name?$"

MemoryGirl_Text_WhatShallNewNameBe:
	.string "Ah, good. Then, what shall the new\n"
	.string "Memory be?$"

MemoryGirl_Text_MonShallBeKnownAsName:
	.string "Done! From now on, this POKéMON\n"
	.string "shall be known as {STR_VAR_1}!\p"
	.string "It is a better name than before!\n"
	.string "How fortunate for you!$"

MemoryGirl_Text_DoVisitAgain:
	.string "I see.\n"
	.string "Do come visit again.$"

MemoryGirl_Text_NameNoDifferentYetSuperior:
	.string "Done! From now on, this POKéMON\n"
	.string "shall be known as {STR_VAR_1}!\p"
	.string "It looks no different from before,\n"
	.string "and yet, this is vastly superior!\p"
	.string "How fortunate for you!$"

MemoryGirl_Text_MagnificentName:
	.string "Hmmm… {STR_VAR_1} it is!\p"
	.string "This is a magnificent Memory!\n"
	.string "It is impeccably beyond reproach!\p"
	.string "You'll do well to cherish your\n"
	.string "{STR_VAR_1} now and beyond.$"

MemoryGirl_Text_ThatIsMerelyAnEgg:
	.string "Now, now.\n"
	.string "That is merely an EGG!$"

MemoryStrings_NoMemory::
    .string "."

MemoryStrings_WonACoolContest::
    .string "."

MemoryStrings_WonABeautyContest::
    .string "."

MemoryStrings_WonACuteContest::
    .string "."

MemoryStrings_WonACleverContest::
    .string "."

MemoryStrings_WonAToughContest::
    .string "."

MemoryStrings_AteACoolPokeblock::
    .string "."

MemoryStrings_AteABeautyPokeblock::
    .string "."

MemoryStrings_AteACutePokeblock::
    .string "."

MemoryStrings_AteACleverPokeblock::
    .string "."

MemoryStrings_AteAToughPokeblock::
    .string "."

MemoryStrings_AteADislikedPokeblock::
    .string "."

MemoryStrings_AteALovedPokeblock::
    .string "."

MemoryStrings_MaxedOutSheen::
    .string "."

MemoryStrings_FoughtRoxanneGym::
    .string "."

MemoryStrings_FoughtBrawlyGym::
    .string "."

MemoryStrings_FoughtWattsonGym::
    .string "."

MemoryStrings_FoughtFlanneryGym::
    .string "."

MemoryStrings_FoughtNormanGym::
    .string "."

MemoryStrings_FoughtWinonaGym::
    .string "."

MemoryStrings_FoughtTateAndLizaGym::
    .string "."

MemoryStrings_FoughtWallaceGym::
    .string "."

MemoryStrings_FoughtTheEliteFour::
    .string "."

MemoryStrings_FoughtASignificant::
    .string "."

MemoryStrings_EarnedSilverFrontier::
    .string "."

MemoryStrings_CaughtALegendary::
    .string "."

MemoryStrings_DefendedChampionTitle::
    .string "."

MemoryStrings_WonAMasterContest::
    .string "."

MemoryStrings_EarnedGoldFrontier::
    .string "."

MemoryStrings_WithX::
    .string "\lalongside {STR_VAR_2}."

MemoryStrings_GivenByX::
    .string "\lgiven by {STR_VAR_2}."