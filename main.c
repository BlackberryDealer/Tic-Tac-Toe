#include "raylib.h"
#include "Team_A/game_state.h"
#include "Team_A/screens.h"

int main(void)
{
    // Initialize window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Tic-Tac-Toe");
    SetTargetFPS(60);
    
    // Initialize game state
    InitGame();
    
    // Main game loop
    while (!WindowShouldClose())
    {
        // Handle input based on current screen
        switch (game.screen)
        {
            case SCREEN_START:
                HandleStartScreen();
                break;
            case SCREEN_MODE_SELECT:
                HandleModeSelectScreen();
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
        
        // Draw
        BeginDrawing();
        ClearBackground(colorBackground);
        
        // Draw based on current screen
        switch (game.screen)
        {
            case SCREEN_START:
                DrawStartScreen();
                break;
            case SCREEN_MODE_SELECT:
                DrawModeSelectScreen();
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
    
    CloseWindow();
    return 0;
}
