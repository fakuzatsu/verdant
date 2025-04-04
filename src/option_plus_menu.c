#include "global.h"
#include "event_data.h"
#include "field_effect.h"
#include "field_weather.h"
#include "field_screen_effect.h"
#include "option_plus_menu.h"
#include "overworld.h"
#include "main.h"
#include "menu.h"
#include "naming_screen.h"
#include "scanline_effect.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "malloc.h"
#include "bg.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "window.h"
#include "text.h"
#include "text_window.h"
#include "international_string_util.h"
#include "item_menu.h"
#include "script.h"
#include "strings.h"
#include "string_util.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"
#include "menu_helpers.h"
#include "decompress.h"

// Menus
enum
{
    MENU_MAIN,
    MENU_DIFFICULTY,
    MENU_RANDOM,
    MENU_COUNT,
};

// Menu items
enum
{
    MENUITEM_MAIN_TEXTSPEED,
    MENUITEM_MAIN_SPEEDUP,
    MENUITEM_MAIN_DAMAGE_NUMS,
    MENUITEM_MAIN_BATTLESCENE,
    MENUITEM_MAIN_MATCHCALL,
    MENUITEM_MAIN_BUTTONMODE,
    MENUITEM_MAIN_UNIT_SYSTEM,
    MENUITEM_MAIN_SOUND,
    MENUITEM_MAIN_CANCEL,
    MENUITEM_MAIN_COUNT,
};

enum
{
    MENUITEM_DIFFICULTY_MODE,
    MENUITEM_DIFFICULTY_LEVELCAPS,
    MENUITEM_DIFFICULTY_VGC_DRAFT,
    MENUITEM_DIFFICULTY_BATTLESTYLE,
    MENUITEM_DIFFICULTY_CANCEL,
    MENUITEM_DIFFICULTY_COUNT,
};

enum
{
    MENUITEM_RANDOM_WILD,
    MENUITEM_RANDOM_TRAIN,
    MENUITEM_RANDOM_RANDABIL,
    MENUITEM_RANDOM_RNGSEED,
    MENUITEM_RANDOM_CANCEL,
    MENUITEM_RANDOM_COUNT,
};

// Window Ids
enum
{
    WIN_TOPBAR,
    WIN_OPTIONS,
    WIN_DESCRIPTION
};

static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    {//WIN_TOPBAR
        .bg = 1,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    {//WIN_OPTIONS
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 3,
        .width = 26,
        .height = 10,
        .paletteNum = 4,
        .baseBlock = 62
    },
    {//WIN_DESCRIPTION
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 1,
        .baseBlock = 500
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
    {
       .bg = 0,
       .charBaseIndex = 1,
       .mapBaseIndex = 30,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
    },
    {
       .bg = 1,
       .charBaseIndex = 1,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
    },
    {
       .bg = 2,
       .charBaseIndex = 0,
       .mapBaseIndex = 29,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
    },
    {
       .bg = 3,
       .charBaseIndex = 3,
       .mapBaseIndex = 27,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 2,
    },
};

struct OptionMenu
{
    u8 submenu;
    u8 sel[MENUITEM_MAIN_COUNT];
    u8 sel_difficulty[MENUITEM_DIFFICULTY_COUNT];
    u8 sel_random[MENUITEM_RANDOM_COUNT];
    int menuCursor[MENU_COUNT];
    int visibleCursor[MENU_COUNT];
    u8 arrowTaskId;
    u8 gfxLoadState;
};

#define Y_DIFF 16 // Difference in pixels between items.
#define OPTIONS_ON_SCREEN 5
#define NUM_OPTIONS_FROM_BORDER 1

// local functions
static void MainCB2(void);
static void VBlankCB(void);
static void DrawTopBarText(void); //top Option text
static void DrawLeftSideOptionText(int selection, int y);
static void DrawRightSideChoiceText(const u8 *str, int x, int y, bool8 choosen, bool8 active);
static void DrawOptionMenuTexts(void); //left side text;
static void DrawChoices(u32 id, int y); //right side draw function
static void HighlightOptionMenuItem(void);
static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void ScrollMenu(int direction);
static void ScrollAll(int direction); // to bottom or top
static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3);
static int XOptions_ProcessInput(int x, int selection);
static int ProcessInput_Options_Two(int selection);
static int ProcessInput_Options_Three(int selection);
static int ProcessInput_Options_Four(int selection);
static const u8 *const OptionTextDescription(void);
static const u8 *const OptionTextRight(u8 menuItem);
static u8 MenuItemCount(void);
static u8 MenuItemCancel(void);
static void DrawDescriptionText(void);
static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style, bool8 active);
static void DrawChoices_Options_Four(const u8 *const *const strings, int selection, int y, bool8 active);
static void ReDrawAll(void);
static void DrawChoices_TextSpeed(int selection, int y);
static void DrawChoices_Speedup(int selection, int y);
static void DrawChoices_Difficulty(int selection, int y);
static void DrawChoices_LevelCaps(int selection, int y);
static void DrawChoices_DamageNums(int selection, int y);
static void DrawChoices_BattleScene(int selection, int y);
static void DrawChoices_BattleStyle(int selection, int y);
static void DrawChoices_VGCDraft(int selection, int y);
static void DrawChoices_Sound(int selection, int y);
static void DrawChoices_ButtonMode(int selection, int y);
static void DrawChoices_UnitSystem(int selection, int y);
static void DrawChoices_MatchCall(int selection, int y);
static void DrawChoices_RandWild(int selection, int y);
static void DrawChoices_RandTrain(int selection, int y);
static void DrawChoices_RandAbil(int selection, int y);
static void DrawBgWindowFrames(void);
static void DoRngSeedScreen(void);
static void CB2_HandleGivenSeed(void);
static void MapPostLoadHook_ReturnToSeedChange(void);
static void Task_ReturnToSeedChange(u8 taskId);

// EWRAM vars
EWRAM_DATA static struct OptionMenu *sOptions = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg3TilemapBuffer = NULL;

// const data
static const u8 sEqualSignGfx[] = INCBIN_U8("graphics/interface/option_menu_equals_sign.4bpp"); // note: this is only used in the Japanese release
static const u16 sOptionMenuBg_Pal[] = {RGB(17, 18, 31)};
static const u16 sOptionMenuText_Pal[] = INCBIN_U16("graphics/interface/option_menu_text_custom.gbapal");

static const u32 sOptionsPlusTiles[] = INCBIN_U32("graphics/ui_options_plus/options_plus_tiles.4bpp.smol");
static const u16 sOptionsPlusPalette[] = INCBIN_U16("graphics/ui_options_plus/options_plus_tiles.gbapal");
static const u32 sOptionsPlusTilemap[] = INCBIN_U32("graphics/ui_options_plus/options_plus_tiles.bin.lz");

#define TEXT_COLOR_OPTIONS_WHITE                1
#define TEXT_COLOR_OPTIONS_GRAY_FG              2
#define TEXT_COLOR_OPTIONS_GRAY_SHADOW          3
#define TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG        4
#define TEXT_COLOR_OPTIONS_ORANGE_FG            5
#define TEXT_COLOR_OPTIONS_ORANGE_SHADOW        6
#define TEXT_COLOR_OPTIONS_RED_FG               7
#define TEXT_COLOR_OPTIONS_RED_SHADOW           8
#define TEXT_COLOR_OPTIONS_GREEN_FG             9
#define TEXT_COLOR_OPTIONS_GREEN_SHADOW         10
#define TEXT_COLOR_OPTIONS_GREEN_DARK_FG        11
#define TEXT_COLOR_OPTIONS_GREEN_DARK_SHADOW    12
#define TEXT_COLOR_OPTIONS_RED_DARK_FG          13
#define TEXT_COLOR_OPTIONS_RED_DARK_SHADOW      14

