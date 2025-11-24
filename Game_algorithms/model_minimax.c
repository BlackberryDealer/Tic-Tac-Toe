/**
 * @file model_minimax.c
 * @brief Machine learning-based AI implementation using logistic regression
 * 
 * This module implements an AI that uses a pre-trained logistic regression
 * model to evaluate board positions. The model provides a different playing
 * style compared to minimax-based AIs, making different types of mistakes.
 * 
 * OVERVIEW OF LOGISTIC REGRESSION IN THIS CODE:
 * =============================================
 * Logistic regression is a machine learning algorithm that learns to classify
 * inputs by computing a weighted sum of features. In this tic-tac-toe AI:
 * 
 * 1. FEATURES: Each of the 9 board positions is a feature
 *    - 'X' at a position = +1 (favorable for AI)
 *    - 'O' at a position = -1 (unfavorable for AI)
 *    - Empty position = 0 (neutral)
 * 
 * 2. WEIGHTS: Each position has a learned weight indicating its importance
 *    - Higher weight = more important position for winning
 *    - The weights were learned from training data (past games)
 * 
 * 3. SCORING: For any board state, we compute:
 *    score = (feature₁ × weight₁) + (feature₂ × weight₂) + ... + bias
 * 
 * 4. DECISION: The AI tries all possible moves and picks the one that
 *    produces the highest score (most favorable board state)
 * 
 * This is different from minimax, which explores future moves. Logistic
 * regression only looks at the immediate resulting board position and
 * evaluates it based on learned patterns.
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
 * @brief Pre-trained logistic regression weights for each board position
 */
const double LR_WEIGHTS[9] = {
    3.928391392624212,      // Top-left corner [0][0]
    3.6032407817955696,     // Top-middle edge [0][1]
    4.011058129716569,      // Top-right corner [0][2]
    3.6831967066011444,     // Middle-left edge [1][0]
    4.313335296889612,      // Center [1][1] - HIGHEST weight!
    3.6169667100902494,     // Middle-right edge [1][2]
    3.9842838685550195,     // Bottom-left corner [2][0]
    3.669842436819702,      // Bottom-middle edge [2][1]
    3.984526284468059       // Bottom-right corner [2][2]
};

/**
 * @brief Pre-trained logistic regression bias term (intercept)
 */
const double LR_BIAS = -1.6450287057758302;

// ============================================================================
// BOARD EVALUATION FUNCTIONS
// ============================================================================

/**
 * @brief Evaluates a board position using logistic regression
 * 
 * This is the CORE of the logistic regression classifier. It computes:
 * 
 *   score = Σ(feature_i × weight_i) + bias
 * 
 * 
 * 
 * @param board 3x3 game board with 'X', 'O', or ' ' (empty)
 * @return Score indicating how favorable the position is for X (the AI)
 *         Higher = better for X, Lower = better for O
 */
double evaluateBoardLogistic(const char board[3][3]) {
    double val = 0.0;  // Accumulator for the weighted sum
    int idx = 0;       // Index into the weights array (0-8)
    
    // STEP 1: Compute the weighted sum by iterating through all 9 positions
    // We traverse the board in row-major order (matches weight array order)
    for (int i = 0; i < 3; ++i) {        // Loop through rows
        for (int j = 0; j < 3; ++j, ++idx) {  // Loop through columns
            int feat = 0;  // Feature value for this position
            
            // STEP 2: Convert board symbol to numerical feature
            if (board[i][j] == 'X') {
                feat = 1;   // X contributes positively (good for AI)
                            // This position helps the AI, so add its full weight
            } else if (board[i][j] == 'O') {
                feat = -1;  // O contributes negatively (bad for AI)
                            // This position helps opponent, so subtract its weight
            }
            // If board[i][j] == ' ' (empty), feat remains 0
            // Empty cells contribute nothing to the score (neutral)
            
            // STEP 3: Add this position's contribution to the total score
            // Contribution = feature_value × position_weight
            val += feat * LR_WEIGHTS[idx];
            
            // TRACE EXAMPLE for position [1][1] (center) when it has 'X':
            // feat = 1, LR_WEIGHTS[4] = 4.313
            // val += 1 × 4.313 = 4.313 added to score
            // This large positive contribution reflects center's importance!
        }
    }
    
    // STEP 4: Add the bias term (constant offset)
    // The bias adjusts the baseline score and was learned during training
    // It represents the model's "default" expectation before seeing any pieces
    val += LR_BIAS;
    
    // STEP 5: Return the final score
    // Note: In traditional logistic regression, we'd apply sigmoid function:
    //   probability = 1 / (1 + e^(-val))
    // But for move selection, we only need relative scores (which is higher?),
    // so we skip the sigmoid and just use the raw linear score.
    // Higher scores = more favorable for X (the AI player)
    return val;
}

// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

