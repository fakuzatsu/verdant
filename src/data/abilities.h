const struct Ability gAbilitiesInfo[ABILITIES_COUNT] =
{
    [ABILITY_NONE] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("No special ability."),
        .aiRating = 0,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_STENCH] =
    {
        .name = _("Stench"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Releases a stench that may cause\nan attacked foe to flinch."),
#else
        .description = COMPOUND_STRING("May cause a foe to flinch."),
#endif
        .aiRating = 1,
    },

    [ABILITY_DRIZZLE] =
    {
        .name = _("Drizzle"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon makes it rain\nwhen it enters a battle."),
#else
        .description = COMPOUND_STRING("Summons rain in battle."),
#endif
        .aiRating = 9,
    },

    [ABILITY_SPEED_BOOST] =
    {
        .name = _("Speed Boost"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's Speed stat\nis boosted every turn."),
#else
        .description = COMPOUND_STRING("Gradually boosts Speed."),
#endif
        .aiRating = 9,
    },

    [ABILITY_BATTLE_ARMOR] =
    {
        .name = _("Battle Armor"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Hard armor protects the\nPokémon from critical hits."),
#else
        .description = COMPOUND_STRING("Blocks critical hits."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_STURDY] =
    {
        .name = _("Sturdy"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon can't be knocked out\nin one hit or by one-hit KO moves."),
#else
        .description = COMPOUND_STRING("Negates 1-hit KO attacks."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DAMP] =
    {
        .name = _("Damp"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Prevents self-destruction by\ndampening its surroundings."),
#else
        .description = COMPOUND_STRING("Prevents self-destruction."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_LIMBER] =
    {
        .name = _("Limber"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's limber body prevents\nit from being paralyzed."),
#else
        .description = COMPOUND_STRING("Prevents paralysis."),
#endif
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SAND_VEIL] =
    {
        .name = _("Sand Veil"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's evasiveness\nin a sandstorm."),
#else
        .description = COMPOUND_STRING("Ups evasion in a sandstorm."),
#endif
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_STATIC] =
    {
        .name = _("Static"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon is statically-charged.\nContact with it may cause paralysis."),
#else
        .description = COMPOUND_STRING("Paralyzes on contact."),
#endif
        .aiRating = 4,
    },

    [ABILITY_VOLT_ABSORB] =
    {
        .name = _("Volt Absorb"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Restores HP if hit by Electric-type\nmoves instead of taking damage."),
#else
        .description = COMPOUND_STRING("Turns electricity into HP."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_WATER_ABSORB] =
    {
        .name = _("Water Absorb"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Restores HP if hit by Water-type\nmoves instead of taking damage."),
#else
        .description = COMPOUND_STRING("Changes water into HP."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_OBLIVIOUS] =
    {
        .name = _("Oblivious"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Keeps the Pokémon from being\ninfatuated, taunted, or intimidated."),
#else
        .description = COMPOUND_STRING("Prevents attraction."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_CLOUD_NINE] =
    {
        .name = _("Cloud Nine"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Eliminates the effects of weather."),
#else
        .description = COMPOUND_STRING("Negates weather effects."),
#endif
        .aiRating = 5,
    },

    [ABILITY_COMPOUND_EYES] =
    {
        .name = _("Compound Eyes"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's compound eyes\nboost its accuracy."),
#else
        .description = COMPOUND_STRING("Raises accuracy."),
#endif
        .aiRating = 7,
    },

    [ABILITY_INSOMNIA] =
    {
        .name = _("Insomnia"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's insomnia prevents it\nfrom falling asleep."),
#else
        .description = COMPOUND_STRING("Prevents sleep."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_COLOR_CHANGE] =
    {
        .name = _("Color Change"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's type becomes the\ntype of the move used on it."),
#else
        .description = COMPOUND_STRING("Changes type to foe's move."),
#endif
        .aiRating = 2,
    },

    [ABILITY_IMMUNITY] =
    {
        .name = _("Immunity"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's immune system\nprevents it from being poisoned."),
#else
        .description = COMPOUND_STRING("Prevents poisoning."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_FLASH_FIRE] =
    {
        .name = _("Flash Fire"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Absorbs Fire-type moves to power up\nthe Pokémon's own Fire-type moves."),
#else
        .description = COMPOUND_STRING("Powers up if hit by fire."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SHIELD_DUST] =
    {
        .name = _("Shield Dust"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Protective dust shields the Pokémon\nfrom additional move effects."),
#else
        .description = COMPOUND_STRING("Prevents added effects."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_OWN_TEMPO] =
    {
        .name = _("Own Tempo"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("This Pokémon has its own tempo, and\ncannot be confused or intimidated."),
#else
        .description = COMPOUND_STRING("Prevents confusion."),
#endif
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SUCTION_CUPS] =
    {
        .name = _("Suction Cups"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon is firmly anchored.\nIt cannot be forcibly switched out."),
#else
        .description = COMPOUND_STRING("Firmly anchors the body."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_INTIMIDATE] =
    {
        .name = _("Intimidate"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon lowers the foe's\nAttack stat upon entering battle."),
#else
        .description = COMPOUND_STRING("Lowers the foe's Attack."),
#endif
        .aiRating = 7,
    },

    [ABILITY_SHADOW_TAG] =
    {
        .name = _("Shadow Tag"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon steps on the foe's\nshadow to prevent it from escaping."),
#else
        .description = COMPOUND_STRING("Prevents the foe's escape."),
#endif
        .aiRating = 10,
    },

    [ABILITY_ROUGH_SKIN] =
    {
        .name = _("Rough Skin"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's rough skin damages\nfoes making direct contact with it."),
#else
        .description = COMPOUND_STRING("Hurts to touch."),
#endif
        .aiRating = 6,
    },

    [ABILITY_WONDER_GUARD] =
    {
        .name = _("Wonder Guard"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's mysterious power only\nlets supereffective moves hit it."),
#else
        .description = COMPOUND_STRING("“Supereffective” hits."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_LEVITATE] =
    {
        .name = _("Levitate"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("By floating in the air, the Pokémon\nis immune to all Ground-type moves."),
#else
        .description = COMPOUND_STRING("Not hit by Ground attacks."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EFFECT_SPORE] =
    {
        .name = _("Effect Spore"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Contact with the Pokémon may inflict\npoison, sleep, or paralysis."),
#else
        .description = COMPOUND_STRING("Leaves spores on contact."),
#endif
        .aiRating = 4,
    },

    [ABILITY_SYNCHRONIZE] =
    {
        .name = _("Synchronize"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Passes poison, paralysis, or burn\nto the Pokémon that inflicted it."),
#else
        .description = COMPOUND_STRING("Passes on status problems."),
#endif
        .aiRating = 4,
    },

    [ABILITY_CLEAR_BODY] =
    {
        .name = _("Clear Body"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Prevents other Pokémon's moves or\nAbilities from lowering its stats."),
#else
        .description = COMPOUND_STRING("Prevents ability reduction."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_NATURAL_CURE] =
    {
        .name = _("Natural Cure"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's status conditions\nare cured when it switches out."),
#else
        .description = COMPOUND_STRING("Heals upon switching out."),
#endif
        .aiRating = 7,
    },

    [ABILITY_LIGHTNING_ROD] =
    {
        .name = _("Lightning Rod"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Draws in Electric-type moves to\nboost Sp. Atk without getting hit."),
#else
        .description = COMPOUND_STRING("Draws electrical moves."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_SERENE_GRACE] =
    {
        .name = _("Serene Grace"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the likelihood of additional\neffects occurring when attacking."),
#else
        .description = COMPOUND_STRING("Promotes added effects."),
#endif
        .aiRating = 8,
    },

    [ABILITY_SWIFT_SWIM] =
    {
        .name = _("Swift Swim"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed\nstat in rain."),
#else
        .description = COMPOUND_STRING("Raises Speed in rain."),
#endif
        .aiRating = 6,
    },

    [ABILITY_CHLOROPHYLL] =
    {
        .name = _("Chlorophyll"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed\nstat in harsh sunlight."),
#else
        .description = COMPOUND_STRING("Raises Speed in sunshine."),
#endif
        .aiRating = 6,
    },

    [ABILITY_ILLUMINATE] =
    {
        .name = _("Illuminate"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Illuminates its surroundings.\nIts acccuracy cannot be lowered."),
#else
        .description = COMPOUND_STRING("Encounter rate increases."),
#endif
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_TRACE] =
    {
        .name = _("Trace"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When it enters a battle, the Pokémon\ncopies a foe Pokémon's ability."),
#else
        .description = COMPOUND_STRING("Copies special ability."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_HUGE_POWER] =
    {
        .name = _("Huge Power"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Doubles the Pokémon's Attack stat."),
#else
        .description = COMPOUND_STRING("Raises Attack."),
#endif
        .aiRating = 10,
    },

    [ABILITY_POISON_POINT] =
    {
        .name = _("Poison Point"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Contact with the Pokémon\nmay poison the attacker."),
#else
        .description = COMPOUND_STRING("Poisons foe on contact."),
#endif
        .aiRating = 4,
    },

    [ABILITY_INNER_FOCUS] =
    {
        .name = _("Inner Focus"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon is intensely focused and\ncannot be flinched or intimidated."),
#else
        .description = COMPOUND_STRING("Prevents flinching."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MAGMA_ARMOR] =
    {
        .name = _("Magma Armor"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's hot magma coating\nprevents it from being frozen."),
#else
        .description = COMPOUND_STRING("Prevents freezing."),
#endif
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_WATER_VEIL] =
    {
        .name = _("Water Veil"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's water veil prevents\nit from being burned."),
#else
        .description = COMPOUND_STRING("Prevents burns."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_MAGNET_PULL] =
    {
        .name = _("Magnet Pull"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Prevents Steel-type Pokémon from\nfleeing with its magenetic force."),
#else
        .description = COMPOUND_STRING("Traps Steel-type Pokémon."),
#endif
        .aiRating = 9,
    },

    [ABILITY_SOUNDPROOF] =
    {
        .name = _("Soundproof"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Soundproofing gives the Pokémon\nimmunity to all sound-based moves."),
#else
        .description = COMPOUND_STRING("Avoids sound-based moves."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_RAIN_DISH] =
    {
        .name = _("Rain Dish"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon gradually\nregains HP in rain."),
#else
        .description = COMPOUND_STRING("Slight HP recovery in rain."),
#endif
        .aiRating = 3,
    },

    [ABILITY_SAND_STREAM] =
    {
        .name = _("Sand Stream"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon summons a sandstorm\nwhen it enters a battle."),
#else
        .description = COMPOUND_STRING("Summons a sandstorm."),
#endif
        .aiRating = 9,
    },

    [ABILITY_PRESSURE] =
    {
        .name = _("Pressure"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Puts other Pokémon under pressure.\nThey will expend more PP to attack."),
#else
        .description = COMPOUND_STRING("Raises foe's PP usage."),
#endif
        .aiRating = 5,
    },

    [ABILITY_THICK_FAT] =
    {
        .name = _("Thick Fat"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("A layer of fat halves the damage\ntaken from Fire- and Ice-type moves."),
#else
        .description = COMPOUND_STRING("Heat-and-cold protection."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EARLY_BIRD] =
    {
        .name = _("Early Bird"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon awakens from sleep\ntwice as fast as other Pokémon."),
#else
        .description = COMPOUND_STRING("Awakens quickly from sleep."),
#endif
        .aiRating = 4,
    },

    [ABILITY_FLAME_BODY] =
    {
        .name = _("Flame Body"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Contact with the Pokémon\nmay burn the attacker."),
#else
        .description = COMPOUND_STRING("Burns the foe on contact."),
#endif
        .aiRating = 4,
    },

    [ABILITY_RUN_AWAY] =
    {
        .name = _("Run Away"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Enables a sure getaway\nfrom wild Pokémon."),
#else
        .description = COMPOUND_STRING("Makes escaping easier."),
#endif
        .aiRating = 0,
    },

    [ABILITY_KEEN_EYE] =
    {
        .name = _("Keen Eye"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's keen eyes prevent its\naccuracy from being lowered."),
#else
        .description = COMPOUND_STRING("Prevents loss of accuracy."),
#endif
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_HYPER_CUTTER] =
    {
        .name = _("Hyper Cutter"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's mighty pincers prevent\nothers from lowering its Attack."),
#else
        .description = COMPOUND_STRING("Prevents Attack reduction."),
#endif
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PICKUP] =
    {
        .name = _("Pickup"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon may pick up an item\nboth in an out of battle."),
#else
        .description = COMPOUND_STRING("May pick up items."),
#endif
        .aiRating = 1,
    },

    [ABILITY_TRUANT] =
    {
        .name = _("Truant"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Each time the Pokémon uses a move,\nit loafs around the next turn."),
#else
        .description = COMPOUND_STRING("Moves only every two turns."),
#endif
        .aiRating = -2,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_HUSTLE] =
    {
        .name = _("Hustle"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Attack stat\nbut lowers its accuracy."),
#else
        .description = COMPOUND_STRING("Trades accuracy for power."),
#endif
        .aiRating = 7,
    },

    [ABILITY_CUTE_CHARM] =
    {
        .name = _("Cute Charm"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Contact with the Pokémon\nmay cause infatuation."),
#else
        .description = COMPOUND_STRING("Infatuates on contact."),
#endif
        .aiRating = 2,
    },

    [ABILITY_PLUS] =
    {
        .name = _("Plus"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Sp. Atk stat\nif an active ally has Plus or Minus."),
#else
        .description = COMPOUND_STRING("Powers up with Minus."),
#endif
        .aiRating = 0,
    },

    [ABILITY_MINUS] =
    {
        .name = _("Minus"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Sp. Atk stat\nif an active ally has Plus or Minus."),
#else
        .description = COMPOUND_STRING("Powers up with Plus."),
#endif
        .aiRating = 0,
    },

    [ABILITY_FORECAST] =
    {
        .name = _("Forecast"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Changes the Pokémon's type to Water,\nFire, or Ice dependent on weather."),
#else
        .description = COMPOUND_STRING("Changes with the weather."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_STICKY_HOLD] =
    {
        .name = _("Sticky Hold"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's held items are stuck\nfast and cannot be forcibly removed."),
#else
        .description = COMPOUND_STRING("Prevents item theft."),
#endif
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SHED_SKIN] =
    {
        .name = _("Shed Skin"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon may cure its own status\nconditions by shedding its skin."),
#else
        .description = COMPOUND_STRING("Heals the body by shedding."),
#endif
        .aiRating = 7,
    },

    [ABILITY_GUTS] =
    {
        .name = _("Guts"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("It's so gutsy that having a status\ncondition boosts its Attack stat."),
#else
        .description = COMPOUND_STRING("Ups Attack if suffering."),
#endif
        .aiRating = 6,
    },

    [ABILITY_MARVEL_SCALE] =
    {
        .name = _("Marvel Scale"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Its scales boost its Defense stat\nif it has a status condition."),
#else
        .description = COMPOUND_STRING("Ups Defense if suffering."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_LIQUID_OOZE] =
    {
        .name = _("Liquid Ooze"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's oozed liquid damages\nattackers that drain its HP."),
#else
        .description = COMPOUND_STRING("Draining causes injury."),
#endif
        .aiRating = 3,
    },

    [ABILITY_OVERGROW] =
    {
        .name = _("Overgrow"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers ups Grass-type moves when\nthe Pokémon's HP is low."),
#else
        .description = COMPOUND_STRING("Ups Grass moves in a pinch."),
#endif
        .aiRating = 5,
    },

    [ABILITY_BLAZE] =
    {
        .name = _("Blaze"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers ups Fire-type moves when\nthe Pokémon's HP is low."),
#else
        .description = COMPOUND_STRING("Ups Fire moves in a pinch."),
#endif
        .aiRating = 5,
    },

    [ABILITY_TORRENT] =
    {
        .name = _("Torrent"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers ups Water-type moves when\nthe Pokémon's HP is low."),
#else
        .description = COMPOUND_STRING("Ups Water moves in a pinch."),
#endif
        .aiRating = 5,
    },

    [ABILITY_SWARM] =
    {
        .name = _("Swarm"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers ups Bug-type moves when\nthe Pokémon's HP is low."),
#else
        .description = COMPOUND_STRING("Ups Bug moves in a pinch."),
#endif
        .aiRating = 5,
    },

    [ABILITY_ROCK_HEAD] =
    {
        .name = _("Rock Head"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Protects the Pokémon\nfrom recoil damage."),
#else
        .description = COMPOUND_STRING("Prevents recoil damage."),
#endif
        .aiRating = 5,
    },

    [ABILITY_DROUGHT] =
    {
        .name = _("Drought"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Turns the sunlight harsh when\nthe Pokémon enters battle."),
#else
        .description = COMPOUND_STRING("Summons sunlight in battle."),
#endif
        .aiRating = 9,
    },

    [ABILITY_ARENA_TRAP] =
    {
        .name = _("Arena Trap"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Prevents the opposing Pokémon\nfrom fleeing battle."),
#else
        .description = COMPOUND_STRING("Prevents fleeing."),
#endif
        .aiRating = 9,
    },

    [ABILITY_VITAL_SPIRIT] =
    {
        .name = _("Vital Spirit"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's incredible vitality\nprevents it from falling asleep."),
#else
        .description = COMPOUND_STRING("Prevents sleep."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_WHITE_SMOKE] =
    {
        .name = _("White Smoke"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Its protective white smoke prevents\nothers from lowering its stats."),
#else
        .description = COMPOUND_STRING("Prevents ability reduction."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_PURE_POWER] =
    {
        .name = _("Pure Power"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Using its pure power, the Pokémon\ndoubles its Attack stat."),
#else
        .description = COMPOUND_STRING("Raises Attack."),
#endif
        .aiRating = 10,
    },

    [ABILITY_SHELL_ARMOR] =
    {
        .name = _("Shell Armor"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("A hard shell protects the Pokémon\nfrom critical hits."),
#else
        .description = COMPOUND_STRING("Blocks critical hits."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_AIR_LOCK] =
    {
        .name = _("Air Lock"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Eliminates the effects of weather."),
#else
        .description = COMPOUND_STRING("Negates weather effects."),
#endif
        .aiRating = 5,
    },

    [ABILITY_TANGLED_FEET] =
    {
        .name = _("Tangled Feet"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's evasiveness\nif it is confused."),
#else
        .description = COMPOUND_STRING("Ups evasion if confused."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MOTOR_DRIVE] =
    {
        .name = _("Motor Drive"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Incoming Electric-type moves do no\ndamage and boost Speed instead."),
#else
        .description = COMPOUND_STRING("Electricity raises Speed."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_RIVALRY] =
    {
        .name = _("Rivalry"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("It does more damage to the same\ngender but less to the opposite."),
#else
        .description = COMPOUND_STRING("Powers up against rivals."),
#endif
        .aiRating = 1,
    },

    [ABILITY_STEADFAST] =
    {
        .name = _("Steadfast"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's determination boosts\nits Speed stat when it flinches."),
#else
        .description = COMPOUND_STRING("Flinching raises Speed."),
#endif
        .aiRating = 2,
    },

    [ABILITY_SNOW_CLOAK] =
    {
        .name = _("Snow Cloak"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's\nevasiveness in snow."),
#else
        .description = COMPOUND_STRING("Ups evasion in Hail or Snow."),
#endif
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_GLUTTONY] =
    {
        .name = _("Gluttony"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon will eat its held Berry\nat half HP instead of at quarter HP."),
#else
        .description = COMPOUND_STRING("Eats Berries early."),
#endif
        .aiRating = 3,
    },

    [ABILITY_ANGER_POINT] =
    {
        .name = _("Anger Point"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon is angered when hit\ncritically, maxing its Attack stat."),
#else
        .description = COMPOUND_STRING("Critical hits raise Attack."),
#endif
        .aiRating = 4,
    },

    [ABILITY_UNBURDEN] =
    {
        .name = _("Unburden"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Speed stat if the\nPokémon's held item is used or lost."),
#else
        .description = COMPOUND_STRING("Using a hold item ups Speed."),
#endif
        .aiRating = 7,
    },

    [ABILITY_HEATPROOF] =
    {
        .name = _("Heatproof"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("With its heatproof body, it halves\ndamage taken from Fire-type moves."),
#else
        .description = COMPOUND_STRING("Heat and burn protection."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SIMPLE] =
    {
        .name = _("Simple"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Doubles the effects of\nthe Pokémon's stat changes."),
#else
        .description = COMPOUND_STRING("Prone to wild stat changes."),
#endif
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_DRY_SKIN] =
    {
        .name = _("Dry Skin"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Water and rain restore its HP, but\nit's damaged by sun and Fire."),
#else
        .description = COMPOUND_STRING("Prefers moisture to heat."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DOWNLOAD] =
    {
        .name = _("Download"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon analyzes a foe to raise\nits most-effective attacking stat."),
#else
        .description = COMPOUND_STRING("Adjusts power favorably."),
#endif
        .aiRating = 7,
    },

    [ABILITY_IRON_FIST] =
    {
        .name = _("Iron Fist"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up punching moves."),
#else
        .description = COMPOUND_STRING("Boosts punching moves."),
#endif
        .aiRating = 6,
    },

    [ABILITY_POISON_HEAL] =
    {
        .name = _("Poison Heal"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("If poisoned, the Pokémon has its HP\nrestored instead of taking damage."),
#else
        .description = COMPOUND_STRING("Restores HP if poisoned."),
#endif
        .aiRating = 8,
    },

    [ABILITY_ADAPTABILITY] =
    {
        .name = _("Adaptability"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up moves of the same type\nas the Pokémon."),
#else
        .description = COMPOUND_STRING("Boosts same type attacks."),
#endif
        .aiRating = 8,
    },

    [ABILITY_SKILL_LINK] =
    {
        .name = _("Skill Link"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Maximizes the number of times\na multistrike move hits."),
#else
        .description = COMPOUND_STRING("Multi-hit moves hit 5 times."),
#endif
        .aiRating = 7,
    },

    [ABILITY_HYDRATION] =
    {
        .name = _("Hydration"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Cures the Pokémon's status\nconditions in rain."),
#else
        .description = COMPOUND_STRING("Cures status in rain."),
#endif
        .aiRating = 4,
    },

    [ABILITY_SOLAR_POWER] =
    {
        .name = _("Solar Power"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Sp. Atk stat in sunlight,\nbut HP is decreased every turn."),
#else
        .description = COMPOUND_STRING("Powers up in sunshine."),
#endif
        .aiRating = 3,
    },

    [ABILITY_QUICK_FEET] =
    {
        .name = _("Quick Feet"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Speed stat if the Pokémon\nhas a status condition."),
#else
        .description = COMPOUND_STRING("Ups Speed if suffering."),
#endif
        .aiRating = 5,
    },

    [ABILITY_NORMALIZE] =
    {
        .name = _("Normalize"),
#if BW_SUMMARY_SCREEN == TRUE
       .description = COMPOUND_STRING("All its moves become Normal-type\nand their power is boosted."),
#else
        .description = COMPOUND_STRING("Moves become Normal-type."),
#endif
        .aiRating = -1,
    },

    [ABILITY_SNIPER] =
    {
        .name = _("Sniper"),
#if BW_SUMMARY_SCREEN == TRUE
       .description = COMPOUND_STRING("If the Pokémon lands a critical hit,\nthe attack is powered up further."),
#else
        .description = COMPOUND_STRING("Boosts critical hits."),
#endif
        .aiRating = 3,
    },

    [ABILITY_MAGIC_GUARD] =
    {
        .name = _("Magic Guard"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon only takes damage\nfrom attacks."),
#else
        .description = COMPOUND_STRING("Only damaged by attacks."),
#endif
        .aiRating = 9,
    },

    [ABILITY_NO_GUARD] =
    {
        .name = _("No Guard"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon ensures incoming\nand outgoing attacks never miss."),
#else
        .description = COMPOUND_STRING("Ensures that all moves hit."),
#endif
        .aiRating = 8,
    },

    [ABILITY_STALL] =
    {
        .name = _("Stall"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon is always the last\nto use its moves."),
#else
        .description = COMPOUND_STRING("Always moves last."),
#endif
        .aiRating = -1,
    },

    [ABILITY_TECHNICIAN] =
    {
        .name = _("Technician"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up weak moves so the Pokémon\ncan deal more damage with them."),
#else
        .description = COMPOUND_STRING("Boosts weaker moves."),
#endif
        .aiRating = 8,
    },

    [ABILITY_LEAF_GUARD] =
    {
        .name = _("Leaf Guard"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Prevents status conditions\nin harsh sunlight."),
#else
        .description = COMPOUND_STRING("Blocks status in sunshine."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_KLUTZ] =
    {
        .name = _("Klutz"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon can't use\nany held items."),
#else
        .description = COMPOUND_STRING("Can't use hold items."),
#endif
        .aiRating = -1,
    },

    [ABILITY_MOLD_BREAKER] =
    {
        .name = _("Mold Breaker"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's moves are unimpeded\nby the Ability of the target."),
#else
        .description = COMPOUND_STRING("Moves hit through abilities."),
#endif
        .aiRating = 7,
    },

    [ABILITY_SUPER_LUCK] =
    {
        .name = _("Super Luck"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon is so lucky that its\nmoves critically hit more often."),
#else
        .description = COMPOUND_STRING("Critical hits land often."),
#endif
        .aiRating = 3,
    },

    [ABILITY_AFTERMATH] =
    {
        .name = _("Aftermath"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Damages the attacker that knocks it\nout with a direct contact move."),
#else
        .description = COMPOUND_STRING("Fainting damages the foe."),
#endif
        .aiRating = 5,
    },

    [ABILITY_ANTICIPATION] =
    {
        .name = _("Anticipation"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon can sense an opposing\nPokémon's dangerous moves."),
#else
        .description = COMPOUND_STRING("Senses dangerous moves."),
#endif
        .aiRating = 2,
    },

    [ABILITY_FOREWARN] =
    {
        .name = _("Forewarn"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon can tell one of its foe's\nmoves when entering battle."),
#else
        .description = COMPOUND_STRING("Determines a foe's move."),
#endif
        .aiRating = 2,
    },

    [ABILITY_UNAWARE] =
    {
        .name = _("Unaware"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When attacking, the Pokémon ignores\nthe target's stat changes."),
#else
        .description = COMPOUND_STRING("Ignores stat changes."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_TINTED_LENS] =
    {
        .name = _("Tinted Lens"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's “not very effective”\nmoves deal regular damage."),
#else
        .description = COMPOUND_STRING("Ups “not very effective”."),
#endif
        .aiRating = 7,
    },

    [ABILITY_FILTER] =
    {
        .name = _("Filter"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Reduces the power of supereffective\nattacks that hit the Pokémon."),
#else
        .description = COMPOUND_STRING("Weakens “supereffective”."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SLOW_START] =
    {
        .name = _("Slow Start"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("For five turns, the Pokémon's\nAttack and Speed stats are halved."),
#else
        .description = COMPOUND_STRING("Takes a while to get going."),
#endif
        .aiRating = -2,
    },

    [ABILITY_SCRAPPY] =
    {
        .name = _("Scrappy"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Hits Ghost types with Fighting- and\nNormal-type moves. Not intimidated."),
#else
        .description = COMPOUND_STRING("Hits Ghost-type Pokémon."),
#endif
        .aiRating = 6,
    },

    [ABILITY_STORM_DRAIN] =
    {
        .name = _("Storm Drain"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Draws in Water-type moves to\nboost Sp. Atk without getting hit."),
#else
        .description = COMPOUND_STRING("Draws in Water moves."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_ICE_BODY] =
    {
        .name = _("Ice Body"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon gradually\nregains HP in snow."),
#else
        .description = COMPOUND_STRING("HP recovery in Hail or Snow."),
#endif
        .aiRating = 3,
    },

    [ABILITY_SOLID_ROCK] =
    {
        .name = _("Solid Rock"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Reduces the power of supereffective\nattacks that hit the Pokémon."),
#else
        .description = COMPOUND_STRING("Weakens “supereffective”."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SNOW_WARNING] =
    {
        .name = _("Snow Warning"),
#if BW_SUMMARY_SCREEN == TRUE
    #if B_SNOW_WARNING >= GEN_9
        .description = COMPOUND_STRING("The Pokémon makes it snow\nwhen it enters a battle."),
    #else
        .description = COMPOUND_STRING("The Pokémon makes it hail\nwhen it enters a battle."),
    #endif
#else // BW_SUMMARY_SCREEN
    #if B_SNOW_WARNING >= GEN_9
        .description = COMPOUND_STRING("Summons snow in battle."),
    #else
        .description = COMPOUND_STRING("Summons hail in battle."),
    #endif
#endif // BW_SUMMARY_SCREEN
        .aiRating = 8,
    },

    [ABILITY_HONEY_GATHER] =
    {
        .name = _("Honey Gather"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon may gather Honey\nafter a battle."),
#else
        .description = COMPOUND_STRING("May gather Honey."),
#endif
        .aiRating = 0,
    },

    [ABILITY_FRISK] =
    {
        .name = _("Frisk"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When it enters battle, the Pokémon\nchecks a foe's held item."),
#else
        .description = COMPOUND_STRING("Checks a foe's item."),
#endif
        .aiRating = 3,
    },

    [ABILITY_RECKLESS] =
    {
        .name = _("Reckless"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up moves that\nhave recoil damage."),
#else
        .description = COMPOUND_STRING("Boosts moves with recoil."),
#endif
        .aiRating = 6,
    },

    [ABILITY_MULTITYPE] =
    {
        .name = _("Multitype"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Changes the Pokémon's type to match\nthe plate it holds."),
#else
        .description = COMPOUND_STRING("Changes type to its Plate."),
#endif
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_FLOWER_GIFT] =
    {
        .name = _("Flower Gift"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Attack and Sp. Def stats\nof itself and allies in sunlight."),
#else
        .description = COMPOUND_STRING("Allies power up in sunshine."),
#endif
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_BAD_DREAMS] =
    {
        .name = _("Bad Dreams"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Damages opposing Pokémon\nthat are asleep."),
#else
        .description = COMPOUND_STRING("Damages sleeping Pokémon."),
#endif
        .aiRating = 4,
    },

    [ABILITY_PICKPOCKET] =
    {
        .name = _("Pickpocket"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon steals an attacker's\nheld item when they make contact."),
#else
        .description = COMPOUND_STRING("Steals the foe's held item."),
#endif
        .aiRating = 3,
    },

    [ABILITY_SHEER_FORCE] =
    {
        .name = _("Sheer Force"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Additional move effects don't occur,\nbut the moves' power is boosted."),
#else
        .description = COMPOUND_STRING("Trades effects for power."),
#endif
        .aiRating = 8,
    },

    [ABILITY_CONTRARY] =
    {
        .name = _("Contrary"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Inverts stat changes. Boosting stats\nlowers them instead, and vice versa."),
#else
        .description = COMPOUND_STRING("Inverts stat changes."),
#endif
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_UNNERVE] =
    {
        .name = _("Unnerve"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Unnerves opposing Pokémon and\nmakes them unable to eat Berries."),
#else
        .description = COMPOUND_STRING("Foes can't eat Berries."),
#endif
        .aiRating = 3,
    },

    [ABILITY_DEFIANT] =
    {
        .name = _("Defiant"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Sharply boosts the Pokémon's Attack\nstat if a foe lowers its stats."),
#else
        .description = COMPOUND_STRING("Lowered stats up Attack."),
#endif
        .aiRating = 5,
    },

    [ABILITY_DEFEATIST] =
    {
        .name = _("Defeatist"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Halves the Pokémon's Attack and Sp.\nAtk stats when HP is half or less."),
#else
        .description = COMPOUND_STRING("Gives up at half HP."),
#endif
        .aiRating = -1,
    },

    [ABILITY_CURSED_BODY] =
    {
        .name = _("Cursed Body"),
#if BW_SUMMARY_SCREEN == TRUE
       .description = COMPOUND_STRING("May disable a move that has dealt\ndamage to the Pokémon."),
#else
        .description = COMPOUND_STRING("Disables moves on contact."),
#endif
        .aiRating = 4,
    },

    [ABILITY_HEALER] =
    {
        .name = _("Healer"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Sometimes cures the status\nconditions of the Pokémon's allies."),
#else
        .description = COMPOUND_STRING("Heals partner Pokémon."),
#endif
        .aiRating = 0,
    },

    [ABILITY_FRIEND_GUARD] =
    {
        .name = _("Friend Guard"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Reduces damage dealt to allies."),
#else
        .description = COMPOUND_STRING("Lowers damage to partner."),
#endif
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_WEAK_ARMOR] =
    {
        .name = _("Weak Armor"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When physically hit, its Defense stat\nfalls and Speed is sharply boosted."),
#else
        .description = COMPOUND_STRING("Its stats change when hit."),
#endif
        .aiRating = 2,
    },

    [ABILITY_HEAVY_METAL] =
    {
        .name = _("Heavy Metal"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Doubles the Pokémon's weight."),
#else
        .description = COMPOUND_STRING("Doubles weight."),
#endif
        .aiRating = -1,
        .breakable = TRUE,
    },

    [ABILITY_LIGHT_METAL] =
    {
        .name = _("Light Metal"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Halves the Pokémon's weight."),
#else
        .description = COMPOUND_STRING("Halves weight."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MULTISCALE] =
    {
        .name = _("Multiscale"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Reduces the amount of damage the\nPokémon takes while its HP is full."),
#else
        .description = COMPOUND_STRING("Halves damage at full HP."),
#endif
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_TOXIC_BOOST] =
    {
        .name = _("Toxic Boost"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up physical moves when\nthe Pokémon is poisoned."),
#else
        .description = COMPOUND_STRING("Ups Attack if poisoned."),
#endif
        .aiRating = 6,
    },

    [ABILITY_FLARE_BOOST] =
    {
        .name = _("Flare Boost"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up special moves when\nthe Pokémon is burned."),
#else
        .description = COMPOUND_STRING("Ups Sp. Atk if burned."),
#endif
        .aiRating = 5,
    },

    [ABILITY_HARVEST] =
    {
        .name = _("Harvest"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("May create another Berry\nafter one is used."),
#else
        .description = COMPOUND_STRING("May recycle a used Berry."),
#endif
        .aiRating = 5,
    },

    [ABILITY_TELEPATHY] =
    {
        .name = _("Telepathy"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon anticipates and dodges\nthe attacks of its allies."),
#else
        .description = COMPOUND_STRING("Can't be damaged by an ally."),
#endif
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_MOODY] =
    {
        .name = _("Moody"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Every turn, one stat will be sharply\nboosted and another lowered."),
#else
        .description = COMPOUND_STRING("Stats change gradually."),
#endif
        .aiRating = 10,
    },

    [ABILITY_OVERCOAT] =
    {
        .name = _("Overcoat"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon is protected from\nsandstorms, powders, and spores."),
#else
        .description = COMPOUND_STRING("Blocks weather and powder."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_POISON_TOUCH] =
    {
        .name = _("Poison Touch"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("May poison a target when\nthe Pokémon makes contact."),
#else
        .description = COMPOUND_STRING("Poisons foe on contact."),
#endif
        .aiRating = 4,
    },

    [ABILITY_REGENERATOR] =
    {
        .name = _("Regenerator"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon has a little of its HP\nrestored when withdrawn from battle."),
#else
        .description = COMPOUND_STRING("Heals upon switching out."),
#endif
        .aiRating = 8,
    },

    [ABILITY_BIG_PECKS] =
    {
        .name = _("Big Pecks"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Prevents the Pokémon from having\nits Defense stat lowered."),
#else
        .description = COMPOUND_STRING("Prevents Defense loss."),
#endif
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_SAND_RUSH] =
    {
        .name = _("Sand Rush"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed stat\nin a sandstorm."),
#else
        .description = COMPOUND_STRING("Ups Speed in a sandstorm."),
#endif
        .aiRating = 6,
    },

    [ABILITY_WONDER_SKIN] =
    {
        .name = _("Wonder Skin"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Makes status moves more likely\nto miss the Pokémon."),
#else
        .description = COMPOUND_STRING("May avoid status problems."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANALYTIC] =
    {
        .name = _("Analytic"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the power of the Pokémon's\nmove if it acts last that turn."),
#else
        .description = COMPOUND_STRING("Moving last boosts power."),
#endif
        .aiRating = 5,
    },

    [ABILITY_ILLUSION] =
    {
        .name = _("Illusion"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Comes out disguised as the Pokémon\nin the party's last spot."),
#else
        .description = COMPOUND_STRING("Appears as a partner."),
#endif
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_IMPOSTER] =
    {
        .name = _("Imposter"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon transforms itself\ninto the Pokémon it's facing."),
#else
        .description = COMPOUND_STRING("Transforms into the foe."),
#endif
        .aiRating = 9,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_INFILTRATOR] =
    {
        .name = _("Infiltrator"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's moves penetrate the\ntarget's barriers, substitutes, etc."),
#else
        .description = COMPOUND_STRING("Passes through barriers."),
#endif
        .aiRating = 6,
    },

    [ABILITY_MUMMY] =
    {
        .name = _("Mummy"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Contact with the Pokémon changes\nthe attacker's Ability to Mummy."),
#else
        .description = COMPOUND_STRING("Spreads with contact."),
#endif
        .aiRating = 5,
    },

    [ABILITY_MOXIE] =
    {
        .name = _("Moxie"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Knocking out a Pokémon shows moxie,\nand that boosts its Attack stat."),
#else
        .description = COMPOUND_STRING("KOs raise Attack."),
#endif
        .aiRating = 7,
    },

    [ABILITY_JUSTIFIED] =
    {
        .name = _("Justified"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When hit by a Dark-type move, its\nAttack stat is boosted for justice."),
#else
        .description = COMPOUND_STRING("Dark hits raise Attack."),
#endif
        .aiRating = 4,
    },

    [ABILITY_RATTLED] =
    {
        .name = _("Rattled"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Ups Speed when intimidated or hit by\nBug-, Ghost-, or Dark-type moves."),
#else
        .description = COMPOUND_STRING("Raises Speed when scared."),
#endif
        .aiRating = 3,
    },

    [ABILITY_MAGIC_BOUNCE] =
    {
        .name = _("Magic Bounce"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon reflects status moves\ninstead of getting hit by them."),
#else
        .description = COMPOUND_STRING("Reflects status moves."),
#endif
        .aiRating = 9,
        .breakable = TRUE,
    },

    [ABILITY_SAP_SIPPER] =
    {
        .name = _("Sap Sipper"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Incoming Grass-type moves do no\ndamage and boost Attack instead."),
#else
        .description = COMPOUND_STRING("Grass increases Attack."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_PRANKSTER] =
    {
        .name = _("Prankster"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Gives priority to the Pokémon's\nstatus moves."),
#else
        .description = COMPOUND_STRING("Status moves go first."),
#endif
        .aiRating = 8,
    },

    [ABILITY_SAND_FORCE] =
    {
        .name = _("Sand Force"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the power of Rock-, Ground-,\nand Steel-type moves in a sandstorm."),
#else
        .description = COMPOUND_STRING("Powers up in a sandstorm."),
#endif
        .aiRating = 4,
    },

    [ABILITY_IRON_BARBS] =
    {
        .name = _("Iron Barbs"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's iron barbs damage\nfoes making direct contact with it."),
#else
        .description = COMPOUND_STRING("Hurts to touch."),
#endif
        .aiRating = 6,
    },

    [ABILITY_ZEN_MODE] =
    {
        .name = _("Zen Mode"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Changes the Pokémon's shape when\nits HP drops to half or less."),
#else
        .description = COMPOUND_STRING("Transforms at half HP."),
#endif
        .aiRating = -1,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = B_UPDATED_ABILITY_DATA >= GEN_7,
    },

    [ABILITY_VICTORY_STAR] =
    {
        .name = _("Victory Star"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the accuracy of the Pokémon\nand its allies."),
#else
        .description = COMPOUND_STRING("Raises party accuracy."),
#endif
        .aiRating = 6,
    },

    [ABILITY_TURBOBLAZE] =
    {
        .name = _("Turboblaze"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's moves are unimpeded\nby the Ability of the target."),
#else
        .description = COMPOUND_STRING("Moves hit through abilities."),
#endif
        .aiRating = 7,
    },

    [ABILITY_TERAVOLT] =
    {
        .name = _("Teravolt"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's moves are unimpeded\nby the Ability of the target."),
#else
        .description = COMPOUND_STRING("Moves hit through abilities."),
#endif
        .aiRating = 7,
    },

    [ABILITY_AROMA_VEIL] =
    {
        .name = _("Aroma Veil"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Protects the Pokémon and its allies\nfrom effects that prevent move use."),
#else
        .description = COMPOUND_STRING("Prevents limiting of moves."),
#endif
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_FLOWER_VEIL] =
    {
        .name = _("Flower Veil"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Ally Grass types are protected from\nstatus conditions and stat lowering."),
#else
        .description = COMPOUND_STRING("Protects Grass-types."),
#endif
        .aiRating = 0,
    },

    [ABILITY_CHEEK_POUCH] =
    {
        .name = _("Cheek Pouch"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Eating a Berry restores HP\nin addition to its usual effect."),
#else
        .description = COMPOUND_STRING("Eating Berries restores HP."),
#endif
        .aiRating = 4,
    },

    [ABILITY_PROTEAN] =
    {
        .name = _("Protean"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Changes its type to that of a queued\nmove once per battle entrance."),
#else
        .description = COMPOUND_STRING("Changes type to used move."),
#endif
        .aiRating = 8,
    },

    [ABILITY_FUR_COAT] =
    {
        .name = _("Fur Coat"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Halves the damage from\nphysical moves."),
#else
        .description = COMPOUND_STRING("Raises Defense."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MAGICIAN] =
    {
        .name = _("Magician"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon steals the held item\nfrom any target it hits with a move."),
#else
        .description = COMPOUND_STRING("Steals the foe's held item."),
#endif
        .aiRating = 3,
    },

    [ABILITY_BULLETPROOF] =
    {
        .name = _("Bulletproof"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Protects the Pokémon from\nball and bomb moves."),
#else
        .description = COMPOUND_STRING("Avoids some projectiles."),
#endif
        .aiRating = 7,
    },

    [ABILITY_COMPETITIVE] =
    {
        .name = _("Competitive"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Sharply boosts the Pokémon's Sp. Atk\nstat if a foe lowers its stats."),
#else
        .description = COMPOUND_STRING("Lowered stats up Sp. Atk."),
#endif
        .aiRating = 5,
    },

    [ABILITY_STRONG_JAW] =
    {
        .name = _("Strong Jaw"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's strong jaw boosts\nthe power of its biting moves."),
#else
        .description = COMPOUND_STRING("Boosts biting moves."),
#endif
        .aiRating = 6,
    },

    [ABILITY_REFRIGERATE] =
    {
        .name = _("Refrigerate"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Normal-type moves become Ice-type\nand their power is boosted."),
#else
        .description = COMPOUND_STRING("Normal moves become Ice."),
#endif
        .aiRating = 8,
    },

    [ABILITY_SWEET_VEIL] =
    {
        .name = _("Sweet Veil"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Prevents the Pokémon and its allies\nfrom falling asleep."),
#else
        .description = COMPOUND_STRING("Prevents party from sleep."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_STANCE_CHANGE] =
    {
        .name = _("Stance Change"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Changes to Blade Forme by attacking,\nand Shield Forme with King's Shield."),
#else
        .description = COMPOUND_STRING("Transforms as it battles."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_GALE_WINGS] =
    {
        .name = _("Gale Wings"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Gives priority to Flying-type\nmoves while its HP is full."),
#else
        .description = COMPOUND_STRING("Flying moves go first."),
#endif
        .aiRating = 6,
    },

    [ABILITY_MEGA_LAUNCHER] =
    {
        .name = _("Mega Launcher"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up pulse moves."),
#else
        .description = COMPOUND_STRING("Boosts pulse moves."),
#endif
        .aiRating = 7,
    },

    [ABILITY_GRASS_PELT] =
    {
        .name = _("Grass Pelt"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Defense\nstat on Grassy Terrain."),
#else
        .description = COMPOUND_STRING("Ups Defense in grass."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SYMBIOSIS] =
    {
        .name = _("Symbiosis"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon passes its item to\nan ally that has used up an item."),
#else
        .description = COMPOUND_STRING("Passes its item to an ally."),
#endif
        .aiRating = 0,
    },

    [ABILITY_TOUGH_CLAWS] =
    {
        .name = _("Tough Claws"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up moves that\nmake direct contact."),
#else
        .description = COMPOUND_STRING("Boosts contact moves."),
#endif
        .aiRating = 7,
    },

    [ABILITY_PIXILATE] =
    {
        .name = _("Pixilate"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Normal-type moves become Fairy-\ntype and their power is boosted."),
#else
        .description = COMPOUND_STRING("Normal moves become Fairy."),
#endif
        .aiRating = 8,
    },

    [ABILITY_GOOEY] =
    {
        .name = _("Gooey"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Contact with the Pokémon lowers\nthe attacker's Speed stat."),
#else
        .description = COMPOUND_STRING("Lowers Speed on contact."),
#endif
        .aiRating = 5,
    },

    [ABILITY_AERILATE] =
    {
        .name = _("Aerilate"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Normal-type moves become Flying-\ntype and their power is boosted."),
#else
        .description = COMPOUND_STRING("Normal moves become Flying."),
#endif
        .aiRating = 8,
    },

    [ABILITY_PARENTAL_BOND] =
    {
        .name = _("Parental Bond"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The parent and child attack\none after the other."),
#else
        .description = COMPOUND_STRING("Moves hit twice."),
#endif
        .aiRating = 10,
    },

    [ABILITY_DARK_AURA] =
    {
        .name = _("Dark Aura"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up the Dark-type moves\nof all Pokémon on the field."),
#else
        .description = COMPOUND_STRING("Boosts Dark moves."),
#endif
        .aiRating = 6,
    },

    [ABILITY_FAIRY_AURA] =
    {
        .name = _("Fairy Aura"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up the Fairy-type moves\nof all Pokémon on the field."),
#else
        .description = COMPOUND_STRING("Boosts Fairy moves."),
#endif
        .aiRating = 6,
    },

    [ABILITY_AURA_BREAK] =
    {
        .name = _("Aura Break"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The effects of “Aura” Abilities are\nreversed, weakening affected moves."),
#else
        .description = COMPOUND_STRING("Reverse aura abilities."),
#endif
        .aiRating = 3,
    },

    [ABILITY_PRIMORDIAL_SEA] =
    {
        .name = _("Primordial Sea"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon changes the weather to\nnullify Fire-type attacks."),
#else
        .description = COMPOUND_STRING("Summons heavy rain."),
#endif
        .aiRating = 10,
    },

    [ABILITY_DESOLATE_LAND] =
    {
        .name = _("Desolate Land"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon changes the weather to\nnullify Water-type attacks."),
#else
        .description = COMPOUND_STRING("Summons intense sunlight."),
#endif
        .aiRating = 10,
    },

    [ABILITY_DELTA_STREAM] =
    {
        .name = _("Delta Stream"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon changes the weather so\nFlying-types have no weaknesses."),
#else
        .description = COMPOUND_STRING("Summons strong winds."),
#endif
        .aiRating = 10,
    },

    [ABILITY_STAMINA] =
    {
        .name = _("Stamina"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Defense stat when the\nPokémon is hit by an attack."),
#else
        .description = COMPOUND_STRING("Boosts Defense when hit."),
#endif
        .aiRating = 6,
    },

    [ABILITY_WIMP_OUT] =
    {
        .name = _("Wimp Out"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon cowardly switches out\nwhen its HP drops to half or less."),
#else
        .description = COMPOUND_STRING("Flees at half HP."),
#endif
        .aiRating = 3,
    },

    [ABILITY_EMERGENCY_EXIT] =
    {
        .name = _("Emergency Exit"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("It, sensing danger, switches out\nwhen its HP drops to half or less."),
#else
        .description = COMPOUND_STRING("Flees at half HP."),
#endif
        .aiRating = 3,
    },

    [ABILITY_WATER_COMPACTION] =
    {
        .name = _("Water Compaction"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Defense stat sharply\nwhen hit by a Water-type move."),
#else
        .description = COMPOUND_STRING("Water boosts Defense."),
#endif
        .aiRating = 4,
    },

    [ABILITY_MERCILESS] =
    {
        .name = _("Merciless"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's attacks hit critically\nif the target is poisoned."),
#else
        .description = COMPOUND_STRING("Criticals poisoned foes."),
#endif
        .aiRating = 4,
    },

    [ABILITY_SHIELDS_DOWN] =
    {
        .name = _("Shields Down"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("At half HP or less, its shell breaks\nand it becomes aggressive."),
#else
        .description = COMPOUND_STRING("Shell breaks at half HP."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_STAKEOUT] =
    {
        .name = _("Stakeout"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Doubles the damage dealt to a target\nthat has just switched into battle."),
#else
        .description = COMPOUND_STRING("Stronger as foes switch in."),
#endif
        .aiRating = 6,
    },

    [ABILITY_WATER_BUBBLE] =
    {
        .name = _("Water Bubble"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Halves Fire damage, prevents burns,\nand doubles Water-type move power."),
#else
        .description = COMPOUND_STRING("Guards from fire and burns."),
#endif
        .aiRating = 8,
    },

    [ABILITY_STEELWORKER] =
    {
        .name = _("Steelworker"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up Steel-type moves."),
#else
        .description = COMPOUND_STRING("Powers up Steel moves."),
#endif
        .aiRating = 6,
    },

    [ABILITY_BERSERK] =
    {
        .name = _("Berserk"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts its Sp. Atk stat when a hit\ndrops its HP to half or less."),
#else
        .description = COMPOUND_STRING("Boosts Sp. Atk at low HP."),
#endif
        .aiRating = 5,
    },

    [ABILITY_SLUSH_RUSH] =
    {
        .name = _("Slush Rush"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's\nSpeed stat in snow."),
#else
        .description = COMPOUND_STRING("Raises Speed in Hail/Snow."),
#endif
        .aiRating = 5,
    },

    [ABILITY_LONG_REACH] =
    {
        .name = _("Long Reach"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon uses its moves without\nmaking contact with the target."),
#else
        .description = COMPOUND_STRING("Never makes contact."),
#endif
        .aiRating = 3,
    },

    [ABILITY_LIQUID_VOICE] =
    {
        .name = _("Liquid Voice"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Sound-based moves become\nWater-type moves."),
#else
        .description = COMPOUND_STRING("Makes sound moves Water."),
#endif
        .aiRating = 5,
    },

    [ABILITY_TRIAGE] =
    {
        .name = _("Triage"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Gives priority to the\nPokémon's healing moves."),
#else
        .description = COMPOUND_STRING("Healing moves go first."),
#endif
        .aiRating = 7,
    },

    [ABILITY_GALVANIZE] =
    {
        .name = _("Galvanize"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Normal-type moves become Electric-\ntype and their power is boosted."),
#else
        .description = COMPOUND_STRING("Normal moves turn Electric."),
#endif
        .aiRating = 8,
    },

    [ABILITY_SURGE_SURFER] =
    {
        .name = _("Surge Surfer"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Doubles the Pokémon's Speed\nstat on Electric Terrain."),
#else
        .description = COMPOUND_STRING("Faster on electricity."),
#endif
        .aiRating = 4,
    },

    [ABILITY_SCHOOLING] =
    {
        .name = _("Schooling"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("With high HP, forms a mighty school.\nIt stops schooling with low HP."),
#else
        .description = COMPOUND_STRING("Forms a school when strong."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_DISGUISE] =
    {
        .name = _("Disguise"),
#if BW_SUMMARY_SCREEN == TRUE
     .description = COMPOUND_STRING("Once per battle, the shroud covering\nit can protect it from an attack."),
#else
        .description = COMPOUND_STRING("Decoy protects it once."),
#endif
        .aiRating = 8,
        .breakable = TRUE,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_BATTLE_BOND] =
    {
        .name = _("Battle Bond"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When it KOs a target, its Attack, Sp.\nAtk, and Speed stats are boosted."),
#else
        .description = COMPOUND_STRING("Changes form after a KO."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_POWER_CONSTRUCT] =
    {
        .name = _("Power Construct"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When its HP drops to half or less,\ncells change it into Complete Forme."),
#else
        .description = COMPOUND_STRING("Cells aid it when weakened."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_CORROSION] =
    {
        .name = _("Corrosion"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon can poison the target\neven if it's a Steel or Poison type."),
#else
        .description = COMPOUND_STRING("Poisons any type."),
#endif
        .aiRating = 5,
    },

    [ABILITY_COMATOSE] =
    {
        .name = _("Comatose"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon is always drowsing. It\ncan still attack without ever waking."),
#else
        .description = COMPOUND_STRING("Always drowsing."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_QUEENLY_MAJESTY] =
    {
        .name = _("Queenly Majesty"),
#if BW_SUMMARY_SCREEN == TRUE
      .description = COMPOUND_STRING("The Pokémon's majesty makes foes\nunable to hit with priority moves."),
#else
        .description = COMPOUND_STRING("Protects from priority."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_INNARDS_OUT] =
    {
        .name = _("Innards Out"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When knocked out, does damage equal\nto its remaining HP to the attacker."),
#else
        .description = COMPOUND_STRING("Hurts foe when defeated."),
#endif
        .aiRating = 5,
    },

    [ABILITY_DANCER] =
    {
        .name = _("Dancer"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When another Pokémon dances, it will\ncopy the move and use it next."),
#else
        .description = COMPOUND_STRING("Dances along with others."),
#endif
        .aiRating = 5,
    },

    [ABILITY_BATTERY] =
    {
        .name = _("Battery"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up ally Pokémon's\nspecial moves."),
#else
        .description = COMPOUND_STRING("Boosts ally's Sp. Atk."),
#endif
        .aiRating = 0,
    },

    [ABILITY_FLUFFY] =
    {
        .name = _("Fluffy"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Halves the damage taken from contact\nmoves, but doubles Fire-type damage."),
#else
        .description = COMPOUND_STRING("Tougher but flammable."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_DAZZLING] =
    {
        .name = _("Dazzling"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon dazzles its foes so\nthey can't hit with priority moves."),
#else
        .description = COMPOUND_STRING("Protects from priority."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SOUL_HEART] =
    {
        .name = _("Soul-Heart"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Sp. Atk stat\nevery time another Pokémon faints."),
#else
        .description = COMPOUND_STRING("KOs raise Sp. Atk."),
#endif
        .aiRating = 7,
    },

    [ABILITY_TANGLING_HAIR] =
    {
        .name = _("Tangling Hair"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Contact with the Pokémon lowers\nthe attacker's Speed stat."),
#else
        .description = COMPOUND_STRING("Lowers Speed on contact."),
#endif
        .aiRating = 5,
    },

    [ABILITY_RECEIVER] =
    {
        .name = _("Receiver"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon copies the Ability\nof a defeated ally."),
#else
        .description = COMPOUND_STRING("Copies ally's ability."),
#endif
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POWER_OF_ALCHEMY] =
    {
        .name = _("Power Of Alchemy"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon copies the Ability\nof a defeated ally."),
#else
        .description = COMPOUND_STRING("Copies ally's ability."),
#endif
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_BEAST_BOOST] =
    {
        .name = _("Beast Boost"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's most proficient\nstat every time it KOs a target."),
#else
        .description = COMPOUND_STRING("KOs boost best stat."),
#endif
        .aiRating = 7,
    },

    [ABILITY_RKS_SYSTEM] =
    {
        .name = _("RKS System"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Changes the Pokémon's type to match\nthe memory disc it holds."),
#else
        .description = COMPOUND_STRING("Memories change its type."),
#endif
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_ELECTRIC_SURGE] =
    {
        .name = _("Electric Surge"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Turns the field into Electric Terrain\nwhen the Pokémon enters battle."),
#else
        .description = COMPOUND_STRING("Field becomes Electric."),
#endif
        .aiRating = 8,
    },

    [ABILITY_PSYCHIC_SURGE] =
    {
        .name = _("Psychic Surge"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Turns the field into Psychic Terrain\nwhen the Pokémon enters battle."),
#else
        .description = COMPOUND_STRING("Field becomes weird."),
#endif
        .aiRating = 8,
    },

    [ABILITY_MISTY_SURGE] =
    {
        .name = _("Misty Surge"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Turns the field into Misty Terrain\nwhen the Pokémon enters battle."),
#else
        .description = COMPOUND_STRING("Field becomes misty."),
#endif
        .aiRating = 8,
    },

    [ABILITY_GRASSY_SURGE] =
    {
        .name = _("Grassy Surge"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Turns the field into Grassy Terrain\nwhen the Pokémon enters battle."),
#else
        .description = COMPOUND_STRING("Field becomes grassy."),
#endif
        .aiRating = 8,
    },

    [ABILITY_FULL_METAL_BODY] =
    {
        .name = _("Full Metal Body"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Prevents other Pokémon's moves or\nAbilities from lowering its stats."),
#else
        .description = COMPOUND_STRING("Prevents stat reduction."),
#endif
        .aiRating = 4,
    },

    [ABILITY_SHADOW_SHIELD] =
    {
        .name = _("Shadow Shield"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Reduces the amount of damage the\nPokémon takes while its HP is full."),
#else
        .description = COMPOUND_STRING("Halves damage at full HP."),
#endif
        .aiRating = 8,
    },

    [ABILITY_PRISM_ARMOR] =
    {
        .name = _("Prism Armor"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Reduces the power of supereffective\nattacks that hit the Pokémon."),
#else
        .description = COMPOUND_STRING("Weakens “supereffective”."),
#endif
        .aiRating = 6,
    },

    [ABILITY_NEUROFORCE] =
    {
        .name = _("Neuroforce"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up the Pokémon's\nsupereffective moves even further."),
#else
        .description = COMPOUND_STRING("Ups “supereffective”."),
#endif
        .aiRating = 6,
    },

    [ABILITY_INTREPID_SWORD] =
    {
        .name = _("Intrepid Sword"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Attack stat\nthe first time it enters battle.")
#else
        .description = COMPOUND_STRING("Ups Attack on entry."),
#endif
        .aiRating = 3,
    },

    [ABILITY_DAUNTLESS_SHIELD] =
    {
        .name = _("Dauntless Shield"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's Defense stat\nthe first time it enters battle."),
#else
        .description = COMPOUND_STRING("Ups Defense on entry."),
#endif
        .aiRating = 3,
    },

    [ABILITY_LIBERO] =
    {
        .name = _("Libero"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Changes its type to that of a queued\nmove once per battle entrance."),
#else
        .description = COMPOUND_STRING("Changes type to move's."),
#endif
    },

    [ABILITY_BALL_FETCH] =
    {
        .name = _("Ball Fetch"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("It will fetch a failed Poké Ball if\nit is not already holding an item."),
#else
        .description = COMPOUND_STRING("Fetches failed Poké Ball."),
#endif
        .aiRating = 0,
    },

    [ABILITY_COTTON_DOWN] =
    {
        .name = _("Cotton Down"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Cotton fluff is scattered when hit,\nlowering all others' Speed stats."),
#else
        .description = COMPOUND_STRING("Lower Speed of all when hit."),
#endif
        .aiRating = 3,
    },

    [ABILITY_PROPELLER_TAIL] =
    {
        .name = _("Propeller Tail"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Ignores redirection effects from\nthe foe's Abilities and moves."),
#else
        .description = COMPOUND_STRING("Ignores foe's redirection."),
#endif
        .aiRating = 2,
    },

    [ABILITY_MIRROR_ARMOR] =
    {
        .name = _("Mirror Armor"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Bounces back only the stat-lowering\neffects that the Pokémon receives."),
#else
        .description = COMPOUND_STRING("Reflect stat decreases."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_GULP_MISSILE] =
    {
        .name = _("Gulp Missile"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Finds prey with Surf or Dive. When\nhit, it spits the prey out at the foe."),
#else
        .description = COMPOUND_STRING("If hit, spits prey from sea."),
#endif
        .aiRating = 3,
        .cantBeCopied = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_STALWART] =
    {
        .name = _("Stalwart"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Ignores redirection effects from\nthe foe's Abilities and moves."),
#else
        .description = COMPOUND_STRING("Ignores foe's redirection."),
#endif
        .aiRating = 2,
    },

    [ABILITY_STEAM_ENGINE] =
    {
        .name = _("Steam Engine"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Drastically boosts Speed if it's hit\nby Fire- or Water-type moves."),
#else
        .description = COMPOUND_STRING("Fire or Water hits up Speed."),
#endif
        .aiRating = 3,
    },

    [ABILITY_PUNK_ROCK] =
    {
        .name = _("Punk Rock"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the power of and takes half\ndamage from sound-based moves."),
#else
        .description = COMPOUND_STRING("Ups and resists sound."),
#endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SAND_SPIT] =
    {
        .name = _("Sand Spit"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon creates a sandstorm\nif it's hit by an attack."),
#else
        .description = COMPOUND_STRING("Creates a sandstorm if hit."),
#endif
        .aiRating = 5,
    },

    [ABILITY_ICE_SCALES] =
    {
        .name = _("Ice Scales"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's ice scales halve\ndamage taken from special moves."),
#else
        .description = COMPOUND_STRING("Halves special damage."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_RIPEN] =
    {
        .name = _("Ripen"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Ripens Berries and doubles\ntheir effect."),
#else
        .description = COMPOUND_STRING("Doubles effect of Berries."),
#endif
        .aiRating = 4,
    },

    [ABILITY_ICE_FACE] =
    {
        .name = _("Ice Face"),
#if BW_SUMMARY_SCREEN == TRUE
      .description = COMPOUND_STRING("Its ice head acts as a substitute.\nThe ice is restored when it snows."),
#else
        .description = COMPOUND_STRING("Hail or Snow renew free hit."),
#endif
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .breakable = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_POWER_SPOT] =
    {
        .name = _("Power Spot"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Just being next to the\nPokémon powers up moves."),
#else
        .description = COMPOUND_STRING("Powers up ally moves."),
#endif
        .aiRating = 2,
    },

    [ABILITY_MIMICRY] =
    {
        .name = _("Mimicry"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Changes the Pokémon's type\ndepending on the terrain."),
#else
        .description = COMPOUND_STRING("Changes type on terrain."),
#endif
        .aiRating = 2,
    },

    [ABILITY_SCREEN_CLEANER] =
    {
        .name = _("Screen Cleaner"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When it enters battle, Light Screen\nReflect, and Aurora Veil all end."),
#else
        .description = COMPOUND_STRING("Removes walls of light."),
#endif
        .aiRating = 3,
    },

    [ABILITY_STEELY_SPIRIT] =
    {
        .name = _("Steely Spirit"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up the Steel-type moves\nof the Pokémon and its allies."),
#else
        .description = COMPOUND_STRING("Boosts ally's Steel moves."),
#endif
        .aiRating = 2,
    },

    [ABILITY_PERISH_BODY] =
    {
        .name = _("Perish Body"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("If the foe makes direct contact, it\nand the foe will faint in three turns."),
#else
        .description = COMPOUND_STRING("Foe faints in 3 turns if hit."),
#endif
        .aiRating = -1,
    },

    [ABILITY_WANDERING_SPIRIT] =
    {
        .name = _("Wandering Spirit"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon trades Abilities with a\nfoe that makes direct contact."),
#else
        .description = COMPOUND_STRING("Trade abilities on contact."),
#endif
        .aiRating = 2,
    },

    [ABILITY_GORILLA_TACTICS] =
    {
        .name = _("Gorilla Tactics"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts its Attack stat but only\nthe first move selected is usable."),
#else
        .description = COMPOUND_STRING("Ups Attack and locks move."),
#endif
        .aiRating = 4,
    },

    [ABILITY_NEUTRALIZING_GAS] =
    {
        .name = _("Neutralizing Gas"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("While the Pokémon is in battle, all\nother Abilities are nullified."),
#else
        .description = COMPOUND_STRING("All Abilities are nullified."),
#endif
        .aiRating = 5,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_PASTEL_VEIL] =
    {
        .name = _("Pastel Veil"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Prevents the Pokémon and its allies\nfrom being poisoned."),
#else
        .description = COMPOUND_STRING("Protects team from poison."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_HUNGER_SWITCH] =
    {
        .name = _("Hunger Switch"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("It oscillates between its Full\nBelly and Hangry Modes each turn."),
#else
        .description = COMPOUND_STRING("Changes form each turn."),
#endif
        .aiRating = 2,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUICK_DRAW] =
    {
        .name = _("Quick Draw"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Enables the Pokémon to\nmove first occasionally."),
#else
        .description = COMPOUND_STRING("Moves first occasionally."),
#endif
        .aiRating = 4,
    },

    [ABILITY_UNSEEN_FIST] =
    {
        .name = _("Unseen Fist"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's direct contact moves\nbypass the foe's protection effects."),
#else
        .description = COMPOUND_STRING("Contact evades protection."),
#endif
        .aiRating = 6,
    },

    [ABILITY_CURIOUS_MEDICINE] =
    {
        .name = _("Curious Medicine"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When the Pokémon enters battle, all\nallies' stat changes are removed."),
#else
        .description = COMPOUND_STRING("Remove ally's stat changes."),
#endif
        .aiRating = 3,
    },

    [ABILITY_TRANSISTOR] =
    {
        .name = _("Transistor"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up Electric-type moves."),
#else
        .description = COMPOUND_STRING("Ups Electric-type moves."),
#endif
        .aiRating = 6,
    },

    [ABILITY_DRAGONS_MAW] =
    {
        .name = _("Dragon's Maw"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up Dragon-type moves."),
#else
        .description = COMPOUND_STRING("Ups Dragon-type moves."),
#endif
        .aiRating = 6,
    },

    [ABILITY_CHILLING_NEIGH] =
    {
        .name = _("Chilling Neigh"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Utters a chilling neigh when KOing\na foe, boosting its Attack stat."),
#else
        .description = COMPOUND_STRING("KOs boost Attack stat."),
#endif
        .aiRating = 7,
    },

    [ABILITY_GRIM_NEIGH] =
    {
        .name = _("Grim Neigh"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Utters a terrifying neigh when KOing\na foe, boosting its Sp. Atk stat."),
#else
        .description = COMPOUND_STRING("KOs boost Sp. Atk stat."),
#endif
        .aiRating = 7,
    },

    [ABILITY_AS_ONE_ICE_RIDER] =
    {
        .name = _("As One"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("KOing a foe boosts its Attack stat.\nThe foe cannot eat Berries."),
#else
        .description = COMPOUND_STRING("Unnerve and Chilling Neigh."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_AS_ONE_SHADOW_RIDER] =
    {
        .name = _("As One"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("KOing a foe boosts its Sp. Atk stat.\nThe foe cannot eat Berries."),
#else
        .description = COMPOUND_STRING("Unnerve and Grim Neigh."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_LINGERING_AROMA] =
    {
        .name = _("Lingering Aroma"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The attacker's Ability changes to\nLingering Aroma when making contact."),
#else
        .description = COMPOUND_STRING("Spreads with contact."),
#endif
        .aiRating = 5,
    },

    [ABILITY_SEED_SOWER] =
    {
        .name = _("Seed Sower"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When hit by an attack, turns the\nfield into Grassy Terrain."),
#else
        .description = COMPOUND_STRING("Affects terrain when hit."),
#endif
        .aiRating = 5,
    },

    [ABILITY_THERMAL_EXCHANGE] =
    {
        .name = _("Thermal Exchange"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts its Attack when hit by Fire-\ntype moves. It cannot be burned."),
#else
        .description = COMPOUND_STRING("Fire hits up Attack."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANGER_SHELL] =
    {
        .name = _("Anger Shell"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("At half HP or less, its defenses\nfall while offenses and Speed rise."),
#else
        .description = COMPOUND_STRING("Gets angry at half HP."),
#endif
        .aiRating = 3,
    },

    [ABILITY_PURIFYING_SALT] =
    {
        .name = _("Purifying Salt"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("It's immune to status conditions.\nGhost-type moves do half damage."),
#else
        .description = COMPOUND_STRING("Protected by pure salts."),
#endif
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_WELL_BAKED_BODY] =
    {
        .name = _("Well-Baked Body"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Incoming Fire-type moves do no\ndamage and sharply boost Defense."),
#else
        .description = COMPOUND_STRING("Strengthened by Fire."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_WIND_RIDER] =
    {
        .name = _("Wind Rider"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Incoming wind moves do no damage\nand boost its Attack stat instead."),
#else
        .description = COMPOUND_STRING("Ups Attack if hit by wind."),
#endif
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_GUARD_DOG] =
    {
        .name = _("Guard Dog"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("If intimidated, its Attack stat is\nboosted. Cannot be forced to flee."),
#else
        .description = COMPOUND_STRING("Cannot be intimidated."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ROCKY_PAYLOAD] =
    {
        .name = _("Rocky Payload"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up Rock-type moves."),
#else
        .description = COMPOUND_STRING("Powers up Rock moves."),
#endif
        .aiRating = 6,
    },

    [ABILITY_WIND_POWER] =
    {
        .name = _("Wind Power"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Being hit by a wind move charges\nits next Electric-type move."),
#else
        .description = COMPOUND_STRING("Gets charged by wind."),
#endif
        .aiRating = 4,
    },

    [ABILITY_ZERO_TO_HERO] =
    {
        .name = _("Zero to Hero"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon transforms into its\nHero Form when it switches out."),
#else
        .description = COMPOUND_STRING("Changes form on switch out."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_COMMANDER] =
    {
        .name = _("Commander"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When it enters battle, it goes into an\nally Dondozo's mouth to command it."),
#else
        .description = COMPOUND_STRING("Commands from Dondozo."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_ELECTROMORPHOSIS] =
    {
        .name = _("Electromorphosis"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Taking damage charges it, boosting\nits next Electric-type move."),
#else
        .description = COMPOUND_STRING("Gets Charged when hit."),
#endif
        .aiRating = 5,
    },

    [ABILITY_PROTOSYNTHESIS] =
    {
        .name = _("Protosynthesis"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts the Pokémon's most proficient\nstat in sun or with a Booster Energy."),
#else
        .description = COMPOUND_STRING("Sun boosts best stat."),
#endif
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUARK_DRIVE] =
    {
        .name = _("Quark Drive"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Boosts its best stat on Electric\nTerrain or with a Booster Energy."),
#else
        .description = COMPOUND_STRING("Elec. field ups best stat."),
#endif
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_GOOD_AS_GOLD] =
    {
        .name = _("Good as Gold"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("A body of pure gold grants immunity\nto other Pokémon's status moves."),
#else
        .description = COMPOUND_STRING("Avoids status problems."),
#endif
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_VESSEL_OF_RUIN] =
    {
        .name = _("Vessel of Ruin"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Lowers all other Pokémon's Sp. Atk\nstats with its ruinous vessel."),
#else
        .description = COMPOUND_STRING("Lowers foes' sp. damage."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SWORD_OF_RUIN] =
    {
        .name = _("Sword of Ruin"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Lowers all other Pokémon's Defense\nstats with its ruinous sword."),
#else
        .description = COMPOUND_STRING("Lowers foes' Defense."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_TABLETS_OF_RUIN] =
    {
        .name = _("Tablets of Ruin"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Lowers all other Pokémon's Attack\nstats with its ruinous tablets."),
#else
        .description = COMPOUND_STRING("Lowers foes' damage."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_BEADS_OF_RUIN] =
    {
        .name = _("Beads of Ruin"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Lowers all other Pokémon's Sp. Def\nstats with its ruinous beads."),
#else
        .description = COMPOUND_STRING("Lowers foes' Sp. Defense."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ORICHALCUM_PULSE] =
    {
        .name = _("Orichalcum Pulse"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Summons harsh sunlight when entering\nbattle. Sunlight raises its Attack."),
#else
        .description = COMPOUND_STRING("Summons sunlight in battle."),
#endif
        .aiRating = 8,
    },

    [ABILITY_HADRON_ENGINE] =
    {
        .name = _("Hadron Engine"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Sets Electric Terrain when entering\nbattle. That terrain ups its Sp. Atk."),
#else
        .description = COMPOUND_STRING("Field becomes Electric."),
#endif
        .aiRating = 8,
    },

    [ABILITY_OPPORTUNIST] =
    {
        .name = _("Opportunist"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("If a foe's stat is boosted, it\nboosts the same stat for itself."),
#else
        .description = COMPOUND_STRING("Copies foe's stat change."),
#endif
        .aiRating = 5,
    },

    [ABILITY_CUD_CHEW] =
    {
        .name = _("Cud Chew"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("An eaten Berry will be regurgitated\nand eaten once more on the next turn."),
#else
        .description = COMPOUND_STRING("Eats a used berry again."),
#endif
        .aiRating = 4,
    },

    [ABILITY_SHARPNESS] =
    {
        .name = _("Sharpness"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Powers up slicing moves."),
#else
        .description = COMPOUND_STRING("Strengthens slicing moves."),
#endif
        .aiRating = 7,
    },

    [ABILITY_SUPREME_OVERLORD] =
    {
        .name = _("Supreme Overlord"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Each fainted party member slightly\nboosts the Pokémon's offensive stats."),
#else
        .description = COMPOUND_STRING("Inherits fallen's strength."),
#endif
        .aiRating = 6,
    },

    [ABILITY_COSTAR] =
    {
        .name = _("Costar"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When the Pokémon enters a battle,\nit copies an ally's stat changes."),
#else
        .description = COMPOUND_STRING("Copies ally's stat changes."),
#endif
        .aiRating = 5,
    },

    [ABILITY_TOXIC_DEBRIS] =
    {
        .name = _("Toxic Debris"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When hit by a physical move, lays\nToxic Spikes at the feet of the foe."),
#else
        .description = COMPOUND_STRING("Throws poison spikes if hit."),
#endif
        .aiRating = 4,
    },

    [ABILITY_ARMOR_TAIL] =
    {
        .name = _("Armor Tail"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's odd tail makes foes\nunable to hit with priority moves."),
#else
        .description = COMPOUND_STRING("Protects from priority."),
#endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_EARTH_EATER] =
    {
        .name = _("Earth Eater"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Restores HP if hit by Ground-type\nmoves instead of taking damage."),
#else
        .description = COMPOUND_STRING("Eats ground to heal HP."),
#endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MYCELIUM_MIGHT] =
    {
        .name = _("Mycelium Might"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's status moves are slow,\nbut can't be blocked by Abilities."),
#else
        .description = COMPOUND_STRING("Status moves never fail."),
#endif
        .aiRating = 2,
    },

    [ABILITY_HOSPITALITY] =
    {
        .name = _("Hospitality"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Restores a quarter of an ally's HP\nwhen the Pokémon enters battle."),
#else
        .description = COMPOUND_STRING("Restores ally's HP."),
#endif
        .aiRating = 5,
    },

    [ABILITY_MINDS_EYE] =
    {
        .name = _("Mind's Eye"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Its accuracy can't drop. Ignores\nevasiveness and Ghost immunities."),
#else
        .description = COMPOUND_STRING("Keen Eye and Scrappy."),
#endif
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_TEAL_MASK] =
    {
        .name = _("Embody Aspect"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Its Teal Mask shines,\nboosting the Pokémon's Speed stat."),
#else
        .description = COMPOUND_STRING("Raises Speed."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_HEARTHFLAME_MASK] =
    {
        .name = _("Embody Aspect"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Its Hearthflame Mask shines,\nboosting the Pokémon's Attack stat."),
#else
        .description = COMPOUND_STRING("Raises Attack."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_WELLSPRING_MASK] =
    {
        .name = _("Embody Aspect"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Its Wellspring Mask shines,\nboosting the Pokémon's Sp. Def stat."),
#else
        .description = COMPOUND_STRING("Raises Sp. Def."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_CORNERSTONE_MASK] =
    {
        .name = _("Embody Aspect"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Its Cornerstone Mask shines,\nboosting the Pokémon's Defense stat."),
#else
        .description = COMPOUND_STRING("Raises Defense."),
#endif
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TOXIC_CHAIN] =
    {
        .name = _("Toxic Chain"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("The Pokémon's toxic chain may badly\npoison foes hit by its moves."),
#else
        .description = COMPOUND_STRING("Moves can poison."),
#endif
        .aiRating = 8,
    },

    [ABILITY_SUPERSWEET_SYRUP] =
    {
        .name = _("Supersweet Syrup"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Once per battle, the Pokémon emits\na scent that lowers evasiveness."),
#else
        .description = COMPOUND_STRING("Lowers the foe's Evasion."),
#endif
        .aiRating = 5,
    },

    [ABILITY_TERA_SHIFT] =
    {
        .name = _("Tera Shift"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When entering battle, it absorbs\nenergy to assume its Terastal Form."),
#else
        .description = COMPOUND_STRING("Terastallizes upon entry."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TERA_SHELL] =
    {
        .name = _("Tera Shell"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When at full HP, all damage-dealing\nmoves it's hit by are resisted."),
#else
        .description = COMPOUND_STRING("Resists all at full HP."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_TERAFORM_ZERO] =
    {
        .name = _("Teraform Zero"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("When assuming its Stellar Form, it\neliminates weather and terrain."),
#else
        .description = COMPOUND_STRING("Zeroes weather and terrain."),
#endif
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POISON_PUPPETEER] =
    {
        .name = _("Poison Puppeteer"),
#if BW_SUMMARY_SCREEN == TRUE
        .description = COMPOUND_STRING("Pokémon poisoned by the Pokémon's\nmoves will also become confused."),
#else
        .description = COMPOUND_STRING("Confuses poisoned foes."),
#endif
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },
};
