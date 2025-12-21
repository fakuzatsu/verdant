#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_terastal.h"
#include "battle_gimmick.h"
#include "battle_scripts.h"
#include "event_data.h"
#include "item.h"
#include "palette.h"
#include "pokemon.h"
#include "safari_zone.h"
#include "sprite.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/rgb.h"

struct TeraFormMove
{
    u16 species;
    u16 teraFormMove;
};

static const struct TeraFormMove sTeraFormMoveTable[] =
{
    {SPECIES_TERAPAGOS_TERASTAL,    MOVE_TERA_STAR},
    {SPECIES_KOMMO_O_TERA,          MOVE_CLANGING_BLADE},
    {SPECIES_SCEPTILE_TERA,         MOVE_AQUATIC_BLOOM},
    {SPECIES_BLAZIKEN_TERA,         MOVE_SOLAR_KICK},
    {SPECIES_SWAMPERT_TERA,         MOVE_MAGMA_SPIN},
    {SPECIES_NINETALES_TERA,        MOVE_SPIRIT_CHRONICLE},
};

// Sets flags and variables upon a battler's Terastallization.
void ActivateTera(u32 battler)
{
    u32 side = GetBattlerSide(battler);

    // Set appropriate flags.
    SetActiveGimmick(battler, GIMMICK_TERA);
    SetGimmickAsActivated(battler, GIMMICK_TERA);

    // Remove Tera Orb charge.
    if (B_FLAG_TERA_ORB_CHARGED != 0
        && (B_FLAG_TERA_ORB_NO_COST == 0 || !FlagGet(B_FLAG_TERA_ORB_NO_COST))
        && side == B_SIDE_PLAYER
        && !(IsDoubleBattle() && !IsPartnerMonFromSameTrainer(battler)))
    {
        FlagClear(B_FLAG_TERA_ORB_CHARGED);
    }

    // Execute battle script.
    PREPARE_TYPE_BUFFER(gBattleTextBuff1, GetBattlerTeraType(battler));
    if (TryBattleFormChange(gBattlerAttacker, FORM_CHANGE_BATTLE_TERASTALLIZATION))
        BattleScriptExecute(BattleScript_TeraFormChange);
    else
        BattleScriptExecute(BattleScript_Terastallization);
}

// Applies palette blend and enables UI indicator after animation has played
void ApplyBattlerVisualsForTeraAnim(u32 battler)
{
    struct Pokemon *party = GetBattlerParty(battler);
    u32 index = gBattlerPartyIndexes[battler];
    u32 species = GetMonData(&party[index], MON_DATA_SPECIES);

    if (!gSpeciesInfo[species].isTeraForm)
    {
        // Show indicator and do palette blend.
        UpdateHealthboxAttribute(gHealthboxSpriteIds[battler], &party[index], HEALTHBOX_ALL);
        BlendPalette(OBJ_PLTT_ID(battler), 16, 8, GetTeraTypeRGB(GetBattlerTeraType(battler)));
        CpuCopy32(gPlttBufferFaded + OBJ_PLTT_ID(battler), gPlttBufferUnfaded + OBJ_PLTT_ID(battler), PLTT_SIZEOF(16));

        // We apply the animation behind a white screen, so restore the blended color here to avoid a pop
        BlendPalette(OBJ_PLTT_ID(battler), 16, 16, RGB_WHITEALPHA);
    }
}

// Returns whether a battler can Terastallize.
bool32 CanTerastallize(u32 battler)
{
    u32 holdEffect = GetBattlerHoldEffect(battler, FALSE);

    // Prevents Zigzagoon from terastalizing in vanilla.
    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE && GetBattlerSide(battler) == B_SIDE_OPPONENT)
        return FALSE;

    if (TESTING || GetBattlerSide(battler) == B_SIDE_OPPONENT)
    {
        // Skip all other checks in this block, go to HasTrainerUsedGimmick
    }
    else if (!CheckBagHasItem(ITEM_TERA_ORB, 1))
    {
        return FALSE;
    }
    else if (FlagGet(B_FLAG_TERA_ORB_NO_COST))
    {
        // Tera Orb is not depleted, go to HasTrainerUsedGimmick
    }
    else if (!FlagGet(B_FLAG_TERA_ORB_CHARGED))
    {
        return FALSE;
    }

    // Check if Trainer has already Terastallized.
    if (HasTrainerUsedGimmick(battler, GIMMICK_TERA))
        return FALSE;

    // Check if AI battler is intended to Terastallize.
    if (!ShouldTrainerBattlerUseGimmick(battler, GIMMICK_TERA))
        return FALSE;

    // Check if battler has another gimmick active.
    if (GetActiveGimmick(battler) != GIMMICK_NONE)
        return FALSE;

    // Check if battler is holding a Z-Crystal or Mega Stone.
    if (!TESTING && (holdEffect == HOLD_EFFECT_Z_CRYSTAL || holdEffect == HOLD_EFFECT_MEGA_STONE)) // tests make this check already
        return FALSE;

    // Every check passed!
    return TRUE;
}

