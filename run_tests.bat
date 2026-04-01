@echo off
REM ============================================================================
REM   TIC-TAC-TOE TEST SUITE - Build and Run
REM ============================================================================

where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo ================================================================
    echo   ERROR: GCC not found in PATH!
    echo   Please install MinGW/MSYS2 and add gcc to your system PATH.
    echo ================================================================
    pause
    exit /b 1
)

echo ================================================================
echo   TIC-TAC-TOE TEST SUITE
echo ================================================================
echo.

echo [1/2] Compiling test suite...
echo ----------------------------------------------------------------

gcc -o Tests\test_suite.exe ^
    Tests\test_suite.c ^
    Tests\raylib_stub.c ^
    GUI_handlers\game_state.c ^
    Game_algorithms\Minimax.c ^
    Game_algorithms\minimax_utils.c ^
    Game_algorithms\model_minimax.c ^
    -Ilib\raylib ^
    -I. ^
    -lm

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Test compilation FAILED!
    echo Check error messages above.
    echo.
    pause
    exit /b 1
)

echo [SUCCESS] test_suite.exe compiled.
echo.

echo [2/2] Running tests...
echo ----------------------------------------------------------------
echo.

Tests\test_suite.exe

echo.
echo ================================================================
echo   Test run complete. Exit code: %errorlevel%
echo ================================================================
pause
