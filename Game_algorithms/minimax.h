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
    int row;    /**< Row index (0-2) */
    int col;    /**< Column index (0-2) */
};

// ============================================================================
// AI FUNCTION DECLARATIONS
// ============================================================================

/**
 * @brief Find the best move using perfect minimax algorithm
 * 
 * Uses alpha-beta pruning with bitboard representation for optimal play.
 * This AI is unbeatable and will always win or draw.
 * 
 * @param board 3x3 game board (char array)
 * @param aiSymbol The symbol the AI is playing ('X' or 'O')
 * @return Move structure with the best move coordinates
 */
struct Move findBestMovePerfect(char board[3][3], char aiSymbol);

/**
 * @brief Find the best move using imperfect minimax algorithm
 * 
 * Uses depth-limited minimax with randomized move ordering to introduce
 * mistakes and make the AI beatable. Good for medium difficulty.
 * 
 * @param board 3x3 game board (char array)
 * @param aiSymbol The symbol the AI is playing ('X' or 'O')
 * @return Move structure with the best move coordinates
 */
struct Move findBestMoveImperfect(char board[3][3], char aiSymbol);

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

#endif // MINIMAX_H
