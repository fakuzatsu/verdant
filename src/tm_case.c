#include "global.h"
#include "malloc.h"
#include "bg.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "task.h"
#include "text_window.h"
#include "menu.h"
#include "menu_helpers.h"
#include "tm_case.h"
#include "list_menu.h"
#include "item.h"
#include "item_menu.h"
#include "link.h"
#include "money.h"
#include "palette.h"
#include "pokemon_icon.h"
#include "pokemon_summary_screen.h"
#include "pokemon_storage_system.h"
#include "party_menu.h"
#include "data.h"
#include "scanline_effect.h"
#include "shop.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/rgb.h"

// Window IDs for the TM case
enum {
    WIN_LIST,
    WIN_DESCRIPTION,
    WIN_SELECTED_MSG,
    WIN_TITLE,
    WIN_MOVE_INFO_LABELS,
    WIN_MOVE_INFO,
    WIN_MESSAGE,
    WIN_SELL_QUANTITY,
    WIN_MONEY,
};

// Window IDs for the context menu that opens when a TM/HM is selected
enum {
    WIN_USE_GIVE_EXIT,
    WIN_GIVE_EXIT,
};

enum {
    COLOR_LIGHT,
    COLOR_DARK,
    COLOR_CURSOR_SELECTED,
    COLOR_MOVE_INFO,
    COLOR_TITLE,
    COLOR_CURSOR_ERASE = 0xFF
};

#define TM_CASE_DISC_TAG 400
#define TAG_SCROLL_ARROW 110

struct TMCaseStaticResources
{
    void (* savedCallback)(void);
    u8 tmCaseMenuType;
    u8 allowSelectClose;
    u8 unused;
    u16 selectedRow;
    u16 scrollOffset;
};

struct TMCaseDynamicResources
{
    void (* savedCallback)(void);
    u8 tmSpriteId;
    u8 maxTMsShown;
    u8 numTMs;
    u8 contextMenuWindowId;
    u8 scrollIndicatorArrowPairId;
    u16 currItem;
    const u8 * menuActionIndices;
    u8 numMenuActions;
    s16 seqId;
    u8 filler_14[8];
};

static EWRAM_DATA struct TMCaseStaticResources sTMCaseStaticResources = {};
static EWRAM_DATA struct TMCaseDynamicResources * sTMCaseDynamicResources = NULL;
static EWRAM_DATA void * sTilemapBuffer = NULL; // tilemap buffer
static EWRAM_DATA struct ListMenuItem * sListMenuItemsBuffer = NULL;
static EWRAM_DATA u8 (* sListMenuStringsBuffer)[29] = NULL;
static EWRAM_DATA u16 * sTMSpritePaletteBuffer = NULL;
static EWRAM_DATA u8    spriteIdData[PARTY_SIZE] = {};
static EWRAM_DATA u16   spriteIdPalette[PARTY_SIZE] = {};

static void CB2_SetUpTMCaseUI_Blocking(void);
static bool8 DoSetUpTMCaseUI(void);
static void ResetBufferPointers_NoFree(void);
static void LoadBGTemplates(void);
static bool8 HandleLoadTMCaseGraphicsAndPalettes(void);
static void CreateTMCaseListMenuBuffers(void);
static void InitTMCaseListMenuItems(void);
static void GetTMNumberAndMoveString(u8 * dest, u16 itemId);
static void TMCase_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void TMCase_ItemPrintFunc(u8 windowId, u32 itemId, u8 y);
static void TMCase_MoveCursor_UpdatePrintedDescription(s32 itemIndex);
static void PrintListMenuCursorAt_WithColorIdx(u8 a0, u8 a1);
static void CreateTMCaseScrollIndicatorArrowPair_Main(void);
static void TMCaseSetup_GetTMCount(void);
static void TMCaseSetup_InitListMenuPositions(void);
static void TMCaseSetup_UpdateVisualMenuOffset(void);
static void Task_FadeOutAndCloseTMCase(u8 taskId);
static void Task_TMCaseMain(u8 taskId);
static void Task_SelectTMAction_FromFieldBag(u8 taskId);
static void Task_TMContextMenu_HandleInput(u8 taskId);
static void TMHMContextMenuAction_Use(u8 taskId);
static void PrintError_ThereIsNoPokemon(u8 taskId);
static void PrintError_ItemCantBeHeld(u8 taskId);
static void Task_WaitButtonAfterErrorPrint(u8 taskId);
static void Subtask_CloseContextMenuAndReturnToMain(u8 taskId);
static void TMHMContextMenuAction_Exit(u8 taskId);
static void Task_SelectTMAction_FromGiveParty(u8 taskId);
static void Task_SelectTMAction_FromGivePC(u8 taskId);
static void InitWindowTemplatesAndPals(void);
static void AddTextPrinterParameterized_ColorByIndex(u8 windowId, u8 fontId, const u8 * str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx);
static void TMCase_SetWindowBorder1(u8 windowId);
static void TMCase_SetWindowBorder2(u8 windowId);
static void TMCase_SetWindowBorder3(u8 windowId);
static void TMCase_PrintMessageWithFollowupTask(u8 taskId, u8 fontId, const u8 * str, TaskFunc func);
static void PrintTMCaseTitle(void);
static void DrawMoveInfoUIMarkers(void);
static void TMCase_MoveCursor_UpdatePrintedTMInfo(u16 itemId);
static void PlaceHMTileInWindow(u8 windowId, u8 x, u8 y);
static void HandlePrintMoneyOnHand(void);
static void HandleCreateYesNoMenu(u8 taskId, const struct YesNoFuncTable * ptrs);
static u8 AddTMContextMenu(u8 * a0, u8 a1);
static void RemoveTMContextMenu(u8 * a0);
static void DrawPartyMonIcons(void);
static void TintPartyMonIcons(u16 tm);
static void DestroyPartyMonIcons(void);

