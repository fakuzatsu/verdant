#ifndef GUARD_LEVEL_CAP_H
#define GUARD_LEVEL_CAP_H

u32 GetCurrentLevelCap(bool8 skipOptionCheck);
u32 GetSoftLevelCapExpValue(u32 level, u32 expValue);
u8 SetLevelCapByFlag(void);
u8 SetLevelCapForFrontier(void);

#endif /* GUARD_LEVEL_CAP_H */
