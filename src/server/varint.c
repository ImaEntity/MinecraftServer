#include "varint.h"
#include "../types.h"

#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80

int ReadVarInt(SOCKET socket, byte *byteLen) {
    int size = 0;
    int val = 0;
    int pos = 0;

    while(true) {
        byte b;
        if(recv(socket, (char *) &b, 1, 0) == 0)
            return ERROR_VALUE;

        size++;

        val |= (b & SEGMENT_BITS) << pos;
        if((b & CONTINUE_BIT) == 0) break;

        pos += 7;
        if(pos >= 32) return ERROR_VALUE;
    }

    if(byteLen != NULL) *byteLen = size;
    return val;
}

llong ReadVarLong(SOCKET socket, byte *byteLen) {
    llong val = 0;
    int size = 0;
    int pos = 0;

    while(true) {
        byte b;
        if(recv(socket, (char *) &b, 1, 0) == 0)
            return ERROR_VALUE;

        size++;

        val |= (b & SEGMENT_BITS) << pos;
        if((b & CONTINUE_BIT) == 0) break;

        pos += 7;
        if(pos >= 64) return ERROR_VALUE;
    }

    if(byteLen != NULL) *byteLen = size;
    return val;
}


byte WriteVarInt(SOCKET socket, int value) {
    byte byteLen = 0;

    while(true) {
        if((value & ~SEGMENT_BITS) == 0) {
            send(socket, (char *) &value, 1, 0);
            return ++byteLen;
        }

        byte b = (value & SEGMENT_BITS) | CONTINUE_BIT;
        send(socket, (char *) &b, 1, 0);

        value >>= 7;
        byteLen++;
    }
}

byte WriteVarLong(SOCKET socket, llong value) {
    byte byteLen = 0;

    while(true) {
        if((value & ~((llong) SEGMENT_BITS)) == 0) {
            send(socket, (char *) &value, 1, 0);
            return ++byteLen;
        }

        byte b = (value & SEGMENT_BITS) | CONTINUE_BIT;
        send(socket, (char *) &b, 1, 0);

        value >>= 7;
        byteLen++;
    }
}


int ConsumeVarInt(byte *buffer, byte *byteLen, ulong *offset) {
    int size = 0;
    int val = 0;
    int pos = 0;

    ulong off = offset != NULL ? *offset : 0;
    while(true) {
        byte b = buffer[off++];
        size++;

        val |= (b & SEGMENT_BITS) << pos;
        if((b & CONTINUE_BIT) == 0) break;

        pos += 7;
        if(pos >= 32) return ERROR_VALUE;
    }

    if(offset != NULL) *offset = off;
    if(byteLen != NULL) *byteLen = size;
    return val;
}

llong ConsumeVarLong(byte *buffer, byte *byteLen, ulong *offset) {
    llong val = 0;
    int size = 0;
    int pos = 0;

    ulong off = offset != NULL ? *offset : 0;
    while(true) {
        byte b = buffer[off++];
        size++;

        val |= (b & SEGMENT_BITS) << pos;
        if((b & CONTINUE_BIT) == 0) break;

        pos += 7;
        if(pos >= 64) return ERROR_VALUE;
    }

    if(offset != NULL) *offset = off;
    if(byteLen != NULL) *byteLen = size;
    return val;
}


byte AppendVarInt(byte *buffer, int value, ulong *offset) {
    byte byteLen = 0;
    ulong off = offset != NULL ? *offset : 0;
    
    while(true) {
        if((value & ~SEGMENT_BITS) == 0) {
            buffer[off++] = value;
            byteLen++;
            break;
        }

        byte b = (value & SEGMENT_BITS) | CONTINUE_BIT;
        buffer[off++] = b;

        value >>= 7;
        byteLen++;
    }

    if(offset != NULL) *offset = off;
    return byteLen;
}

byte AppendVarLong(byte *buffer, llong value, ulong *offset) {
    byte byteLen = 0;
    ulong off = offset != NULL ? *offset : 0;

    while(true) {
        if((value & ~((llong) SEGMENT_BITS)) == 0) {
            buffer[off++] = value;
            byteLen++;
            break;
        }

        byte b = (value & SEGMENT_BITS) | CONTINUE_BIT;
        buffer[off++] = b;

        value >>= 7;
        byteLen++;
    }

    if(offset != NULL) *offset = off;
    return byteLen;
}