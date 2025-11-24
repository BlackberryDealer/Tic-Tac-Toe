/**
 * @file minimax_utils.c
 * @brief Shared utilities for Minimax AI.
 */

#include "minimax_utils.h"
#include <stdbool.h>

/**
 * @brief Precomputed winning line bit masks.
 *
 * Represents rows, columns, and diagonals as 9-bit integers.
 */
const int WIN_MASKS[8] = {
    // ROWS: Three consecutive horizontal cells
    (1 << 0) | (1 << 1) | (1 << 2), // 0b000000111 - Top row
    (1 << 3) | (1 << 4) | (1 << 5), // 0b000111000 - Middle row
    (1 << 6) | (1 << 7) | (1 << 8), // 0b111000000 - Bottom row

    // COLUMNS: Three consecutive vertical cells
    (1 << 0) | (1 << 3) | (1 << 6), // 0b001001001 - Left column
    (1 << 1) | (1 << 4) | (1 << 7), // 0b010010010 - Middle column
    (1 << 2) | (1 << 5) | (1 << 8), // 0b100100100 - Right column

    // DIAGONALS: Corner-to-corner cells
    (1 << 0) | (1 << 4) | (1 << 8), // 0b100010001 - Main diagonal (\)
    (1 << 2) | (1 << 4) | (1 << 6)  // 0b001010100 - Anti-diagonal (/)
};

/**
 * @brief Preferred move order for alpha-beta pruning (Center -> Corners ->
 * Edges).
 */
const int MOVE_ORDER[9] = {4, 0, 2, 6, 8, 1, 3, 5, 7};

/**
 * @brief Converts 3x3 char board to bitmasks.
 */
void boardToMasks(const char board[3][3], int *maskX, int *maskO) {
  *maskX = 0;
  *maskO = 0;
  for (int r = 0; r < 3; ++r) {
    for (int c = 0; c < 3; ++c) {
      int idx = r * 3 + c;
      char ch = board[r][c];
      if (ch == 'X')
        *maskX |= (1 << idx);
      else if (ch == 'O')
        *maskO |= (1 << idx);
    }
  }
}

/**
 * @brief Checks if a mask contains a winning pattern.
 */
bool isWinnerMask(int mask) {
  for (int i = 0; i < 8; ++i) {
    if ((mask & WIN_MASKS[i]) == WIN_MASKS[i])
      return true;
  }
  return false;
}

/**
 * @brief Counts set bits in a mask.
 */
int countBits(int mask) {
  int count = 0;
  while (mask) {
    count += mask & 1;
    mask >>= 1;
  }
  return count;
}

void getPlayerMasks(int maskX, int maskO, char aiSymbol, int *aiMask,
                    int *oppMask) {
  int countX = countBits(maskX);
  int countO = countBits(maskO);

  if (countX == 0 && countO == 0) {
    // Empty board - use the provided aiSymbol
    if (aiSymbol == 'X') {
      *aiMask = maskX;  // AI plays X
      *oppMask = maskO; // Opponent plays O
    } else {
      *aiMask = maskO;  // AI plays O
      *oppMask = maskX; // Opponent plays X
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