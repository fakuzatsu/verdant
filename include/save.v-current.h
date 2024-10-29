#ifndef GUARD_SAVE_V_CURRENT_H
#define GUARD_SAVE_V_CURRENT_H

/*
 * For the purpose of save versioning, the saveblocks have been moved to this header.
 * When a new release is made, this file should be copied to a new file in src/data/old_saves.
 * In addition, the preproc flags should be expanded out as well as the defines.
**/

struct SaveBlock3
{
    u8 saveVersion;
    // u8 padding[3];
    u32 PID;
    u32 grottoSeed;
#if OW_USE_FAKE_RTC
    struct Time fakeRTC;
#endif
#if USE_DEXNAV_SEARCH_LEVELS == TRUE
    u8 dexNavSearchLevels[ROUND_BITS_TO_BYTES(NUM_SPECIES)];
#endif
    u8 dexNavChain;
#if OW_SHOW_ITEM_DESCRIPTIONS == OW_ITEM_DESCRIPTIONS_FIRST_TIME
    u8 itemFlags[ITEM_FLAGS_COUNT];
#endif
}; /* max size 1624 bytes */

struct SaveBlock2
{
    u8 playerName[PLAYER_NAME_LENGTH + 1];
    u8 playerGender; // MALE, FEMALE
    u8 specialSaveWarpFlags;
    u8 playerTrainerId[TRAINER_ID_LENGTH];
    u16 playTimeHours;
    u8 playTimeMinutes;
    u8 playTimeSeconds;
    u8 playTimeVBlanks;
    u8 optionsButtonMode; // OPTIONS_BUTTON_MODE_[NORMAL/LR/L_EQUALS_A]
    u16 optionsTextSpeed:3; // OPTIONS_TEXT_SPEED_[SLOW/MID/FAST]
    u16 optionsWindowFrameType:5; // Specifies one of the 20 decorative borders for text boxes
    u16 optionsSound:1; // OPTIONS_SOUND_[MONO/STEREO]
    u16 optionsBattleStyle:1; // OPTIONS_BATTLE_STYLE_[SHIFT/SET]
    u16 optionsBattleSceneOff:1; // whether battle animations are disabled
    u16 regionMapZoom:1; // whether the map is zoomed in
    u16 optionsUnitSystem:1; //tx_optionsPlus
    u16 optionsDifficulty:1; // custom addition to tx_optionsPlus
    u16 optionsLevelCap:2; // custom addition to tx_optionsPlus
    u16 optionsSpeedModifer:3; //tx_optionsPlus
    u16 optionsPadding:1; // padding
    u16 optionsExpBarSpeed:4; //tx_optionsPlus
    u16 optionsDisableMatchCall:1; //tx_optionsPlus
    u16 optionsCurrentFont:1; //tx_optionsPlus
    u16 optionsWildRandomiser:1; // custom addition to tx_optionsPlus
    u16 optionsTrainerRandomiser:1; // custom addition to tx_optionsPlus
    u16 optionsAbilityRandomiser:1; // custom addition to tx_optionsPlus
    u16 optionsVGCDraft:2; // custom addition to tx_optionsPlus
    u16 optionsDamageNumsOff:1; // custom addition to tx_optionsPlus
    u32 randomiserSeed;
    struct Pokedex pokedex;
    u8 filler_90[0x8];
    struct Time localTimeOffset;
    struct Time lastBerryTreeUpdate;
    u32 gcnLinkFlags; // Read by Pokémon Colosseum/XD
    u32 encryptionKey;
    struct PlayersApprentice playerApprentice;
    struct Apprentice apprentices[APPRENTICE_COUNT];
    struct BerryCrush berryCrush;
#if FREE_POKEMON_JUMP == FALSE
    struct PokemonJumpRecords pokeJump;
#endif //FREE_POKEMON_JUMP
    struct BerryPickingResults berryPick;
#if FREE_RECORD_MIXING_HALL_RECORDS == FALSE
    struct RankingHall1P hallRecords1P[HALL_FACILITIES_COUNT][FRONTIER_LVL_MODE_COUNT][HALL_RECORDS_COUNT]; // From record mixing.
    struct RankingHall2P hallRecords2P[FRONTIER_LVL_MODE_COUNT][HALL_RECORDS_COUNT]; // From record mixing.
#endif //FREE_RECORD_MIXING_HALL_RECORDS
    u16 contestLinkResults[CONTEST_CATEGORIES_COUNT][CONTESTANT_COUNT];
    struct BattleFrontier frontier;
};

