MemoryGirl_EventScript_BeginConversation::
	lock
	faceplayer
	msgbox MemoryGirl_Text_ICanReadMonMemory, MSGBOX_DEFAULT
	msgbox MemoryGirl_Text_ReadMonMemoryNow, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MemoryGirl_EventScript_ChooseMonToRead
	goto_if_eq VAR_RESULT, NO, MemoryGirl_EventScript_DeclineNameRead
	end

MemoryGirl_EventScript_ChooseMonToRead::
	msgbox MemoryGirl_Text_ReadWhichMonMemory, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	goto_if_ne VAR_0x8004, PARTY_NOTHING_CHOSEN, MemoryGirl_EventScript_ReadMonMemory
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, MemoryGirl_EventScript_DeclineNameRead
	end

MemoryGirl_EventScript_DeclineNameRead::
	msgbox MemoryGirl_Text_ComeBackAnytime, MSGBOX_DEFAULT
	release
	end

MemoryGirl_EventScript_ReadMonMemory::
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, MemoryGirl_EventScript_CantReadEgg
	special BufferMonNickname
	buffermemorymessage VAR_0x8004, MON_MEMORY_ORIGIN
	msgbox gStringVar4, MSGBOX_DEFAULT
	call MemoryGirl_EventScript_LetsSeeWhatElse
	getmemory VAR_0x8004, MON_MEMORY_OLD
	copyvar VAR_TEMP_8, VAR_RESULT
	goto_if_eq VAR_RESULT, 0, MemoryGirl_EventScript_SkipToNewestMemory
	buffermemorymessage VAR_0x8004, MON_MEMORY_OLD
	msgbox gStringVar4, MSGBOX_DEFAULT
MemoryGirl_EventScript_SkipToNewestMemory:
	getmemory VAR_0x8004, MON_MEMORY_NEW
	goto_if_eq VAR_RESULT, 0, MemoryGirl_EventScript_NoMemoryToRead
	goto_if_eq VAR_RESULT, VAR_TEMP_8, MemoryGirl_EventScript_NoMemoryToRead
	call_if_ne VAR_TEMP_8, 0, MemoryGirl_EventScript_AndAlso
	buffermemorymessage VAR_0x8004, MON_MEMORY_NEW
	msgbox gStringVar4, MSGBOX_DEFAULT
	special IsMonOTIDNotPlayers
	goto_if_eq VAR_RESULT, TRUE, MemoryGirl_EventScript_WellTravelledMon
	specialvar VAR_RESULT, MonOTNameNotPlayer
	goto_if_eq VAR_RESULT, TRUE, MemoryGirl_EventScript_WellTravelledMon
	specialvar VAR_RESULT, GiveMonHistoricRibbon
	special BufferMonNickname
	msgbox MemoryGirl_Text_YouveHadGreatTimes, MSGBOX_DEFAULT
	call_if_eq VAR_RESULT, TRUE, MemoryGirl_EventScript_GiveHistoricRibbon
	release
	end

MemoryGirl_EventScript_LetsSeeWhatElse::
	msgbox MemoryGirl_Text_LetsSeeWhatElse, MSGBOX_DEFAULT
	return

MemoryGirl_EventScript_AndAlso::
	msgbox MemoryGirl_Text_AndAlso, MSGBOX_DEFAULT
	return

MemoryGirl_EventScript_NoMemoryToRead::
	msgbox MemoryGirl_Text_NoMemoriesToRead, MSGBOX_DEFAULT
	release
	end

MemoryGirl_EventScript_CantReadEgg::
	msgbox MemoryGirl_Text_ThatIsJustAnEgg, MSGBOX_DEFAULT
	release
	end

MemoryGirl_EventScript_WellTravelledMon::
	specialvar VAR_RESULT, GiveMonTravellerRibbon
	special BufferMonNickname
	msgbox MemoryGirl_Text_WellTravelledMon, MSGBOX_DEFAULT
	call_if_eq VAR_RESULT, TRUE, MemoryGirl_EventScript_GiveTravellerRibbon
	release
	end

MemoryGirl_EventScript_GiveHistoricRibbon::
	msgbox MemoryGirl_Text_PleaseGiveItThisHistoricRibbon, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message MemoryGirl_Text_ReceivedHistoricRibbon
	waitfanfare
	msgbox MemoryGirl_Text_PutHistoricRibbonOnMon, MSGBOX_DEFAULT
	return

MemoryGirl_EventScript_GiveTravellerRibbon::
	msgbox MemoryGirl_Text_PleaseGiveItThisTravellerRibbon, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message MemoryGirl_Text_ReceivedTravellerRibbon
	waitfanfare
	msgbox MemoryGirl_Text_PutTravellerRibbonOnMon, MSGBOX_DEFAULT
	return
