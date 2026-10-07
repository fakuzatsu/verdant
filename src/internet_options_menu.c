#include "global.h"
#include "main.h"
#include "text.h"
#include "task.h"
#include "data.h"
#include "malloc.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "scanline_effect.h"
#include "text_window.h"
#include "bg.h"
#include "window.h"
#include "strings.h"
#include "text_window.h"
#include "menu.h"
#include "palette.h"
#include "constants/songs.h"
#include "sound.h"
#include "internet_options_menu.h"
#include "union_room.h"
#include "title_screen.h"
#include "ereader_screen.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "string_util.h"
#include "mystery_gift.h"
#include "mystery_gift_view.h"
#include "mystery_gift_menu.h"
#include "save.h"
#include "link.h"
#include "mystery_gift_client.h"
#include "mystery_gift_server.h"
#include "event_data.h"
#include "link_rfu.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokedex.h"
#include "wonder_news.h"
#include "constants/cable_club.h"
#include "constants/party_menu.h"
#include "field_weather.h"
#include "constants/rgb.h"
#include "field_screen_effect.h"
#include "trade.h"
#include "trainer_pokemon_sprites.h"
#include "trig.h"
#include "mobile_adapter.h"
#include "item.h"
#include "naming_screen.h"
#include "record_mixing.h"
#include "reload_save.h"

#define LIST_MENU_TILE_NUM 10
#define LIST_MENU_PAL_NUM 224

#define LETTER_IN_RANGE_UPPER(letter, range) \
    ((letter) >= sLetterSearchRanges[range][0]                                  \
  && (letter) < sLetterSearchRanges[range][0] + sLetterSearchRanges[range][1])  \

#define LETTER_IN_RANGE_LOWER(letter, range) \
    ((letter) >= sLetterSearchRanges[range][2]                                  \
  && (letter) < sLetterSearchRanges[range][2] + sLetterSearchRanges[range][3])  \

#define PING_RECIEVED       0x0001
#define PING_NOT_RECIEVED   0x0000

#define INTERNET_MYSTERY_GIFT_URL "http://127.0.0.1/Gift?gameidentifier=1VERDANT"
#define INTERNET_RECORD_MIX_URL "http://127.0.0.1/Record?gameidentifier=1VERDANT"
#define INTERNET_RECORD_MIX_DOWNLOAD_URL INTERNET_RECORD_MIX_URL "&code="
#define INTERNET_RECORD_MIX_URL_LENGTH 96

// States for Task_InternetOptions
enum {
    INTERNET_STATE_TO_MAIN_MENU,
    INTERNET_STATE_MAIN_MENU,
    INTERNET_STATE_MA_CONNECTED,
    INTERNET_STATE_CONNECT_TO_SERVER,
    INTERNET_STATE_DOWNLOAD_GIFT,
    INTERNET_STATE_RECORD_MENU,
    INTERNET_STATE_DOWNLOAD_RECORD,
    INTERNET_STATE_UPLOAD_RECORD,
    INTERNET_STATE_PRINT_MESSAGE,
    INTERNET_STATE_GET_TOKEN,
    INTERNET_STATE_SHOW_FRIENDCODE,
    INTERNET_STATE_PING_SERVER,
    INTERNET_STATE_SET_PROFILE,
    INTERNET_STATE_ASK_PID,
    INTERNET_STATE_ASK_COUNTRY,
    INTERNET_STATE_ASK_REGION,
    INTERNET_STATE_GET_PID,
    INTERNET_STATE_INITIAL_SETUP,
    INTERNET_STATE_WAIT,
    INTERNET_STATE_SAVE_GAME,
    INTERNET_STATE_CONFIG_ERROR,
    INTERNET_STATE_EXIT,
    INTERNET_STATE_ROLLBACK_EXIT,
    INTERNET_STATE_SOURCE_PROMPT,
    INTERNET_STATE_SOURCE_PROMPT_INPUT,
};

struct InternetOptionsTaskData;

static void CB2_InternetOptions(void);
static bool32 CreateInternetOptionsTask(void);
static void Task_InternetOptions(u8 taskId);
static bool32 HandleInternetOptionsSetup(void);
static bool32 SaveOnInternetOptionMenu(u8 *state, s32 *saveResult);
static u32 InternetOptions_HandleMenu(u8 whichMenu);
static s8 DoInternetYesNo(u8 *textState, u16 *windowId, bool8 yesNoBoxPlacement, const u8 *str);
static bool32 PrintInternetOptionsMenuMessage(u8 *textState, const u8 *str);
#if (!TESTING || MOBILE_TESTING)
static void MainCB_FreeAllBuffersAndShowMobileAdapterConfigError(void);
static void CB2_MobileAdapterConfigErrorScreen(void);
static void VBlankCB_MobileAdapterConfigError(void);
static void PrintMobileAdapterConfigError(void);
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
static bool32 StartInternetRecordUpload(struct InternetOptionsTaskData *data);
#endif

