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

gcc ^
src/main.c src/math.c ^
-s -Wall -Werror -Os -o bin/main.exe ^
-lkernel32 -lws2_32 -luser32 ^
-fno-asynchronous-unwind-tables -fno-ident -falign-functions=1 -falign-jumps=1 -ffreestanding ^
-Wl,-e,CRTBootstrapper ^
-nostdlib -nostartfiles -nodefaultlibs

:: Compress if flag is set
if %compress%==1 mpress -s bin/main.exe >nul