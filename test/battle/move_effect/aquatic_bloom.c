#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gMovesInfo[MOVE_AQUATIC_BLOOM].effect == EFFECT_TERA_BLAST);
    ASSUME(MoveHasAdditionalEffectSelf(MOVE_AQUATIC_BLOOM, MOVE_EFFECT_BOOST_HEALING) == TRUE);
}

SINGLE_BATTLE_TEST("Aquatic Bloom becomes a physical move if the user has a higher Attack stat", s16 damage)
{
    bool32 tera;
    PARAMETRIZE { tera = GIMMICK_NONE; }
    PARAMETRIZE { tera = GIMMICK_TERA; }
    GIVEN {
        ASSUME(gMovesInfo[MOVE_AQUATIC_BLOOM].category == DAMAGE_CATEGORY_SPECIAL);
        PLAYER(SPECIES_SCEPTILE_TERA) { Attack(100); SpAttack(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(200); SpDefense(200); }
    } WHEN {
        TURN { MOVE(player, MOVE_AQUATIC_BLOOM, gimmick: tera); }
    } SCENE {
        MESSAGE("Sceptile used Aquatic Bloom!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUATIC_BLOOM, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(3), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Aquatic Bloom boosts the next instance of healing by 50 percent then ends")
{
    s16 healing1;
    s16 healing2;

    GIVEN {
        PLAYER(SPECIES_SCEPTILE_TERA);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_ABSORB); }
        TURN { MOVE(player, MOVE_AQUATIC_BLOOM); }
        TURN { MOVE(player, MOVE_ABSORB); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ABSORB, player);
        HP_BAR(player, captureDamage: &healing1);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUATIC_BLOOM, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ABSORB, player);
        HP_BAR(player, captureDamage: &healing2);
        MESSAGE("Sceptile's healing is no longer boosted!");
    } FINALLY {
        EXPECT_MUL_EQ(healing1, UQ_4_12(1.5), healing2);
    }
}