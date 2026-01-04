#include "global.h"
#include "test/test.h"
#include "constants/pokemon.h"
#include "pokemon.h"
#include "battle.h"

extern const struct TypeInfo gTypesInfo[];
extern const uq4_12_t gTypeEffectivenessTable[NUMBER_OF_MON_TYPES][NUMBER_OF_MON_TYPES];

// Convert UQ4_12 format to integer value multiplied by 10
// 0.5 becomes 5, 1.0 becomes 10, 2.0 becomes 20
static u8 ConvertEffectivenessToScaledInt(uq4_12_t value) 
{
    // UQ4_12 is a fixed-point format where 1.0 is represented as 2^12 (4096)
    // Convert to u8 value multiplied by 10
    return (u8)((value * 10) / (1 << 12));
}

TEST("DEX: Types")
{
    DebugPrintf("===DEBUG OUTPUT START===");
    
    for (u32 i = 0; i < NUMBER_OF_MON_TYPES; i++)
    {
        const struct TypeInfo *type = &gTypesInfo[i];
        
        DebugPrintf("- typeId: %u", i);
        DebugPrintf("  name: %S", type->name);
        
        // Build the effectiveness array values directly in the debug print
        // First print the opening bracket with the first value
        DebugPrintf("  effectiveness: [%u", ConvertEffectivenessToScaledInt(gTypeEffectivenessTable[i][0]));
        
        // Add remaining values with comma separators
        for (u32 j = 1; j < NUMBER_OF_MON_TYPES; j++)
        {
            u8 value = ConvertEffectivenessToScaledInt(gTypeEffectivenessTable[i][j]);
            DebugPrintf(", %u", value);
        }
        
        // Close the effectiveness array
        DebugPrintf("]");
    }
    
    DebugPrintf("===DEBUG OUTPUT END===");
}