extern const struct SpritePalette gMonIconPaletteTable[6];

static const struct BgTemplate sBGTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }, 
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, 
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x000
    }
};

static void (*const sSelectTMActionTasks[])(u8 taskId) = {
    [TMCASE_FROM_FIELD] = Task_SelectTMAction_FromFieldBag,
    [TMCASE_FROM_GIVE_PARTY] = Task_SelectTMAction_FromGiveParty,
    [TMCASE_FROM_GIVE_PC] = Task_SelectTMAction_FromGivePC
};

static const struct MenuAction sMenuActions_UseGiveExit[] = {
    {gMenuText_Use,  {TMHMContextMenuAction_Use}},
    {gText_Cancel,   {TMHMContextMenuAction_Exit}},
};

static const u8 sMenuActionIndices_Field[] = {0, 1};
static const u8 sMenuActionIndices_UnionRoom[] = {0, 1};

static const u8 sText_ClearTo18[] = _("{CLEAR_TO 18}");
static const u8 sText_SingleSpace[] = _(" ");

const u8 gItemDescription_ITEM_TM_CASE[] = _("A case that holds TMs and HMs.\nIt is attached to the Bag's\ncompartment for important items.");

static ALIGNED(4) const u16 sPal3Override[] = {RGB(8, 8, 8), RGB(30, 16, 6)};

static const u8 sTMCaseTextColors[][3] = {
    [COLOR_LIGHT]           = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY},
    [COLOR_DARK]            = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
    [COLOR_CURSOR_SELECTED] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GRAY, TEXT_COLOR_GREEN},
    [COLOR_MOVE_INFO]       = {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_5, TEXT_DYNAMIC_COLOR_1},
    [COLOR_TITLE]           = {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_5, TEXT_COLOR_LIGHT_GRAY},
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
};

static const struct WindowTemplate sWindowTemplates[] = {
    [WIN_LIST] = {
        .bg = 0,
        .tilemapLeft = 14,
        .tilemapTop = 1,
        .width = 15,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 0x081
    },
    [WIN_DESCRIPTION] = {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 12,
        .width = 18,
        .height = 8,
        .paletteNum = 10,
        .baseBlock = 0x13f
    },
    [WIN_SELECTED_MSG] = {
        .bg = 1,
        .tilemapLeft = 5,
        .tilemapTop = 15,
        .width = 15,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x1f9
    },
    [WIN_TITLE] = {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 1,
        .width = 10,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x235
    },
    [WIN_MOVE_INFO_LABELS] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 13,
        .width = 5,
        .height = 6,
        .paletteNum = 12,
        .baseBlock = 0x249
    },
    [WIN_MOVE_INFO] = {
        .bg = 0,
        .tilemapLeft = 7,
        .tilemapTop = 13,
        .width = 5,
        .height = 6,
        .paletteNum = 12,
        .baseBlock = 0x267
    },
    [WIN_MESSAGE] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 11,
        .baseBlock = 0x285
    },
    [WIN_SELL_QUANTITY] = {
        .bg = 1,
        .tilemapLeft = 17,
        .tilemapTop = 9,
        .width = 12,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x2ed
    },
    [WIN_MONEY] = {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 8,
        .height = 3,
        .paletteNum = 13,
        .baseBlock = 0x31d
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sYesNoWindowTemplate = {
    .bg = 1,
    .tilemapLeft = 21,
    .tilemapTop = 9,
    .width = 6,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x335
};

static const struct WindowTemplate sWindowTemplates_ContextMenu[] = {
    [WIN_USE_GIVE_EXIT] = {
        .bg = 1,
        .tilemapLeft = 22,
        .tilemapTop = 13,
        .width = 7,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 0x1cf
    },
    [WIN_GIVE_EXIT] = {
        .bg = 1,
        .tilemapLeft = 22,
        .tilemapTop = 15,
        .width = 7,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x1cf
    },
};

static const struct CompressedSpriteSheet sTMSpriteSheet = {
    .data = (const void *)gTMCaseDisc_Gfx,
    .size = 0x400,
    .tag = TM_CASE_DISC_TAG
};

void InitTMCase(u8 type, void (* exitCallback)(void), u8 allowSelectClose)
{
    ResetBufferPointers_NoFree();
    sTMCaseDynamicResources = Alloc(sizeof(struct TMCaseDynamicResources));
    sTMCaseDynamicResources->savedCallback = NULL;
    sTMCaseDynamicResources->scrollIndicatorArrowPairId = TASK_NONE;
    sTMCaseDynamicResources->contextMenuWindowId = WINDOW_NONE;
    if (type != TMCASE_REOPENING)
        sTMCaseStaticResources.tmCaseMenuType = type;
    if (exitCallback != NULL)
        sTMCaseStaticResources.savedCallback = exitCallback;
    if (allowSelectClose != TMCASE_KEEP_PREV)
        sTMCaseStaticResources.allowSelectClose = allowSelectClose;
    gTextFlags.autoScroll = FALSE;
    SetMainCallback2(CB2_SetUpTMCaseUI_Blocking);
}

static void CB2_Idle(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_Idle(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_SetUpTMCaseUI_Blocking(void)
{
    while (1)
    {
        if (MenuHelpers_ShouldWaitForLinkRecv() == TRUE)
            break;
        if (DoSetUpTMCaseUI() == TRUE)
            break;
        if (MenuHelpers_IsLinkActive() == TRUE)
            break;
    }
}

static bool8 DoSetUpTMCaseUI(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 2:
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 3:
        ResetPaletteFade();
        gMain.state++;
        break;
    case 4:
        ResetSpriteData();
        gMain.state++;
        break;
    case 5:
        ResetTasks();
        gMain.state++;
        break;
    case 6:
        LoadBGTemplates();
        sTMCaseDynamicResources->seqId = 0;
        gMain.state++;
        break;
    case 7:
        InitWindowTemplatesAndPals();
        gMain.state++;
        break;
    case 8:
        if (HandleLoadTMCaseGraphicsAndPalettes())
            gMain.state++;
        break;
    case 9:
        UpdatePocketItemList(TMHM_POCKET);
        gMain.state++;
        break;
    case 10:
        TMCaseSetup_GetTMCount();
        TMCaseSetup_InitListMenuPositions();
        TMCaseSetup_UpdateVisualMenuOffset();
        gMain.state++;
        break;
    case 11:
        DrawMoveInfoUIMarkers();
        DrawPartyMonIcons();
        gMain.state++;
        break;
    case 12:
        CreateTMCaseListMenuBuffers();
        InitTMCaseListMenuItems();
        gMain.state++;
        break;
    case 13:
        PrintTMCaseTitle();
        gMain.state++;
        break;
    case 14:
        taskId = CreateTask(Task_TMCaseMain, 0);
        gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sTMCaseStaticResources.scrollOffset, sTMCaseStaticResources.selectedRow);
        gMain.state++;
        break;
    case 15:
        CreateTMCaseScrollIndicatorArrowPair_Main();
        gMain.state++;
        break;
    case 16:
        gMain.state++;
        break;
    case 17:
        BlendPalettes(PALETTES_ALL, 16, 0);
        gMain.state++;
        break;
    case 18:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(VBlankCB_Idle);
        SetMainCallback2(CB2_Idle);
        return TRUE;
    }

    return FALSE;
}

static void ResetBufferPointers_NoFree(void)
{
    sTMCaseDynamicResources = NULL;
    sTilemapBuffer = NULL;
    sListMenuItemsBuffer = NULL;
    sListMenuStringsBuffer = NULL;
    sTMSpritePaletteBuffer = NULL;
}

static void LoadBGTemplates(void)
{
    void ** ptr;
    ResetVramOamAndBgCntRegs();
    ResetAllBgsCoordinates();
    ptr = &sTilemapBuffer;
    *ptr = AllocZeroed(0x800);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBGTemplates, NELEMS(sBGTemplates));
    SetBgTilemapBuffer(2, *ptr);
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
}

