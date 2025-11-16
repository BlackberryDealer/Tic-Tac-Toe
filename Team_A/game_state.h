#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include <stdbool.h>

// Screen dimensions for responsive design
#define SCREEN_WIDTH GetScreenWidth()
#define SCREEN_HEIGHT GetScreenHeight()

// --- NEW THEMES ---
// Makes it easy to ask for a theme by name
typedef enum {
    THEME_DEFAULT,
    THEME_DARK,
    THEME_FOREST,
    THEME_SPACE,
    THEME_AQUATIC,
    THEME_COUNT // This will automatically be 5
} ThemeID;

// Holds all the colors for a single theme
typedef struct {
    const char* name;
    Color primary;
    Color secondary;
    Color accent;
    Color warning;
    Color background;
    Color dark;
    Color light;
} Theme;

// Game states
typedef enum {
    SCREEN_START,
    SCREEN_MODE_SELECT,
    SCREEN_THEME_SELECT,
    SCREEN_DIFFICULTY_SELECT,
    SCREEN_SYMBOL_SELECT_1P,
    SCREEN_SYMBOL_SELECT_2P,
    SCREEN_INSTRUCTIONS,
    SCREEN_GAME,
    SCREEN_GAME_OVER
} GameScreen;

// Game modes
typedef enum {
    MODE_ONE_PLAYER,
    MODE_TWO_PLAYER
} GameMode;

// Difficulty levels (mapped to Team B's AI)
#define DIFF_EASY 3      // Model-based (makes mistakes)
#define DIFF_MEDIUM 2    // Imperfect
#define DIFF_HARD 1      // Perfect

// Global game state
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
} GameState;

// UI Colors (extern declarations)
extern Color colorPrimary;
extern Color colorSecondary;
extern Color colorAccent;
extern Color colorWarning;
extern Color colorBackground;
extern Color colorDark;
extern Color colorLight;

// Global game state instance
extern GameState game;

// Game state functions
void InitGame(void);
void ResetBoard(void);
bool CheckWinner(void);
bool IsBoardFull(void);
void MakeAIMove(void);
void ChangeTheme(ThemeID newTheme); //new for themes

#endif // GAME_STATE_H
