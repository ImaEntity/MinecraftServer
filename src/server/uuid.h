#ifndef HH_SERVER_UUID
#define HH_SERVER_UUID

#include "../types.h"
#include <winsock2.h>

UUID ReadUUID(SOCKET socket);
void WriteUUID(SOCKET socket, UUID value);
UUID ConsumeUUID(byte *buffer, ulong *offset);
void AppendUUID(byte *buffer, UUID value, ulong *offset);

UUID DeriveUUID(char *str, ulong length);

#endif