#include "global.h"
#include "gpu_regs.h"
#include "main.h"
#include "mobile_adapter.h"
#include "pokemon.h"
#include "pokedex.h"

// The MA Library has to be excluded from the test ROM which necessitates excluding any references to its functions
// Unfortunately this means that we will also have to exclude references to the functions defined here...
#if (!TESTING)

extern void MA_IntrSerialIO(void);
extern void MA_IntrTimer(void);

// Checks if the MA is connected by starting the library and seeing if there's an errors. Ends the communication as well
bool8 maConnected(void)
{
    int isError = 0;

    isError = maInitLibrary();
    maEnd();

    if(isError == 0)
    {
        return TRUE;
    } 
    else
    {
        return FALSE;
    }

}

// Start the MA Library
int maInitLibrary(void)
{
    gIntrTable[1] = MA_IntrSerialIO;
    gIntrTable[2] = MA_IntrTimer;
    u8 hardware;

    MA_InitLibrary(&hardware);

    return maWait();
}

//Disconnect Telephone line  
int maDisconnect(void)
{
    MA_Offline();
    return maWait();
}

//Forcefully terminate Mobile Adapter Communication 
void maKill(void)
{
    MA_Stop();
    maEnd();
}

//Ends the Library Communication and restore the correct interrupts  
void maEnd(void)
{
    MA_End();
    RestoreSerialTimer3IntrHandlers();
}

//Get data stored in the MA EEPROM
int maGetEEPROMData(MA_TELDATA *maTel, char *maUserID, char *maMailID)
{
    int isError = 0;

    MA_GetTel(maTel);
    if((isError = maWait() > 0)) 
    {
        return isError;
    }

    MA_GetUserID(maUserID);
    if((isError = maWait() > 0)) 
    {
        return isError;
    }

    MA_GetMailID(maMailID);
    if((isError = maWait() > 0)) 
    {
        return isError;
    }

    return 0;
}

//Connect to a Server (PPP)
int maConnectServer(const MA_TELDATA *pTelNo, const char *pUserID, const char *pPassword)
{
    MA_TelServer(pTelNo->telNo,pUserID,pPassword);
    return maWait();
}

//Download data
int maDownload(const char *pURL, char *pHeadBuf, u16 headBufSize, u8 *pRecvData, u16 recvBufSize, u16 *pRecvSize, const char *pUserID, const char *pPassword)
{
    MA_HTTP_Get(pURL, pHeadBuf, headBufSize, pRecvData, recvBufSize, pRecvSize, pUserID, pPassword);

    return maWait();
}

//Upload data (and then also download data)
int maUpload(const char *pURL, char *pHeadBuf, u16 headBufSize, const u8 *pSendData, u16 sendSize, u8 *pRecvData, u16 recvBufSize, u16 *pRecvSize, const char *pUserID, const char *pPassword)
{
    MA_HTTP_Post(pURL, pHeadBuf, headBufSize, pSendData, sendSize, pRecvData, 4, pRecvSize, pUserID, pPassword);

    return maWait();
}

//Call a Phone (P2P)
int maCallPhone(const char *pTelNo)
{
    MA_Tel(pTelNo);
    return maWait();
}

//Receive a Phone call (P2P)
int maReceivePhoneCall(void)
{
    MA_Receive();
    return maWait();
}

//Send Data to Phone
int maSendData(const u8 *pSendData, u8 sendSize, u8 *pResult)
{
    MA_SData(pSendData, sendSize, pResult);
    return maWait();
}

//Receive Data from Phone
int maReceiveData(u8 *pRecvData, u8 *pRecvSize)
{
    MA_GData(pRecvData, pRecvSize);
    return maWait();
}


//Waits until action has been completed, check if there's an error and supplies the error code if there is (Nearly all MA functions should use this as their return value, except for maKill and maEnd)
int maWait(void)
{
    u8 maError= 0;
    u16 maErrorProtocol= 0;
    u16 maCondition= 0;

    do {
        maCondition = MAAPI_GetConditionFlag();

        if (maCondition & MA_CONDITION_BUFFER_FULL)
	        return 0;

        if(maCondition & MA_CONDITION_ERROR)
        {
            maError=MAAPI_ErrorCheck(&maErrorProtocol);
            break;
        }

    } while(maCondition & MA_CONDITION_APIWAIT);

    return (maError << 16) | maErrorProtocol;
}

