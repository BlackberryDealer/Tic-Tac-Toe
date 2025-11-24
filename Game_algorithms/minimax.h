/**
 * @file minimax.h
 * @brief Minimax AI algorithms for Tic-Tac-Toe.
 */

#ifndef MINIMAX_H
#define MINIMAX_H

// ============================================================================
// DATA STRUCTURES
// ============================================================================

/**
 * @struct Move
 * @brief Represents a move on the game board.
 */
struct Move {
  int row; /**< Row index (0-2) */
  int col; /**< Column index (0-2) */
};

// ============================================================================
// AI FUNCTION DECLARATIONS
// ============================================================================

/**
 * @brief Find the best move using Minimax with Adjustable Difficulty.
 * @param board 3x3 game board.
 * @param aiSymbol The symbol the AI is playing ('X' or 'O').
 * @param errorRate Percentage chance (0-100) to make a random move.
 * @return Move structure with the best move coordinates.
 */
struct Move findBestMoveMinimax(char board[3][3], char aiSymbol, int errorRate);

/**
 * @brief Find the best move using model-based evaluation.
 * @param board 3x3 game board.
 * @return Move structure with the best move coordinates.
 */
struct Move findBestMoveModel(char board[3][3]);

// Global for benchmarking
extern int g_max_depth_reached;

#endif // MINIMAX_H
