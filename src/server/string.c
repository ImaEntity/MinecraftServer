#include "string.h"
#include "varint.h"
#include "../types.h"

#include <windows.h>

char *ReadString(SOCKET socket, ushort *outLen) {
    int length = ReadVarInt(socket, NULL);
    char *string = HeapAlloc(GetProcessHeap(), 0, length + 1);

    recv(socket, string, length, 0);
    string[length] = '\0';

    if(outLen != NULL) *outLen = length;
    return string;
}

ushort WriteString(SOCKET socket, char *string) {
    int length = strlen(string);

    WriteVarInt(socket, length);
    send(socket, string, length, 0);

    return length;
}

char *ConsumeString(byte *buffer, ushort *outLen, ulong *offset) {
    ulong off = offset != NULL ? *offset : 0;
    int length = ConsumeVarInt(buffer, NULL, &off);
    char *string = HeapAlloc(GetProcessHeap(), 0, length + 1);

    memcpy(string, buffer + off, length);
    string[length] = '\0';
    off += length;

    if(offset != NULL) *offset = off;
    if(outLen != NULL) *outLen = length;
    return string;
}

ushort AppendString(byte *buffer, char *string, ulong *offset) {
    ulong off = offset != NULL ? *offset : 0;
    ushort length = strlen(string);

    AppendVarInt(buffer, length, &off);
    memcpy(buffer + off, string, length);
    off += length;

    if(offset != NULL) *offset = off;
    return length;
}

void FreeString(char *string) {
    HeapFree(GetProcessHeap(), 0, string);
}