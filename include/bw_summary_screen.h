#ifndef GUARD_BW_SUMMARY_SCREEN_H
#define GUARD_BW_SUMMARY_SCREEN_H

#include "main.h"
#include "config/bw_summary_screen.h"

/* Info for users

General tilemap setup
BG3 - scrolling grid background (or not scrolling if you turned the config off) 
BG2 - main UI overlayed on scrolling BG
BG1 - pop in move effect windows
BG0 - text windows

Mosaic effect used when transitioning between screens and actvating
effect windows is controlled by tMosaicStrength in the relevant
task functions.

BG scrolling speed can be modified by altering the value parameter
of the ChangeBgX and ChangeBgY functions in VBlank()

Main UI and shadow transparency levels can be adjusted by changing the
value written to the alpha blend register in this line in bw_summary_screen.c:

static void InitBGs(void)
...
SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(14, 6));
...
}

*/

/* ravetodo in future updates

- hidden power
- extended move desc window
- ribbons

*/

void ShowPokemonSummaryScreen_BW(u8 mode, void *mons, u8 monIndex, u8 maxMonIndex, void (*callback)(void));
void ShowSelectMovePokemonSummaryScreen_BW(struct Pokemon *mons, u8 monIndex, u8 maxMonIndex, void (*callback)(void), u16 newMove);
void ShowPokemonSummaryScreenHandleDeoxys_BW(u8 mode, struct BoxPokemon *mons, u8 monIndex, u8 maxMonIndex, void (*callback)(void));
u8 GetMoveSlotToReplace_BW(void);
void SummaryScreen_SetAnimDelayTaskId_BW(u8 taskId);
void SummaryScreen_SetShadowAnimDelayTaskId_BW(u8 taskId);

// The Pokémon Summary Screen can operate in different modes. Certain features,
// such as move re-ordering, are available in the different modes.
enum PokemonSummaryScreenMode_BW
{
    BW_SUMMARY_MODE_NORMAL,
    BW_SUMMARY_MODE_LOCK_MOVES,
    BW_SUMMARY_MODE_BOX,
    BW_SUMMARY_MODE_SELECT_MOVE,
};

#endif // GUARD_BW_SUMMARY_SCREEN_H
