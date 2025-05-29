#ifndef HH_SERVER
#define HH_SERVER

#include <winsock2.h>
#include "types.h"

// wtvrs normally in server.properties
typedef struct {
    boolean allowNether;
    Difficulty difficulty;
    boolean enableCommandBlock;
    Gamemode gamemode;
    boolean generateStructures;
    boolean hardcore;
    unsigned llong levelSeed;
    int maxPlayers;
    char *motd;
    boolean onlineMode; // FALSE (online not yet supported)
    boolean pvp;
    int simulationDistance;
    boolean spawnMonsters;
    int spawnProtection;
    int viewDistance;
} ServerProperties;

typedef struct {
    unsigned short port;
    ServerProperties properties;
    SOCKET socket;
} Server;

Server CreateServer(ServerProperties properties, unsigned short port);
void StartServer(Server server);
void DestroyServer(Server server);

#endif