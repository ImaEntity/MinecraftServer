#include <winsock2.h>

#include "../json/yyjson.h"
#include "../utils/io.h"
#include "server.h"
#include "packet.h"

void CRTBootstrapper() {}

Server CreateServer(ServerProperties properties, unsigned short port) {
    Server server = {
        .socket = socket(AF_INET, SOCK_STREAM, 0),
        .properties = properties,
        .port = port
    };

    return server;
}

typedef struct {
    Server server;
    ServerState state;
    SOCKET socket;
} Client;

boolean handle_handshake(Client *client, MCPacket packet) {
    switch(packet.packetID.value) {
        case C2S_HANDSHAKE_HANDSHAKE: {
            int offset = 0;
            
            MCVarInt protocolVersion = ReadVarIntBuffer(packet.data + offset);
            offset += protocolVersion.size;

            String serverAddress = ReadStringBuffer(packet.data + offset);
            offset += serverAddress.length.size + serverAddress.length.value;

            // Don't care abt server port
            offset += sizeof(unsigned short);

            ServerState nextState = ReadVarIntBuffer(packet.data + offset).value;
            offset++;

            client -> state = nextState;

            FreeString(serverAddress);
            break;
        }

        default:
            return false;
    }

    return true;
}

boolean handle_status(Client *client, MCPacket packet) {
    switch(packet.packetID.value) {
        case C2S_STATUS_STATUS_REQUEST: {
            yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
            yyjson_mut_val *root = yyjson_mut_obj(doc);
            yyjson_mut_val *version = yyjson_mut_obj(doc);
            yyjson_mut_val *players = yyjson_mut_obj(doc);
            yyjson_mut_val *description = yyjson_mut_obj(doc);
            
            yyjson_mut_obj_add_int(doc, players, "max", INT_MAX);
            yyjson_mut_obj_add_int(doc, players, "online", -1);

            yyjson_mut_obj_add_str(doc, version, "name", "1.20.1");
            yyjson_mut_obj_add_int(doc, version, "protocol", 763); // 1.20 - 1.20.1

            yyjson_mut_obj_add_bool(doc, root, "enforcesSecureChat", false);

            yyjson_mut_obj_add_str(doc, description, "text", "Hello, world!");

            yyjson_mut_obj_add_val(doc, root, "players", players);
            yyjson_mut_obj_add_val(doc, root, "version", version);
            yyjson_mut_obj_add_val(doc, root, "description", description);
            yyjson_mut_obj_add_str(doc, root, "favicon", "data:image/png;base64,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");
            yyjson_mut_doc_set_root(doc, root);

            char *json = yyjson_mut_write(doc, 0, NULL);
            String data = CreateString(json);

            unsigned byte *buffer = LocalAlloc(LMEM_FIXED, 8192);
            WriteStringBuffer(data, buffer);

            MCPacket res = CreatePacket(
                S2C_STATUS_STATUS_RESPONSE,
                buffer,
                data.length.size + data.length.value
            );

            WritePacket(client -> socket, res);

            yyjson_mut_doc_free(doc);
            LocalFree(buffer);
            FreeString(data);

            // TODO: FIX THE MEMORY LEAK HERE (json isn't freed)
            // You don't have access to the standard `free` tho, gl
            
            break;
        }

        case C2S_STATUS_PING_REQUEST: {
            MCPacket res = CreatePacket(
                S2C_STATUS_PING_RESPONSE,
                packet.data, // echo data
                packet.length.value - packet.packetID.size
            );

            WritePacket(client -> socket, res);
            // Fall through to connection close
        }

        default:
            return false;
    }

    return true;
}

