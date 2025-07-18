#ifndef GUARD_UI_MENU_H
#define GUARD_UI_MENU_H

#include "main.h"
#include "data.h"

void Task_OpenBO3MenuFromStartMenu(u8 taskId);
void BO3Menu_Init(u32 trainerPicId, const struct TrainerMon *opponentTeam, u8 opponentTeamSize, const u8 *opponentName, MainCallback callback);


#endif // GUARD_UI_MENU_H