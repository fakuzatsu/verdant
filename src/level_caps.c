#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "level_caps.h"
#include "pokemon.h"

static const u32 sLevelCapFlagMap[][2] =
{
    {FLAG_BADGE01_GET, 16},
    {FLAG_BADGE02_GET, 24},
    {FLAG_BADGE03_GET, 32},
    {FLAG_BADGE04_GET, 38},
    {FLAG_BADGE05_GET, 40},
    {FLAG_BADGE06_GET, 46},
    {FLAG_BADGE07_GET, 56},
    {FLAG_BADGE08_GET, 64},
    {FLAG_IS_CHAMPION, 80},
};

u32 GetCurrentLevelCap(bool8 skipOptionCheck)
{
    u32 i;

    if (gSaveBlock2Ptr->optionsLevelCap || skipOptionCheck)
    {
        i = VarGet(B_LEVEL_CAP_VARIABLE);
        return (i != 0) ? i : MAX_LEVEL;
    }

    return MAX_LEVEL;
}

u32 GetSoftLevelCapExpValue(u32 level, u32 expValue)
{
    static const u32 sExpScalingDown[5] = { 4, 8, 16, 32, 64 };
    static const u32 sExpScalingUp[5]   = { 16, 8, 4, 2, 1 };

    u32 levelDifference;
    u32 currentLevelCap = GetCurrentLevelCap(FALSE);

    if (!gSaveBlock2Ptr->optionsLevelCap)
        return expValue;

    if (level < currentLevelCap)
    {
        if (B_LEVEL_CAP_EXP_UP)
        {
            levelDifference = currentLevelCap - level;
            if (levelDifference > ARRAY_COUNT(sExpScalingUp) - 1)
                return expValue + (expValue / sExpScalingUp[ARRAY_COUNT(sExpScalingUp) - 1]);
            else
                return expValue + (expValue / sExpScalingUp[levelDifference]);
        }
        else
        {
            return expValue;
        }
    }
    else if (gSaveBlock2Ptr->optionsLevelCap == OPTIONS_LEVEL_CAPS_SOFT)
    {
        levelDifference = level - currentLevelCap;
        if (levelDifference > ARRAY_COUNT(sExpScalingDown) - 1)
            return expValue / sExpScalingDown[ARRAY_COUNT(sExpScalingDown) - 1];
        else
            return expValue / sExpScalingDown[levelDifference];
    }
    else if (gSaveBlock2Ptr->optionsLevelCap == OPTIONS_LEVEL_CAPS_HARD)
    {
        return 0;
    }
    else
    {
       return expValue;
    }
}

u8 SetLevelCapByFlag(void)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sLevelCapFlagMap); i++)
    {
        if (!FlagGet(sLevelCapFlagMap[i][0]))
        {
            VarSet(B_LEVEL_CAP_VARIABLE, sLevelCapFlagMap[i][1]);
            return sLevelCapFlagMap[i][1];
        }
    }

    VarSet(B_LEVEL_CAP_VARIABLE, MAX_LEVEL);
    return MAX_LEVEL;
}

u8 SetLevelCapForFrontier(void)
{
    VarSet(B_LEVEL_CAP_VARIABLE, 50);
    return TRUE;
}
