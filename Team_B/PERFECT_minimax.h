#ifndef MINIMAX_H
#define MINIMAX_H

struct Move {
    int row, col;
};

extern char player;
extern char opponent;

int evaluate(char board[3][3]);
int minimax(char board[3][3], int depth, int isMax);
struct Move findBestMove(char board[3][3]);

#endif