#define TEXT_COLOR_OPTIONS_PAL4_BLACK           1
#define TEXT_COLOR_OPTIONS_PAL4_DARK_GREY       2
#define TEXT_COLOR_OPTIONS_PAL4_GREY            4
#define TEXT_COLOR_OPTIONS_PAL4_WHITE           6
#define TEXT_COLOR_OPTIONS_PAL4_DARK_BLUE       8
#define TEXT_COLOR_OPTIONS_PAL4_LIGHT_BLUE      9

// Menu draw and input functions
struct // MENU_MAIN
{
    void (*drawChoices)(int selection, int y);
    int (*processInput)(int selection);
} static const sItemFunctionsMain[MENUITEM_MAIN_COUNT] =
{
    [MENUITEM_MAIN_TEXTSPEED]    = {DrawChoices_TextSpeed,   ProcessInput_Options_Four},
    [MENUITEM_MAIN_SPEEDUP]      = {DrawChoices_Speedup,     ProcessInput_Options_Four},
    [MENUITEM_MAIN_DAMAGE_NUMS]  = {DrawChoices_DamageNums,  ProcessInput_Options_Two},
    [MENUITEM_MAIN_BATTLESCENE]  = {DrawChoices_BattleScene, ProcessInput_Options_Two},
    [MENUITEM_MAIN_MATCHCALL]    = {DrawChoices_MatchCall,   ProcessInput_Options_Two},
    [MENUITEM_MAIN_BUTTONMODE]   = {DrawChoices_ButtonMode,  ProcessInput_Options_Three},
    [MENUITEM_MAIN_UNIT_SYSTEM]  = {DrawChoices_UnitSystem,  ProcessInput_Options_Two},
    [MENUITEM_MAIN_SOUND]        = {DrawChoices_Sound,       ProcessInput_Options_Two},
    [MENUITEM_MAIN_CANCEL]       = {NULL, NULL},
};

struct // MENU_DIFFICULTY
{
    void (*drawChoices)(int selection, int y);
    int (*processInput)(int selection);
} static const sItemFunctionsDifficulty[MENUITEM_DIFFICULTY_COUNT] =
{
    [MENUITEM_DIFFICULTY_MODE]         = {DrawChoices_Difficulty, ProcessInput_Options_Three},
    [MENUITEM_DIFFICULTY_LEVELCAPS]    = {DrawChoices_LevelCaps,  ProcessInput_Options_Three},
    [MENUITEM_DIFFICULTY_BATTLESTYLE]  = {DrawChoices_BattleStyle, ProcessInput_Options_Two},
    [MENUITEM_DIFFICULTY_VGC_DRAFT]    = {DrawChoices_VGCDraft, ProcessInput_Options_Three},
    [MENUITEM_DIFFICULTY_CANCEL]       = {NULL, NULL},
};

struct // MENU_RANDOM
{
    void (*drawChoices)(int selection, int y);
    int (*processInput)(int selection);
} static const sItemFunctionsRandom[MENUITEM_RANDOM_COUNT] =
{
    [MENUITEM_RANDOM_WILD]       = {DrawChoices_RandWild,    ProcessInput_Options_Two},
    [MENUITEM_RANDOM_TRAIN]      = {DrawChoices_RandTrain,   ProcessInput_Options_Two},
    [MENUITEM_RANDOM_RANDABIL]   = {DrawChoices_RandAbil,    ProcessInput_Options_Two},
    [MENUITEM_RANDOM_RNGSEED]    = {NULL, NULL},
    [MENUITEM_RANDOM_CANCEL]     = {NULL, NULL},
};

// Menu left side option names text
static const u8 sText_Speedup[]     = _("Speedup");
static const u8 sText_DamageNums[]  = _("DMG Numbers");
static const u8 sText_UnitSystem[]  = _("Unit System");
static const u8 sText_RandWild[]    = _("Encounters");
static const u8 sText_RandTrain[]   = _("Trainers");
static const u8 sText_RandAbil[]    = _("Abilities");
static const u8 sText_RngSeed[]     = _("RNG Seed");
static const u8 sText_Difficulty[]  = _("Mode");
static const u8 sText_LevelCaps[]   = _("LVL Caps");
static const u8 sText_VGCDraft[]    = _("Competitive");
static const u8 *const sOptionMenuItemsNamesMain[MENUITEM_MAIN_COUNT] =
{
    [MENUITEM_MAIN_TEXTSPEED]   = gText_TextSpeed,
    [MENUITEM_MAIN_SPEEDUP]     = sText_Speedup,
    [MENUITEM_MAIN_DAMAGE_NUMS] = sText_DamageNums,
    [MENUITEM_MAIN_BATTLESCENE] = gText_BattleScene,
    [MENUITEM_MAIN_MATCHCALL]   = gText_OptionMatchCalls,
    [MENUITEM_MAIN_BUTTONMODE]  = gText_ButtonMode,
    [MENUITEM_MAIN_UNIT_SYSTEM] = sText_UnitSystem,
    [MENUITEM_MAIN_SOUND]       = gText_Sound,
    [MENUITEM_MAIN_CANCEL]      = gText_OptionMenuSave,
};

static const u8 *const OptionMenuItemsNamesDifficulty[MENUITEM_DIFFICULTY_COUNT] =
{
    [MENUITEM_DIFFICULTY_MODE]        = sText_Difficulty,
    [MENUITEM_DIFFICULTY_LEVELCAPS]   = sText_LevelCaps,
    [MENUITEM_DIFFICULTY_BATTLESTYLE] = gText_BattleStyle,
    [MENUITEM_DIFFICULTY_VGC_DRAFT]   = sText_VGCDraft,
    [MENUITEM_DIFFICULTY_CANCEL]      = gText_OptionMenuSave,
};

static const u8 *const sOptionMenuItemsNamesCustom[MENUITEM_RANDOM_COUNT] =
{
    [MENUITEM_RANDOM_WILD]      = sText_RandWild,
    [MENUITEM_RANDOM_TRAIN]     = sText_RandTrain,
    [MENUITEM_RANDOM_RANDABIL]  = sText_RandAbil,
    [MENUITEM_RANDOM_RNGSEED]   = sText_RngSeed,
    [MENUITEM_RANDOM_CANCEL]    = gText_OptionMenuSave,
};

static const u8 *const OptionTextRight(u8 menuItem)
{
    switch (sOptions->submenu)
    {
        default:
        case MENU_MAIN:
            return sOptionMenuItemsNamesMain[menuItem];
        case MENU_DIFFICULTY:
            return OptionMenuItemsNamesDifficulty[menuItem];
        case MENU_RANDOM:
            return sOptionMenuItemsNamesCustom[menuItem];
    }
}

// Menu left side text conditions
static bool8 CheckConditions(int selection)
{
    switch (sOptions->submenu)
    {
    default:
    case MENU_MAIN:
        switch(selection)
        {
        default:
        case MENUITEM_MAIN_TEXTSPEED:       return TRUE;
        case MENUITEM_MAIN_SPEEDUP:         return TRUE;
        case MENUITEM_MAIN_DAMAGE_NUMS:     return TRUE;
        case MENUITEM_MAIN_BATTLESCENE:     return TRUE;
        case MENUITEM_MAIN_MATCHCALL:       return TRUE;
        case MENUITEM_MAIN_BUTTONMODE:      return TRUE;
        case MENUITEM_MAIN_UNIT_SYSTEM:     return TRUE;
        case MENUITEM_MAIN_SOUND:           return TRUE;
        case MENUITEM_MAIN_CANCEL:          return TRUE;
        case MENUITEM_MAIN_COUNT:           return TRUE;
        }
    case MENU_DIFFICULTY:
        switch(selection)
        {
        case MENUITEM_DIFFICULTY_MODE:      return TRUE;
        case MENUITEM_DIFFICULTY_LEVELCAPS: return TRUE;
        case MENUITEM_DIFFICULTY_BATTLESTYLE: return TRUE;
        case MENUITEM_DIFFICULTY_VGC_DRAFT: return TRUE;
        case MENUITEM_DIFFICULTY_CANCEL:    return TRUE;
        case MENUITEM_DIFFICULTY_COUNT:     return TRUE;
        }
    case MENU_RANDOM:
        switch(selection)
        {
        default:
        case MENUITEM_RANDOM_WILD:          return TRUE;
        case MENUITEM_RANDOM_TRAIN:         return TRUE;
        case MENUITEM_RANDOM_RANDABIL:      return TRUE;
        case MENUITEM_RANDOM_RNGSEED:       return TRUE;
        case MENUITEM_RANDOM_CANCEL:        return TRUE;
        case MENUITEM_RANDOM_COUNT:         return TRUE;
        }
    }
}

