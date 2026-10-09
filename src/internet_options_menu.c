#include "global.h"
#include "main.h"
#include "text.h"
#include "task.h"
#include "malloc.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "scanline_effect.h"
#include "text_window.h"
#include "bg.h"
#include "window.h"
#include "strings.h"
#include "menu.h"
#include "palette.h"
#include "constants/songs.h"
#include "sound.h"
#include "internet_options_menu.h"
#include "internet_protocol.h"
#include "title_screen.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "string_util.h"
#include "mystery_gift.h"
#include "mystery_gift_menu.h"
#include "save.h"
#include "link.h"
#include "field_screen_effect.h"
#include "mobile_adapter.h"
#include "item.h"
#include "naming_screen.h"
#include "pokemon.h"
#include "record_mixing.h"
#include "reload_save.h"

#define LIST_MENU_TILE_NUM 10
#define LIST_MENU_PAL_NUM 224

#define INTERNET_API_URL(path) "http://127.0.0.1/" path "?gameidentifier=" INTERNET_GAME_IDENTIFIER
#define INTERNET_SESSION_CHALLENGE_URL INTERNET_API_URL("Session/Challenge")
#define INTERNET_SESSION_URL INTERNET_API_URL("Session")
#define INTERNET_IDENTITY_URL INTERNET_API_URL("Identity")
#define INTERNET_MYSTERY_GIFT_URL INTERNET_API_URL("Gift")
#define INTERNET_MYSTERY_GIFT_ACK_URL INTERNET_API_URL("Gift/Acknowledge")
#define INTERNET_RECORD_MIX_URL INTERNET_API_URL("Record")
#define INTERNET_RECORD_MIX_DOWNLOAD_URL INTERNET_RECORD_MIX_URL "&code="
#define INTERNET_RECORD_MIX_URL_LENGTH 96
#define INTERNET_REQUEST_URL_LENGTH 128

// States for Task_InternetOptions
enum {
    INTERNET_STATE_TO_MAIN_MENU,
    INTERNET_STATE_MAIN_MENU,
    INTERNET_STATE_MA_CONNECTED,
    INTERNET_STATE_CONNECT_TO_SERVER,
    INTERNET_STATE_AUTHENTICATE_SESSION,
    INTERNET_STATE_REGISTER_IDENTITY,
    INTERNET_STATE_DOWNLOAD_GIFT,
    INTERNET_STATE_RECORD_MENU,
    INTERNET_STATE_DOWNLOAD_RECORD,
    INTERNET_STATE_UPLOAD_RECORD,
    INTERNET_STATE_PRINT_MESSAGE,
    INTERNET_STATE_CONFIG_ERROR,
    INTERNET_STATE_NOT_CONNECTED_ERROR,
    INTERNET_STATE_EXIT,
    INTERNET_STATE_ROLLBACK_EXIT,
};

struct InternetOptionsTaskData;
struct MAClientDetails;

static void CB2_InternetOptions(void);
static bool32 CreateInternetOptionsTask(void);
static void Task_InternetOptions(u8 taskId);
static bool32 HandleInternetOptionsSetup(void);
static u32 InternetOptions_HandleMenu(u8 whichMenu);
static bool32 PrintInternetOptionsMenuMessage(u8 *textState, const u8 *str);
#if (!TESTING || MOBILE_TESTING)
static bool32 HandleMobileAdapterError(u8 *state, const u8 *message);
#endif

static void PrintTopMenu(bool32 connecting);
static void LoadTextboxBorder(u8 bgId);
static void DrawCheckerboardBackground(u32 bg);
static void AddTextPrinterToWindow1(const u8 *str);
static void ClearTextWindow(void);
#if (!TESTING || MOBILE_TESTING)
static void CB2_ReturnFromInternetRecordCode(void);
static void BeginInternetRecordCodeEntry(u8 taskId, struct InternetOptionsTaskData *data);
static bool32 BuildInternetRecordMixUrl(char *url, const u8 *code);
static bool32 BuildAuthenticatedInternetUrl(char *url, u32 capacity, const char *baseUrl,
                                            const struct MAClientDetails *clientDetails);
static void ClearInternetSession(struct InternetOptionsTaskData *data);
static void AbortInternetConnection(struct InternetOptionsTaskData *data);
static void CloseInternetConnection(struct InternetOptionsTaskData *data);
static void FreeInternetOptionsScreen(void);
static void FreeInternetOptionsTask(u8 taskId, struct InternetOptionsTaskData *data);
static void FreeInternetOperationBuffers(struct InternetOptionsTaskData *data);
static bool32 AllocInternetResponseBuffer(struct InternetOptionsTaskData *data, u16 capacity);
static bool32 AllocInternetGift(struct InternetOptionsTaskData *data);
static void SetInternetMessageResult(struct InternetOptionsTaskData *data, const u8 *message);
static void SetInternetSaveResult(struct InternetOptionsTaskData *data, const u8 *successMessage);
static bool32 StartInternetDownload(struct InternetOptionsTaskData *data, const char *url, u16 capacity, const u8 *invalidMessage);
static bool32 StartInternetUpload(struct InternetOptionsTaskData *data, const char *url, const u8 *packet, u16 packetSize,
                                  u16 responseCapacity, const u8 *errorMessage);
static bool32 StartInternetRecordUpload(struct InternetOptionsTaskData *data);
#endif

EWRAM_DATA static u8 sDownArrowCounterAndYCoordIdx[8] = {};
EWRAM_DATA static u8 sInternetRecordCode[INTERNET_RECORD_CODE_LENGTH + 1] = {};
EWRAM_DATA static char sInternetRecordUrl[INTERNET_RECORD_MIX_URL_LENGTH] = {};
EWRAM_DATA static char sInternetRequestUrl[INTERNET_REQUEST_URL_LENGTH] = {};
EWRAM_DATA static bool8 sInternetRecordCodePending = FALSE;
EWRAM_DATA static bool8 sInternetRecordCodeInvalid = FALSE;

