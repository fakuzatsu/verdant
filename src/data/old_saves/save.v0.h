#include "global.h"
#include "save.h"

/*
 * Prior to release, change all constants in favor of literal numbers so that the struct does
 * not change if we decide to, say, change how many items are in the bag or PC. The idea is to
 * preserve this struct exactly as it was previously, so that we can upgrade it (see below).
**/

struct SaveBlock3_v0
{
    u16 saveVersion;
    u32 PID;
    u32 grottoSeed;
    u8 dexNavSearchLevels[ROUND_BITS_TO_BYTES(1524)]; // NUM_SPECIES
    u8 dexNavChain;
    u8 itemFlags[106]; // ITEM_FLAGS_COUNT
};

struct SaveBlock2_v0
{
    u8 playerName[8]; // PLAYER_NAME_LENGTH + 1
    u8 playerGender;
    u8 specialSaveWarpFlags;
    u8 playerTrainerId[4]; // TRAINER_ID_LENGTH
    u16 playTimeHours;
    u8 playTimeMinutes;
    u8 playTimeSeconds;
    u8 playTimeVBlanks;
    u8 optionsButtonMode;
    u16 optionsTextSpeed:3;
    u16 optionsWindowFrameType:5;
    u16 optionsSound:1;
    u16 optionsBattleStyle:1;
    u16 optionsBattleSceneOff:1;
    u16 regionMapZoom:1;
    u16 optionsUnitSystem:1;
    u16 optionsDifficulty:1;
    u16 optionsLevelCap:2;
    u16 optionsSpeedModifer:3;
    u16 optionsPadding:1;
    u16 optionsExpBarSpeed:4;
    u16 optionsDisableMatchCall:1;
    u16 optionsCurrentFont:1;
    u16 optionsWildRandomiser:1;
    u16 optionsTrainerRandomiser:1;
    u16 optionsAbilityRandomiser:1;
    u16 optionsVGCDraft:2;
    u16 optionsDamageNumsOff:1;
    u32 randomiserSeed;
    struct Pokedex pokedex;
    u8 filler_90[0x8];
    struct Time localTimeOffset;
    struct Time lastBerryTreeUpdate;
    u32 gcnLinkFlags;
    u32 encryptionKey;
    struct PlayersApprentice playerApprentice;
    struct Apprentice apprentices[4]; // APPRENTICE_COUNT
    struct BerryCrush berryCrush;
    struct PokemonJumpRecords pokeJump;
    struct BerryPickingResults berryPick;
    struct RankingHall1P hallRecords1P[9][2][3]; // HALL_FACILITIES_COUNT // FRONTIER_LVL_MODE_COUNT // HALL_RECORDS_COUNT
    struct RankingHall2P hallRecords2P[2][3]; // FRONTIER_LVL_MODE_COUNT // HALL_RECORDS_COUNT
    u16 contestLinkResults[5][4]; // CONTEST_CATEGORIES_COUNT // CONTESTANT_COUNT
    struct BattleFrontier frontier;
};

