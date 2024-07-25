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

#define NUM_GROTTO_LAND_SPECIES 70
#define NUM_GROTTO_WATER_SPECIES 20

#define SPECIES_1 1
#define SPECIES_2 2
#define SPECIES_3 3
#define SPECIES_4 4
#define SPECIES_5 5

void GetGrottoWarp(void);
void GetGrottoReturnWarp(void);
void SetGrottoWarp(void);
void ClearGrottoVars(void);
bool32 IsPlayerInAGrotto(void);
u8 GetCurrentGrottoEscapeWarp(void);
u8 GetHiddenGrottoEncounterChance(void);
struct WildPokemon *GetHiddenGrottoLandEncounters(void);
struct WildPokemon *GetHiddenGrottoWaterEncounters(void);

#endif // GUARD_HIDDEN_GROTTOS_H