static const u16 sTextboxBorder_Pal[] = INCBIN_U16("graphics/interface/mystery_gift_textbox_border.gbapal");
static const u32 sTextboxBorder_Gfx[] = INCBIN_U32("graphics/interface/mystery_gift_textbox_border.4bpp.smol");

struct InternetOptionsTaskData
{
    struct MAClientDetails *clientDetails;
    u8 state;
    u8 subState;
    u8 nextState;
    u8 textState;
    bool8 maInitialized;
    bool8 pppConnected;
    s32 errorNum;
    u8 *clientMsg;
    const u8 *message;
    struct InternetMysteryGift *gift;
    u16 recvSize;
};

STATIC_ASSERT(sizeof(struct InternetOptionsTaskData) <= sizeof(gTasks[0].data), InternetOptionsTaskDataTooLarge);

struct MAClientDetails
{
    MA_TELDATA maTel;
    char pUserID[32];
    u8 sessionToken[INTERNET_SESSION_TOKEN_SIZE];
    bool8 sessionAuthenticated;
};

static const struct BgTemplate sBGTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 15,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, 
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }, 
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 13,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x000
    }, 
    {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 12,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0x000
    }
};

static const struct WindowTemplate sMainWindows[] = {
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 12,
        .baseBlock = 0x0013
    }, 
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 12,
        .baseBlock = 0x004f
    }, 
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 15,
        .width = 30,
        .height = 5,
        .paletteNum = 13,
        .baseBlock = 0x004f
    }, 
    {
        .bg = 0,
        .tilemapLeft = 18,
        .tilemapTop = 2,
        .width = 12,
        .height = 12,
        .paletteNum = 12,
        .baseBlock = 0x00e5
    },
    DUMMY_WIN_TEMPLATE
};

#if (!TESTING || MOBILE_TESTING)
static const char sMobileAdapterPassword[] = "password1";

static const struct WindowTemplate sMobileAdapterErrorWindow =
{
    .bg = 0,
    .tilemapLeft = 3,
    .tilemapTop = 2,
    .width = 24,
    .height = 16,
    .paletteNum = 15,
    .baseBlock = 0x0175,
};

static const u8 sText_MobileAdapterConfigError[] = _(
    "{COLOR RED}ERROR! {COLOR DARK_GRAY}Adapter config missing!\n"
    "\n"
    "To use online features, load the\n"
    "Mobile Adapter config file bundled\n"
    "with this patch into your emulator.\n"
    "\n"
    "Keep the adapter window open, then\n"
    "try connecting again.");

static const u8 sText_MobileAdapterNotConnectedError[] = _(
    "{COLOR RED}ERROR! {COLOR DARK_GRAY}Mobile Adapter\n"
    "not connected!\n"
    "\n"
    "To use internet features, open the\n"
    "game in the mGBA Mobile Adapter fork:\n"
    "github.com/fakuzatsu/mgba-ma\n"
    "\n"
    "Turn on the adapter, then try again.");
#endif

static const struct WindowTemplate sWindowTemplate_ThreeOptions = 
{
    .bg = 0,
    .tilemapLeft = 8,
    .tilemapTop = 6,
    .width = 14,
    .height = 6,
    .paletteNum = 12,
    .baseBlock = 0x0155
};

static const struct ListMenuItem sListMenuItems_InternetOptions[] =
{
    { gText_MysteryGift,        0 },
    { gText_RecordMix,          1 },
    { gText_Exit3,    LIST_CANCEL },
};

static const struct ListMenuItem sListMenuItems_RecordMix[] =
{
    { gText_Send,               0 },
    { gText_Receive,            1 },
    { gText_Cancel,   LIST_CANCEL },
};

static const struct ListMenuTemplate sListMenuTemplate_ThreeOptions = 
{
    .items = NULL,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = 3,
    .maxShowed = 3,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = FONT_NORMAL,
    .cursorKind = 0
};

ALIGNED(2) static const u8 sTextColors_TopMenu[]                   = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,     TEXT_COLOR_DARK_GRAY };
ALIGNED(2) static const u8 sInternetOptions_Ereader_TextColor_2[]  = { TEXT_COLOR_WHITE,       TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY };

void CB2_InitInternetOptions(void)
{
    if (HandleInternetOptionsSetup())
    {
        FadeInNewBGM(MUS_RG_MYSTERY_GIFT,4);
        SetMainCallback2(CB2_InternetOptions);
        if (!CreateInternetOptionsTask())
            SetMainCallback2(MainCB_FreeAllBuffersAndReturnToInitTitleScreen);
    }
    RunTasks();
}

