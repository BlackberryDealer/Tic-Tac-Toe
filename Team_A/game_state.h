/**
 * @file game_state.h
 * @brief Game state management module for Tic-Tac-Toe
 * 
 * This module defines the core game state structure, including board state,
 * player information, game settings, and theme management. It provides
 * functions to initialize, reset, and manage the game state throughout
 * the application lifecycle.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include <stdbool.h>
#include <stdio.h> // --- NEW for file saving --

// Screen dimensions for responsive design
#define SCREEN_WIDTH GetScreenWidth()
#define SCREEN_HEIGHT GetScreenHeight()

// ============================================================================
// THEME SYSTEM
// ============================================================================

/**
 * @enum ThemeID
 * @brief Enumeration of available color themes for the game UI
 * 
 * Makes it easy to request a theme by name. Each theme provides a complete
 * color palette for the game interface.
 */
typedef enum {
    THEME_DEFAULT,
    THEME_DARK,
    THEME_FOREST,
    THEME_SPACE,
    THEME_AQUATIC,
    THEME_COUNT  /**< Total number of themes (automatically calculated) */
} ThemeID;

/**
 * @struct Theme
 * @brief Complete color palette for a single UI theme
 * 
 * Holds all the colors needed to render the game interface with a
 * consistent visual style. Each theme provides primary, secondary,
 * accent, warning, background, dark, and light colors.
 */
typedef struct {
    const char* name;
    Color primary;
    Color secondary;
    Color accent;
    Color warning;
    Color background;
    Color dark;
    Color light;        /**< Light color for text/UI elements */
} Theme;

// ============================================================================
// GAME STATE ENUMERATIONS
// ============================================================================

/**
 * @enum GameScreen
 * @brief Represents the current screen/menu state of the application
 * 
 * Used to manage navigation between different screens (start menu,
 * game screen, settings, etc.)
 */
typedef enum {
    SCREEN_START,
    SCREEN_MODE_SELECT,
    SCREEN_THEME_SELECT,
    SCREEN_DIFFICULTY_SELECT,
    SCREEN_SYMBOL_SELECT_1P,
    SCREEN_SYMBOL_SELECT_2P,
    SCREEN_INSTRUCTIONS,
    SCREEN_GAME,
    SCREEN_GAME_OVER    /**< Game over screen with results */
} GameScreen;

/**
 * @enum GameMode
 * @brief Defines the game play mode (single or two player)
 */
typedef enum {
    MODE_ONE_PLAYER,
    MODE_TWO_PLAYER     /**< Two human players mode */
} GameMode;

// ============================================================================
// DIFFICULTY CONSTANTS
// ============================================================================
// Define difficulty levels for AI behavior. Chose define instead of enum for easier
// integration with existing code structure.
// Enum is more suited for distinct states, while these constants represent levels of difficulty.

/**
 * @def DIFF_EASY
 * @brief Easy difficulty - uses model-based AI (makes mistakes)
 */
#define DIFF_EASY 3

/**
 * @def DIFF_MEDIUM
 * @brief Medium difficulty - uses imperfect minimax AI
 */
#define DIFF_MEDIUM 2

/**
 * @def DIFF_HARD
 * @brief Hard difficulty - uses perfect minimax AI (unbeatable)
 */
#define DIFF_HARD 1

// ============================================================================
// GAME STATE STRUCTURE
// ============================================================================

/**
 * @struct GameState
 * @brief Central game state structure containing all game information
 * 
 * This structure maintains the complete state of the game:
 * - Current screen and mode - (screen/mode)
 * - Board state (3x3 grid) - (board)
 * - Player symbols and turn information - (human/AISymbols, currentPlayer)
 * - Game status (winner, game over flag) - (gameOver, winner)
 * - Statistics (wins, draws) - (player1Wins, player2Wins, draws)
 * - UI settings (theme, fullscreen) - (isFullscreen, currentTheme)
 *  * These are called
 */
typedef struct {
    GameScreen screen;
    GameMode mode;
    int difficulty;
    char board[3][3];
    char humanSymbol;
    char aiSymbol;
    char currentPlayer;
    bool gameOver;
    char winner;
    bool aiTurn;
    float aiMoveTimer;
    int player1Wins;
    int player2Wins;
    int draws;
    bool isFullscreen;
    ThemeID currentTheme; // Current theme ID
    // --- ADD THESE ---
    char saveMessage[64];
    float saveMessageTimer;
    // --- END ADD ---
} GameState;

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

/** Global UI color variables (extern declarations) */
extern Color colorPrimary;      /**< Primary UI color */
extern Color colorSecondary;    /**< Secondary UI color */
extern Color colorAccent;       /**< Accent/highlight color */
extern Color colorWarning;      /**< Warning/alert color */
extern Color colorBackground;   /**< Background color */
extern Color colorDark;         /**< Dark text/grid color */
extern Color colorLight;        /**< Light text color */

/** Global game state instance */
extern GameState game;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

/**
 * @brief Initialize the game state with default values
 * 
 * Sets up the game with default settings, initializes the board,
 * and applies the default theme. Should be called once at program start.
 */
void InitGame(void);

/**
 * @brief Reset the game board to an empty state
 * 
 * Clears all cells on the board and resets game status flags.
 * Does not reset statistics (wins, draws) or settings.
 */
void ResetBoard(void);

/**
 * @brief Check if there is a winner on the current board
 * 
 * Examines all rows, columns, and diagonals to determine if
 * any player has achieved three in a row. Updates the winner
 * field and statistics if a winner is found.
 * 
 * @return true if a winner is found, false otherwise
 */
bool CheckWinner(void);

/**
 * @brief Check if the board is completely filled
 * 
 * Determines if all 9 cells are occupied. If the board is full
 * and no winner exists, increments the draw counter.
 * 
 * @return true if board is full, false otherwise
 */
bool IsBoardFull(void);

/**
 * @brief Execute the AI's move based on current difficulty
 * 
 * Selects the appropriate AI algorithm (perfect, imperfect, or model)
 * based on the current difficulty setting and makes a move on the board.
 * Updates the current player after the move.
 */
void MakeAIMove(void);
void ChangeTheme(ThemeID newTheme); //new for themes

// --- New for file saving ---
void SaveGame(void);
bool LoadGame(void);
// --- END ADD ---

#endif // GAME_STATE_H
