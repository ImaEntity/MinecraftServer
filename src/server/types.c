#include <winsock2.h>
#include "types.h"

#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80

MCVarInt CreateVarInt(int value) {
    MCVarInt v = {.value = value, .size = 0};

    while(true) {
        if((value & ~SEGMENT_BITS) == 0) {
            v.bytes[v.size++] = value;
            break;
        }

        v.bytes[v.size++] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (int) ((unsigned int) value >> 7); // force logical shift
    }
    
    return v;
}

MCVarInt ReadVarIntBuffer(unsigned byte *bytes) {
    MCVarInt v = {.value = 0, .size = 0};
    int position = 0;

    while(true) {
        byte b = bytes[v.size];
        
        v.bytes[v.size] = b;
        v.size++;

        v.value |= (b & SEGMENT_BITS) << position;
        if((b & CONTINUE_BIT) == 0) break;

        position += 7;
        if(position >= 32) return NULL_VARINT;
    }

    return v;
}

MCVarInt ReadVarIntSocket(SOCKET socket) {
    MCVarInt v = {.value = 0, .size = 0};
    int position = 0;

    while(true) {
        byte b;
        recv(socket, &b, 1, 0);

        v.bytes[v.size] = b;
        v.size++;

        v.value |= (b & SEGMENT_BITS) << position;
        if((b & CONTINUE_BIT) == 0) break;

        position += 7;
        if(position >= 32)
            return NULL_VARINT;
    }

    return v;
}

MCVarLong CreateVarLong(llong value) {
    MCVarLong v = {.value = value, .size = 0};

    while(true) {
        if((value & ~SEGMENT_BITS) == 0) {
            v.bytes[v.size++] = value;
            break;
        }

        v.bytes[v.size++] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (llong) ((unsigned llong) value >> 7); // force logical shift
    }

    return v;
}

MCVarLong ReadVarLongBuffer(unsigned byte *bytes) {
    MCVarLong v = {.value = 0, .size = 0};
    int position = 0;

    while(true) {
        byte b = bytes[v.size];

        v.bytes[v.size] = b;
        v.size++;

        v.value |= (b & SEGMENT_BITS) << position;
        if((b & CONTINUE_BIT) == 0) break;

        position += 7;
        if(position >= 64) return NULL_VARLONG;
    }

    return v;
}

MCVarLong ReadVarLongSocket(SOCKET socket) {
    MCVarLong v = {.value = 0, .size = 0};
    int position = 0;

    while(true) {
        byte b;
        recv(socket, &b, 1, 0);

        v.bytes[v.size] = b;
        v.size++;

        v.value |= (b & SEGMENT_BITS) << position;
        if((b & CONTINUE_BIT) == 0) break;

        position += 7;
        if(position >= 64) return NULL_VARLONG;
    }

    return v;
}


String CreateString(char *ascii) {
    int len = 0;
    while(ascii[len++] != '\0');

    String string = {
        .length = CreateVarInt(len - 1),
        .bytes = LocalAlloc(LMEM_FIXED, len)
    };

    for(int i = 0; i < len; i++)
        string.bytes[i] = ascii[i];

    return string;
}

String ReadStringBuffer(unsigned byte *bytes) {
    MCVarInt len = ReadVarIntBuffer(bytes);
    String string = {
        .length = len,
        .bytes = LocalAlloc(LMEM_FIXED, len.value)
    };

    for(int i = 0; i < len.value; i++)
        string.bytes[i] = bytes[len.size + i];

    return string;
}

String ReadStringSocket(SOCKET socket) {
    MCVarInt len = ReadVarIntSocket(socket);
    String string = {
        .length = len,
        .bytes = LocalAlloc(LMEM_FIXED, len.value)
    };

    recv(socket, (char *) string.bytes, len.value, 0);
    return string;
}

void WriteStringBuffer(String string, unsigned byte *bytes) {
    for(int i = 0; i < string.length.size; i++)
        bytes[i] = string.length.bytes[i];

    for(int i = 0; i < string.length.value; i++)
        bytes[string.length.size + i] = string.bytes[i];
}

