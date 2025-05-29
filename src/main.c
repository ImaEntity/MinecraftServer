#include "utils/math.h"
#include "utils/vector.h"
#include "server/server.h"

#include <windows.h>

void CRTBootstrapper() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    Server server = CreateServer((ServerProperties) {
        .allowNether = true,
        .difficulty = DIF_EASY,
        .enableCommandBlock = true,
        .gamemode = GM_SURVIVAL,
        .generateStructures = true,
        .hardcore = true,
        .levelSeed = -1,
        .maxPlayers = 8,
        .motd = "Hello, world!",
        .onlineMode = false,
        .pvp = true,
        .simulationDistance = 5,
        .spawnMonsters = true,
        .spawnProtection = 0,
        .viewDistance = 8
    }, 25565);

    StartServer(server);

    WSACleanup();
    ExitProcess(0);
}