static bool8 HandleLoadTMCaseGraphicsAndPalettes(void)
{
    switch (sTMCaseDynamicResources->seqId)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, gTMCase_Gfx, 0, 0, 0);
        sTMCaseDynamicResources->seqId++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(gTMCaseMenu_Tilemap, sTilemapBuffer);
            sTMCaseDynamicResources->seqId++;
        }
        break;
    case 2:
        sTMCaseDynamicResources->seqId++;
        break;
    case 3:
        if (gSaveBlock2Ptr->playerGender == MALE)
            LoadCompressedPalette(gTMCaseMenu_Male_Pal, BG_PLTT_ID(0), 4 * PLTT_SIZE_4BPP);
        else
            LoadCompressedPalette(gTMCaseMenu_Female_Pal, BG_PLTT_ID(0), 4 * PLTT_SIZE_4BPP);
        sTMCaseDynamicResources->seqId++;
        break;
    case 4:
        LoadCompressedSpriteSheet(&sTMSpriteSheet);
        sTMCaseDynamicResources->seqId++;
        break;
    default:
        sTMCaseDynamicResources->seqId = 0;
        return TRUE;
    }

    return FALSE;
}

static void CreateTMCaseListMenuBuffers(void)
{
    struct BagPocket * pocket = &gBagPockets[POCKET_TM_HM - 1];
    sListMenuItemsBuffer = Alloc((pocket->capacity) * sizeof(struct ListMenuItem));
    sListMenuStringsBuffer = Alloc(sTMCaseDynamicResources->numTMs * 29);
}

static void InitTMCaseListMenuItems(void)
{
    struct BagPocket * pocket = &gBagPockets[POCKET_TM_HM - 1];
    u16 i;

    for (i = 0; i < sTMCaseDynamicResources->numTMs; i++)
    {
        GetTMNumberAndMoveString(sListMenuStringsBuffer[i], pocket->itemSlots[i].itemId);
        sListMenuItemsBuffer[i].name = sListMenuStringsBuffer[i];
        sListMenuItemsBuffer[i].id = i;
    }

    gMultiuseListMenuTemplate.items = sListMenuItemsBuffer;
    gMultiuseListMenuTemplate.totalItems = sTMCaseDynamicResources->numTMs;
    gMultiuseListMenuTemplate.windowId = WIN_LIST;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.maxShowed = sTMCaseDynamicResources->maxTMsShown;
    gMultiuseListMenuTemplate.fontId = FONT_SHORT;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 0;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = TMCase_MoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = TMCase_ItemPrintFunc;
    gMultiuseListMenuTemplate.cursorKind = 0;
    gMultiuseListMenuTemplate.scrollMultiple = LIST_MULTIPLE_SCROLL_L_R;
}

static void GetTMNumberAndMoveString(u8 * dest, u16 itemId)
{
    StringCopy(gStringVar4, gText_FontSmall);
    if (itemId >= ITEM_HM01)
    {
        StringAppend(gStringVar4, sText_ClearTo18);
        StringAppend(gStringVar4, gText_NumberClear01);
        ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_HM01 + 1, STR_CONV_MODE_LEADING_ZEROS, 1);
        StringAppend(gStringVar4, gStringVar1);
    }
    else if (itemId >= ITEM_TM100)
    {
        StringAppend(gStringVar4, gText_NumberClear01);
        ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_TM01 + 1, STR_CONV_MODE_LEADING_ZEROS, 3);
        StringAppend(gStringVar4, gStringVar1);
    }
    else
    {
        StringAppend(gStringVar4, gText_NumberClear01);
        ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_TM01 + 1, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringAppend(gStringVar4, gStringVar1);
    }
    StringAppend(gStringVar4, sText_SingleSpace);
    StringAppend(gStringVar4, gText_FontShort);
    StringAppend(gStringVar4, gMovesInfo[ItemIdToBattleMoveId(itemId)].name);
    StringCopy(dest, gStringVar4);
}

