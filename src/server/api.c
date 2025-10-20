#include "api.h"
#include "player.h"
#include "packet.h"
#include "varint.h"
#include "string.h"
#include "uuid.h"
#include "../types.h"

#include "../yyjson.h"
#include <windows.h>

typedef enum {
    HANDSHAKE,
    STATUS,
    LOGIN,
    PLAY
} ClientState;

typedef struct {
    ServerProperties properties;
    ushort port;

    HANDLE socketThread;
    HANDLE tickThread;

    SOCKET socket;
    
    struct Client *firstClient;
    ushort clientCount;
} iServer;

typedef struct Client {
    iServer *server;

    struct Client *last;
    struct Client *next;

    SOCKET socket;
    char *ip;
    ushort port;

    ClientState state;
    Player player;
} Client;

Server CreateServer(ServerProperties properties) {
    iServer *server = HeapAlloc(GetProcessHeap(), 0, sizeof(iServer));

    server -> properties = properties;    
    server -> clientCount = 0;

    server -> firstClient = HeapAlloc(
        GetProcessHeap(),
        0, sizeof(Client)
    );

    server -> firstClient -> next = NULL;
    server -> firstClient -> last = NULL;

    return server;
}

boolean sapi_handle_handshake(Client *client, MCPacket packet) {
    if(packet.id != C2S_HANDSHAKE_HANDSHAKE) return true; // invalid

    ulong offset = 0;
    ConsumeVarInt(packet.data, NULL, &offset);             // protocol number
    FreeString(ConsumeString(packet.data, NULL, &offset)); // server address
    offset += sizeof(ushort);                              // server port

    int nextState = ConsumeVarInt(packet.data, NULL, &offset);
    switch(nextState) {
        case 1: client -> state = STATUS; break;
        case 2: client -> state = LOGIN; break;
        default: return true; // invalid
    }

    return false;
}

boolean sapi_handle_status(Client *client, MCPacket packet) {
    switch(packet.id) {
        case C2S_STATUS_STATUS_REQUEST: {
            yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
            yyjson_mut_val *root = yyjson_mut_obj(doc);
            yyjson_mut_val *version = yyjson_mut_obj(doc);
            yyjson_mut_val *players = yyjson_mut_obj(doc);
            yyjson_mut_val *description = yyjson_mut_obj(doc);
            
            yyjson_mut_obj_add_int(doc, players, "max", client -> server -> properties.maxPlayers);
            yyjson_mut_obj_add_int(doc, players, "online", -1);

            yyjson_mut_obj_add_str(doc, version, "name", "1.20.1");
            yyjson_mut_obj_add_int(doc, version, "protocol", 763); // 1.20 - 1.20.1

            yyjson_mut_obj_add_bool(doc, root, "enforcesSecureChat", false);

            yyjson_mut_obj_add_str(doc, description, "text", client -> server -> properties.motd);

            yyjson_mut_obj_add_val(doc, root, "players", players);
            yyjson_mut_obj_add_val(doc, root, "version", version);
            yyjson_mut_obj_add_val(doc, root, "description", description);
            yyjson_mut_obj_add_str(doc, root, "favicon", "data:image/png;base64,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");
            yyjson_mut_doc_set_root(doc, root);

            char *json = yyjson_mut_write(doc, 0, NULL);
            
            ulong offset = 0;
            byte buffer[8192];
            AppendString(buffer, json, &offset);

            SendPacket(
                client -> socket,
                S2C_STATUS_STATUS_RESPONSE,
                buffer, offset
            );

            yyjson_mut_doc_free(doc);
            free(json);
            break;
        }

        case C2S_STATUS_PING_REQUEST: {
            // echo packet data back
            packet.id = S2C_STATUS_PING_RESPONSE;
            WritePacket(
                client -> socket,
                packet
            );

            // client assumes disconnect once this packet is received
            return true;
        }

        default:
            return true;
    }

    return false;
}