// Descriptions
static const u8 sText_Empty[]                   = _("");
static const u8 sText_Desc_Save[]               = _("Save your settings.");
static const u8 sText_Desc_TextSpeed[]          = _("Choose one of the four text-display\nspeeds.");
static const u8 sText_Desc_BattleScene_On[]     = _("Show the Pokémon battle animations.");
static const u8 sText_Desc_BattleScene_Off[]    = _("Skip the Pokémon battle animations.");
static const u8 sText_Desc_BattleStyle_Shift[]  = _("Get the option to switch your\nPokémon after the enemies faints.");
static const u8 sText_Desc_BattleStyle_Set[]    = _("No free switch after fainting the\nenemies Pokémon.");
static const u8 sText_Desc_SoundMono[]          = _("Sound is the same in all speakers.\nRecommended for original hardware.");
static const u8 sText_Desc_SoundStereo[]        = _("Play the left and right audio channel\nseperatly. Great with headphones.");
static const u8 sText_Desc_ButtonMode[]         = _("All buttons work as normal.");
static const u8 sText_Desc_ButtonMode_LR[]      = _("On some screens the L and R buttons\nact as left and right.");
static const u8 sText_Desc_ButtonMode_LA[]      = _("The L button acts as another A\nbutton for one-handed play.");
static const u8 sText_Desc_UnitSystemImperial[] = _("Display Berry and Pokémon weight\nand size in pounds and inches.");
static const u8 sText_Desc_UnitSystemMetric[]   = _("Display Berry and Pokémon weight\nand size in kilograms and meters.");
static const u8 sText_Desc_BattleSpeedup[]      = _("Adjusts how quickly time advances\nwhen holding the L button.");
static const u8 sText_Desc_BattleExpBar[]       = _("Choose how fast the EXP Bar will get\nfilled in battles.");
static const u8 sText_Desc_DamageNums_On[]      = _("Floating numbers will appear when\nPokémon take damage.");
static const u8 sText_Desc_DamageNums_Off[]     = _("Floating damage numbers will always\nbe hidden.");
static const u8 sText_Desc_SurfOff[]            = _("Disables the Surf theme when\nusing Surf.");
static const u8 sText_Desc_SurfOn[]             = _("Enables the Surf theme\nwhen using Surf.");
static const u8 sText_Desc_BikeOff[]            = _("Disables the Bike theme when\nusing the Bike.");
static const u8 sText_Desc_BikeOn[]             = _("Enables the Bike theme when\nusing the Bike.");
static const u8 sText_Desc_OverworldCallsOn[]   = _("Trainers will be able to call you,\noffering rematches and info.");
static const u8 sText_Desc_OverworldCallsOff[]  = _("You will not receive calls.\nSpecial events will still occur.");
static const u8 sText_Desc_RandWildNorm[]       = _("Wild Pokémon encounters will\nbe normal.");
static const u8 sText_Desc_RandWildRand[]       = _("Wild Pokémon encounters will\nbe randomised.");
static const u8 sText_Desc_RandTrainNorm[]      = _("Trainer Pokémon will be\nnormal.");
static const u8 sText_Desc_RandTrainRand[]      = _("Trainer Pokémon will be\nrandomised.");
static const u8 sText_Desc_RandAbilNorm[]       = _("Pokémon Abilities will be\nnormal.");
static const u8 sText_Desc_RandAbilRand[]       = _("Pokémon Abilities will be\nrandomised.");
static const u8 sText_Desc_RngSeed[]            = _("Change the RNG Seed.\nInvalidates challenge runs.");
static const u8 sText_Desc_DifficultyEasy[]     = _("Trainer's have their standard teams.\nThe intended difficulty.");
static const u8 sText_Desc_DifficultyHard[]     = _("Trainer's have more difficult teams.\nIntended for challenge runs.");
static const u8 sText_Desc_DifficultyDouble[]  = _("Trainer battles are always doubles.\nIntended for challenge runs.");
static const u8 sText_Desc_LevelCaps_Off[]      = _("No level caps applied.\nThe regular Pokémon experience.");
static const u8 sText_Desc_LevelCaps_Soft[]     = _("Exp is reduced as you exceed\nthe level of the next gym.");
static const u8 sText_Desc_LevelCaps_Hard[]     = _("Once you reach the level of the\nnext gym, exp is halted.");
static const u8 sText_Desc_VGCDraft_None[]      = _("Competitive battle format is\ncompletely disabled.");
static const u8 sText_Desc_VGCDraft_Draft[]     = _("Draft up to four Pokémon for\nsignificant battles.");
static const u8 sText_Desc_VGCDraft_Both[]      = _("Win two matches out of three\nto beat significant battles.");

static const u8 *const sOptionMenuItemDescriptionsMain[MENUITEM_MAIN_COUNT][3] =
{
    [MENUITEM_MAIN_TEXTSPEED]   = {sText_Desc_TextSpeed,           sText_Empty,                  sText_Empty},
    [MENUITEM_MAIN_SPEEDUP]     = {sText_Desc_BattleSpeedup,       sText_Empty,                  sText_Empty},
    [MENUITEM_MAIN_DAMAGE_NUMS] = {sText_Desc_DamageNums_On,       sText_Desc_DamageNums_Off,    sText_Empty},
    [MENUITEM_MAIN_BATTLESCENE] = {sText_Desc_BattleScene_On,      sText_Desc_BattleScene_Off,   sText_Empty},
    [MENUITEM_MAIN_MATCHCALL]   = {sText_Desc_OverworldCallsOn,    sText_Desc_OverworldCallsOff, sText_Empty},
    [MENUITEM_MAIN_BUTTONMODE]  = {sText_Desc_ButtonMode,          sText_Desc_ButtonMode_LR,     sText_Desc_ButtonMode_LA},
    [MENUITEM_MAIN_UNIT_SYSTEM] = {sText_Desc_UnitSystemImperial,  sText_Desc_UnitSystemMetric,  sText_Empty},
    [MENUITEM_MAIN_SOUND]       = {sText_Desc_SoundMono,           sText_Desc_SoundStereo,       sText_Empty},
    [MENUITEM_MAIN_CANCEL]      = {sText_Desc_Save,                sText_Empty,                  sText_Empty},
};

static const u8 *const sOptionMenuItemDescriptionsDifficulty[MENUITEM_DIFFICULTY_COUNT][3] =
{
    [MENUITEM_DIFFICULTY_MODE]        = {sText_Desc_DifficultyEasy,     sText_Desc_DifficultyHard,  sText_Desc_DifficultyDouble},
    [MENUITEM_DIFFICULTY_LEVELCAPS]   = {sText_Desc_LevelCaps_Off,      sText_Desc_LevelCaps_Soft,  sText_Desc_LevelCaps_Hard},
    [MENUITEM_DIFFICULTY_BATTLESTYLE] = {sText_Desc_BattleStyle_Shift,  sText_Desc_BattleStyle_Set, sText_Empty},
    [MENUITEM_DIFFICULTY_VGC_DRAFT]   = {sText_Desc_VGCDraft_None,      sText_Desc_VGCDraft_Draft,  sText_Desc_VGCDraft_Both},
    [MENUITEM_DIFFICULTY_CANCEL]      = {sText_Desc_Save,               sText_Empty,                sText_Empty},
};

