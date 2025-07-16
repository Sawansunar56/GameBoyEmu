@echo off
set SRC=..\src\*.cpp
set LINKER_FLAGS=/out:main.exe /SUBSYSTEM:CONSOLE
set BUILD_TYPE=Debug
set LIBS=shell32.lib opengl32.lib gdi32.lib user32.lib SDL2.lib SDL2_ttf.lib

set SDL2_INC=/I..\vendor\SDL2\include
set SDL2_TTF_INC=/I..\vendor\SDL2_ttf\include
set SDL2_LIB=/LIBPATH:"..\vendor\SDL2\lib\x64"
set SDL2_TTF_LIB=/LIBPATH:"..\vendor\SDL2_ttf\lib\x64"
set CHECK_DIR=/LIBPATH:"..\vendor\check"

set INCLUDES=%SDL2_TTF_INC% %SDL2_INC%
set LIBRARIES=%SDL2_LIB%  %SDL2_TTF_LIB% %CHECK_DIR%

if not "%1" == "" (
    if "%1" == "release" (
        set BUILD_TYPE=Release
    ) else if not "%1" == "debug" (
        echo Invalid build type: %1
        exit /b 1
    )
)

if "%BUILD_TYPE%" == "Debug" (
    set CFLAGS=/MDd /Zi /Od /DDEBUG /std:c++20 /EHsc
    set LFLAGS=/DEBUG
) else if "%BUILD_TYPE%" == "Release" (
    set CFLAGS=/MD /Zi /O2 /DNDEBUG
    set LFLAGS=
)

if not exist .\build mkdir .\build
pushd .\build

ctime -begin c_time.ctm
cl %CFLAGS% %INCLUDES% /DSDL_MAIN_HANDLED /c %SRC% 
link %LINKER_FLAGS% %LFLAGS% *.obj %LIBRARIES% %LIBS%
ctime -end c_time.ctm

popd

REM set BUILD_TYPE=Debug
REM if /I "%1"=="release" set BUILD_TYPE=Release
REM
REM pushd .\build
REM
REM ctime -begin c_time.ctm
REM echo Building in %BUILD_TYPE% mode...
REM msbuild gba_emu.vcxproj /p:Configuration=%BUILD_TYPE% /m
REM
REM ctime -end c_time.ctm
REM
REM popd
