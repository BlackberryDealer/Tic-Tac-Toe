/**
 * @file game_state.h
 * @brief Game state management module for Tic-Tac-Toe
 * * This module defines the core game state structure, including board state,
 * player information, game settings, and theme management. It provides
 * functions to initialize, reset, and manage the game state throughout
 * the application lifecycle.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include <stdbool.h>
#include <stdio.h> // For file I/O (Save/Load)

// Screen dimensions for responsive design
// These macros ensure that UI calculations always use the *current* window size,
// which is essential for a resizable window.
#define SCREEN_WIDTH GetScreenWidth()
#define SCREEN_HEIGHT GetScreenHeight()

// ============================================================================
// THEME SYSTEM
// ============================================================================

/**
 * @enum ThemeID
 * @brief Enumeration of available color themes for the game UI
 * * Makes it easy to request a theme by name. Each theme provides a complete
 * color palette for the game interface. Using an enum here allows us to
 * use the theme ID as a direct index into the `allThemes` array in game_state.c.
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
 * * Holds all the colors needed to render the game interface with a
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
 * * This is the heart of the application's state machine. The main loop in
 * `main.c` uses the value of `game.screen` to decide which `Handle...`
 * and `Draw...` functions to call each frame.
 */
typedef enum {
    SCREEN_START,                /**< Main menu screen */
    SCREEN_MODE_SELECT,          /**< Player mode selection screen */
    SCREEN_THEME_SELECT,         /**< Theme selection screen */
    SCREEN_DIFFICULTY_SELECT,    /**< Difficulty selection screen */
    SCREEN_SYMBOL_SELECT_1P,     /**< Symbol selection screen for 1-player mode */
    SCREEN_SYMBOL_SELECT_2P,     /**< Symbol selection screen for 2-player mode */
    SCREEN_INSTRUCTIONS,         /**< Instructions screen */
    SCREEN_HISTORY,              /**< Game history screen */
    SCREEN_GAME,                 /**< Main game screen */
    SCREEN_GAME_OVER             /**< Game over screen with results */
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
// We use #define for difficulty instead of an enum. This is a stylistic
// choice, but it allows these values to be used easily in contexts
// that might not be expecting a specific enum type.

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
 * * This structure holds the *entire* state of the application in one
 * globally accessible object (`game`). This "singleton" approach is common
 * for smaller projects as it simplifies state management, avoiding the
 * need to pass a `GameState*` pointer to every function.
 * * This includes:
 * - Navigation state (screen, mode)
 * - Game data (board, players, winner)
 * - Statistics (wins, draws)
 * - UI settings (theme, fullscreen)
 * - Temporary UI state (save messages)
 */
typedef struct {
    // Navigation & Mode
    GameScreen screen;
    GameMode mode;
    int difficulty;
    
    // Board & Player State
    char board[3][3];       // a nested 3x3 array representing the Tic-Tac-Toe board - stores 'X', 'O', or ' ' (empty)
    char humanSymbol;       // variable to store the human player's selected symbol ('X' or 'O')
    char aiSymbol;          // variable to store the AI player's symbol ('X' or 'O')
    char currentPlayer;     // variable to track whose turn it is ('X' or 'O')
    
    // Game Status
    bool gameOver;
    char winner;
    
    // AI Control
    bool aiTurn;
    float aiMoveTimer;  // Used to add a small delay to AI moves for game feel
    
    // Statistics
    int player1Wins;
    int player2Wins;
    int draws;
    
    // UI & Settings
    bool isFullscreen;
    ThemeID currentTheme; // Current theme ID
    
    // Save/Load Feedback
    char saveMessage[64];    // Buffer to hold "Game Saved!" or "Error"
    float saveMessageTimer; // Countdown timer to hide the message

    // --- Undo System ---
    /**
     * @brief Stores the state of the game for each move.
     * Used by the Undo button.
     */
    struct {
        char board[3][3];
        char currentPlayer;
        bool aiTurn;
    } moveHistory[9]; // Max 9 moves (0-8)
    
    /**
     * @brief The number of moves currently stored in moveHistory.
     * This acts as a stack pointer for our history.
     */
    int moveCount;

    // --- History System ---
    /**
     * @brief A buffer to hold game history lines read from file.
     * We'll read up to 20 lines.
     */
    char gameHistory[20][128];
    int historyLineCount;
    
} GameState;

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

/** * Global UI color variables (extern declarations)
 * * These variables are updated by `ChangeTheme`. All drawing functions in
 * `screens.c` use these variables (e.g., `colorPrimary`) instead of
 * hard-coded colors. This is *why* the theme system works: drawing functions
 * are decoupled from the specific theme data.
 */
extern Color colorPrimary;
extern Color colorSecondary;
extern Color colorAccent;
extern Color colorWarning;
extern Color colorBackground;
extern Color colorDark;
extern Color colorLight;

/** * Global game state instance
 * * This is the single, authoritative instance of the GameState struct
 * that is used by the entire application.
 */
extern GameState game;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

/**
 * @brief Initialize the game state with default values
 * * Sets up the game with default settings, initializes the board,
 * and applies the default theme. Should be called once at program start.
 */
void InitGame(void);

/**
 * @brief Reset the game board to an empty state
 * * Clears all cells on the board and resets game status flags.
 * Crucially, this *does not* reset statistics (wins, draws) or settings,
 * making it perfect for the "Play Again" button.
 */
void ResetBoard(void);

/**
 * @brief Check if there is a winner on the current board
 * * Examines all rows, columns, and diagonals to determine if
 * any player has achieved three in a row. Updates the winner
 * field and statistics if a winner is found.
 * * @return true if a winner is found, false otherwise
 */
bool CheckWinner(void);

/**
 * @brief Check if the board is completely filled
 * * Determines if all 9 cells are occupied. If the board is full
 * and no winner exists, increments the draw counter.
 * * @return true if board is full, false otherwise
 */
bool IsBoardFull(void);

/**
 * @brief Execute the AI's move based on current difficulty
 * * Acts as a dispatcher. It selects the appropriate AI algorithm 
 * (from Team_B's minimax.h) based on `game.difficulty` and makes a move.
 * This keeps the game state logic separate from the AI implementation.
 */
void MakeAIMove(void);

/**
 * @brief Change the active UI theme
 * * This function updates `game.currentTheme` and copies all colors
 * from the chosen `allThemes` entry into the global `Color` variables
 * (colorPrimary, etc.). The new colors will be used in the next draw cycle.
 * * @param newTheme The ID of the theme to apply.
 */
void ChangeTheme(ThemeID newTheme);

/**
 * @brief Save the current GameState to a binary file
 * * Serializes the *entire* `game` struct and writes it to "save.dat".
 * This is a simple but non-portable save method. If the `GameState`
 * struct ever changes, old save files will become incompatible.
 */
void SaveGame(void);

/**
 * @brief Load a GameState from a binary file
 * * Reads from "save.dat" and overwrites the current `game` state.
 * Includes important safety checks to prevent crashes from
 * corrupted or empty save files.
 * * @return true if loading was successful, false otherwise
 */
bool LoadGame(void);

/**
 * @brief Appends the result of the completed game to "game_history.txt"
 */
void AppendGameToHistory(void);

/**
 * @brief Loads the game history from "game_history.txt" into the state.
 */
void LoadGameHistory(void);

/**
 * @brief Clears all game history by truncating the history file.
 */
void ClearGameHistory(void);

#endif // GAME_STATE_H