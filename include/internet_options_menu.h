#ifndef GUARD_InternetOptions_MENU_H
#define GUARD_InternetOptions_MENU_H

void CB2_InitInternetOptions(void);
void InternetOptions_DrawCheckerboardPattern(u32 bg);

void PrintInternetOptionsTopMenu(bool32 connecting);
void pid_to_fc(u32 pid, u8 *hexdigest);

//extern EWRAM_DATA struct GTSPokedexView *sGTSPokedexView;

#endif // GUARD_InternetOptions_MENU_H