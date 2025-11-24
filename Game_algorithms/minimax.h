/**
 * @file minimax.h
 * @brief Minimax AI algorithms for Tic-Tac-Toe
 *
 * This module provides three different AI implementations using minimax
 * algorithms with varying levels of play strength. All implementations
 * use bitboard representations internally for efficient game state evaluation.
 */

#ifndef MINIMAX_H
#define MINIMAX_H

// ============================================================================
// DATA STRUCTURES
// ============================================================================

/**
 * @struct Move
 * @brief Represents a move on the game board
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
 * * This function unifies the Perfect and Imperfect logic.
 * - If errorRate == 0: Plays Perfectly (Unbeatable).
 * - If errorRate > 0:  Has a chance to ignore strategy and play randomly.
 * * @param board 3x3 game board (char array)
 * @param aiSymbol The symbol the AI is playing ('X' or 'O')
 * @param errorRate Percentage chance (0-100) to make a random random move.
 * @return Move structure with the best move coordinates
 */
struct Move findBestMoveMinimax(char board[3][3], char aiSymbol, int errorRate);

/**
 * @brief Find the best move using model-based evaluation
 *
 * Uses a trained logistic regression model to evaluate board positions.
 * Makes different types of mistakes compared to imperfect minimax.
 * Good for easy difficulty.
 *
 * @param board 3x3 game board (char array)
 * @return Move structure with the best move coordinates
 */
struct Move findBestMoveModel(char board[3][3]);

// Global for benchmarking
extern int g_max_depth_reached;

#endif // MINIMAX_H
