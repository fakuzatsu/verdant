/*
 * Pokemon Emerald's original save layout.
 *
 * Keep every array bound and layout assertion literal. This file describes a
 * released on-cartridge format, so it must not move when current project
 * constants or feature flags change.
 */

struct Pokedex_v0
{
    u8 order;
    u8 mode;
    u8 nationalMagic;
    u8 unknown2;
    u32 unownPersonality;
    u32 spindaPersonality;
    u32 unknown3;
    u8 owned[52];
    u8 seen[52];
};

struct BerryCrush_v0
{
    u16 pressingSpeeds[4];
    u32 berryPowderAmount;
    u32 unk;
};

struct Time_v0
{
    s16 days;
    s8 hours;
    s8 minutes;
    s8 seconds;
    u8 padding[3];
};

struct ContestWinner_v0
{
    u32 personality;
    u32 trainerId;
    u16 species;
    u8 contestCategory;
    u8 monName[11];
    u8 trainerName[8];
    u8 contestRank;
};

struct DayCare_v0
{
    struct DaycareMon mons[2];
    u32 offspringPersonality;
    u8 stepCounter;
    u8 padding[3];
};

struct SaveBlock2_v0
{
    u8 playerName[8];
    u8 playerGender;
    u8 specialSaveWarpFlags;
    u8 playerTrainerId[4];
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
    u16 optionsPadding:4;
    struct Pokedex_v0 pokedex;
    u8 filler_90[8];
    struct Time_v0 localTimeOffset;
    struct Time_v0 lastBerryTreeUpdate;
    u32 gcnLinkFlags;
    u32 encryptionKey;
    struct PlayersApprentice playerApprentice;
    struct Apprentice apprentices[4];
    struct BerryCrush_v0 berryCrush;
    struct PokemonJumpRecords pokeJump;
    struct BerryPickingResults berryPick;
    struct RankingHall1P hallRecords1P[9][2][3];
    struct RankingHall2P hallRecords2P[2][3];
    u16 contestLinkResults[5][4];
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
    struct Pokemon playerParty[6];
    u32 money;
    u16 coins;
    u16 registeredItem;
    struct ItemSlot pcItems[50];
    struct ItemSlot bagPocket_Items[30];
    struct ItemSlot bagPocket_KeyItems[30];
    struct ItemSlot bagPocket_PokeBalls[16];
    struct ItemSlot bagPocket_TMHM[64];
    struct ItemSlot bagPocket_Berries[46];
    struct Pokeblock pokeblocks[40];
    u8 seen1[52];
    u16 berryBlenderRecords[3];
    u8 unused_9C2[6];
    u16 trainerRematchStepCounter;
    u8 trainerRematches[100];
    struct ObjectEvent objectEvents[16];
    struct ObjectEventTemplate objectEventTemplates[64];
    u8 flags[300];
    u16 vars[256];
    u32 gameStats[64];
    struct BerryTree berryTrees[128];
    struct SecretBase secretBases[20];
    u8 playerRoomDecorations[12];
    u8 playerRoomDecorationPositions[12];
    u8 decorationDesks[10];
    u8 decorationChairs[10];
    u8 decorationPlants[10];
    u8 decorationOrnaments[30];
    u8 decorationMats[30];
    u8 decorationPosters[10];
    u8 decorationDolls[40];
    u8 decorationCushions[10];
    TVShow tvShows[25];
    PokeNews pokeNews[16];
    u16 outbreakPokemonSpecies;
    u8 outbreakLocationMapNum;
    u8 outbreakLocationMapGroup;
    u8 outbreakPokemonLevel;
    u8 outbreakUnused1;
    u16 outbreakUnused2;
    u16 outbreakPokemonMoves[4];
    u8 outbreakUnused3;
    u8 outbreakPokemonProbability;
    u16 outbreakDaysLeft;
    struct GabbyAndTyData gabbyAndTyData;
    u16 easyChatProfile[6];
    u16 easyChatBattleStart[6];
    u16 easyChatBattleWon[6];
    u16 easyChatBattleLost[6];
    struct Mail mail[16];
    u8 unlockedTrendySayings[5];
    OldMan oldMan;
    struct DewfordTrend dewfordTrends[5];
    struct ContestWinner_v0 contestWinners[13];
    struct DayCare_v0 daycare;
    struct LinkBattleRecords linkBattleRecords;
    u8 giftRibbons[11];
    struct ExternalEventData externalEventData;
    struct ExternalEventFlags externalEventFlags;
    struct Roamer roamer;
    struct EnigmaBerry enigmaBerry;
    u8 mysteryGift[0x36C];
    u8 unused_3598[0x180];
    u32 trainerHillTimes[4];
    u8 ramScript[0x3EC];
    struct RecordMixingGift recordMixingGift;
    u8 seen2[52];
    LilycoveLady lilycoveLady;
    struct TrainerNameRecord trainerNameRecords[20];
    u8 registeredTexts[10][21];
    u8 unused_3D5A[10];
    struct TrainerHillSave trainerHill;
    struct WaldaPhrase waldaPhrase;
};