static const u8 *const sOptionMenuItemDescriptionsRandom[MENUITEM_RANDOM_COUNT][2] =
{
    [MENUITEM_RANDOM_WILD]      = {sText_Desc_RandWildNorm,       sText_Desc_RandWildRand},
    [MENUITEM_RANDOM_TRAIN]     = {sText_Desc_RandTrainNorm,      sText_Desc_RandTrainRand},
    [MENUITEM_RANDOM_RANDABIL]  = {sText_Desc_RandAbilNorm,       sText_Desc_RandAbilRand},
    [MENUITEM_RANDOM_RNGSEED]   = {sText_Desc_RngSeed,            sText_Empty},
    [MENUITEM_RANDOM_CANCEL]    = {sText_Desc_Save,               sText_Empty},
};

// Disabled Descriptions
static const u8 *const sOptionMenuItemDescriptionsDisabledMain[MENUITEM_MAIN_COUNT] =
{
    [MENUITEM_MAIN_TEXTSPEED]         = sText_Empty,
    [MENUITEM_MAIN_SPEEDUP]           = sText_Empty,
    [MENUITEM_MAIN_DAMAGE_NUMS]       = sText_Empty,
    [MENUITEM_MAIN_BATTLESCENE]       = sText_Empty,
    [MENUITEM_MAIN_MATCHCALL]         = sText_Empty,
    [MENUITEM_MAIN_BUTTONMODE]        = sText_Empty,
    [MENUITEM_MAIN_UNIT_SYSTEM]       = sText_Empty,
    [MENUITEM_MAIN_SOUND]             = sText_Empty,
    [MENUITEM_MAIN_CANCEL]            = sText_Empty,
};

// Disabled Difficulty
static const u8 *const sOptionMenuItemDescriptionsDisabledDifficulty[MENUITEM_DIFFICULTY_COUNT] =
{
    [MENUITEM_DIFFICULTY_MODE]        = sText_Empty,
    [MENUITEM_DIFFICULTY_LEVELCAPS]   = sText_Empty,
    [MENUITEM_DIFFICULTY_BATTLESTYLE] = sText_Empty,
    [MENUITEM_DIFFICULTY_VGC_DRAFT]   = sText_Empty,
    [MENUITEM_DIFFICULTY_CANCEL]      = sText_Empty,
};

// Disabled Random
static const u8 *const sOptionMenuItemDescriptionsDisabledRandom[MENUITEM_RANDOM_COUNT] =
{
    [MENUITEM_RANDOM_WILD]     = sText_Empty,
    [MENUITEM_RANDOM_TRAIN]    = sText_Empty,
    [MENUITEM_RANDOM_RANDABIL] = sText_Empty,
    [MENUITEM_RANDOM_RNGSEED]  = sText_Empty,
    [MENUITEM_RANDOM_CANCEL]   = sText_Empty,
};

static const u8 *const OptionTextDescription(void)
{
    u8 menuItem = sOptions->menuCursor[sOptions->submenu];
    u8 selection;

    switch (sOptions->submenu)
    {
    default:
    case MENU_MAIN:
        if (!CheckConditions(menuItem))
            return sOptionMenuItemDescriptionsDisabledMain[menuItem];
        selection = sOptions->sel[menuItem];
        if (menuItem == MENUITEM_MAIN_TEXTSPEED || menuItem == MENUITEM_MAIN_SPEEDUP)
            selection = 0;
        return sOptionMenuItemDescriptionsMain[menuItem][selection];
    case MENU_DIFFICULTY:
        if (!CheckConditions(menuItem))
            return sOptionMenuItemDescriptionsDisabledDifficulty[menuItem];
        selection = sOptions->sel_difficulty[menuItem];
        return sOptionMenuItemDescriptionsDifficulty[menuItem][selection];
    case MENU_RANDOM:
        if (!CheckConditions(menuItem))
            return sOptionMenuItemDescriptionsDisabledRandom[menuItem];
        selection = sOptions->sel_random[menuItem];
        if (menuItem == MENUITEM_RANDOM_RNGSEED)
            selection = 0;
        return sOptionMenuItemDescriptionsRandom[menuItem][selection];
    }
}

static u8 MenuItemCount(void)
{
    switch (sOptions->submenu)
    {
    default:
    case MENU_MAIN:       return MENUITEM_MAIN_COUNT;
    case MENU_DIFFICULTY: return MENUITEM_DIFFICULTY_COUNT;
    case MENU_RANDOM:     return MENUITEM_RANDOM_COUNT;
    }
}

static u8 MenuItemCancel(void)
{
    switch (sOptions->submenu)
    {
    default:
    case MENU_MAIN:       return MENUITEM_MAIN_CANCEL;
    case MENU_DIFFICULTY: return MENUITEM_DIFFICULTY_CANCEL;
    case MENU_RANDOM:     return MENUITEM_RANDOM_CANCEL;
    }
}

// Main code
static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    ChangeBgX(3, 64, BG_COORD_ADD);
    ChangeBgY(3, 64, BG_COORD_ADD);
}

static const u8 sText_TopBar_Main[]                = _("General");
static const u8 sText_TopBar_Main_To_Left[]        = _("{L_BUTTON}General");
static const u8 sText_TopBar_Main_To_Right[]       = _("{R_BUTTON}General");
static const u8 sText_TopBar_Random[]              = _("Random");
static const u8 sText_TopBar_Random_To_Left[]      = _("{L_BUTTON}Random");
static const u8 sText_TopBar_Random_To_Right[]     = _("{R_BUTTON}Random");
static const u8 sText_TopBar_Difficulty[]          = _("Difficulty");
static const u8 sText_TopBar_Difficulty_To_Left[]  = _("{L_BUTTON}Difficulty");
static const u8 sText_TopBar_Difficulty_To_Right[] = _("{R_BUTTON}Difficulty");

static void DrawTopBarText(void)
{
    const u8 color[3] = { 0, TEXT_COLOR_WHITE, TEXT_COLOR_OPTIONS_GRAY_FG };

    FillWindowPixelBuffer(WIN_TOPBAR, PIXEL_FILL(0));
    switch (sOptions->submenu)
    {
        case MENU_MAIN:
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 94, 1, color, 0, sText_TopBar_Main);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 2, 1, color, 0, sText_TopBar_Random_To_Left);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 172, 1, color, 0, sText_TopBar_Difficulty_To_Right);
            break;
        case MENU_DIFFICULTY:
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 96, 1, color, 0, sText_TopBar_Difficulty);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 2, 1, color, 0, sText_TopBar_Main_To_Left);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 190, 1, color, 0, sText_TopBar_Random_To_Right);
            break;
        case MENU_RANDOM:
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 110, 1, color, 0, sText_TopBar_Random);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 2, 1, color, 0, sText_TopBar_Difficulty_To_Left);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 186, 1, color, 0, sText_TopBar_Main_To_Right);
            break;
    }
    PutWindowTilemap(WIN_TOPBAR);
    CopyWindowToVram(WIN_TOPBAR, COPYWIN_FULL);
}

