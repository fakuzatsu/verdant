#include "global.h"
#include "battle.h"
#include "test/test.h"
#include "global.h"
#include "dex/gf_string.h"
#include "constants/battle_move_effects.h"

extern const struct MoveInfo gMovesInfo[];

TEST("DEX: Moves")
{
    DebugPrintf("===DEBUG OUTPUT START===");

    for (u32 i = 0; i < MOVES_COUNT_DYNAMAX; i++)
    {
        // Skip MOVE_NONE (usually index 0)
        if (i == MOVE_NONE)
            continue;

        const struct MoveInfo *move = &gMovesInfo[i];

        // Print basic ability info
        DebugPrintf("- id: %u", i);
        DebugPrintf("  name: %S", move->name);
        // Print Type
        DebugPrintf("  type: %u", move->type);
        if (move->description != NULL)
        {
            char descBuf[256];
            ConvertGfStringToUtf8(move->description, descBuf);
            DebugPrintf("  description: %s", descBuf);
        }
        else
        {
            DebugPrintf("  description: null");
        }

        // Print Power
        if (move->effect == EFFECT_RETURN)
        {
            DebugPrintf("  power: %u", 102);
        }
        else if (move->power != 0)
        {
            DebugPrintf("  power: %u", move->power);
        }

        else
        {
            DebugPrintf("  power: %u", 0);
        }
        DebugPrintf(""); // Blank line


        // Print Accuracy
        if (move->effect == EFFECT_HIT && move->accuracy == 0)
        {
            DebugPrintf("  acc: %u", 999);
        }
        else
        {
            DebugPrintf("  acc: %u", move->accuracy);
        }
        DebugPrintf(""); // Blank line

        //Print Category
        {
            DebugPrintf("  cat: %u", move->category);
        }

        // Print move special properties
        char properties[256] = {0}; // Buffer to hold properties
        bool32 hasProperty = FALSE;

        // Check for each property and add to the list if present
        if (move->makesContact)
        {
            strcat(properties, hasProperty ? ", \"contact\"" : "\"contact\"");
            hasProperty = TRUE;
        }

        if (move->punchingMove)
        {
            strcat(properties, hasProperty ? ", \"punching\"" : "\"punching\"");
            hasProperty = TRUE;
        }

        if (move->slicingMove)
        {
            strcat(properties, hasProperty ? ", \"slicing\"" : "\"slicing\"");
            hasProperty = TRUE;
        }
        if (move->bitingMove)
        {
            strcat(properties, hasProperty ? ", \"biting\"" : "\"biting\"");
            hasProperty = TRUE;
        }

        if (move->pulseMove)
        {
            strcat(properties, hasProperty ? ", \"pulse\"" : "\"pulse\"");
            hasProperty = TRUE;
        }

        if (move->soundMove)
        {
            strcat(properties, hasProperty ? ", \"sound\"" : "\"sound\"");
            hasProperty = TRUE;
        }

        if (move->ballisticMove)
        {
            strcat(properties, hasProperty ? ", \"ballistic\"" : "\"ballistic\"");
            hasProperty = TRUE;
        }

        if (move->powderMove)
        {
            strcat(properties, hasProperty ? ", \"powder\"" : "\"powder\"");
            hasProperty = TRUE;
        }

        if (move->danceMove)
        {
            strcat(properties, hasProperty ? ", \"dance\"" : "\"dance\"");
            hasProperty = TRUE;
        }

        if (move->windMove)
        {
            strcat(properties, hasProperty ? ", \"wind\"" : "\"wind\"");
            hasProperty = TRUE;
        }

        if (move->healingMove)
        {
            strcat(properties, hasProperty ? ", \"healing\"" : "\"healing\"");
            hasProperty = TRUE;
        }

        // Print the properties
        if (hasProperty)
        {
            DebugPrintf("  properties: [%s]", properties);
        }
        else
        {
            DebugPrintf("  properties: []");
        }
    }

    DebugPrintf("===DEBUG OUTPUT END===");
}
