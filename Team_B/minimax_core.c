#include "minimax_core.h"
#include <stdio.h>

// Globals for player and opponent used by the minimax logic
char player = 'x';
char opponent = 'o';

// This function returns true if there are moves
// remaining on the board. It returns false if
// there are no moves left to play.
bool isMovesLeft(char board[3][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '_')
                return true;
    return false;
}

// This is the evaluation function
int evaluate(char b[3][3])
{
    // Checking for Rows for X or O victory.
    for (int row = 0; row < 3; row++) {
        if (b[row][0] == b[row][1]
            && b[row][1] == b[row][2]) {
            if (b[row][0] == player)
                return +10;
            else if (b[row][0] == opponent)
                return -10;
        }
    }

    // Checking for Columns for X or O victory.
    for (int col = 0; col < 3; col++) {
        if (b[0][col] == b[1][col]
            && b[1][col] == b[2][col]) {
            if (b[0][col] == player)
                return +10;

            else if (b[0][col] == opponent)
                return -10;
        }
    }

    // Checking for Diagonals for X or O victory.
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) {
        if (b[0][0] == player)
            return +10;
        else if (b[0][0] == opponent)
            return -10;
    }

    if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) {
        if (b[0][2] == player)
            return +10;
        else if (b[0][2] == opponent)
            return -10;
    }

    // Else if none of them have won then return 0
    return 0;
}

// Minimax with alpha-beta pruning for better memory/time efficiency
int minimaxAB(char board[3][3], int depth, bool isMax, int alpha, int beta)
{
    int score = evaluate(board);

    // Prefer faster wins and slower losses by factoring in depth
    if (score == 10)
        return score - depth; // sooner win -> larger value
    if (score == -10)
        return score + depth; // sooner loss -> smaller (worse) value
    if (!isMovesLeft(board))
        return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '_') {
                    board[i][j] = player;
                    int val = minimaxAB(board, depth + 1, false, alpha, beta);
                    board[i][j] = '_';
                    if (val > best) best = val;
                    if (best > alpha) alpha = best;
                    if (beta <= alpha)
                        return best; // beta cut-off
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
                    int val = minimaxAB(board, depth + 1, true, alpha, beta);
                    board[i][j] = '_';
                    if (val < best) best = val;
                    if (best < beta) beta = best;
                    if (beta <= alpha)
                        return best; // alpha cut-off
                }
            }
        }
        return best;
    }
}

// Backwards-compatible minimax (calls alpha-beta variant)
int minimax(char board[3][3], int depth, bool isMax)
{
    return minimaxAB(board, depth, isMax, -10000, 10000);
}

// Helper: collect all empty cells and their minimax scores
#include <stdlib.h>

struct ScoredMove { struct Move m; int score; };

// This will return the best possible move for the player (perfect play)
struct Move findBestMove(char board[3][3])
{
    int bestVal = -1000;
    struct Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '_') {
                board[i][j] = player;
                int moveVal = minimaxAB(board, 0, false, -10000, 10000);
                board[i][j] = '_';

                if (moveVal > bestVal) {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                } else if (moveVal == bestVal) {
                    // tie-breaker: prefer center, then corners
                    int curIsCenter = (bestMove.row == 1 && bestMove.col == 1);
                    int candIsCenter = (i == 1 && j == 1);
                    int curIsCorner = ( (bestMove.row==0&&bestMove.col==0) || (bestMove.row==0&&bestMove.col==2) || (bestMove.row==2&&bestMove.col==0) || (bestMove.row==2&&bestMove.col==2) );
                    int candIsCorner = ( (i==0&&j==0) || (i==0&&j==2) || (i==2&&j==0) || (i==2&&j==2) );
                    if (!curIsCenter && candIsCenter) {
                        bestMove.row = i; bestMove.col = j;
                    } else if (!curIsCenter && !candIsCenter && !curIsCorner && candIsCorner) {
                        bestMove.row = i; bestMove.col = j;
                    }
                }
            }
        }
    }
    return bestMove;
}

// Imperfect move selection depending on difficulty:
// EASY   - choose a random legal move
// MEDIUM - choose the best move most of the time, but occasionally pick a suboptimal move
// HARD   - behave like perfect minimax
struct Move findBestMoveWithDifficulty(char board[3][3], int difficulty)
{
    // Count empty cells and store scored moves
    struct ScoredMove candidates[9];
    int candCount = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == '_') {
                board[i][j] = player;
                int sc = minimaxAB(board, 0, false, -10000, 10000);
                board[i][j] = '_';
                candidates[candCount].m.row = i;
                candidates[candCount].m.col = j;
                candidates[candCount].score = sc;
                candCount++;
            }
        }
    }

    if (candCount == 0) {
        struct Move none = { -1, -1 };
        return none;
    }

    // EASY: pick uniformly at random
    if (difficulty == 0) {
        int r = rand() % candCount;
        return candidates[r].m;
    }

    // MEDIUM: mostly pick the best, but with some probability pick a suboptimal
    int bestIdx = 0;
    for (int i = 1; i < candCount; ++i)
        if (candidates[i].score > candidates[bestIdx].score)
            bestIdx = i;

    if (difficulty == 1) {
        int roll = rand() % 100;
        if (roll < 60) { // 60% choose best (was 75%)
            return candidates[bestIdx].m;
        } else {
            // choose a random non-best move
            int r = rand() % (candCount - 1);
            if (r >= bestIdx) r++;
            return candidates[r].m;
        }
    }

    // HARD: choose best (perfect)
    return candidates[bestIdx].m;
}

/* duplicate older findBestMove removed - new implementations above
   provide findBestMove() and findBestMoveWithDifficulty(). */
