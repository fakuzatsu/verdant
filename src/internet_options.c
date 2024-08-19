#include "global.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "main.h"
#include "mobile_adapter.h"
#include "pokemon.h"
#include "pokedex.h"
#include "script.h"
#include "constants/internet_options.h"

u8 maTestConnection(void);
u8 maGetGiftMon(void);

void serializePokemon(const struct BoxPokemon *mon, u8 *buffer) 
{
    memcpy(buffer, mon, sizeof(struct BoxPokemon));
}

void deserializePokemon(struct BoxPokemon *mon, const u8 *buffer) 
{
    memcpy(mon, buffer, sizeof(struct BoxPokemon));
}

void concat(char *dest, const char *src)
{
    while (*dest != '\0') dest++;
    for (;;) {
        *dest++ = *src;
        if (*src == '\0') break;
        src++;
    }
}

void maInternetOptions(struct ScriptContext *ctx)
{
    u8 internetOption = ScriptReadByte(ctx);

    switch (internetOption)
    {
        case INTERNET_TEST_CONNECTION:
            gSpecialVar_Result = maTestConnection();
            return;
        case INTERNET_GET_GIFT_MON:
            gSpecialVar_Result = maGetGiftMon();
            return;
        default:
            gSpecialVar_Result = INTERNET_STATE_INPUT_ERR;
            return;
    }
}

u8 maTestConnection(void)
{
    MA_TELDATA maTel;           // MA Telephone struct
    char pUserID[32];           // User ID from the MA EEPROM, has max lenght of 32
    char pPassword[16];         // User password from the MA EEPROM, has max lenght of 16
    char maMailID[30];          // User mail ID from the MA EEPROM
    char pURL[1024];            // URL for the API req
    u8 pSendData[4];            // Data to send
    u16 recvBufSize = 100;      // Size of received data
    u8 pRecvData[recvBufSize];  // Buffer to hold received data
    u16 pRecvSize = 0;          // How many bytes were copied to pRecvData after calling maUpload once
    u32 tracker = 0;            // Tracks how much data has been sent
    u32 errNum = 0;

    // Initialize defaults
    pUserID[0]='\0';
    pPassword[0]='\0';
    maMailID[0]='\0';
    memcpy(pURL, "\0", 1);

    // Initialize destination and send data
    concat(pURL,"http://www.PutYourDomainHere.com/Debug\0");
    memcpy(pSendData, "GIFT", 4);

    // Initialize the Mobile Adapter library
    if ((errNum = maInitLibrary()) != 0) 
    {
        maKill();
        return INTERNET_STATE_MA_ERR;
    }

    // Get EEPROM Data
    if ((errNum = maGetEEPROMData(&maTel, pUserID, maMailID)) != 0) 
    {
        maKill();
        return INTERNET_STATE_MA_ERR;
    }

    // Establish a PPP connection to the server
    if ((errNum = maConnectServer(&maTel, pUserID, pPassword)) != 0) 
    {
        maKill();
        return INTERNET_STATE_MA_ERR;
    }

    // Send a POST request and receive the response data
    if ((errNum = maUpload(pURL, NULL, 0, pSendData, sizeof(pSendData), pRecvData, recvBufSize, &pRecvSize, pUserID, pPassword)) != 0) 
    {
        maKill();
        return INTERNET_STATE_MA_ERR;
    }

    if(pRecvSize < recvBufSize)
    {
        tracker = pRecvSize;
        while(tracker < recvBufSize)
        {
            //Keep calling maUpload until all data copied over
            if ((errNum = maUpload(pURL, NULL, 0, pSendData, sizeof(pSendData), &pRecvData[tracker], recvBufSize - tracker, &pRecvSize, pUserID, pPassword)) != 0)
            {
                maKill();
                return INTERNET_STATE_MA_ERR;
            }
            tracker = tracker + pRecvSize;
        }
    }

    // Disconnect and stop the Mobile Adapter library
    if (maDisconnect() == 0) 
    {
        maEnd();
        return INTERNET_STATE_SUCCESS;
    } 
    else 
    {
        maKill();
        return INTERNET_STATE_MA_ERR;
    }

    return INTERNET_STATE_INPUT_ERR;
}

u8 maGetGiftMon(void)
{
    return INTERNET_STATE_INPUT_ERR;
}
