@echo off
title Tic-Tac-Toe Builder

echo ========================================
echo   TIC-TAC-TOE: Build and Run
echo ========================================
echo.

REM Build the project
call build.bat

REM Check if build succeeded
if %errorlevel% neq 0 (
    echo.
    echo Build failed! Fix errors and try again.
    pause
    exit /b 1
)

echo.
echo ========================================
echo Press any key to run the game...
echo ========================================
pause >nul

REM Run the game
call run.bat
