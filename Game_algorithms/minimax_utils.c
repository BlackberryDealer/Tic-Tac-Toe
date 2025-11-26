/*
 * minimax_utils.c
 * Shared utilities used by multiple minimax AI implementations
 */

#include "minimax_utils.h"
#include <stdbool.h>

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
 * Rationale:
 * - Checking wins using bitwise AND is much faster than checking array cells.
 * - A player wins if their bitmask contains all bits of any WIN_MASK.
 * - The check is: (playerMask & WIN_MASK) == WIN_MASK
 */
const int WIN_MASKS[8] = {
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

/**
 * MOVE_ORDER: Preferred ordering of moves when searching.
 *
 * Why order matters:
 * - Alpha-beta pruning prunes more branches if good moves are tried first.
 * - Center is most important, then corners, then edges.
 */
const int MOVE_ORDER[9] = { 4, 0, 2, 6, 8, 1, 3, 5, 7 };

/**
 * boardToMasks: Convert 3x3 char board to two integer bitmasks.
 *
 * Each bit of maskX/maskO corresponds to one board cell (0-8). This
 * transformation lets the AIs operate on fast bitwise operations.
 */
void boardToMasks(const char (*board)[3], int *maskX, int *maskO) {
    *maskX = 0;
    *maskO = 0;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            int idx = r * 3 + c;
            char ch = board[r][c];
            if (ch == 'X') *maskX |= (1 << idx);
            else if (ch == 'O') *maskO |= (1 << idx);
        }
    }
}

/**
 * isWinnerMask: Return true if the given mask contains any win pattern.
 *
 * This function replaces multiple array-access checks with 8 bitwise
 * comparisons, making it simpler and faster.
 */
bool isWinnerMask(int mask) {
    for (int i = 0; i < 8; ++i) {
        if ((mask & WIN_MASKS[i]) == WIN_MASKS[i]) return true;
    }
    return false;
}

/**
 * countBits: Return number of set bits in mask.
 *
 * Used to decide whose turn it is (countX vs countO) and simple heuristics.
 */
int countBits(int mask) {
    int count = 0;
    while (mask) {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}

void getPlayerMasks(int maskX, int maskO, char aiSymbol, int *aiMask, int *oppMask) {
    int countX = countBits(maskX);
    int countO = countBits(maskO);

    if (countX == 0 && countO == 0) {
        // Empty board - use the provided aiSymbol
        if (aiSymbol == 'X') {
            *aiMask = maskX;    // AI plays X
            *oppMask = maskO;   // Opponent plays O
        } else {
            *aiMask = maskO;    // AI plays O
            *oppMask = maskX;   // Opponent plays X
        }
    } else {
        // Board has pieces - determine from counts
        // The player with fewer (or equal) pieces goes next
        if (countX <= countO) {
            // X's turn (X has fewer or equal pieces)
            *aiMask = maskX;
            *oppMask = maskO;
        } else {
            // O's turn (O has fewer pieces)
            *aiMask = maskO;
            *oppMask = maskX;
        }
    }
}