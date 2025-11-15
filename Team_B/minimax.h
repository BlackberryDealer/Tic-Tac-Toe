#ifndef MINIMAX_H
#define MINIMAX_H

struct Move {
    int row, col;
};

// Perfect AI
// aiSymbol: 'X' or 'O' - the symbol the AI is playing
struct Move findBestMovePerfect(char board[3][3], char aiSymbol);

// Imperfect AI
struct Move findBestMoveImperfect(char board[3][3]);

// Imperfect Model AI
struct Move findBestMoveModel(char board[3][3]);

#endif
