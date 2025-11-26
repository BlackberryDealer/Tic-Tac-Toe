/**
 * @file model_minimax.c
 * @brief Machine learning-based AI implementation using logistic regression.
 * 
 * This module uses a pre-trained logistic regression model to evaluate board 
 * positions. Unlike minimax, it evaluates the immediate resulting board state 
 * using learned weights rather than exploring future game trees.
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
 * @brief Pre-trained logistic regression weights for each board position (3x3).
 */
const double LR_WEIGHTS[9] = {
    3.928391392624212,     // [0][0]
    3.6032407817955696,    // [0][1]
    4.011058129716569,     // [0][2]
    3.6831967066011444,    // [1][0]
    4.313335296889612,     // [1][1]
    3.6169667100902494,    // [1][2]
    3.9842838685550195,    // [2][0]
    3.669842436819702,     // [2][1]
    3.984526284468059      // [2][2]
};

/**
 * @brief Pre-trained logistic regression bias term.
 */
const double LR_BIAS = -1.6450287057758302;

// ============================================================================
// BOARD EVALUATION FUNCTIONS
// ============================================================================

/**
 * @brief Evaluates a board position using the logistic regression model.
 * @param board 3x3 game board.
 * @return Score (Higher = better for AI/X).
 */
double evaluateBoardLogistic(const char (*board)[3]) {
    double val = 0.0;
    int idx = 0;
    
    // Compute weighted sum of features
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j, ++idx) {
            int feat = 0;
            
            // Convert board state to numerical features: X=1, O=-1, Empty=0
            if (board[i][j] == 'X') {
                feat = 1;
            } else if (board[i][j] == 'O') {
                feat = -1;
            }
            
            val += feat * LR_WEIGHTS[idx];
        }
    }
    
    return val + LR_BIAS;
}

// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

/**
 * @brief Finds the best move using greedy search with logistic regression evaluation.
 * @param board Current 3x3 game board.
 * @return The move that results in the highest evaluation score.
 */
struct Move findBestMoveModel(char (*board)[3]) {
    struct Move bestMove = {-1, -1};
    double bestVal = -1000.0;
    
    // Iterate through all possible moves
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            
            if (board[i][j] == ' ') {
                // Simulate move
                board[i][j] = 'X';
                
                // Evaluate resulting board state
                double moveVal = evaluateBoardLogistic(board);
                
                // Undo move
                board[i][j] = ' ';
                
                // Update best move if this score is higher
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


