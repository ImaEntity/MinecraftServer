#include "packet.h"

MCPacket CreatePacket(int id, unsigned byte *data, unsigned llong dataSize) {
    MCVarInt packetID = CreateVarInt(id);
    MCVarInt length = CreateVarInt(packetID.size + dataSize);
    return (MCPacket) {
        .length = length,
        .packetID = packetID,
        .data = data
    };
}  

void WritePacket(SOCKET socket, MCPacket packet) {
    MCVarInt length = packet.length;
    MCVarInt id = packet.packetID;

    send(socket, (char *) length.bytes, length.size, 0);
    send(socket, (char *) id.bytes, id.size, 0);
    send(socket, (char *) packet.data, packet.length.value, 0);
}

MCPacket ReadPacket(SOCKET socket) {
    MCVarInt length = ReadVarIntSocket(socket);
    MCVarInt packetID = ReadVarIntSocket(socket);

    unsigned byte *buffer = LocalAlloc(LMEM_FIXED, length.value - packetID.size);
    
    int len = 0;
    
    while(len < length.value - packetID.size)
        len += recv(socket, (char *) buffer + len, length.value - packetID.size - len, 0);
    
    return (MCPacket) {
        .length = length,
        .packetID = packetID,
        .data = buffer
    };
}

void FreePacket(MCPacket packet) {
    if(packet.data == NULL)
        return;

    free(packet.data);
}