static void DrawOptionMenuTexts(void) //left side text
{
    u8 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(0));
    for (i = 0; i < MenuItemCount(); i++)
        DrawLeftSideOptionText(i, (i * Y_DIFF) + 1);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void DrawDescriptionText(void)
{
    u8 color_gray[3];
    color_gray[0] = TEXT_COLOR_TRANSPARENT;
    color_gray[1] = TEXT_COLOR_OPTIONS_GRAY_FG;
    color_gray[2] = TEXT_COLOR_OPTIONS_GRAY_SHADOW;
        
    FillWindowPixelBuffer(WIN_DESCRIPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized4(WIN_DESCRIPTION, FONT_NORMAL, 8, 1, 0, 0, color_gray, TEXT_SKIP_DRAW, OptionTextDescription());
    CopyWindowToVram(WIN_DESCRIPTION, COPYWIN_FULL);
}

static void DrawLeftSideOptionText(int selection, int y)
{
    u8 color_yellow[3];
    u8 color_gray[3];

    color_yellow[0] = TEXT_COLOR_TRANSPARENT;
    color_yellow[1] = TEXT_COLOR_OPTIONS_PAL4_WHITE;
    color_yellow[2] = TEXT_COLOR_OPTIONS_PAL4_DARK_GREY;
    color_gray[0] = TEXT_COLOR_TRANSPARENT;
    color_gray[1] = TEXT_COLOR_OPTIONS_PAL4_WHITE;
    color_gray[2] = TEXT_COLOR_OPTIONS_PAL4_DARK_GREY;

    if (CheckConditions(selection))
        AddTextPrinterParameterized4(WIN_OPTIONS, FONT_NORMAL, 8, y, 0, 0, color_yellow, TEXT_SKIP_DRAW, OptionTextRight(selection));
    else
        AddTextPrinterParameterized4(WIN_OPTIONS, FONT_NORMAL, 8, y, 0, 0, color_gray, TEXT_SKIP_DRAW, OptionTextRight(selection));
}

static void DrawRightSideChoiceText(const u8 *text, int x, int y, bool8 choosen, bool8 active)
{
    u8 color_red[3];
    u8 color_gray[3];

    if (active)
    {
        color_red[0] = TEXT_COLOR_TRANSPARENT;
        color_red[1] = TEXT_COLOR_OPTIONS_PAL4_LIGHT_BLUE; 
        color_red[2] = TEXT_COLOR_OPTIONS_PAL4_DARK_BLUE;
        color_gray[0] = TEXT_COLOR_TRANSPARENT;
        color_gray[1] = TEXT_COLOR_OPTIONS_PAL4_BLACK;
        color_gray[2] = TEXT_COLOR_OPTIONS_PAL4_GREY;
    }
    else
    {
        color_red[0] = TEXT_COLOR_TRANSPARENT;
        color_red[1] = 1;
        color_red[2] = 4;
        color_gray[0] = TEXT_COLOR_TRANSPARENT;
        color_gray[1] = 1;
        color_gray[2] = 4;
    }


    if (choosen)
        AddTextPrinterParameterized4(WIN_OPTIONS, FONT_NORMAL, x, y, 0, 0, color_red, TEXT_SKIP_DRAW, text);
    else
        AddTextPrinterParameterized4(WIN_OPTIONS, FONT_NORMAL, x, y, 0, 0, color_gray, TEXT_SKIP_DRAW, text);
}

static void DrawChoices(u32 id, int y) //right side draw function
{
    switch (sOptions->submenu)
    {
        case MENU_MAIN:
            if (sItemFunctionsMain[id].drawChoices != NULL)
                sItemFunctionsMain[id].drawChoices(sOptions->sel[id], y);
            break;
        case MENU_DIFFICULTY:
            if (sItemFunctionsDifficulty[id].drawChoices != NULL)
                sItemFunctionsDifficulty[id].drawChoices(sOptions->sel_difficulty[id], y);
            break;
        case MENU_RANDOM:
            if (sItemFunctionsRandom[id].drawChoices != NULL)
                sItemFunctionsRandom[id].drawChoices(sOptions->sel_random[id], y);
            break;
    }
}

static void HighlightOptionMenuItem(void)
{
    int cursor = sOptions->visibleCursor[sOptions->submenu];

    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(8, 232));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(cursor * Y_DIFF + 24, cursor * Y_DIFF + 40));
}

static bool8 OptionsMenu_LoadGraphics(void) // Load all the tilesets, tilemaps, spritesheets, and palettes
{
    switch (sOptions->gfxLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(2, sOptionsPlusTiles, 0, 0, 0);
        sOptions->gfxLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(sOptionsPlusTilemap, sBg2TilemapBuffer);
            sOptions->gfxLoadState++;
        }
        break;
    case 2:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(3, gScrollBgTiles, 0, 0, 0);
        sOptions->gfxLoadState++;
        break;
    case 3:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(gScrollBgTilemap, sBg3TilemapBuffer);
            sOptions->gfxLoadState++;
        }
        break;
    case 4:
        LoadPalette(sOptionsPlusPalette, BG_PLTT_ID(4), PLTT_SIZE_4BPP);
        LoadPalette(gScrollBgPalette, BG_PLTT_ID(2), PLTT_SIZE_4BPP);
        sOptions->gfxLoadState++;
        break;
    default:
        sOptions->gfxLoadState = 0;
        return TRUE;
    }
    return FALSE;
}

