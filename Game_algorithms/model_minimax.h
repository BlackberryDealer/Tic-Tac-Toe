/**
 * @file model_minimax.h
 * @brief Machine learning-based board evaluation for Tic-Tac-Toe AI
 * 
 * This module provides a logistic regression model for evaluating
 * board positions. The model was trained on game data and provides
 * a different playing style compared to minimax-based AIs.
 */

#ifndef MODEL_MINIMAX_H
#define MODEL_MINIMAX_H

#include "minimax.h" // For struct Move definition

/**
 * @brief Evaluate a board position using logistic regression
 * 
 * Uses pre-trained weights and bias to compute a score for the
 * current board state. Higher scores indicate better positions
 * for the maximizing player.
 * 
 * @param board 3x3 game board array
 * @return Evaluation score (higher is better for the maximizing player)
 */
double evaluateBoardLogistic(const char board[3][3]);

/**
 * @brief Finds the best move using greedy search with logistic regression evaluation.
 * 
 * @param board Current 3x3 game board.
 * @param aiSymbol The symbol the AI is playing ('X' or 'O').
 * @return The move that results in the highest evaluation score.
 */
struct Move findBestMoveModel(char (*board)[3], char aiSymbol);

#endif // MODEL_MINIMAX_H