// Returns a battler's Tera type.
u32 GetBattlerTeraType(u32 battler)
{
    return GetMonData(&GetBattlerParty(battler)[gBattlerPartyIndexes[battler]], MON_DATA_TERA_TYPE);
}

// Uses up a type's Stellar boost.
void ExpendTypeStellarBoost(u32 battler, u32 type)
{
    if (type < 32 && gBattleMons[battler].species != SPECIES_TERAPAGOS_STELLAR) // avoid OOB access
        gBattleStruct->stellarBoostFlags[GetBattlerSide(battler)] |= 1u << type;
}

// Checks whether a type's Stellar boost has been expended.
bool32 IsTypeStellarBoosted(u32 battler, u32 type)
{
    if (type < 32) // avoid OOB access
        return !(gBattleStruct->stellarBoostFlags[GetBattlerSide(battler)] & (1u << type));
    else
        return FALSE;
}

// Returns the STAB power multiplier to use when Terastallized.
// Power multipliers from Smogon Research thread.
uq4_12_t GetTeraMultiplier(u32 battler, u32 type)
{
    u32 teraType = GetBattlerTeraType(battler);
    bool32 hasAdaptability = (GetBattlerAbility(battler) == ABILITY_ADAPTABILITY);

    // Safety check.
    if (GetActiveGimmick(battler) != GIMMICK_TERA)
        return UQ_4_12(1.0);

    // Stellar-type checks.
    if (teraType == TYPE_STELLAR)
    {
        bool32 shouldBoost = IsTypeStellarBoosted(battler, type);
        if (IS_BATTLER_OF_BASE_TYPE(battler, type))
        {
            if (shouldBoost)
                return UQ_4_12(2.0);
            else
                return UQ_4_12(1.5);
        }
        else if (shouldBoost)
            return UQ_4_12(1.2);
        else
            return UQ_4_12(1.0);
    }
    // Base and Tera type.
    if (type == teraType && IS_BATTLER_OF_BASE_TYPE(battler, type))
    {
        if (hasAdaptability)
            return UQ_4_12(2.25);
        else
            return UQ_4_12(2.0);
    }
    // Base or Tera type only.
    else if ((type == teraType && !IS_BATTLER_OF_BASE_TYPE(battler, type))
             || (type != teraType && IS_BATTLER_OF_BASE_TYPE(battler, type)))
    {
        if (hasAdaptability)
            return UQ_4_12(2.0);
        else
            return UQ_4_12(1.5);
    }
    // Neither base or Tera type.
    else
    {
        return UQ_4_12(1.0);
    }
}

u16 GetTeraTypeRGB(u32 type)
{
    return gTypesInfo[type].teraTypeRGBValue;
}

static u16 GetTeraBlastFormMove(u32 battler)
{
    u32 i;
    u32 species = gBattleMons[battler].species;
    u32 targetSpecies = SPECIES_NONE;

    if (!gSpeciesInfo[species].isTeraForm)
        targetSpecies = GetBattleFormChangeTargetSpecies(battler, FORM_CHANGE_BATTLE_TERASTALLIZATION);

    if (targetSpecies != SPECIES_NONE)
        species = targetSpecies;

    if (gSpeciesInfo[species].isTeraForm)
    {
        for (i = 0; i < ARRAY_COUNT(sTeraFormMoveTable); i++)
        {
            if (sTeraFormMoveTable[i].species == species)
                return sTeraFormMoveTable[i].teraFormMove;
        }
    }

    return MOVE_TERA_BLAST;
}

// Returns the appropriate Tera Form Move. 
// Set up to be expandable. Currently only Tera Blast is changed.
u16 GetTeraMove(u32 battler, u32 baseMove)
{
    u32 move = baseMove;

    if (baseMove == MOVE_NONE) // for move display
    {
        return MOVE_NONE;
    }
    else if (baseMove == MOVE_STRUGGLE)
    {
        return MOVE_STRUGGLE;
    }
    else if (baseMove == MOVE_TERA_BLAST)
    {
        move = GetTeraBlastFormMove(battler);
    }

    return move;
}

// Returns whether a move is a Tera Move or not.
bool32 IsTeraFormMove(u32 move)
{
    return move >= FIRST_TERA_MOVE && move <= LAST_TERA_MOVE;
}
