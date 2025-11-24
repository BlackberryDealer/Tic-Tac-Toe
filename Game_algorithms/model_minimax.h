/**
 * @file model_minimax.h
 * @brief Machine learning-based board evaluation for Tic-Tac-Toe AI.
 */

#ifndef MODEL_MINIMAX_H
#define MODEL_MINIMAX_H

/**
 * @brief Evaluate a board position using logistic regression.
 * @param board 3x3 game board array.
 * @return Evaluation score (higher is better for X).
 */
double evaluateBoardLogistic(const char board[3][3]);

#endif // MODEL_MINIMAX_H