/**
 * @file IMPERFECT_minimax.c
 * @brief Medium Difficulty AI - Smart but Beatable
 * 
 * This is the MEDIUM difficulty AI. It plays reasonably well but makes
 * deliberate mistakes so humans can beat it.
 * 
 * How it works:
 * - It only thinks 5 moves ahead (instead of looking at every possibility)
 * - It doesn't always pick the absolute best move (introduces variability)
 * - Sometimes it overlooks winning opportunities or defensive moves
 * 
 * This uses a "bitboard" representation (binary numbers) internally to make
 * the calculations super fast. Think of it as a compact way to represent
 * which cells have X's and which have O's.
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


/**
 * @brief Precomputed winning line bit masks
 * 
 * BITBOARD LAYOUT:
 *   Board positions:    Bit indices:
 *     0 | 1 | 2           bit0 | bit1 | bit2
 *    -----------          --------------------
 *     3 | 4 | 5           bit3 | bit4 | bit5
 *    -----------          --------------------
 *     6 | 7 | 8           bit6 | bit7 | bit8
 * 
 * Each WIN_MASK represents a winning combination in binary:
 * - Row 0: bits 0,1,2 → 0b000000111 = 0x007
 * - Main diagonal: bits 0,4,8 → 0b100010001 = 0x111
 * 
 * EFFICIENCY: A player wins if their bitmask contains ALL bits of ANY win mask.
 * Check: (playerMask & WIN_MASK) == WIN_MASK
 * This is a single CPU instruction vs. checking multiple array positions.
 */
static const int WIN_MASKS[8] = {
    // ROWS: Three consecutive horizontal cells
    (1 << 0) | (1 << 1) | (1 << 2),  // 0b000000111 - Top row
    (1 << 3) | (1 << 4) | (1 << 5),  // 0b000111000 - Middle row
    (1 << 6) | (1 << 7) | (1 << 8),  // 0b111000000 - Bottom row
    
    // COLUMNS: Three consecutive vertical cells
    (1 << 0) | (1 << 3) | (1 << 6),  // 0b001001001 - Left column
    (1 << 1) | (1 << 4) | (1 << 7),  // 0b010010010 - Middle column
    (1 << 2) | (1 << 5) | (1 << 8),  // 0b100100100 - Right column
    
    // DIAGONALS: Corner-to-corner cells
    (1 << 0) | (1 << 4) | (1 << 8),  // 0b100010001 - Main diagonal (\)
    (1 << 2) | (1 << 4) | (1 << 6)   // 0b001010100 - Anti-diagonal (/)
};

// ============================================================================
// BITBOARD UTILITY FUNCTIONS
// ============================================================================

/**
 * @brief Convert 3x3 board to bitboard representation
 * 
 * Two bitmasks: maskX (where X is) and maskO (where O is).
 * Each bit position represents one cell (0-8 in row-major order).
 * Much faster for win checking and game tree exploration.
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
 * @brief Check if bitmask contains a winning line
 * 
 * If (mask & WIN_MASK) == WIN_MASK, then all 3 cells of that line are in mask.
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
 * @brief Imperfect minimax with depth limiting and randomization
 * 
 * Explores possible moves but only thinks 5 moves ahead (beatable).
 * Randomizes move order for unpredictability.
 * Scoring: +10 (AI wins), -10 (AI loses), 0 (draw/uncertain).
 * Depth is subtracted from wins to prefer faster victories.
 *
 * @param playerMask Bitmask for maximizing player (AI)
 * @param oppMask Bitmask for minimizing player (human)
 * @param depth Current search depth (0 = start)
 * @param isMax true if AI's turn, false if opponent's turn
 * @return Score: positive for AI win, negative for human win, 0 for draw
 */
static int minimax_masks(int playerMask, int oppMask, int depth, bool isMax) {
    // Terminal state checks
    if (isWinnerMask(playerMask)) return 10 - depth;  // AI wins
    if (isWinnerMask(oppMask)) return -10 + depth;    // Human wins
    if ((playerMask | oppMask) == 0x1FF || depth >= 5) return 0;  // Draw or depth cutoff

    int best = isMax ? -1000 : 1000;
    int moves[9], count = 0;

    // Collect available cells
    for (int i = 0; i < 9; ++i) {
        if (!((playerMask | oppMask) & (1 << i))) {
            moves[count++] = i;
        }
    }

    // Shuffle moves to add randomness
    for (int i = count - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = moves[i];
        moves[i] = moves[j];
        moves[j] = tmp;
    }

    // Evaluate each move
    for (int i = 0; i < count; ++i) {
        int pos = moves[i];
        int bit = (1 << pos);
        
        if (isMax) {
            int val = minimax_masks(playerMask | bit, oppMask, depth + 1, false);
            if (val > best) best = val;
        } else {
            int val = minimax_masks(playerMask, oppMask | bit, depth + 1, true);
            if (val < best) best = val;
        }
    }
    
    return best;
}

// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

/**
 * @brief Find best move (medium). Uses depth-limited minimax.
 *
 * Compact behavior:
 * - Convert board to bitboards
 * - Map aiSymbol to the correct mask (so AI can be 'X' or 'O')
 * - Evaluate every empty cell with minimax_masks (depth limit = 5)
 * - Return the best-scoring move or {-1,-1} if none
 */
struct Move findBestMoveImperfect(char board[3][3], char aiSymbol) {
    int maskX = 0, maskO = 0;
    boardToMasks(board, &maskX, &maskO);

    // Map AI/opponent masks based on aiSymbol so function works for 'X' or 'O'
    int aiMask = (aiSymbol == 'O') ? maskO : maskX;
    int oppMask = (aiSymbol == 'O') ? maskX : maskO;

    int occupied = aiMask | oppMask;
    struct Move bestMove = { -1, -1 };
    int bestVal = -1000;

    // Collect empty cells (guarantees we try all available moves)
    int emptyCells[9];
    int nEmpty = 0;
    for (int i = 0; i < 9; ++i) {
        if (!((occupied) & (1 << i))) emptyCells[nEmpty++] = i;
    }

    if (nEmpty == 0) return bestMove; // board full

    // Shuffle to introduce variety (keeps AI imperfect)
    for (int i = nEmpty - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = emptyCells[i]; emptyCells[i] = emptyCells[j]; emptyCells[j] = tmp;
    }

    // Evaluate each possible AI move: place AI bit, ask minimax what follows
    for (int k = 0; k < nEmpty; ++k) {
        int pos = emptyCells[k];
        int bit = (1 << pos);

        // Simulate AI placing at pos: pass (aiMask | bit) as the maximizing mask
        int score = minimax_masks(aiMask | bit, oppMask, 1, false);

        if (score > bestVal) {
            bestVal = score;
            bestMove.row = pos / 3;
            bestMove.col = pos % 3;
        }
    }

    return bestMove;
}