boolean sapi_handle_login(Client *client, MCPacket packet) {
    switch(packet.id) {
        case C2S_LOGIN_LOGIN_START: {
            ulong offset = 0;

            ushort nameLen;
            char *username = ConsumeString(packet.data, &nameLen, &offset);
            boolean hasUUID = *(packet.data + offset++) == 1;
            
            char *userBuf = LocalAlloc(LMEM_FIXED, nameLen + 15);
            memcpy(userBuf, "OfflinePlayer:", 14);
            memcpy(userBuf + 14, username, nameLen);
            userBuf[nameLen + 14] = 0;

            UUID uuid = DeriveUUID(userBuf, nameLen + 14);
            if(hasUUID) uuid = ConsumeUUID(packet.data, &offset);

            offset = 0;
            byte *buffer = LocalAlloc(LMEM_FIXED, 48);
        
            AppendUUID(buffer, uuid, &offset);
            AppendString(buffer, username, &offset);
            // idk what goes in here so just sent a zero length array
            AppendVarInt(buffer, 0, &offset);

            SendPacket(
                client -> socket,
                S2C_LOGIN_LOGIN_SUCCESS,
                buffer, offset
            );

            break;
        }
    }

    return false;
}

boolean sapi_handle_play(Client *client, MCPacket packet) {
    return true;
}

DWORD sapi_listener_thread(Client *client) {
    // iServer *serv = client -> server;
    
    while(true) {
        MCPacket packet;
        if(!ReadPacket(client -> socket, &packet))
            break;

        boolean shouldBreak = false;
        switch(client -> state) {
            case HANDSHAKE:
                shouldBreak = sapi_handle_handshake(client, packet);
                break;
            
            case STATUS:
                shouldBreak = sapi_handle_status(client, packet);
                break;
            
            case LOGIN:
                shouldBreak = sapi_handle_login(client, packet);
                break;
            
            case PLAY:
                shouldBreak = sapi_handle_play(client, packet);
                break;

            default:
                shouldBreak = true;
                break;
        }

        FreePacket(&packet);
        if(shouldBreak) break;
    }

    printf("Disconnection from: %s:%u\n", client -> ip, client -> port);

    closesocket(client -> socket);
    HeapFree(GetProcessHeap(), 0, client -> ip);

    if(client -> last != NULL) client -> last -> next = client -> next;
    if(client -> next != NULL) client -> next -> last = client -> last;

    return 0;
}

DWORD sapi_socket_thread(Server server) {
    iServer *serv = (iServer *) server;
    while(IsRunning(server)) {
        Client *client = serv -> firstClient;
        while(client -> next != NULL)
            client = client -> next;

        client -> server = serv;
        client -> next = HeapAlloc(
            GetProcessHeap(), 0,
            sizeof(Client)
        );

        client -> next -> last = client;
        client -> next -> next = NULL;

        client -> state = HANDSHAKE;
        client -> ip = HeapAlloc(
            GetProcessHeap(), 0,
            sizeof(char) * 16
        );

        SOCKADDR_IN addr;
        int addrSize = sizeof(SOCKADDR);
        client -> socket = accept(
            serv -> socket,
            (SOCKADDR *) &addr,
            &addrSize
        );

        char *ip = inet_ntoa(addr.sin_addr);
        int len = strlen(ip);
        memcpy(client -> ip, ip, len);
        client -> ip[len] = 0;

        client -> port = ntohs(addr.sin_port);
        printf("Connection from: %s:%u\n", client -> ip, client -> port);

        HANDLE clientThread = CreateThread(
            NULL,
            0,
            (void *) sapi_listener_thread,
            client,
            0,
            NULL
        );

        CloseHandle(clientThread);
    }

    return 0;
}

DWORD sapi_tick_thread(Server server) {
    return 0;
}

void StartServer(Server server, ushort port) {
    iServer *serv = (iServer *) server;
    
    serv -> socket = socket(AF_INET, SOCK_STREAM, 0);
    serv -> port = port;

    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serv -> port);

    bind(serv -> socket, (SOCKADDR *) &addr, sizeof(addr));
    listen(serv -> socket, serv -> properties.maxPlayers);

    serv -> tickThread = CreateThread(
        NULL,
        0,
        (void *) sapi_tick_thread,
        serv,
        0,
        NULL
    );

    serv -> socketThread = CreateThread(
        NULL,
        0,
        (void *) sapi_socket_thread,
        serv,
        0,
        NULL
    );
}

void RunCommand(Server server, const char *cmd) {

}

boolean IsRunning(Server server) {
    return true;
}

void StopServer(Server server) {

}