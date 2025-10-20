#ifndef HH_SERVER_STRING
#define HH_SERVER_STRING

#include "../types.h"
#include <winsock2.h>

char *ReadString(SOCKET socket, ushort *outLen);
ushort WriteString(SOCKET socket, char *string);
char *ConsumeString(byte *buffer, ushort *outLen, ulong *offset);
ushort AppendString(byte *buffer, char *string, ulong *offset);

void FreeString(char *string);

#endif