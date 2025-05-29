@echo off

set compress=1

:: Handle parameters and set correct flags
:paramHandler
if not "%1"=="" (
  if "%1"=="--nocompress" set compress=0

  shift
  goto :paramHandler
)

:: Setup
if not exist bin mkdir bin >nul

@REM gcc src/json/yyjson.c -s -Os -shared -o bin/yyjson.dll

gcc ^
src/server/packet.c src/utils/io.c src/utils/math.c src/server/types.c src/server/server.c ^
-s -Wall -Werror -Os -shared -o bin/server.dll ^
-lkernel32 -lws2_32 -luser32 -ladvapi32 -L./bin -lyyjson ^
-fno-asynchronous-unwind-tables -fno-ident -falign-functions=1 -falign-jumps=1 -ffreestanding ^
-Wl,-e,CRTBootstrapper ^
-nostdlib -nostartfiles -nodefaultlibs

@REM gcc src/utils/io.c src/utils/math.c src/main.c ^
@REM -s -Wall -Werror -Os -o bin/main.exe ^
@REM -lkernel32 -lws2_32 -luser32 -L./bin -lserver ^
@REM -fno-asynchronous-unwind-tables -fno-ident -falign-functions=1 -falign-jumps=1 -ffreestanding ^
@REM -Wl,-e,CRTBootstrapper ^
@REM -nostdlib -nostartfiles -nodefaultlibs

:: Compress if flag is set
if %compress%==1 mpress -s bin/server.dll >nul
if %compress%==1 mpress -s bin/main.exe >nul