struct SaveBlock1_v0
{
    struct Coords16 pos;
    struct WarpData location;
    struct WarpData continueGameWarp;
    struct WarpData dynamicWarp;
    struct WarpData lastHealLocation;
    struct WarpData escapeWarp;
    u16 savedMusic;
    u8 weather;
    u8 weatherCycleStage;
    u8 flashLevel;
    u16 mapLayoutId;
    u16 mapView[0x100];
    u8 playerPartyCount;
    struct Pokemon playerParty[6]; // PARTY_SIZE
    u32 money;
    u16 coins;
    u16 registeredItemCompat;
    struct ItemSlot pcItems[50]; // PC_ITEMS_COUNT
    struct ItemSlot bagPocket_Items[100]; // BAG_ITEMS_COUNT
    struct ItemSlot bagPocket_KeyItems[30]; // BAG_KEYITEMS_COUNT
    struct ItemSlot bagPocket_TeraShards[19]; // BAG_TERASHARDS_COUNT
    struct ItemSlot bagPocket_PokeBalls[27]; // BAG_POKEBALLS_COUNT
    struct ItemSlot bagPocket_TMHM[120]; // BAG_TMHM_COUNT
    struct ItemSlot bagPocket_Berries[48]; // BAG_BERRIES_COUNT
    struct Pokeblock pokeblocks[40]; // POKEBLOCKS_COUNT
    u16 berryBlenderRecords[3];
    u8 unused_9C2[6];
    u16 trainerRematchStepCounter;
    u8 trainerRematches[92]; // MAX_REMATCH_ENTRIES
    u16 registeredItems[4]; // MAX_REGISTERED_ITEMS
    struct ObjectEvent objectEvents[16];
    struct ObjectEventTemplate objectEventTemplates[64]; // OBJECT_EVENT_TEMPLATES_COUNT
    u8 flags[300]; // NUM_FLAGS
    u16 vars[256]; // NUM_VARS
    u32 gameStats[64]; // NUM_GAME_STATS
    struct BerryTree berryTrees[128]; // BERRY_TREES_COUNT
    struct SecretBase secretBases[10]; // SECRET_BASES_COUNT
    u8 playerRoomDecorations[12]; // DECOR_MAX_PLAYERS_HOUSE
    u8 playerRoomDecorationPositions[12]; // DECOR_MAX_PLAYERS_HOUSE
    u8 decorationDesks[10];
    u8 decorationChairs[10];
    u8 decorationPlants[10];
    u8 decorationOrnaments[30];
    u8 decorationMats[30];
    u8 decorationPosters[10];
    u8 decorationDolls[40];
    u8 decorationCushions[10];
    TVShow tvShows[25]; // TV_SHOWS_COUNT
    PokeNews pokeNews[16]; // POKE_NEWS_COUNT
    u16 outbreakPokemonSpecies;
    u8 outbreakLocationMapNum;
    u8 outbreakLocationMapGroup;
    u8 outbreakPokemonLevel;
    u8 outbreakUnused1;
    u16 outbreakUnused2;
    u16 outbreakPokemonMoves[4]; // MAX_MON_MOVES
    u8 outbreakUnused3;
    u8 outbreakPokemonProbability;
    u16 outbreakDaysLeft;
    struct GabbyAndTyData gabbyAndTyData;
    u16 easyChatProfile[6]; // EASY_CHAT_BATTLE_WORDS_COUNT
    u16 easyChatBattleStart[6]; // EASY_CHAT_BATTLE_WORDS_COUNT
    u16 easyChatBattleWon[6]; // EASY_CHAT_BATTLE_WORDS_COUNT
    u16 easyChatBattleLost[6]; // EASY_CHAT_BATTLE_WORDS_COUNT
    struct Mail mail[16]; // MAIL_COUNT
    u8 unlockedTrendySayings[5]; // NUM_TRENDY_SAYING_BYTES
    OldMan oldMan;
    struct DewfordTrend dewfordTrends[5]; // SAVED_TRENDS_COUNT
    struct ContestWinner contestWinners[13]; // NUM_CONTEST_WINNERS
    struct DayCare daycare;
    struct LinkBattleRecords linkBattleRecords;
    u8 giftRibbons[11]; // GIFT_RIBBONS_COUNT
    struct ExternalEventData externalEventData;
    struct ExternalEventFlags externalEventFlags;
    struct Roamer roamer[1]; // ROAMER_COUNT
    u8 dexSeen[129]; // NUM_DEX_FLAG_BYTES
    u8 dexCaught[129]; // NUM_DEX_FLAG_BYTES
    u32 trainerHillTimes[4]; // NUM_TRAINER_HILL_MODES
    struct RecordMixingGift recordMixingGift;
    LilycoveLady lilycoveLady;
    struct TrainerNameRecord trainerNameRecords[20];
    u8 registeredTexts[10][21]; // UNION_ROOM_KB_ROW_COUNT // raw
    struct TrainerHillSave trainerHill;
    struct WaldaPhrase waldaPhrase;
};

