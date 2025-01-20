#include "global.h"
#include "malloc.h"
#include "event_data.h"
#include "field_screen_effect.h"
#include "hidden_grottos.h"
#include "level_caps.h"
#include "overworld.h"
#include "pokemon.h"
#include "random.h"
#include "wild_encounter.h"

static const struct Grotto sHiddenGrottos[] =
{
    {MAP_NUM(GROTTO01), MAP_GROUP(GROTTO01)},
    {MAP_NUM(GROTTO02), MAP_GROUP(GROTTO02)},
    {MAP_NUM(GROTTO03), MAP_GROUP(GROTTO03)},
    {MAP_NUM(GROTTO04), MAP_GROUP(GROTTO04)},
    {MAP_NUM(GROTTO05), MAP_GROUP(GROTTO05)},
    {MAP_NUM(GROTTO06), MAP_GROUP(GROTTO06)},
    {MAP_NUM(GROTTO07), MAP_GROUP(GROTTO07)},
    {MAP_NUM(GROTTO08), MAP_GROUP(GROTTO08)},
    //{MAP_NUM(GROTTO09), MAP_GROUP(GROTTO09)},
    //{MAP_NUM(GROTTO10), MAP_GROUP(GROTTO10)},
    //{MAP_NUM(GROTTO11), MAP_GROUP(GROTTO11)},
    //{MAP_NUM(GROTTO12), MAP_GROUP(GROTTO12)},
    //{MAP_NUM(GROTTO13), MAP_GROUP(GROTTO13)},
    //{MAP_NUM(GROTTO14), MAP_GROUP(GROTTO14)},
    //{MAP_NUM(GROTTO15), MAP_GROUP(GROTTO15)},
    //{MAP_NUM(GROTTO16), MAP_GROUP(GROTTO16)},
    //{MAP_NUM(GROTTO17), MAP_GROUP(GROTTO17)},
    //{MAP_NUM(GROTTO18), MAP_GROUP(GROTTO18)},
    //{MAP_NUM(GROTTO19), MAP_GROUP(GROTTO19)},
    //{MAP_NUM(GROTTO20), MAP_GROUP(GROTTO20)},
    //{MAP_NUM(GROTTO21), MAP_GROUP(GROTTO21)},
    //{MAP_NUM(GROTTO22), MAP_GROUP(GROTTO22)},
    //{MAP_NUM(GROTTO23), MAP_GROUP(GROTTO23)},
    //{MAP_NUM(GROTTO24), MAP_GROUP(GROTTO24)},
    //{MAP_NUM(GROTTO25), MAP_GROUP(GROTTO25)},
    //{MAP_NUM(GROTTO26), MAP_GROUP(GROTTO26)},
    //{MAP_NUM(GROTTO27), MAP_GROUP(GROTTO27)},
    //{MAP_NUM(GROTTO28), MAP_GROUP(GROTTO28)},
    //{MAP_NUM(GROTTO29), MAP_GROUP(GROTTO29)},
    //{MAP_NUM(GROTTO30), MAP_GROUP(GROTTO30)},
};

static const struct GrottoEntrance sHiddenGrottoEntrances[] = 
{
    {MAP_NUM(ROUTE103), MAP_GROUP(ROUTE103), VAR_HIDDEN_GROTTO_ROUTE_103,       1}, // Yes
    {MAP_NUM(ROUTE104), MAP_GROUP(ROUTE104), VAR_HIDDEN_GROTTO_PETALBURG_WOODS, 6}, // Yes
    {MAP_NUM(ROUTE112), MAP_GROUP(ROUTE112), VAR_HIDDEN_GROTTO_ROUTE_112,       6}, // Yes
    {MAP_NUM(ROUTE117), MAP_GROUP(ROUTE117), VAR_HIDDEN_GROTTO_ROUTE_117,       1}, // Yes
    {MAP_NUM(ROUTE119), MAP_GROUP(ROUTE119), VAR_HIDDEN_GROTTO_ROUTE_119,       2}, // Yes
    {MAP_NUM(ROUTE120), MAP_GROUP(ROUTE120), VAR_HIDDEN_GROTTO_ROUTE_120,       2}, // Yes
    {MAP_NUM(ROUTE121), MAP_GROUP(ROUTE121), VAR_HIDDEN_GROTTO_ROUTE_121,       1}, // Yes
    {MAP_NUM(ROUTE123), MAP_GROUP(ROUTE123), VAR_HIDDEN_GROTTO_ROUTE_123,       1}, // Yes
};