static UNUSED char EncodeBase64(u32 checksum, char *data, size_t input_length);
static UNUSED int EncryptData(u32 pid, char *data, int datasize);
static UNUSED int DigestSHA1(uint8_t *digest, u8 *hexdigest, const uint8_t *data, size_t databytes);
static UNUSED void GenerateFriendcodeFromPID(u32 pid, u8 *hexdigest);

EWRAM_DATA static u8 sDownArrowCounterAndYCoordIdx[8] = {};
EWRAM_DATA struct InternetOptionsPokedexView *sInternetOptionsPokedexView = NULL;
EWRAM_DATA static u8 sInternetRecordCode[INTERNET_RECORD_CODE_LENGTH + 1] = {};
EWRAM_DATA static char sInternetRecordUrl[INTERNET_RECORD_MIX_URL_LENGTH] = {};
EWRAM_DATA static bool8 sInternetRecordCodePending = FALSE;
EWRAM_DATA static bool8 sInternetRecordCodeInvalid = FALSE;

static const u16 sTextboxBorder_Pal[] = INCBIN_U16("graphics/interface/mystery_gift_textbox_border.gbapal");
static const u32 sTextboxBorder_Gfx[] = INCBIN_U32("graphics/interface/mystery_gift_textbox_border.4bpp.smol");

// const rom data
//#include "data/pokemon/pokedex_orders.h"
extern const u16 gPokedexOrder_Alphabetical[];

struct InternetOptionsTaskData
{
    struct MAClientDetails *clientDetails;
    // struct InternetProfile *userProfile;
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
    char pPassword[16];
    char maMailID[30];
};

struct InternetProfile
{
    u8 version;
    u32 romhackID;
    u16 romhackVer;
    u8 language;
    u8 country;
    u8 region;
    u8 trainerID[4];
    u8 trainerName[16];
    u8 MAC[6];
    u8 email[56];
    u32 notify;
    u16 clientSecret;
    u16 mailSecret;
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
static const struct BgTemplate sMobileAdapterConfigErrorBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
};

static const struct WindowTemplate sMobileAdapterConfigErrorWindowTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 3,
        .tilemapTop = 2,
        .width = 24,
        .height = 16,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    DUMMY_WIN_TEMPLATE,
};

static const u8 sText_MobileAdapterConfigError[] = _(
    "{COLOR RED}ERROR! {COLOR DARK_GRAY}Adapter config missing!\n"
    "\n"
    "To use online features, load the\n"
    "Mobile Adapter config file bundled\n"
    "with this patch into your emulator.\n"
    "\n"
    "Keep the adapter window open, then\n"
    "try connecting again.\n"
    "Press A or B to return.");

static const u16 sMobileAdapterConfigErrorBackgroundColor = RGB(17, 18, 31);
#endif

static const struct WindowTemplate sWindowTemplate_YesNoMsg_Wide = 
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 15,
    .width = 28,
    .height = 4,
    .paletteNum = 12,
    .baseBlock = 0x00e5
};

static const struct WindowTemplate sWindowTemplate_YesNoMsg = 
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 15,
    .width = 20,
    .height = 4,
    .paletteNum = 12,
    .baseBlock = 0x00e5
};

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

static const struct WindowTemplate sWindowTemplate_YesNoBox = 
{
    .bg = 0,
    .tilemapLeft = 23,
    .tilemapTop = 15,
    .width = 6,
    .height = 4,
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
ALIGNED(2) static const u8 sTextColors_TopMenu_Copy[]              = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,     TEXT_COLOR_DARK_GRAY };
ALIGNED(2) static const u8 sInternetOptions_Ereader_TextColor_2[]  = { TEXT_COLOR_WHITE,       TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY };
ALIGNED(2) static const u8 sGTS_Ereader_TextColor_2[]              = { TEXT_COLOR_WHITE,       TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY };

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
    // data->userProfile   = AllocZeroed(sizeof(struct InternetProfile));
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

static inline void TerminateIfError(struct InternetOptionsTaskData *data, const u8 *errorMessage)
{
    if (data->errorNum != 0)
    {
        DebugPrintf("Mobile Adapter error: %d", data->errorNum);
        maKill();
        data->maInitialized = FALSE;
        data->pppConnected = FALSE;
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
            return;
        }
        data->pppConnected = FALSE;
    }

    if (data->maInitialized)
    {
        maEnd();
        data->maInitialized = FALSE;
    }
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

    ClearTextWindow();
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