boolean handle_login(Client *client, MCPacket packet) {
    switch(packet.packetID.value) {
        case C2S_LOGIN_LOGIN_START: {
            int offset = 0;

            String username = ReadStringBuffer(packet.data + offset);
            offset += username.length.size + username.length.value;

            boolean hasPlayerUUID = (boolean) packet.data[offset];
            offset++;

            char nameBuf[32] = "OfflinePlayer:";
            nameBuf[username.length.size + username.length.value] = 0;
            for(int i = 0; i < username.length.value; i++)
                nameBuf[i + 14] = username.bytes[i];

            UUID uuid = DeriveUUID(nameBuf, username.length.value + 14);
            if(hasPlayerUUID) {
                uuid = ReadUUIDBuffer(packet.data + offset);
                offset += sizeof(uuid);
            }

            int length = 0;
            unsigned byte *buffer = LocalAlloc(LMEM_FIXED, 64);

            WriteUUIDBuffer(uuid, buffer + length);
            length += sizeof(uuid);

            WriteStringBuffer(username, buffer + length);
            length += username.length.size + username.length.value;

            // Idk what the properties are, so just make an empty array
            MCVarInt propertyCount = CreateVarInt(0);
            for(int i = 0; i < propertyCount.size; i++)
                buffer[i + length] = propertyCount.bytes[i];
            
            length += propertyCount.size;

            MCPacket res = CreatePacket(
                S2C_LOGIN_LOGIN_SUCCESS,
                buffer,
                length
            );

            WritePacket(client -> socket, res);
            client -> state = SS_PLAY;
            FreeString(username);
            LocalFree(buffer);

            // EVERYTHING BELOW THIS LINE IS TEMPORARRY!!!
            yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
            yyjson_mut_val *root = yyjson_mut_obj(doc);

            yyjson_mut_obj_add_str(doc, root, "text", "Indev! Come back later.");
            yyjson_mut_obj_add_str(doc, root, "color", "#FF4B64");

            yyjson_mut_doc_set_root(doc, root);

            char *json = yyjson_mut_write(doc, 0, NULL);
            String data = CreateString(json);

            unsigned byte tmpBuf[256];
            WriteStringBuffer(data, tmpBuf);

            MCPacket tmp = CreatePacket(
                S2C_PLAY_DISCONNECT,
                tmpBuf,
                data.length.size + data.length.value
            );

            WritePacket(client -> socket, tmp);
            yyjson_mut_doc_free(doc);
            FreeString(data);

            return false;
            // EVERYTHING ABOVE THIS LINE IS TEMPORARRY!!!

            break;
        }

        default:
            return false;
    }

    return true;
}

boolean handle_play(Client *client, MCPacket packet) {
    switch(packet.packetID.value) {
        default:
            return false;
    }

    return true;
}

DWORD handle_client(LPVOID lParam) {
    Client *client = (Client *) lParam;

    const char *states[] = {
        "HANDSHAKE",
        "STATUS",
        "LOGIN",
        "PLAY"
    };

    PrintStrings("Client connected!\n", NULL);

    boolean running = true;
    while(running) {
        MCPacket packet = ReadPacket(client -> socket);

        const char hex[] = "0123456789ABCDEF";
        char buf[3];
       
        buf[0] = hex[packet.packetID.value >> 4];
        buf[1] = hex[packet.packetID.value & 0xF];
        buf[2] = '\0';

        PrintStrings("State: %s, Packet ID: %s\n", (const char *[]) {
            states[client -> state],
            buf
        });

        Sleep(100); // artificial ping, for debugging
        switch(client -> state) {
            case SS_HANDSHAKE:
                running = handle_handshake(client, packet);
                break;
            
            case SS_STATUS:
                running = handle_status(client, packet);
                break;
            
            case SS_LOGIN:
                running = handle_login(client, packet);
                break;
            
            case SS_PLAY:
                running = handle_play(client, packet);
                break;

            default:
                running = false;
                break;
        }

        FreePacket(packet);
    }

    PrintStrings("Client disconnected!\n\n", NULL);
    closesocket(client -> socket);
    return 0;
}

void StartServer(Server server) {
    SOCKADDR_IN addr = {
        .sin_family = AF_INET,
        .sin_port = htons(server.port),
        .sin_addr.s_addr = INADDR_ANY
    };

    bind(server.socket, (SOCKADDR *) &addr, sizeof(addr));
    listen(server.socket, 1);

    PrintStrings("Server started!\n", NULL);

    while(true) {
        SOCKADDR_IN clientAddr;
        int addrSize = sizeof(clientAddr);

        SOCKET client = accept(server.socket, (SOCKADDR *) &clientAddr, &addrSize);
        if(client == INVALID_SOCKET) continue;

        Client clientData = {
            .server = server,
            .state = SS_HANDSHAKE,
            .socket = client
        };

        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) handle_client, (LPVOID) &clientData, 0, NULL);
    }
}