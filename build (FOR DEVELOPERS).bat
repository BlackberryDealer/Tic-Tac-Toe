@echo off
echo ============================================
echo   Building Tic-Tac-Toe Game
echo ============================================

REM Create output directory
if not exist "bin" mkdir bin

REM Compile all source files with static linking
gcc -o bin\TicTacToe.exe ^
    main.c ^
    GUI_handlers\game_state.c ^
    GUI_handlers\screens.c ^
    GUI_handlers\ui.c ^
    Game_algorithms\PERFECT_minimax.c ^
    Game_algorithms\IMPERFECT_minimax.c ^
    Game_algorithms\model_minimax.c ^
    Game_algorithms\minimax_utils.c ^
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
    if exist "Game_algorithms\logistic_regression_params.json" (
        copy /Y "Game_algorithms\logistic_regression_params.json" "bin\" >nul
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