static void TMCase_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    u16 itemId;

    if (itemIndex == LIST_CANCEL)
        itemId = ITEM_NONE;
    else
        itemId = BagGetItemIdByPocketPosition(POCKET_TM_HM, itemIndex);

    if (onInit != TRUE)
    {
        PlaySE(SE_RG_BAG_CURSOR);
    }
    TMCase_MoveCursor_UpdatePrintedDescription(itemIndex);
    TMCase_MoveCursor_UpdatePrintedTMInfo(itemId);
}

static void TMCase_ItemPrintFunc(u8 windowId, u32 itemId, u8 y)
{
    if (itemId != -2)
    {
        if (BagGetItemIdByPocketPosition(POCKET_TM_HM, itemId) >= ITEM_HM01)
        {
            PlaceHMTileInWindow(windowId, 8, y);
        }
    }
}

static void TMCase_MoveCursor_UpdatePrintedDescription(s32 itemIndex)
{
    const u8 * str;
    u16 itemId = BagGetItemIdByPocketPosition(POCKET_TM_HM, itemIndex);
    if (itemIndex != -2)
    {
        str = ItemId_GetDescription(itemId);
    }
    else
    {
        str = gText_TMCaseWillBePutAway;
    }
    FillWindowPixelBuffer(1, 0);
    AddTextPrinterParameterized_ColorByIndex(WIN_DESCRIPTION, FONT_SHORT, str, 2, 3, 1, 0, 0, COLOR_LIGHT);

    // update icons
    TintPartyMonIcons(itemId);
}

static void FillBG2RowWithPalette_2timesNplus1(s32 a0)
{
    SetBgTilemapPalette(2, 0, 12, 30, 8, 2 * a0 + 1);
    ScheduleBgCopyTilemapToVram(2);
}

static void PrintListMenuCursorByID_WithColorIdx(u8 listTaskId, u8 colorIdx)
{
    PrintListMenuCursorAt_WithColorIdx(ListMenuGetYCoordForPrintingArrowCursor(listTaskId), colorIdx);
}

static void PrintListMenuCursorAt_WithColorIdx(u8 y, u8 colorIdx)
{
    if (colorIdx == COLOR_CURSOR_ERASE)
    {
        // Never used. Would erase cursor (but also a portion of the list text)
        FillWindowPixelRect(WIN_LIST, 0, 0, y, GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_WIDTH), GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_HEIGHT));
        CopyWindowToVram(WIN_LIST, COPYWIN_GFX);
    }
    else
    {
        AddTextPrinterParameterized_ColorByIndex(WIN_LIST, FONT_SHORT, gText_SelectorArrow2, 0, y, 0, 0, 0, colorIdx);
    }
}

static void CreateTMCaseScrollIndicatorArrowPair_Main(void)
{
    sTMCaseDynamicResources->scrollIndicatorArrowPairId = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 
                                                                                                   0xA0, 0x08, 0x58, 
                                                                                                   sTMCaseDynamicResources->numTMs - sTMCaseDynamicResources->maxTMsShown, 
                                                                                                   TAG_SCROLL_ARROW, TAG_SCROLL_ARROW, 
                                                                                                   &sTMCaseStaticResources.scrollOffset);
}

static void UNUSED CreateTMCaseScrollIndicatorArrowPair_SellQuantitySelect(void)
{
    sTMCaseDynamicResources->currItem = 1;
    sTMCaseDynamicResources->scrollIndicatorArrowPairId = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 
                                                                                                   0x98, 0x48, 0x68, 
                                                                                                   2, 
                                                                                                   TAG_SCROLL_ARROW, TAG_SCROLL_ARROW, 
                                                                                                   &sTMCaseDynamicResources->currItem);
}

static void RemoveTMCaseScrollIndicatorArrowPair(void)
{
    if (sTMCaseDynamicResources->scrollIndicatorArrowPairId != TASK_NONE)
    {
        RemoveScrollIndicatorArrowPair(sTMCaseDynamicResources->scrollIndicatorArrowPairId);
        sTMCaseDynamicResources->scrollIndicatorArrowPairId = TASK_NONE;
    }
}

void ResetTMCaseCursorPos(void)
{
    sTMCaseStaticResources.selectedRow = 0;
    sTMCaseStaticResources.scrollOffset = 0;
}

static void TMCaseSetup_GetTMCount(void)
{
    struct BagPocket * pocket = &gBagPockets[POCKET_TM_HM - 1];
    u16 i;

    UpdatePocketItemList(TMHM_POCKET);
    sTMCaseDynamicResources->numTMs = 0;
    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == ITEM_NONE)
            break;
        sTMCaseDynamicResources->numTMs++;
    }
    sTMCaseDynamicResources->maxTMsShown = min(sTMCaseDynamicResources->numTMs, 5);
}