// Function to download a Pokemon
// 1. Initializes the Mobile Adapter Library
// 2. Reads the data from the Mobile Adapter's EEPROM
// 3. Establishes a PPP connection to a server
// 4. Sends a POST request to download the Pokemon data
// 5. Disconnects the connection and stops the Mobile Adapter Library
// 6. Adds the Pokemon to the player's party or sends it to the PC if the party is full
// 7. Returns 0 if successful, Mobile Adapter error code if there’s an error, or 2 if the PC is full
/*
int maExample(void) 
{
    MA_TELDATA maTel;               // MA Telephone struct
    int errNum;                     // Error code
    const char *pURL;               // URL to query
    u8 pSendData[4];                // Data to send
    u16 recvBufSize = 100;          // Size of the receive buffer
    u8 pRecvData[recvBufSize];      // Buffer to hold received data
    u16 pRecvSize = 0;              // Size of data received in each upload call
    int totalRecvSize = 0;          // Total size of data received
    char pUserID[32];               // User ID from EEPROM
    char pPassword[16];             // User password
    char maMailID[30];              // Mail ID from EEPROM
    struct BoxPokemon mon;          // Struct to hold Pokemon data being deserialized

    // Initialize send data and destination
    pURL = "http://www.PutYourDomainHere.com";
    memcpy(pSendData, "GIFT", 4);

    // Initialize the Mobile Adapter library
    errNum = maInitLibrary();
    if (errNum != 0) 
    {
        maKill();
        return INTERNET_STATE_MA_ERR;
    }

    // Get EEPROM Data
    errNum = maGetEEPROMData(&maTel, pUserID, maMailID);
    if (errNum != 0) 
    {
        maKill();
        return INTERNET_STATE_MA_ERR;
    }

    // Establish a PPP connection to the server
    errNum = maConnectServer(&maTel, pUserID, pPassword);
    if (errNum != 0) 
    {
        maKill();
        return INTERNET_STATE_MA_ERR;
    }

    // Send a POST request and receive the response data
    errNum = maUpload(pURL, NULL, 0, pSendData, sizeof(pSendData), pRecvData, recvBufSize, &pRecvSize, pUserID, pPassword);
    if (errNum != 0) 
    {
        maKill();
        return INTERNET_STATE_MA_ERR;
    }

    // Ensure all data is received
    totalRecvSize = pRecvSize;
    while (totalRecvSize < recvBufSize) 
    {
        errNum = maUpload(pURL, NULL, 0, pSendData, sizeof(pSendData), &pRecvData[totalRecvSize], recvBufSize - totalRecvSize, &pRecvSize, pUserID, pPassword);
        if (errNum != 0) 
        {
            maKill();
            return INTERNET_STATE_MA_ERR;
        }
        totalRecvSize += pRecvSize;
    }

    // Disconnect and stop the Mobile Adapter library
    if (maDisconnect() == 0) 
    {
        maEnd();
    } 
    else 
    {
        maKill();
    }

    // Calculate the number of Pokémon in the party
    int partyIndex;
    for (partyIndex = 0; partyIndex < PARTY_SIZE; partyIndex++) 
    {
        if (GetMonData(&gPlayerParty[partyIndex], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            break;
    }

    deserializePokemon(&mon, pRecvData);

    // If the party is full, attempt to send the Pokémon to the PC
    if (partyIndex >= PARTY_SIZE) 
    {
        int pcResult = CopyBoxMonToPC(&mon);
        if (pcResult == 2) 
        {  
            // PC is full
            return INTERNET_STATE_PC_FULL;
        }

        return INTERNET_STATE_SENT_TO_PC;
    } 
    else 
    {
        BoxMonToMon(&mon, &gPlayerParty[partyIndex]);

        return INTERNET_STATE_SENT_TO_PARTY;
    }

    return INTERNET_STATE_INPUT_ERR;
}
*/

#endif
