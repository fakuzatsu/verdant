// ============================================================================
// PC Storage System - Data
// ============================================================================

// Wallpaper IDs
enum {
    WALLPAPER_BASE,
    WALLPAPER_NORMAL,
    WALLPAPER_FIGHTING,
    WALLPAPER_FLYING,
    WALLPAPER_POISON,
    WALLPAPER_GROUND,
    WALLPAPER_ROCK,
    WALLPAPER_BUG,
    WALLPAPER_GHOST,
    WALLPAPER_STEEL,
    WALLPAPER_FIRE,
    WALLPAPER_WATER,
    WALLPAPER_GRASS,
    WALLPAPER_ELECTRIC,
    WALLPAPER_PSYCHIC,
    WALLPAPER_ICE,
    WALLPAPER_DRAGON,
    WALLPAPER_DARK,
    WALLPAPER_FAIRY,
    WALLPAPER_STELLAR,
    WALLPAPER_COUNT
};
#define MAX_DEFAULT_WALLPAPER WALLPAPER_FLYING

// ============================================================================
// Structs
// ============================================================================

struct Wallpaper
{
    const u32 *tiles;
    const u32 *tilemap;
    const u16 *palettes;
};

struct StorageMessage
{
    const u8 *text;
    u8 format;
};

// ============================================================================
// Graphics - Storage System UI
// ============================================================================

static const u32 sSwShStorage_Gfx[]           = INCBIN_U32("graphics/pokemon_storage/swsh/tiles.4bpp.smol");
static const u16 sSwShStorage_Pal[]           = INCBIN_U16("graphics/pokemon_storage/swsh/tiles.gbapal");
static const u32 sSwShStorage_BG1_Tilemap[]   = INCBIN_U32("graphics/pokemon_storage/swsh/bg1.bin.lz");
static const u32 sSwShStorage_BG2_Tilemap[]   = INCBIN_U32("graphics/pokemon_storage/swsh/bg2.bin.lz");
static const u32 sMonInfo_Gfx[]               = INCBIN_U32("graphics/pokemon_storage/swsh/mon_info.4bpp.smol");
static const u32 sMonInfo_Tilemap[]           = INCBIN_U32("graphics/pokemon_storage/swsh/mon_info.bin.lz");
static const u16 sTextWindows_Pal[]           = INCBIN_U16("graphics/pokemon_storage/swsh/text_windows.gbapal");

static const u32 sCursor_Gfx[]                = INCBIN_U32("graphics/pokemon_storage/swsh/cursor.4bpp.smol");
static const u16 sCursor_Pal[]                = INCBIN_U16("graphics/pokemon_storage/swsh/cursor.gbapal");
static const u32 sBoxSelection_Gfx[]          = INCBIN_U32("graphics/pokemon_storage/swsh/box_selection.4bpp.smol");
static const u32 sBoxTitleFrame_Gfx[]         = INCBIN_U32("graphics/pokemon_storage/swsh/box_title_frame.4bpp.smol");
static const u32 sBoxTitleArrow_Gfx[]         = INCBIN_U32("graphics/pokemon_storage/swsh/box_title_arrow.4bpp.smol");
static const u32 sGenderIcons_Gfx[]           = INCBIN_U32("graphics/pokemon_storage/swsh/gender_icons.4bpp.smol");
static const u32 sShinyIcon_Gfx[]             = INCBIN_U32("graphics/pokemon_storage/swsh/shiny_icon.4bpp.smol");
static const u32 sStatLabels_Gfx[]            = INCBIN_U32("graphics/pokemon_storage/swsh/stat_labels.4bpp.smol");
static const ALIGNED(4) u8 sTypeIcons_Gfx[]   = INCBIN_U8("graphics/pokemon_storage/swsh/type_icons.4bpp");
static const u16 sTypeIcons_Pal[]             = INCBIN_U16("graphics/pokemon_storage/swsh/type_icons.gbapal");
static const u16 sMarkings_Pal[]              = INCBIN_U16("graphics/pokemon_storage/swsh/markings.gbapal");

static const u32 sItemInfoFrame_Gfx[]         = INCBIN_U32("graphics/pokemon_storage/swsh/item_info_frame.4bpp");

// ============================================================================
// Graphics - Wallpapers
// ============================================================================

static const u32 sWallpaperTiles_Base[]       = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/base.4bpp.smol");
static const u32 sWallpaperTilemap_Base[]     = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/base.bin.lz");
static const u16 sWallpaperPalette_Base[]     = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/base.gbapal");

static const u32 sWallpaperTiles_Normal[]     = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/normal.4bpp.smol");
static const u32 sWallpaperTilemap_Normal[]   = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/normal.bin.lz");
static const u16 sWallpaperPalette_Normal[]   = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/normal.gbapal");

