@echo off
call %CD:~0,2%\Projects\Vex\shell.bat

set CXX= arm-none-eabi-g++
set OBJCOPY= arm-none-eabi-objcopy

set OUTPUT_BIN=bin\output.bin
set OUTPUT_ELF=bin\output.elf

set ROOT=%CD%
set COMPILE_OPTIONS= -mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=softfp -Os -g -D_POSIX_THREADS -D_UNIX98_THREAD_MUTEX_ATTRIBUTES -ffunction-sections -fdata-sections -funwind-tables -Wno-psabi --std=gnu++17 -MT $$@ -MMD -MP -nostdlib 
set LIBRARIES= -Wl,firmware\libc.a -Wl,firmware\libm.a -Wl,firmware\libpros.a -Wl,firmware\okapilib.a
set LINK_OPTIONS= -Wl,-T".\firmware\v5.ld" -Wl,--gc-sections -Wl,--start-group %LIBRARIES% -Wl,-lgcc -Wl,-lstdc++ -Wl,-lsupc++ -Wl,--end-group
set INCLUDE_PATHS= -I".\include"
set LIBRARY_PATHS= -L".\firmware"


REM Do the compiling:

%CXX% %INCLUDE_PATHS% .\src\main.cpp -o%OUTPUT_ELF% %COMPILE_OPTIONS% %LINK_OPTIONS% 
%OBJCOPY% %OUTPUT_ELF% -O binary -R .hot_init %OUTPUT_BIN%

REM DO the uploading:

REM call upload.bat