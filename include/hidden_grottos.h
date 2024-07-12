#ifndef GUARD_HIDDEN_GROTTOS_H
#define GUARD_HIDDEN_GROTTOS_H

struct Grotto
{
    u8 mapNum;
    u8 mapGroup;
};

struct GrottoEntrance
{
    u8 mapNum;
    u8 mapGroup;
    u16 grottoVariable;
    u8 warpId;
};

void GetGrottoWarp(void);
void GetGrottoReturnWarp(void);
void SetGrottoWarp(void);
void ClearGrottoVars(void);
bool32 IsPlayerInAGrotto(void);
u8 GetCurrentGrottoEscapeWarp(void);

#endif // GUARD_HIDDEN_GROTTOS_H