void CB2_InitOptionPlusMenu(void)
{
    u32 i;
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        ResetVramOamAndBgCntRegs();
        sOptions = AllocZeroed(sizeof(*sOptions));
        FreeAllSpritePalettes();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        ResetBgsAndClearDma3BusyFlags(0);
        ResetBgPositions();

        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(14, 6));
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);

        ResetAllBgsCoordinates();
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, NELEMS(sOptionMenuBgTemplates));
        InitWindows(sOptionMenuWinTemplates);

        sBg2TilemapBuffer = Alloc(0x800);
        memset(sBg2TilemapBuffer, 0, 0x800);
        SetBgTilemapBuffer(2, sBg2TilemapBuffer);
        ScheduleBgCopyTilemapToVram(2);

        sBg3TilemapBuffer = Alloc(0x800);
        memset(sBg3TilemapBuffer, 0, 0x800);
        SetBgTilemapBuffer(3, sBg3TilemapBuffer);
        ScheduleBgCopyTilemapToVram(3);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        gMain.state++;
        sOptions->gfxLoadState = 0;
        break;
    case 3:
        if (OptionsMenu_LoadGraphics() == TRUE)
        {
            gMain.state++;
            LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        }
        break;
    case 4:
        LoadPalette(sOptionMenuBg_Pal, 0, sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, 0x70, 0x20);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sOptionMenuText_Pal, 16, sizeof(sOptionMenuText_Pal));
        gMain.state++;
        break;
    case 6:
        sOptions->sel[MENUITEM_MAIN_TEXTSPEED]   = gSaveBlock2Ptr->optionsTextSpeed;
        sOptions->sel[MENUITEM_MAIN_SPEEDUP]     = gSaveBlock2Ptr->optionsSpeedModifer;
        sOptions->sel[MENUITEM_MAIN_DAMAGE_NUMS] = gSaveBlock2Ptr->optionsDamageNumsOff;
        sOptions->sel[MENUITEM_MAIN_BATTLESCENE] = gSaveBlock2Ptr->optionsBattleSceneOff;
        sOptions->sel[MENUITEM_MAIN_MATCHCALL]   = gSaveBlock2Ptr->optionsDisableMatchCall;
        sOptions->sel[MENUITEM_MAIN_BUTTONMODE]  = gSaveBlock2Ptr->optionsButtonMode;
        sOptions->sel[MENUITEM_MAIN_UNIT_SYSTEM] = gSaveBlock2Ptr->optionsUnitSystem;
        sOptions->sel[MENUITEM_MAIN_SOUND]       = gSaveBlock2Ptr->optionsSound;

        sOptions->sel_difficulty[MENUITEM_DIFFICULTY_MODE]        = gSaveBlock2Ptr->optionsDifficulty;
        sOptions->sel_difficulty[MENUITEM_DIFFICULTY_LEVELCAPS]   = gSaveBlock2Ptr->optionsLevelCap;
        sOptions->sel_difficulty[MENUITEM_DIFFICULTY_BATTLESTYLE] = gSaveBlock2Ptr->optionsBattleStyle;
        sOptions->sel_difficulty[MENUITEM_DIFFICULTY_VGC_DRAFT]   = gSaveBlock2Ptr->optionsVGCDraft;

        sOptions->sel_random[MENUITEM_RANDOM_WILD]     = gSaveBlock2Ptr->optionsWildRandomiser;
        sOptions->sel_random[MENUITEM_RANDOM_TRAIN]    = gSaveBlock2Ptr->optionsTrainerRandomiser;
        sOptions->sel_random[MENUITEM_RANDOM_RANDABIL] = gSaveBlock2Ptr->optionsAbilityRandomiser;

        sOptions->submenu = MENU_MAIN;

        gMain.state++;
        break;
    case 7:
        PutWindowTilemap(WIN_TOPBAR);
        DrawTopBarText();
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_DESCRIPTION);
        DrawDescriptionText();
        gMain.state++;
        break;
    case 9:
        PutWindowTilemap(WIN_OPTIONS);
        DrawOptionMenuTexts();
        gMain.state++;
        break;
    case 10:
        CreateTask(Task_OptionMenuFadeIn, 0);
        sOptions->arrowTaskId = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 110, 20, 110, MENUITEM_MAIN_COUNT - 1, 110, 110, 0);

        for (i = 0; i < min(OPTIONS_ON_SCREEN, MenuItemCount()); i++)
            DrawChoices(i, i * Y_DIFF);

        HighlightOptionMenuItem();

        CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
        gMain.state++;
        break;
    case 11:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 12:
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_OptionMenuProcessInput;
        SetGpuReg(REG_OFFSET_WIN0H, 0); // Idk man Im just trying to stop this stupid graphical bug from happening dont judge me
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(14, 6));
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        HighlightOptionMenuItem();
        return;
    }
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
    u8 optionsToDraw = min(OPTIONS_ON_SCREEN , MenuItemCount());
    if (JOY_NEW(A_BUTTON))
    {
        if (sOptions->menuCursor[sOptions->submenu] == MenuItemCancel())
            gTasks[taskId].func = Task_OptionMenuSave;
        if (sOptions->menuCursor[sOptions->submenu] == MENUITEM_RANDOM_RNGSEED)
            DoRngSeedScreen();
    }
    else if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sOptions->visibleCursor[sOptions->submenu] == NUM_OPTIONS_FROM_BORDER) // don't advance visible cursor until scrolled to the bottom
        {
            if (--sOptions->menuCursor[sOptions->submenu] == 0)
                sOptions->visibleCursor[sOptions->submenu]--;
            else
                ScrollMenu(1);
        }
        else
        {
            if (--sOptions->menuCursor[sOptions->submenu] < 0) // Scroll all the way to the bottom.
            {
                sOptions->visibleCursor[sOptions->submenu] = sOptions->menuCursor[sOptions->submenu] = optionsToDraw-2;
                ScrollAll(0);
                sOptions->visibleCursor[sOptions->submenu] = optionsToDraw-1;
                sOptions->menuCursor[sOptions->submenu] = MenuItemCount() - 1;
            }
            else
            {
                sOptions->visibleCursor[sOptions->submenu]--;
            }
        }
        HighlightOptionMenuItem();
        DrawDescriptionText();
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sOptions->visibleCursor[sOptions->submenu] == optionsToDraw-2) // don't advance visible cursor until scrolled to the bottom
        {
            if (++sOptions->menuCursor[sOptions->submenu] == MenuItemCount() - 1)
                sOptions->visibleCursor[sOptions->submenu]++;
            else
                ScrollMenu(0);
        }
        else
        {
            if (++sOptions->menuCursor[sOptions->submenu] >= MenuItemCount()-1) // Scroll all the way to the top.
            {
                sOptions->visibleCursor[sOptions->submenu] = optionsToDraw-2;
                sOptions->menuCursor[sOptions->submenu] = MenuItemCount() - optionsToDraw-1;
                ScrollAll(1);
                sOptions->visibleCursor[sOptions->submenu] = sOptions->menuCursor[sOptions->submenu] = 0;
            }
            else
            {
                sOptions->visibleCursor[sOptions->submenu]++;
            }
        }
        HighlightOptionMenuItem();
        DrawDescriptionText();
    }
    else if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        if (sOptions->submenu == MENU_MAIN)
        {
            int cursor = sOptions->menuCursor[sOptions->submenu];
            u8 previousOption = sOptions->sel[cursor];
            if (CheckConditions(cursor))
            {
                if (sItemFunctionsMain[cursor].processInput != NULL)
                {
                    sOptions->sel[cursor] = sItemFunctionsMain[cursor].processInput(previousOption);
                    ReDrawAll();
                    DrawDescriptionText();
                }

                if (previousOption != sOptions->sel[cursor])
                    DrawChoices(cursor, sOptions->visibleCursor[sOptions->submenu] * Y_DIFF);
            }
        }
        else if (sOptions->submenu == MENU_DIFFICULTY)
        {
            int cursor = sOptions->menuCursor[sOptions->submenu];
            u8 previousOption = sOptions->sel_difficulty[cursor];
            if (CheckConditions(cursor))
            {
                if (sItemFunctionsDifficulty[cursor].processInput != NULL)
                {
                    sOptions->sel_difficulty[cursor] = sItemFunctionsDifficulty[cursor].processInput(previousOption);
                    ReDrawAll();
                    DrawDescriptionText();
                }

                if (previousOption != sOptions->sel_difficulty[cursor])
                    DrawChoices(cursor, sOptions->visibleCursor[sOptions->submenu] * Y_DIFF);
            }
        }
        else if (sOptions->submenu == MENU_RANDOM)
        {
            int cursor = sOptions->menuCursor[sOptions->submenu];
            u8 previousOption = sOptions->sel_random[cursor];
            if (CheckConditions(cursor))
            {
                if (sItemFunctionsRandom[cursor].processInput != NULL)
                {
                    sOptions->sel_random[cursor] = sItemFunctionsRandom[cursor].processInput(previousOption);
                    ReDrawAll();
                    DrawDescriptionText();
                }

                if (previousOption != sOptions->sel_random[cursor])
                    DrawChoices(cursor, sOptions->visibleCursor[sOptions->submenu] * Y_DIFF);
            }
        }
    }
    else if (JOY_NEW(R_BUTTON))
    {
        if (sOptions->submenu == MENU_RANDOM)
            sOptions->submenu = MENU_MAIN;
        else
            sOptions->submenu++;

        DrawTopBarText();
        ReDrawAll();
        HighlightOptionMenuItem();
        DrawDescriptionText();
    }
    else if (JOY_NEW(L_BUTTON))
    {
        if (sOptions->submenu == MENU_MAIN)
            sOptions->submenu = MENU_RANDOM;
        else
            sOptions->submenu--;
        
        DrawTopBarText();
        ReDrawAll();
        HighlightOptionMenuItem();
        DrawDescriptionText();
    }
}

