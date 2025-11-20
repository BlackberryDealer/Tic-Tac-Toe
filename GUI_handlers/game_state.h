/**
 * @file game_state.h
 * @brief Central header for game state and theme management
 * 
* This file defines:
 * - The GameState struct that holds all game data
 * - Theme system enums and structs
 * - Global color variables
 * - Function declarations for game logic
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    const char* name;      // Name of the theme
    Color primary;         // Primary color
    Color secondary;       // Secondary color
    Color accent;          // Accent color
    Color warning;         // Warning color
    Color background;      // Background color
    Color dark;            // Dark shade
    Color light;           // Light shade
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
    SCREEN_START,              // Main menu
    SCREEN_MODE_SELECT,        // Choose 1P or 2P
    SCREEN_THEME_SELECT,       // Choose a theme
    SCREEN_DIFFICULTY_SELECT,  // Choose AI difficulty (1P only)
    SCREEN_SYMBOL_SELECT_1P,   // Choose X or O (1P)
    SCREEN_SYMBOL_SELECT_2P,   // Choose who goes first (2P)
    SCREEN_INSTRUCTIONS,       // How to play
    SCREEN_HISTORY,            // View game history
    SCREEN_GAME,               // The actual game
    SCREEN_GAME_OVER           // Game ended
} GameScreen;

/**
 * @enum GameMode
 * @brief Enum for game modes.
 */
typedef enum {
    MODE_ONE_PLAYER,  // Human vs AI
    MODE_TWO_PLAYER   // Human vs Human
} GameMode;

// Difficulty levels for AI
#define DIFF_EASY 3
#define DIFF_MEDIUM 2
#define DIFF_HARD 1

// ============================================================================
// OPTIMIZATION: Move History Structure (for UNDO feature)
// ============================================================================

/**
 * @brief Stores the state of the game board for each move.
 * Used by the Undo button to restore previous game states.
 */
typedef struct {
    char board[3][3];
    char currentPlayer;
    bool aiTurn;
} MoveSnapshot;

// ============================================================================
// MAIN GAME STATE
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
    // OPTIMIZATION: Using pointer instead of char array to save memory
    const char *saveMessage;    // Pointer to message string ("Game Saved!" or "Error")
    float saveMessageTimer;     // Countdown timer to hide the message


    // --- OPTIMIZED Undo System ---
    /**
     * @brief OPTIMIZATION: Dynamic array to store game state history.
     * Allocated using malloc/realloc only when moves are made.
     */
    MoveSnapshot *moveHistory;  // Dynamic array (replaces fixed moveHistory[9])
    
    /**
     * @brief The number of moves currently stored in moveHistory.
     * This acts as a stack pointer for our history.
     */
    int moveCount;

    /**
     * @brief OPTIMIZATION: Tracks allocated capacity of moveHistory array.
     * Allows us to grow the array dynamically with realloc.
     */
    int moveCapacity;


    // --- OPTIMIZED History System ---
    /**
     * @brief OPTIMIZATION: Dynamic 2D array to hold game history lines read from file.
     * Each element is a dynamically allocated string.
     */
    char **gameHistory;         // Dynamic array of strings (replaces gameHistory[100][128])
    
    /**
     * @brief Number of lines currently in gameHistory.
     */
    int historyLineCount;
    
    /**
     * @brief OPTIMIZATION: Tracks allocated capacity of gameHistory array.
     * Allows unlimited history without wasting memory.
     */
    int historyCapacity;
    
    // Tracks the scroll position (index of the top visible line)
    int historyScrollOffset; 
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

// Array of all available themes
extern Theme allThemes[THEME_COUNT];

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// Core game functions
void InitGame(void);

// OPTIMIZATION: Added to free dynamic memory
void CleanupGame(void);  // OPTIMIZATION: Added to free dynamic memory

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

// Theme management
void ChangeTheme(ThemeID newTheme);

// Save/Load functions
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