static const u16 sLandEncounters[NUM_GROTTO_LAND_SPECIES] = 
{
    SPECIES_PATRAT,
    SPECIES_YUNGOOS,
    SPECIES_NIDORAN_F,
    SPECIES_NIDORAN_M,
    SPECIES_SNUBBULL,
    SPECIES_PUMPKABOO,
    SPECIES_HONEDGE,
    SPECIES_PACHIRISU,
    SPECIES_SKWOVET,
    SPECIES_ELECTRIKE,
    SPECIES_TAROUNTULA,
    SPECIES_BLITZLE,
    SPECIES_KOMALA,
    SPECIES_BIDOOF,
    SPECIES_BUNNELBY,
    SPECIES_VENONAT,
    SPECIES_LOTAD,
    SPECIES_INDEEDEE_M,
    SPECIES_INDEEDEE_F,
    SPECIES_MORELULL,
    SPECIES_FOONGUS,
    SPECIES_PURRLOIN,
    SPECIES_NYMBLE,
    SPECIES_MINCCINO,
    SPECIES_MUNNA,
    SPECIES_KLEFKI,
    SPECIES_WOOBAT,
    SPECIES_DUNSPARCE,
    SPECIES_NOSEPASS,
    SPECIES_SHROODLE,
    SPECIES_ZANGOOSE,
    SPECIES_SEVIPER,
    SPECIES_PAWNIARD,
    SPECIES_CUBCHOO,
    SPECIES_FLETCHLING,
    SPECIES_GRIMER,
    SPECIES_TRUBBISH,
    SPECIES_BOUNSWEET,
    SPECIES_BOUFFALANT,
    SPECIES_MAUSHOLD,
    SPECIES_FALINKS,
    SPECIES_TANGELA,
    SPECIES_SKORUPI,
    SPECIES_HATENNA,
    SPECIES_DRIFLOON,
    SPECIES_GOLETT,
    SPECIES_GLIMMET,
    SPECIES_CUTIEFLY,
    SPECIES_APPLIN,
    SPECIES_CHATOT,
    SPECIES_MIENFOO,
    SPECIES_IMPIDIMP,
    SPECIES_GLIGAR,
    SPECIES_MURKROW,
    SPECIES_SMOLIV,
    SPECIES_NICKIT,
    SPECIES_ESPURR,
    SPECIES_PHANTUMP,
    SPECIES_CLEFAIRY,
    SPECIES_PIKIPEK,
    SPECIES_SNEASEL,
    SPECIES_VULPIX,
    SPECIES_WATTREL,
    SPECIES_BRONZOR,
    SPECIES_SWABLU,
    SPECIES_TOXEL,
    SPECIES_STUNKY,
    SPECIES_FOMANTIS,
    SPECIES_GLAMEOW,
};

static const u16 sWaterEncounters[NUM_GROTTO_WATER_SPECIES] = 
{
    SPECIES_POLIWAG,
    SPECIES_SURSKIT,
    SPECIES_BASCULIN_RED_STRIPED,
    SPECIES_BASCULIN_BLUE_STRIPED,
    SPECIES_BASCULIN_WHITE_STRIPED,
    SPECIES_DUCKLETT,
    SPECIES_TADBULB,
    SPECIES_REMORAID,
    SPECIES_BUIZEL,
    SPECIES_TYMPOLE,
    SPECIES_YANMA,
    SPECIES_PSYDUCK,
    SPECIES_ARROKUDA,
    SPECIES_LOTAD,
    SPECIES_DEWPIDER,
    SPECIES_CRAMORANT,
    SPECIES_CLOBBOPUS,
    SPECIES_LAPRAS,
    SPECIES_MARILL,
    SPECIES_FLAMIGO,
};

static const u8 sLandDistribution[LAND_WILD_COUNT][2] = 
{
    // SPECIES_1 = 30% || SPECIES_1 = 24%
    // SPECIES_2 = 26% || SPECIES_2 = 20%
    // SPECIES_3 = 26% || SPECIES_3 = 20%
    // SPECIES_4 = 18% || SPECIES_4 = 20%
    // SPECIES_5 = 0%  || SPECIES_5 = 16%
    {SPECIES_1, SPECIES_1}, // 20%
    {SPECIES_2, SPECIES_2}, // 20%
    {SPECIES_1, SPECIES_3}, // 10%
    {SPECIES_3, SPECIES_3}, // 10%
    {SPECIES_3, SPECIES_4}, // 10%
    {SPECIES_4, SPECIES_5}, // 10%
    {SPECIES_2, SPECIES_4}, // 5%
    {SPECIES_3, SPECIES_5}, // 5%
    {SPECIES_4, SPECIES_1}, // 4%
    {SPECIES_4, SPECIES_4}, // 4%
    {SPECIES_2, SPECIES_4}, // 1%
    {SPECIES_3, SPECIES_5}, // 1%
};

static const u8 sWaterDistribution[WATER_WILD_COUNT] = 
{
    // SPECIES_1 = 60
    // SPECIES_2 = 40
    SPECIES_1,
    SPECIES_2,
    SPECIES_2,
    SPECIES_2,
    SPECIES_2,
};

