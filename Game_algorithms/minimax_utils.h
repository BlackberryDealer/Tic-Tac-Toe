/**
 * @file minimax_utils.h
 * @brief Shared bitboard utilities for minimax AIs
 */
#ifndef MINIMAX_UTILS_H
#define MINIMAX_UTILS_H

#include <stdbool.h>

// Precomputed win masks (8 possible lines)
extern const int WIN_MASKS[8];

// Preferred move ordering (center, corners, edges)
extern const int MOVE_ORDER[9];

// Convert 3x3 char board to two bitmasks (maskX, maskO)
void boardToMasks(const char board[3][3], int *maskX, int *maskO);

// Return true if mask contains any winning line
bool isWinnerMask(int mask);

// Count number of set bits in mask
int countBits(int mask);

#endif // MINIMAX_UTILS_H
