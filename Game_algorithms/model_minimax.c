/**
 * @file model_minimax.c
 * @brief Machine learning-based AI implementation using logistic regression.
 */

#include "model_minimax.h"
#include "minimax.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


// ============================================================================
// MODEL PARAMETERS
// ============================================================================

/**
 * @brief Pre-trained logistic regression weights for each board position.
 *
 * Weights correspond to board positions in row-major order.
 * Learned from training data (logistic_regression_params.json).
 */
const double LR_WEIGHTS[9] = {
    3.928391392624212,  // Top-left corner [0][0]
    3.6032407817955696, // Top-middle edge [0][1]
    4.011058129716569,  // Top-right corner [0][2]
    3.6831967066011444, // Middle-left edge [1][0]
    4.313335296889612,  // Center [1][1] - HIGHEST weight!
    3.6169667100902494, // Middle-right edge [1][2]
    3.9842838685550195, // Bottom-left corner [2][0]
    3.669842436819702,  // Bottom-middle edge [2][1]
    3.984526284468059   // Bottom-right corner [2][2]
};

/**
 * @brief Pre-trained logistic regression bias term (intercept).
 */
const double LR_BIAS = -1.6450287057758302;

// ============================================================================
// BOARD EVALUATION FUNCTIONS
// ============================================================================

/**
 * @brief Evaluates a board position using logistic regression.
 *
 * Computes score = Σ(feature_i × weight_i) + bias.
 * Higher positive scores = board looks better for X (the AI).
 *
 * @param board 3x3 game board.
 * @return Score indicating how favorable the position is for X.
 */
double evaluateBoardLogistic(const char board[3][3]) {
  double val = 0.0; // Accumulator for the weighted sum
  int idx = 0;      // Index into the weights array (0-8)

  // Compute the weighted sum by iterating through all 9 positions
  for (int i = 0; i < 3; ++i) {          // Loop through rows
    for (int j = 0; j < 3; ++j, ++idx) { // Loop through columns
      int feat = 0;                      // Feature value for this position

      // Convert board symbol to numerical feature
      if (board[i][j] == 'X') {
        feat = 1; // X contributes positively
      } else if (board[i][j] == 'O') {
        feat = -1; // O contributes negatively
      }

      // Add this position's contribution to the total score
      val += feat * LR_WEIGHTS[idx];
    }
  }

  // Add the bias term
  val += LR_BIAS;

  return val;
}

// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

/**
 * @brief Finds the best move for the AI using logistic regression evaluation.
 *
 * Implements a greedy search strategy:
 * 1. Try placing 'X' in each empty cell.
 * 2. Evaluate the resulting board state.
 * 3. Return the move with the highest score.
 *
 * @param board 3x3 game board array.
 * @return Move struct containing the row and column of the best move.
 */
struct Move findBestMoveModel(char board[3][3]) {
  // Initialize the move structure to store the best move found
  struct Move bestMove;
  bestMove.row = -1;
  bestMove.col = -1;

  // Initialize best score to a very low value
  double bestVal = -1000.0;

  // Try every empty cell on the board as a potential move
  for (int i = 0; i < 3; i++) {   // Iterate through rows
    for (int j = 0; j < 3; j++) { // Iterate through columns

      if (board[i][j] == ' ') { // Only consider empty cells

        // Simulate the move
        board[i][j] = 'X';

        // Evaluate the resulting board state
        double moveVal = evaluateBoardLogistic(board);

        // Undo the move
        board[i][j] = ' ';

        // Update best move if this one is better
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