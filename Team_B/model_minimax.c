/**
 * @file model_minimax.c
 * @brief Machine learning-based AI implementation using logistic regression
 * 
 * This module implements an AI that uses a pre-trained logistic regression
 * model to evaluate board positions. The model provides a different playing
 * style compared to minimax-based AIs, making different types of mistakes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "model_minimax.h"
#include "minimax.h"

// ============================================================================
// MODEL PARAMETERS
// ============================================================================

/**
 * @brief Pre-trained logistic regression weights
 * 
 * These weights were trained on game data and loaded from
 * logistic_regression_params.json. Each weight corresponds to a board position.
 */
const double LR_WEIGHTS[9] = {
    3.928391392624212,
    3.6032407817955696,
    4.011058129716569,
    3.6831967066011444,
    4.313335296889612,
    3.6169667100902494,
    3.9842838685550195,
    3.669842436819702,
    3.984526284468059
};

/**
 * @brief Pre-trained logistic regression bias term
 */
const double LR_BIAS = -1.6450287057758302;

// ============================================================================
// BOARD EVALUATION FUNCTIONS
// ============================================================================

double evaluateBoardLogistic(const char board[3][3]) {
    double val = 0.0;
    int idx = 0;
    
    // Compute weighted sum: X=+1, O=-1, empty=0
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j, ++idx) {
            int feat = 0;
            if (board[i][j] == 'X') {
                feat = 1;   // X contributes positively
            } else if (board[i][j] == 'O') {
                feat = -1;  // O contributes negatively
            }
            // Empty cells contribute 0
            val += feat * LR_WEIGHTS[idx];
        }
    }
    
    // Add bias term
    val += LR_BIAS;
    return val;
}

/**
 * @brief Check if there are any empty cells remaining
 * 
 * @param board 3x3 game board array
 * @return 1 if moves are available, 0 otherwise
 */
static int isMovesLeft(const char board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                return 1;
    return 0;
}

// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

struct Move findBestMoveModel(char board[3][3]) {
    struct Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
    
    double bestVal = -1000.0;
    
    // Try all empty cells and evaluate each move
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                // Temporarily make the move (assume AI is X)
                board[i][j] = 'X';
                
                // Evaluate this board position using logistic regression
                double moveVal = evaluateBoardLogistic(board);
                
                // Undo the move (restore board state)
                board[i][j] = ' ';
                
                // Update best move if this position scores higher
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