static void SaveOptionsForExit(void)
{
    gSaveBlock2Ptr->optionsTextSpeed         = sOptions->sel[MENUITEM_MAIN_TEXTSPEED];
    gSaveBlock2Ptr->optionsSpeedModifer      = sOptions->sel[MENUITEM_MAIN_SPEEDUP];
    gSaveBlock2Ptr->optionsDamageNumsOff     = sOptions->sel[MENUITEM_MAIN_DAMAGE_NUMS];
    gSaveBlock2Ptr->optionsBattleSceneOff    = sOptions->sel[MENUITEM_MAIN_BATTLESCENE];
    gSaveBlock2Ptr->optionsDisableMatchCall  = sOptions->sel[MENUITEM_MAIN_MATCHCALL];
    gSaveBlock2Ptr->optionsButtonMode        = sOptions->sel[MENUITEM_MAIN_BUTTONMODE];
    gSaveBlock2Ptr->optionsUnitSystem        = sOptions->sel[MENUITEM_MAIN_UNIT_SYSTEM];
    gSaveBlock2Ptr->optionsSound             = sOptions->sel[MENUITEM_MAIN_SOUND];

    gSaveBlock2Ptr->optionsDifficulty        = sOptions->sel_difficulty[MENUITEM_DIFFICULTY_MODE];
    gSaveBlock2Ptr->optionsLevelCap          = sOptions->sel_difficulty[MENUITEM_DIFFICULTY_LEVELCAPS];
    gSaveBlock2Ptr->optionsBattleStyle       = sOptions->sel_difficulty[MENUITEM_DIFFICULTY_BATTLESTYLE];
    gSaveBlock2Ptr->optionsVGCDraft          = sOptions->sel_difficulty[MENUITEM_DIFFICULTY_VGC_DRAFT];

    gSaveBlock2Ptr->optionsWildRandomiser    = sOptions->sel_random[MENUITEM_RANDOM_WILD];
    gSaveBlock2Ptr->optionsTrainerRandomiser = sOptions->sel_random[MENUITEM_RANDOM_TRAIN];
    gSaveBlock2Ptr->optionsAbilityRandomiser = sOptions->sel_random[MENUITEM_RANDOM_RANDABIL];
}

static void Task_OptionMenuSave(u8 taskId)
{
    SaveOptionsForExit();
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

#define try_free(ptr) ({        \
    void ** ptr__ = (void **)&(ptr);   \
    if (*ptr__ != NULL)                \
        Free(*ptr__);                  \
})

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        FREE_AND_SET_NULL(sOptions);
        try_free(sBg2TilemapBuffer);
        try_free(sBg3TilemapBuffer);
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, 0);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        HideBg(2);
        HideBg(3);
        SetMainCallback2(gMain.savedCallback);
    }
}

static void ScrollMenu(int direction)
{
    int menuItem, pos;
    u8 optionsToDraw = min(OPTIONS_ON_SCREEN, MenuItemCount());

    if (direction == 0) // scroll down
        menuItem = sOptions->menuCursor[sOptions->submenu] + NUM_OPTIONS_FROM_BORDER, pos = optionsToDraw - 1;
    else
        menuItem = sOptions->menuCursor[sOptions->submenu] - NUM_OPTIONS_FROM_BORDER, pos = 0;

    // Hide one
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF, PIXEL_FILL(0));
    // Show one
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(0), 0, Y_DIFF * pos, 26 * 8, Y_DIFF);
    // Print
    DrawChoices(menuItem, pos * Y_DIFF);
    DrawLeftSideOptionText(menuItem, (pos * Y_DIFF) + 1);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}
static void ScrollAll(int direction) // to bottom or top
{
    int i, y, menuItem, pos;
    int scrollCount;
    u8 optionsToDraw = min(OPTIONS_ON_SCREEN, MenuItemCount());

    scrollCount = MenuItemCount() - optionsToDraw;

    // Move items up/down
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF * scrollCount, PIXEL_FILL(1));

    // Clear moved items
    if (direction == 0)
    {
        y = optionsToDraw - scrollCount;
        if (y < 0)
            y = optionsToDraw;
        y *= Y_DIFF;
    }
    else
    {
        y = 0;
    }

    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(0), 0, y, 26 * 8, Y_DIFF * scrollCount);
    // Print new texts
    for (i = 0; i < scrollCount; i++)
    {
        if (direction == 0) // From top to bottom
            menuItem = MenuItemCount() - 1 - i, pos = optionsToDraw - 1 - i;
        else // From bottom to top
            menuItem = i, pos = i;
        DrawChoices(menuItem, pos * Y_DIFF);
        DrawLeftSideOptionText(menuItem, (pos * Y_DIFF) + 1);
    }
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}

// Process Input functions ****GENERIC****
static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3)
{
    int xMid;
    int widthLeft = GetStringWidth(1, txt1, 0);
    int widthMid = GetStringWidth(1, txt2, 0);
    int widthRight = GetStringWidth(1, txt3, 0);

    widthMid -= (198 - 104);
    xMid = (widthLeft - widthMid - widthRight) / 2 + 104;
    return xMid;
}

static int XOptions_ProcessInput(int x, int selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (++selection > (x - 1))
            selection = 0;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (--selection < 0)
            selection = (x - 1);
    }
    return selection;
}

static int ProcessInput_Options_Two(int selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
        selection ^= 1;

    return selection;
}

static int ProcessInput_Options_Three(int selection)
{
    return XOptions_ProcessInput(3, selection);
}

static int ProcessInput_Options_Four(int selection)
{
    return XOptions_ProcessInput(4, selection);
}

// Draw Choices functions ****GENERIC****
static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style, bool8 active)
{
    bool8 choosen = FALSE;
    if (style != 0)
        choosen = TRUE;

    DrawRightSideChoiceText(text, x, y+1, choosen, active);
}

static void DrawChoices_Options_Four(const u8 *const *const strings, int selection, int y, bool8 active)
{
    static const u8 choiceOrders[][3] =
    {
        {0, 1, 2},
        {0, 1, 2},
        {1, 2, 3},
        {1, 2, 3},
    };
    u8 styles[4] = {0};
    int xMid;
    const u8 *order = choiceOrders[selection];

    styles[selection] = 1;
    xMid = GetMiddleX(strings[order[0]], strings[order[1]], strings[order[2]]);

    DrawOptionMenuChoice(strings[order[0]], 104, y, styles[order[0]], active);
    DrawOptionMenuChoice(strings[order[1]], xMid, y, styles[order[1]], active);
    DrawOptionMenuChoice(strings[order[2]], GetStringRightAlignXOffset(1, strings[order[2]], 198), y, styles[order[2]], active);
}

static void ReDrawAll(void)
{
    u8 menuItem = sOptions->menuCursor[sOptions->submenu] - sOptions->visibleCursor[sOptions->submenu];
    u8 i;
    u8 optionsToDraw = min(OPTIONS_ON_SCREEN, MenuItemCount());

    if (MenuItemCount() <= OPTIONS_ON_SCREEN) // Draw or delete the scrolling arrows based on options in the menu
    {
        if (sOptions->arrowTaskId != TASK_NONE)
        {
            RemoveScrollIndicatorArrowPair(sOptions->arrowTaskId);
            sOptions->arrowTaskId = TASK_NONE;
        }
    }
    else
    {
        if (sOptions->arrowTaskId == TASK_NONE)
            sOptions->arrowTaskId = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 110, 20, 110, MenuItemCount() - 1, 110, 110, 0);

    }

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(0));
    for (i = 0; i < optionsToDraw; i++)
    {
        DrawChoices(menuItem+i, i * Y_DIFF);
        DrawLeftSideOptionText(menuItem+i, (i * Y_DIFF) + 1);
    }
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}

// Process Input functions ****SPECIFIC****
static const u8 sText_Faster[] = _("Faster");
static const u8 sText_Instant[] = _("Fastest");
static const u8 *const sTextSpeedStrings[] = {gText_TextSpeedSlow, gText_TextSpeedMid, gText_TextSpeedFast, sText_Faster};
static void DrawChoices_TextSpeed(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_TEXTSPEED);
    DrawChoices_Options_Four(sTextSpeedStrings, selection, y, active);
}

