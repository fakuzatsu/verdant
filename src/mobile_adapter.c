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
extern u8 CopyMonToPC(struct Pokemon *mon);

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

u32 extractBits(const u8 *data, u32 *bitBuffer, int *bitCount, int *dataIndex, int bitSize) 
{
    u32 result = 0;

    while (bitSize > 0) 
    {
        if (*bitCount == 0) 
        {
            // Read the next 4 bytes into the bitBuffer
            u32 buffer = 0;
            buffer |= data[*dataIndex] | (data[*dataIndex + 1] << 8) |
                      (data[*dataIndex + 2] << 16) | (data[*dataIndex + 3] << 24);
            *bitBuffer = buffer;
            *dataIndex += 4; // Move to the next set of bytes
            *bitCount = 32;  // Reset bit count
        }

        int bitsToCopy = bitSize < *bitCount ? bitSize : *bitCount;
        result |= ((*bitBuffer) & ((1 << bitsToCopy) - 1)) << (bitSize - bitsToCopy);
        *bitBuffer >>= bitsToCopy;
        *bitCount -= bitsToCopy;
        bitSize -= bitsToCopy;
    }

    return result;
}

void deserializePokemonSubstruct(union PokemonSubstruct *substruct, const u8 *data) 
{
    for (int i = 0; i < 4; ++i) 
    {
        // Each substruct is 12 bytes (96 bits)
        memcpy(&substruct->raw[i], data + (i * 12), 12);
    }
}

void deserializeBoxPokemon(struct BoxPokemon *box, const u8 *data)
{
    u32 bitBuffer = 0;
    int bitCount = 0;
    int dataIndex = 0;

    box->personality = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 32);

    box->otId = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 32);

    for (int i = 0; i < 10; i++) {
        box->nickname[i] = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 8);
    }

    box->language = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 3);
    box->hiddenNatureModifier = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 5);

    box->isBadEgg = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 1);
    box->hasSpecies = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 1);
    box->isEgg = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 1);
    box->memory = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 5);

    for (int i = 0; i < PLAYER_NAME_LENGTH; i++) 
    {
        box->otName[i] = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 8);
    }

    box->markings = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 4);
    box->compressedStatus = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 4);

    box->checksum = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 16);

    box->hpLost = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 10);
    box->shinyModifier = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 1);
    box->memory2 = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 5);

    deserializePokemonSubstruct(&box->secure.substructs[0], data + dataIndex);
}

void deserializePokemon(struct Pokemon *pokemon, const u8 *data) 
{
    u32 bitBuffer = 0;
    int dataIndex = 0;
    int bitCount = 0;

    deserializeBoxPokemon(&pokemon->box, data);
    dataIndex += sizeof(struct BoxPokemon);

    pokemon->status = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 32);
    pokemon->level = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 8);
    pokemon->mail = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 8);
    pokemon->hp = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 16);
    pokemon->maxHP = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 16);
    pokemon->attack = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 16);
    pokemon->defense = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 16);
    pokemon->speed = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 16);
    pokemon->spAttack = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 16);
    pokemon->spDefense = extractBits(data, &bitBuffer, &bitCount, &dataIndex, 16);
}

// Function to download a Pokemon
// 1. Initializes the Mobile Adapter Library
// 2. Reads the data from the Mobile Adapter's EEPROM
// 3. Establishes a PPP connection to a server
// 4. Sends a POST request to download the Pokemon data
// 5. Disconnects the connection and stops the Mobile Adapter Library
// 6. Adds the Pokemon to the player's party or sends it to the PC if the party is full
// 7. Returns 0 if successful, Mobile Adapter error code if there’s an error, or 2 if the PC is full
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
    char pUserID[MA_USER_SIZE];     // User ID from EEPROM
    char pPassword[MA_PASS_SIZE];   // User password
    char maMailID[MA_MAIL_SIZE];    // Mail ID from EEPROM
    struct Pokemon mon;             // Struct to hold Pokemon data being deserialized

    // Initialize send data and destination
    pURL = "http://www.PutYourDomainHere.com";
    memcpy(pSendData, "GIFT", 4);

    // Initialize the Mobile Adapter library
    errNum = maInitLibrary();
    if (errNum != 0) 
    {
        maKill();
        return errNum;
    }

    // Get EEPROM Data
    errNum = maGetEEPROMData(&maTel, pUserID, maMailID);
    if (errNum != 0) 
    {
        maKill();
        return errNum;
    }

    // Establish a PPP connection to the server
    errNum = maConnectServer(&maTel, pUserID, pPassword);
    if (errNum != 0) 
    {
        maKill();
        return errNum;
    }

    // Send a POST request and receive the response data
    errNum = maUpload(pURL, NULL, 0, pSendData, sizeof(pSendData), pRecvData, recvBufSize, &pRecvSize, pUserID, pPassword);
    if (errNum != 0) 
    {
        maKill();
        return errNum;
    }

    // Ensure all data is received
    totalRecvSize = pRecvSize;
    while (totalRecvSize < recvBufSize) 
    {
        errNum = maUpload(pURL, NULL, 0, pSendData, sizeof(pSendData), &pRecvData[totalRecvSize], recvBufSize - totalRecvSize, &pRecvSize, pUserID, pPassword);
        if (errNum != 0) 
        {
            maKill();
            return errNum;
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
        int pcResult = CopyMonToPC(&mon);
        if (pcResult == 2) 
        {  
            // PC is full
            return 2;
        }
    } 
    else 
    {
        CopyMon(&gPlayerParty[partyIndex], &mon, sizeof(mon));
    }

    // Return 0 to indicate success
    return 0;
}

#endif
