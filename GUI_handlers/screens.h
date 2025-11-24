/**
 * @file screens.h
 * @brief Screen management module declarations.
 */

#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>

// ============================================================================
// SCREEN DRAWING FUNCTIONS (The "View")
// ============================================================================

/**
 * @brief Render the start/main menu screen.
 */
void DrawStartScreen(void);

/**
 * @brief Render the game mode selection screen (1 player vs 2 player).
 */
void DrawModeSelectScreen(void);

/**
 * @brief Render the theme selection screen.
 */
void DrawThemeSelectScreen(void);

/**
 * @brief Render the difficulty selection screen (Easy/Medium/Hard).
 */
void DrawDifficultySelectScreen(void);

/**
 * @brief Render the symbol selection screen.
 * @param isPlayer1 True for Player 1 selection.
 */
void DrawSymbolSelectScreen(bool isPlayer1);

/**
 * @brief Render the game instructions screen.
 */
void DrawInstructionsScreen(void);

/**
 * @brief Render the main game screen with board and UI.
 */
void DrawGameScreen(void);

/**
 * @brief Render the game over overlay with results.
 */
void DrawGameOverScreen(void);

/**
 * @brief Render the game history screen.
 */
void DrawHistoryScreen(void);

// ============================================================================
// SCREEN INPUT HANDLING FUNCTIONS (The "Controller")
// ============================================================================

/**
 * @brief Process input for the start screen.
 */
void HandleStartScreen(void);

/**
 * @brief Process input for the mode selection screen.
 */
void HandleModeSelectScreen(void);

/**
 * @brief Process input for the theme selection screen.
 */
void HandleThemeSelectScreen(void);

/**
 * @brief Process input for the difficulty selection screen.
 */
void HandleDifficultySelectScreen(void);

/**
 * @brief Process input for the symbol selection screen.
 * @param isPlayer1 True for Player 1 selection.
 */
void HandleSymbolSelectScreen(bool isPlayer1);

/**
 * @brief Process input for the instructions screen.
 */
void HandleInstructionsScreen(void);

/**
 * @brief Process input for the game screen (board clicks, AI moves, buttons).
 */
void HandleGameScreen(void);

/**
 * @brief Process input for the game over screen.
 */
void HandleGameOverScreen(void);

/**
 * @brief Process input for the game history screen.
 */
void HandleHistoryScreen(void);

#endif // SCREENS_H