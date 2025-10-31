#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define COMPUTER 1
#define HUMAN 2
#define SIDE 3
#define COMPUTERMOVE 'O'
#define HUMANMOVE 'X'

// display / helper functions
void showBoard(char board[][SIDE]);
void showInstructions(void);
void initialise(char board[][SIDE], int moves[]);
int rowCrossed(char board[][SIDE]);
int columnCrossed(char board[][SIDE]);
int diagonalCrossed(char board[][SIDE]);
int gameOver(char board[][SIDE]);

#endif