/**
 * @brief Finds the best move for the AI using logistic regression evaluation
 * 
 * ALGORITHM OVERVIEW:
 * ===================
 * This function implements a GREEDY SEARCH strategy:
 * 1. Try placing 'X' in each empty cell (simulate each possible move)
 * 2. Evaluate the resulting board state using logistic regression
 * 3. Keep track of which move produces the highest score
 * 4. Return that move
 * 
 * DIFFERENCE FROM MINIMAX:
 * - Minimax: Explores future moves recursively, assumes optimal opponent
 * - Logistic Regression: Only looks at immediate next board state, uses
 *   learned patterns to evaluate positions
 * 
 * LOGISTIC REGRESSION CLASSIFICATION IN ACTION:
 * ==============================================
 * The AI is essentially asking: "For each possible move I could make,
 * what would the board look like, and how 'good' would that board be
 * according to patterns learned from training data?"
 * 
 * The logistic regression model has learned from examples like:
 * - Boards where X won tended to have X in center → high center weight
 * - Boards where X won tended to have X in corners → high corner weights
 * - Boards where O had many pieces tended to lead to O winning → negative
 *   contribution when O occupies important squares
 * 
 * By encoding this learned knowledge in weights, the model can quickly
 * evaluate "Is this board position favorable?" without simulating future
 * moves. It's faster but less perfect than minimax.
 * 
 * @param board 3x3 game board array that will be temporarily modified
 *              during evaluation, then restored
 * @return Move struct containing the row and column of the best move
 *         Returns {-1, -1} if no valid moves exist (shouldn't happen in practice)
 */
struct Move findBestMoveModel(char board[3][3]) {
    // Initialize the move structure to store the best move found
    struct Move bestMove;
    bestMove.row = -1;  // -1 indicates no valid move found yet
    bestMove.col = -1;
    
    // Initialize best score to a very low value
    // Any real move will score higher than this, ensuring we pick something
    double bestVal = -1000.0;
    
    // STEP 1: ENUMERATE ALL POSSIBLE MOVES
    // Try every empty cell on the board as a potential move
    for (int i = 0; i < 3; i++) {        // Iterate through rows
        for (int j = 0; j < 3; j++) {    // Iterate through columns
            
            // STEP 2: CHECK IF THIS CELL IS AVAILABLE
            if (board[i][j] == ' ') {    // Only consider empty cells
                
                // STEP 3: SIMULATE THE MOVE
                // Temporarily place 'X' (AI's piece) at this position
                // This creates a hypothetical "what if I move here?" scenario
                board[i][j] = 'X';
                
                // STEP 4: EVALUATE THE RESULTING BOARD STATE
                // Use logistic regression to score this board configuration
                // The score represents how favorable this position is for X
                //
                // HOW CLASSIFICATION WORKS HERE:
                // The logistic regression model takes the board as input
                // (9 features: each cell's state) and outputs a score.
                // This score was learned from training data showing which
                // board configurations led to X winning vs. losing.
                //
                // Higher score = model thinks this board is more likely to
                // lead to a win for X (based on patterns in training data)
                double moveVal = evaluateBoardLogistic(board);
                
                // STEP 5: UNDO THE MOVE
                // Restore the board to its original state so we can try
                // other moves. This is crucial for trying all possibilities.
                board[i][j] = ' ';
                
                // STEP 6: UPDATE BEST MOVE IF THIS ONE IS BETTER
                // Compare this move's score to the best score we've seen
                if (moveVal > bestVal) {
                    // This move scores higher than any previous move!
                    // Update our best move to this position
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;  // Remember this score as the new best
                    
                    // INTERPRETATION:
                    // The logistic regression model has classified this
                    // move as producing a "more favorable" board state
                    // than other moves we've tried. Based on learned
                    // patterns from training, placing X at (i,j) creates
                    // a board configuration that historically correlates
                    // with X winning.
                }
            }
            // If cell is not empty, skip it (can't move there)
        }
    }
    
    // STEP 7: RETURN THE BEST MOVE FOUND
    // After evaluating all possible moves, return the one with highest score
    // This is the move that the logistic regression model predicts will
    // lead to the most favorable board position for X
    return bestMove;
    
    // FINAL NOTE ON LOGISTIC REGRESSION CLASSIFICATION:
    // ==================================================
    // This AI doesn't "think ahead" like minimax. Instead, it uses a
    // CLASSIFICATION MODEL that learned from data to answer the question:
    // "Which of these board positions looks most like winning positions
    // I've seen in my training data?"
    //
    // The weights encode patterns like:
    // - Center control is important (high weight)
    // - Corners are valuable (high weights)
    // - Having more X pieces in important positions = higher score
    // - Having O pieces in important positions = lower score
    //
    // This makes it fast (no recursion, just arithmetic) but potentially
    // weaker than minimax, since it doesn't consider future consequences
    // of moves. It might miss tactics that require looking ahead.
}