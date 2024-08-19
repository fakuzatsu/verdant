EventScript_TestAPIConnection::
    lock
    msgbox Text_TestAPIWantToConnect, MSGBOX_YESNO
    goto_if_eq VAR_RESULT, NO, EventScript_TestAPIAbort
    message Text_TestAPIConnecting
    waitmessage
    internetconnection INTERNET_TEST_CONNECTION
    closemessage
    switch VAR_RESULT
    case INTERNET_STATE_MA_ERR, EventScript_TestAPIMaFailure
    case INTERNET_STATE_INPUT_ERR, EventScript_TestAPIInputFailure
    case INTERNET_STATE_SUCCESS, EventScript_TestAPISuccess
    goto EventScript_TestAPIFailure
    end

EventScript_TestAPIFailure:
    msgbox Text_TestAPIFailure, MSGBOX_DEFAULT
EventScript_TestAPIAbort:
    release
    end

EventScript_TestAPIMaFailure:
    msgbox Text_TestAPIMaFailure, MSGBOX_DEFAULT
    release
    end

EventScript_TestAPIInputFailure:
    msgbox Text_TestAPIInputFailure, MSGBOX_DEFAULT
    release
    end

EventScript_TestAPISuccess::
    msgbox Text_TestAPISuccess, MSGBOX_DEFAULT
    release
    end
