#include "global.h"
#include "best_of_three.h"
#include "strings.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "field_effect.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "list_menu.h"
#include "item_icon.h"
#include "item_use.h"
#include "pokemon_icon.h"
#include "international_string_util.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "party_menu.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "overworld.h"
#include "event_data.h"
#include "constants/items.h"
#include "constants/field_weather.h"
#include "constants/songs.h"
#include "constants/rgb.h"
#include "constants/opponents.h"

/*
 * 
 */
 
//==========DEFINES==========//
struct MenuResources
{
    MainCallback savedCallback;     // determines callback to run when we exit. e.g. where do we want to go after closing the menu
    u8 gfxLoadState;
    u32 opponentTrainerPicId;
    const struct TrainerMon *opponentTeam;
    u8 opponentTeamSize;
    u8 playerSpriteIds[PARTY_SIZE];
    u8 opponentSpriteIds[PARTY_SIZE];
    u8 playerTrainerSprite;
    u8 opponentTrainerSprite;
};

enum WindowIds
{
    WINDOW_1,
};

//==========EWRAM==========//
static EWRAM_DATA struct MenuResources *sMenuDataPtr = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL; // BO3InfoCard

//==========STATIC=DEFINES==========//
static void Menu_RunSetup(void);
static bool8 Menu_DoGfxSetup(void);
static bool8 Menu_InitBgs(void);
static void Menu_FadeAndBail(void);
static bool8 Menu_LoadGraphics(void);
static void Menu_InitWindows(void);
static void PrintToWindow(u8 windowId, u8 colorIdx);
static void Task_MenuWaitFadeIn(u8 taskId);
static void Task_MenuMain(u8 taskId);
static void DrawTeamMonIcons(void);
static void DrawTrainerPics(void);
static void DestroyTeamMonIcons(void);
static void DestroyTrainerPics(void);

//==========CONST=DATA==========//
static const struct BgTemplate sMenuBgTemplates[] =
{
    {
        .bg = 0,    // windows, etc
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
    }, 
    {
        .bg = 1,    // idk why this is here anymore
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
    },
    {
        .bg = 2,    // sBg2TilemapBuffer
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
    },
    {
        .bg = 3,    // Put Scrolling Bg here
        .charBaseIndex = 0,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
    }
};

static const struct WindowTemplate sMenuWindowTemplates[] = 
{
    [WINDOW_1] = 
    {
        .bg = 0,            // which bg to print text on
        .tilemapLeft = 4,   // position from left (per 8 pixels)
        .tilemapTop = 3,    // position from top (per 8 pixels)
        .width = 10,        // width (per 8 pixels)
        .height = 3,        // height (per 8 pixels)
        .paletteNum = 15,   // palette index to use for text
        .baseBlock = 1,     // tile start in VRAM
    },
    DUMMY_WIN_TEMPLATE,
};

enum Colors
{
    FONT_BLACK,
    FONT_WHITE,
    FONT_RED,
    FONT_BLUE,
};
static const u8 sMenuWindowFontColors[][3] =
{
    [FONT_BLACK]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY},
    [FONT_WHITE]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_WHITE,  TEXT_COLOR_DARK_GRAY},
    [FONT_RED]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_GRAY},
    [FONT_BLUE]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_GRAY},
};

#define PLAYER_ICON_START_X 16
#define OPPONENT_ICON_START_X 160
#define ICON_START_Y 40
#define ICON_X_SPACING 32
#define ICON_Y_SPACING 32

enum Bo3Sprites
{
    SPRITE_1 = 0,
    SPRITE_2,
    SPRITE_3,
    SPRITE_4,
    SPRITE_5,
    SPRITE_6,
};

struct Bo3SpriteLocations
{
    u8 x;
    u8 y;
};

static const struct Bo3SpriteLocations sPlayerSpriteLocations[] =
{
    [SPRITE_1]
    { .x = 97, .y = 38 },

    [SPRITE_2]
    { .x = 97, .y = 72 },

    [SPRITE_3]
    { .x = 97, .y = 102 },

    [SPRITE_4]
    { .x = 97, .y = 135 },

    [SPRITE_5]
    { .x = 60, .y = 135 },

    [SPRITE_6]
    { .x = 23, .y = 135 },
};

static const struct Bo3SpriteLocations sOpponentSpriteLocations[] =
{
    [SPRITE_1]
    { .x = 143, .y = 38 },

    [SPRITE_2]
    { .x = 143, .y = 72 },

    [SPRITE_3]
    { .x = 143, .y = 102 },

    [SPRITE_4]
    { .x = 143, .y = 135 },

    [SPRITE_5]
    { .x = 179, .y = 135 },

    [SPRITE_6]
    { .x = 220, .y = 135 },
};

