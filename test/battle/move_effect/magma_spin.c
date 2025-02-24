#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gMovesInfo[MOVE_MAGMA_SPIN].effect == EFFECT_TERA_BLAST);
    ASSUME(MoveHasAdditionalEffectSelf(MOVE_MAGMA_SPIN, MOVE_EFFECT_RAPID_SPIN) == TRUE);
}

SINGLE_BATTLE_TEST("Magma Spin becomes a physical move if the user has a higher Attack stat", s16 damage)
{
    bool32 tera;
    PARAMETRIZE { tera = GIMMICK_NONE; }
    PARAMETRIZE { tera = GIMMICK_TERA; }
    GIVEN {
        ASSUME(gMovesInfo[MOVE_MAGMA_SPIN].category == DAMAGE_CATEGORY_SPECIAL);
        PLAYER(SPECIES_SWAMPERT_TERA) { Attack(100); SpAttack(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(200); SpDefense(200); }
    } WHEN {
        TURN { MOVE(player, MOVE_MAGMA_SPIN, gimmick: tera); }
    } SCENE {
        MESSAGE("Swampert used Magma Spin!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGMA_SPIN, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(3), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Magma Spin blows away Wrap, hazards, etc.")
{
    GIVEN {
        PLAYER(SPECIES_SWAMPERT_TERA);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_WRAP); }
        TURN { MOVE(opponent, MOVE_STEALTH_ROCK); MOVE(player, MOVE_MAGMA_SPIN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEALTH_ROCK, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGMA_SPIN, player);
        MESSAGE("Swampert got free of Foe Wobbuffet's Wrap!");
        MESSAGE("Swampert blew away Stealth Rock!");
    }
}