#ifndef GUARD_HIDDEN_GROTTOS_H
#define GUARD_HIDDEN_GROTTOS_H

struct Grotto
{
    u16 mapNum;
    u16 mapGroup;
};

struct GrottoEntrance
{
    u16 mapNum;
    u16 mapGroup;
    u16 grottoVariable;
    u8 warpId;
};

void GetGrottoWarp(void);
void GetGrottoReturnWarp(void);
void SetGrottoWarp(void);
void ClearGrottoVars(void);

#endif // GUARD_HIDDEN_GROTTOS_H