static const u32 sWallpaperTiles_Fighting[]   = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/fighting.4bpp.smol");
static const u32 sWallpaperTilemap_Fighting[] = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/fighting.bin.lz");
static const u16 sWallpaperPalette_Fighting[] = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/fighting.gbapal");

static const u32 sWallpaperTiles_Flying[]     = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/flying.4bpp.smol");
static const u32 sWallpaperTilemap_Flying[]   = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/flying.bin.lz");
static const u16 sWallpaperPalette_Flying[]   = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/flying.gbapal");

static const u32 sWallpaperTiles_Poison[]     = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/poison.4bpp.smol");
static const u32 sWallpaperTilemap_Poison[]   = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/poison.bin.lz");
static const u16 sWallpaperPalette_Poison[]   = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/poison.gbapal");

static const u32 sWallpaperTiles_Ground[]     = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/ground.4bpp.smol");
static const u32 sWallpaperTilemap_Ground[]   = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/ground.bin.lz");
static const u16 sWallpaperPalette_Ground[]   = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/ground.gbapal");

static const u32 sWallpaperTiles_Rock[]       = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/rock.4bpp.smol");
static const u32 sWallpaperTilemap_Rock[]     = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/rock.bin.lz");
static const u16 sWallpaperPalette_Rock[]     = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/rock.gbapal");

static const u32 sWallpaperTiles_Bug[]        = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/bug.4bpp.smol");
static const u32 sWallpaperTilemap_Bug[]      = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/bug.bin.lz");
static const u16 sWallpaperPalette_Bug[]      = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/bug.gbapal");

static const u32 sWallpaperTiles_Ghost[]      = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/ghost.4bpp.smol");
static const u32 sWallpaperTilemap_Ghost[]    = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/ghost.bin.lz");
static const u16 sWallpaperPalette_Ghost[]    = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/ghost.gbapal");

static const u32 sWallpaperTiles_Steel[]      = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/steel.4bpp.smol");
static const u32 sWallpaperTilemap_Steel[]    = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/steel.bin.lz");
static const u16 sWallpaperPalette_Steel[]    = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/steel.gbapal");

static const u32 sWallpaperTiles_Fire[]       = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/fire.4bpp.smol");
static const u32 sWallpaperTilemap_Fire[]     = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/fire.bin.lz");
static const u16 sWallpaperPalette_Fire[]     = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/fire.gbapal");

static const u32 sWallpaperTiles_Water[]      = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/water.4bpp.smol");
static const u32 sWallpaperTilemap_Water[]    = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/water.bin.lz");
static const u16 sWallpaperPalette_Water[]    = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/water.gbapal");

static const u32 sWallpaperTiles_Grass[]      = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/grass.4bpp.smol");
static const u32 sWallpaperTilemap_Grass[]    = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/grass.bin.lz");
static const u16 sWallpaperPalette_Grass[]    = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/grass.gbapal");

static const u32 sWallpaperTiles_Electric[]   = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/electric.4bpp.smol");
static const u32 sWallpaperTilemap_Electric[] = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/electric.bin.lz");
static const u16 sWallpaperPalette_Electric[] = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/electric.gbapal");

static const u32 sWallpaperTiles_Psychic[]    = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/psychic.4bpp.smol");
static const u32 sWallpaperTilemap_Psychic[]  = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/psychic.bin.lz");
static const u16 sWallpaperPalette_Psychic[]  = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/psychic.gbapal");

static const u32 sWallpaperTiles_Ice[]        = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/ice.4bpp.smol");
static const u32 sWallpaperTilemap_Ice[]      = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/ice.bin.lz");
static const u16 sWallpaperPalette_Ice[]      = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/ice.gbapal");

static const u32 sWallpaperTiles_Dragon[]     = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/dragon.4bpp.smol");
static const u32 sWallpaperTilemap_Dragon[]   = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/dragon.bin.lz");
static const u16 sWallpaperPalette_Dragon[]   = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/dragon.gbapal");

static const u32 sWallpaperTiles_Dark[]       = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/dark.4bpp.smol");
static const u32 sWallpaperTilemap_Dark[]     = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/dark.bin.lz");
static const u16 sWallpaperPalette_Dark[]     = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/dark.gbapal");

static const u32 sWallpaperTiles_Fairy[]      = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/fairy.4bpp.smol");
static const u32 sWallpaperTilemap_Fairy[]    = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/fairy.bin.lz");
static const u16 sWallpaperPalette_Fairy[]    = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/fairy.gbapal");

static const u32 sWallpaperTiles_Stellar[]      = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/stellar.4bpp.smol");
static const u32 sWallpaperTilemap_Stellar[]    = INCBIN_U32("graphics/pokemon_storage/swsh/wallpapers/stellar.bin.lz");
static const u16 sWallpaperPalette_Stellar[]    = INCBIN_U16("graphics/pokemon_storage/swsh/wallpapers/stellar.gbapal");