static const u8 sText_SpeedupOff[] = _("Off");
static const u8 sText_SpeedupTwoX[] = _("2x");
static const u8 sText_SpeedupThreeX[] = _("3x");
static const u8 sText_SpeedupFourX[] = _("4x");
static const u8 *const sSpeedupStrings[] = {sText_SpeedupOff, sText_SpeedupTwoX, sText_SpeedupThreeX, sText_SpeedupFourX};
static void DrawChoices_Speedup(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_SPEEDUP);
    DrawChoices_Options_Four(sSpeedupStrings, selection, y, active);
}

static const u8 gText_DifficultyEasy[] = _("Nrml");
static const u8 gText_DifficultyHard[] = _("Hard");
static const u8 gText_DifficultyDouble[] = _("Doubl");
static void DrawChoices_Difficulty(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_DIFFICULTY_LEVELCAPS);
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_DifficultyEasy, gText_DifficultyHard, gText_DifficultyDouble);
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_DifficultyEasy, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_DifficultyHard, xMid, y, styles[1], active);
    DrawOptionMenuChoice(gText_DifficultyDouble, GetStringRightAlignXOffset(1, gText_DifficultyDouble, 198), y, styles[2], active);
}

static const u8 sText_LevelCaps_Off[] = _("Off");
static const u8 sText_LevelCaps_Soft[] = _("Soft");
static const u8 sText_LevelCaps_Hard[] = _("Hard");
static void DrawChoices_LevelCaps(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_DIFFICULTY_LEVELCAPS);
    u8 styles[3] = {0};
    int xMid = GetMiddleX(sText_LevelCaps_Off, sText_LevelCaps_Soft, sText_LevelCaps_Hard);
    styles[selection] = 1;

    DrawOptionMenuChoice(sText_LevelCaps_Off, 104, y, styles[0], active);
    DrawOptionMenuChoice(sText_LevelCaps_Soft, xMid, y, styles[1], active);
    DrawOptionMenuChoice(sText_LevelCaps_Hard, GetStringRightAlignXOffset(1, sText_LevelCaps_Hard, 198), y, styles[2], active);
}

static void DrawChoices_DamageNums(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_DAMAGE_NUMS);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOff, 198), y, styles[1], active);
}

static void DrawChoices_BattleScene(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_BATTLESCENE);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOff, 198), y, styles[1], active);
}

static void DrawChoices_BattleStyle(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_DIFFICULTY_BATTLESTYLE);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleStyleShift, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_BattleStyleSet, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleStyleSet, 198), y, styles[1], active);
}

static const u8 sText_VGCDraft_None[] = _("Off");
static const u8 sText_VGCDraft_Draft[] = _("Draft");
static const u8 sText_VGCDraft_Both[] = _("BO3");
static void DrawChoices_VGCDraft(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_DIFFICULTY_VGC_DRAFT);
    u8 styles[3] = {0};
    int xMid = GetMiddleX(sText_VGCDraft_None, sText_VGCDraft_Draft, sText_VGCDraft_Both);
    styles[selection] = 1;

    DrawOptionMenuChoice(sText_VGCDraft_None, 104, y, styles[0], active);
    DrawOptionMenuChoice(sText_VGCDraft_Draft, xMid, y, styles[1], active);
    DrawOptionMenuChoice(sText_VGCDraft_Both, GetStringRightAlignXOffset(1, sText_VGCDraft_Both, 198), y, styles[2], active);
}

static void DrawChoices_Sound(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_SOUND);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_SoundMono, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_SoundStereo, GetStringRightAlignXOffset(FONT_NORMAL, gText_SoundStereo, 198), y, styles[1], active);
}

static void DrawChoices_ButtonMode(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_BUTTONMODE);
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_ButtonTypeNormal, gText_ButtonTypeLR, gText_ButtonTypeLEqualsA);
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_ButtonTypeNormal, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_ButtonTypeLR, xMid, y, styles[1], active);
    DrawOptionMenuChoice(gText_ButtonTypeLEqualsA, GetStringRightAlignXOffset(1, gText_ButtonTypeLEqualsA, 198), y, styles[2], active);
}

static void DrawChoices_UnitSystem(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_UNIT_SYSTEM);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_UnitSystemImperial, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_UnitSystemMetric, GetStringRightAlignXOffset(1, gText_UnitSystemMetric, 198), y, styles[1], active);
}

static void DrawChoices_MatchCall(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_MATCHCALL);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(1, gText_BattleSceneOff, 198), y, styles[1], active);
}

static const u8 sText_RandomiserNormal[] = _("Normal");
static const u8 sText_RandomiserRandom[] = _("Random");
static void DrawChoices_RandWild(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_RANDOM_WILD);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(sText_RandomiserNormal, 104, y, styles[0], active);
    DrawOptionMenuChoice(sText_RandomiserRandom, GetStringRightAlignXOffset(1, sText_RandomiserRandom, 198), y, styles[1], active);
}

static void DrawChoices_RandTrain(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_RANDOM_TRAIN);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(sText_RandomiserNormal, 104, y, styles[0], active);
    DrawOptionMenuChoice(sText_RandomiserRandom, GetStringRightAlignXOffset(1, sText_RandomiserRandom, 198), y, styles[1], active);
}

static void DrawChoices_RandAbil(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_RANDOM_RANDABIL);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(sText_RandomiserNormal, 104, y, styles[0], active);
    DrawOptionMenuChoice(sText_RandomiserRandom, GetStringRightAlignXOffset(1, sText_RandomiserRandom, 198), y, styles[1], active);
}

// Background tilemap
#define TILE_TOP_CORNER_L 0x1A2 // 418
#define TILE_TOP_EDGE     0x1A3 // 419
#define TILE_TOP_CORNER_R 0x1A4 // 420
#define TILE_LEFT_EDGE    0x1A5 // 421
#define TILE_RIGHT_EDGE   0x1A7 // 423
#define TILE_BOT_CORNER_L 0x1A8 // 424
#define TILE_BOT_EDGE     0x1A9 // 425
#define TILE_BOT_CORNER_R 0x1AA // 426

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Option Texts window
    //FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  2,  1,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  2, 26,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  2,  1,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  3,  1, 16,  7);
    //FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  3,  1, 16,  7);
    //FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 13,  1,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 13, 26,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 13,  1,  1,  7);

    // Description window
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1, 14,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2, 14, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28, 14,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1, 15,  1,  4,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28, 15,  1,  4,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}

static const u8 sText_ChangedSeed[] = _("Changed randomiser seed.{PAUSE_UNTIL_PRESS}");
static const u8 sText_InvalidSeed[] = _("Invalid seed.{PAUSE_UNTIL_PRESS}");

static void DoRngSeedScreen(void)
{
    ConvertUIntToDecimalStringN(gStringVar2, gSaveBlock2Ptr->randomiserSeed, STR_CONV_MODE_LEFT_ALIGN, 10);
    DoNamingScreen(NAMING_SCREEN_RNG_SEED, gStringVar2, 0, 0, 0, CB2_HandleGivenSeed);
}

static void CB2_HandleGivenSeed(void)
{
    gFieldCallback = MapPostLoadHook_ReturnToSeedChange;
    SetMainCallback2(CB2_ReturnToField);
}

static void MapPostLoadHook_ReturnToSeedChange(void)
{
    FadeInFromBlack();
    CreateTask(Task_ReturnToSeedChange, 8);
}

static void Task_ReturnToSeedChange(u8 taskId)
{
    u32 seed;

    if (IsWeatherNotFadingIn() == TRUE)
    {
        if (ParseWholeUnsigned(gStringVar2, &seed)) {
            gSaveBlock2Ptr->randomiserSeed = seed;
            FlagSet(FLAG_SYS_INVALID_CHALLENGE);
            DisplayItemMessageOnField(taskId, sText_ChangedSeed, Task_OptionMenuSave);
        } else {
            DisplayItemMessageOnField(taskId, sText_InvalidSeed, Task_OptionMenuSave);
        }
    }
}
