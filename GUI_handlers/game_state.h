/**
 * @file game_state.h
 * @brief Central header for game state and theme management.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Screen dimensions for responsive design
#define SCREEN_WIDTH GetScreenWidth()
#define SCREEN_HEIGHT GetScreenHeight()

// ============================================================================
// THEME SYSTEM
// ============================================================================

/**
 * @enum ThemeID
 * @brief Enumeration of available color themes for the game UI.
 */
typedef enum {
  THEME_DEFAULT,
  THEME_DARK,
  THEME_FOREST,
  THEME_SPACE,
  THEME_AQUATIC,
  THEME_COUNT /**< Total number of themes */
} ThemeID;

/**
 * @struct Theme
 * @brief Complete color palette for a single UI theme.
 */
typedef struct {
  const char *name; // Name of the theme
  Color primary;    // Primary color
  Color secondary;  // Secondary color
  Color accent;     // Accent color
  Color warning;    // Warning color
  Color background; // Background color
  Color dark;       // Dark shade
  Color light;      // Light shade
} Theme;

// ============================================================================
// GAME STATE ENUMERATIONS
// ============================================================================

/**
 * @enum GameScreen
 * @brief Represents the current screen/menu state of the application.
 */
typedef enum {
  SCREEN_START,             // Main menu
  SCREEN_MODE_SELECT,       // Choose 1P or 2P
  SCREEN_THEME_SELECT,      // Choose a theme
  SCREEN_DIFFICULTY_SELECT, // Choose AI difficulty (1P only)
  SCREEN_SYMBOL_SELECT_1P,  // Choose X or O (1P)
  SCREEN_SYMBOL_SELECT_2P,  // Choose who goes first (2P)
  SCREEN_INSTRUCTIONS,      // How to play
  SCREEN_HISTORY,           // View game history
  SCREEN_GAME,              // The actual game
  SCREEN_GAME_OVER          // Game ended
} GameScreen;

/**
 * @enum GameMode
 * @brief Enum for game modes.
 */
typedef enum {
  MODE_ONE_PLAYER, // Human vs AI
  MODE_TWO_PLAYER  // Human vs Human
} GameMode;

// Difficulty levels for AI
#define DIFF_EASY 3
#define DIFF_MEDIUM 2
#define DIFF_HARD 1

// ============================================================================
// Move History Structure (for UNDO feature)
// ============================================================================

/**
 * @brief Stores the state of the game board for each move.
 */
typedef struct {
  char board[3][3];
  char currentPlayer;
  bool aiTurn;
} MoveSnapshot;

typedef struct {
  Sound click;
  Sound win;
  Sound lose;
  Sound draw;
} GameAudio;

// ============================================================================
// MAIN GAME STATE
// ============================================================================

/**
 * @struct GameState
 * @brief Central game state structure containing all game information.
 */
typedef struct {
  // Navigation & Mode
  GameScreen screen;
  GameMode mode;
  int difficulty;

  // Board & Player State
  char board[3][3];   // 3x3 array representing the board
  char humanSymbol;   // Human player's symbol ('X' or 'O')
  char aiSymbol;      // AI player's symbol ('X' or 'O')
  char currentPlayer; // Whose turn it is ('X' or 'O')

  // Game Status
  bool gameOver;
  char winner;

  // AI Control
  bool aiTurn;
  float aiMoveTimer; // Delay for AI moves

  // Statistics
  int player1Wins;
  int player2Wins;
  int draws;

  // UI & Settings
  bool isFullscreen;
  ThemeID currentTheme; // Current theme ID

  // Save/Load Feedback
  const char *saveMessage; // Pointer to message string
  float saveMessageTimer;  // Countdown timer

  GameAudio sfx; // Sound effects

  // --- Undo System ---
  /**
   * @brief Dynamic array to store game state history.
   */
  MoveSnapshot *moveHistory;

  /**
   * @brief The number of moves currently stored in moveHistory.
   */
  int moveCount;

  /**
   * @brief Tracks allocated capacity of moveHistory array.
   */
  int moveCapacity;

  // --- History System ---
  /**
   * @brief Dynamic 2D array to hold game history lines read from file.
   */
  char **gameHistory;

  /**
   * @brief Number of lines currently in gameHistory.
   */
  int historyLineCount;

  /**
   * @brief Tracks allocated capacity of gameHistory array.
   */
  int historyCapacity;

  // Tracks the scroll position
  int historyScrollOffset;
} GameState;

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

/**
 * Global UI color variables.
 */
extern Color colorPrimary;
extern Color colorSecondary;
extern Color colorAccent;
extern Color colorWarning;
extern Color colorBackground;
extern Color colorDark;
extern Color colorLight;

/**
 * Global game state instance.
 */
extern GameState game;

// Array of all available themes
extern Theme allThemes[THEME_COUNT];

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// Core game functions
void InitGame(void);

void CleanupGame(void);

/**
 * @brief Reset the game board to an empty state.
 */
void ResetBoard(void);

/**
 * @brief Check if there is a winner on the current board.
 * @return true if a winner is found.
 */
bool CheckWinner(const char (*board)[3]);

/**
 * @brief Check if the board is completely filled.
 * @return true if board is full.
 */
bool IsBoardFull(const char (*board)[3]);

/**
 * @brief Execute the AI's move based on current difficulty.
 */
void MakeAIMove(void);

// Theme management
void ChangeTheme(ThemeID newTheme);

// Save/Load functions
void SaveGame(void);

/**
 * @brief Load a GameState from a binary file.
 * @return true if loading was successful.
 */
bool LoadGame(void);

/**
 * @brief Deletes the save file "save.dat" if it exists.
 */
void DeleteSaveGame(void);

/**
 * @brief Appends the result of the completed game to "game_history.txt".
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