static void CB2_InternetOptions(void)
{
    RunTasks();
    RunTextPrinters();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_MysteryGiftEReader(void)
{
    ProcessSpriteCopyRequests();
    LoadOam();
    TransferPlttBuffer();
}

static bool32 HandleInternetOptionsSetup(void)
{
    switch (gMain.state)
    {
    case 0:
    {
        void *tilemapBuffers[4];
        u32 i;

        SetVBlankCallback(NULL);
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetTasks();
        ScanlineEffect_Stop();
        ResetBgsAndClearDma3BusyFlags(0);

        InitBgsFromTemplates(0, sBGTemplates, ARRAY_COUNT(sBGTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        ChangeBgX(3, 0, BG_COORD_SET);
        ChangeBgY(3, 0, BG_COORD_SET);

        for (i = 0; i < ARRAY_COUNT(tilemapBuffers); i++)
        {
            tilemapBuffers[i] = Alloc(BG_SCREEN_SIZE);
            if (tilemapBuffers[i] == NULL)
            {
                while (i != 0)
                    Free(tilemapBuffers[--i]);
                gMain.state = 0;
                SetMainCallback2(CB2_InitTitleScreen);
                return FALSE;
            }
        }
        for (i = 0; i < ARRAY_COUNT(tilemapBuffers); i++)
            SetBgTilemapBuffer(i, tilemapBuffers[i]);

        LoadTextboxBorder(3);
        if (!InitWindows(sMainWindows))
        {
            FreeAllWindowBuffers();
            for (i = 0; i < ARRAY_COUNT(tilemapBuffers); i++)
            {
                Free(tilemapBuffers[i]);
                SetBgTilemapBuffer(i, NULL);
            }
            gMain.state = 0;
            SetMainCallback2(CB2_InitTitleScreen);
            return FALSE;
        }
        DeactivateAllTextPrinters();
        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        gMain.state++;
        break;
    }
    case 1:
        LoadPalette(sTextboxBorder_Pal, 0, 0x20);
        LoadPalette(GetTextWindowPalette(2), 0xd0, 0x20);
        Menu_LoadStdPalAt(0xC0);
        LoadUserWindowBorderGfx(0, 0xA, 0xE0);
        LoadUserWindowBorderGfx_(0, 0x1, 0xF0);
        FillBgTilemapBufferRect(0, 0x000, 0, 0, 32, 32, 0x11);
        FillBgTilemapBufferRect(1, 0x000, 0, 0, 32, 32, 0x11);
        FillBgTilemapBufferRect(2, 0x000, 0, 0, 32, 32, 0x11);
        DrawCheckerboardBackground(3);
        PrintTopMenu(FALSE);
        gMain.state++;
        break;
    case 2:
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);
        gMain.state++;
        break;
    case 3:
        ShowBg(0);
        ShowBg(3);
        FadeInFromBlack();
        SetVBlankCallback(VBlankCB_MysteryGiftEReader);
        EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_VCOUNT | INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
        return TRUE;
    }

    return FALSE;
}

static bool32 CreateInternetOptionsTask(void)
{
    u8 taskId;
    struct InternetOptionsTaskData *data;

    if (GetTaskCount() >= NUM_TASKS)
        return FALSE;

    taskId = CreateTask(Task_InternetOptions, 0);
    data = (void *)gTasks[taskId].data;

    data->clientDetails = AllocZeroed(sizeof(struct MAClientDetails));
    data->state         = INTERNET_STATE_MA_CONNECTED;
    data->subState      = 0;
    data->nextState     = 0;
    data->errorNum      = 0;
    data->maInitialized = FALSE;
    data->pppConnected  = FALSE;
    data->clientMsg     = NULL;
    data->message       = gText_UnableToInitialiseMALib;
    data->gift          = NULL;
    data->recvSize      = 0;

    if (data->clientDetails == NULL)
    {
        data->message = gText_InternetOutOfMemory;
        data->nextState = INTERNET_STATE_EXIT;
        data->state = INTERNET_STATE_PRINT_MESSAGE;
    }

    return TRUE;
}

#if (!TESTING || MOBILE_TESTING)
static void FreeInternetOperationBuffers(struct InternetOptionsTaskData *data)
{
    TRY_FREE_AND_SET_NULL(data->clientMsg);
    TRY_FREE_AND_SET_NULL(data->gift);
}

static bool32 AllocInternetResponseBuffer(struct InternetOptionsTaskData *data, u16 capacity)
{
    TRY_FREE_AND_SET_NULL(data->clientMsg);
    data->clientMsg = AllocZeroed(capacity);
    return data->clientMsg != NULL;
}

static bool32 AllocInternetGift(struct InternetOptionsTaskData *data)
{
    TRY_FREE_AND_SET_NULL(data->gift);
    data->gift = AllocZeroed(sizeof(*data->gift));
    return data->gift != NULL;
}

static void ClearInternetSession(struct InternetOptionsTaskData *data)
{
    if (data->clientDetails != NULL)
    {
        data->clientDetails->sessionAuthenticated = FALSE;
        memset(data->clientDetails->sessionToken, 0, sizeof(data->clientDetails->sessionToken));
    }
}

static inline void TerminateIfError(struct InternetOptionsTaskData *data, const u8 *errorMessage)
{
    if (data->errorNum != 0)
    {
        DebugPrintf("Mobile Adapter error: %d", data->errorNum);
        maKill();
        data->maInitialized = FALSE;
        data->pppConnected = FALSE;
        ClearInternetSession(data);
        FreeInternetOperationBuffers(data);
        data->message = errorMessage;
        data->nextState = INTERNET_STATE_EXIT;
        data->state = INTERNET_STATE_PRINT_MESSAGE;
    }
}

static void AbortInternetConnection(struct InternetOptionsTaskData *data)
{
    maKill();
    data->maInitialized = FALSE;
    data->pppConnected = FALSE;
    ClearInternetSession(data);
}

static void CloseInternetConnection(struct InternetOptionsTaskData *data)
{
    if (data->pppConnected)
    {
        if (maDisconnect() != MA_RESULT_OK)
        {
            maKill();
            data->pppConnected = FALSE;
            data->maInitialized = FALSE;
        }
        else
        {
            data->pppConnected = FALSE;
        }
    }

    if (data->maInitialized)
    {
        maEnd();
        data->maInitialized = FALSE;
    }

    ClearInternetSession(data);
}

static void FreeInternetOptionsScreen(void)
{
    u32 i;

    SetVBlankCallback(NULL);
    FreeAllWindowBuffers();
    for (i = 0; i < 4; i++)
        Free(GetBgTilemapBuffer(i));
}

static void FreeInternetOptionsTask(u8 taskId, struct InternetOptionsTaskData *data)
{
    CloseLink();
    Free(data->clientDetails);
    FreeInternetOperationBuffers(data);
    DestroyTask(taskId);
}

static void SetInternetMessageResult(struct InternetOptionsTaskData *data, const u8 *message)
{
    CloseInternetConnection(data);
    FreeInternetOperationBuffers(data);
    data->message = message;
    data->subState = 0;
    data->nextState = INTERNET_STATE_TO_MAIN_MENU;
    data->state = INTERNET_STATE_PRINT_MESSAGE;
}

static void SetInternetSaveResult(struct InternetOptionsTaskData *data, const u8 *successMessage)
{
    CloseInternetConnection(data);
    FreeInternetOperationBuffers(data);
    if (TrySavingDataNoErrorScreen(SAVE_NORMAL) == SAVE_STATUS_OK)
    {
        data->message = successMessage;
        data->nextState = INTERNET_STATE_TO_MAIN_MENU;
    }
    else
    {
        data->message = gText_InternetSaveFailedRollback;
        data->nextState = INTERNET_STATE_ROLLBACK_EXIT;
    }
    data->subState = 0;
    data->state = INTERNET_STATE_PRINT_MESSAGE;
}

static bool32 StartInternetDownload(struct InternetOptionsTaskData *data, const char *url, u16 capacity, const u8 *invalidMessage)
{
    if (!AllocInternetResponseBuffer(data, capacity))
    {
        SetInternetMessageResult(data, gText_InternetOutOfMemory);
        return FALSE;
    }

    AddTextPrinterToWindow1(gText_Communicating);
    data->recvSize = 0;
    data->errorNum = maDownload(url, NULL, 0, data->clientMsg, capacity, &data->recvSize, "", "");
    if (data->errorNum == MA_RESULT_BUFFER_FULL)
    {
        AbortInternetConnection(data);
        SetInternetMessageResult(data, invalidMessage);
        return FALSE;
    }
    if (data->errorNum != MA_RESULT_OK)
    {
        TerminateIfError(data, gText_DisconnectedWhileDownloading);
        return FALSE;
    }
    return TRUE;
}

static bool32 StartInternetUpload(struct InternetOptionsTaskData *data, const char *url, const u8 *packet, u16 packetSize,
                                  u16 responseCapacity, const u8 *errorMessage)
{
    if (!AllocInternetResponseBuffer(data, responseCapacity))
    {
        SetInternetMessageResult(data, gText_InternetOutOfMemory);
        return FALSE;
    }

    AddTextPrinterToWindow1(gText_Communicating);
    data->recvSize = 0;
    data->errorNum = maUpload(url, NULL, 0, packet, packetSize, data->clientMsg, responseCapacity,
                              &data->recvSize, "", "");
    if (data->errorNum != MA_RESULT_OK)
    {
        AbortInternetConnection(data);
        SetInternetMessageResult(data, errorMessage);
        return FALSE;
    }
    return TRUE;
}

static bool32 StartInternetRecordUpload(struct InternetOptionsTaskData *data)
{
    u8 *packet;
    u16 packetSize;

    packet = Alloc(INTERNET_RECORD_MIX_MAX_PACKET_SIZE);
    if (packet == NULL)
    {
        SetInternetMessageResult(data, gText_InternetOutOfMemory);
        return FALSE;
    }

    packetSize = BuildInternetRecordMixPacket(packet, INTERNET_RECORD_MIX_MAX_PACKET_SIZE);
    if (!BuildAuthenticatedInternetUrl(sInternetRequestUrl, sizeof(sInternetRequestUrl), INTERNET_RECORD_MIX_URL,
                                       data->clientDetails))
    {
        Free(packet);
        SetInternetMessageResult(data, gText_InternetRecordUploadInvalid);
        return FALSE;
    }
    if (!StartInternetUpload(data, sInternetRequestUrl, packet, packetSize, INTERNET_RECORD_CODE_LENGTH + 1,
                             gText_DisconnectedWhileUploading))
    {
        Free(packet);
        return FALSE;
    }
    Free(packet);
    return TRUE;
}

static bool32 IsMobileAdapterConfigError(u32 error)
{
    u8 apiError = error >> 16;

    return apiError == MAAPIE_REGISTRATION || apiError == MAAPIE_EEPROM_SUM;
}

static bool32 HandleMobileAdapterError(u8 *state, const u8 *message)
{
    static const u8 colors[] =
    {
        TEXT_COLOR_TRANSPARENT,
        TEXT_DYNAMIC_COLOR_6,
        TEXT_COLOR_LIGHT_GRAY,
    };

    switch (*state)
    {
    case 0:
    {
        u32 windowId = AddWindow(&sMobileAdapterErrorWindow);

        if (windowId == WINDOW_NONE)
            return TRUE;
        LoadPalette(gStandardMenuPalette, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        DrawStdFrameWithCustomTileAndPalette(windowId, TRUE, 0xA, 0xE);
        FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
        AddTextPrinterParameterized4(windowId, FONT_NORMAL, 8, 1, 0, 0, colors, 0, message);
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_FULL);
        (*state)++;
        break;
    }
    case 1:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
            return TRUE;
        break;
    }

    return FALSE;
}
#endif

#if (!TESTING || MOBILE_TESTING)
static void BeginInternetRecordCodeEntry(u8 taskId, struct InternetOptionsTaskData *data)
{
    CloseInternetConnection(data);
    FreeInternetOptionsTask(taskId, data);

    FreeInternetOptionsScreen();

    sInternetRecordCode[0] = EOS;
    sInternetRecordCodePending = FALSE;
    sInternetRecordCodeInvalid = FALSE;
    gMain.state = 0;
    DoNamingScreen(NAMING_SCREEN_RECORD_CODE, sInternetRecordCode, 0, 0, 0, CB2_ReturnFromInternetRecordCode);
}

static void CB2_ReturnFromInternetRecordCode(void)
{
    sInternetRecordCodePending = BuildInternetRecordMixUrl(sInternetRecordUrl, sInternetRecordCode);
    sInternetRecordCodeInvalid = !sInternetRecordCodePending && sInternetRecordCode[0] != EOS;
    gMain.state = 0;
    SetMainCallback2(CB2_InitInternetOptions);
}

static bool32 BuildInternetRecordMixUrl(char *url, const u8 *code)
{
    const char *prefix = INTERNET_RECORD_MIX_DOWNLOAD_URL;
    char *dest = url;
    u32 length = 0;

    while (*prefix != '\0')
        *dest++ = *prefix++;

    while (*code != EOS && length < INTERNET_RECORD_CODE_LENGTH)
    {
        if (*code >= CHAR_A && *code <= CHAR_Z)
            *dest++ = 'A' + (*code - CHAR_A);
        else if (*code >= CHAR_a && *code <= CHAR_z)
            *dest++ = 'A' + (*code - CHAR_a);
        else if (*code >= CHAR_0 && *code <= CHAR_9)
            *dest++ = '0' + (*code - CHAR_0);
        else
            return FALSE;

        code++;
        length++;
    }

    *dest = '\0';
    return length != 0 && *code == EOS;
}

static bool32 BuildAuthenticatedInternetUrl(char *url, u32 capacity, const char *baseUrl,
                                            const struct MAClientDetails *clientDetails)
{
    static const char sessionParameter[] = "&session=";
    char tokenText[INTERNET_SESSION_TOKEN_TEXT_SIZE];
    u32 baseLength = strlen(baseUrl);
    u32 parameterLength = sizeof(sessionParameter) - 1;

    if (!clientDetails->sessionAuthenticated
     || baseLength + parameterLength + sizeof(tokenText) > capacity)
        return FALSE;

    FormatInternetSessionToken(clientDetails->sessionToken, tokenText);
    memcpy(url, baseUrl, baseLength);
    memcpy(&url[baseLength], sessionParameter, parameterLength);
    memcpy(&url[baseLength + parameterLength], tokenText, sizeof(tokenText));
    return TRUE;
}
#endif

// Main Task Machine for Internet Options
static void Task_InternetOptions(u8 taskId)
{
#if (!TESTING || MOBILE_TESTING)
    struct InternetOptionsTaskData *data  = (void *)gTasks[taskId].data;
    struct MAClientDetails *clientDetails = data->clientDetails;

    switch (data->state)
    {
    case INTERNET_STATE_TO_MAIN_MENU:
        ClearTextWindow();
        PrintTopMenu(TRUE);
        data->state = INTERNET_STATE_MAIN_MENU;
        break;
    case INTERNET_STATE_MA_CONNECTED:
        if (!gPaletteFade.active)
        {
            if (!maConnected())
            {
                data->state = INTERNET_STATE_NOT_CONNECTED_ERROR;
            }
            else if (sInternetRecordCodeInvalid)
            {
                sInternetRecordCodeInvalid = FALSE;
                data->message = gText_InternetRecordInvalid;
                data->nextState = INTERNET_STATE_TO_MAIN_MENU;
                data->state = INTERNET_STATE_PRINT_MESSAGE;
            }
            else if (sInternetRecordCodePending)
            {
                sInternetRecordCodePending = FALSE;
                data->nextState = INTERNET_STATE_DOWNLOAD_RECORD;
                data->state = INTERNET_STATE_CONNECT_TO_SERVER;
                AddTextPrinterToWindow1(gText_Communicating);
            }
            else if (gSaveBlock3Ptr->PID == NO_PID)
            {
                data->nextState = INTERNET_STATE_REGISTER_IDENTITY;
                data->state = INTERNET_STATE_CONNECT_TO_SERVER;
                ClearTextWindow();
                AddTextPrinterToWindow1(gText_InternetRegistering);
            }
            else
            {
                PrintTopMenu(TRUE);
                data->state = INTERNET_STATE_MAIN_MENU;
            }
        }
        break;
    case INTERNET_STATE_CONNECT_TO_SERVER:
        switch (data->subState)
        {
        case 0:
            // Initialise MA Library
            DebugPrintf("Initialising MA Library");
            data->errorNum = maInitLibrary();
            if (data->errorNum == 0)
                data->maInitialized = TRUE;
            TerminateIfError(data, gText_UnableToInitialiseMALib);
            data->subState++;
            break;
        case 1:
            // Get the connection data stored in EEPROM.
            DebugPrintf("Getting EEPROM connection data");
            data->errorNum = maGetConnectionData(&clientDetails->maTel, clientDetails->pUserID);
            if (IsMobileAdapterConfigError(data->errorNum))
            {
                DebugPrintf("Mobile Adapter configuration error: %d", data->errorNum);
                maKill();
                data->maInitialized = FALSE;
                data->subState = 0;
                data->state = INTERNET_STATE_CONFIG_ERROR;
            }
            else
            {
                TerminateIfError(data, gText_UnableToInitialiseMALib);
                data->subState++;
            }
            break;
        case 2:
            // Makes a call and establishes a PPP connection
            DebugPrintf("Making a call and establishing PPP connection");
            data->errorNum = maConnectServer(&clientDetails->maTel, clientDetails->pUserID, sMobileAdapterPassword);
            if (data->errorNum == 0)
                data->pppConnected = TRUE;
            TerminateIfError(data, gText_UnableToConnectToServer);
            data->subState++;
            break;
        case 3:
            data->state = INTERNET_STATE_AUTHENTICATE_SESSION;
            data->subState = 0;
            break;
        }
        break;
    case INTERNET_STATE_AUTHENTICATE_SESSION:
        switch (data->subState)
        {
        case 0:
            if (StartInternetDownload(data, INTERNET_SESSION_CHALLENGE_URL,
                                      INTERNET_SESSION_CHALLENGE_PACKET_SIZE + 1,
                                      gText_InternetSessionFailed))
                data->subState++;
            break;
        case 1:
        {
            u8 nonce[INTERNET_SESSION_NONCE_SIZE];
            u8 packet[INTERNET_SESSION_AUTH_PACKET_SIZE];

            if (!DecodeInternetSessionChallenge(data->clientMsg, data->recvSize, nonce)
             || !BuildInternetSessionAuthPacket(nonce, packet)
             || !StartInternetUpload(data, INTERNET_SESSION_URL, packet, sizeof(packet),
                                     INTERNET_SESSION_RESPONSE_PACKET_SIZE + 1,
                                     gText_InternetSessionFailed))
            {
                if (data->state == INTERNET_STATE_AUTHENTICATE_SESSION)
                    SetInternetMessageResult(data, gText_InternetSessionFailed);
                break;
            }
            data->subState++;
            break;
        }
        case 2:
            if (!DecodeInternetSessionResponse(data->clientMsg, data->recvSize, clientDetails->sessionToken))
            {
                SetInternetMessageResult(data, gText_InternetSessionFailed);
                break;
            }
            clientDetails->sessionAuthenticated = TRUE;
            FreeInternetOperationBuffers(data);
            data->state = data->nextState;
            data->nextState = 0;
            data->subState = 0;
            break;
        }
        break;
    case INTERNET_STATE_REGISTER_IDENTITY:
        switch (data->subState)
        {
        case 0:
        {
            u8 packet[INTERNET_IDENTITY_REQUEST_PACKET_SIZE];

            BuildInternetIdentityRequest(packet);
            if (!BuildAuthenticatedInternetUrl(sInternetRequestUrl, sizeof(sInternetRequestUrl), INTERNET_IDENTITY_URL,
                                               clientDetails))
            {
                SetInternetMessageResult(data, gText_InternetRegistrationFailed);
                break;
            }
            if (StartInternetUpload(data, sInternetRequestUrl, packet, sizeof(packet),
                                    INTERNET_IDENTITY_RESPONSE_PACKET_SIZE + 1,
                                    gText_InternetRegistrationFailed))
                data->subState++;
            break;
        }
        case 1:
        {
            u32 pid;
            u8 token[INTERNET_IDENTITY_TOKEN_SIZE];
            bool8 replacingIdentity = gSaveBlock3Ptr->PID != NO_PID;
            const u8 *successMessage;

            if (!DecodeInternetIdentityResponse(data->clientMsg, data->recvSize, &pid, token))
            {
                SetInternetMessageResult(data, gText_InternetRegistrationFailed);
                break;
            }

            gSaveBlock3Ptr->PID = pid;
            memcpy(gSaveBlock3Ptr->internetToken, token, sizeof(gSaveBlock3Ptr->internetToken));
            successMessage = replacingIdentity
                           ? gText_InternetIdentityReplaced
                           : gText_InternetIdentityRegistered;
            CloseInternetConnection(data);
            FreeInternetOperationBuffers(data);
            if (TrySavingDataNoErrorScreen(SAVE_NORMAL) == SAVE_STATUS_OK)
            {
                data->message = successMessage;
                data->nextState = INTERNET_STATE_TO_MAIN_MENU;
            }
            else
            {
                data->message = gText_InternetSaveFailedRollback;
                data->nextState = INTERNET_STATE_ROLLBACK_EXIT;
            }
            data->subState = 0;
            data->state = INTERNET_STATE_PRINT_MESSAGE;
            break;
        }
        }
        break;
    case INTERNET_STATE_DOWNLOAD_RECORD:
        switch (data->subState)
        {
        case 0:
            if (!BuildAuthenticatedInternetUrl(sInternetRequestUrl, sizeof(sInternetRequestUrl), sInternetRecordUrl,
                                               clientDetails))
            {
                SetInternetMessageResult(data, gText_InternetRecordInvalid);
                break;
            }
            if (StartInternetDownload(data, sInternetRequestUrl, INTERNET_RECORD_MIX_MAX_PACKET_SIZE + 1,
                                      gText_InternetRecordInvalid))
                data->subState++;
            break;
        case 1:
            switch (ReceiveInternetRecordMix(data->clientMsg, data->recvSize, gStringVar1))
            {
            case INTERNET_RECORD_MIX_RECEIVED:
                SetInternetSaveResult(data, gText_InternetRecordReceived);
                break;
            case INTERNET_RECORD_MIX_OUT_OF_MEMORY:
                SetInternetMessageResult(data, gText_InternetRecordOutOfMemory);
                break;
            case INTERNET_RECORD_MIX_INVALID_PACKET:
            default:
                SetInternetMessageResult(data, gText_InternetRecordInvalid);
                break;
            }
            break;
        }
        break;
    case INTERNET_STATE_UPLOAD_RECORD:
        switch (data->subState)
        {
        case 0:
            if (StartInternetRecordUpload(data))
                data->subState++;
            break;
        case 1:
            if (DecodeInternetRecordMixCode(data->clientMsg, data->recvSize, gStringVar1))
                SetInternetMessageResult(data, gText_InternetRecordUploaded);
            else
                SetInternetMessageResult(data, gText_InternetRecordUploadInvalid);
            break;
        }
        break;
    case INTERNET_STATE_DOWNLOAD_GIFT:
        switch (data->subState)
        {
        case 0:
        {
            u8 packet[INTERNET_CREDENTIAL_PACKET_SIZE];

            BuildInternetCredentialPacket("PMGR", gSaveBlock3Ptr->PID, gSaveBlock3Ptr->internetToken, packet);
            if (!BuildAuthenticatedInternetUrl(sInternetRequestUrl, sizeof(sInternetRequestUrl), INTERNET_MYSTERY_GIFT_URL,
                                               clientDetails))
            {
                SetInternetMessageResult(data, gText_InternetGiftInvalid);
                break;
            }
            if (StartInternetUpload(data, sInternetRequestUrl, packet, sizeof(packet),
                                    INTERNET_MYSTERY_GIFT_MAX_PACKET_SIZE + 1,
                                    gText_InternetGiftInvalid))
                data->subState++;
            break;
        }
        case 1:
        {
            enum InternetMysteryGiftResult result;

            if (!AllocInternetGift(data))
            {
                SetInternetMessageResult(data, gText_InternetOutOfMemory);
                break;
            }
            result = ReceiveInternetMysteryGift(data->clientMsg, data->recvSize, data->gift);
            switch (result)
            {
            case INTERNET_MYSTERY_GIFT_ALREADY_RECEIVED:
                SetInternetMessageResult(data, gText_InternetGiftAlreadyReceived);
                return;
            case INTERNET_MYSTERY_GIFT_UNKNOWN_IDENTITY:
                FreeInternetOperationBuffers(data);
                data->message = gText_InternetIdentityLost;
                data->nextState = INTERNET_STATE_REGISTER_IDENTITY;
                data->subState = 0;
                data->state = INTERNET_STATE_PRINT_MESSAGE;
                return;
            case INTERNET_MYSTERY_GIFT_RECEIVED_ITEM:
                CopyItemNameHandlePlural(data->gift->data.item.itemId, gStringVar1, data->gift->data.item.quantity);
                ConvertIntToDecimalStringN(gStringVar2, data->gift->data.item.quantity, STR_CONV_MODE_LEFT_ALIGN, 3);
                data->message = gText_InternetGiftItemReceived;
                break;
            case INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PARTY:
                GetMonData(&data->gift->data.pokemon, MON_DATA_NICKNAME, gStringVar1);
                data->message = gText_InternetGiftPokemonReceived;
                break;
            case INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PC:
                GetMonData(&data->gift->data.pokemon, MON_DATA_NICKNAME, gStringVar1);
                data->message = gText_InternetGiftPokemonSentToPC;
                break;
            case INTERNET_MYSTERY_GIFT_NO_SPACE:
                SetInternetMessageResult(data, gText_InternetGiftNoSpace);
                return;
            case INTERNET_MYSTERY_GIFT_INVALID_PACKET:
            default:
                SetInternetMessageResult(data, gText_InternetGiftInvalid);
                return;
            }

            FreeInternetOperationBuffers(data);
            if (TrySavingDataNoErrorScreen(SAVE_NORMAL) != SAVE_STATUS_OK)
            {
                CloseInternetConnection(data);
                data->message = gText_InternetSaveFailedRollback;
                data->nextState = INTERNET_STATE_ROLLBACK_EXIT;
                data->subState = 0;
                data->state = INTERNET_STATE_PRINT_MESSAGE;
                return;
            }
            data->subState++;
            break;
        }
        case 2:
        {
            u8 packet[INTERNET_CREDENTIAL_PACKET_SIZE];

            BuildInternetCredentialPacket("PMGA", gSaveBlock3Ptr->PID, gSaveBlock3Ptr->internetToken, packet);
            if (!BuildAuthenticatedInternetUrl(sInternetRequestUrl, sizeof(sInternetRequestUrl),
                                               INTERNET_MYSTERY_GIFT_ACK_URL, clientDetails))
            {
                SetInternetMessageResult(data, gText_InternetGiftAckFailed);
                break;
            }
            if (!StartInternetUpload(data, sInternetRequestUrl, packet, sizeof(packet),
                                     INTERNET_ACK_PACKET_SIZE + 1, gText_InternetGiftAckFailed))
                break;
            data->subState++;
            break;
        }
        case 3:
            if (DecodeInternetAcknowledgement(data->clientMsg, data->recvSize))
                SetInternetMessageResult(data, data->message);
            else
                SetInternetMessageResult(data, gText_InternetGiftAckFailed);
            break;
        }
        break;
    case INTERNET_STATE_MAIN_MENU:
        switch (InternetOptions_HandleMenu(0))
        {
        case 0: // Mystery Gift
            data->nextState = gSaveBlock3Ptr->PID == NO_PID
                            ? INTERNET_STATE_REGISTER_IDENTITY
                            : INTERNET_STATE_DOWNLOAD_GIFT;
            data->state = INTERNET_STATE_CONNECT_TO_SERVER;
            data->subState = 0;
            AddTextPrinterToWindow1(gText_Communicating);
            PlaySE(SE_SELECT);
            break;
        case 1: // Record Mix
            data->state = INTERNET_STATE_RECORD_MENU;
            PlaySE(SE_SELECT);
            break;
        case LIST_CANCEL:
            data->state = INTERNET_STATE_EXIT;
            break;
        }
        break;
    case INTERNET_STATE_RECORD_MENU:
        switch (InternetOptions_HandleMenu(1))
        {
        case 0: // Send
            data->nextState = INTERNET_STATE_UPLOAD_RECORD;
            data->state = INTERNET_STATE_CONNECT_TO_SERVER;
            data->subState = 0;
            AddTextPrinterToWindow1(gText_Communicating);
            PlaySE(SE_SELECT);
            break;
        case 1: // Receive
            PlaySE(SE_SELECT);
            BeginInternetRecordCodeEntry(taskId, data);
            break;
        case LIST_CANCEL:
            data->state = INTERNET_STATE_TO_MAIN_MENU;
            break;
        }
        break;
    case INTERNET_STATE_PRINT_MESSAGE:
        if (PrintInternetOptionsMenuMessage(&data->textState, data->message))
            data->state = data->nextState;
        break;
    case INTERNET_STATE_CONFIG_ERROR:
        if (HandleMobileAdapterError(&data->subState, sText_MobileAdapterConfigError))
            data->state = INTERNET_STATE_EXIT;
        break;
    case INTERNET_STATE_NOT_CONNECTED_ERROR:
        if (HandleMobileAdapterError(&data->subState, sText_MobileAdapterNotConnectedError))
            data->state = INTERNET_STATE_EXIT;
        break;
    case INTERNET_STATE_EXIT:
        CloseInternetConnection(data);
        FreeInternetOptionsTask(taskId, data);
        SetMainCallback2(MainCB_FreeAllBuffersAndReturnToInitTitleScreen);
        break;
    case INTERNET_STATE_ROLLBACK_EXIT:
        CloseInternetConnection(data);
        FreeInternetOptionsTask(taskId, data);
        FreeInternetOptionsScreen();
        ReloadSaveToTitle();
        break;
    }
#else
    DestroyTask(taskId);
    SetMainCallback2(MainCB_FreeAllBuffersAndReturnToInitTitleScreen);
#endif
}

#define DOWN_ARROW_X 208
#define DOWN_ARROW_Y 20

static bool32 PrintInternetOptionsMenuMessage(u8 *textState, const u8 *str)
{
    switch (*textState)
    {
    case 0:
        AddTextPrinterToWindow1(str);
        (*textState)++;
        break;
    case 1:
        DrawDownArrow(1, DOWN_ARROW_X, DOWN_ARROW_Y, 1, FALSE, &sDownArrowCounterAndYCoordIdx[0], &sDownArrowCounterAndYCoordIdx[1]);
        if (({JOY_NEW(A_BUTTON);}))
            (*textState)++;
        if (({JOY_NEW(B_BUTTON);}))
        {
            *textState = 0;
            return TRUE;
        }
        break;
    case 2:
        DrawDownArrow(1, DOWN_ARROW_X, DOWN_ARROW_Y, 1, TRUE, &sDownArrowCounterAndYCoordIdx[0], &sDownArrowCounterAndYCoordIdx[1]);
        *textState = 0;
        return TRUE;
    case 0xFF:
        *textState = 2;
        return FALSE;
    }
    return FALSE;
}

static u32 InternetOptions_HandleMenu(u8 whichMenu)
{
    struct ListMenuTemplate listMenuTemplate = sListMenuTemplate_ThreeOptions;
    struct WindowTemplate windowTemplate = sWindowTemplate_ThreeOptions;
    s32 width;
    s32 response;

    if (whichMenu == 0)
        listMenuTemplate.items = sListMenuItems_InternetOptions;
    else
        listMenuTemplate.items = sListMenuItems_RecordMix;

    width = Intl_GetListMenuWidth(&listMenuTemplate);
    if (width & 1)
        width++;

    windowTemplate.width = width;
    if (width < 30)
        windowTemplate.tilemapLeft = (30 - width) / 2;
    else
        windowTemplate.tilemapLeft = 0;

    response = DoMysteryGiftListMenu(&windowTemplate, &listMenuTemplate, 1, LIST_MENU_TILE_NUM, LIST_MENU_PAL_NUM);
    if (response != LIST_NOTHING_CHOSEN)
    {
        ClearWindowTilemap(2);
        CopyWindowToVram(2, COPYWIN_MAP);
    }
    return response;
}

static void PrintTopMenu(bool32 connecting)
{
    const u8 *options = connecting ? gText_PickOKCancel : gText_Communicating;
    
    FillWindowPixelBuffer(0, 0);
    AddTextPrinterParameterized4(0, FONT_NORMAL, 4, 1, 0, 0, sTextColors_TopMenu, TEXT_SKIP_DRAW, gText_InternetOptions);
    AddTextPrinterParameterized4(0, FONT_SMALL, GetStringRightAlignXOffset(FONT_SMALL, options, 0xDE), 1, 0, 0, sTextColors_TopMenu, TEXT_SKIP_DRAW, options);
    CopyWindowToVram(0, COPYWIN_GFX);
    PutWindowTilemap(0);
}

static void LoadTextboxBorder(u8 bgId)
{
    DecompressAndLoadBgGfxUsingHeap(bgId, sTextboxBorder_Gfx, 0x100, 0, 0);
}

static void DrawCheckerboardBackground(u32 bg)
{
    s32 i = 0, j;

    FillBgTilemapBufferRect(bg, 0x003, 0, 0, 32, 2, 0x11);

    for (i = 0; i < 18; i++)
    {
        for (j = 0; j < 32; j++)
        {
            if ((i & 1) != (j & 1))
                FillBgTilemapBufferRect(bg, 1, j, i + 2, 1, 1, 0x11);
            else
                FillBgTilemapBufferRect(bg, 2, j, i + 2, 1, 1, 0x11);
        }
    }
}

static void AddTextPrinterToWindow1(const u8 *str)
{
    StringExpandPlaceholders(gStringVar4, str);
    FillWindowPixelBuffer(1, 0x11);
    AddTextPrinterParameterized4(1, FONT_NORMAL, 0, 1, 0, 0, sInternetOptions_Ereader_TextColor_2, 0, gStringVar4);
    DrawTextBorderOuter(1, 0x001, 0xF);
    PutWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_FULL);
}

static void ClearTextWindow(void)
{
    rbox_fill_rectangle(1);
    ClearWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_MAP);
}