static void TMCaseSetup_InitListMenuPositions(void)
{
    if (sTMCaseStaticResources.scrollOffset != 0)
    {
        if (sTMCaseStaticResources.scrollOffset + sTMCaseDynamicResources->maxTMsShown > sTMCaseDynamicResources->numTMs)
            sTMCaseStaticResources.scrollOffset = sTMCaseDynamicResources->numTMs - sTMCaseDynamicResources->maxTMsShown;
    }
    if (sTMCaseStaticResources.scrollOffset + sTMCaseStaticResources.selectedRow >= sTMCaseDynamicResources->numTMs)
    {
        if (sTMCaseDynamicResources->numTMs < 2)
            sTMCaseStaticResources.selectedRow = 0;
        else
            sTMCaseStaticResources.selectedRow = sTMCaseDynamicResources->numTMs;
    }
}

static void TMCaseSetup_UpdateVisualMenuOffset(void)
{
    u8 i;
    if (sTMCaseStaticResources.selectedRow > 3)
    {
        for (i = 0; i <= sTMCaseStaticResources.selectedRow - 3 && sTMCaseStaticResources.scrollOffset + sTMCaseDynamicResources->maxTMsShown != sTMCaseDynamicResources->numTMs; i++)
        {
            do {} while (0);
            sTMCaseStaticResources.selectedRow--;
            sTMCaseStaticResources.scrollOffset++;
        }
    }
}

static void DestroyTMCaseBuffers(void)
{
    if (sTMCaseDynamicResources != NULL)
        Free(sTMCaseDynamicResources);
    if (sTilemapBuffer != NULL)
        Free(sTilemapBuffer);
    if (sListMenuItemsBuffer != NULL)
        Free(sListMenuItemsBuffer);
    if (sListMenuStringsBuffer != NULL)
        Free(sListMenuStringsBuffer);
    if (sTMSpritePaletteBuffer != NULL)
        Free(sTMSpritePaletteBuffer);
    FreeAllWindowBuffers();
}

static void Task_BeginFadeOutFromTMCase(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_FadeOutAndCloseTMCase;
}

static void Task_FadeOutAndCloseTMCase(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        DestroyListMenuTask(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
        if (sTMCaseDynamicResources->savedCallback != NULL)
            SetMainCallback2(sTMCaseDynamicResources->savedCallback);
        else
            SetMainCallback2(sTMCaseStaticResources.savedCallback);
        RemoveTMCaseScrollIndicatorArrowPair();
        DestroyTMCaseBuffers();
        DestroyTask(taskId);
    }
}

static void Task_TMCaseMain(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    s32 input;

    if (!gPaletteFade.active)
    {
        if (MenuHelpers_ShouldWaitForLinkRecv() != TRUE)
        {
            input = ListMenu_ProcessInput(data[0]);
            ListMenuGetScrollAndRow(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
            if (JOY_NEW(SELECT_BUTTON) && sTMCaseStaticResources.allowSelectClose == 1)
            {
                PlaySE(SE_SELECT);
                gSpecialVar_ItemId = ITEM_NONE;
                Task_BeginFadeOutFromTMCase(taskId);
            }
            else
            {
                switch (input)
                {
                case LIST_NOTHING_CHOSEN:
                    break;
                case LIST_CANCEL:
                    PlaySE(SE_SELECT);
                    gSpecialVar_ItemId = ITEM_NONE;
                    Task_BeginFadeOutFromTMCase(taskId);
                    break;
                default:
                    PlaySE(SE_SELECT);
                    FillBG2RowWithPalette_2timesNplus1(1);
                    RemoveTMCaseScrollIndicatorArrowPair();
                    PrintListMenuCursorByID_WithColorIdx(data[0], COLOR_CURSOR_SELECTED);
                    data[1] = input;
                    data[2] = BagGetQuantityByPocketPosition(POCKET_TM_HM, input);
                    gSpecialVar_ItemId = BagGetItemIdByPocketPosition(POCKET_TM_HM, input);
                    gTasks[taskId].func = sSelectTMActionTasks[sTMCaseStaticResources.tmCaseMenuType];
                    break;
                }
            }
        }
    }
}

static void Subtask_ReturnToTMCaseMain(u8 taskId)
{
    FillBG2RowWithPalette_2timesNplus1(0);
    CreateTMCaseScrollIndicatorArrowPair_Main();
    gTasks[taskId].func = Task_TMCaseMain;
}

static void Task_SelectTMAction_FromFieldBag(u8 taskId)
{
    u8 * strbuf;
    // create context menu
    if (!MenuHelpers_IsLinkActive() && InUnionRoom() != TRUE) // menu code
    {
        // Regular TM/HM context menu
        AddTMContextMenu(&sTMCaseDynamicResources->contextMenuWindowId, WIN_GIVE_EXIT);
        sTMCaseDynamicResources->menuActionIndices = sMenuActionIndices_Field;
        sTMCaseDynamicResources->numMenuActions = NELEMS(sMenuActionIndices_Field);
    }
    else
    {
        // Union Room TM/HM context menu
        AddTMContextMenu(&sTMCaseDynamicResources->contextMenuWindowId, WIN_GIVE_EXIT);
        sTMCaseDynamicResources->menuActionIndices = sMenuActionIndices_UnionRoom;
        sTMCaseDynamicResources->numMenuActions = NELEMS(sMenuActionIndices_UnionRoom);
    }
    // Print context window actions
    PrintMenuActionTexts(sTMCaseDynamicResources->contextMenuWindowId, 
                         FONT_SHORT, 
                         GetMenuCursorDimensionByFont(FONT_SHORT, 0), 
                         2, 
                         0, 
                         GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_HEIGHT) + 2, 
                         sTMCaseDynamicResources->numMenuActions, 
                         sMenuActions_UseGiveExit, 
                         sTMCaseDynamicResources->menuActionIndices);

    // context menu cursor
    InitMenuInUpperLeftCornerNormal(sTMCaseDynamicResources->contextMenuWindowId, sTMCaseDynamicResources->numMenuActions, 0);
    
    //"Move xyz is selected" text and window (no shoing the right palette)
    TMCase_SetWindowBorder3(2); // context window border style
    strbuf = Alloc(256);
    GetTMNumberAndMoveString(strbuf, gSpecialVar_ItemId);
    StringAppend(strbuf, gText_Var1IsSelected + 2); // +2 skips over the stringvar
    AddTextPrinterParameterized_ColorByIndex(WIN_SELECTED_MSG, FONT_SHORT, strbuf, 0, 2, 1, 0, 0, COLOR_DARK);
    Free(strbuf);

    //show HM icon
    if (gSpecialVar_ItemId >= ITEM_HM01)
    {
        PlaceHMTileInWindow(WIN_SELECTED_MSG, 0, 2);
        CopyWindowToVram(WIN_SELECTED_MSG, COPYWIN_GFX);
    }

    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    gTasks[taskId].func = Task_TMContextMenu_HandleInput;
}

