#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "model_minimax.h"
#include "minimax.h"

// Logistic regression weights and bias from logistic_regression_params.json
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
const double LR_BIAS = -1.6450287057758302;

// Board evaluation using logistic regression
double evaluateBoardLogistic(const char board[3][3]) {
    double val = 0.0;
    int idx = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j, ++idx) {
            int feat = 0;
            if (board[i][j] == 'X')
                feat = 1;
            else if (board[i][j] == 'O')
                feat = -1;
            // else 0 for empty
            val += feat * LR_WEIGHTS[idx];
        }
    }
    val += LR_BIAS;
    return val;
}

// Check if there are moves left
static int isMovesLeft(const char board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                return 1;
    return 0;
}

// Find best move using model evaluation
struct Move findBestMoveModel(char board[3][3]) {
    struct Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
    
    double bestVal = -1000.0;
    
    // Try all empty cells
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                // Make the move
                board[i][j] = 'X';
                
                // Evaluate this move
                double moveVal = evaluateBoardLogistic(board);
                
                // Undo the move
                board[i][j] = ' ';
                
                // If this move is better, update best move
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
