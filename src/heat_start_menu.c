#include "global.h"
#include "option_menu.h"
#include "heat_start_menu.h"
#include "battle_pike.h"
#include "battle_pyramid.h"
#include "battle_pyramid_bag.h"
#include "bg.h"
#include "decompress.h"
#include "dexnav.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_object_lock.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "field_weather.h"
#include "field_screen_effect.h"
#include "frontier_pass.h"
#include "frontier_util.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item_menu.h"
#include "link.h"
#include "load_save.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "new_game.h"
#include "option_menu.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokedex.h"
#include "pokenav.h"
#include "safari_zone.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sprite.h"
#include "sound.h"
#include "start_menu.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trainer_card.h"
#include "window.h"
#include "union_room.h"
#include "constants/battle_frontier.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "rtc.h"
#include "event_object_movement.h"
#include "gba/isagbprint.h"

/* ENUMs */
enum MenuOption 
{
    MENU_POKEDEX,
    MENU_PARTY,
    MENU_BAG,
    MENU_POKETCH,
    MENU_TRAINER_CARD,
    MENU_SAVE,
    MENU_OPTIONS,
    MENU_FLAG,
    MENU_COUNT,
};

enum PlayerLocation {
    LOCATION_OVERWORLD,
    LOCATION_SAFARI,
    LOCATION_PYRAMID
};

static const u8 sOverworldMenu[] = { MENU_POKEDEX, MENU_PARTY, MENU_BAG, MENU_TRAINER_CARD, MENU_SAVE, MENU_OPTIONS };
static const u8 sSafariMenu[] = { MENU_POKEDEX, MENU_PARTY, MENU_BAG, MENU_TRAINER_CARD, MENU_OPTIONS, MENU_FLAG };
static const u8 sPyramidMenu[] = { MENU_PARTY, MENU_BAG, MENU_TRAINER_CARD, MENU_SAVE, MENU_FLAG, MENU_OPTIONS };

enum FLAG_VALUES 
{
    FLAG_VALUE_NOT_SET,
    FLAG_VALUE_SET,
};

enum SAVE_STATES 
{
    SAVE_IN_PROGRESS,
    SAVE_SUCCESS,
    SAVE_CANCELED,
    SAVE_ERROR
};

/* STRUCTs */
struct HeatStartMenu 
{
    MainCallback savedCallback;
    u32 loadState;
    u32 sStartClockWindowId;
    u32 sMenuNameWindowId;
    u32 sTopLeftWindowId;
    u32 flag; // some u32 holding values for controlling the sprite anims and lifetime
    u32 lightLevel;

    u32 spriteIdPoketch;
    u32 spriteIdPokedex;
    u32 spriteIdParty;
    u32 spriteIdBag;
    u32 spriteIdTrainerCard;
    u32 spriteIdSave;
    u32 spriteIdOptions;
    u32 spriteIdFlag;
};

/* CALLBACKS */
static void SpriteCB_IconPoketch(struct Sprite* sprite);
static void SpriteCB_IconPokedex(struct Sprite* sprite);
static void SpriteCB_IconParty(struct Sprite* sprite);
static void SpriteCB_IconBag(struct Sprite* sprite);
static void SpriteCB_IconTrainerCard(struct Sprite* sprite);
static void SpriteCB_IconSave(struct Sprite* sprite);
static void SpriteCB_IconOptions(struct Sprite* sprite);
static void SpriteCB_IconFlag(struct Sprite* sprite);

/* TASKs */
static void Task_HeatStartMenu_HandleInput(u8 taskId);
static void Task_HandleSave(u8 taskId);

/* OTHER FUNCTIONS */
static void SetupHeatMenuCommonComponents(void);
static void HeatStartMenu_LoadSprites(void);
static void HeatStartMenu_CreateSprites(void);
static void HeatStartMenu_SafariZone_CreateSprites(void);
static void HeatStartMenu_BattlePyramid_CreateSprites(void);
static void HeatStartMenu_LoadBgGfx(void);
static void HeatStartMenu_ShowTimeWindow(void);
static void HeatStartMenu_UpdateClockDisplay(void);
static void HeatStartMenu_UpdateMenuName(void);
static u8 RunSaveCallback(void);
static u8 SaveDoSaveCallback(void);
static void HideSaveInfoWindow(void);
static void HideSaveMessageWindow(void);
static u8 SaveOverwriteInputCallback(void);
static u8 SaveConfirmOverwriteDefaultNoCallback(void);
static void ShowSaveMessage(const u8 *message, u8 (*saveCallback)(void));
static u8 SaveFileExistsCallback(void);
static u8 SaveSavingMessageCallback(void);
static u8 SaveConfirmInputCallback(void);
static u8 SaveYesNoCallback(void);
static void ShowSaveInfoWindow(void);
static u8 SaveConfirmSaveCallback(void);
static void Task_HeatStartMenu_HandleInput(u8 taskId);
static void HeatStartMenu_HandleInput_Move(int direction, enum PlayerLocation location);
static void InitSave(void);

static EWRAM_DATA struct HeatStartMenu *sHeatStartMenu = NULL;
static EWRAM_DATA u8 menuSelected = 0;
static EWRAM_DATA u8 (*sSaveDialogCallback)(void) = NULL;
static EWRAM_DATA u8 sSaveDialogTimer = 0;
static EWRAM_DATA u8 sSaveInfoWindowId = 0;

// --BG-GFX--
static const u32 sStartMenuTiles[] = INCBIN_U32("graphics/heat_start_menu/bg.4bpp.smol");
static const u32 sStartMenuTilemap[] = INCBIN_U32("graphics/heat_start_menu/bg.bin.lz");
static const u32 sStartMenuTilemapDexNav[] = INCBIN_U32("graphics/heat_start_menu/bg_dexnav.bin.lz");
static const u32 sStartMenuTilemapSafari[] = INCBIN_U32("graphics/heat_start_menu/bg_safari.bin.lz");
static const u16 sStartMenuPalette[] = INCBIN_U16("graphics/heat_start_menu/bg.gbapal");
const u16 gStandardMenuPalette[] = INCBIN_U16("graphics/interface/std_menu.gbapal");

//--SPRITE-GFX--
#define TAG_ICON_GFX 1234
#define TAG_ICON_PAL 0x4654

#define ICON_POS_X 224

#define ICON_POS_1 14
#define ICON_POS_2 38
#define ICON_POS_3 60
#define ICON_POS_4 84
#define ICON_POS_5 109
#define ICON_POS_6 130
#define ICON_POS_7 150

static const u32 sIconGfx[] = INCBIN_U32("graphics/heat_start_menu/icons.4bpp.fastSmol");
static const u16 sIconPal[] = INCBIN_U16("graphics/heat_start_menu/icons.gbapal");

static const struct WindowTemplate sSaveInfoWindowTemplate = 
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 14,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 8
};

static const struct WindowTemplate sWindowTemplate_StartClock = 
{
    .bg = 0, 
    .tilemapLeft = 2, 
    .tilemapTop = 17, 
    .width = 12, // If you want to shorten the dates to Sat., Sun., etc., change this to 9
    .height = 2, 
    .paletteNum = 15,
    .baseBlock = 0x30
};

static const struct WindowTemplate sWindowTemplate_MenuName = 
{
    .bg = 0, 
    .tilemapLeft = 16, 
    .tilemapTop = 17, 
    .width = 7, 
    .height = 2, 
    .paletteNum = 15,
    .baseBlock = 0x30 + (12*2)
};

