MemoryGirl_EventScript_BeginConversation::
	lock
	faceplayer
	msgbox MemoryGirl_Text_ICanReadMonMemory, MSGBOX_YESNO
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
	msgbox MemoryGirl_Text_LetsSeeWhatElse, MSGBOX_DEFAULT
	getmemory VAR_0x8004, MON_MEMORY_OLD
	goto_if_le VAR_RESULT, 0, MemoryGirl_EventScript_ReadNewestMemory
	buffermemorymessage VAR_0x8004, MON_MEMORY_OLD
	msgbox gStringVar4, MSGBOX_DEFAULT
	msgbox MemoryGirl_Text_LetsSeeWhatElse, MSGBOX_DEFAULT
MemoryGirl_EventScript_ReadNewestMemory:
	getmemory VAR_0x8004, MON_MEMORY_NEW
	goto_if_le VAR_RESULT, 0, MemoryGirl_EventScript_NoMemoriesToRead
	buffermemorymessage VAR_0x8004, MON_MEMORY_NEW
	msgbox gStringVar4, MSGBOX_DEFAULT
	release
	end

MemoryGirl_EventScript_NoMemoriesToRead::
	msgbox MemoryGirl_Text_NoMemoriesToRead, MSGBOX_DEFAULT
	release
	end

MemoryGirl_EventScript_CantReadEgg::
	msgbox MemoryGirl_Text_ThatIsJustAnEgg, MSGBOX_DEFAULT
	release
	end
