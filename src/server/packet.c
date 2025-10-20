#include "packet.h"
#include "varint.h"
#include "../types.h"

#include <stdio.h>
#include <windows.h>

boolean ReadPacket(SOCKET socket, MCPacket *packet) {
    byte idLen;
    packet -> length = ReadVarInt(socket, NULL);
    packet -> id = ReadVarInt(socket, &idLen);

    if(packet -> length == ERROR_VALUE) return false;
    if(packet -> id == ERROR_VALUE) return false;

    printf("vPacket{ID=%02X,LEN=%d}\n", packet -> id, packet -> length);
    if(packet -> length - idLen == 0) return true;

    packet -> data = HeapAlloc(GetProcessHeap(), 0, packet -> length - idLen);
    if(packet -> data == NULL) return false;
    
    if(recv(socket, (char *) packet -> data, packet -> length - idLen, 0) == 0) {
        HeapFree(GetProcessHeap(), 0, packet -> data);
        return false;
    }
    
    return true;
}

void WritePacket(SOCKET socket, MCPacket packet) {
    byte *buf = LocalAlloc(LMEM_FIXED, packet.length);
    if(buf == NULL) return;

    ulong off = 0;
    byte idLen = AppendVarInt(buf, packet.id, &off);

    WriteVarInt(socket, packet.length);
    if(packet.length - idLen > 0) {
        memcpy(buf + off, packet.data, packet.length - off);
        send(socket, (char *) buf, packet.length, 0);
    }

    LocalFree(buf);
    printf("^Packet{ID=%02X,LEN=%d}\n", packet.id, packet.length);
}

void SendPacket(SOCKET socket, int id, byte *data, ulong dataSize) {
    byte idBuf[MAX_VARINT_LEN];
    byte idLen = AppendVarInt(idBuf, id, NULL);

    WritePacket(socket, (MCPacket) {
        .length = idLen + dataSize,
        .data = data,
        .id = id
    });
}

void FreePacket(MCPacket *packet) {
    if(packet == NULL) return;
    if(packet -> data == NULL) return;

    HeapFree(GetProcessHeap(), 0, packet -> data);
    packet -> data = NULL;
}