struct SaveBlock1
{
    struct Coords16 pos;
    struct WarpData location;
    struct WarpData continueGameWarp;
    struct WarpData dynamicWarp;
    struct WarpData lastHealLocation; // used by white-out and teleport
    struct WarpData escapeWarp; // used by Dig and Escape Rope
    u16 savedMusic;
    u8 weather;
    u8 weatherCycleStage;
    u8 flashLevel;
    //u8 padding1;
    u16 mapLayoutId;
    u16 mapView[0x100];
    u8 playerPartyCount;
    //u8 padding2[3];
    struct Pokemon playerParty[PARTY_SIZE];
    u32 money;
    u16 coins;
    u16 registeredItemCompat; // used for vanilla registered item
    struct ItemSlot pcItems[PC_ITEMS_COUNT];
    struct ItemSlot bagPocket_Items[BAG_ITEMS_COUNT];
    struct ItemSlot bagPocket_KeyItems[BAG_KEYITEMS_COUNT];
    struct ItemSlot bagPocket_TeraShards[BAG_TERASHARDS_COUNT];
    struct ItemSlot bagPocket_PokeBalls[BAG_POKEBALLS_COUNT];
    struct ItemSlot bagPocket_TMHM[BAG_TMHM_COUNT];
    struct ItemSlot bagPocket_Berries[BAG_BERRIES_COUNT];
    struct Pokeblock pokeblocks[POKEBLOCKS_COUNT];
#if FREE_EXTRA_SEEN_FLAGS_SAVEBLOCK1 == FALSE
    u8 filler1[0x34]; // Previously Dex Flags, feel free to remove.
#endif //FREE_EXTRA_SEEN_FLAGS_SAVEBLOCK1
    u16 berryBlenderRecords[3];
    u8 unused_9C2[6];
#if FREE_MATCH_CALL == FALSE
    u16 trainerRematchStepCounter;
    // MAX_REMATCH_ENTRIES decreased from vanilla's 100 to 92
    // This is to accomodate 4 non-vanilla registeredItems
    u8 trainerRematches[MAX_REMATCH_ENTRIES];
#endif //FREE_MATCH_CALL
    u16 registeredItems[MAX_REGISTERED_ITEMS];
    //u8 padding3[2];
    struct ObjectEvent objectEvents[OBJECT_EVENTS_COUNT];
    struct ObjectEventTemplate objectEventTemplates[OBJECT_EVENT_TEMPLATES_COUNT];
    u8 flags[NUM_FLAG_BYTES];
    u16 vars[VARS_COUNT];
    u32 gameStats[NUM_GAME_STATS];
    struct BerryTree berryTrees[BERRY_TREES_COUNT];
    struct SecretBase secretBases[SECRET_BASES_COUNT];
    u8 playerRoomDecorations[DECOR_MAX_PLAYERS_HOUSE];
    u8 playerRoomDecorationPositions[DECOR_MAX_PLAYERS_HOUSE];
    u8 decorationDesks[10];
    u8 decorationChairs[10];
    u8 decorationPlants[10];
    u8 decorationOrnaments[30];
    u8 decorationMats[30];
    u8 decorationPosters[10];
    u8 decorationDolls[40];
    u8 decorationCushions[10];
    TVShow tvShows[TV_SHOWS_COUNT];
    //u8 padding4[2];
    PokeNews pokeNews[POKE_NEWS_COUNT];
    u16 outbreakPokemonSpecies;
    u8 outbreakLocationMapNum;
    u8 outbreakLocationMapGroup;
    u8 outbreakPokemonLevel;
    u8 outbreakUnused1;
    u16 outbreakUnused2;
    u16 outbreakPokemonMoves[MAX_MON_MOVES];
    u8 outbreakUnused3;
    u8 outbreakPokemonProbability;
    u16 outbreakDaysLeft;
    struct GabbyAndTyData gabbyAndTyData;
    u16 easyChatProfile[EASY_CHAT_BATTLE_WORDS_COUNT];
    u16 easyChatBattleStart[EASY_CHAT_BATTLE_WORDS_COUNT];
    u16 easyChatBattleWon[EASY_CHAT_BATTLE_WORDS_COUNT];
    u16 easyChatBattleLost[EASY_CHAT_BATTLE_WORDS_COUNT];
    struct Mail mail[MAIL_COUNT];
    u8 unlockedTrendySayings[NUM_TRENDY_SAYING_BYTES]; // Bitfield for unlockable Easy Chat words in EC_GROUP_TRENDY_SAYING
    //u8 padding5[3];
    OldMan oldMan;
    struct DewfordTrend dewfordTrends[SAVED_TRENDS_COUNT];
    struct ContestWinner contestWinners[NUM_CONTEST_WINNERS]; // see CONTEST_WINNER_*
    struct DayCare daycare;
#if FREE_LINK_BATTLE_RECORDS == FALSE
    struct LinkBattleRecords linkBattleRecords;
#endif //FREE_LINK_BATTLE_RECORDS
    u8 giftRibbons[GIFT_RIBBONS_COUNT];
    struct ExternalEventData externalEventData;
    struct ExternalEventFlags externalEventFlags;
    struct Roamer roamer[ROAMER_COUNT];
#if FREE_ENIGMA_BERRY == FALSE
    struct EnigmaBerry enigmaBerry;
#endif //FREE_ENIGMA_BERRY
#if FREE_MYSTERY_GIFT == FALSE
    struct MysteryGiftSave mysteryGift;
#endif //FREE_MYSTERY_GIFT
    u8 dexSeen[NUM_DEX_FLAG_BYTES];
    u8 dexCaught[NUM_DEX_FLAG_BYTES];
#if FREE_TRAINER_HILL == FALSE
    u32 trainerHillTimes[NUM_TRAINER_HILL_MODES];
#endif //FREE_TRAINER_HILL
#if FREE_MYSTERY_EVENT_BUFFERS == FALSE
    struct RamScript ramScript;
#endif //FREE_MYSTERY_EVENT_BUFFERS
    struct RecordMixingGift recordMixingGift;
    LilycoveLady lilycoveLady;
    struct TrainerNameRecord trainerNameRecords[20];
#if FREE_UNION_ROOM_CHAT == FALSE
    u8 registeredTexts[UNION_ROOM_KB_ROW_COUNT][21];
#endif //FREE_UNION_ROOM_CHAT
#if FREE_TRAINER_HILL == FALSE
    struct TrainerHillSave trainerHill;
#endif //FREE_TRAINER_HILL
    struct WaldaPhrase waldaPhrase;
};

#endif // GUARD_SAVE_V_CURRENT_H