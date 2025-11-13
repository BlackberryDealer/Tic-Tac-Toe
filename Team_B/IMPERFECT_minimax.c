#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "minimax.h"
#include "model_minimax.h" 

// Local (file-only) globals for player symbols
// These are marked static so they don't clash with other files
static char player = 'X';
static char opponent = 'O';

// Precomputed winning line masks (rows, columns, diagonals)
// Each mask is a 9-bit integer where bits 0..8 represent cells
// Example: (1<<0)|(1<<1)|(1<<2) = top row (cells 0,1,2)
static const int WIN_MASKS[8] = {
    (1 << 0) | (1 << 1) | (1 << 2), // row 0
    (1 << 3) | (1 << 4) | (1 << 5), // row 1
    (1 << 6) | (1 << 7) | (1 << 8), // row 2
    (1 << 0) | (1 << 3) | (1 << 6), // col 0
    (1 << 1) | (1 << 4) | (1 << 7), // col 1
    (1 << 2) | (1 << 5) | (1 << 8), // col 2
    (1 << 0) | (1 << 4) | (1 << 8), // main diagonal
    (1 << 2) | (1 << 4) | (1 << 6)  // anti-diagonal
};

// Convert a 3x3 board into two bitmasks: one for X, one for O
// Each cell index (0..8) corresponds to a bit position
static inline void boardToMasks(const char board[3][3], int *maskX, int *maskO) {
    *maskX = 0;
    *maskO = 0;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) {
            int idx = r * 3 + c;   // cell index
            char ch = board[r][c];
            if (ch == 'X') *maskX |= (1 << idx); // set bit for X
            else if (ch == 'O') *maskO |= (1 << idx); // set bit for O
        }
}

// Check if a given mask contains any winning line
static inline bool isWinnerMask(int mask) {
    for (int i = 0; i < 8; ++i)
        if ((mask & WIN_MASKS[i]) == WIN_MASKS[i])
            return true;
    return false;
}

// Imperfect minimax search:
// - Depth-limited (stops after depth 2, so it doesn't see far ahead)
// - Randomized move order (shuffles moves before exploring)
// This makes the AI weaker and capable of mistakes.
static int minimax_masks(int playerMask, int oppMask, int depth, bool isMax) {
    // Terminal checks: win/loss/draw
    if (isWinnerMask(playerMask)) return 10 - depth;
    if (isWinnerMask(oppMask))    return -10 + depth;
    if ((playerMask | oppMask) == 0x1FF || depth >= 2) return 0; // full board or cutoff

    // Initialize best score depending on role
    int best = isMax ? -1000 : 1000;
    int moves[9], count = 0;

    // Collect all available moves
    for (int i = 0; i < 9; ++i)
        if (!((playerMask | oppMask) & (1 << i)))
            moves[count++] = i;

    // Shuffle move order to introduce randomness
    for (int i = count - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = moves[i]; moves[i] = moves[j]; moves[j] = tmp;
    }

    // Explore each move recursively
    for (int i = 0; i < count; ++i) {
        int pos = moves[i];
        int bit = (1 << pos);

        if (isMax) {
            // Maximizer (AI's turn)
            int val = minimax_masks(playerMask | bit, oppMask, depth + 1, false);
            if (val > best) best = val;
        } else {
            // Minimizer (opponent's turn)
            int val = minimax_masks(playerMask, oppMask | bit, depth + 1, true);
            if (val < best) best = val;
        }
    }

    return best;
}

// Public function: choose the AI's move
// Uses imperfect minimax (depth-limited + randomization)
// Returns a Move struct with row/col coordinates
struct Move findBestMoveImperfect(char board[3][3]) {
    int maskP = 0, maskO = 0;
    boardToMasks(board, &maskP, &maskO);
    int occupied = maskP | maskO;

    int bestVal = -1000;
    struct Move bestMove = { -1, -1 };

    // Seed randomness for shuffling and random move selection
    srand(time(NULL));

    // Try random positions until a good move is found
    for (int i = 0; i < 9; ++i) {
        int pos = rand() % 9;   // pick random cell
        int bit = (1 << pos);
        if (occupied & bit) continue; // skip if already taken

        // Evaluate move using imperfect minimax
        int moveVal = minimax_masks(maskP | bit, maskO, 1, false);
        if (moveVal > bestVal) {
            bestVal = moveVal;
            bestMove.row = pos / 3;
            bestMove.col = pos % 3;
        }
    }

    return bestMove;
}

struct Move findBestMoveModel(char board[3][3]) {
    int bestRow = -1, bestCol = -1;
    double bestScore = -1e9;
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            if (board[i][j] == '_') {
                board[i][j] = 'X'; // Simulate AI move
                double score = evaluateBoardLogistic(board);
                board[i][j] = '_'; // Undo move
                if (score > bestScore) {
                    bestScore = score;
                    bestRow = i; bestCol = j;
                }
            }
        }
    }
    struct Move mv = {bestRow, bestCol};
    return mv;
}

