#ifndef HH_TYPES
#define HH_TYPES
#pragma pack(push, 1)

#include <winsock2.h>

#define true 1
#define false 0

#define byte char
#define llong long long

typedef unsigned byte boolean;

typedef struct {
    int value;
    unsigned byte size;
    byte bytes[5];
} MCVarInt;

typedef struct {
    llong value;
    unsigned byte size;
    byte bytes[10];
} MCVarLong;

typedef struct {
    MCVarInt length;
    byte *bytes;
} String;

typedef struct {
    String id;
    String value;
} Identifier;

typedef enum {
    SS_HANDSHAKE,
    SS_STATUS,
    SS_LOGIN,
    SS_PLAY
} ServerState;

typedef enum {
    DIF_PEACEFUL,
    DIF_EASY,
    DIF_NORMAL,
    DIF_HARD
} Difficulty;

typedef enum {
    GM_SURVIVAL,
    GM_CREATIVE,
    GM_ADVENTURE,
    GM_SPECTATOR
} Gamemode;

#define NULL_VARINT ((MCVarInt) {0, 0, {0, 0, 0, 0, 0}})
#define NULL_VARLONG ((MCVarLong) {0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})

MCVarInt CreateVarInt(int value);
MCVarInt ReadVarIntBuffer(unsigned byte *bytes);
MCVarInt ReadVarIntSocket(SOCKET socket);

MCVarLong CreateVarLong(llong value);
MCVarLong ReadVarLongBuffer(unsigned byte *bytes);
MCVarLong ReadVarLongSocket(SOCKET socket);

String CreateString(char *ascii);
String ReadStringBuffer(unsigned byte *bytes);
String ReadStringSocket(SOCKET socket);
void WriteStringBuffer(String string, unsigned byte *bytes);
void WriteStringSocket(String string, SOCKET socket);
void FreeString(String string);

UUID DeriveUUID(char *str, unsigned llong length);
UUID ReadUUIDBuffer(unsigned byte *bytes);
UUID ReadUUIDSocket(SOCKET socket);
void WriteUUIDBuffer(UUID uuid, unsigned byte *bytes);
void WriteUUIDSocket(UUID uuid, SOCKET socket);

#pragma pack(pop)
#endif