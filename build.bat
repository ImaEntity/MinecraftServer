@echo off

set compress=1
set compileyyjson=1
set compiledriver=1

:: Handle parameters and set correct flags
:paramHandler
if not "%1"=="" (
  if "%1"=="--nocompress" set compress=0
  if "%1"=="--nojson" set compileyyjson=0
  if "%1"=="--nodriver" set compiledriver=0

  shift
  goto :paramHandler
)

:: Setup
if not exist bin mkdir bin >nul

if %compileyyjson%==1 gcc src/json/yyjson.c -s -Os -shared -o bin/yyjson.dll

gcc ^
src/server/packet.c src/utils/io.c src/utils/math.c src/server/types.c src/server/server.c ^
-s -Wall -Werror -Os -shared -o bin/server.dll ^
-lkernel32 -lws2_32 -luser32 -ladvapi32 -L./bin -lyyjson ^
-fno-asynchronous-unwind-tables -fno-ident -falign-functions=1 -falign-jumps=1 -ffreestanding ^
-Wl,-e,CRTBootstrapper ^
-nostdlib -nostartfiles -nodefaultlibs

if %compiledriver%==1 (
  gcc src/utils/io.c src/utils/math.c src/main.c ^
  -s -Wall -Werror -Os -o bin/main.exe ^
  -lkernel32 -lws2_32 -luser32 -L./bin -lserver ^
  -fno-asynchronous-unwind-tables -fno-ident -falign-functions=1 -falign-jumps=1 -ffreestanding ^
  -Wl,-e,CRTBootstrapper ^
  -nostdlib -nostartfiles -nodefaultlibs
)

:: Compress if flag is set
if %compress%==1 mpress -s bin/server.dll >nul
if %compress%==1 mpress -s bin/main.exe >nul