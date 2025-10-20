// Header for Minimax core functions
#ifndef MINIMAX_CORE_H
#define MINIMAX_CORE_H

#include <stdbool.h>

struct Move { int row, col; };

// External player/opponent chars used by minimax module
extern char player;
extern char opponent;

// Difficulty levels for imperfect play
enum Difficulty { DIFF_EASY = 0, DIFF_MEDIUM = 1, DIFF_HARD = 2 };

// Functions exported from minimax_core.c
bool isMovesLeft(char board[3][3]);
int evaluate(char b[3][3]);
int minimax(char board[3][3], int depth, bool isMax);
// Alpha-beta variant (internal use)
int minimaxAB(char board[3][3], int depth, bool isMax, int alpha, int beta);
struct Move findBestMove(char board[3][3]);
// Returns a move according to difficulty (may be imperfect)
struct Move findBestMoveWithDifficulty(char board[3][3], int difficulty);

#endif // MINIMAX_CORE_H
