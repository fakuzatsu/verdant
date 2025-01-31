#ifndef GUARD_TM_CASE_H
#define GUARD_TM_CASE_H

// Values for 'type' argument to InitTMCase
enum TMCaseType
{
    TMCASE_FROM_FIELD,
    TMCASE_FROM_GIVE_PARTY,
    TMCASE_FROM_MART_SELL,
    TMCASE_FROM_GIVE_PC,
    TMCASE_FROM_BATTLE,
    TMCASE_REOPENING
};

// Alternative value for 'allowSelectClose' argument to InitTMCase.
// Indicates that the previous value should be preserved
#define TMCASE_KEEP_PREV 0xFF

void InitTMCase(u8 type, void (* exitCallback)(void), u8 allowSelectClose);
void ResetTMCaseCursorPos(void);
void Pokedude_InitTMCase(void);

#endif //GUARD_TM_CASE_H
