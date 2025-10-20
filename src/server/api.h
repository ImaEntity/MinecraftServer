#ifndef HH_SERVER_API
#define HH_SERVER_API

#include "../types.h"

#define DEFAULT_PORT 25565

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

typedef struct {
    boolean allowNether;
    Difficulty difficulty;
    boolean enableCommandBlock;
    Gamemode gamemode;
    boolean generateStructures;
    boolean hardcore;
    ulong levelSeed;
    int maxPlayers;
    char *motd;
    boolean onlineMode; // false (online not yet supported)
    boolean pvp;
    int simulationDistance;
    boolean spawnMonsters;
    int spawnProtection;
    int viewDistance;
} ServerProperties;

typedef void *Server;

Server CreateServer(ServerProperties properties);
void StartServer(Server server, ushort port);
boolean IsRunning(Server server);
void RunCommand(Server server, const char *cmd);
void StopServer(Server server);

#endif