static bool32 StartInternetRecordUpload(struct InternetOptionsTaskData *data)
{
    u8 *packet;
    u16 packetSize;

    if (!AllocInternetResponseBuffer(data, INTERNET_RECORD_CODE_LENGTH + 1))
    {
        SetInternetMessageResult(data, gText_InternetOutOfMemory);
        return FALSE;
    }

    packet = Alloc(INTERNET_RECORD_MIX_MAX_PACKET_SIZE);
    if (packet == NULL)
    {
        SetInternetMessageResult(data, gText_InternetOutOfMemory);
        return FALSE;
    }

    packetSize = BuildInternetRecordMixPacket(packet, INTERNET_RECORD_MIX_MAX_PACKET_SIZE);
    ClearTextWindow();
    AddTextPrinterToWindow1(gText_Communicating);
    data->recvSize = 0;
    data->errorNum = maUpload(INTERNET_RECORD_MIX_URL, NULL, 0, packet, packetSize,
                              data->clientMsg, INTERNET_RECORD_CODE_LENGTH + 1,
                              &data->recvSize, "", "");
    Free(packet);

    if (data->errorNum == MA_RESULT_BUFFER_FULL)
    {
        AbortInternetConnection(data);
        SetInternetMessageResult(data, gText_InternetRecordUploadInvalid);
        return FALSE;
    }
    if (data->errorNum != MA_RESULT_OK)
    {
        TerminateIfError(data, gText_DisconnectedWhileUploading);
        return FALSE;
    }
    return TRUE;
}

static bool32 IsMobileAdapterConfigError(u32 error)
{
    u8 apiError = error >> 16;

    return apiError == MAAPIE_REGISTRATION || apiError == MAAPIE_EEPROM_SUM;
}

static void MainCB_FreeAllBuffersAndShowMobileAdapterConfigError(void)
{
    FreeInternetOptionsScreen();
    gMain.state = 0;
    SetMainCallback2(CB2_MobileAdapterConfigErrorScreen);
}

