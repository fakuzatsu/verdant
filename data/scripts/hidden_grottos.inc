Grotto_EventScript_GrottoInteract::
	msgbox Grotto_Text_FollowThePath, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Grotto_EventScript_CancelGrotto
    special SetGrottoWarp
    releaseall
    end

Grotto_EventScript_CancelGrotto::
    releaseall
    end

Grotto_Text_FollowThePath:
	.string "Look!\n"
	.string "You've found a narrow path!\p"
	.string "Will you follow it?$"

