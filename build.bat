@echo off
REM Build script for TTS_V2 Project - Windows

echo.
echo ====================================
echo   TTS_V2 Build Script
echo ====================================
echo.

REM Check if build directory exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

REM Check if ARM GCC is available
where arm-none-eabi-gcc >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: ARM GCC toolchain not found!
    echo Please install ARM GCC and add it to PATH
    pause
    exit /b 1
)

echo Building TTS_V2 project...
echo.

REM Clean previous build
if exist "build\*.o" del /q "build\*.o"
if exist "build\*.elf" del /q "build\*.elf"
if exist "build\*.hex" del /q "build\*.hex"
if exist "build\*.bin" del /q "build\*.bin"

REM Build using Make
make clean
make all

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ====================================
    echo   Build completed successfully!
    echo ====================================
    echo.
    echo Output files:
    if exist "build\tts_v2.elf" echo   - build\tts_v2.elf
    if exist "build\tts_v2.hex" echo   - build\tts_v2.hex
    if exist "build\tts_v2.bin" echo   - build\tts_v2.bin
) else (
    echo.
    echo ====================================
    echo   Build failed!
    echo ====================================
    echo.
    echo Check the error messages above.
)

echo.
pause
