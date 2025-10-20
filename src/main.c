#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <windows.h>

#include "types.h"
#include "server/api.h"

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    Server server = CreateServer((ServerProperties) {
        .allowNether = true,
        .difficulty = DIF_NORMAL,
        .enableCommandBlock = true,
        .gamemode = GM_SURVIVAL,
        .generateStructures = true,
        .hardcore = false,
        .levelSeed = 0xC0FFEE,
        .maxPlayers = 8,
        .motd = "Hello, world!",
        .onlineMode = false, // easier to implement
        .pvp = true,
        .simulationDistance = 8,
        .spawnMonsters = true,
        .spawnProtection = 0,
        .viewDistance = 16
    });

    StartServer(server, DEFAULT_PORT);

    char buffer[4096];
    ulong size = 0;

    while(IsRunning(server)) {
        // check input for new chars
        // if buffer has a newline
        // split at newline and parse as command

        if(!_kbhit()) continue;
        
        char c = _getch();
        if(c == '\r') continue; // nice try

        buffer[size++] = c;
        if(c == '\n') {
            buffer[size] = 0;
            RunCommand(server, buffer);
            size = 0;
        }
    }

    StopServer(server);
    WSACleanup();
    return 0;
}