static u8 FindExistingGrottoWarp(void)
{
    for (u32 i = 0; i < ARRAY_COUNT(sHiddenGrottoEntrances); i++)
    {
        if (gSaveBlock1Ptr->location.mapGroup == sHiddenGrottoEntrances[i].mapGroup 
            && gSaveBlock1Ptr->location.mapNum == sHiddenGrottoEntrances[i].mapNum)
        {
            VarSet(VAR_HIDDEN_GROTTO_RETURN_WARP, i);
            return VarGet(sHiddenGrottoEntrances[i].grottoVariable);
        }
    }
    return 0;
}

void GetGrottoWarp(void)
{
    u8 warpId = FindExistingGrottoWarp();

    if (warpId != 0)
        SetDynamicWarp(0, sHiddenGrottos[warpId -1].mapGroup, sHiddenGrottos[warpId -1].mapNum, 0);
}

void GetGrottoReturnWarp(void)
{
    u16 var = VarGet(VAR_HIDDEN_GROTTO_RETURN_WARP);

    SetDynamicWarp(0, sHiddenGrottoEntrances[var].mapGroup, sHiddenGrottoEntrances[var].mapNum, sHiddenGrottoEntrances[var].warpId);
}

void SetGrottoWarp(void)
{
    u32 warpId = FindExistingGrottoWarp();
    if (warpId == 0)
    {
        warpId = (Random() % ARRAY_COUNT(sHiddenGrottos)) + 1;
        u32 nonZeroGrottoVarsCount = 0;

        for (u32 i = 0; i < ARRAY_COUNT(sHiddenGrottoEntrances); i++)
        {
            if (VarGet(sHiddenGrottoEntrances[i].grottoVariable) != 0)
            {
                nonZeroGrottoVarsCount++;
            }
        }

        if (nonZeroGrottoVarsCount < ARRAY_COUNT(sHiddenGrottos))
        {
            bool8 isUnique;
            do
            {
                warpId = (Random() % ARRAY_COUNT(sHiddenGrottos)) + 1;
                isUnique = TRUE;

                for (u32 j = 0; j < ARRAY_COUNT(sHiddenGrottoEntrances); j++)
                {
                    if (warpId == VarGet(sHiddenGrottoEntrances[j].grottoVariable))
                    {
                        isUnique = FALSE;
                        break;
                    }
                }
            } while (!isUnique);
        }

        VarSet(sHiddenGrottoEntrances[VarGet(VAR_HIDDEN_GROTTO_RETURN_WARP)].grottoVariable, warpId);
    }

    if (warpId != 0)
    {
        SetWarpDestinationToMapWarp(sHiddenGrottos[warpId - 1].mapGroup, sHiddenGrottos[warpId - 1].mapNum, 0);
        DoWarp();
        ResetInitialPlayerAvatarState();
    }
}

void SetGrottoSeed(void)
{
    gSaveBlock3Ptr->grottoSeed = Random32();
}

void ClearGrottoVars(void)
{
    for (u32 i = GROTTO_VARS_START; i <= GROTTO_VARS_END; ++i)
    {
        VarSet(i, 0);
    }
}

bool32 IsPlayerInAGrotto(void)
{
    if (gMapHeader.regionMapSectionId == MAPSEC_HIDDEN_GROTTO)
        return TRUE;
    return FALSE;
}

u8 GetHiddenGrottoEncounterChance(void)
{
    return 10; // To be expanded upon
}

struct WildPokemon *GetHiddenGrottoLandEncounters(void) 
{
    static struct WildPokemon wildPokemonArray[LAND_WILD_COUNT];
    u32 seed = gSaveBlock3Ptr->grottoSeed;
    u32 j = (gMapHeader.mapLayoutId * seed) & 1;

    u32 level = GetCurrentLevelCap(TRUE) / 2;

    for (u32 i = 0; i < LAND_WILD_COUNT; i++)
    {
        u32 speciesNum = ((gMapHeader.mapLayoutId << sLandDistribution[i][j]) * seed) % NUM_GROTTO_LAND_SPECIES;
        wildPokemonArray[i].species = GetSpeciesEvolutionStageAtLevel(sLandEncounters[speciesNum], level - 1, TRUE);
        wildPokemonArray[i].minLevel = level;
        wildPokemonArray[i].maxLevel = level + 2;
    }

    return wildPokemonArray;
}

struct WildPokemon *GetHiddenGrottoWaterEncounters(void) 
{
    static struct WildPokemon wildPokemonArray[WATER_WILD_COUNT];
    u32 seed = gSaveBlock3Ptr->grottoSeed;

    u32 level = GetCurrentLevelCap(TRUE) / 2;

    for (u32 i = 0; i < WATER_WILD_COUNT; i++)
    {
        u32 speciesNum = ((gMapHeader.mapLayoutId << sWaterDistribution[i]) * seed) % NUM_GROTTO_WATER_SPECIES;
        wildPokemonArray[i].species = GetSpeciesEvolutionStageAtLevel(sWaterEncounters[speciesNum], level - 1, TRUE);
        wildPokemonArray[i].minLevel = level;
        wildPokemonArray[i].maxLevel = level + 2;
    }

    return wildPokemonArray;
}