static void VBlankCB_MobileAdapterConfigError(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void PrintMobileAdapterConfigError(void)
{
    static const u8 colors[] =
    {
        TEXT_COLOR_TRANSPARENT,
        TEXT_DYNAMIC_COLOR_6,
        TEXT_COLOR_LIGHT_GRAY,
    };

    AddTextPrinterParameterized4(0, FONT_NORMAL, 8, 1, 0, 0, colors, 0, sText_MobileAdapterConfigError);
}

static void CB2_MobileAdapterConfigErrorScreen(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BG0CNT, 0);
        SetGpuReg(REG_OFFSET_BG0HOFS, 0);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        DmaFill16(3, 0, VRAM, VRAM_SIZE);
        DmaFill32(3, 0, OAM, OAM_SIZE);
        DmaFill16(3, 0, PLTT, PLTT_SIZE);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sMobileAdapterConfigErrorBgTemplates, ARRAY_COUNT(sMobileAdapterConfigErrorBgTemplates));
        LoadBgTiles(0, gTextWindowFrame1_Gfx, 0x120, 0x214);
        DeactivateAllTextPrinters();
        ResetSpriteData();
        ResetTasks();
        ResetPaletteFade();
        LoadPalette(&sMobileAdapterConfigErrorBackgroundColor, BG_PLTT_ID(0), PLTT_SIZEOF(1));
        LoadPalette(gTextWindowFrame1_Pal, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
        LoadPalette(gStandardMenuPalette, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        if (!InitWindows(sMobileAdapterConfigErrorWindowTemplates))
        {
            FreeAllWindowBuffers();
            gMain.state = 0;
            SetMainCallback2(CB2_InitTitleScreen);
            return;
        }
        DrawStdFrameWithCustomTileAndPalette(0, TRUE, 0x214, 0xE);
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        PrintMobileAdapterConfigError();
        CopyWindowToVram(0, COPYWIN_FULL);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_MobileAdapterConfigError);
        ShowBg(0);
        gMain.state++;
        break;
    case 1:
        if (!UpdatePaletteFade() && JOY_NEW(A_BUTTON | B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            gMain.state++;
        }
        break;
    case 2:
        if (!UpdatePaletteFade())
        {
            SetVBlankCallback(NULL);
            FreeAllWindowBuffers();
            gMain.state = 0;
            SetMainCallback2(CB2_InitTitleScreen);
        }
        break;
    }
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
#endif

// Main Task Machine for Internet Options
static void Task_InternetOptions(u8 taskId)
{
#if (!TESTING || MOBILE_TESTING)
    struct InternetOptionsTaskData *data  = (void *)gTasks[taskId].data;
    struct MAClientDetails *clientDetails = data->clientDetails;
    // struct InternetProfile *userProfile   = data->userProfile;
    // char pURL[1024] = {'\0'};    // URL for the endpoint
    // u16 recvBufSize = 100;       // Size of received data
    // u8 pRecvData[recvBufSize];   // Buffer to hold received data
    // u16 pRecvSize;               // # of Bytes copied to pRecvData

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
                data->message = gText_MobileAdapterNotConnected;
                data->nextState = INTERNET_STATE_EXIT;
                data->state = INTERNET_STATE_PRINT_MESSAGE;
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
            //Get EEPROM Data
            DebugPrintf("Getting EEPROM Data");
            data->errorNum = maGetEEPROMData(&clientDetails->maTel, clientDetails->pUserID, clientDetails->maMailID);
            if (IsMobileAdapterConfigError(data->errorNum))
            {
                DebugPrintf("Mobile Adapter configuration error: %d", data->errorNum);
                maKill();
                data->maInitialized = FALSE;
                data->state = INTERNET_STATE_CONFIG_ERROR;
            }
            else
            {
                TerminateIfError(data, gText_UnableToInitialiseMALib);
                data->subState++;
            }
            break;
        case 2:
            // Set your password, must end in Null byte
            DebugPrintf("Setting password to default");
            memcpy(clientDetails->pPassword, "password1", 10);
            data->subState++;
            break;
        case 3:
            // Makes a call and establishes a PPP connection
            DebugPrintf("Making a call and establishing PPP connection");
            data->errorNum = maConnectServer(&clientDetails->maTel, clientDetails->pUserID, clientDetails->pPassword);
            if (data->errorNum == 0)
                data->pppConnected = TRUE;
            TerminateIfError(data, gText_UnableToConnectToServer);
            data->subState++;
            break;
        case 4:
            PrintTopMenu(TRUE);
            data->state = data->nextState;
            data->nextState = 0;
            data->subState = 0;
            break;
        }
        break;
    case INTERNET_STATE_DOWNLOAD_RECORD:
        switch (data->subState)
        {
        case 0:
            if (StartInternetDownload(data, sInternetRecordUrl, INTERNET_RECORD_MIX_MAX_PACKET_SIZE + 1,
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
            if (StartInternetDownload(data, INTERNET_MYSTERY_GIFT_URL, INTERNET_MYSTERY_GIFT_MAX_PACKET_SIZE + 1,
                                      gText_InternetGiftInvalid))
                data->subState++;
            break;
        case 1:
            if (!AllocInternetGift(data))
            {
                SetInternetMessageResult(data, gText_InternetOutOfMemory);
                break;
            }
            switch (ReceiveInternetMysteryGift(data->clientMsg, data->recvSize, data->gift))
            {
            case INTERNET_MYSTERY_GIFT_RECEIVED_ITEM:
                CopyItemNameHandlePlural(data->gift->data.item.itemId, gStringVar1, data->gift->data.item.quantity);
                ConvertIntToDecimalStringN(gStringVar2, data->gift->data.item.quantity, STR_CONV_MODE_LEFT_ALIGN, 3);
                SetInternetSaveResult(data, gText_InternetGiftItemReceived);
                break;
            case INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PARTY:
                GetMonData(&data->gift->data.pokemon, MON_DATA_NICKNAME, gStringVar1);
                SetInternetSaveResult(data, gText_InternetGiftPokemonReceived);
                break;
            case INTERNET_MYSTERY_GIFT_RECEIVED_POKEMON_PC:
                GetMonData(&data->gift->data.pokemon, MON_DATA_NICKNAME, gStringVar1);
                SetInternetSaveResult(data, gText_InternetGiftPokemonSentToPC);
                break;
            case INTERNET_MYSTERY_GIFT_NO_SPACE:
                SetInternetMessageResult(data, gText_InternetGiftNoSpace);
                break;
            case INTERNET_MYSTERY_GIFT_INVALID_PACKET:
            default:
                SetInternetMessageResult(data, gText_InternetGiftInvalid);
                break;
            }
            break;
        }
        break;
    case INTERNET_STATE_PING_SERVER:
        // Removed as this behaviour needs to be customised
        DebugPrintf("Reached ping state: exiting");
        data->state = gSaveBlock3Ptr->PID == NO_PID ? INTERNET_STATE_ASK_PID : INTERNET_STATE_SET_PROFILE;
        break;
    case INTERNET_STATE_SET_PROFILE:
        // Removed as this behaviour needs to be customised
        data->state = INTERNET_STATE_MAIN_MENU;
        break;
    case INTERNET_STATE_ASK_PID:
        switch (DoInternetYesNo(&data->textState, 0, FALSE, gText_CreateFriendCode))
        {
        case 0: // Yes
            data->message = gText_Communicating;
            data->state = INTERNET_STATE_PRINT_MESSAGE;
            data->nextState = INTERNET_STATE_GET_PID;
            break;
        case 1: // No
        case MENU_B_PRESSED:
            data->state = INTERNET_STATE_EXIT;
            break;
        }
        break;
    case INTERNET_STATE_GET_PID:
        // Removed as this behaviour needs to be customised
        data->state = INTERNET_STATE_SHOW_FRIENDCODE;
        data->nextState = INTERNET_STATE_SET_PROFILE;
        break;
    case INTERNET_STATE_SHOW_FRIENDCODE:
        // Removed as this behaviour needs to be customised
        data->state = INTERNET_STATE_SAVE_GAME;
        break;
    case INTERNET_STATE_ASK_COUNTRY:
        data->state = INTERNET_STATE_ASK_REGION;
        break;
    case INTERNET_STATE_ASK_REGION:
        data->state = INTERNET_STATE_INITIAL_SETUP;
        break;
    case INTERNET_STATE_INITIAL_SETUP:
        // Removed as this behaviour needs to be customised
        data->state = INTERNET_STATE_MAIN_MENU;
        break;
    case INTERNET_STATE_MAIN_MENU:
        switch (InternetOptions_HandleMenu(0))
        {
        case 0: // Mystery Gift
            data->nextState = INTERNET_STATE_DOWNLOAD_GIFT;
            data->state = INTERNET_STATE_CONNECT_TO_SERVER;
            data->subState = 0;
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
    case INTERNET_STATE_SAVE_GAME:
        if (SaveOnInternetOptionMenu(&data->textState, &data->errorNum))
            data->state = data->errorNum == SAVE_STATUS_OK ? data->nextState : INTERNET_STATE_ROLLBACK_EXIT;
        break;
    case INTERNET_STATE_CONFIG_ERROR:
        FreeInternetOptionsTask(taskId, data);
        SetMainCallback2(MainCB_FreeAllBuffersAndShowMobileAdapterConfigError);
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
            return TRUE;
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

static UNUSED s8 DoInternetOptionsYesNo(u8 *textState, u16 *windowId, bool8 yesNoBoxPlacement, const u8 *str)
{
    struct WindowTemplate windowTemplate;
    s8 input;

    switch (*textState)
    {
    case 0:
        // Print question message
        StringExpandPlaceholders(gStringVar4, str);
        if (yesNoBoxPlacement == 0)
            *windowId = AddWindow(&sWindowTemplate_YesNoMsg_Wide);
        else
            *windowId = AddWindow(&sWindowTemplate_YesNoMsg);
        FillWindowPixelBuffer(*windowId, 0x11);
        AddTextPrinterParameterized4(*windowId, FONT_NORMAL, 0, 1, 0, 0, sInternetOptions_Ereader_TextColor_2, 0, gStringVar4);
        DrawTextBorderOuter(*windowId, 0x001, 0x0F);
        CopyWindowToVram(*windowId, COPYWIN_GFX);
        PutWindowTilemap(*windowId);
        (*textState)++;
        break;
    case 1:
        // Create Yes/No
        windowTemplate = sWindowTemplate_YesNoBox;
        if (yesNoBoxPlacement == 0)
            windowTemplate.tilemapTop = 9;
        else
            windowTemplate.tilemapTop = 15;
        CreateYesNoMenu(&windowTemplate, 10, 14, 0);
        (*textState)++;
        break;
    case 2:
        // Handle Yes/No input
        input = Menu_ProcessInputNoWrapClearOnChoose();
        if (input == MENU_B_PRESSED || input == 0 || input == 1)
        {
            *textState = 0;
            rbox_fill_rectangle(*windowId);
            ClearWindowTilemap(*windowId);
            CopyWindowToVram(*windowId, COPYWIN_MAP);
            RemoveWindow(*windowId);
            return input;
        }
        break;
    case 0xFF:
        *textState = 0;
        rbox_fill_rectangle(*windowId);
        ClearWindowTilemap(*windowId);
        CopyWindowToVram(*windowId, COPYWIN_MAP);
        RemoveWindow(*windowId);
        return MENU_B_PRESSED;
    }

    return MENU_NOTHING_CHOSEN;
}

static bool32 SaveOnInternetOptionMenu(u8 *state, s32 *saveResult)
{
    switch (*state)
    {
    case 0:
        AddTextPrinterToWindow1(gText_DataWillBeSaved);
        (*state)++;
        break;
    case 1:
        *saveResult = TrySavingDataNoErrorScreen(SAVE_NORMAL);
        (*state)++;
        break;
    case 2:
        AddTextPrinterToWindow1(*saveResult == SAVE_STATUS_OK
                              ? gText_SaveCompletedPressA
                              : gText_InternetSaveFailedRollback);
        (*state)++;
        break;
    case 3:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
            (*state)++;
        break;
    case 4:
        *state = 0;
        ClearTextWindow();
        return TRUE;
    }

    return FALSE;
}

static s8 DoInternetYesNo(u8 *textState, u16 *windowId, bool8 yesNoBoxPlacement, const u8 *str)
{
    struct WindowTemplate windowTemplate;
    s8 input;

    switch (*textState)
    {
    case 0:
        // Print question message
        StringExpandPlaceholders(gStringVar4, str);
        if (yesNoBoxPlacement == 0)
            *windowId = AddWindow(&sWindowTemplate_YesNoMsg_Wide);
        else
            *windowId = AddWindow(&sWindowTemplate_YesNoMsg);
        FillWindowPixelBuffer(*windowId, 0x11);
        AddTextPrinterParameterized4(*windowId, FONT_NORMAL, 0, 1, 0, 0, sGTS_Ereader_TextColor_2, 0, gStringVar4);
        DrawTextBorderOuter(*windowId, 0x001, 0x0F);
        CopyWindowToVram(*windowId, COPYWIN_GFX);
        PutWindowTilemap(*windowId);
        (*textState)++;
        break;
    case 1:
        // Create Yes/No
        windowTemplate = sWindowTemplate_YesNoBox;
        if (yesNoBoxPlacement == 0)
            windowTemplate.tilemapTop = 9;
        else
            windowTemplate.tilemapTop = 15;
        CreateYesNoMenu(&windowTemplate, 10, 14, 0);
        (*textState)++;
        break;
    case 2:
        // Handle Yes/No input
        input = Menu_ProcessInputNoWrapClearOnChoose();
        if (input == MENU_B_PRESSED || input == 0 || input == 1)
        {
            *textState = 0;
            rbox_fill_rectangle(*windowId);
            ClearWindowTilemap(*windowId);
            CopyWindowToVram(*windowId, COPYWIN_MAP);
            RemoveWindow(*windowId);
            return input;
        }
        break;
    case 0xFF:
        *textState = 0;
        rbox_fill_rectangle(*windowId);
        ClearWindowTilemap(*windowId);
        CopyWindowToVram(*windowId, COPYWIN_MAP);
        RemoveWindow(*windowId);
        return MENU_B_PRESSED;
    }

    return MENU_NOTHING_CHOSEN;
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

/*******************************************************************************
                             Encoding/Encryption
 ******************************************************************************/

static UNUSED char EncodeBase64(u32 checksum, char *data, size_t input_length)
{
    int output_length = 4 * ((input_length + 2) / 3);
    int mod_table[] = {0, 2, 1};

    char encoding_table[] = 
    {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '-', '_',
    };

    char input_data[output_length];

    input_data[0] = checksum >> 24 & 0xFF;
    input_data[1] = checksum >> 16 & 0xFF;
    input_data[2] = checksum >> 8 & 0xFF;
    input_data[3] = checksum & 0xFF;

    for (int i = 4; i < input_length; i++)
    {
        input_data[i] = data[i-4];
    }
 
    char encoded_data[output_length];

    for (int i = 0, j = 0; i < input_length;)
    {
        uint32_t octet_a = i < input_length ? input_data[i++] : 0;
        uint32_t octet_b = i < input_length ? input_data[i++] : 0;
        uint32_t octet_c = i < input_length ? input_data[i++] : 0;
 
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
 
        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }
 
    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[output_length - 1 - i] = '=';
 
    return *encoded_data;
}

static UNUSED int EncryptData(u32 pid, char *data, int datasize)
{
    int checksum = 0;
    int i = 0;
    checksum = checksum + (pid >> 24 & 0xFF);
    checksum = checksum + (pid >> 16 & 0xFF);
    checksum = checksum + (pid >> 8 & 0xFF);
    checksum = checksum + (pid & 0xFF);

    for(i =0; i < datasize; i++)
    {
        checksum = checksum + data[i];
    }

    checksum = 0x4a3b2c1d ^ checksum;
    int GRNG = checksum | (checksum << 16);
    u8 keystream = (GRNG >> 16) & 0xFF;
    i = 0;

    while(i < datasize)
    {
        data[i]=data[i] ^ keystream;
        GRNG = (GRNG * 0x45 + 0x1111) & 0x7FFFFFFF;
        keystream = (GRNG >> 16) & 0xFF;
    }

    return checksum;
}

static const u8 crc_table[] = 
{
 0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, 0x38, 0x3f, 0x36, 0x31,
 0x24, 0x23, 0x2a, 0x2d, 0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,
 0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d, 0xe0, 0xe7, 0xee, 0xe9,
 0xfc, 0xfb, 0xf2, 0xf5, 0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
 0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85, 0xa8, 0xaf, 0xa6, 0xa1,
 0xb4, 0xb3, 0xba, 0xbd, 0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,
 0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea, 0xb7, 0xb0, 0xb9, 0xbe,
 0xab, 0xac, 0xa5, 0xa2, 0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
 0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32, 0x1f, 0x18, 0x11, 0x16,
 0x03, 0x04, 0x0d, 0x0a, 0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,
 0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a, 0x89, 0x8e, 0x87, 0x80,
 0x95, 0x92, 0x9b, 0x9c, 0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
 0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec, 0xc1, 0xc6, 0xcf, 0xc8,
 0xdd, 0xda, 0xd3, 0xd4, 0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,
 0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44, 0x19, 0x1e, 0x17, 0x10,
 0x05, 0x02, 0x0b, 0x0c, 0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
 0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b, 0x76, 0x71, 0x78, 0x7f,
 0x6a, 0x6d, 0x64, 0x63, 0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,
 0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13, 0xae, 0xa9, 0xa0, 0xa7,
 0xb2, 0xb5, 0xbc, 0xbb, 0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
 0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb, 0xe6, 0xe1, 0xe8, 0xef,
 0xfa, 0xfd, 0xf4, 0xf3
};

static inline u8 gencrc8(u8 *data)
{
    u8 crc = 0xff;
    u32 i;
    for (i = 0; i < 8; i++)
    {
        crc ^= data[i];
        crc = crc_table[crc ^ data[i]];
    }
    return crc;
}

/*******************************************************************************
 * DigestSHA1: https://github.com/CTrabant/teeny-sha1
 *
 * Calculate the SHA-1 value for supplied data buffer and generate a
 * text representation in hexadecimal.
 *
 * Based on https://github.com/jinqiangshou/EncryptionLibrary, credit
 * goes to @jinqiangshou, all new bugs are mine.
 *
 * @input:
 *    data      -- data to be hashed
 *    databytes -- bytes in data buffer to be hashed
 *
 * @output:
 *    digest    -- the result, MUST be at least 20 bytes
 *    hexdigest -- the result in hex, MUST be at least 41 bytes
 *
 * At least one of the output buffers must be supplied.  The other, if not 
 * desired, may be set to NULL.
 *
 * @return: 0 on success and non-zero on error.
 ******************************************************************************/

static UNUSED int DigestSHA1(uint8_t *digest, u8 *hexdigest, const uint8_t *data, size_t databytes)
{
    #define SHA1ROTATELEFT(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

    uint32_t W[80];

    uint32_t H[] = 
    {
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    };

    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint32_t e;
    uint32_t f = 0;
    uint32_t k = 0;

    uint32_t idx;
    uint32_t lidx;
    uint32_t widx;
    uint32_t didx = 0;

    int32_t wcount;
    uint32_t temp;
    uint64_t databits = ((uint64_t)databytes) * 8;
    uint32_t loopcount = (databytes + 8) / 64 + 1;
    uint32_t tailbytes = 64 * loopcount - databytes;
    uint8_t datatail[128] = {0};

    if (!digest && !hexdigest)
        return -1;

    if (!data)
        return -1;

    /* Pre-processing of data tail (includes padding to fill out 512-bit chunk):
        Add bit '1' to end of message (big-endian)
        Add 64-bit message length in bits at very end (big-endian) */
    datatail[0] = 0x80;
    datatail[tailbytes - 8] = (uint8_t) (databits >> 56 & 0xFF);
    datatail[tailbytes - 7] = (uint8_t) (databits >> 48 & 0xFF);
    datatail[tailbytes - 6] = (uint8_t) (databits >> 40 & 0xFF);
    datatail[tailbytes - 5] = (uint8_t) (databits >> 32 & 0xFF);
    datatail[tailbytes - 4] = (uint8_t) (databits >> 24 & 0xFF);
    datatail[tailbytes - 3] = (uint8_t) (databits >> 16 & 0xFF);
    datatail[tailbytes - 2] = (uint8_t) (databits >> 8 & 0xFF);
    datatail[tailbytes - 1] = (uint8_t) (databits >> 0 & 0xFF);

    /* Process each 512-bit chunk */
    for (lidx = 0; lidx < loopcount; lidx++)
    {
        /* Compute all elements in W */
        memset (W, 0, 80 * sizeof (uint32_t));

        /* Break 512-bit chunk into sixteen 32-bit, big endian words */
        for (widx = 0; widx <= 15; widx++)
        {
            wcount = 24;

            /* Copy byte-per byte from specified buffer */
            while (didx < databytes && wcount >= 0)
            {
                W[widx] += (((uint32_t)data[didx]) << wcount);
                didx++;
                wcount -= 8;
            }
            /* Fill out W with padding as needed */
            while (wcount >= 0)
            {
                W[widx] += (((uint32_t)datatail[didx - databytes]) << wcount);
                didx++;
                wcount -= 8;
            }
        }

        /* Extend the sixteen 32-bit words into eighty 32-bit words, with potential optimization from:
            "Improving the Performance of the Secure Hash Algorithm (SHA-1)" by Max Locktyukhin */
        for (widx = 16; widx <= 31; widx++)
        {
            W[widx] = SHA1ROTATELEFT ((W[widx - 3] ^ W[widx - 8] ^ W[widx - 14] ^ W[widx - 16]), 1);
        }
        for (widx = 32; widx <= 79; widx++)
        {
            W[widx] = SHA1ROTATELEFT ((W[widx - 6] ^ W[widx - 16] ^ W[widx - 28] ^ W[widx - 32]), 2);
        }

        /* Main loop */
        a = H[0];
        b = H[1];
        c = H[2];
        d = H[3];
        e = H[4];

        for (idx = 0; idx <= 79; idx++)
        {
            if (idx <= 19)
            {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if (idx >= 20 && idx <= 39)
            {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (idx >= 40 && idx <= 59)
            {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else if (idx >= 60 && idx <= 79)
            {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            temp = SHA1ROTATELEFT (a, 5) + f + e + k + W[idx];
            e = d;
            d = c;
            c = SHA1ROTATELEFT (b, 30);
            b = a;
            a = temp;
        }

        H[0] += a;
        H[1] += b;
        H[2] += c;
        H[3] += d;
        H[4] += e;
    }

    /* Store binary digest in supplied buffer */
    if (digest)
    {
        for (idx = 0; idx < 5; idx++)
        {
            digest[idx * 4 + 0] = (uint8_t) (H[idx] >> 24);
            digest[idx * 4 + 1] = (uint8_t) (H[idx] >> 16);
            digest[idx * 4 + 2] = (uint8_t) (H[idx] >> 8);
            digest[idx * 4 + 3] = (uint8_t) (H[idx]);
        }
    }

    /* Store hex version of digest in supplied buffer */
    ConvertIntToHexStringN_v2(hexdigest, H[0], STR_CONV_MODE_RIGHT_ALIGN, 8);
    ConvertIntToHexStringN_v2(hexdigest+8, H[1], STR_CONV_MODE_RIGHT_ALIGN, 8);
    ConvertIntToHexStringN_v2(hexdigest+16, H[2], STR_CONV_MODE_RIGHT_ALIGN, 8);
    ConvertIntToHexStringN_v2(hexdigest+24, H[3], STR_CONV_MODE_RIGHT_ALIGN, 8);
    ConvertIntToHexStringN_v2(hexdigest+32, H[4], STR_CONV_MODE_RIGHT_ALIGN, 8);

    return 0;
}

static UNUSED void GenerateFriendcodeFromPID(u32 pid, u8 *hexdigest)
{
    if (pid == 0)
    {
        // behaviour observed in MKWii, more likely an error condition
        hexdigest[0] = 'z';
    }
    else 
    {
        u8 i = 0;
        u8 buffer[8];
        // buffer is pid in little endian, followed by RMCJ in little endian
        buffer[0] = pid >> 0;
        buffer[1] = pid >> 8;
        buffer[2] = pid >> 16;
        buffer[3] = pid >> 24;

        buffer[4] = 'E'; // the reversed online relevant game id
        buffer[5] = 'E';
        buffer[6] = 'P';
        buffer[7] = 'B';

        // md5digest computes the digest of the second parameter and stores it in the first.
        u64 checksum = gencrc8(buffer);
        checksum = ((checksum << 32) | pid);

        ConvertUIntToDecimalStringN(hexdigest, checksum, STR_CONV_MODE_LEADING_ZEROS, 16);
        for(i = 0; i < 12; i++)
        {
            hexdigest[i]=hexdigest[i + 4];
        }

        for(i = 13; i > 9; i--)
        {
            hexdigest[i]=hexdigest[i - 2];
        }
        for(i = 8; i > 4; i--)
        {
            hexdigest[i]=hexdigest[i - 1];
        }

        hexdigest[4]='-';
        hexdigest[9]='-';
        hexdigest[14]='\0';
    }
}
