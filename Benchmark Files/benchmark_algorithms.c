/**
 * @file benchmark_algorithms.c
 * @brief Implementation of Benchmark-specific Algorithms
 */

#include "benchmark_algorithms.h"
#include "../Game_algorithms/minimax_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Reuse the same benchmarking globals from minimax.c
extern int g_max_depth_reached;

// ============================================================================
// HELPER FUNCTIONS (ARRAY BASED)
// ============================================================================

static bool checkWin(char board[3][3], char player) {
  // Rows & Cols
  for (int i = 0; i < 3; i++) {
    if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
      return true;
    if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
      return true;
  }
  // Diagonals
  if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
    return true;
  if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
    return true;

  return false;
}

static bool isBoardFull(char board[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ')
        return false;
    }
  }
  return true;
}

// ============================================================================
// ARRAY-BASED MINIMAX (WITH ALPHA-BETA)
// ============================================================================

static int minimax_array(char board[3][3], int depth, bool isMax, int alpha,
                         int beta, char aiSymbol, char humanSymbol) {
  if (checkWin(board, aiSymbol))
    return 10 - depth;
  if (checkWin(board, humanSymbol))
    return depth - 10;
  if (isBoardFull(board))
    return 0;

  if (isMax) {
    int best = -1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board[i][j] == ' ') {
          board[i][j] = aiSymbol;
          int val = minimax_array(board, depth + 1, !isMax, alpha, beta,
                                  aiSymbol, humanSymbol);
          board[i][j] = ' ';
          if (val > best)
            best = val;
          if (val > alpha)
            alpha = val;
          if (beta <= alpha)
            break;
        }
      }
      if (beta <= alpha)
        break;
    }
    return best;
  } else {
    int best = 1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board[i][j] == ' ') {
          board[i][j] = humanSymbol;
          int val = minimax_array(board, depth + 1, !isMax, alpha, beta,
                                  aiSymbol, humanSymbol);
          board[i][j] = ' ';
          if (val < best)
            best = val;
          if (val < beta)
            beta = val;
          if (beta <= alpha)
            break;
        }
      }
      if (beta <= alpha)
        break;
    }
    return best;
  }
}

struct Move findBestMoveMinimax_NoBitboard(char board[3][3], char aiSymbol,
                                           int errorRate) {
  char humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';
  struct Move bestMove = {-1, -1};
  int bestVal = -1000;

  // Collect available moves for random mistake logic
  struct Move emptyCells[9];
  int nEmpty = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        emptyCells[nEmpty].row = i;
        emptyCells[nEmpty].col = j;
        nEmpty++;
      }
    }
  }

  if (nEmpty == 0)
    return bestMove;

  // Imperfect Mode Logic
  if (errorRate > 0) {
    int roll = rand() % 100;
    if (roll < errorRate) {
      int randomIdx = rand() % nEmpty;
      return emptyCells[randomIdx];
    }
  }

  // Perfect Mode Logic
  int alpha = -1000;
  int beta = 1000;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        board[i][j] = aiSymbol;

        // Call minimax for the minimizing player (human)
        int moveVal =
            minimax_array(board, 0, false, alpha, beta, aiSymbol, humanSymbol);

        board[i][j] = ' '; // Undo

        if (moveVal > bestVal) {
          bestMove.row = i;
          bestMove.col = j;
          bestVal = moveVal;
        }

        // Update alpha for the root level
        if (moveVal > alpha) {
          alpha = moveVal;
        }
      }
    }
  }

  return bestMove;
}

// ============================================================================
// ARRAY-BASED MINIMAX (NO OPTIMIZATIONS)
// ============================================================================

static int minimax_array_no_pruning(char board[3][3], int depth, bool isMax,
                                    char aiSymbol, char humanSymbol) {
  if (checkWin(board, aiSymbol))
    return 10 - depth;
  if (checkWin(board, humanSymbol))
    return depth - 10;
  if (isBoardFull(board))
    return 0;

  if (isMax) {
    int best = -1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board[i][j] == ' ') {
          board[i][j] = aiSymbol;
          int val = minimax_array_no_pruning(board, depth + 1, !isMax, aiSymbol,
                                             humanSymbol);
          board[i][j] = ' ';
          if (val > best)
            best = val;
        }
      }
    }
    return best;
  } else {
    int best = 1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board[i][j] == ' ') {
          board[i][j] = humanSymbol;
          int val = minimax_array_no_pruning(board, depth + 1, !isMax, aiSymbol,
                                             humanSymbol);
          board[i][j] = ' ';
          if (val < best)
            best = val;
        }
      }
    }
    return best;
  }
}

struct Move findBestMoveMinimax_NoBitboard_NoPruning(char board[3][3],
                                                     char aiSymbol) {
  char humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';
  struct Move bestMove = {-1, -1};
  int bestVal = -1000;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        board[i][j] = aiSymbol;

        int moveVal =
            minimax_array_no_pruning(board, 0, false, aiSymbol, humanSymbol);

        board[i][j] = ' '; // Undo

        if (moveVal > bestVal) {
          bestMove.row = i;
          bestMove.col = j;
          bestVal = moveVal;
        }
      }
    }
  }

  return bestMove;
}

// ============================================================================
// BITBOARD-BASED MINIMAX (NO ALPHA-BETA)
// ============================================================================

/**
 * @brief Core minimax algorithm WITHOUT Alpha-Beta Pruning.
 * * Used for benchmarking comparison.
 */
static int minimax_masks_no_pruning(int playerMask, int oppMask, int depth,
                                    bool isMax) {
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

  // 2. RECURSIVE SEARCH (No Pruning)
  int best = isMax ? -1000 : 1000;

  for (int mi = 0; mi < 9; ++mi) {
    int pos = MOVE_ORDER[mi];
    int bit = (1 << pos);

    if ((playerMask | oppMask) & bit)
      continue;

    if (isMax) {
      int val =
          minimax_masks_no_pruning(playerMask | bit, oppMask, depth + 1, false);
      if (val > best)
        best = val;
      // No Alpha update, No Beta cutoff
    } else {
      int val =
          minimax_masks_no_pruning(playerMask, oppMask | bit, depth + 1, true);
      if (val < best)
        best = val;
      // No Beta update, No Alpha cutoff
    }
  }
  return best;
}

struct Move findBestMoveMinimax_NoAlphaBeta(char board[3][3], char aiSymbol) {
  // 1. SETUP: Convert Board to Bitmasks
  int maskX = 0, maskO = 0;
  boardToMasks(board, &maskX, &maskO);

  int aiMask, oppMask;
  getPlayerMasks(maskX, maskO, aiSymbol, &aiMask, &oppMask);

  int occupied = aiMask | oppMask;
  struct Move bestMove = {-1, -1};
  int bestVal = -1000;

  // 2. ITERATE MOVES (No Randomized Ties logic for simplicity in benchmark)
  for (int mi = 0; mi < 9; ++mi) {
    int pos = MOVE_ORDER[mi];
    int bit = (1 << pos);

    if (occupied & bit)
      continue;

    // Run minimax WITHOUT pruning
    int moveVal = minimax_masks_no_pruning(aiMask | bit, oppMask, 1, false);

    if (moveVal > bestVal) {
      bestVal = moveVal;
      bestMove.row = pos / 3;
      bestMove.col = pos % 3;
    }
  }

  return bestMove;
}