#define WALLPAPER_ENTRY(name) {sWallpaperTiles_##name, sWallpaperTilemap_##name, sWallpaperPalette_##name}

static const struct Wallpaper sSwShWallpapers[] =
{
    [WALLPAPER_BASE]     = WALLPAPER_ENTRY(Base),
    [WALLPAPER_NORMAL]   = WALLPAPER_ENTRY(Normal),
    [WALLPAPER_FIGHTING] = WALLPAPER_ENTRY(Fighting),
    [WALLPAPER_FLYING]   = WALLPAPER_ENTRY(Flying),
    [WALLPAPER_POISON]   = WALLPAPER_ENTRY(Poison),
    [WALLPAPER_GROUND]   = WALLPAPER_ENTRY(Ground),
    [WALLPAPER_ROCK]     = WALLPAPER_ENTRY(Rock),
    [WALLPAPER_BUG]      = WALLPAPER_ENTRY(Bug),
    [WALLPAPER_GHOST]    = WALLPAPER_ENTRY(Ghost),
    [WALLPAPER_STEEL]    = WALLPAPER_ENTRY(Steel),
    [WALLPAPER_FIRE]     = WALLPAPER_ENTRY(Fire),
    [WALLPAPER_WATER]    = WALLPAPER_ENTRY(Water),
    [WALLPAPER_GRASS]    = WALLPAPER_ENTRY(Grass),
    [WALLPAPER_ELECTRIC] = WALLPAPER_ENTRY(Electric),
    [WALLPAPER_PSYCHIC]  = WALLPAPER_ENTRY(Psychic),
    [WALLPAPER_ICE]      = WALLPAPER_ENTRY(Ice),
    [WALLPAPER_DRAGON]   = WALLPAPER_ENTRY(Dragon),
    [WALLPAPER_DARK]     = WALLPAPER_ENTRY(Dark),
    [WALLPAPER_FAIRY]    = WALLPAPER_ENTRY(Fairy),
    [WALLPAPER_STELLAR]  = WALLPAPER_ENTRY(Stellar),
};

// ============================================================================
// Text Strings
// ============================================================================

static const u8 gText_JustOnePkmn[]   = _("There is just one POKéMON with you.");
static const u8 gText_PartyFull[]     = _("Your party is full!");
static const u8 gText_Box[]           = _("BOX");
static const u8 sText_OutOf30[]       = _("/30");
static const u8 sText_Lv[]            = _("Lv");

static const u8 gText_PkmnIsSelected[] = _("{DYNAMIC 0} is selected.");
static const u8 gPCText_Give[]         = _("GIVE");

struct {
    const u8 *text;
    const u8 *desc;
} static const sMainMenuTexts[OPTIONS_COUNT] =
{
    [OPTION_WITHDRAW]   = {COMPOUND_STRING("WITHDRAW POKéMON"), COMPOUND_STRING("Move POKéMON stored in BOXES to\nyour party.")},
    [OPTION_DEPOSIT]    = {COMPOUND_STRING("DEPOSIT POKéMON"),  COMPOUND_STRING("Store POKéMON in your party in BOXES.")},
    [OPTION_MOVE_MONS]  = {COMPOUND_STRING("MOVE POKéMON"),     COMPOUND_STRING("Organize the POKéMON in BOXES and\nin your party.")},
    [OPTION_MOVE_ITEMS] = {COMPOUND_STRING("MOVE ITEMS"),       COMPOUND_STRING("Move items held by any POKéMON\nin a BOX or your party.")},
    [OPTION_EXIT]       = {COMPOUND_STRING("SEE YA!"),          COMPOUND_STRING("Return to the previous menu.")}
};