static void Task_TMContextMenu_HandleInput(u8 taskId)
{
    s8 input;

    if (MenuHelpers_ShouldWaitForLinkRecv() != TRUE)
    {
        input = Menu_ProcessInputNoWrap();
        switch (input)
        {
        case MENU_B_PRESSED:
            PlaySE(SE_SELECT);
            sMenuActions_UseGiveExit[sTMCaseDynamicResources->menuActionIndices[sTMCaseDynamicResources->numMenuActions - 1]].func.void_u8(taskId);
            break;
        case MENU_NOTHING_CHOSEN:
            break;
        default:
            PlaySE(SE_SELECT);
            sMenuActions_UseGiveExit[sTMCaseDynamicResources->menuActionIndices[input]].func.void_u8(taskId);
            break;
        }
    }
}

static void TMHMContextMenuAction_Use(u8 taskId)
{
    RemoveTMContextMenu(&sTMCaseDynamicResources->contextMenuWindowId);
    ClearStdWindowAndFrameToTransparent(WIN_SELECTED_MSG, FALSE);
    ClearWindowTilemap(WIN_SELECTED_MSG);
    PutWindowTilemap(WIN_LIST);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    if (CalculatePlayerPartyCount() == 0)
    {
        PrintError_ThereIsNoPokemon(taskId);
    }
    else
    {
        gItemUseCB = ItemUseCB_TMHM;
        sTMCaseDynamicResources->savedCallback = CB2_ShowPartyMenuForItemUseTMCase;
        Task_BeginFadeOutFromTMCase(taskId);
    }
}

static void PrintError_ThereIsNoPokemon(u8 taskId)
{
    TMCase_PrintMessageWithFollowupTask(taskId, FONT_SHORT, gText_NoPokemon, Task_WaitButtonAfterErrorPrint);
}

static void PrintError_ItemCantBeHeld(u8 taskId)
{
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_Var1CantBeHeld);
    TMCase_PrintMessageWithFollowupTask(taskId, FONT_SHORT, gStringVar4, Task_WaitButtonAfterErrorPrint);
}

static void Task_WaitButtonAfterErrorPrint(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        Subtask_CloseContextMenuAndReturnToMain(taskId);
    }
}

static void Subtask_CloseContextMenuAndReturnToMain(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    DestroyListMenuTask(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sTMCaseStaticResources.scrollOffset, sTMCaseStaticResources.selectedRow);
    PrintListMenuCursorByID_WithColorIdx(data[0], COLOR_DARK);
    ClearDialogWindowAndFrameToTransparent(WIN_MESSAGE, FALSE);
    ClearWindowTilemap(WIN_MESSAGE);
    PutWindowTilemap(WIN_DESCRIPTION);
    PutWindowTilemap(WIN_MOVE_INFO_LABELS);
    PutWindowTilemap(WIN_MOVE_INFO);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    Subtask_ReturnToTMCaseMain(taskId);
}

static void TMHMContextMenuAction_Exit(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    RemoveTMContextMenu(&sTMCaseDynamicResources->contextMenuWindowId);
    ClearStdWindowAndFrameToTransparent(WIN_SELECTED_MSG, FALSE);
    ClearWindowTilemap(WIN_SELECTED_MSG);
    PutWindowTilemap(WIN_LIST);
    PrintListMenuCursorByID_WithColorIdx(data[0], COLOR_DARK);
    PutWindowTilemap(WIN_DESCRIPTION);
    PutWindowTilemap(WIN_MOVE_INFO_LABELS);
    PutWindowTilemap(WIN_MOVE_INFO);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    Subtask_ReturnToTMCaseMain(taskId);
}

static void Task_SelectTMAction_FromGiveParty(u8 taskId)
{
    PrintError_ItemCantBeHeld(taskId);
}

static void Task_SelectTMAction_FromGivePC(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (!ItemId_GetImportance(BagGetItemIdByPocketPosition(POCKET_TM_HM, data[1])))
    {
        sTMCaseDynamicResources->savedCallback = CB2_ReturnToPokeStorage;
        Task_BeginFadeOutFromTMCase(taskId);
    }
    else
    {
        PrintError_ItemCantBeHeld(taskId);
    }
}

