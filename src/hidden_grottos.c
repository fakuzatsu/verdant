#include "global.h"
#include "event_data.h"
#include "field_screen_effect.h"
#include "hidden_grottos.h"
#include "overworld.h"
#include "random.h"
#include "constants/maps.h"
#include "constants/vars.h"

static const struct Grotto sHiddenGrottos[] =
{
    {MAP_NUM(GROTTO01), MAP_GROUP(GROTTO01)},
    {MAP_NUM(GROTTO02), MAP_GROUP(GROTTO02)},
    //{MAP_NUM(GROTTO03), MAP_GROUP(GROTTO03)},
    //{MAP_NUM(GROTTO04), MAP_GROUP(GROTTO04)},
    //{MAP_NUM(GROTTO05), MAP_GROUP(GROTTO05)},
    //{MAP_NUM(GROTTO06), MAP_GROUP(GROTTO06)},
    //{MAP_NUM(GROTTO07), MAP_GROUP(GROTTO07)},
    //{MAP_NUM(GROTTO08), MAP_GROUP(GROTTO08)},
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

static const struct GrottoEntrance sHiddenGrottoEntrances[] = {
    {MAP_NUM(ROUTE103), MAP_GROUP(ROUTE103), VAR_HIDDEN_GROTTO_ROUTE_103, 1},
    {MAP_NUM(ROUTE104), MAP_GROUP(ROUTE104), VAR_HIDDEN_GROTTO_ROUTE_104, 2},
    {MAP_NUM(ROUTE112), MAP_GROUP(ROUTE112), VAR_HIDDEN_GROTTO_ROUTE_112, 6},
    {MAP_NUM(ROUTE115), MAP_GROUP(ROUTE115), VAR_HIDDEN_GROTTO_ROUTE_115, 3},
    {MAP_NUM(ROUTE116), MAP_GROUP(ROUTE116), VAR_HIDDEN_GROTTO_ROUTE_116, 5},
    {MAP_NUM(ROUTE117), MAP_GROUP(ROUTE117), VAR_HIDDEN_GROTTO_ROUTE_117, 1},
    {MAP_NUM(ROUTE119), MAP_GROUP(ROUTE119), VAR_HIDDEN_GROTTO_ROUTE_119, 2},
    {MAP_NUM(ROUTE120), MAP_GROUP(ROUTE120), VAR_HIDDEN_GROTTO_ROUTE_120, 2},
    {MAP_NUM(ROUTE121), MAP_GROUP(ROUTE121), VAR_HIDDEN_GROTTO_ROUTE_121, 1},
    {MAP_NUM(ROUTE123), MAP_GROUP(ROUTE123), VAR_HIDDEN_GROTTO_ROUTE_123, 1},
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
    if ((gSaveBlock1Ptr->location.mapGroup >= MAP_GROUP(GROTTO01) && gSaveBlock1Ptr->location.mapGroup <= MAP_GROUP(GROTTO02))
        && (gSaveBlock1Ptr->location.mapNum >= MAP_NUM(GROTTO01) && gSaveBlock1Ptr->location.mapNum <= MAP_NUM(GROTTO02)))
    {
        u16 var = VarGet(VAR_HIDDEN_GROTTO_RETURN_WARP);
        SetDynamicWarp(0, sHiddenGrottoEntrances[var].mapGroup, sHiddenGrottoEntrances[var].mapNum, sHiddenGrottoEntrances[var].warpId);
    }
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

u8 GetCurrentGrottoEscapeWarp(void)
{
    u16 var = VarGet(VAR_HIDDEN_GROTTO_RETURN_WARP);
    return sHiddenGrottoEntrances[var].warpId;
}
