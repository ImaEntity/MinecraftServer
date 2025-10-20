#include "uuid.h"
#include "../types.h"

#include <windows.h>

UUID ReadUUID(SOCKET socket) {
    byte tlBuf[4];
    recv(socket, (char *) tlBuf, 4, 0);
    uint tl = ntohl(*(uint *) tlBuf);

    byte tmBuf[2];
    recv(socket, (char *) tmBuf, 2, 0);
    ushort tm = ntohs(*(ushort *) tmBuf);

    byte thBuf[2];
    recv(socket, (char *) thBuf, 2, 0);
    ushort th = ntohs(*(ushort *) thBuf);

    byte uuidBuf[8];
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

void WriteUUID(SOCKET socket, UUID value) {
    byte tlBuf[4];
    *(uint *) tlBuf = htonl(value.Data1);
    send(socket, (char *) tlBuf, 4, 0);

    byte tmBuf[2];
    *(ushort *) tmBuf = htons(value.Data2);
    send(socket, (char *) tmBuf, 2, 0);

    byte thBuf[2];
    *(ushort *) thBuf = htons(value.Data3);
    send(socket, (char *) thBuf, 2, 0);

    byte uuidBuf[8];
    for(int i = 0; i < 8; i++)
        uuidBuf[i] = value.Data4[i];

    send(socket, (char *) uuidBuf, 8, 0);
}

UUID ConsumeUUID(byte *buffer, ulong *offset) {
    ulong off = offset != NULL ? *offset : 0;

    u32 tl = ntohl(*(u32 *) (buffer + off));
    off += 4;

    u16 tm = ntohs(*(u16 *) (buffer + off));
    off += 2;

    u16 th = ntohs(*(u16 *) (buffer + off));
    off += 2;

    UUID uuid = (UUID) {
        .Data1 = tl,
        .Data2 = tm,
        .Data3 = th
    };

    for(int i = 0; i < 8; i++)
        uuid.Data4[i] = buffer[off + i];

    off += 8;
    
    if(offset != NULL) *offset = off;
    return uuid;
}

void AppendUUID(byte *buffer, UUID value, ulong *offset) {
    ulong off = offset != NULL ? *offset : 0;

    *(u32 *) (buffer + off) = htonl(value.Data1);
    off += 4;

    *(u16 *) (buffer + off) = htons(value.Data2);
    off += 2;

    *(u16 *) (buffer + off) = htons(value.Data3);
    off += 2;

    for(int i = 0; i < 8; i++)
        buffer[off + i] = value.Data4[i];

    off += 8;

    if(offset != NULL) *offset = off;
}

UUID DeriveUUID(char *str, ulong length) {
    byte uuid[16] = {0};
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