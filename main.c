/**
 * @file main.c
 * @brief Main entry point for the Tic-Tac-Toe game.
 *
 * Handles window initialization, the main game loop, and screen navigation.
 */

#include "GUI_handlers/game_state.h"
#include "GUI_handlers/screens.h"
#include "raylib.h"


/**
 * @brief Main function.
 *
 * Initializes the window and game state, then enters the main loop.
 * @return 0 on successful exit.
 */
int main(void) {
  // Initialize window with resizable flag
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1280, 720, "Tic-Tac-Toe");
  InitAudioDevice();
  SetTargetFPS(60);

  // Initialize game state (board, settings, theme)
  // This sets up the global 'game' object.
  InitGame();

  // Main Game Loop
  while (!WindowShouldClose()) {
    // Update Logic
    switch (game.screen) {
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

    // Draw Phase
    BeginDrawing();

    ClearBackground(colorBackground);

    // Render the active screen
    switch (game.screen) {
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
      // Render game screen first, then overlay game over panel
      DrawGameScreen();
      DrawGameOverScreen();
      break;
    }

    EndDrawing();
  }
  // OPTIMIZATION: Clean up dynamic memory before exit
  CleanupGame();
  // Cleanup: close window and exit
  CloseAudioDevice();
  CloseWindow();
  return 0;
}