void WriteStringSocket(String string, SOCKET socket) {
    send(socket, (char *) string.length.bytes, string.length.size, 0);
    send(socket, (char *) string.bytes, string.length.value, 0);
}

void FreeString(String string) {
    if(string.bytes == NULL)
        return;

    LocalFree(string.bytes);
}

UUID DeriveUUID(char *str, unsigned llong length) {
    unsigned byte uuid[16] = {0};
    DWORD uuidLen = 16;
    HCRYPTPROV hProv;
    HCRYPTHASH hHash;

    CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
    CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);

    CryptHashData(hHash, (unsigned char *) str, length, 0);
    CryptGetHashParam(hHash, HP_HASHVAL, uuid, &uuidLen, 0);

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    uuid[6] = (uuid[6] & 0x0F) | 0x30;
    uuid[8] = (uuid[8] & 0x3F) | 0x80;

    return *(UUID *) uuid;
}

UUID ReadUUIDBuffer(unsigned byte *bytes) {
    unsigned byte *tlBuf = bytes + 0;
    unsigned int tl = ntohl(*(unsigned int *) tlBuf);

    unsigned byte *tmBuf = bytes + 4;
    unsigned short tm = ntohs(*(unsigned short *) tmBuf);

    unsigned byte *thBuf = bytes + 6;
    unsigned short th = ntohs(*(unsigned short *) thBuf);

    unsigned byte *uuidBuf = bytes + 8;

    UUID uuid = (UUID) {
        .Data1 = tl,
        .Data2 = tm,
        .Data3 = th
    };

    for(int i = 0; i < 8; i++)
        uuid.Data4[i] = uuidBuf[i];
    
    return uuid;
}

UUID ReadUUIDSocket(SOCKET socket) {
    unsigned byte tlBuf[4];
    recv(socket, (char *) tlBuf, 4, 0);
    unsigned int tl = ntohl(*(unsigned int *) tlBuf);

    unsigned byte tmBuf[2];
    recv(socket, (char *) tmBuf, 2, 0);
    unsigned short tm = ntohs(*(unsigned short *) tmBuf);

    unsigned byte thBuf[2];
    recv(socket, (char *) thBuf, 2, 0);
    unsigned short th = ntohs(*(unsigned short *) thBuf);

    unsigned byte uuidBuf[8];
    recv(socket, (char *) uuidBuf, 8, 0);

    UUID uuid = (UUID) {
        .Data1 = tl,
        .Data2 = tm,
        .Data3 = th
    };

    for(int i = 0; i < 8; i++)
        uuid.Data4[i] = uuidBuf[i];
    
    return uuid;
}

void WriteUUIDBuffer(UUID uuid, unsigned byte *bytes) {
    unsigned byte *tlBuf = bytes + 0;
    *(unsigned int *) tlBuf = htonl(uuid.Data1);

    unsigned byte *tmBuf = bytes + 4;
    *(unsigned short *) tmBuf = htons(uuid.Data2);

    unsigned byte *thBuf = bytes + 6;
    *(unsigned short *) thBuf = htons(uuid.Data3);

    unsigned byte *uuidBuf = bytes + 8;

    for(int i = 0; i < 8; i++)
        uuidBuf[i] = uuid.Data4[i];
}

void WriteUUIDSocket(UUID uuid, SOCKET socket) {
    unsigned byte tlBuf[4];
    *(unsigned int *) tlBuf = htonl(uuid.Data1);
    send(socket, (char *) tlBuf, 4, 0);

    unsigned byte tmBuf[2];
    *(unsigned short *) tmBuf = htons(uuid.Data2);
    send(socket, (char *) tmBuf, 2, 0);

    unsigned byte thBuf[2];
    *(unsigned short *) thBuf = htons(uuid.Data3);
    send(socket, (char *) thBuf, 2, 0);

    unsigned byte uuidBuf[8];
    for(int i = 0; i < 8; i++)
        uuidBuf[i] = uuid.Data4[i];

    send(socket, (char *) uuidBuf, 8, 0);
}