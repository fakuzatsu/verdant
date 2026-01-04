#include "global.h"
#include "battle.h"
#include "pokemon.h"
#include "test/test.h"
#include "dex/gf_string.h"
#include "constants/opponents.h" // For TRAINERS_COUNT

extern const struct Trainer gTrainers[];
extern const struct TrainerSprite gTrainerSprites[];

TEST("DEX: Trainer Sprites")
{
    DebugPrintf("===DEBUG OUTPUT START===");

    // Loop through every trainer in gTrainers
    for (u32 i = 0; i < TRAINERS_COUNT; i++)
    {
        const struct Trainer *trainer = &gTrainers[i];

        // Skip empty/null trainers
        if (trainer->party)
        {
            DebugPrintf("- trainerPic: %u", trainer->trainerPic);
            
            // Get sprite data using trainerPic as index
            if (trainer->trainerPic < TRAINER_PIC_COUNT)
            {
                const struct TrainerSprite *sprite = &gTrainerSprites[trainer->trainerPic];
                
                // Print sprite pointer addresses in hex format
                DebugPrintf("  frontSprite pointer: \"0x%08X\"", (uintptr_t)sprite->frontPic.data);
                DebugPrintf("  palette pointer: \"0x%08X\"", (uintptr_t)sprite->palette.data);
            }
        }
    }

    DebugPrintf("===DEBUG OUTPUT END===");
}