static void InitWindowTemplatesAndPals(void)
{
    u8 i;

    InitWindows(sWindowTemplates);
    DeactivateAllTextPrinters();
    LoadMessageBoxGfx(0, 0x64, BG_PLTT_ID(11));
    LoadUserWindowBorderGfx(0, 0x78, BG_PLTT_ID(14));
    LoadPalette(gTMCaseMainWindowPalette, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
    LoadPalette(gTMCaseMainWindowPalette, BG_PLTT_ID(10), PLTT_SIZE_4BPP);
    LoadPalette(sPal3Override, BG_PLTT_ID(15) + 6, sizeof(sPal3Override));
    LoadPalette(sPal3Override, BG_PLTT_ID(13) + 6, sizeof(sPal3Override));
    ListMenuLoadStdPalAt(BG_PLTT_ID(12), 0x01);
    for (i = 0; i < ARRAY_COUNT(sWindowTemplates) - 1; i++)
        FillWindowPixelBuffer(i, 0x00);
    PutWindowTilemap(WIN_LIST);
    PutWindowTilemap(WIN_DESCRIPTION);
    PutWindowTilemap(WIN_TITLE);
    PutWindowTilemap(WIN_MOVE_INFO_LABELS);
    PutWindowTilemap(WIN_MOVE_INFO);
    ScheduleBgCopyTilemapToVram(0);
}

static void AddTextPrinterParameterized_ColorByIndex(u8 windowId, u8 fontId, const u8 * str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx)
{
    AddTextPrinterParameterized4(windowId, fontId, x, y, letterSpacing, lineSpacing, sTMCaseTextColors[colorIdx], speed, str);
}

static void UNUSED TMCase_SetWindowBorder1(u8 windowId)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, 0x5B, 0x0E);
}

static void TMCase_SetWindowBorder2(u8 windowId)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, 0x78, 0x0E);
}

static void TMCase_SetWindowBorder3(u8 windowId)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, 0x78, 0x0E);
}

static void TMCase_PrintMessageWithFollowupTask(u8 taskId, u8 fontId, const u8 * str, TaskFunc func)
{
    DisplayMessageAndContinueTask(taskId, WIN_MESSAGE, 0x64, 0x0B, fontId, GetPlayerTextSpeedDelay(), str, func);
    ScheduleBgCopyTilemapToVram(1);
}

static void PrintTMCaseTitle(void)
{
    u32 distance = 72 - GetStringWidth(FONT_NORMAL, gText_TMCase, 0);
    AddTextPrinterParameterized3(WIN_TITLE, FONT_NORMAL, distance / 2, 1, sTMCaseTextColors[COLOR_TITLE], 0, gText_TMCase);
}

static void DrawMoveInfoUIMarkers(void)
{
    #ifndef POKEMON_EXPANSION
        BlitMenuInfoIcon(WIN_MOVE_INFO_LABELS, 19, 0, 0); // "Type" sprite
        BlitMenuInfoIcon(WIN_MOVE_INFO_LABELS, 20, 0, 12); // "Power" sprite
        BlitMenuInfoIcon(WIN_MOVE_INFO_LABELS, 21, 0, 24); // "Accuracy" sprite
        BlitMenuInfoIcon(WIN_MOVE_INFO_LABELS, 22, 0, 36); // "PP" sprite
    #else
        BlitMenuInfoIcon(WIN_MOVE_INFO_LABELS, MENU_INFO_ICON_TYPE, 0, 0); // "Type" sprite
        BlitMenuInfoIcon(WIN_MOVE_INFO_LABELS, MENU_INFO_ICON_POWER, 0, 12); // "Power" sprite
        BlitMenuInfoIcon(WIN_MOVE_INFO_LABELS, MENU_INFO_ICON_ACCURACY, 0, 24); // "Accuracy" sprite
        BlitMenuInfoIcon(WIN_MOVE_INFO_LABELS, MENU_INFO_ICON_PP, 0, 36); // "PP" sprite
    #endif
    CopyWindowToVram(WIN_MOVE_INFO_LABELS, COPYWIN_GFX);
}

static void TMCase_MoveCursor_UpdatePrintedTMInfo(u16 itemId)
{
    u8 i;
    const u8 * str;

    FillWindowPixelRect(WIN_MOVE_INFO, 0, 0, 0, 40, 48);
    if (itemId == ITEM_NONE)
    {
        for (i = 0; i < 4; i++)
        {
            AddTextPrinterParameterized_ColorByIndex(WIN_MOVE_INFO, FONT_SHORT_COPY_1, gText_ThreeDashes, 7, 12 * i, 0, 0, TEXT_SKIP_DRAW, COLOR_MOVE_INFO);
        }
        CopyWindowToVram(WIN_MOVE_INFO, COPYWIN_GFX);
    }
    else
    {
        // Draw type icon
        u16 move = ItemIdToBattleMoveId(itemId);
        BlitMenuInfoIcon(WIN_MOVE_INFO, gMovesInfo[move].type + 1, 0, 0);

        // Print power
        if (gMovesInfo[move].power < 2)
            str = gText_ThreeDashes;
        else
        {
            ConvertIntToDecimalStringN(gStringVar1, gMovesInfo[move].power, STR_CONV_MODE_RIGHT_ALIGN, 3);
            str = gStringVar1;
        }
        AddTextPrinterParameterized_ColorByIndex(WIN_MOVE_INFO, FONT_SHORT_COPY_1, str, 7, 12, 0, 0, TEXT_SKIP_DRAW, COLOR_MOVE_INFO);

        // Print accuracy
        if (gMovesInfo[move].accuracy == 0)
            str = gText_ThreeDashes;
        else
        {
            ConvertIntToDecimalStringN(gStringVar1, gMovesInfo[move].accuracy, STR_CONV_MODE_RIGHT_ALIGN, 3);
            str = gStringVar1;
        }
        AddTextPrinterParameterized_ColorByIndex(WIN_MOVE_INFO, FONT_SHORT_COPY_1, str, 7, 24, 0, 0, TEXT_SKIP_DRAW, COLOR_MOVE_INFO);

        // Print PP
        ConvertIntToDecimalStringN(gStringVar1, gMovesInfo[move].pp, STR_CONV_MODE_RIGHT_ALIGN, 3);
        AddTextPrinterParameterized_ColorByIndex(WIN_MOVE_INFO, FONT_SHORT_COPY_1, gStringVar1, 7, 36, 0, 0, TEXT_SKIP_DRAW, COLOR_MOVE_INFO);
        CopyWindowToVram(WIN_MOVE_INFO, COPYWIN_GFX);
    }
}