static const u8 *const sMenuTexts[] =
{
    [MENU_CANCEL]     = COMPOUND_STRING("CANCEL"),
    [MENU_STORE]      = COMPOUND_STRING("STORE"),
    [MENU_WITHDRAW]   = COMPOUND_STRING("WITHDRAW"),
    [MENU_MOVE]       = COMPOUND_STRING("MOVE"),
    [MENU_SHIFT]      = COMPOUND_STRING("SHIFT"),
    [MENU_PLACE]      = COMPOUND_STRING("PLACE"),
    [MENU_SUMMARY]    = COMPOUND_STRING("SUMMARY"),
    [MENU_RELEASE]    = COMPOUND_STRING("RELEASE"),
    [MENU_MARK]       = COMPOUND_STRING("MARK"),
    [MENU_JUMP]       = COMPOUND_STRING("JUMP"),
    [MENU_WALLPAPER]  = COMPOUND_STRING("WALLPAPER"),
    [MENU_NAME]       = COMPOUND_STRING("NAME"),
    [MENU_TAKE]       = COMPOUND_STRING("TAKE"),
    [MENU_GIVE]       = gPCText_Give,
    [MENU_GIVE_2]     = gPCText_Give,
    [MENU_SWITCH]     = COMPOUND_STRING("SWITCH"),
    [MENU_BAG]        = COMPOUND_STRING("BAG"),
    [MENU_INFO]       = COMPOUND_STRING("INFO"),
    [MENU_BASE]       = COMPOUND_STRING("DEFAULT"),
    [MENU_NORMAL]     = COMPOUND_STRING("NORMAL"),
    [MENU_FIGHTING]   = COMPOUND_STRING("FIGHTING"),
    [MENU_FLYING]     = COMPOUND_STRING("FLYING"),
    [MENU_POISON]     = COMPOUND_STRING("POISON"),
    [MENU_GROUND]     = COMPOUND_STRING("GROUND"),
    [MENU_ROCK]       = COMPOUND_STRING("ROCK"),
    [MENU_BUG]        = COMPOUND_STRING("BUG"),
    [MENU_GHOST]      = COMPOUND_STRING("GHOST"),
    [MENU_STEEL]      = COMPOUND_STRING("STEEL"),
    [MENU_FIRE]       = COMPOUND_STRING("FIRE"),
    [MENU_WATER]      = COMPOUND_STRING("WATER"),
    [MENU_GRASS]      = COMPOUND_STRING("GRASS"),
    [MENU_ELECTRIC]   = COMPOUND_STRING("ELECTRIC"),
    [MENU_PSYCHIC]    = COMPOUND_STRING("PSYCHIC"),
    [MENU_ICE]        = COMPOUND_STRING("ICE"),
    [MENU_DRAGON]     = COMPOUND_STRING("DRAGON"),
    [MENU_DARK]       = COMPOUND_STRING("DARK"),
    [MENU_FAIRY]      = COMPOUND_STRING("FAIRY"),
    [MENU_STELLAR]    = COMPOUND_STRING("STELLAR"),
    [MENU_COUNT]      = gText_EmptyString2,
};

// ============================================================================
// Messages
// ============================================================================

static const struct StorageMessage sMessages[] =
{
    [MSG_EXIT_BOX]             = {COMPOUND_STRING("Exit from the BOX?"),         MSG_VAR_NONE},
    [MSG_WHAT_YOU_DO]          = {COMPOUND_STRING("What do you want to do?"),    MSG_VAR_NONE},
    [MSG_IS_SELECTED]          = {gText_PkmnIsSelected,                          MSG_VAR_MON_NAME_1},
    [MSG_JUMP_TO_WHICH_BOX]    = {COMPOUND_STRING("Jump to which BOX?"),         MSG_VAR_NONE},
    [MSG_DEPOSIT_IN_WHICH_BOX] = {COMPOUND_STRING("Deposit in which BOX?"),      MSG_VAR_NONE},
    [MSG_WAS_DEPOSITED]        = {COMPOUND_STRING("{DYNAMIC 0} was deposited."), MSG_VAR_MON_NAME_1},
    [MSG_BOX_IS_FULL]          = {COMPOUND_STRING("The BOX is full."),           MSG_VAR_NONE},
    [MSG_RELEASE_POKE]         = {COMPOUND_STRING("Release this POKéMON?"),      MSG_VAR_NONE},
    [MSG_WAS_RELEASED]         = {COMPOUND_STRING("{DYNAMIC 0} was released."),  MSG_VAR_RELEASE_MON_1},
    [MSG_BYE_BYE]              = {COMPOUND_STRING("Bye-bye, {DYNAMIC 0}!"),      MSG_VAR_RELEASE_MON_3},
    [MSG_MARK_POKE]            = {COMPOUND_STRING("Mark your POKéMON."),         MSG_VAR_NONE},
    [MSG_LAST_POKE]            = {COMPOUND_STRING("That's your last POKéMON!"),  MSG_VAR_NONE},
    [MSG_PARTY_FULL]           = {gText_YourPartysFull,                          MSG_VAR_NONE},
    [MSG_HOLDING_POKE]         = {COMPOUND_STRING("You're holding a POKéMON!"),  MSG_VAR_NONE},
    [MSG_WHICH_ONE_WILL_TAKE]  = {COMPOUND_STRING("Which one will you take?"),   MSG_VAR_NONE},
    [MSG_CANT_RELEASE_EGG]     = {COMPOUND_STRING("You can't release an EGG."),  MSG_VAR_NONE},
    [MSG_CONTINUE_BOX]         = {COMPOUND_STRING("Continue BOX operations?"),   MSG_VAR_NONE},
    [MSG_CAME_BACK]            = {COMPOUND_STRING("{DYNAMIC 0} came back!"),     MSG_VAR_MON_NAME_1},
    [MSG_WORRIED]              = {COMPOUND_STRING("Was it worried about you?"),  MSG_VAR_NONE},
    [MSG_SURPRISE]             = {COMPOUND_STRING("… … … … !"),                  MSG_VAR_NONE},
    [MSG_PLEASE_REMOVE_MAIL]   = {COMPOUND_STRING("Please remove the MAIL."),    MSG_VAR_NONE},
    [MSG_IS_SELECTED2]         = {gText_PkmnIsSelected,                          MSG_VAR_ITEM_NAME},
    [MSG_GIVE_TO_MON]          = {COMPOUND_STRING("GIVE to a POKéMON?"),         MSG_VAR_NONE},
    [MSG_PLACED_IN_BAG]        = {COMPOUND_STRING("Placed item in the BAG."),    MSG_VAR_ITEM_NAME},
    [MSG_BAG_FULL]             = {COMPOUND_STRING("The BAG is full."),           MSG_VAR_NONE},
    [MSG_PUT_IN_BAG]           = {COMPOUND_STRING("Put this item in the BAG?"),  MSG_VAR_NONE},
    [MSG_ITEM_IS_HELD]         = {COMPOUND_STRING("{DYNAMIC 0} is now held."),   MSG_VAR_ITEM_NAME},
    [MSG_CHANGED_TO_ITEM]      = {COMPOUND_STRING("Changed to {DYNAMIC 0}."),    MSG_VAR_ITEM_NAME},
    [MSG_CANT_STORE_MAIL]      = {COMPOUND_STRING("MAIL can't be stored!"),      MSG_VAR_NONE},
};

