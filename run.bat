@echo off
echo ============================================
echo   Running Tic-Tac-Toe Game
echo ============================================

if not exist "bin\TicTacToe.exe" (
    echo ERROR: Executable not found!
    echo Please run build.bat first.
    pause
    exit /b 1
)

REM Run the game from bin directory
cd bin
TicTacToe.exe
cd ..

pause
