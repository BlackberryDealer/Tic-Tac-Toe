#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define COMPUTER 1
#define HUMAN 2
#define SIDE 3
#define COMPUTERMOVE 'O'
#define HUMANMOVE 'X'

/* game status values */
typedef enum {
    GAME_ONGOING = 0,
    GAME_WIN = 1,
    GAME_DRAW = 2
} GameStatus;

// display / helper functions
void showBoard(char board[][SIDE]);
void showInstructions(void);
void initialise(char board[][SIDE], int moves[]);
int rowCrossed(char board[][SIDE]);
int columnCrossed(char board[][SIDE]);
int diagonalCrossed(char board[][SIDE]);
int gameOver(char board[][SIDE]);

/* precise status function */
GameStatus gameStatus(char board[][SIDE]);

#endif
