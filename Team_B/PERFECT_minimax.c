/**
 * @file PERFECT_minimax.c
 * @brief Perfect minimax AI implementation with bitboard optimization
 * 
 * This module implements an unbeatable Tic-Tac-Toe AI using the minimax
 * algorithm with alpha-beta pruning. The implementation uses bitboard
 * representation for efficient game state evaluation.
 * 
 * Internally uses two 9-bit masks: bits 0..8 correspond to cells:
 *  0 1 2
 *  3 4 5
 *  6 7 8
 * 
 * Empty cells are represented by ' ' (space character).
 */

#include <stdio.h>
#include <stdbool.h>
#include "minimax.h"

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
 * A player wins if their bitmask contains all bits of any winning mask.
 */
static const int WIN_MASKS[8] = {
    (1 << 0) | (1 << 1) | (1 << 2), // row 0
    (1 << 3) | (1 << 4) | (1 << 5), // row 1
    (1 << 6) | (1 << 7) | (1 << 8), // row 2
    (1 << 0) | (1 << 3) | (1 << 6), // col 0
    (1 << 1) | (1 << 4) | (1 << 7), // col 1
    (1 << 2) | (1 << 5) | (1 << 8), // col 2
    (1 << 0) | (1 << 4) | (1 << 8), // main diagonal
    (1 << 2) | (1 << 4) | (1 << 6)  // anti-diagonal (top-right to bottom-left)
};

/**
 * @brief Optimized move ordering for alpha-beta pruning
 * 
 * Center → corners → edges ordering helps alpha-beta pruning
 * find cutoffs earlier, significantly reducing search time.
 */
static const int MOVE_ORDER[9] = {4, 0, 2, 6, 8, 1, 3, 5, 7};

// ============================================================================
// BITBOARD UTILITY FUNCTIONS
// ============================================================================

/**
 * @brief Convert 3×3 board array to bitboard representation
 * 
 * Creates two 9-bit masks: one for X pieces, one for O pieces.
 * Each bit position (0-8) corresponds to a board cell.
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
            int idx = r * 3 + c;
            char ch = board[r][c];
            if (ch == 'X') *maskX |= (1 << idx);
            else if (ch == 'O') *maskO |= (1 << idx);
        }
}

/**
 * @brief Check if a bitmask contains a winning line
 * 
 * Tests if the given mask contains all bits of any winning line mask.
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
// MINIMAX ALGORITHM WITH ALPHA-BETA PRUNING
// ============================================================================

/**
 * @brief Internal minimax algorithm using bitboard representation
 * 
 * Recursively evaluates all possible game states using minimax with
 * alpha-beta pruning. Uses bitboard representation for efficiency.
 * 
 * @param playerMask Bitmask for the maximizing player (AI)
 * @param oppMask Bitmask for the minimizing player (human)
 * @param depth Current search depth (0 = root)
 * @param alpha Best value for maximizer found so far
 * @param beta Best value for minimizer found so far
 * @param isMax true if it's the maximizer's turn, false otherwise
 * @return Score: positive for AI win, negative for human win, 0 for draw
 *         Score is adjusted by depth (prefer shorter wins)
 */
static int minimax_masks(int playerMask, int oppMask, int depth,
                         int alpha, int beta, bool isMax)
{
    // Terminal state checks
    if (isWinnerMask(playerMask)) {
        return 10 - depth;  // AI wins (prefer shorter wins)
    }
    if (isWinnerMask(oppMask)) {
        return -10 + depth;  // Human wins (prefer longer losses)
    }
    // Draw: all 9 cells filled (0x1FF = 0b111111111)
    if ((playerMask | oppMask) == 0x1FF || depth == 9) {
        return 0;
    }

    // Initialize best score based on player role
    int best = isMax ? -1000 : 1000;

    // Try moves in optimized order (center → corners → edges)
    for (int mi = 0; mi < 9; ++mi) {
        int pos = MOVE_ORDER[mi];
        int bit = (1 << pos);
        
        // Skip occupied cells
        if ((playerMask | oppMask) & bit) {
            continue;
        }

        if (isMax) {
            // Maximizer's turn (AI)
            int newPlayer = playerMask | bit;
            int val = minimax_masks(newPlayer, oppMask, depth + 1, alpha, beta, false);
            if (val > best) {
                best = val;
            }
            alpha = (val > alpha) ? val : alpha;
            if (alpha >= beta) {
                break;  // Beta cutoff: human has a better option elsewhere
            }
        } else {
            // Minimizer's turn (human)
            int newOpp = oppMask | bit;
            int val = minimax_masks(playerMask, newOpp, depth + 1, alpha, beta, true);
            if (val < best) {
                best = val;
            }
            beta = (val < beta) ? val : beta;
            if (alpha >= beta) {
                break;  // Alpha cutoff: AI has a better option elsewhere
            }
        }
    }

    return best;
}

/**
 * @brief Count the number of set bits in a bitmask
 * 
 * Used to determine which player's turn it is by comparing
 * the number of pieces each player has placed.
 * 
 * @param mask Bitmask to count bits in
 * @return Number of set bits (pieces on board)
 */
static inline int countBits(int mask) {
    int count = 0;
    while (mask) {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}

// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

struct Move findBestMovePerfect(char board[3][3], char aiSymbol) {
    // Convert board to bitboard representation
    int maskX = 0, maskO = 0;
    boardToMasks(board, &maskX, &maskO);
    int occupied = maskX | maskO;

    // Determine which symbol the AI is playing
    // Strategy: If board is empty, use provided aiSymbol.
    // Otherwise, determine from piece counts (player with fewer pieces goes next)
    int countX = countBits(maskX);
    int countO = countBits(maskO);
    
    int aiMask, oppMask;
    if (countX == 0 && countO == 0) {
        // Empty board - use the provided aiSymbol
        if (aiSymbol == 'X') {
            aiMask = maskX;
            oppMask = maskO;
        } else {
            aiMask = maskO;
            oppMask = maskX;
        }
    } else {
        // Board has moves - determine from piece counts
        // The player with fewer pieces (or equal) is the one whose turn it is
        if (countX <= countO) {
            // X has fewer or equal pieces, so it's X's turn
            aiMask = maskX;
            oppMask = maskO;
        } else {
            // O has fewer pieces, so it's O's turn
            aiMask = maskO;
            oppMask = maskX;
        }
    }

    // Search for best move using minimax
    int bestVal = -1000;
    struct Move bestMove = { -1, -1 };

    // Try all possible moves in optimized order
    for (int mi = 0; mi < 9; ++mi) {
        int pos = MOVE_ORDER[mi];
        int bit = (1 << pos);
        
        // Skip occupied cells
        if (occupied & bit) {
            continue;
        }

        // Evaluate this move
        int newAiMask = aiMask | bit;
        int moveVal = minimax_masks(newAiMask, oppMask, 1, -1000, 1000, false);

        // Update best move if this is better
        if (moveVal > bestVal) {
            bestVal = moveVal;
            bestMove.row = pos / 3;
            bestMove.col = pos % 3;
        }
    }

    return bestMove;
}