const u32 sBO3InfoCard_Gfx[] = INCBIN_U32("graphics/best_of_three/infocard.4bpp.smol");
const u32 sBO3InfoCard_Tilemap[] = INCBIN_U32("graphics/best_of_three/infocard_tilemap.bin.lz");
const u32 sBO3InfoCard_Pal[] = INCBIN_U32("graphics/best_of_three/infocard.gbapal");

#define sMonIconStill data[3]
static void SpriteCb_MonIcon(struct Sprite *sprite)
{
    if (!sprite->sMonIconStill)
        UpdateMonIconFrame(sprite);
}
#undef sMonIconStill

//==========FUNCTIONS==========//
// UI loader template
void Task_OpenBO3MenuFromStartMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        
        CleanupOverworldWindowsAndTilemaps();
        const struct TrainerMon *party = GetTrainerPartyFromId(TRAINER_JUAN_1);
        u32 trainerPicId = GetTrainerPicFromId(TRAINER_JUAN_1);
        u8 size = GetTrainerPartySizeFromId(TRAINER_JUAN_1);
        BO3Menu_Init(trainerPicId, party, size, CB2_ReturnToFieldWithOpenMenu);
        DestroyTask(taskId);
    }
}

// Initialize the Best of Three menu with the provided opponent team
void BO3Menu_Init(u32 trainerPicId, const struct TrainerMon *opponentTeam, u8 opponentTeamSize, MainCallback callback)
{
    if ((sMenuDataPtr = AllocZeroed(sizeof(struct MenuResources))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }
    
    // initialize stuff
    sMenuDataPtr->gfxLoadState = 0;
    sMenuDataPtr->savedCallback = callback;
    sMenuDataPtr->opponentTrainerPicId = trainerPicId;
    sMenuDataPtr->opponentTeam = opponentTeam;
    sMenuDataPtr->opponentTeamSize = opponentTeamSize;
    memset(sMenuDataPtr->playerSpriteIds, SPRITE_NONE, sizeof(sMenuDataPtr->playerSpriteIds));
    memset(sMenuDataPtr->opponentSpriteIds, SPRITE_NONE, sizeof(sMenuDataPtr->opponentSpriteIds));

    SetMainCallback2(Menu_RunSetup);
}

static void Menu_RunSetup(void)
{
    while (1)
    {
        if (Menu_DoGfxSetup() == TRUE)
            break;
    }
}

static void Menu_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void Menu_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    ChangeBgX(3, 64, BG_COORD_ADD);
    ChangeBgY(3, 64, BG_COORD_ADD);
}

static bool8 Menu_DoGfxSetup(void)
{
    switch (gMain.state)
    {
    case 0:
        DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000)
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        ResetVramOamAndBgCntRegs();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        gMain.state++;
        break;
    case 2:
        if (Menu_InitBgs())
        {
            sMenuDataPtr->gfxLoadState = 0;
            gMain.state++;
        }
        else
        {
            Menu_FadeAndBail();
            return TRUE;
        }
        break;
    case 3:
        if (Menu_LoadGraphics() == TRUE)
            gMain.state++;
        break;
    case 4:
        LoadMessageBoxAndBorderGfx();
        FreeMonIconPalettes();
        LoadMonIconPalettes();
        Menu_InitWindows();
        DrawTeamMonIcons();
        DrawTrainerPics();
        gMain.state++;
        break;
    case 5:
        PrintToWindow(WINDOW_1, FONT_WHITE);
        CreateTask(Task_MenuWaitFadeIn, 0);
        BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(Menu_VBlankCB);
        SetMainCallback2(Menu_MainCB);
        return TRUE;
    }
    return FALSE;
}

#define try_free(ptr) ({        \
    void ** ptr__ = (void **)&(ptr);   \
    if (*ptr__ != NULL)                \
        Free(*ptr__);                  \
})

static void Menu_FreeResources(void)
{
    DestroyTeamMonIcons();
    DestroyTrainerPics();
    try_free(sMenuDataPtr);
    try_free(sBg2TilemapBuffer);
    FreeAllWindowBuffers();
}


static void Task_MenuWaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sMenuDataPtr->savedCallback);
        Menu_FreeResources();
        DestroyTask(taskId);
    }
}

static void Menu_FadeAndBail(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_MenuWaitFadeAndBail, 0);
    SetVBlankCallback(Menu_VBlankCB);
    SetMainCallback2(Menu_MainCB);
}

static bool8 Menu_InitBgs(void)
{
    ResetAllBgsCoordinates();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sMenuBgTemplates, NELEMS(sMenuBgTemplates));

    sBg2TilemapBuffer = Alloc(0x1000);
    if (sBg2TilemapBuffer == NULL)
        return FALSE;
    memset(sBg2TilemapBuffer, 0, 0x1000);
    SetBgTilemapBuffer(2, sBg2TilemapBuffer);
    ScheduleBgCopyTilemapToVram(2);

    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
    return TRUE;
}

