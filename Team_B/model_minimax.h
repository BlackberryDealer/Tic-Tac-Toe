/**
 * @file model_minimax.h
 * @brief Machine learning-based board evaluation for Tic-Tac-Toe AI
 * 
 * This module provides a logistic regression model for evaluating
 * board positions. The model was trained on game data and provides
 * a different playing style compared to minimax-based AIs.
 * 
 * @author Team B
 * @date 2025
 */

#ifndef MODEL_MINIMAX_H
#define MODEL_MINIMAX_H

/**
 * @brief Evaluate a board position using logistic regression
 * 
 * Uses pre-trained weights and bias to compute a score for the
 * current board state. Higher scores indicate better positions
 * for the maximizing player (X).
 * 
 * @param board 3x3 game board array
 * @return Evaluation score (higher is better for X)
 */
double evaluateBoardLogistic(const char board[3][3]);

#endif // MODEL_MINIMAX_H