/**
 * @file IMPERFECT_minimax.c
 * @brief Imperfect minimax AI implementation with depth limiting
 * 
 * This module implements a beatable Tic-Tac-Toe AI using depth-limited
 * minimax with randomized move ordering. The AI makes mistakes by:
 * 1. Limiting search depth to 2 (doesn't see far ahead)
 * 2. Randomizing move order (introduces variability)
 * 
 * Uses bitboard representation internally for efficiency.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "minimax.h"
#include "model_minimax.h"

// ============================================================================
// CONSTANTS AND STATIC VARIABLES
// ============================================================================

/* Player symbols (not used in bitboard version, kept for compatibility) */
static char player = 'X';
static char opponent = 'O';

/**
 * @brief Precomputed winning line bit masks
 * 
 * Each mask represents a winning line (row, column, or diagonal).
 * Example: (1<<0)|(1<<1)|(1<<2) = top row (cells 0,1,2)
 */
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

// ============================================================================
// BITBOARD UTILITY FUNCTIONS
// ============================================================================

/**
 * @brief Convert 3x3 board array to bitboard representation
 * 
 * Creates two 9-bit masks: one for X pieces, one for O pieces.
 * Each cell index (0..8) corresponds to a bit position.
 * 
 * @param board 3x3 character array representing the board
 * @param maskX Output parameter: bitmask for X pieces
 * @param maskO Output parameter: bitmask for O pieces
 */
static inline void boardToMasks(const char board[3][3], int *maskX, int *maskO) {
    *maskX = 0;
    *maskO = 0;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) {
            int idx = r * 3 + c; // cell index
            char ch = board[r][c];
            if (ch == 'X') *maskX |= (1 << idx); // set bit for X
            else if (ch == 'O') *maskO |= (1 << idx); // set bit for O
        }
}

/**
 * @brief Check if a bitmask contains a winning line
 * 
 * @param mask Bitmask to check (either X or O mask)
 * @return true if mask contains a winning line, false otherwise
 */
static inline bool isWinnerMask(int mask) {
    for (int i = 0; i < 8; ++i)
        if ((mask & WIN_MASKS[i]) == WIN_MASKS[i])
            return true;
    return false;
}

// ============================================================================
// IMPERFECT MINIMAX ALGORITHM
// ============================================================================

/**
 * @brief Imperfect minimax algorithm with depth limiting and randomization
 * 
 * This implementation makes the AI beatable by:
 * - Limiting search depth to 2 (doesn't see far ahead)
 * - Randomizing move order (introduces variability and mistakes)
 * 
 * @param playerMask Bitmask for the maximizing player (AI)
 * @param oppMask Bitmask for the minimizing player (human)
 * @param depth Current search depth
 * @param isMax true if it's the maximizer's turn, false otherwise
 * @return Score: positive for AI win, negative for human win, 0 for draw
 */
static int minimax_masks(int playerMask, int oppMask, int depth, bool isMax) {
    // Terminal state checks
    if (isWinnerMask(playerMask)) {
        return 10 - depth;  // AI wins
    }
    if (isWinnerMask(oppMask)) {
        return -10 + depth;  // Human wins
    }
    // Draw or depth cutoff (imperfect AI stops at depth 2)
    if ((playerMask | oppMask) == 0x1FF || depth >= 2) {
        return 0;
    }

    // Initialize best score depending on player role
    int best = isMax ? -1000 : 1000;
    int moves[9], count = 0;

    // Collect all available moves
    for (int i = 0; i < 9; ++i) {
        if (!((playerMask | oppMask) & (1 << i))) {
            moves[count++] = i;
        }
    }

    // Shuffle move order to introduce randomness (makes AI beatable)
    for (int i = count - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = moves[i];
        moves[i] = moves[j];
        moves[j] = tmp;
    }

    // Explore each move recursively
    for (int i = 0; i < count; ++i) {
        int pos = moves[i];
        int bit = (1 << pos);
        
        if (isMax) {
            // Maximizer's turn (AI)
            int val = minimax_masks(playerMask | bit, oppMask, depth + 1, false);
            if (val > best) {
                best = val;
            }
        } else {
            // Minimizer's turn (human)
            int val = minimax_masks(playerMask, oppMask | bit, depth + 1, true);
            if (val < best) {
                best = val;
            }
        }
    }
    
    return best;
}

// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

struct Move findBestMoveImperfect(char board[3][3]) {
    // Convert board to bitboard representation
    int maskP = 0, maskO = 0;
    boardToMasks(board, &maskP, &maskO);
    int occupied = maskP | maskO;
    int bestVal = -1000;
    struct Move bestMove = { -1, -1 };

    // Seed randomness for move shuffling (makes AI less predictable)
    srand(time(NULL));

    // Try random positions to find a good move
    // This random selection combined with depth limiting makes the AI beatable
    for (int i = 0; i < 9; ++i) {
        int pos = rand() % 9;  // Pick random cell
        int bit = (1 << pos);
        
        // Skip if cell is already occupied
        if (occupied & bit) {
            continue;
        }

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
