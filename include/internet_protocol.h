#ifndef GUARD_INTERNET_PROTOCOL_H
#define GUARD_INTERNET_PROTOCOL_H

#include "global.h"

#define INTERNET_PROTOCOL_VERSION 1
#define INTERNET_SESSION_NONCE_SIZE 16
#define INTERNET_SESSION_TOKEN_SIZE 8
#define INTERNET_IDENTITY_TOKEN_SIZE 3
#define INTERNET_SESSION_CHALLENGE_PACKET_SIZE 24
#define INTERNET_SESSION_AUTH_PACKET_SIZE 32
#define INTERNET_SESSION_RESPONSE_PACKET_SIZE 16
#define INTERNET_IDENTITY_REQUEST_PACKET_SIZE 8
#define INTERNET_IDENTITY_RESPONSE_PACKET_SIZE 16
#define INTERNET_CREDENTIAL_PACKET_SIZE 16
#define INTERNET_ACK_PACKET_SIZE 8
#define INTERNET_SESSION_TOKEN_TEXT_SIZE (INTERNET_SESSION_TOKEN_SIZE * 2 + 1)

bool32 DecodeInternetSessionChallenge(const u8 *packet, u16 packetSize, u8 *nonce);
bool32 BuildInternetSessionAuthPacket(const u8 *nonce, u8 *packet);
bool32 DecodeInternetSessionResponse(const u8 *packet, u16 packetSize, u8 *token);
void FormatInternetSessionToken(const u8 *token, char *text);
void BuildInternetIdentityRequest(u8 *packet);
bool32 DecodeInternetIdentityResponse(const u8 *packet, u16 packetSize, u32 *pid, u8 *token);
void BuildInternetCredentialPacket(const char *magic, u32 pid, const u8 *token, u8 *packet);
bool32 DecodeInternetAcknowledgement(const u8 *packet, u16 packetSize);

#endif // GUARD_INTERNET_PROTOCOL_H