static bool8 Menu_LoadGraphics(void)
{
    switch (sMenuDataPtr->gfxLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(2, sBO3InfoCard_Gfx, 0, 0, 0);
        sMenuDataPtr->gfxLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(sBO3InfoCard_Tilemap, sBg2TilemapBuffer);
            sMenuDataPtr->gfxLoadState++;
        }
        break;
    case 2:
        LoadPalette(sBO3InfoCard_Pal, 0, PLTT_SIZE_8BPP);
        sMenuDataPtr->gfxLoadState++;
        break;
    default:
        sMenuDataPtr->gfxLoadState = 0;
        return TRUE;
    }
    return FALSE;
}

static void Menu_InitWindows(void)
{
    InitWindows(sMenuWindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    
    FillWindowPixelBuffer(WINDOW_1, 0);
    LoadUserWindowBorderGfx(WINDOW_1, 720, 14 * 16);
    PutWindowTilemap(WINDOW_1);
    CopyWindowToVram(WINDOW_1, 3);
    
    ScheduleBgCopyTilemapToVram(1);
}

static const u8 sText_MyMenu[] = _("May");
static void PrintToWindow(u8 windowId, u8 colorIdx)
{
    const u8 *str = sText_MyMenu;
    u8 x = 1;
    u8 y = 1;
    
    FillWindowPixelBuffer(windowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    AddTextPrinterParameterized4(windowId, 1, x, y, 0, 0, sMenuWindowFontColors[colorIdx], 0xFF, str);
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, 3);
}

static void Task_MenuWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_MenuMain;
}

static void Task_MenuTurnOff(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sMenuDataPtr->savedCallback);
        Menu_FreeResources();
        DestroyTask(taskId);
    }
}


/* This is the meat of the UI. This is where you wait for player inputs and can branch to other tasks accordingly */
static void Task_MenuMain(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_PC_OFF);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_MenuTurnOff;
    }
}

static void DrawTeamMonIcons(void)
{
    u8 i;
    u16 species;

    for (i = 0; i < gPlayerPartyCount && i < PARTY_SIZE; i++)
    {
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);
        sMenuDataPtr->playerSpriteIds[i] = CreateMonIcon(species, SpriteCb_MonIcon, sPlayerSpriteLocations[i].x, sPlayerSpriteLocations[i].y, 1, GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY));
        gSprites[sMenuDataPtr->playerSpriteIds[i]].oam.priority = 0;
        StartSpriteAnim(&gSprites[sMenuDataPtr->playerSpriteIds[i]], 4);
    }

    for (i = 0; i < sMenuDataPtr->opponentTeamSize && i < PARTY_SIZE; i++)
    {
        species = sMenuDataPtr->opponentTeam[i].species;
        sMenuDataPtr->opponentSpriteIds[i] = CreateMonIconNoPersonality(GetIconSpeciesNoPersonality(species), SpriteCb_MonIcon, sOpponentSpriteLocations[i].x, sOpponentSpriteLocations[i].y, 1);
        gSprites[sMenuDataPtr->opponentSpriteIds[i]].oam.priority = 0;
        StartSpriteAnim(&gSprites[sMenuDataPtr->opponentSpriteIds[i]], 4);
    }
}

static void DrawTrainerPics(void)
{
    u32 trainerPicId = sMenuDataPtr->opponentTrainerPicId;

    sMenuDataPtr->opponentTrainerSprite = CreateTrainerSprite(trainerPicId,
        200, 86, 0, NULL);

    sMenuDataPtr->playerTrainerSprite = CreateTrainerSprite(
        PlayerGenderToFrontTrainerPicId(gSaveBlock2Ptr->playerGender), 
        44, 86, 0, NULL);
}

static void DestroyTeamMonIcons(void)
{
    u8 i;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (sMenuDataPtr->playerSpriteIds[i] != SPRITE_NONE)
            FreeAndDestroyMonIconSprite(&gSprites[sMenuDataPtr->playerSpriteIds[i]]);
        if (sMenuDataPtr->opponentSpriteIds[i] != SPRITE_NONE)
            FreeAndDestroyMonIconSprite(&gSprites[sMenuDataPtr->opponentSpriteIds[i]]);
    }
    FreeMonIconPalettes();
}

static void DestroyTrainerPics(void)
{
    if (sMenuDataPtr->opponentTrainerSprite != SPRITE_NONE)
        DestroySpriteAndFreeResources(&gSprites[sMenuDataPtr->opponentTrainerSprite]);
    if (sMenuDataPtr->playerTrainerSprite != SPRITE_NONE)
        DestroySpriteAndFreeResources(&gSprites[sMenuDataPtr->playerTrainerSprite]);
}
