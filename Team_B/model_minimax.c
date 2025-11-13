#include <stdio.h>
#include <math.h>
#include "model_minimax.h"

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