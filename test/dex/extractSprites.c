#include "global.h"
#include "battle.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/abilities.h"

TEST("DEX: Sprites")
{
    DebugPrintf("===DEBUG OUTPUT START===");
    u32 endVal = NUM_SPECIES;
    
    for (u32 i = 1; i < endVal; i++)
    {
        // Skip egg since it doesn't have complete data
        if (i == SPECIES_LUGIA_SHADOW || i == SPECIES_EGG)
        {
            DebugPrintf("- index: %u", i);
            DebugPrintf("  speciesName: null");
            DebugPrintf(""); // Blank line
            continue;
        }

        const struct SpeciesInfo *currSpecies = &gSpeciesInfo[i];

        // Print Species Information
        DebugPrintf("- index: %u", i);
        DebugPrintf("  speciesName: %S", currSpecies->speciesName);
        
        // Print pointer addresses in hex format
        DebugPrintf("  frontSprite pointer: \"0x%08X\"", (uintptr_t)currSpecies->frontPic);
        DebugPrintf("  normalPalette pointer: \"0x%08X\"", (uintptr_t)currSpecies->palette);
        DebugPrintf("  shinyPalette pointer: \"0x%08X\"", (uintptr_t)currSpecies->shinyPalette);
        DebugPrintf("  iconSprite pointer: \"0x%08X\"", (uintptr_t)currSpecies->iconSprite);
        
        // Print National Dex number if available
        DebugPrintf("  nationalDex: %u", currSpecies->natDexNum);

        // Extract and print first 32 bytes of sprite data as hex (if pointer is valid)
        if (currSpecies->frontPic != NULL) {
            const u32 *sprite32 = currSpecies->frontPic;  // Use correct type
            const u8 *sprite = (const u8 *)sprite32;
            
            // Determine if compressed
            bool32 isCompressed = (sprite[0] == 0x10);
            DebugPrintf("  isCompressed: %s", isCompressed ? "true" : "false");
            
            // For compressed sprites, extract size from header
            u32 decompressedSize = 0;
            if (isCompressed) {
                decompressedSize = sprite[1] | (sprite[2] << 8) | (sprite[3] << 16);
                DebugPrintf("  decompressedSize: %u", decompressedSize);
            }
        }
        
        DebugPrintf(""); // Blank line to separate entries
    }
    DebugPrintf("===DEBUG OUTPUT END===");
}
