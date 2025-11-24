@echo off
echo ================================================================
echo   TIC-TAC-TOE AI BENCHMARK SUITE
echo ================================================================
echo.

REM Create output directory for binaries
if not exist "bin" mkdir bin

REM ================================================================
REM   COMPILE BENCHMARK.C
REM ================================================================
echo [1/2] Compiling benchmark.c...
echo ----------------------------------------------------------------

gcc -o "bin\benchmark.exe" ^
    "Benchmark Files\benchmark.c" ^
    "Benchmark Files\benchmark_algorithms.c" ^
    "Game_algorithms\Minimax.c" ^
    "Game_algorithms\minimax_utils.c" ^
    "Game_algorithms\model_minimax.c" ^
    -I"Game_algorithms" ^
    -I"Benchmark Files" ^
    -static -lm

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] benchmark.c compilation FAILED!
    echo Check error messages above.
    echo.
    pause
    exit /b 1
)

echo [SUCCESS] benchmark.exe created in bin\
echo.

REM ================================================================
REM   COMPILE SIMULATION.C
REM ================================================================
echo [2/2] Compiling simulation.c...
echo ----------------------------------------------------------------

gcc -o "bin\simulation.exe" ^
    "Benchmark Files\simulation.c" ^
    "GUI_handlers\game_state.c" ^
    "Game_algorithms\Minimax.c" ^
    "Game_algorithms\model_minimax.c" ^
    "Game_algorithms\minimax_utils.c" ^
    -I"lib\raylib" ^
    -L"lib\raylib" ^
    -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -static -lm

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] simulation.c compilation FAILED!
    echo Check error messages above.
    echo.
    pause
    exit /b 1
)

echo [SUCCESS] simulation.exe created in bin\
echo.


echo ================================================================
echo   COMPILATION COMPLETE!
echo ================================================================
echo.
echo Both benchmark programs have been compiled successfully.
echo.
echo Available executables:
echo   - bin\benchmark.exe   : Performance benchmarking
echo   - bin\simulation.exe  : AI difficulty simulation
echo.
echo ----------------------------------------------------------------
echo.

REM ================================================================
REM   RUN BENCHMARKS
REM ================================================================
:menu
echo What would you like to run?
echo.
echo   1. Run benchmark.exe (Performance benchmarking)
echo   2. Run simulation.exe (AI difficulty simulation)
echo   3. Run both (benchmark first, then simulation)
echo   4. Exit
echo.
set /p choice="Enter your choice (1-4): "

if "%choice%"=="1" goto run_benchmark
if "%choice%"=="2" goto run_simulation
if "%choice%"=="3" goto run_both
if "%choice%"=="4" goto end

echo Invalid choice. Please try again.
echo.
goto menu

:run_benchmark
echo.
echo ================================================================
echo   RUNNING BENCHMARK.EXE
echo ================================================================
echo.
cd bin
benchmark.exe
cd ..
echo.
echo ================================================================
echo   BENCHMARK COMPLETE
echo ================================================================
echo.
pause
goto menu

:run_simulation
echo.
echo ================================================================
echo   RUNNING SIMULATION.EXE
echo ================================================================
echo.
cd bin
simulation.exe
cd ..
echo.
echo ================================================================
echo   SIMULATION COMPLETE
echo ================================================================
echo.
pause
goto menu

:run_both
echo.
echo ================================================================
echo   RUNNING BENCHMARK.EXE
echo ================================================================
echo.
cd bin
benchmark.exe
echo.
echo.
echo ================================================================
echo   RUNNING SIMULATION.EXE
echo ================================================================
echo.
simulation.exe
cd ..
echo.
echo ================================================================
echo   ALL BENCHMARKS COMPLETE
echo ================================================================
echo.
pause
goto menu

:end
echo.
echo Exiting...
echo.
