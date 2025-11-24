/**
 * @file PERFECT_minimax.c
 * @brief Unified Minimax Implementation (Perfect & Imperfect)
 * * This file implements the core Minimax algorithm.
 * It supports an adjustable "errorRate" to toggle between:
 * 1. PERFECT PLAY (Hard): errorRate = 0
 * 2. IMPERFECT PLAY (Medium): errorRate = 20 (Forced Randomness)
 */

#include "minimax.h"
#include "minimax_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Global for benchmarking
int g_max_depth_reached = 0;

// ============================================================================
// MINIMAX ALGORITHM (PRIVATE HELPER)
// ============================================================================

/**
 * @brief Core minimax algorithm using bitboards and alpha-beta pruning.
 * * This function is PURE STRATEGY (Unlimited Depth).
 * * It does not handle the randomness; that is handled in the public wrapper.
 */
static int minimax_masks(int playerMask, int oppMask, int depth, int alpha,
                         int beta, bool isMax) {
  // Track max depth for benchmarking
  if (depth > g_max_depth_reached) {
    g_max_depth_reached = depth;
  }

  // 1. TERMINAL STATE CHECKS
  if (isWinnerMask(playerMask))
    return 10 - depth;
  if (isWinnerMask(oppMask))
    return -10 + depth;
  if ((playerMask | oppMask) == 0x1FF)
    return 0; // Draw

  // 2. RECURSIVE SEARCH
  int best = isMax ? -1000 : 1000;

  for (int mi = 0; mi < 9; ++mi) {
    int pos = MOVE_ORDER[mi]; // Optimize order (Center -> Corners -> Edges)
    int bit = (1 << pos);

    if ((playerMask | oppMask) & bit)
      continue; // Skip occupied

    if (isMax) {
      int val = minimax_masks(playerMask | bit, oppMask, depth + 1, alpha, beta,
                              false);
      if (val > best)
        best = val;
      if (val > alpha)
        alpha = val;
      if (alpha >= beta)
        break; // Beta cutoff
    } else {
      int val = minimax_masks(playerMask, oppMask | bit, depth + 1, alpha, beta,
                              true);
      if (val < best)
        best = val;
      if (val < beta)
        beta = val;
      if (alpha >= beta)
        break; // Alpha cutoff
    }
  }
  return best;
}

// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

struct Move findBestMoveMinimax(char board[3][3], char aiSymbol,
                                int errorRate) {
  // 1. SETUP: Convert Board to Bitmasks
  int maskX = 0, maskO = 0;
  boardToMasks(board, &maskX, &maskO);

  int aiMask, oppMask;
  getPlayerMasks(maskX, maskO, aiSymbol, &aiMask, &oppMask);

  int occupied = aiMask | oppMask;
  struct Move bestMove = {-1, -1};

  // 2. COLLECT AVAILABLE MOVES
  int emptyCells[9];
  int nEmpty = 0;
  for (int i = 0; i < 9; ++i) {
    if (!((occupied) & (1 << i))) {
      emptyCells[nEmpty++] = i;
    }
  }

  if (nEmpty == 0)
    return bestMove;

  // ========================================================================
  // LOGIC: FORCED RANDOM MISTAKE (Imperfect Mode)
  // ========================================================================
  if (errorRate > 0) {
    int roll = rand() % 100; // 0 to 99
    if (roll < errorRate) {
      int randomIdx = rand() % nEmpty;
      int pos = emptyCells[randomIdx];
      bestMove.row = pos / 3;
      bestMove.col = pos % 3;
      return bestMove; // Exit Early
    }
  }

  // ========================================================================
  // LOGIC: PERFECT MINIMAX (With Randomized Ties)
  // ========================================================================
  int bestVal = -1000;
  struct Move bestCandidates[9]; // Array to store equally good moves
  int candidateCount = 0;

  for (int mi = 0; mi < 9; ++mi) {
    int pos = MOVE_ORDER[mi]; // Use strategic ordering for efficiency
    int bit = (1 << pos);

    if (occupied & bit)
      continue;

    // Run perfect minimax (depth 1, alpha-beta bounds)
    int moveVal = minimax_masks(aiMask | bit, oppMask, 1, -1000, 1000, false);

    // If we found a BETTER move, discard old candidates and start fresh
    if (moveVal > bestVal) {
      bestVal = moveVal;
      candidateCount = 0; // Reset list
      bestCandidates[candidateCount].row = pos / 3;
      bestCandidates[candidateCount].col = pos % 3;
      candidateCount++;
    }
    // If we found an EQUAL move, add it to the list
    else if (moveVal == bestVal) {
      bestCandidates[candidateCount].row = pos / 3;
      bestCandidates[candidateCount].col = pos % 3;
      candidateCount++;
    }
  }

  // 3. PICK A RANDOM MOVE FROM THE BEST CANDIDATES
  if (candidateCount > 0) {
    int randomIndex = rand() % candidateCount;
    bestMove = bestCandidates[randomIndex];
  }

  return bestMove;
}