struct PokemonStorage_v0
{
    u8 currentBox;
    struct BoxPokemon boxes[14][30];
    u8 boxNames[14][9];
    u8 boxWallpapers[14];
};

/* Offsets and sizes from vanilla Pokemon Emerald's global.h. */
STATIC_ASSERT(sizeof(struct Pokedex_v0) == 0x78, Pokedex_v0Size);
STATIC_ASSERT(sizeof(struct Time_v0) == 0x8, Time_v0Size);
STATIC_ASSERT(sizeof(struct BerryCrush_v0) == 0x10, BerryCrush_v0Size);
STATIC_ASSERT(sizeof(struct ContestWinner_v0) == 0x20, ContestWinner_v0Size);
STATIC_ASSERT(sizeof(struct DayCare_v0) == 0x120, DayCare_v0Size);
STATIC_ASSERT(offsetof(struct SaveBlock2_v0, pokedex) == 0x18, SaveBlock2_v0PokedexOffset);
STATIC_ASSERT(offsetof(struct SaveBlock2_v0, localTimeOffset) == 0x98, SaveBlock2_v0LocalTimeOffset);
STATIC_ASSERT(offsetof(struct SaveBlock2_v0, frontier) == 0x64C, SaveBlock2_v0FrontierOffset);
STATIC_ASSERT(sizeof(struct SaveBlock2_v0) == 0xF2C, SaveBlock2_v0Size);

STATIC_ASSERT(offsetof(struct SaveBlock1_v0, playerParty) == 0x238, SaveBlock1_v0PartyOffset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, pcItems) == 0x498, SaveBlock1_v0PcItemsOffset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, seen1) == 0x988, SaveBlock1_v0Seen1Offset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, objectEvents) == 0xA30, SaveBlock1_v0ObjectEventsOffset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, flags) == 0x1270, SaveBlock1_v0FlagsOffset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, secretBases) == 0x1A9C, SaveBlock1_v0SecretBasesOffset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, tvShows) == 0x27CC, SaveBlock1_v0TvShowsOffset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, enigmaBerry) == 0x31F8, SaveBlock1_v0EnigmaBerryOffset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, ramScript) == 0x3728, SaveBlock1_v0RamScriptOffset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, seen2) == 0x3B24, SaveBlock1_v0Seen2Offset);
STATIC_ASSERT(offsetof(struct SaveBlock1_v0, waldaPhrase) == 0x3D70, SaveBlock1_v0WaldaPhraseOffset);
STATIC_ASSERT(sizeof(struct SaveBlock1_v0) == 0x3D88, SaveBlock1_v0Size);

