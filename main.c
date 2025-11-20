/**
 * @file main.c
 * @brief Main entry point for Tic-Tac-Toe game
 * * This file contains the main game loop that handles window initialization,
 * screen navigation, input processing, and rendering. Uses Raylib for
 * graphics and window management.
 */

#include "raylib.h"
#include "GUI_handlers/game_state.h"
#include "GUI_handlers/screens.h"

/**
 * @brief Main entry point
 * * Initializes the game window, sets up the game state, and runs the
 * main game loop until the window is closed.
 * * @return 0 on successful exit
 */
int main(void)
{
    // Initialize window with resizable flag
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Tic-Tac-Toe");
    SetTargetFPS(60);
    
    // Initialize game state (board, settings, theme)
    // This sets up the global 'game' object.
    InitGame();
    
    // ========================================================================
    // MAIN GAME LOOP
    // =VERSION 2================================================================
    /**
     * This is the main game loop. It runs once per frame (target 60 FPS).
     * It follows a clean "Handle Input -> Update -> Draw" pattern.
     * * 1. **Input Processing (Handle):** The first switch statement.
     * It checks the current `game.screen` and calls the *one*
     * corresponding `Handle...` function to process input for that screen.
     * * 2. **Rendering (Draw):** The `BeginDrawing()` block.
     * The second switch statement checks `game.screen` *again*
     * and calls the corresponding `Draw...` function to render the UI.
     * * This separation of "Handling" (logic) and "Drawing" (rendering)
     * is a fundamental and clean pattern for game development.
     */
    while (!WindowShouldClose())
    {
        // --------------------------------------------------------------------
        // Input Processing / Logic Phase
        // --------------------------------------------------------------------
        // This switch statement acts as the "Controller" part of our
        // state machine. It dispatches to the correct logic function.
        switch (game.screen)
        {
            case SCREEN_START:
                HandleStartScreen();
                break;
            case SCREEN_MODE_SELECT:
                HandleModeSelectScreen();
                break;
            case SCREEN_THEME_SELECT:
                HandleThemeSelectScreen();
                break;    
            case SCREEN_DIFFICULTY_SELECT:
                HandleDifficultySelectScreen();
                break;
            case SCREEN_SYMBOL_SELECT_1P:
                HandleSymbolSelectScreen(false); // false = not 2P, so show "vs AI"
                break;
            case SCREEN_SYMBOL_SELECT_2P:
                HandleSymbolSelectScreen(true); // true = 2P, show "Player 1"
                break;
            case SCREEN_INSTRUCTIONS:
                HandleInstructionsScreen();
                break;
            case SCREEN_HISTORY:
                HandleHistoryScreen();
                break;
            case SCREEN_GAME:
                HandleGameScreen();
                break;
            case SCREEN_GAME_OVER:
                HandleGameOverScreen();
                break;
        }
        
        // ====================================================================
        // RENDERING PHASE
        // ====================================================================
        BeginDrawing();
        
        // Clear the background with the *current* theme's background color
        ClearBackground(colorBackground);
        
        // This switch statement acts as the "View" part of our state
        // machine. It dispatches to the correct drawing function.
        switch (game.screen)
        {
            case SCREEN_START:
                DrawStartScreen();
                break;
            case SCREEN_MODE_SELECT:
                DrawModeSelectScreen();
                break;
            case SCREEN_THEME_SELECT:
                DrawThemeSelectScreen();
                break;
            case SCREEN_DIFFICULTY_SELECT:
                DrawDifficultySelectScreen();
                break;
            case SCREEN_SYMBOL_SELECT_1P:
                DrawSymbolSelectScreen(false);
                break;
            case SCREEN_SYMBOL_SELECT_2P:
                DrawSymbolSelectScreen(true);
                break;
            case SCREEN_INSTRUCTIONS:
                DrawInstructionsScreen();
                break;
            case SCREEN_HISTORY:
                DrawHistoryScreen();
                break;
            case SCREEN_GAME:
                DrawGameScreen();
                break;
            case SCREEN_GAME_OVER:
                /**
                 * This is a special case. We want to show the final board
                 * *and* the game over panel on top. So, we call
                 * `DrawGameScreen` first, then `DrawGameOverScreen`
                 * to render the panel over it.
                 */
                DrawGameScreen();
                DrawGameOverScreen();
                break;
        }
        
        EndDrawing();
    }
     // OPTIMIZATION: Clean up dynamic memory before exit
    CleanupGame();
    // Cleanup: close window and exit
    CloseWindow();
    return 0;
}