static void PlaceHMTileInWindow(u8 windowId, u8 x, u8 y)
{
    BlitBitmapToWindow(windowId, gTMCaseHM_Gfx, x, y, 16, 12);
}

static void UNUSED HandlePrintMoneyOnHand(void)
{
    PrintMoneyAmountInMoneyBoxWithBorder(WIN_MONEY, 0x78, 0xD, GetMoney(&gSaveBlock1Ptr->money));
}

static void UNUSED HandleCreateYesNoMenu(u8 taskId, const struct YesNoFuncTable *ptrs)
{
    CreateYesNoMenuWithCallbacks(taskId, &sYesNoWindowTemplate, FONT_SHORT, 0, 2, 0x5B, 0x0E, ptrs);
}

static u8 AddTMContextMenu(u8 * windowId, u8 windowIndex)
{
    if (*windowId == WINDOW_NONE)
    {
        *windowId = AddWindow(&sWindowTemplates_ContextMenu[windowIndex]);
        TMCase_SetWindowBorder2(*windowId);
        ScheduleBgCopyTilemapToVram(0);
    }
    return *windowId;
}

static void RemoveTMContextMenu(u8 * windowId)
{
    ClearStdWindowAndFrameToTransparent(*windowId, FALSE);
    ClearWindowTilemap(*windowId);
    RemoveWindow(*windowId);
    ScheduleBgCopyTilemapToVram(0);
    *windowId = WINDOW_NONE;
}

#define sMonIconStill data[3]
static void SpriteCb_MonIcon(struct Sprite *sprite)
{
    if (!sprite->sMonIconStill)
        UpdateMonIconFrame(sprite);
}
#undef sMonIconStill

#define MON_ICON_START_X  0x10
#define MON_ICON_START_Y  0x2a
#define MON_ICON_PADDING  0x20


void LoadMonIconPalettesTinted(void)
{
    u8 i;
    for (i = 0; i < ARRAY_COUNT(gMonIconPaletteTable); i++)
    {
        LoadSpritePalette(&gMonIconPaletteTable[i]);
        TintPalette_GrayScale2(&gPlttBufferUnfaded[0x170 + i*16], 16);
    }
}
        

static void DrawPartyMonIcons(void)
{
    u8 i;
    u16 species;
    u8 icon_x = 0;
    u8 icon_y = 0;

    LoadMonIconPalettesTinted();

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        //calc icon position (centered)
        if (gPlayerPartyCount == 1)
        {
            icon_x = MON_ICON_START_X + MON_ICON_PADDING;
            icon_y = MON_ICON_START_Y + MON_ICON_PADDING*0.5;
        }
        else if (gPlayerPartyCount == 2)
        {
            icon_x = i < 2 ? MON_ICON_START_X + MON_ICON_PADDING*0.5 + MON_ICON_PADDING * i : MON_ICON_START_X + MON_ICON_PADDING*0.5 + MON_ICON_PADDING * (i - 2);
            icon_y = MON_ICON_START_Y + MON_ICON_PADDING*0.5;
        }else if (gPlayerPartyCount == 3)
        {
            icon_x = i < 3 ? MON_ICON_START_X + MON_ICON_PADDING * i : MON_ICON_START_X + MON_ICON_PADDING * (i - 3);
            icon_y = MON_ICON_START_Y + MON_ICON_PADDING*0.5;
        }
        else if (gPlayerPartyCount == 4)
        {
            icon_x = i < 2 ? MON_ICON_START_X + MON_ICON_PADDING*0.5 + MON_ICON_PADDING * i : MON_ICON_START_X + MON_ICON_PADDING*0.5 + MON_ICON_PADDING * (i - 2);
            icon_y = i < 2 ? MON_ICON_START_Y : MON_ICON_START_Y + MON_ICON_PADDING;
        }
        else
        {
            icon_x = i < 3 ? MON_ICON_START_X + MON_ICON_PADDING * i : MON_ICON_START_X + MON_ICON_PADDING * (i - 3);
            icon_y = i < 3 ? MON_ICON_START_Y : MON_ICON_START_Y + MON_ICON_PADDING;
        }
        //get species
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);

        //create icon sprite
        #ifndef POKEMON_EXPANSION
            spriteIdData[i] = CreateMonIcon(species, SpriteCb_MonIcon, icon_x, icon_y, 1, GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY), TRUE);
        #else
            spriteIdData[i] = CreateMonIcon(species, SpriteCb_MonIcon, icon_x, icon_y, 1, GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY));
        #endif

        //Set priority, stop movement and save original palette position
        gSprites[spriteIdData[i]].oam.priority = 0;
        StartSpriteAnim(&gSprites[spriteIdData[i]], 4); //full stop
        spriteIdPalette[i] = gSprites[spriteIdData[i]].oam.paletteNum; //save correct palette number to array
    }
}

static void TintPartyMonIcons(u16 tm)
{
    u8 i;
    u16 species;

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(7, 11));
        if (!CanLearnTeachableMove(species, ItemIdToBattleMoveId(tm)))
        {
            gSprites[spriteIdData[i]].oam.objMode = ST_OAM_OBJ_BLEND;        
        }
        else
        {
            gSprites[spriteIdData[i]].oam.objMode = ST_OAM_OBJ_NORMAL;
        }
    }
    
}

static void UNUSED DestroyPartyMonIcons(void)
{
    u8 i;
    for (i = 0; i < gPlayerPartyCount; i++)
    {
        FreeAndDestroyMonIconSprite(&gSprites[spriteIdData[i]]);
        FreeMonIconPalettes();
    }
}