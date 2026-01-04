#include "global.h"
#include "battle.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/abilities.h"
#include <string.h>
#include "global.h"
#include "dex/gf_string.h"

extern const struct Ability gAbilitiesInfo[];

TEST("DEX: Abilities")
{
    DebugPrintf("===DEBUG OUTPUT START===");
    
    for (u32 i = 0; i < ABILITIES_COUNT; i++)
    {
        // Skip ABILITY_NONE (usually index 0)
        if (i == ABILITY_NONE)
            continue;
            
        const struct Ability *ability = &gAbilitiesInfo[i];
        
        // Print basic ability info
        DebugPrintf("- id: %u", i);
        DebugPrintf("  name: %S", ability->name);
        char descBuf[256];
        ConvertGfStringToUtf8(ability->description, descBuf);
        DebugPrintf("  description: %s", descBuf);
        // for (int i = 0; ability->description[i] != 0xFF; i++) {
        //     DebugPrintf("Byte %d: 0x%02X", i, ability->description[i]);
        // }

        
        DebugPrintf(""); // Blank line
    }
    
    DebugPrintf("===DEBUG OUTPUT END===");
}