STATIC_ASSERT(offsetof(struct PokemonStorage_v0, boxes) == 0x4, PokemonStorage_v0BoxesOffset);
STATIC_ASSERT(offsetof(struct PokemonStorage_v0, boxNames) == 0x8344, PokemonStorage_v0NamesOffset);
STATIC_ASSERT(offsetof(struct PokemonStorage_v0, boxWallpapers) == 0x83C2, PokemonStorage_v0WallpapersOffset);
STATIC_ASSERT(sizeof(struct PokemonStorage_v0) == 0x83D0, PokemonStorage_v0Size);

static bool8 UpdateSave_v0_v1(const struct SaveSectorLocation *locations)
{
    const struct SaveBlock2_v0 *oldSaveBlock2 = (const void *)locations[0].data;
    const struct SaveBlock1_v0 *oldSaveBlock1 = (const void *)locations[1].data;
    const struct PokemonStorage_v0 *oldPokemonStorage = (const void *)locations[5].data;
    u32 i;

    gSaveBlock3Ptr->saveVersion = SAVE_VERSION_1;

#define COPY_FIELD(field) gSaveBlock2Ptr->field = oldSaveBlock2->field
#define COPY_BLOCK(field) memcpy(&gSaveBlock2Ptr->field, &oldSaveBlock2->field, min(sizeof(gSaveBlock2Ptr->field), sizeof(oldSaveBlock2->field)))
#define COPY_ARRAY(field) for (i = 0; i < min(ARRAY_COUNT(gSaveBlock2Ptr->field), ARRAY_COUNT(oldSaveBlock2->field)); i++) gSaveBlock2Ptr->field[i] = oldSaveBlock2->field[i]

    COPY_ARRAY(playerName);
    COPY_FIELD(playerGender);
    COPY_FIELD(specialSaveWarpFlags);
    COPY_ARRAY(playerTrainerId);
    COPY_FIELD(playTimeHours);
    COPY_FIELD(playTimeMinutes);
    COPY_FIELD(playTimeSeconds);
    COPY_FIELD(playTimeVBlanks);

    COPY_FIELD(optionsButtonMode);
    COPY_FIELD(optionsTextSpeed);
    COPY_FIELD(optionsWindowFrameType);
    COPY_FIELD(optionsSound);
    COPY_FIELD(optionsBattleStyle);
    COPY_FIELD(optionsBattleSceneOff);
    COPY_FIELD(regionMapZoom);

    gSaveBlock2Ptr->pokedex.order = oldSaveBlock2->pokedex.order;
    gSaveBlock2Ptr->pokedex.mode = oldSaveBlock2->pokedex.mode;
    gSaveBlock2Ptr->pokedex.nationalMagic = oldSaveBlock2->pokedex.nationalMagic;
    gSaveBlock2Ptr->pokedex.unknown2 = oldSaveBlock2->pokedex.unknown2;
    gSaveBlock2Ptr->pokedex.unownPersonality = oldSaveBlock2->pokedex.unownPersonality;
    gSaveBlock2Ptr->pokedex.spindaPersonality = oldSaveBlock2->pokedex.spindaPersonality;
    gSaveBlock2Ptr->pokedex.unknown3 = oldSaveBlock2->pokedex.unknown3;

    gSaveBlock2Ptr->localTimeOffset.days = oldSaveBlock2->localTimeOffset.days;
    gSaveBlock2Ptr->localTimeOffset.hours = oldSaveBlock2->localTimeOffset.hours;
    gSaveBlock2Ptr->localTimeOffset.minutes = oldSaveBlock2->localTimeOffset.minutes;
    gSaveBlock2Ptr->localTimeOffset.seconds = oldSaveBlock2->localTimeOffset.seconds;
    gSaveBlock2Ptr->lastBerryTreeUpdate.days = oldSaveBlock2->lastBerryTreeUpdate.days;
    gSaveBlock2Ptr->lastBerryTreeUpdate.hours = oldSaveBlock2->lastBerryTreeUpdate.hours;
    gSaveBlock2Ptr->lastBerryTreeUpdate.minutes = oldSaveBlock2->lastBerryTreeUpdate.minutes;
    gSaveBlock2Ptr->lastBerryTreeUpdate.seconds = oldSaveBlock2->lastBerryTreeUpdate.seconds;
    COPY_FIELD(gcnLinkFlags);
    COPY_FIELD(encryptionKey);

    COPY_FIELD(playerApprentice);
    COPY_BLOCK(apprentices);
    for (i = 0; i < ARRAY_COUNT(oldSaveBlock2->berryCrush.pressingSpeeds); i++)
        gSaveBlock2Ptr->berryCrush.pressingSpeeds[i] = oldSaveBlock2->berryCrush.pressingSpeeds[i];
    gSaveBlock2Ptr->berryCrush.berryPowderAmount = oldSaveBlock2->berryCrush.berryPowderAmount;
    gSaveBlock2Ptr->berryCrush.unk = oldSaveBlock2->berryCrush.unk;
#if FREE_POKEMON_JUMP == FALSE
    COPY_FIELD(pokeJump);
#endif
    COPY_FIELD(berryPick);
#if FREE_RECORD_MIXING_HALL_RECORDS == FALSE
    COPY_BLOCK(hallRecords1P);
    COPY_BLOCK(hallRecords2P);
#endif
    COPY_BLOCK(contestLinkResults);
    COPY_FIELD(frontier);

#undef COPY_FIELD
#undef COPY_BLOCK
#undef COPY_ARRAY

#define COPY_FIELD(field) gSaveBlock1Ptr->field = oldSaveBlock1->field
#define COPY_BLOCK(field) memcpy(&gSaveBlock1Ptr->field, &oldSaveBlock1->field, min(sizeof(gSaveBlock1Ptr->field), sizeof(oldSaveBlock1->field)))
#define COPY_ARRAY(field) for (i = 0; i < min(ARRAY_COUNT(gSaveBlock1Ptr->field), ARRAY_COUNT(oldSaveBlock1->field)); i++) gSaveBlock1Ptr->field[i] = oldSaveBlock1->field[i]

    COPY_FIELD(pos);
    COPY_FIELD(location);
    COPY_FIELD(continueGameWarp);
    COPY_FIELD(dynamicWarp);
    COPY_FIELD(lastHealLocation);
    COPY_FIELD(escapeWarp);

    /* Force the destination map to reload instead of restoring stale map data. */
    COPY_FIELD(playerPartyCount);
    COPY_ARRAY(playerParty);

    COPY_FIELD(money);
    COPY_FIELD(coins);
    gSaveBlock1Ptr->registeredItemCompat = oldSaveBlock1->registeredItem;
    gSaveBlock1Ptr->registeredItems[0] = oldSaveBlock1->registeredItem;
    COPY_ARRAY(pcItems);
    COPY_ARRAY(bagPocket_Items);
    COPY_ARRAY(bagPocket_KeyItems);
    COPY_ARRAY(bagPocket_PokeBalls);
    COPY_ARRAY(bagPocket_TMHM);
    COPY_ARRAY(bagPocket_Berries);
    COPY_BLOCK(pokeblocks);

    COPY_BLOCK(berryBlenderRecords);
#if FREE_MATCH_CALL == FALSE
    COPY_FIELD(trainerRematchStepCounter);
    COPY_BLOCK(trainerRematches);
#endif

    COPY_BLOCK(flags);
    COPY_BLOCK(vars);
    COPY_BLOCK(gameStats);
    COPY_BLOCK(berryTrees);

    COPY_ARRAY(secretBases);
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

    COPY_BLOCK(easyChatProfile);
    COPY_BLOCK(easyChatBattleStart);
    COPY_BLOCK(easyChatBattleWon);
    COPY_BLOCK(easyChatBattleLost);

    COPY_BLOCK(mail);
    COPY_BLOCK(unlockedTrendySayings);
    COPY_FIELD(oldMan);
    COPY_BLOCK(dewfordTrends);
    for (i = 0; i < min(ARRAY_COUNT(gSaveBlock1Ptr->contestWinners), ARRAY_COUNT(oldSaveBlock1->contestWinners)); i++)
    {
        gSaveBlock1Ptr->contestWinners[i].personality = oldSaveBlock1->contestWinners[i].personality;
        gSaveBlock1Ptr->contestWinners[i].trainerId = oldSaveBlock1->contestWinners[i].trainerId;
        gSaveBlock1Ptr->contestWinners[i].species = oldSaveBlock1->contestWinners[i].species;
        gSaveBlock1Ptr->contestWinners[i].contestCategory = oldSaveBlock1->contestWinners[i].contestCategory;
        memcpy(gSaveBlock1Ptr->contestWinners[i].monName, oldSaveBlock1->contestWinners[i].monName, sizeof(oldSaveBlock1->contestWinners[i].monName));
        memcpy(gSaveBlock1Ptr->contestWinners[i].trainerName, oldSaveBlock1->contestWinners[i].trainerName, sizeof(oldSaveBlock1->contestWinners[i].trainerName));
        gSaveBlock1Ptr->contestWinners[i].contestRank = oldSaveBlock1->contestWinners[i].contestRank;
    }
    COPY_BLOCK(daycare.mons);
    gSaveBlock1Ptr->daycare.offspringPersonality = oldSaveBlock1->daycare.offspringPersonality;
    gSaveBlock1Ptr->daycare.stepCounter = oldSaveBlock1->daycare.stepCounter;
#if FREE_LINK_BATTLE_RECORDS == FALSE
    COPY_FIELD(linkBattleRecords);
#endif
    COPY_BLOCK(giftRibbons);
    COPY_FIELD(externalEventData);
    COPY_FIELD(externalEventFlags);
    gSaveBlock1Ptr->roamer[0] = oldSaveBlock1->roamer;
#if FREE_ENIGMA_BERRY == FALSE
    COPY_FIELD(enigmaBerry);
#endif
    for (i = 0; i < 52 && i < ARRAY_COUNT(gSaveBlock1Ptr->dexSeen); i++)
    {
        gSaveBlock1Ptr->dexCaught[i] = oldSaveBlock2->pokedex.owned[i];
        gSaveBlock1Ptr->dexSeen[i] = oldSaveBlock2->pokedex.seen[i]
                                    | oldSaveBlock1->seen1[i]
                                    | oldSaveBlock1->seen2[i];
    }

#if FREE_TRAINER_HILL == FALSE
    COPY_BLOCK(trainerHillTimes);
#endif
    COPY_FIELD(recordMixingGift);
    COPY_FIELD(lilycoveLady);
    COPY_BLOCK(trainerNameRecords);
#if FREE_UNION_ROOM_CHAT == FALSE
    COPY_BLOCK(registeredTexts);
#endif
#if FREE_TRAINER_HILL == FALSE
    COPY_FIELD(trainerHill);
#endif
    COPY_FIELD(waldaPhrase);

#undef COPY_FIELD
#undef COPY_BLOCK
#undef COPY_ARRAY

    gPokemonStoragePtr->currentBox = oldPokemonStorage->currentBox;
    memcpy(gPokemonStoragePtr->boxes, oldPokemonStorage->boxes, sizeof(oldPokemonStorage->boxes));
    memcpy(gPokemonStoragePtr->boxNames, oldPokemonStorage->boxNames, sizeof(oldPokemonStorage->boxNames));
    memcpy(gPokemonStoragePtr->boxWallpapers, oldPokemonStorage->boxWallpapers, sizeof(oldPokemonStorage->boxWallpapers));

    SetContinueGameWarpStatus();
    gSaveBlock1Ptr->continueGameWarp = gSaveBlock1Ptr->lastHealLocation;

    return SAVE_UFR_SUCCESS;
}
