@echo off
title Tic-Tac-Toe - Build and Run Tool

:menu
cls
echo ============================================
echo   TIC-TAC-TOE - Build and Run Tool
echo ============================================
echo.
echo Please select an option:
echo.
echo   1. Build only
echo   2. Run only
echo   3. Build and Run
echo   4. Exit
echo.
echo ============================================
set /p choice="Enter your choice (1-4): "

if "%choice%"=="1" goto build
if "%choice%"=="2" goto run
if "%choice%"=="3" goto build_and_run
if "%choice%"=="4" goto end
echo Invalid choice! Please try again.
timeout /t 2 >nul
goto menu

:build
cls
echo ============================================
echo   Building Tic-Tac-Toe Game
echo ============================================
echo.

REM Create output directory
if not exist "bin" mkdir bin

REM Compile all source files with static linking (Single line to avoid batch errors)
gcc -o bin\TicTacToe.exe main.c GUI_handlers\game_state.c GUI_handlers\screens.c GUI_handlers\ui.c Game_algorithms\Minimax.c Game_algorithms\model_minimax.c Game_algorithms\minimax_utils.c -Ilib\raylib -Llib\raylib -lraylib -lopengl32 -lgdi32 -lwinmm -static -lm

if %errorlevel% equ 0 (
    echo.
    echo ============================================
    echo   SUCCESS! Game compiled.
    echo   Location: bin\TicTacToe.exe
    echo ============================================
    
) else (
    echo.
    echo ============================================
    echo   COMPILATION FAILED!
    echo   Check error messages above.
    echo ============================================
    pause
    goto menu
)

echo.
pause
goto menu

:run
cls
echo ============================================
echo   Running Tic-Tac-Toe Game
echo ============================================
echo.

if not exist "bin\TicTacToe.exe" (
    echo ERROR: Executable not found!
    echo Please build the game first (Option 1).
    pause
    goto menu
)

REM Run the game from bin directory
cd bin
TicTacToe.exe
cd ..

pause
goto menu

:build_and_run
cls
echo ========================================
echo   TIC-TAC-TOE: Build and Run
echo ========================================
echo.

REM Build the project
echo ============================================
echo   Building Tic-Tac-Toe Game
echo ============================================
echo.

REM Create output directory
if not exist "bin" mkdir bin

REM Compile all source files with static linking (Single line to avoid batch errors)
gcc -o bin\TicTacToe.exe main.c GUI_handlers\game_state.c GUI_handlers\screens.c GUI_handlers\ui.c Game_algorithms\Minimax.c Game_algorithms\model_minimax.c Game_algorithms\minimax_utils.c -Ilib\raylib -Llib\raylib -lraylib -lopengl32 -lgdi32 -lwinmm -static -lm

if %errorlevel% equ 0 (
    echo.
    echo ============================================
    echo   SUCCESS! Game compiled.
    echo   Location: bin\TicTacToe.exe
    echo ============================================
    
) else (
    echo.
    echo ============================================
    echo   COMPILATION FAILED!
    echo   Check error messages above.
    echo ============================================
    pause
    goto menu
)

echo.
echo ========================================
echo   Press any key to run the game...
echo ========================================
pause >nul

REM Run the game
echo.
echo ============================================
echo   Running Tic-Tac-Toe Game
echo ============================================
echo.

cd bin
TicTacToe.exe
cd ..

pause
goto menu

:end
echo.
echo Goodbye!
timeout /t 1 >nul
exit /b 0
