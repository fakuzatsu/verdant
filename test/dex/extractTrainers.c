#include "global.h"
#include "battle.h"
#include "pokemon.h"
#include "test/test.h"
#include "dex/gf_string.h"
#include "constants/opponents.h" // For TRAINERS_COUNT
#include "constants/pokemon.h" // For MAX_MON_MOVES

extern const struct Trainer gTrainers[];
extern const struct TrainerSprite gTrainerSprites[];
extern const struct TrainerClass gTrainerClasses[];

TEST("DEX: Trainers")
{
    DebugPrintf("===DEBUG OUTPUT START===");

    // Loop through every trainer in gTrainers
    for (u32 i = 0; i < TRAINERS_COUNT; i++)
    {
        const struct Trainer *trainer = &gTrainers[i];

        // Skip empty/null trainers
        if (trainer->party)
        {
            DebugPrintf("- trainerId: %u", i);
            char trainerNameBuf[256];
            ConvertGfStringToUtf8(trainer->trainerName, trainerNameBuf);
            DebugPrintf("  trainerName: %s", trainerNameBuf);

            const struct TrainerClass *class = &gTrainerClasses[trainer->trainerClass];
            char trainerClassBuf[256];
            ConvertGfStringToUtf8(class->name, trainerClassBuf);
            DebugPrintf("  trainerClass: %s", trainerClassBuf );


            DebugPrintf("  partySize: %u", trainer->partySize);
            DebugPrintf("  trainerPic: %u", trainer->trainerPic);
            
            // Print party information
            DebugPrintf("  party:");
            for (u32 j = 0; j < trainer->partySize; j++)
            {
                const struct TrainerMon *mon = &trainer->party[j];
                DebugPrintf("    - species: %u", mon->species);
                DebugPrintf("      level: %u", mon->lvl);
                DebugPrintf("      heldItem: %u", mon->heldItem);
                
                // Print moves (up to 4)
                DebugPrintf("      moves:");
                for (u32 k = 0; k < MAX_MON_MOVES; k++)
                {
                    if (mon->moves[k] != 0)  // 0 means no move
                    {
                        DebugPrintf("        - %u", mon->moves[k]);
                    }
                }
                
                // Print other trainer mon data if available
                if (mon->ability != 0)
                    DebugPrintf("      ability: %u", mon->ability);
                if (mon->nature != 0)
                    DebugPrintf("      nature: %u", mon->nature);
                
                // Decode IVs from packed format
                if (mon->iv != 0)
                {
                    u32 packedIV = mon->iv;
                    u32 hp_iv = packedIV & 0x1F;           // Extract bits 0-4
                    u32 atk_iv = (packedIV >> 5) & 0x1F;   // Extract bits 5-9
                    u32 def_iv = (packedIV >> 10) & 0x1F;  // Extract bits 10-14
                    u32 speed_iv = (packedIV >> 15) & 0x1F; // Extract bits 15-19
                    u32 spatk_iv = (packedIV >> 20) & 0x1F; // Extract bits 20-24
                    u32 spdef_iv = (packedIV >> 25) & 0x1F; // Extract bits 25-29
                    
                    DebugPrintf("      ivs: [%u,%u,%u,%u,%u,%u]", hp_iv, atk_iv, def_iv, spatk_iv, spdef_iv, speed_iv);
                }
                
                // Print EVs if available (HP, Attack, Defense, SpAttack, SpDefense, Speed)
                if (mon->ev != NULL)
                {
                    DebugPrintf("      evs: [%u,%u,%u,%u,%u,%u]", 
                        mon->ev[0], mon->ev[1], mon->ev[2], mon->ev[3], mon->ev[4], mon->ev[5]);
                }

                if (mon->isShiny)
                    DebugPrintf("      isShiny: true");
            }
            
        }
    }

    DebugPrintf("===DEBUG OUTPUT END===");
}
