#ifndef GUARD_BW_SUMMARY_SCREEN_H
#define GUARD_BW_SUMMARY_SCREEN_H

#include "main.h"
#include "config/bw_summary_screen.h"

//ravetodo
// - hidden power
// - extended move desc window

void ShowPokemonSummaryScreen_BW(u8 mode, void *mons, u8 monIndex, u8 maxMonIndex, void (*callback)(void));
void ShowSelectMovePokemonSummaryScreen_BW(struct Pokemon *mons, u8 monIndex, u8 maxMonIndex, void (*callback)(void), u16 newMove);
void ShowPokemonSummaryScreenHandleDeoxys_BW(u8 mode, struct BoxPokemon *mons, u8 monIndex, u8 maxMonIndex, void (*callback)(void));
u8 GetMoveSlotToReplace_BW(void);
void SummaryScreen_SetAnimDelayTaskId_BW(u8 taskId);

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
