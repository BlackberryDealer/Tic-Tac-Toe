@echo off
echo ============================================
echo   Building Tic-Tac-Toe Game
echo ============================================

REM Create output directory
if not exist "bin" mkdir bin

REM Compile all source files with static linking
gcc -o bin\TicTacToe.exe ^
    main.c ^
    Team_A\game_state.c ^
    Team_A\screens.c ^
    Team_A\ui.c ^
    Team_B\1_player.c ^
    Team_B\2_player.c ^
    Team_B\gameboard.c ^
    Team_B\PERFECT_minimax.c ^
    Team_B\IMPERFECT_minimax.c ^
    Team_B\model_minimax.c ^
    -Ilib\raylib ^
    -Llib\raylib ^
    -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -static -lm

if %errorlevel% equ 0 (
    echo.
    echo ============================================
    echo   SUCCESS! Game compiled.
    echo   Location: bin\TicTacToe.exe
    echo ============================================
    
    REM Copy JSON file if it exists
    if exist "logistic_regression_params.json" (
        copy /Y "logistic_regression_params.json" "bin\" >nul
        echo   Copied: logistic_regression_params.json
    )
    
    REM Copy any other required files
    if exist "Team_B\logistic_regression_params.json" (
        copy /Y "Team_B\logistic_regression_params.json" "bin\" >nul
        echo   Copied: logistic_regression_params.json
    )
    
) else (
    echo.
    echo ============================================
    echo   COMPILATION FAILED!
    echo   Check error messages above.
    echo ============================================
    pause
    exit /b 1
)

echo.
pause
