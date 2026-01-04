#include "global.h"
#include "battle.h"
#include "pokemon.h"
#include "test/test.h"
#include "dex/gf_string.h"
#include "constants/abilities.h"

bool32 MoveInLevelUpLearnset(u16 species, u16 move)
{
    const struct LevelUpMove *learnset = gSpeciesInfo[species].levelUpLearnset;
    u32 j;

    for (j = 0; learnset[j].move != LEVEL_UP_MOVE_END; j++)
    {
        if (learnset[j].move == move)
            return TRUE;
    }
    return FALSE;
}

static const u16 sUniversalMoves[] = {
    MOVE_BIDE,
    MOVE_FRUSTRATION,
    MOVE_HIDDEN_POWER,
    MOVE_MIMIC,
    MOVE_NATURAL_GIFT,
    MOVE_PROTECT,
    MOVE_RAGE,
    MOVE_RETURN,
    MOVE_SECRET_POWER,
    MOVE_SUBSTITUTE,
    MOVE_TERA_BLAST,
    MOVE_TOXIC,
    MOVE_FACADE,
    MOVE_REST,
    MOVE_SLEEP_TALK,
};

TEST("DEX: Pokemon")
{
    DebugPrintf("===DEBUG OUTPUT START===");
    u32 endVal = NUM_SPECIES;
    for (u32 i = 1; i < endVal; i++)
    {

        // Skip egg since it doesn't have complete data. Idk what shadow lugia is
        if (i == SPECIES_LUGIA_SHADOW || i == SPECIES_EGG)
        {
            // DebugPrintf("- index: %u", i);
            // DebugPrintf("  speciesName: null");
            // DebugPrintf(""); // Blank line
            continue;
        }

        const struct SpeciesInfo *currSpecies = &gSpeciesInfo[i];

        // Print index
        DebugPrintf("- speciesId: %u", i);

        // Print Name
        char nameBuf[256];
        ConvertGfStringToUtf8(currSpecies->speciesName, nameBuf);
        DebugPrintf("  speciesName: \"%s\"", nameBuf);

        // Print types
        {
            const u8 *types = currSpecies->types;
            if (types[0] != types[1])
            {
                DebugPrintf("  types: [%u,%u]", types[0], types[1]);
            }
            else
            {
                DebugPrintf("  types: [%u]", types[0]);
            }
        }

        // Print stats
        DebugPrintf("  stats: [%u,%u,%u,%u,%u,%u]",
                    currSpecies->baseHP,
                    currSpecies->baseAttack,
                    currSpecies->baseDefense,
                    currSpecies->baseSpeed,
                    currSpecies->baseSpAttack,
                    currSpecies->baseSpDefense);

        const u16 *abilities = currSpecies->abilities;
        DebugPrintf("  abilities: [%u,%u,%u]", abilities[0], abilities[1], abilities[2]);

        //Print held items
        DebugPrintf("  heldItems: [%u,%u]", currSpecies->itemCommon, currSpecies->itemRare );

        // Print level-up moves
        DebugPrintf("  levelUpMoves:");
        bool32 shouldContinue = TRUE;
        u32 moveIndex = 0;
        const struct LevelUpMove *levelUpLearnset = currSpecies->levelUpLearnset;

        // Opening bracket for the array
        DebugPrintf("    [");

        while (shouldContinue)
        {
            if (moveIndex > 0)
                DebugPrintf("     [%u, %u],", levelUpLearnset[moveIndex].move, levelUpLearnset[moveIndex].level);
            else
                DebugPrintf("     [%u, %u],", levelUpLearnset[moveIndex].move, levelUpLearnset[moveIndex].level);

            moveIndex++;
            if (levelUpLearnset[moveIndex].move == LEVEL_UP_MOVE_END)
            {
                shouldContinue = FALSE;
            }
        }

        // Closing bracket for the array
        DebugPrintf("    ]");

        DebugPrintf("  tmMoves:");
        DebugPrintf("    [");
        moveIndex = 0;
        const u16 *teachableLearnset = currSpecies->teachableLearnset;

        bool32 hasAnyMove = FALSE;
        while (teachableLearnset[moveIndex] != MOVE_UNAVAILABLE)
        {
            u16 moveId = teachableLearnset[moveIndex];
            // Add comma only if not the first move
            if (hasAnyMove)
                DebugPrintf(",");
            DebugPrintf("%u", moveId);
            hasAnyMove = TRUE;
            moveIndex++;
        }

        // Add universal moves with the same format
        for (u32 univIndex = 0; univIndex < ARRAY_COUNT(sUniversalMoves); univIndex++)
        {
            u16 moveId = sUniversalMoves[univIndex];
            // Apply the same check as CanLearnTeachableMove
            if (!currSpecies->tmIlliterate ||
                (P_TM_LITERACY >= GEN_6 && MoveInLevelUpLearnset(i, moveId)))
            {
                // Skip special exceptions
                if ((moveId == MOVE_TERA_BLAST && GET_BASE_SPECIES_ID(i) == SPECIES_TERAPAGOS) ||
                    (GET_BASE_SPECIES_ID(i) == SPECIES_PYUKUMUKU &&
                     (moveId == MOVE_HIDDEN_POWER || moveId == MOVE_RETURN || moveId == MOVE_FRUSTRATION)))
                    continue;

                // Add comma if we already have at least one move
                if (hasAnyMove)
                    DebugPrintf(",");
                DebugPrintf("%u", moveId);
                hasAnyMove = TRUE;
            }
        }

        // Closing bracket for TM moves
        DebugPrintf("    ]");

        // Print egg moves
        DebugPrintf("  eggMoves:");
        shouldContinue = TRUE;
        moveIndex = 0;
        const u16 *eggMoves = GetSpeciesEggMoves(i);
        while (eggMoves[moveIndex] != MOVE_UNAVAILABLE)
        {
            u16 moveId = eggMoves[moveIndex];
            // DebugPrintf("    - %S", gMovesInfo[moveId].name);
            DebugPrintf("    - %u", moveId);
            moveIndex++;
        }

        DebugPrintf("  dexId: %u", currSpecies->natDexNum);

        // Print evolutions
        if (currSpecies->evolutions != NULL) // Check if evolutions exist
        {
            DebugPrintf("  evolutions:");
            const struct Evolution *evolutions = currSpecies->evolutions;

            // Define a safe maximum number of evolutions to prevent infinite loops
            const u8 MAX_EVOLUTION_COUNT = 10; // A reasonable upper limit

            for (u8 j = 0; j < MAX_EVOLUTION_COUNT; j++)
            {
                // Break if we reach the sentinel value
                if (evolutions[j].method == EVOLUTIONS_END)
                    break;

                DebugPrintf("    - [%u, %u, %u]", evolutions[j].method, evolutions[j].targetSpecies, evolutions[j].param);
            }
        }
        else
        {
            DebugPrintf("  evolutions: null");
        }

        {
            // Maximum number of possible form types
            const u8 MAX_FORMS = 10;
            const char *formTypes[MAX_FORMS];
            u8 formCount = 0;

            // Check each form type in the specified order
            if (currSpecies->isAlolanForm && formCount < MAX_FORMS)
                formTypes[formCount++] = "alolan";

            if (currSpecies->isGalarianForm && formCount < MAX_FORMS)
                formTypes[formCount++] = "galarian";

            if (currSpecies->isHisuianForm && formCount < MAX_FORMS)
                formTypes[formCount++] = "hisuian";

            if (currSpecies->isPaldeanForm && formCount < MAX_FORMS)
                formTypes[formCount++] = "paldean";

            if (currSpecies->isTotem && formCount < MAX_FORMS)
                formTypes[formCount++] = "totem";

            if (currSpecies->isMegaEvolution && formCount < MAX_FORMS)
                formTypes[formCount++] = "mega";

            if (currSpecies->isPrimalReversion && formCount < MAX_FORMS)
                formTypes[formCount++] = "primal";

            if (currSpecies->isUltraBurst && formCount < MAX_FORMS)
                formTypes[formCount++] = "ultraburst";

            if (currSpecies->isGigantamax && formCount < MAX_FORMS)
                formTypes[formCount++] = "gigantamax";

            if (currSpecies->isTeraForm && formCount < MAX_FORMS)
                formTypes[formCount++] = "tera";

            // Print forms array
            if (formCount > 0)
            {
                DebugPrintf("  forms: [");
                for (u8 i = 0; i < formCount; i++)
                {
                    if (i > 0)
                        DebugPrintf(",");
                    DebugPrintf("\"%s\"", formTypes[i]);
                }
                DebugPrintf("]");
            }
            else
            {
                DebugPrintf("  forms: null");
            }
        }

        // Check for form ID in formSpeciesIdTable
        if (currSpecies->formSpeciesIdTable != NULL)
        {
            const u16 *formTable = currSpecies->formSpeciesIdTable;
            const u8 MAX_FORM_COUNT = 20; // Safety limit

            for (u8 k = 0; k < MAX_FORM_COUNT; k++)
            {
                if (formTable[k] == FORM_SPECIES_END)
                    break;

                if (formTable[k] == i) // Current species found in form table
                {
                    DebugPrintf("  formId: %u", k); // Update with actual index
                    break;
                }
            }
        }
        else
        {
            DebugPrintf("  formId: 0"); // Default for species without form tables
        }

        DebugPrintf(""); // Blank line
    }
    DebugPrintf("===DEBUG OUTPUT END===");
}
