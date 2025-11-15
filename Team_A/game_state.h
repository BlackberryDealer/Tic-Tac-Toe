#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include <stdbool.h>

// Screen dimensions for responsive design
#define SCREEN_WIDTH GetScreenWidth()
#define SCREEN_HEIGHT GetScreenHeight()

// Game states
typedef enum {
    SCREEN_START,
    SCREEN_MODE_SELECT,
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
} GameState;

// UI Colors (extern declarations)
extern Color colorPrimary;
extern Color colorSecondary;
extern Color colorAccent;
extern Color colorWarning;
extern Color colorBackground;
extern Color colorDark;
extern Color colorLight;

// Global game state (extern declaration)
extern GameState game;

// Function declarations
void InitGame(void);
void ResetBoard(void);
bool CheckWinner(void);
bool IsBoardFull(void);
void MakeAIMove(void);

#endif // GAME_STATE_H