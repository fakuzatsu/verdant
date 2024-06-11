Common_EventScript_PkmnCenterBattleMaster::
    lock
    faceplayer
	message gText_ImTheBattleMaster
	waitmessage
    waitbuttonpress
    goto_if_eq VAR_LEVEL_CAP, 0, Common_EventScript_PkmnCenterBattleMaster_NoLevelCap
    goto_if_ge VAR_LEVEL_CAP, 100, Common_EventScript_PkmnCenterBattleMaster_NoLevelCap
    goto Common_EventScript_PkmnCenterBattleMaster_EffectiveLevelCap

Common_EventScript_PkmnCenterBattleMaster_NoLevelCap::
    goto_if_set FLAG_IS_CHAMPION, Common_EventScript_PkmnCenterBattleMaster_StoryBeaten
    message gText_NoEffectiveCap
    waitmessage
    waitbuttonpress
    msgbox gText_TurnLevelCapsOn, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, YES, Common_EventScript_PkmnCenterBattleMaster_TurnLevelCapOn
    release
    end

Common_EventScript_PkmnCenterBattleMaster_StoryBeaten::
    message gText_YouHaveOvercomeAllChallenges
    waitmessage
    waitbuttonpress
    release
    end

Common_EventScript_PkmnCenterBattleMaster_EffectiveLevelCap::
    specialvar VAR_TEMP_1, GetLevelCapByFlag
    setvar VAR_LEVEL_CAP, VAR_TEMP_1
    buffernumberstring STR_VAR_2, VAR_TEMP_1
    message gText_EffectiveCap
    waitmessage
    waitbuttonpress
    message gText_TurnLevelCapsOff, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, YES, Common_EventScript_PkmnCenterBattleMaster_TurnLevelCapOff
    release
    end

Common_EventScript_PkmnCenterBattleMaster_TurnLevelCapOn::
    specialvar VAR_TEMP_1, GetLevelCapByFlag
    setvar VAR_LEVEL_CAP, VAR_TEMP_1
    message gText_OkItIsOn
    waitmessage
    waitbuttonpress
    release
    end

Common_EventScript_PkmnCenterBattleMaster_TurnLevelCapOff::
    setvar VAR_LEVEL_CAP, 0
    message gText_OkItIsOff
    waitmessage
    waitbuttonpress
    release
    end

