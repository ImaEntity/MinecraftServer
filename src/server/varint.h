#ifndef HH_SERVER_VARINT
#define HH_SERVER_VARINT

#include "../types.h"
#include <winsock2.h>

#define MAX_VARINT_LEN 5
#define MAX_VARLONG_LEN 10

int ReadVarInt(SOCKET socket, byte *byteLen);
llong ReadVarLong(SOCKET socket, byte *byteLen);

byte WriteVarInt(SOCKET socket, int value);
byte WriteVarLong(SOCKET socket, llong value);

int ConsumeVarInt(byte *buffer, byte *byteLen, ulong *offset);
llong ConsumeVarLong(byte *buffer, byte *byteLen, ulong *offset);

byte AppendVarInt(byte *buffer, int value, ulong *offset);
byte AppendVarLong(byte *buffer, llong value, ulong *offset);

#endif