static const struct WindowTemplate sWindowTemplate_SafariBallsOrPyramidFloor = 
{
    .bg = 0,
    .tilemapLeft = 2,
    .tilemapTop = 1,
    .width = 7,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = (0x30 + (12*2)) + (7*2)
};

static const struct WindowTemplate sWindowTemplate_DexNav =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 7,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = (0x30 + (12*2)) + (7*2)
};

static const struct SpritePalette sSpritePal_Icon[] =
{
    {sIconPal, TAG_ICON_PAL},
    {NULL},
};

static const struct CompressedSpriteSheet sSpriteSheet_Icon[] = 
{
    {sIconGfx, 32*512/2 , TAG_ICON_GFX},
    {NULL},
};

static const struct OamData gOamIcon = 
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
};

static const union AnimCmd gAnimCmdPoketch_NotSelected[] = 
{
    ANIMCMD_FRAME(112, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdPoketch_Selected[] = 
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconPoketchAnim[] = 
{
    gAnimCmdPoketch_NotSelected,
    gAnimCmdPoketch_Selected,
};

static const union AnimCmd gAnimCmdPokedex_NotSelected[] = 
{
    ANIMCMD_FRAME(128, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdPokedex_Selected[] = 
{
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconPokedexAnim[] = 
{
    gAnimCmdPokedex_NotSelected,
    gAnimCmdPokedex_Selected,
};

static const union AnimCmd gAnimCmdParty_NotSelected[] = 
{
    ANIMCMD_FRAME(144, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdParty_Selected[] = 
{
    ANIMCMD_FRAME(32, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconPartyAnim[] = 
{
    gAnimCmdParty_NotSelected,
    gAnimCmdParty_Selected,
};

static const union AnimCmd gAnimCmdBag_NotSelected[] = 
{
    ANIMCMD_FRAME(160, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdBag_Selected[] = 
{
    ANIMCMD_FRAME(48, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconBagAnim[] = 
{
    gAnimCmdBag_NotSelected,
    gAnimCmdBag_Selected,
};

static const union AnimCmd gAnimCmdTrainerCard_NotSelected[] = 
{
    ANIMCMD_FRAME(176, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdTrainerCard_Selected[] = 
{
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconTrainerCardAnim[] = 
{
    gAnimCmdTrainerCard_NotSelected,
    gAnimCmdTrainerCard_Selected,
};

static const union AnimCmd gAnimCmdSave_NotSelected[] = 
{
    ANIMCMD_FRAME(192, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdSave_Selected[] = 
{
    ANIMCMD_FRAME(80, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconSaveAnim[] = 
{
    gAnimCmdSave_NotSelected,
    gAnimCmdSave_Selected,
};

static const union AnimCmd gAnimCmdOptions_NotSelected[] = 
{
    ANIMCMD_FRAME(208, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdOptions_Selected[] = 
{
    ANIMCMD_FRAME(96, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconOptionsAnim[] = 
{
    gAnimCmdOptions_NotSelected,
    gAnimCmdOptions_Selected,
};

static const union AnimCmd gAnimCmdFlag_NotSelected[] = 
{
    ANIMCMD_FRAME(240, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdFlag_Selected[] = 
{
    ANIMCMD_FRAME(224, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconFlagAnim[] = 
{
    gAnimCmdFlag_NotSelected,
    gAnimCmdFlag_Selected,
};

static const union AffineAnimCmd sAffineAnimIcon_NoAnim[] =
{
    AFFINEANIMCMD_FRAME(0,0, 0, 60),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnimIcon_Anim[] =
{
    AFFINEANIMCMD_FRAME(20, 20, 0, 5),    // Scale big
    AFFINEANIMCMD_FRAME(-10, -10, 0, 10), // Scale smol
    AFFINEANIMCMD_FRAME(0, 0, 1, 4),    // Begin rotating

    AFFINEANIMCMD_FRAME(0, 0, -1, 4),     // Loop starts from here ; Rotate/Tilt left 
    AFFINEANIMCMD_FRAME(0, 0, 0, 2),
    AFFINEANIMCMD_FRAME(0, 0, -1, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 2),
    AFFINEANIMCMD_FRAME(0, 0, -1, 4),

    AFFINEANIMCMD_FRAME(0, 0, 1, 4),    // Rotate/Tilt Right
    AFFINEANIMCMD_FRAME(0, 0, 0, 2),
    AFFINEANIMCMD_FRAME(0, 0, 1, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 2),
    AFFINEANIMCMD_FRAME(0, 0, 1, 4),

    AFFINEANIMCMD_JUMP(3),
};

static const union AffineAnimCmd *const sAffineAnimsIcon[] =
{   
    sAffineAnimIcon_NoAnim,
    sAffineAnimIcon_Anim,
};

static const struct SpriteTemplate gSpriteIconPoketch = 
{
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconPoketchAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconPoketch,
};

static const struct SpriteTemplate gSpriteIconPokedex = 
{
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconPokedexAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconPokedex,
};

static const struct SpriteTemplate gSpriteIconParty = 
{
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconPartyAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconParty,
};

static const struct SpriteTemplate gSpriteIconBag = 
{
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconBagAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconBag,
};

static const struct SpriteTemplate gSpriteIconTrainerCard = 
{
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconTrainerCardAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconTrainerCard,
};

static const struct SpriteTemplate gSpriteIconSave = 
{
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconSaveAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconSave,
};

static const struct SpriteTemplate gSpriteIconOptions = 
{
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconOptionsAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconOptions,
};

static const struct SpriteTemplate gSpriteIconFlag = 
{
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconFlagAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconFlag,
};

static void SpriteCB_IconPoketch(struct Sprite* sprite) 
{
    if (menuSelected == MENU_POKETCH && sHeatStartMenu->flag == FLAG_VALUE_NOT_SET) 
    {
        sHeatStartMenu->flag = FLAG_VALUE_SET;
        StartSpriteAnim(sprite, 1);
        StartSpriteAffineAnim(sprite, 1);
    } 
    else if (menuSelected != MENU_POKETCH) 
    {
        StartSpriteAnim(sprite, 0);
        StartSpriteAffineAnim(sprite, 0);
    }
}

static void SpriteCB_IconPokedex(struct Sprite* sprite) 
{
    if (menuSelected == MENU_POKEDEX && sHeatStartMenu->flag == FLAG_VALUE_NOT_SET) 
    {
        sHeatStartMenu->flag = FLAG_VALUE_SET;
        StartSpriteAnim(sprite, 1);
        StartSpriteAffineAnim(sprite, 1);
    } 
    else if (menuSelected != MENU_POKEDEX) 
    {
        StartSpriteAnim(sprite, 0);
        StartSpriteAffineAnim(sprite, 0);
    }
}

static void SpriteCB_IconParty(struct Sprite* sprite) 
{
    if (menuSelected == MENU_PARTY && sHeatStartMenu->flag == FLAG_VALUE_NOT_SET) 
    {
        sHeatStartMenu->flag = FLAG_VALUE_SET;
        StartSpriteAnim(sprite, 1);
        StartSpriteAffineAnim(sprite, 1);
    } 
    else if (menuSelected != MENU_PARTY) 
    {
        StartSpriteAnim(sprite, 0);
        StartSpriteAffineAnim(sprite, 0);
    }
}

static void SpriteCB_IconBag(struct Sprite* sprite) 
{
    if (menuSelected == MENU_BAG && sHeatStartMenu->flag == FLAG_VALUE_NOT_SET) 
    {
        sHeatStartMenu->flag = FLAG_VALUE_SET;
        StartSpriteAnim(sprite, 1);
        StartSpriteAffineAnim(sprite, 1);
    } 
    else if (menuSelected != MENU_BAG) 
    {
        StartSpriteAnim(sprite, 0);
        StartSpriteAffineAnim(sprite, 0);
    } 
}

static void SpriteCB_IconTrainerCard(struct Sprite* sprite) 
{
    if (menuSelected == MENU_TRAINER_CARD && sHeatStartMenu->flag == FLAG_VALUE_NOT_SET) 
    {
        sHeatStartMenu->flag = FLAG_VALUE_SET;
        StartSpriteAnim(sprite, 1);
        StartSpriteAffineAnim(sprite, 1);
    } 
    else if (menuSelected != MENU_TRAINER_CARD) 
    {
        StartSpriteAnim(sprite, 0);
        StartSpriteAffineAnim(sprite, 0);
    } 
}

static void SpriteCB_IconSave(struct Sprite* sprite) 
{
    if (menuSelected == MENU_SAVE && sHeatStartMenu->flag == FLAG_VALUE_NOT_SET) 
    {
        sHeatStartMenu->flag = FLAG_VALUE_SET;
        StartSpriteAnim(sprite, 1);
        StartSpriteAffineAnim(sprite, 1);
    } 
    else if (menuSelected != MENU_SAVE) 
    {
        StartSpriteAnim(sprite, 0);
        StartSpriteAffineAnim(sprite, 0);
    } 
}

static void SpriteCB_IconOptions(struct Sprite* sprite) 
{
    if (menuSelected == MENU_OPTIONS && sHeatStartMenu->flag == FLAG_VALUE_NOT_SET) 
    {
        sHeatStartMenu->flag = FLAG_VALUE_SET;
        StartSpriteAnim(sprite, 1);
        StartSpriteAffineAnim(sprite, 1);
    } 
    else if (menuSelected != MENU_OPTIONS) 
    {
        StartSpriteAnim(sprite, 0);
        StartSpriteAffineAnim(sprite, 0);
    } 
}

static void SpriteCB_IconFlag(struct Sprite* sprite) 
{
    if (menuSelected == MENU_FLAG && sHeatStartMenu->flag == FLAG_VALUE_NOT_SET) 
    {
        sHeatStartMenu->flag = FLAG_VALUE_SET;
        StartSpriteAnim(sprite, 1);
        StartSpriteAffineAnim(sprite, 1);
    } 
    else if (menuSelected != MENU_FLAG) 
    {
        StartSpriteAnim(sprite, 0);
        StartSpriteAffineAnim(sprite, 0);
    } 
}

// If you want to shorten the dates to Sat., Sun., etc., change this to 70
#define CLOCK_WINDOW_WIDTH 100

static const u8 gText_CurrentTime[]    = _("  {STR_VAR_3} {CLEAR_TO 64}{STR_VAR_1}:{STR_VAR_2}");
static const u8 gText_CurrentTimeOff[]   = _("  {STR_VAR_3} {CLEAR_TO 64}{STR_VAR_1} {STR_VAR_2}");
static const u8 gText_CurrentTimeAM[]    = _("  {STR_VAR_3} {CLEAR_TO 51}{STR_VAR_1}:{STR_VAR_2} AM");
static const u8 gText_CurrentTimeAMOff[] = _("  {STR_VAR_3} {CLEAR_TO 51}{STR_VAR_1} {STR_VAR_2} AM");
static const u8 gText_CurrentTimePM[]    = _("  {STR_VAR_3} {CLEAR_TO 51}{STR_VAR_1}:{STR_VAR_2} PM");
static const u8 gText_CurrentTimePMOff[] = _("  {STR_VAR_3} {CLEAR_TO 51}{STR_VAR_1} {STR_VAR_2} PM");

static void SetSelectedMenu(void) 
{
    if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE) 
    {
        menuSelected = MENU_POKETCH;
    } 
    else if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE) 
    {
        menuSelected = MENU_POKEDEX;
    } 
    else if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE) 
    {
        menuSelected = MENU_PARTY;
    } 
    else 
    {
        menuSelected = MENU_BAG;
    }
}

static void ShowDexNavWindow(void)
{
    sHeatStartMenu->sTopLeftWindowId = AddWindow(&sWindowTemplate_DexNav);
    FillWindowPixelBuffer(sHeatStartMenu->sTopLeftWindowId, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sHeatStartMenu->sTopLeftWindowId);
    AddTextPrinterParameterized(sHeatStartMenu->sTopLeftWindowId, FONT_NARROW, gText_MenuDexNavShortcut, 0, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sHeatStartMenu->sTopLeftWindowId, COPYWIN_GFX);
}

static void ShowBattlePyramidFloor(void)
{
    sHeatStartMenu->sTopLeftWindowId = AddWindow(&sWindowTemplate_SafariBallsOrPyramidFloor);
    FillWindowPixelBuffer(sHeatStartMenu->sTopLeftWindowId, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sHeatStartMenu->sTopLeftWindowId);
    StringCopy(gStringVar1, sPyramidFloorNames[gSaveBlock2Ptr->frontier.curChallengeBattleNum]);
    StringExpandPlaceholders(gStringVar4, gText_PyramidFloor);
    AddTextPrinterParameterized(sHeatStartMenu->sTopLeftWindowId, FONT_NARROW, gStringVar4, 0, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sHeatStartMenu->sTopLeftWindowId, COPYWIN_GFX);
}

static void ShowSafariBallsWindow(void)
{
    sHeatStartMenu->sTopLeftWindowId = AddWindow(&sWindowTemplate_SafariBallsOrPyramidFloor);
    FillWindowPixelBuffer(sHeatStartMenu->sTopLeftWindowId, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sHeatStartMenu->sTopLeftWindowId);
    ConvertIntToDecimalStringN(gStringVar1, gNumSafariBalls, STR_CONV_MODE_RIGHT_ALIGN, 2);
    StringExpandPlaceholders(gStringVar4, gText_SafariBallStock);
    AddTextPrinterParameterized(sHeatStartMenu->sTopLeftWindowId, FONT_NARROW, gStringVar4, 0, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sHeatStartMenu->sTopLeftWindowId, COPYWIN_GFX);
}

bool8 InitHeatMenuStep(void)
{
    if (!IsOverworldLinkActive()) 
    {
        FreezeObjectEvents();
        PlayerFreeze();
        StopPlayerAvatar();
    }

    LockPlayerFieldControls();

    if (sHeatStartMenu == NULL) 
    {
        sHeatStartMenu = AllocZeroed(sizeof(struct HeatStartMenu));
        sHeatStartMenu->savedCallback = CB2_ReturnToFieldWithOpenMenu;
        sHeatStartMenu->loadState = 0;
        sHeatStartMenu->sStartClockWindowId = 0;
        sHeatStartMenu->flag = 0;
        sHeatStartMenu->lightLevel = GetFlashLevel() || gSaveBlock2Ptr->frontier.pyramidLightRadius;
        sHeatStartMenu->spriteIdPoketch = MAX_SPRITES;
        sHeatStartMenu->spriteIdPokedex = MAX_SPRITES;
        sHeatStartMenu->spriteIdParty = MAX_SPRITES;
        sHeatStartMenu->spriteIdBag = MAX_SPRITES;
        sHeatStartMenu->spriteIdTrainerCard = MAX_SPRITES;
        sHeatStartMenu->spriteIdSave = MAX_SPRITES;
        sHeatStartMenu->spriteIdOptions = MAX_SPRITES;
        sHeatStartMenu->spriteIdFlag = MAX_SPRITES;
    }

    return TRUE;
}

inline bool8 FadingComplete(void)
{
    if (!gPaletteFade.active && IsWeatherNotFadingIn())
        return TRUE;
    return FALSE;
}

void Task_HeatStartMenu_Init(u8 taskId)
{
    if (FadingComplete())
    {
        HeatStartMenu_Init();
        DestroyTask(taskId);
    }
}

void HeatStartMenu_Init(void) 
{
    if (sHeatStartMenu == NULL)
        InitHeatMenuStep();

    if (InBattlePyramid())
    {
        if (menuSelected == 255 || menuSelected == MENU_POKETCH || menuSelected == MENU_POKEDEX)
            menuSelected = MENU_PARTY;
        HeatStartMenu_LoadSprites();
        HeatStartMenu_BattlePyramid_CreateSprites();
        SetupHeatMenuCommonComponents();
        ShowBattlePyramidFloor();
    }
    else if (GetSafariZoneFlag() == TRUE)
    {
        if (menuSelected == 255 || menuSelected == MENU_POKETCH || menuSelected == MENU_SAVE) 
            menuSelected = MENU_FLAG;
        HeatStartMenu_LoadSprites();
        HeatStartMenu_SafariZone_CreateSprites();
        SetupHeatMenuCommonComponents();
        ShowSafariBallsWindow();
    }
    else
    { 
        if (FlagGet(FLAG_SYS_POKENAV_GET) == FALSE && menuSelected == 0)
            menuSelected = 255;
        if (menuSelected == MENU_FLAG) 
            menuSelected = MENU_POKEDEX;
        if (menuSelected == 255)
            SetSelectedMenu();
        HeatStartMenu_LoadSprites();
        HeatStartMenu_CreateSprites();
        SetupHeatMenuCommonComponents();
        if (GetDexNavFlag())
            ShowDexNavWindow();
    }

    CreateTask(Task_HeatStartMenu_HandleInput, 0);
}

static void SetupHeatMenuCommonComponents(void)
{
    HeatStartMenu_LoadBgGfx();
    HeatStartMenu_ShowTimeWindow();
    sHeatStartMenu->sMenuNameWindowId = AddWindow(&sWindowTemplate_MenuName);
    HeatStartMenu_UpdateMenuName();
}

static void HeatStartMenu_LoadSprites(void) 
{
    u32 index;
    LoadSpritePalette(sSpritePal_Icon);
    index = IndexOfSpritePaletteTag(TAG_ICON_PAL);
    LoadPalette(sIconPal, OBJ_PLTT_ID(index), PLTT_SIZE_4BPP); 
    LoadCompressedSpriteSheet(sSpriteSheet_Icon);
}

static inline u32 CreateSpriteBasedOnFlashLevel(const struct SpriteTemplate *template, s16 x, s16 y, u32 subpriority)
{
    if (sHeatStartMenu->lightLevel != 0)
        return CreateSpriteAndMask(template, x, y, subpriority);
    else
        return CreateSprite(template, x, y, subpriority);
}

static void HeatStartMenu_CreateSprites(void) 
{
    if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE) 
    {
        sHeatStartMenu->spriteIdPokedex     = CreateSpriteBasedOnFlashLevel(&gSpriteIconPokedex,     ICON_POS_X -1, ICON_POS_1 -2, 0);
        sHeatStartMenu->spriteIdParty       = CreateSpriteBasedOnFlashLevel(&gSpriteIconParty,       ICON_POS_X,    ICON_POS_2 -3, 0);
        sHeatStartMenu->spriteIdBag         = CreateSpriteBasedOnFlashLevel(&gSpriteIconBag,         ICON_POS_X,    ICON_POS_3 -2, 0);
        sHeatStartMenu->spriteIdPoketch     = CreateSpriteBasedOnFlashLevel(&gSpriteIconPoketch,     ICON_POS_X,    ICON_POS_4 +1, 0);
        sHeatStartMenu->spriteIdTrainerCard = CreateSpriteBasedOnFlashLevel(&gSpriteIconTrainerCard, ICON_POS_X,    ICON_POS_5,    0);
        sHeatStartMenu->spriteIdSave        = CreateSpriteBasedOnFlashLevel(&gSpriteIconSave,        ICON_POS_X,    ICON_POS_6,    0);
        sHeatStartMenu->spriteIdOptions     = CreateSpriteBasedOnFlashLevel(&gSpriteIconOptions,     ICON_POS_X,    ICON_POS_7,    0);
        return;
    }
    else if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE) 
    {
        sHeatStartMenu->spriteIdPokedex     = CreateSpriteBasedOnFlashLevel(&gSpriteIconPokedex,     ICON_POS_X -1, ICON_POS_1,    0);
        sHeatStartMenu->spriteIdParty       = CreateSpriteBasedOnFlashLevel(&gSpriteIconParty,       ICON_POS_X,    ICON_POS_2 -1, 0);
        sHeatStartMenu->spriteIdBag         = CreateSpriteBasedOnFlashLevel(&gSpriteIconBag,         ICON_POS_X,    ICON_POS_3 +1, 0);
        sHeatStartMenu->spriteIdTrainerCard = CreateSpriteBasedOnFlashLevel(&gSpriteIconTrainerCard, ICON_POS_X,    ICON_POS_4 +2, 0);
        sHeatStartMenu->spriteIdSave        = CreateSpriteBasedOnFlashLevel(&gSpriteIconSave,        ICON_POS_X,    ICON_POS_5 -1, 0);
        sHeatStartMenu->spriteIdOptions     = CreateSpriteBasedOnFlashLevel(&gSpriteIconOptions,     ICON_POS_X,    ICON_POS_6 -2, 0);
    return;
    }
    else if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE) 
    {
        sHeatStartMenu->spriteIdParty       = CreateSpriteBasedOnFlashLevel(&gSpriteIconParty,       ICON_POS_X,    ICON_POS_1,    0);
        sHeatStartMenu->spriteIdBag         = CreateSpriteBasedOnFlashLevel(&gSpriteIconBag,         ICON_POS_X,    ICON_POS_2 +1, 0);
        sHeatStartMenu->spriteIdTrainerCard = CreateSpriteBasedOnFlashLevel(&gSpriteIconTrainerCard, ICON_POS_X,    ICON_POS_3 +3, 0);
        sHeatStartMenu->spriteIdSave        = CreateSpriteBasedOnFlashLevel(&gSpriteIconSave,        ICON_POS_X,    ICON_POS_4 +1, 0);
        sHeatStartMenu->spriteIdOptions     = CreateSpriteBasedOnFlashLevel(&gSpriteIconOptions,     ICON_POS_X,    ICON_POS_5 -4, 0);
    return;
    }
    else 
    {
        sHeatStartMenu->spriteIdBag         = CreateSpriteBasedOnFlashLevel(&gSpriteIconBag,         ICON_POS_X,    ICON_POS_1,    0);
        sHeatStartMenu->spriteIdTrainerCard = CreateSpriteBasedOnFlashLevel(&gSpriteIconTrainerCard, ICON_POS_X,    ICON_POS_2 +1, 0);
        sHeatStartMenu->spriteIdSave        = CreateSpriteBasedOnFlashLevel(&gSpriteIconSave,        ICON_POS_X,    ICON_POS_3 +3, 0);
        sHeatStartMenu->spriteIdOptions     = CreateSpriteBasedOnFlashLevel(&gSpriteIconOptions,     ICON_POS_X,    ICON_POS_4 +1, 0);
    }
}

static void HeatStartMenu_SafariZone_CreateSprites(void) 
{
    sHeatStartMenu->spriteIdFlag        = CreateSpriteBasedOnFlashLevel(&gSpriteIconFlag,        ICON_POS_X,    ICON_POS_1, 0);
    sHeatStartMenu->spriteIdPokedex     = CreateSpriteBasedOnFlashLevel(&gSpriteIconPokedex,     ICON_POS_X -1, ICON_POS_2, 0);
    sHeatStartMenu->spriteIdParty       = CreateSpriteBasedOnFlashLevel(&gSpriteIconParty,       ICON_POS_X,    ICON_POS_3, 0);
    sHeatStartMenu->spriteIdBag         = CreateSpriteBasedOnFlashLevel(&gSpriteIconBag,         ICON_POS_X,    ICON_POS_4, 0);
    sHeatStartMenu->spriteIdTrainerCard = CreateSpriteBasedOnFlashLevel(&gSpriteIconTrainerCard, ICON_POS_X,    ICON_POS_5, 0);
    sHeatStartMenu->spriteIdOptions     = CreateSpriteBasedOnFlashLevel(&gSpriteIconOptions,     ICON_POS_X,    ICON_POS_6, 0);
}

static void HeatStartMenu_BattlePyramid_CreateSprites(void)
{
    sHeatStartMenu->spriteIdParty       = CreateSpriteBasedOnFlashLevel(&gSpriteIconParty,       ICON_POS_X,    ICON_POS_1, 0);
    sHeatStartMenu->spriteIdBag         = CreateSpriteBasedOnFlashLevel(&gSpriteIconBag,         ICON_POS_X,    ICON_POS_2, 0);
    sHeatStartMenu->spriteIdTrainerCard = CreateSpriteBasedOnFlashLevel(&gSpriteIconTrainerCard, ICON_POS_X,    ICON_POS_3, 0);
    sHeatStartMenu->spriteIdSave        = CreateSpriteBasedOnFlashLevel(&gSpriteIconSave,        ICON_POS_X,    ICON_POS_4, 0);
    sHeatStartMenu->spriteIdFlag        = CreateSpriteBasedOnFlashLevel(&gSpriteIconFlag,        ICON_POS_X,    ICON_POS_5, 0);
    sHeatStartMenu->spriteIdOptions     = CreateSpriteBasedOnFlashLevel(&gSpriteIconOptions,     ICON_POS_X,    ICON_POS_6, 0);
}

static void HeatStartMenu_LoadBgGfx(void) 
{
    u8* buf = GetBgTilemapBuffer(0);
    LoadBgTilemap(0, 0, 0, 0);
    DecompressAndCopyTileDataToVram(0, sStartMenuTiles, 0, 0, 0);
    if (GetSafariZoneFlag() || InBattlePyramid())
        DecompressDataWithHeaderWram(sStartMenuTilemapSafari, buf);
    else if (GetDexNavFlag())
        DecompressDataWithHeaderWram(sStartMenuTilemapDexNav, buf);
    else
        DecompressDataWithHeaderWram(sStartMenuTilemap, buf);

    LoadPalette(gStandardMenuPalette, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
    LoadPalette(sStartMenuPalette, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
    ScheduleBgCopyTilemapToVram(0);
}

static void HeatStartMenu_ShowTimeWindow(void)
{
    u8 analogHour;

	RtcCalcLocalTime();
      // print window
    sHeatStartMenu->sStartClockWindowId = AddWindow(&sWindowTemplate_StartClock);
    FillWindowPixelBuffer(sHeatStartMenu->sStartClockWindowId, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sHeatStartMenu->sStartClockWindowId);

    analogHour = (gLocalTime.hours >= 13 && gLocalTime.hours <= 24) ? gLocalTime.hours - 12 : gLocalTime.hours;

    if (gLocalTime.dayOfWeek <= DAY_SATURDAY)
        StringCopy(gStringVar3, gDayNameStringsTable[gLocalTime.dayOfWeek]);
    else
        StringCopy(gStringVar3, gText_None);
    ConvertIntToDecimalStringN(gStringVar1, gLocalTime.hours, STR_CONV_MODE_LEADING_ZEROS, 2);
	ConvertIntToDecimalStringN(gStringVar2, gLocalTime.minutes, STR_CONV_MODE_LEADING_ZEROS, 2);
	ConvertIntToDecimalStringN(gStringVar1, analogHour, STR_CONV_MODE_LEADING_ZEROS, 2);
    
	StringExpandPlaceholders(gStringVar4, gText_CurrentTime);
    if (gLocalTime.hours >= 13 && gLocalTime.hours <= 24)
        StringExpandPlaceholders(gStringVar4, gText_CurrentTimePM); 
    else
        StringExpandPlaceholders(gStringVar4, gText_CurrentTimeAM);  
    
	AddTextPrinterParameterized(sHeatStartMenu->sStartClockWindowId, 1, gStringVar4, 0, 1, 0xFF, NULL);
	CopyWindowToVram(sHeatStartMenu->sStartClockWindowId, COPYWIN_GFX);
}

static void HeatStartMenu_UpdateClockDisplay(void)
{
    u8 analogHour;

	RtcCalcLocalTime();
    analogHour = (gLocalTime.hours >= 13 && gLocalTime.hours <= 24) ? gLocalTime.hours - 12 : gLocalTime.hours;

    if (gLocalTime.dayOfWeek <= DAY_SATURDAY)
        StringCopy(gStringVar3, gDayNameStringsTable[gLocalTime.dayOfWeek]);
    else
        StringCopy(gStringVar3, gText_None);
    ConvertIntToDecimalStringN(gStringVar1, gLocalTime.hours, STR_CONV_MODE_LEADING_ZEROS, 2);
	ConvertIntToDecimalStringN(gStringVar2, gLocalTime.minutes, STR_CONV_MODE_LEADING_ZEROS, 2);
	ConvertIntToDecimalStringN(gStringVar1, analogHour, STR_CONV_MODE_LEADING_ZEROS, 2);
    if (gLocalTime.hours == 0)
	    ConvertIntToDecimalStringN(gStringVar1, 12, STR_CONV_MODE_LEADING_ZEROS, 2);
    if (gLocalTime.hours == 12)
	    ConvertIntToDecimalStringN(gStringVar1, 12, STR_CONV_MODE_LEADING_ZEROS, 2);

	if (gLocalTime.seconds % 2)
	{
        StringExpandPlaceholders(gStringVar4, gText_CurrentTime);
        if (gLocalTime.hours >= 12 && gLocalTime.hours <= 24)
            StringExpandPlaceholders(gStringVar4, gText_CurrentTimePM); 
        else
            StringExpandPlaceholders(gStringVar4, gText_CurrentTimeAM);  
    }
	else
	{
        StringExpandPlaceholders(gStringVar4, gText_CurrentTimeOff);
        if (gLocalTime.hours >= 12 && gLocalTime.hours <= 24)
            StringExpandPlaceholders(gStringVar4, gText_CurrentTimePMOff); 
        else
            StringExpandPlaceholders(gStringVar4, gText_CurrentTimeAMOff);  
    }
    
	AddTextPrinterParameterized(sHeatStartMenu->sStartClockWindowId, 1, gStringVar4, 0, 1, 0xFF, NULL);
	CopyWindowToVram(sHeatStartMenu->sStartClockWindowId, COPYWIN_GFX);
}

static const u8 gText_Poketch[] = _("  PokeNav");
static const u8 gText_Pokedex[] = _("  Pokédex");
static const u8 gText_Party[]   = _("    Party ");
static const u8 gText_Bag[]     = _("      Bag  ");
static const u8 gText_Trainer[] = _("   Trainer");
static const u8 gText_Save[]    = _("     Save  ");
static const u8 gText_Options[] = _("   Options");
static const u8 gText_Flag[]    = _("   Retire");

static void HeatStartMenu_UpdateMenuName(void) 
{
    
    FillWindowPixelBuffer(sHeatStartMenu->sMenuNameWindowId, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sHeatStartMenu->sMenuNameWindowId);

    switch(menuSelected) 
    {
    case MENU_POKETCH:
        AddTextPrinterParameterized(sHeatStartMenu->sMenuNameWindowId, 1, gText_Poketch, 1, 0, 0xFF, NULL);
        break;
    case MENU_POKEDEX:
        AddTextPrinterParameterized(sHeatStartMenu->sMenuNameWindowId, 1, gText_Pokedex, 1, 0, 0xFF, NULL);
        break;
    case MENU_PARTY:
        AddTextPrinterParameterized(sHeatStartMenu->sMenuNameWindowId, 1, gText_Party, 1, 0, 0xFF, NULL);
        break;
    case MENU_BAG:
        AddTextPrinterParameterized(sHeatStartMenu->sMenuNameWindowId, 1, gText_Bag, 1, 0, 0xFF, NULL);
        break;
    case MENU_TRAINER_CARD:
        AddTextPrinterParameterized(sHeatStartMenu->sMenuNameWindowId, 1, gText_Trainer, 1, 0, 0xFF, NULL);
        break;
    case MENU_SAVE:
        AddTextPrinterParameterized(sHeatStartMenu->sMenuNameWindowId, 1, gText_Save, 1, 0, 0xFF, NULL);
        break;
    case MENU_OPTIONS:
        AddTextPrinterParameterized(sHeatStartMenu->sMenuNameWindowId, 1, gText_Options, 1, 0, 0xFF, NULL);
        break;
    case MENU_FLAG:
        AddTextPrinterParameterized(sHeatStartMenu->sMenuNameWindowId, 1, gText_Flag, 1, 0, 0xFF, NULL);
        break;
    }
    CopyWindowToVram(sHeatStartMenu->sMenuNameWindowId, COPYWIN_GFX);
}

static inline void FreeSpriteOamMatrixBasedOnFlashLevel(u32 spriteIds)
{
    if (spriteIds == MAX_SPRITES)
        return;

    if (sHeatStartMenu->lightLevel != 0)
        FreeSpriteAndMaskOamMatrix(spriteIds);
    else
        FreeSpriteOamMatrix(&gSprites[spriteIds]);
}

static inline void DestroySpriteBasedOnFlashLevel(u32 spriteIds)
{
    if (spriteIds == MAX_SPRITES)
        return;

    if (sHeatStartMenu->lightLevel != 0)
        DestroySpriteAndMask(spriteIds);
    else
        DestroySprite(&gSprites[spriteIds]);
}

static void HeatStartMenu_ExitAndClearTilemap(void) 
{
    u32 i;
    u8 *buf = GetBgTilemapBuffer(0);
 
    FillWindowPixelBuffer(sHeatStartMenu->sMenuNameWindowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(sHeatStartMenu->sStartClockWindowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    ClearWindowTilemap(sHeatStartMenu->sMenuNameWindowId);
    ClearWindowTilemap(sHeatStartMenu->sStartClockWindowId);

    CopyWindowToVram(sHeatStartMenu->sMenuNameWindowId, COPYWIN_GFX);
    CopyWindowToVram(sHeatStartMenu->sStartClockWindowId, COPYWIN_GFX);

    RemoveWindow(sHeatStartMenu->sStartClockWindowId);
    RemoveWindow(sHeatStartMenu->sMenuNameWindowId);

    if (GetDexNavFlag() || GetSafariZoneFlag() || InBattlePyramid())
    {
        FillWindowPixelBuffer(sHeatStartMenu->sTopLeftWindowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
        ClearWindowTilemap(sHeatStartMenu->sTopLeftWindowId);
        CopyWindowToVram(sHeatStartMenu->sTopLeftWindowId, COPYWIN_GFX);
        RemoveWindow(sHeatStartMenu->sTopLeftWindowId);
    }

    for(i = 0; i < 2048; i++)
        buf[i] = 0;
    ScheduleBgCopyTilemapToVram(0);

    FreeSpriteOamMatrixBasedOnFlashLevel(sHeatStartMenu->spriteIdPoketch);
    FreeSpriteOamMatrixBasedOnFlashLevel(sHeatStartMenu->spriteIdPokedex);
    FreeSpriteOamMatrixBasedOnFlashLevel(sHeatStartMenu->spriteIdParty);
    FreeSpriteOamMatrixBasedOnFlashLevel(sHeatStartMenu->spriteIdBag);
    FreeSpriteOamMatrixBasedOnFlashLevel(sHeatStartMenu->spriteIdTrainerCard);
    FreeSpriteOamMatrixBasedOnFlashLevel(sHeatStartMenu->spriteIdSave);
    FreeSpriteOamMatrixBasedOnFlashLevel(sHeatStartMenu->spriteIdOptions);
    FreeSpriteOamMatrixBasedOnFlashLevel(sHeatStartMenu->spriteIdFlag);

    DestroySpriteBasedOnFlashLevel(sHeatStartMenu->spriteIdPoketch);
    DestroySpriteBasedOnFlashLevel(sHeatStartMenu->spriteIdPokedex);
    DestroySpriteBasedOnFlashLevel(sHeatStartMenu->spriteIdParty);
    DestroySpriteBasedOnFlashLevel(sHeatStartMenu->spriteIdBag);
    DestroySpriteBasedOnFlashLevel(sHeatStartMenu->spriteIdTrainerCard);
    DestroySpriteBasedOnFlashLevel(sHeatStartMenu->spriteIdSave);
    DestroySpriteBasedOnFlashLevel(sHeatStartMenu->spriteIdOptions);
    DestroySpriteBasedOnFlashLevel(sHeatStartMenu->spriteIdFlag);

    if (sHeatStartMenu != NULL) 
    {
        FreeSpriteTilesByTag(TAG_ICON_GFX);  
        Free(sHeatStartMenu);
        sHeatStartMenu = NULL;
    }

    ScriptUnfreezeObjectEvents();  
    UnlockPlayerFieldControls();
}

static void DoCleanUpAndChangeCallback(MainCallback callback) 
{
    if (!gPaletteFade.active) 
    {
        DestroyTask(FindTaskIdByFunc(Task_HeatStartMenu_HandleInput));
        PlayRainStoppingSoundEffect();
        HeatStartMenu_ExitAndClearTilemap();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(callback);
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;
    }
}

static void DoCleanUpAndOpenTrainerCard(void) 
{
    if (!gPaletteFade.active) 
    {
        PlayRainStoppingSoundEffect();
        HeatStartMenu_ExitAndClearTilemap();
        CleanupOverworldWindowsAndTilemaps();
        if (IsOverworldLinkActive() || InUnionRoom())
        {
            ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
        }
        else if (FlagGet(FLAG_SYS_FRONTIER_PASS))
        {
            ShowFrontierPass(CB2_ReturnToFieldWithOpenMenu); // Display frontier pass
        }
        else
        {
            ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
        }
        DestroyTask(FindTaskIdByFunc(Task_HeatStartMenu_HandleInput));
    }
}

static void DoCleanUpAndOpenDexNav(void)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(FindTaskIdByFunc(Task_HeatStartMenu_HandleInput));
        PlayRainStoppingSoundEffect();
        HeatStartMenu_ExitAndClearTilemap();
        CreateTask(Task_OpenDexNavFromStartMenu, 0);
    }
}

static u8 RunSaveCallback(void)
{
    // True if text is still printing
    if (RunTextPrintersAndIsPrinter0Active() == TRUE)
    {
        return SAVE_IN_PROGRESS;
    }

    return sSaveDialogCallback();
}

static void SaveStartTimer(void)
{
    sSaveDialogTimer = 60;
}

static bool8 SaveSuccesTimer(void)
{
    sSaveDialogTimer--;


    if (JOY_HELD(A_BUTTON) || JOY_HELD(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        return TRUE;
    }
    if (sSaveDialogTimer == 0)
    {
        return TRUE;
    }

    return FALSE;
}

static bool8 SaveErrorTimer(void)
{
    if (sSaveDialogTimer != 0)
    {
        sSaveDialogTimer--;
    }
    else if (JOY_HELD(A_BUTTON))
    {
        return TRUE;
    }

    return FALSE;
}

static u8 SaveReturnSuccessCallback(void)
{
    if (!IsSEPlaying() && SaveSuccesTimer())
    {
        HideSaveInfoWindow();
        return SAVE_SUCCESS;
    }
    else
    {
        return SAVE_IN_PROGRESS;
    }
}

static u8 SaveSuccessCallback(void)
{
    if (!IsTextPrinterActive(0))
    {
        PlaySE(SE_SAVE);
        sSaveDialogCallback = SaveReturnSuccessCallback;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveReturnErrorCallback(void)
{
    if (!SaveErrorTimer())
    {
        return SAVE_IN_PROGRESS;
    }
    else
    {
        HideSaveInfoWindow();
        return SAVE_ERROR;
    }
}

static u8 SaveErrorCallback(void)
{
    if (!IsTextPrinterActive(0))
    {
        PlaySE(SE_BOO);
        sSaveDialogCallback = SaveReturnErrorCallback;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveDoSaveCallback(void)
{
    u8 saveStatus;

    IncrementGameStat(GAME_STAT_SAVED_GAME);
    PausePyramidChallenge();

    if (gDifferentSaveFile == TRUE)
    {
        saveStatus = TrySavingData(SAVE_OVERWRITE_DIFFERENT_FILE);
        gDifferentSaveFile = FALSE;
    }
    else
    {
        saveStatus = TrySavingData(SAVE_NORMAL);
    }

    if (saveStatus == SAVE_STATUS_OK)
        ShowSaveMessage(gText_PlayerSavedGame, SaveSuccessCallback);
    else
        ShowSaveMessage(gText_SaveError, SaveErrorCallback);

    SaveStartTimer();
    return SAVE_IN_PROGRESS;
}

static void HideSaveInfoWindow(void) 
{
    ClearStdWindowAndFrame(sSaveInfoWindowId, FALSE);
    RemoveWindow(sSaveInfoWindowId);
}

static void HideSaveMessageWindow(void) 
{
    ClearDialogWindowAndFrame(0, TRUE);
}

static u8 SaveOverwriteInputCallback(void)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes
        sSaveDialogCallback = SaveSavingMessageCallback;
        return SAVE_IN_PROGRESS;
    case MENU_B_PRESSED:
    case 1: // No
        HideSaveInfoWindow();
        HideSaveMessageWindow();
        return SAVE_CANCELED;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveConfirmOverwriteDefaultNoCallback(void)
{
    DisplayYesNoMenuWithDefault(1); // Show Yes/No menu (No selected as default)
    sSaveDialogCallback = SaveOverwriteInputCallback;
    return SAVE_IN_PROGRESS;
}

static void ShowSaveMessage(const u8 *message, u8 (*saveCallback)(void)) 
{
    StringExpandPlaceholders(gStringVar4, message);
    LoadMessageBoxAndFrameGfx(0, TRUE);
    AddTextPrinterForMessage_2(TRUE);
    sSaveDialogCallback = saveCallback;
}

static u8 SaveFileExistsCallback(void)
{
    if (gDifferentSaveFile == TRUE)
    {
        ShowSaveMessage(gText_DifferentSaveFile, SaveConfirmOverwriteDefaultNoCallback);
    }
    else
    {
        sSaveDialogCallback = SaveSavingMessageCallback;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveSavingMessageCallback(void) 
{
    ShowSaveMessage(gText_SavingDontTurnOff, SaveDoSaveCallback);
    return SAVE_IN_PROGRESS;
}

static u8 SaveConfirmInputCallback(void)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes
        switch (gSaveFileStatus)
        {
        case SAVE_STATUS_EMPTY:
        case SAVE_STATUS_CORRUPT:
            if (gDifferentSaveFile == FALSE)
            {
                sSaveDialogCallback = SaveFileExistsCallback;
                return SAVE_IN_PROGRESS;
            }

            sSaveDialogCallback = SaveSavingMessageCallback;
            return SAVE_IN_PROGRESS;
        default:
            sSaveDialogCallback = SaveFileExistsCallback;
            return SAVE_IN_PROGRESS;
        }
    case MENU_B_PRESSED: // No break, thats smart 
    case 1: // No
        HideSaveInfoWindow();
        HideSaveMessageWindow();
        return SAVE_CANCELED;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveYesNoCallback(void) 
{
    DisplayYesNoMenuDefaultYes(); // Show Yes/No menu
    sSaveDialogCallback = SaveConfirmInputCallback;
    return SAVE_IN_PROGRESS;
}


static void ShowSaveInfoWindow(void) 
{
    struct WindowTemplate saveInfoWindow = sSaveInfoWindowTemplate;
    u8 gender;
    u8 color;
    u32 xOffset;
    u32 yOffset;

    if (!FlagGet(FLAG_SYS_POKEDEX_GET))
        saveInfoWindow.height -= 2;
    
    sSaveInfoWindowId = AddWindow(&saveInfoWindow);
    DrawStdWindowFrame(sSaveInfoWindowId, FALSE);

    gender = gSaveBlock2Ptr->playerGender;
    color = TEXT_COLOR_RED;  // Red when female, blue when male.

    if (gender == MALE)
        color = TEXT_COLOR_BLUE;

    // Print region name
    yOffset = 1;
    BufferSaveMenuText(SAVE_MENU_LOCATION, gStringVar4, TEXT_COLOR_GREEN);
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, 0, yOffset, TEXT_SKIP_DRAW, NULL);

    // Print player name
    yOffset += 16;
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingPlayer, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    BufferSaveMenuText(SAVE_MENU_NAME, gStringVar4, color);
    xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
    PrintPlayerNameOnWindow(sSaveInfoWindowId, gStringVar4, xOffset, yOffset);

    // Print badge count
    yOffset += 16;
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingBadges, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    BufferSaveMenuText(SAVE_MENU_BADGES, gStringVar4, color);
    xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);

    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
    {
        // Print pokedex count
        yOffset += 16;
        AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingPokedex, 0, yOffset, TEXT_SKIP_DRAW, NULL);
        BufferSaveMenuText(SAVE_MENU_CAUGHT, gStringVar4, color);
        xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
        AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);
    }

    // Print play time
    yOffset += 16;
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingTime, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    BufferSaveMenuText(SAVE_MENU_PLAY_TIME, gStringVar4, color);
    xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);

    CopyWindowToVram(sSaveInfoWindowId, COPYWIN_GFX);
}

static u8 SaveConfirmSaveCallback(void) 
{
    ClearStdWindowAndFrame(GetStartMenuWindowId(), FALSE);
    //RemoveStartMenuWindow();
    ShowSaveInfoWindow();

    if (InBattlePyramid()) {
        ShowSaveMessage(gText_BattlePyramidConfirmRest, SaveYesNoCallback);
    } else {
        ShowSaveMessage(gText_ConfirmSave, SaveYesNoCallback);
    }
    return SAVE_IN_PROGRESS;
}

static void InitSave(void)
{
    SaveMapView();
    sSaveDialogCallback = SaveConfirmSaveCallback;
}

static void Task_HandleSave(u8 taskId) 
{
    switch (RunSaveCallback()) 
    {
    case SAVE_IN_PROGRESS:
        break;
    case SAVE_SUCCESS:
    case SAVE_CANCELED: // Back to start menu
        ClearDialogWindowAndFrameToTransparent(0, TRUE);
        ScriptUnfreezeObjectEvents();  
        UnlockPlayerFieldControls();
        DestroyTask(taskId);
        break;
    case SAVE_ERROR:    // Close start menu
        ClearDialogWindowAndFrameToTransparent(0, TRUE);
        ScriptUnfreezeObjectEvents();
        UnlockPlayerFieldControls();
        SoftResetInBattlePyramid();
        DestroyTask(taskId);
        break;
    }
}

#define STD_WINDOW_BASE_TILE_NUM 0x214
#define STD_WINDOW_PALETTE_NUM 14

static void DoCleanUpAndStartSaveMenu(void) 
{
    if (!gPaletteFade.active) 
    {
        HeatStartMenu_ExitAndClearTilemap();
        FreezeObjectEvents();
        LoadUserWindowBorderGfx(sSaveInfoWindowId, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
        LockPlayerFieldControls();
        DestroyTask(FindTaskIdByFunc(Task_HeatStartMenu_HandleInput));
        InitSave();
        CreateTask(Task_HandleSave, 0x80);
    }
}

static void DoCleanUpAndStartSafariZoneRetire(void) 
{
    if (!gPaletteFade.active) 
    {
        HeatStartMenu_ExitAndClearTilemap();
        FreezeObjectEvents();
        LockPlayerFieldControls();
        DestroyTask(FindTaskIdByFunc(Task_HeatStartMenu_HandleInput));
        SafariZoneRetirePrompt();
    }
}

static void DoCleanUpAndStartBattlePyramidRetire(void)
{
    if (!gPaletteFade.active) 
    {
        HeatStartMenu_ExitAndClearTilemap();
        FreezeObjectEvents();
        LockPlayerFieldControls();
        DestroyTask(FindTaskIdByFunc(Task_HeatStartMenu_HandleInput));
        SafariZoneRetirePrompt();
    }
}
 
static void HeatStartMenu_OpenMenu(void) 
{
    switch (menuSelected) 
    {
    case MENU_POKETCH:
        DoCleanUpAndChangeCallback(CB2_InitPokeNav);
        break;
    case MENU_POKEDEX:
        DoCleanUpAndChangeCallback(CB2_OpenPokedex);
        break;
    case MENU_PARTY: 
        DoCleanUpAndChangeCallback(CB2_PartyMenuFromStartMenu);
        break;
    case MENU_BAG:
        if (InBattlePyramid())
            DoCleanUpAndChangeCallback(CB2_PyramidBagMenuFromStartMenu);
        else
            DoCleanUpAndChangeCallback(CB2_BagMenuFromStartMenu);
        break;
    case MENU_TRAINER_CARD:
        DoCleanUpAndOpenTrainerCard();
        break;
    case MENU_OPTIONS:
        DoCleanUpAndChangeCallback(CB2_InitOptionMenu);
        break;
    case MENU_SAVE:
        DoCleanUpAndStartSaveMenu();
        break;
    case MENU_FLAG:
        if (InBattlePyramid())
            DoCleanUpAndStartBattlePyramidRetire();
        else
            DoCleanUpAndStartSafariZoneRetire();
        break;
    }
}

static void Task_HeatStartMenu_HandleInput(u8 taskId)
{
    enum PlayerLocation location;
    HeatStartMenu_UpdateClockDisplay();
    
    if (InBattlePyramid())
        location = LOCATION_PYRAMID;
    else if (GetSafariZoneFlag() == TRUE)
        location = LOCATION_SAFARI;
    else
        location = LOCATION_OVERWORLD;

    if (sHeatStartMenu->loadState == 0 && FadingComplete())
    {
        u32 index = IndexOfSpritePaletteTag(TAG_ICON_PAL);
        LoadPalette(sIconPal, OBJ_PLTT_ID(index), PLTT_SIZE_4BPP);
    }

    if (JOY_NEW(A_BUTTON))
    {
        if (sHeatStartMenu->loadState == 0)
        {
            if (menuSelected != MENU_FLAG && menuSelected != MENU_SAVE)
            {
                FadeScreen(FADE_TO_BLACK, 0);
            }
            sHeatStartMenu->loadState = 1;
        }
    }
    else if (JOY_NEW(B_BUTTON) && sHeatStartMenu->loadState == 0)
    {
        PlaySE(SE_SELECT);
        HeatStartMenu_ExitAndClearTilemap();
        DestroyTask(taskId);
    }
    else if (JOY_NEW(L_BUTTON) && sHeatStartMenu->loadState == 0 
        && GetDexNavFlag() && location == LOCATION_OVERWORLD)
    {
        FadeScreen(FADE_TO_BLACK, 0);
        sHeatStartMenu->loadState = 2;
    }
    else if (gMain.newKeys & DPAD_DOWN && sHeatStartMenu->loadState == 0)
    {
        HeatStartMenu_HandleInput_Move(1, location);
    }
    else if (gMain.newKeys & DPAD_UP && sHeatStartMenu->loadState == 0)
    {
        HeatStartMenu_HandleInput_Move(-1, location);
    }
    
    if (sHeatStartMenu->loadState == 1)
    {
        HeatStartMenu_OpenMenu();
    }
    else if (sHeatStartMenu->loadState == 2)
    {
        DoCleanUpAndOpenDexNav();
    }
}

static void HeatStartMenu_HandleInput_Move(int direction, enum PlayerLocation location)
{
    const u8 *menu;
    int menuSize;
    int index;

    sHeatStartMenu->flag = 0;
    switch (location)
    {
        case LOCATION_SAFARI:
            menu = sSafariMenu;
            menuSize = sizeof(sSafariMenu);
            break;
        case LOCATION_PYRAMID:
            menu = sPyramidMenu;
            menuSize = sizeof(sPyramidMenu);
            break;
        default:
            menu = sOverworldMenu;
            menuSize = sizeof(sOverworldMenu);
            break;
    }

    for (index = 0; index < menuSize; index++)
    {
        if (menu[index] == menuSelected)
            break;
    }

    if (direction == 1) // Moving down
    {
        if (index < menuSize - 1)
            menuSelected = menu[index + 1];
        else
            menuSelected = menu[0];
        PlaySE(SE_SELECT);
    }
    else if (direction == -1) // Moving up
    {
        if (index > 0)
            menuSelected = menu[index - 1];
        else
            menuSelected = menu[menuSize - 1];
        PlaySE(SE_SELECT);
    }
    
    HeatStartMenu_UpdateMenuName();
}