/**
 * The following is the function which copies all the data from the old save file to the new one.
**/

 bool8 UpdateSave_v0_v1(const struct SaveSectorLocation *locations)
{
    gSaveBlock3Ptr->saveVersion = SAVE_VERSION_1; // Updates save version to the next.

    /** 
     * The passed locations structure has been filled with pointers to the old save data on the heap already.
     * We just need to assign the old save blocks to the pointers on the heap accordingly. (Again I'm using
     * literal numbers here as I'm being overly cautious: what if I change the SECTOR_IDs for the save blocks in 
     * later save version?)
    **/

    const struct SaveBlock2_v0* sOldSaveBlock2Ptr = (struct SaveBlock2_v0*)(locations[0].data); // SECTOR_ID_SAVEBLOCK2
    const struct SaveBlock1_v0* sOldSaveBlock1Ptr = (struct SaveBlock1_v0*)(locations[1].data); // SECTOR_ID_SAVEBLOCK1_START
    const struct PokemonStorage* sOldPokemonStoragePtr = (struct PokemonStorage*)(locations[5].data); // SECTOR_ID_PKMN_STORAGE_START
    u32 i;

#define COPY_FIELD(field) gSaveBlock2Ptr->field = sOldSaveBlock2Ptr->field
#define COPY_BLOCK(field) CpuCopy16(&sOldSaveBlock2Ptr->field, &gSaveBlock2Ptr->field, sizeof(gSaveBlock2Ptr->field))
#define COPY_ARRAY(field) for(i = 0; i < min(ARRAY_COUNT(gSaveBlock2Ptr->field), ARRAY_COUNT(sOldSaveBlock2Ptr->field)); i++) gSaveBlock2Ptr->field[i] = sOldSaveBlock2Ptr->field[i];

    // Player Stats
    COPY_ARRAY(playerName);
    COPY_FIELD(playerGender);
    COPY_FIELD(specialSaveWarpFlags);
    COPY_ARRAY(playerTrainerId);
    COPY_FIELD(playTimeHours);
    COPY_FIELD(playTimeMinutes);
    COPY_FIELD(playTimeSeconds);
    COPY_FIELD(playTimeVBlanks);

    // Options
    COPY_FIELD(optionsButtonMode);
    COPY_FIELD(optionsTextSpeed);
    COPY_FIELD(optionsWindowFrameType);
    COPY_FIELD(optionsSound);
    COPY_FIELD(optionsBattleStyle);
    COPY_FIELD(optionsBattleSceneOff);
    COPY_FIELD(regionMapZoom);
    COPY_FIELD(optionsUnitSystem);
    COPY_FIELD(optionsDifficulty);
    COPY_FIELD(optionsLevelCap);
    COPY_FIELD(optionsSpeedModifer);
    COPY_FIELD(optionsPadding);
    COPY_FIELD(optionsExpBarSpeed);
    COPY_FIELD(optionsDisableMatchCall);
    COPY_FIELD(optionsCurrentFont);
    COPY_FIELD(optionsWildRandomiser);
    COPY_FIELD(optionsTrainerRandomiser);
    COPY_FIELD(optionsAbilityRandomiser);
    COPY_FIELD(optionsVGCDraft);
    COPY_FIELD(optionsDamageNumsOff);
    COPY_FIELD(randomiserSeed);

    // Misc
    COPY_FIELD(pokedex);
    COPY_FIELD(localTimeOffset);
    COPY_FIELD(lastBerryTreeUpdate);
    COPY_FIELD(gcnLinkFlags);
    COPY_FIELD(encryptionKey);

    // Minigames
    COPY_FIELD(playerApprentice);
    COPY_BLOCK(apprentices);
    COPY_FIELD(berryCrush);
    COPY_FIELD(pokeJump);
    COPY_FIELD(berryPick);
    COPY_BLOCK(hallRecords1P);
    COPY_BLOCK(hallRecords2P);
    COPY_BLOCK(contestLinkResults);
    COPY_FIELD(frontier);

#undef COPY_FIELD
#undef COPY_BLOCK
#undef COPY_ARRAY

#define COPY_FIELD(field) gSaveBlock1Ptr->field = sOldSaveBlock1Ptr->field
#define COPY_BLOCK(field) CpuCopy16(&sOldSaveBlock1Ptr->field, &gSaveBlock1Ptr->field, sizeof(gSaveBlock1Ptr->field))
#define COPY_ARRAY(field) for(i = 0; i < min(ARRAY_COUNT(gSaveBlock1Ptr->field), ARRAY_COUNT(sOldSaveBlock1Ptr->field)); i++) gSaveBlock1Ptr->field[i] = sOldSaveBlock1Ptr->field[i];

    // Location
    COPY_FIELD(pos);
    COPY_FIELD(location);
    COPY_FIELD(continueGameWarp);
    COPY_FIELD(dynamicWarp);
    COPY_FIELD(lastHealLocation);
    COPY_FIELD(escapeWarp);

    /** We don't need to copy things related to the current map, see below. */
    // COPY_FIELD(savedMusic);
    // COPY_FIELD(weather);
    // COPY_FIELD(weatherCycleStage);
    // COPY_FIELD(flashLevel);
    // COPY_FIELD(mapLayoutId);
    // COPY_BLOCK(mapView);
    // COPY_BLOCK(objectEvents);
    // COPY_BLOCK(objectEventTemplates);

    // Party
    COPY_FIELD(playerPartyCount);
    COPY_ARRAY(playerParty);

    // Bag
    COPY_FIELD(money);
    COPY_FIELD(coins);
    COPY_FIELD(registeredItemCompat);
    COPY_ARRAY(pcItems);
    COPY_ARRAY(bagPocket_Items);
    COPY_ARRAY(bagPocket_KeyItems);
    COPY_ARRAY(bagPocket_TeraShards);
    COPY_ARRAY(bagPocket_PokeBalls);
    COPY_ARRAY(bagPocket_TMHM);
    COPY_ARRAY(bagPocket_Berries);
    COPY_BLOCK(pokeblocks);

    // Trainer
    COPY_BLOCK(berryBlenderRecords);
    COPY_FIELD(trainerRematchStepCounter);
    COPY_BLOCK(trainerRematches);

    // Player Progression
    COPY_BLOCK(registeredItems);
    COPY_BLOCK(flags);
    COPY_BLOCK(vars);
    COPY_BLOCK(gameStats);
    COPY_BLOCK(berryTrees);

    // Secret Base
    COPY_BLOCK(secretBases);
    COPY_BLOCK(playerRoomDecorations);
    COPY_BLOCK(playerRoomDecorationPositions);
    COPY_BLOCK(decorationDesks);
    COPY_BLOCK(decorationChairs);
    COPY_BLOCK(decorationPlants);
    COPY_BLOCK(decorationOrnaments);
    COPY_BLOCK(decorationMats);
    COPY_BLOCK(decorationPosters);
    COPY_BLOCK(decorationDolls);
    COPY_BLOCK(decorationCushions);

    // TV
    COPY_BLOCK(tvShows);
    COPY_BLOCK(pokeNews);
    COPY_FIELD(outbreakPokemonSpecies);
    COPY_FIELD(outbreakLocationMapNum);
    COPY_FIELD(outbreakLocationMapGroup);
    COPY_FIELD(outbreakPokemonLevel);
    COPY_FIELD(outbreakUnused1);
    COPY_FIELD(outbreakUnused2);
    COPY_BLOCK(outbreakPokemonMoves);
    COPY_FIELD(outbreakUnused3);
    COPY_FIELD(outbreakPokemonProbability);
    COPY_FIELD(outbreakDaysLeft);
    COPY_FIELD(gabbyAndTyData);

    // Easy Chat
    COPY_BLOCK(easyChatProfile);
    COPY_BLOCK(easyChatBattleStart);
    COPY_BLOCK(easyChatBattleWon);
    COPY_BLOCK(easyChatBattleLost);

    // World
    COPY_BLOCK(mail);
    COPY_BLOCK(unlockedTrendySayings);
    COPY_FIELD(oldMan);
    COPY_BLOCK(dewfordTrends);
    COPY_BLOCK(contestWinners);
    COPY_FIELD(daycare);
    COPY_FIELD(linkBattleRecords);
    COPY_BLOCK(giftRibbons);
    COPY_FIELD(externalEventData);
    COPY_FIELD(externalEventFlags);
    COPY_ARRAY(roamer);

    // Pokedex
    COPY_BLOCK(dexSeen);
    COPY_BLOCK(dexCaught);

    // Misc
    COPY_BLOCK(trainerHillTimes);
    COPY_FIELD(recordMixingGift);
    COPY_FIELD(lilycoveLady);
    COPY_BLOCK(trainerNameRecords);
    COPY_BLOCK(registeredTexts);
    COPY_FIELD(trainerHill);
    COPY_FIELD(waldaPhrase);

#undef COPY_FIELD
#undef COPY_BLOCK
#undef COPY_ARRAY

    /**
     * The pokemon structure hasn't changed at all this version, so
     * we can just assign across the old box storage to the new.  */ 
    *gPokemonStoragePtr = *sOldPokemonStoragePtr;

    /**
     * The most common kind of change that might happen between major versions are 
     * map changes. The save file usually saves the area around the player and 
     * event objects currently on the map, so that when the player resumes play, 
     * everything will be exactly where it was when they saved. But if the map has
     * changed at all, this means there could be incongruity between the saved
     * version of the map and the new version in the updated ROM. So to make it so
     * that the map reloads properly, we use the "Continue Game Warp" usually used
     * when the player clears the game to place the player at a known position and
     * with a newly loaded map and event objects. Here, we're using the last location
     * that the player healed, so the player will appear in the same spot they would
     * as if they blacked out. */
    SetContinueGameWarpStatus();
    gSaveBlock1Ptr->continueGameWarp = gSaveBlock1Ptr->lastHealLocation;

    return SAVE_UFR_SUCCESS;
}