// ============================================================================
// Window Templates
// ============================================================================

static const struct WindowTemplate sWindowTemplate_MainMenu =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 17,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 0x1,
};

static const struct WindowTemplate sYesNoWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 24,
    .tilemapTop = 11,
    .width = 5,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x5C,
};

static const struct WindowTemplate sWindowTemplate_MultiMove =
{
    .bg = 0,
    .tilemapLeft = 10,
    .tilemapTop = 3,
    .width = 19,
    .height = 16,
    .paletteNum = 9,
    .baseBlock = 0x1,
};

static const struct WindowTemplate sWindowTemplates[] =
{
    [WIN_MESSAGE] = {
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 17,
        .width = 18,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 44,
    },
    [WIN_ITEM_DESC] = {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 13,
        .width = 21,
        .height = 7,
        .paletteNum = 15,
        .baseBlock = 44,
    },
    [WIN_MON_INFO_NICKNAME_LEFT] = {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 23,
        .width = 8,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 202
    },
    [WIN_MON_INFO_LEVEL_LEFT] = {
        .bg = 0,
        .tilemapLeft = 9,
        .tilemapTop = 23,
        .width = 4,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 218
    },
    [WIN_MON_INFO_STATS_COL1_LEFT] = {
        .bg = 0,
        .tilemapLeft = 3,
        .tilemapTop = 27,
        .width = 3,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 226
    },
    [WIN_MON_INFO_STATS_COL2_LEFT] = {
        .bg = 0,
        .tilemapLeft = 10,
        .tilemapTop = 27,
        .width = 3,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 244
    },
    [WIN_MON_INFO_ABILITY_LEFT] = {
        .bg = 0,
        .tilemapLeft = 4,
        .tilemapTop = 34,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 262
    },
    [WIN_MON_INFO_ITEM_LEFT] = {
        .bg = 0,
        .tilemapLeft = 4,
        .tilemapTop = 36,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 280
    },
    [WIN_MON_INFO_NICKNAME_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 17,
        .tilemapTop = 43,
        .width = 8,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 202
    },
    [WIN_MON_INFO_LEVEL_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 26,
        .tilemapTop = 43,
        .width = 4,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 218
    },
    [WIN_MON_INFO_STATS_COL1_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 20,
        .tilemapTop = 47,
        .width = 3,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 226
    },
    [WIN_MON_INFO_STATS_COL2_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 27,
        .tilemapTop = 47,
        .width = 3,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 244
    },
    [WIN_MON_INFO_ABILITY_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 54,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 262
    },
    [WIN_MON_INFO_ITEM_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 56,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 280
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sTextColors[][3] =
{
    {1, 2, 3}, // Standard menus, mon info (stats, ability, item)
    {4, 2, 5}, // Mon info (nickname and level) (grey BG)
    {0, 4, 7}, // Choose box menu
};


// ============================================================================
// BG Templates
// ============================================================================

static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 26,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 27,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = 3,
        .charBaseIndex = 3,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
};

// ============================================================================
// Choose Box Menu Sprites
// ============================================================================

static const struct OamData sOamData_BoxSelection =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .size = SPRITE_SIZE(32x64),
    .x = 0,
    .matrixNum = 0,
    .shape = SPRITE_SHAPE(32x64),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_BoxSelectionLeft[] = {
    ANIMCMD_FRAME(0, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_BoxSelectionRight[] = {
    ANIMCMD_FRAME(0, 0, TRUE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_BoxSelection[] = {
    sSpriteAnim_BoxSelectionLeft,
    sSpriteAnim_BoxSelectionRight,
};

static const struct CompressedSpriteSheet sSpriteSheet_BoxSelection =
{
    .data = sBoxSelection_Gfx,
    .size = (32 * 64) / 2,
    .tag = GFXTAG_BOX_SELECTION,
};

static const struct SpriteTemplate sSpriteTemplate_BoxSelection =
{
    .tileTag = GFXTAG_BOX_SELECTION,
    .paletteTag = PALTAG_MISC_3,
    .oam = &sOamData_BoxSelection,
    .anims = sSpriteAnimTable_BoxSelection,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// ============================================================================
// Box Selection Text Overlay Sprite (loaded dynamically like BOX_TITLE)
// ============================================================================

static const struct OamData sOamData_BoxSelectionText =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .size = SPRITE_SIZE(64x32),
    .x = 0,
    .matrixNum = 0,
    .shape = SPRITE_SHAPE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteTemplate sSpriteTemplate_BoxSelectionText =
{
    .tileTag = GFXTAG_BOX_SELECTION_BOX_NAME,
    .paletteTag = PALTAG_MISC_3,
    .oam = &sOamData_BoxSelectionText,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSpriteTemplate_BoxSelectionText2 =
{
    .tileTag = GFXTAG_BOX_SELECTION_PER_30,
    .paletteTag = PALTAG_MISC_3,
    .oam = &sOamData_BoxSelectionText,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// ============================================================================
// Box Title Frame Sprites
// ============================================================================

static const struct OamData sOamData_BoxTitleFrame =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .size = SPRITE_SIZE(16x16),
    .x = 0,
    .matrixNum = 0,
    .shape = SPRITE_SHAPE(16x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_BoxTitleFrameLeft[] = {
    ANIMCMD_FRAME(0, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_BoxTitleFrameMiddle[] = {
    ANIMCMD_FRAME(4, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_BoxTitleFrameRight[] = {
    ANIMCMD_FRAME(0, 0, TRUE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_BoxTitleFrame[] = {
    sSpriteAnim_BoxTitleFrameLeft,
    sSpriteAnim_BoxTitleFrameMiddle,
    sSpriteAnim_BoxTitleFrameRight,
};

static const struct CompressedSpriteSheet sSpriteSheet_BoxTitleFrame =
{
    .data = sBoxTitleFrame_Gfx,
    .size = (16 * 16 * 2) / 2,
    .tag = GFXTAG_BOX_TITLE_FRAME,
};

static const struct SpriteTemplate sSpriteTemplate_BoxTitleFrame =
{
    .tileTag = GFXTAG_BOX_TITLE_FRAME,
    .paletteTag = PALTAG_MISC_1,
    .oam = &sOamData_BoxTitleFrame,
    .anims = sSpriteAnimTable_BoxTitleFrame,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// ============================================================================
// Box Title Arrow Sprites
// ============================================================================

static const struct OamData sOamData_BoxTitleArrow =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .size = SPRITE_SIZE(8x8),
    .x = 0,
    .matrixNum = 0,
    .shape = SPRITE_SHAPE(8x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_BoxTitleArrowLeft[] = {
    ANIMCMD_FRAME(0, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_BoxTitleArrowRight[] = {
    ANIMCMD_FRAME(0, 0, TRUE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_BoxTitleArrow[] = {
    sSpriteAnim_BoxTitleArrowLeft,
    sSpriteAnim_BoxTitleArrowRight,
};

static const struct CompressedSpriteSheet sSpriteSheet_BoxTitleArrow =
{
    .data = sBoxTitleArrow_Gfx,
    .size = (8 * 8) / 2,
    .tag = GFXTAG_BOX_TITLE_ARROW,
};

static const struct SpriteTemplate sSpriteTemplate_BoxTitleArrow =
{
    .tileTag = GFXTAG_BOX_TITLE_ARROW,
    .paletteTag = PALTAG_MISC_1,
    .oam = &sOamData_BoxTitleArrow,
    .anims = sSpriteAnimTable_BoxTitleArrow,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Arrow,
};

// ============================================================================
// Box Title Sprites
// ============================================================================

static const u16 sUnusedColor = RGB(26, 29, 8);

#define BOX_TITLE_SHADOW_HOVER  RGB(7, 7, 7)
#define BOX_TITLE_TEXT_HOVER    RGB_WHITE
#define BOX_TITLE_FRAME_HOVER   RGB_BLACK
#define BOX_TITLE_SHADOW_MAIN   RGB(26, 26, 25)
#define BOX_TITLE_TEXT_MAIN     RGB_BLACK
#define BOX_TITLE_FRAME_MAIN    RGB_WHITE

static const struct OamData sOamData_BoxTitle =
{
    .shape = SPRITE_SHAPE(32x16),
    .size = SPRITE_SIZE(32x16),
    .priority = 1
};

static const union AnimCmd sAnim_BoxTitle_Left[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END
};

static const union AnimCmd sAnim_BoxTitle_Right[] =
{
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_END
};

static const union AnimCmd *const sAnims_BoxTitle[] =
{
    sAnim_BoxTitle_Left,
    sAnim_BoxTitle_Right
};

static const struct SpriteTemplate sSpriteTemplate_BoxTitle =
{
    .tileTag = GFXTAG_BOX_TITLE,
    .paletteTag = PALTAG_MISC_1,
    .oam = &sOamData_BoxTitle,
    .anims = sAnims_BoxTitle,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

// ============================================================================
// Cursor Sprites
// ============================================================================

static const struct CompressedSpriteSheet sSpriteSheet_Cursor[] =
{
    {
        .data = sCursor_Gfx,
        .size = (16 * 16 * 3) / 2,
        .tag = GFXTAG_CURSOR,
    },
    {},
};

static const struct SpritePalette sSpritePal_Cursor[] =
{
    {
        .data = sCursor_Pal,
        .tag = PALTAG_MISC_1,
    },
    {
        .data = sCursor_Pal + 16,
        .tag = PALTAG_MISC_2,
    },
    {
        .data = sCursor_Pal + 32,
        .tag = PALTAG_MISC_3,
    },
    {},
};

static const struct OamData sOamData_Cursor =
{
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 1,
};

static const union AnimCmd sAnim_Cursor_Bouncing[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(8, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Cursor_Main[] =
{
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sAnims_Cursor[] =
{
    [CURSOR_ANIM_BOUNCE] = sAnim_Cursor_Bouncing,
    [CURSOR_ANIM_MAIN]   = sAnim_Cursor_Main,
};

static const struct SpriteTemplate sSpriteTemplate_Cursor =
{
    .tileTag = GFXTAG_CURSOR,
    .paletteTag = PALTAG_MISC_1,
    .oam = &sOamData_Cursor,
    .anims = sAnims_Cursor,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// ============================================================================
// Mon Icon Sprites
// ============================================================================

static const struct OamData sOamData_MonIcon;
static const struct SpriteTemplate sSpriteTemplate_MonIcon =
{
    .tileTag = GFXTAG_MON_ICON,
    .paletteTag = PALTAG_MON_ICON_0,
    .oam = &sOamData_MonIcon,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct OamData sOamData_MonIcon =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AffineAnimCmd sAffineAnim_ReleaseMon_Release[] =
{
    AFFINEANIMCMD_FRAME(-2, -2, 0, 120),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_ReleaseMon_CameBack[] =
{
    AFFINEANIMCMD_FRAME(16, 16, 0, 0),
    AFFINEANIMCMD_FRAME(16, 16, 0, 15),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sAffineAnims_ReleaseMon[] =
{
    [RELEASE_ANIM_RELEASE]   = sAffineAnim_ReleaseMon_Release,
    [RELEASE_ANIM_CAME_BACK] = sAffineAnim_ReleaseMon_CameBack
};

// ============================================================================
// Gender Icon Sprites
// ============================================================================

static const struct OamData sOamData_GenderIcons =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 7,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_GenderFemale[] = {
    ANIMCMD_FRAME(0, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_GenderMale[] = {
    ANIMCMD_FRAME(2, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_GenderIcons[] = {
    sSpriteAnim_GenderFemale,
    sSpriteAnim_GenderMale,
};

static const struct CompressedSpriteSheet sSpriteSheet_GenderIcons =
{
    .data = sGenderIcons_Gfx,
    .size = (8 * 16 * 2) / 2,
    .tag = GFXTAG_GENDER_ICON
};

static const struct SpriteTemplate sSpriteTemplate_GenderIcons =
{
    .tileTag = GFXTAG_GENDER_ICON,
    .paletteTag = PALTAG_MISC_2,
    .oam = &sOamData_GenderIcons,
    .anims = sSpriteAnimTable_GenderIcons,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

// ============================================================================
// Shiny Icon Sprites
// ============================================================================

static const struct OamData sOamData_ShinyIcon =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct CompressedSpriteSheet sSpriteSheet_ShinyIcon =
{
    .data = sShinyIcon_Gfx,
    .size = (8 * 8) / 2,
    .tag = GFXTAG_SHINY_ICON
};

static const struct SpriteTemplate sSpriteTemplate_ShinyIcon =
{
    .tileTag = GFXTAG_SHINY_ICON,
    .paletteTag = PALTAG_MISC_2,
    .oam = &sOamData_ShinyIcon,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

// ============================================================================
// Stat Label Sprites
// ============================================================================

static const struct OamData sOamData_StatLabels =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 0,
};

static const union AnimCmd sSpriteAnim_StatAtk[] = {
    ANIMCMD_FRAME(0, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatDef[] = {
    ANIMCMD_FRAME(4, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatSpAtk[] = {
    ANIMCMD_FRAME(8, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatSpDef[] = {
    ANIMCMD_FRAME(12, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatSpeed[] = {
    ANIMCMD_FRAME(16, 0, FALSE, FALSE),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_StatLabels[] = {
    sSpriteAnim_StatAtk,
    sSpriteAnim_StatDef,
    sSpriteAnim_StatSpAtk,
    sSpriteAnim_StatSpDef,
    sSpriteAnim_StatSpeed,
};

static const struct CompressedSpriteSheet sSpriteSheet_StatLabels =
{
    .data = sStatLabels_Gfx,
    .size = (16 * 16 * 5) / 2,
    .tag = GFXTAG_STAT_LABELS
};

static const struct SpriteTemplate sSpriteTemplate_StatLabels =
{
    .tileTag = GFXTAG_STAT_LABELS,
    .paletteTag = PALTAG_MISC_2,
    .oam = &sOamData_StatLabels,
    .anims = sSpriteAnimTable_StatLabels,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

// ============================================================================
// Type Icon Sprites
// ============================================================================

static const struct OamData sOamData_TypeIcons =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

// Type icons now use DMA copy instead of animations to save VRAM (only loads 2 slots instead of all 21 types)
// Uncompressed sprite sheet (only 2 slots loaded to save VRAM)
static const struct SpriteSheet sSpriteSheet_TypeIcons =
{
    .data = sTypeIcons_Gfx,
    .size = 2 * 0x100, // Only load 2 type icon slots (saves 4.75 KB VRAM)
    .tag = GFXTAG_TYPE_ICON,
};

static const struct SpriteTemplate sSpriteTemplate_TypeIcons =
{
    .tileTag = GFXTAG_TYPE_ICON,
    .paletteTag = PALTAG_TYPE_ICON,
    .oam = &sOamData_TypeIcons,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

// ============================================================================
// Item Icon Sprites
// ============================================================================

static const struct OamData sOamData_ItemIcon =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AffineAnimCmd sAffineAnim_ItemIcon_Small[] =
{
    AFFINEANIMCMD_FRAME(192, 192, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_ItemIcon_Appear[] =
{
    AFFINEANIMCMD_FRAME(152, 152, 0, 0),
    AFFINEANIMCMD_FRAME(5, 5, 0, 8),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_ItemIcon_Disappear[] =
{
    AFFINEANIMCMD_FRAME(192, 192, 0, 0),
    AFFINEANIMCMD_FRAME(-5, -5, 0, 8),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_ItemIcon_PickUp[] =
{
    AFFINEANIMCMD_FRAME(192, 192, 0, 0),
    AFFINEANIMCMD_FRAME(6, 6, 0, 12),
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_ItemIcon_PutDown[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_FRAME(-6, -6, 0, 12),
    AFFINEANIMCMD_FRAME(192, 192, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_ItemIcon_PutAway[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_FRAME(-5, -5, 0, 16),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_ItemIcon_Large[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sAffineAnims_ItemIcon[] =
{
    [ITEM_ANIM_NONE]      = sAffineAnim_ItemIcon_Small,
    [ITEM_ANIM_APPEAR]    = sAffineAnim_ItemIcon_Appear,
    [ITEM_ANIM_DISAPPEAR] = sAffineAnim_ItemIcon_Disappear,
    [ITEM_ANIM_PICK_UP]   = sAffineAnim_ItemIcon_PickUp,
    [ITEM_ANIM_PUT_DOWN]  = sAffineAnim_ItemIcon_PutDown,
    [ITEM_ANIM_PUT_AWAY]  = sAffineAnim_ItemIcon_PutAway,
    [ITEM_ANIM_LARGE]     = sAffineAnim_ItemIcon_Large
};

static const struct SpriteTemplate sSpriteTemplate_ItemIcon =
{
    .tileTag = GFXTAG_ITEM_ICON_0,
    .paletteTag = PALTAG_ITEM_ICON_0,
    .oam = &sOamData_ItemIcon,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_ItemIcon,
    .callback = SpriteCallbackDummy,
};
