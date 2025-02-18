#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gMovesInfo[MOVE_CLANGING_BLADE].effect == EFFECT_TERA_BLAST);
}

SINGLE_BATTLE_TEST("Clanging Blade becomes a physical move if the user has a higher Attack stat", s16 damage)
{
    bool32 tera;
    PARAMETRIZE { tera = GIMMICK_NONE; }
    PARAMETRIZE { tera = GIMMICK_TERA; }
    GIVEN {
        ASSUME(gMovesInfo[MOVE_CLANGING_BLADE].category == DAMAGE_CATEGORY_SPECIAL);
        PLAYER(SPECIES_KOMMO_O_TERA) { Attack(100); SpAttack(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(200); SpDefense(200); }
    } WHEN {
        TURN { MOVE(player, MOVE_CLANGING_BLADE, gimmick: tera); }
    } SCENE {
        MESSAGE("Kommo-o used Clanging Blade!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLANGING_BLADE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(3), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Clanging Blade raises the user's Speed")
{
    u16 species;
    GIVEN {
        ASSUME(MoveHasAdditionalEffectSelf(MOVE_CLANGING_BLADE, MOVE_EFFECT_SPD_PLUS_1) == TRUE);
        PLAYER(SPECIES_KOMMO_O_TERA);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CLANGING_BLADE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLANGING_BLADE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("Kommo-o's Speed rose!");
    }
}
