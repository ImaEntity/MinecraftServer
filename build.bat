@echo off

gcc ^
src/server/api.c ^
src/server/packet.c ^
src/server/string.c ^
src/server/varint.c ^
src/server/uuid.c ^
-s -Wall -Werror -O3 -lws2_32 -shared -L./bin -lyyjson -o bin/server.dll

gcc src/main.c -s -Wall -Werror -O3 -lws2_32 -L./bin -lserver -o bin/main.exe