/**
 * minimax.c
 * Ultra-efficient, perfect Tic-Tac-Toe minimax using bitboards + alpha-beta pruning
 *
 * Internally uses two 9-bit masks: bits 0..8 correspond to cells:
 *  0 1 2
 *  3 4 5
 *  6 7 8
 *
 * EMPTY char is '_' as before.
 */

#include <stdio.h>
#include <stdbool.h>
#include "minimax.h"

/* External globals (preserved for your project) */
static char player = 'X';
static char opponent = 'O';

/* Precomputed winning bit masks */
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

/* Move order: center → corners → edges (optimizes alpha-beta pruning) */
static const int MOVE_ORDER[9] = {4, 0, 2, 6, 8, 1, 3, 5, 7};

/* Convert 3×3 board to bit masks */
static inline void boardToMasks(const char board[3][3], int *maskX, int *maskO) {
    *maskX = 0;
    *maskO = 0;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) {
            int idx = r * 3 + c;
            char ch = board[r][c];
            if (ch == 'X') *maskX |= (1 << idx);
            else if (ch == 'O') *maskO |= (1 << idx);
        }
}

/* Check if given bitmask represents a winning position */
static inline bool isWinnerMask(int mask) {
    for (int i = 0; i < 8; ++i)
        if ((mask & WIN_MASKS[i]) == WIN_MASKS[i])
            return true;
    return false;
}

/*
 * Internal minimax on bit masks with alpha-beta pruning
 * isMax = true → player's turn (maximizer)
 * Returns score in range roughly [−10, +10] adjusted by depth.
 */
static int minimax_masks(int playerMask, int oppMask, int depth,
                         int alpha, int beta, bool isMax)
{
    /* Terminal checks first */
    if (isWinnerMask(playerMask)) return 10 - depth;
    if (isWinnerMask(oppMask))    return -10 + depth;
    if ((playerMask | oppMask) == 0x1FF || depth == 9) return 0; // draw or full board

    int best = isMax ? -1000 : 1000;

    for (int mi = 0; mi < 9; ++mi) {
        int pos = MOVE_ORDER[mi];
        int bit = (1 << pos);
        if ((playerMask | oppMask) & bit) continue; // occupied

        if (isMax) {
            int newPlayer = playerMask | bit;
            int val = minimax_masks(newPlayer, oppMask, depth + 1, alpha, beta, false);
            if (val > best) best = val;
            alpha = (val > alpha) ? val : alpha;
            if (alpha >= beta) break; // beta cutoff
        } else {
            int newOpp = oppMask | bit;
            int val = minimax_masks(playerMask, newOpp, depth + 1, alpha, beta, true);
            if (val < best) best = val;
            beta = (val < beta) ? val : beta;
            if (alpha >= beta) break; // alpha cutoff
        }
    }

    return best;
}

/*
 * findBestMove: returns the best move for 'player'
 * using alpha-beta-optimized mask minimax.
 */
struct Move findBestMovePerfect(char board[3][3]) {
    int maskP = 0, maskO = 0;
    boardToMasks(board, &maskP, &maskO);
    int occupied = maskP | maskO;

    int bestVal = -1000;
    struct Move bestMove = { -1, -1 };

    for (int mi = 0; mi < 9; ++mi) {
        int pos = MOVE_ORDER[mi];
        int bit = (1 << pos);
        if (occupied & bit) continue;

        int newPlayer = maskP | bit;
        int moveVal = minimax_masks(newPlayer, maskO, 1, -1000, 1000, false);

        if (moveVal > bestVal) {
            bestVal = moveVal;
            bestMove.row = pos / 3;
            bestMove.col = pos % 3;
        }
    }

    return bestMove;
}
