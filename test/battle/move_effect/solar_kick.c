#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gMovesInfo[MOVE_SOLAR_KICK].effect == EFFECT_SOLAR_KICK);
    ASSUME(gMovesInfo[MOVE_FLAMETHROWER].power != 0);
    ASSUME(gMovesInfo[MOVE_FLAMETHROWER].type == TYPE_FIRE);
}

SINGLE_BATTLE_TEST("Solar Kick becomes a physical move if the user has a higher Attack stat", s16 damage)
{
    bool32 tera;
    PARAMETRIZE { tera = GIMMICK_NONE; }
    PARAMETRIZE { tera = GIMMICK_TERA; }
    GIVEN {
        ASSUME(gMovesInfo[MOVE_SOLAR_KICK].category == DAMAGE_CATEGORY_SPECIAL);
        PLAYER(SPECIES_BLAZIKEN_TERA) { Attack(100); SpAttack(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(200); SpDefense(200); }
    } WHEN {
        TURN { MOVE(player, MOVE_SOLAR_KICK, gimmick: tera); }
    } SCENE {
        MESSAGE("Blaziken used Solar Kick!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SOLAR_KICK, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(3), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Solar Kick used in the sun deals extra damage")
{
    s16 normalDamage = 0;
    s16 chargedUpDamage = 0;
    GIVEN {
        PLAYER(SPECIES_BLAZIKEN_TERA);
        OPPONENT(SPECIES_WOBBUFFET) { HP(1000); MaxHP(1000); }; // pls don't die
    } WHEN {
        TURN { MOVE(player, MOVE_SOLAR_KICK); }
        TURN { MOVE(player, MOVE_SUNNY_DAY); }
        TURN { MOVE(player, MOVE_SOLAR_KICK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SOLAR_KICK, player);
        HP_BAR(opponent, captureDamage: &normalDamage);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SOLAR_KICK, player);
        HP_BAR(opponent, captureDamage: &chargedUpDamage);
    } THEN {
        EXPECT_MUL_EQ(normalDamage, Q_4_12(1.5), chargedUpDamage);
    }
}

SINGLE_BATTLE_TEST("Solar Kick used in the sun doubles the damage of the next Fire move of the user")
{
    s16 normalDamage = 0;
    s16 chargedUpDamage = 0;
    GIVEN {
        PLAYER(SPECIES_BLAZIKEN_TERA);
        OPPONENT(SPECIES_WOBBUFFET) { HP(1000); MaxHP(1000); }; // pls don't die
    } WHEN {
        TURN { MOVE(player, MOVE_SUNNY_DAY); }
        TURN { MOVE(player, MOVE_FLAMETHROWER); }
        TURN { MOVE(player, MOVE_SOLAR_KICK); }
        TURN { MOVE(player, MOVE_FLAMETHROWER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLAMETHROWER, player);
        HP_BAR(opponent, captureDamage: &normalDamage);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SOLAR_KICK, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLAMETHROWER, player);
        HP_BAR(opponent, captureDamage: &chargedUpDamage);
    } THEN {
        EXPECT_MUL_EQ(normalDamage, Q_4_12(1.5), chargedUpDamage);
    }
}