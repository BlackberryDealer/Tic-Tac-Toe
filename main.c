/**
 * @file main.c
 * @brief Main entry point for Tic-Tac-Toe game
 * 
 * This file contains the main game loop that handles window initialization,
 * screen navigation, input processing, and rendering. Uses Raylib for
 * graphics and window management.
 */

#include "raylib.h"
#include "Team_A/game_state.h"
#include "Team_A/screens.h"

/**
 * @brief Main entry point
 * 
 * Initializes the game window, sets up the game state, and runs the
 * main game loop until the window is closed.
 * 
 * @return 0 on successful exit
 */
int main(void)
{
    // Initialize window with resizable flag
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Tic-Tac-Toe");
    SetTargetFPS(60);
    
    // Initialize game state (board, settings, theme)
    InitGame();
    
    // ========================================================================
    // MAIN GAME LOOP
    // ========================================================================
    while (!WindowShouldClose())
    {
        // Process input based on current screen
        switch (game.screen)        // switch based on current screen user is on
        {
            case SCREEN_START:          // case [current screen user is on]
                HandleStartScreen();    // call corresponding handle function
                break;
            case SCREEN_MODE_SELECT:
                HandleModeSelectScreen();
                break;
            case SCREEN_THEME_SELECT:        //new case for theme select screen
                HandleThemeSelectScreen();
                break;    
            case SCREEN_DIFFICULTY_SELECT:
                HandleDifficultySelectScreen();
                break;
            case SCREEN_SYMBOL_SELECT_1P:
                HandleSymbolSelectScreen(false);
                break;
            case SCREEN_SYMBOL_SELECT_2P:
                HandleSymbolSelectScreen(true);
                break;
            case SCREEN_INSTRUCTIONS:
                HandleInstructionsScreen();
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
        // Same style as before, but now includes theme select screen and is for rendering
        BeginDrawing();
        ClearBackground(colorBackground);
        
        // Render appropriate screen based on current state
        switch (game.screen)
        {
            case SCREEN_START:
                DrawStartScreen();
                break;
            case SCREEN_MODE_SELECT:
                DrawModeSelectScreen();
                break;
            case SCREEN_THEME_SELECT:  //new case for theme select screen
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
            case SCREEN_GAME:
                DrawGameScreen();
                break;
            case SCREEN_GAME_OVER:
                DrawGameScreen();
                DrawGameOverScreen();
                break;
        }
        
        EndDrawing();
    }
    
    // Cleanup: close window and exit
    CloseWindow();
    return 0;
}
