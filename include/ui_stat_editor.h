#ifndef GUARD_UI_STAT_EDIT_H
#define GUARD_UI_STAT_EDIT_H

#include "main.h"

void Task_OpenStatEditorFromStartMenu(u8 taskId);
void StatEditor_Init(MainCallback callback);

extern const struct SpeciesInfo gSpeciesInfo[];
extern const struct Ability gAbilitiesInfo[];
extern const struct NatureInfo gNaturesInfo[];


#endif // GUARD_UI_MENU_H
