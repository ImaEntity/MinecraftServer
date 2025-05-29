#ifndef HH_PACKET
#define HH_PACKET

#include <winsock2.h>
#include "types.h"

/*==============================================================================
 * Client to Server packets
 *============================================================================*/

// Handshake
#define C2S_HANDSHAKE_HANDSHAKE 0x00

// Status
#define C2S_STATUS_STATUS_REQUEST 0x00
#define C2S_STATUS_PING_REQUEST   0x01

// Login
#define C2S_LOGIN_LOGIN_START         0x00
#define C2S_LOGIN_ENCRYPTION_RESPONSE 0x01
#define C2S_LOGIN_PLUGIN_RESPONSE     0x02

/*==============================================================================
 * Server to Client packets
 * Handshake not included as no S2C handshake packets exist
 *============================================================================*/

// Status
#define S2C_STATUS_STATUS_RESPONSE 0x00
#define S2C_STATUS_PING_RESPONSE   0x01

// Login
#define S2C_LOGIN_DISCONNECT         0x00
#define S2C_LOGIN_ENCRYPTION_REQUEST 0x01
#define S2C_LOGIN_LOGIN_SUCCESS      0x02
#define S2C_LOGIN_SET_COMPRESSION    0x03
#define S2C_LOGIN_PLUGIN_REQUEST     0x04

// Play
#define S2C_PLAY_DISCONNECT 0x1A

typedef struct {
    MCVarInt length;
    MCVarInt packetID;
    unsigned byte *data;
} MCPacket;

MCPacket CreatePacket(int id, unsigned byte *data, unsigned llong dataSize);
void WritePacket(SOCKET socket, MCPacket packet);
MCPacket ReadPacket(SOCKET socket);
void FreePacket(MCPacket packet);

#endif