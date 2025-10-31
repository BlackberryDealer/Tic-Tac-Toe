/**
 * Modular minimax implementation for Tic-Tac-Toe.
 * Exposes evaluate, minimax, findBestMove and defines
 * the player/opponent characters used by the algorithm.
 */

#include <stdio.h>
#include <stdbool.h>
#include "minimax.h"

char player = 'x';
char opponent = 'o';

bool isMovesLeft(char board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '_')
                return true;
    return false;
}

int evaluate(char b[3][3]) {
    // rows
    for (int row = 0; row < 3; row++) {
        if (b[row][0] == b[row][1] && b[row][1] == b[row][2]) {
            if (b[row][0] == player) return +10;
            else if (b[row][0] == opponent) return -10;
        }
    }
    // columns
    for (int col = 0; col < 3; col++) {
        if (b[0][col] == b[1][col] && b[1][col] == b[2][col]) {
            if (b[0][col] == player) return +10;
            else if (b[0][col] == opponent) return -10;
        }
    }
    // diagonals
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) {
        if (b[0][0] == player) return +10;
        else if (b[0][0] == opponent) return -10;
    }
    if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) {
        if (b[0][2] == player) return +10;
        else if (b[0][2] == opponent) return -10;
    }
    return 0;
}

int minimax(char board[3][3], int depth, int isMax) {
    int score = evaluate(board);
    if (score == 10) return score;
    if (score == -10) return score;
    if (!isMovesLeft(board)) return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '_') {
                    board[i][j] = player;
                    int val = minimax(board, depth + 1, 0);
                    if (val > best) best = val;
                    board[i][j] = '_';
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '_') {
                    board[i][j] = opponent;
                    int val = minimax(board, depth + 1, 1);
                    if (val < best) best = val;
                    board[i][j] = '_';
                }
            }
        }
        return best;
    }
}

struct Move findBestMove(char board[3][3]) {
    int bestVal = -1000;
    struct Move bestMove = { -1, -1 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '_') {
                board[i][j] = player;
                int moveVal = minimax(board, 0, 0);
                board[i][j] = '_';

                if (moveVal > bestVal) {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}
