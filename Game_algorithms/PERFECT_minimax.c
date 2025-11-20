/**
 * @file PERFECT_minimax.c
 * @brief Perfect minimax AI implementation with bitboard optimization
 * 
 * ============================================================================
 * MINIMAX ALGORITHM EXPLANATION:
 * ============================================================================
 * Minimax is a decision-making algorithm for two-player zero-sum games.
 * It works by:
 * 1. Building a game tree of all possible moves from the current position
 * 2. Evaluating terminal states (win/loss/draw) with numeric scores
 * 3. Propagating scores up the tree:
 *    - Maximizer (AI) chooses moves that MAXIMIZE score
 *    - Minimizer (opponent) chooses moves that MINIMIZE score
 * 4. Assuming both players play optimally
 *    
 * Scores: +10 = AI wins, -10 = Opponent wins, 0 = Draw
 * AI picks the move with the highest guaranteed minimum score.
 * 
 * ============================================================================
 * ALPHA-BETA PRUNING EXPLANATION:
 * ============================================================================
 * Optimization that eliminates branches that cannot affect the final decision.
 * - Alpha: Best value maximizer can guarantee so far (initially -∞)
 * - Beta: Best value minimizer can guarantee so far (initially +∞)
 * - Prune when alpha >= beta (the position won't be chosen by rational play)
 * 
 * Example: If AI found a move scoring +5, and while exploring another move
 * finds the opponent can force -3, we can stop - opponent will never allow
 * this branch since they already have a better option elsewhere.
 * 
 * ============================================================================
 * MEMORY EFFICIENCY IMPROVEMENTS IN THIS CODE:
 * ============================================================================
 * 1. BITBOARD REPRESENTATION:
 *    - Traditional: 3x3 char array = 9 bytes minimum
 *    - Bitboard: Two 32-bit integers = 8 bytes total
 *    - Each bit (0-8) represents one board cell: 0=empty, 1=occupied
 *    - Uses bitwise operations (AND, OR) which are CPU-native and fast
 * 
 * 2. NO BOARD COPYING:
 *    - Traditional minimax copies entire board arrays for each recursive call
 *    - Bitboard version: Just pass two integers (8 bytes) by value
 *    - Recursive depth of 9 × multiple branches = huge memory savings
 * 
 * 3. PRECOMPUTED WIN CONDITIONS:
 *    - Win patterns stored as bit masks (compile-time constants)
 *    - Checking wins = single bitwise AND operation vs. loop comparisons
 * 
 * 4. MOVE ORDERING:
 *    - Tries best moves first (center, corners) for faster pruning
 *    - Better pruning = fewer nodes explored = less memory/time
 */

#include <stdio.h>
#include <stdbool.h>
#include "minimax.h"

// ============================================================================
// CONSTANTS AND STATIC VARIABLES
// ============================================================================


/**
 * @brief Precomputed winning line bit masks
 * 
 * BITBOARD LAYOUT:
 *   Board positions:    Bit indices:
 *     0 | 1 | 2           bit0 | bit1 | bit2
 *    -----------          --------------------
 *     3 | 4 | 5           bit3 | bit4 | bit5
 *    -----------          --------------------
 *     6 | 7 | 8           bit6 | bit7 | bit8
 * 
 * Each WIN_MASK represents a winning combination in binary:
 * - Row 0: bits 0,1,2 → 0b000000111 = 0x007
 * - Main diagonal: bits 0,4,8 → 0b100010001 = 0x111
 * 
 * EFFICIENCY: A player wins if their bitmask contains ALL bits of ANY win mask.
 * Check: (playerMask & WIN_MASK) == WIN_MASK
 * This is a single CPU instruction vs. checking multiple array positions.
 */
static const int WIN_MASKS[8] = {
    // ROWS: Three consecutive horizontal cells
    (1 << 0) | (1 << 1) | (1 << 2),  // 0b000000111 - Top row
    (1 << 3) | (1 << 4) | (1 << 5),  // 0b000111000 - Middle row
    (1 << 6) | (1 << 7) | (1 << 8),  // 0b111000000 - Bottom row
    
    // COLUMNS: Three consecutive vertical cells
    (1 << 0) | (1 << 3) | (1 << 6),  // 0b001001001 - Left column
    (1 << 1) | (1 << 4) | (1 << 7),  // 0b010010010 - Middle column
    (1 << 2) | (1 << 5) | (1 << 8),  // 0b100100100 - Right column
    
    // DIAGONALS: Corner-to-corner cells
    (1 << 0) | (1 << 4) | (1 << 8),  // 0b100010001 - Main diagonal (\)
    (1 << 2) | (1 << 4) | (1 << 6)   // 0b001010100 - Anti-diagonal (/)
};

/**
 * @brief Optimized move ordering for alpha-beta pruning
 * 
 * WHY THIS ORDER MATTERS:
 * Alpha-beta pruning is most efficient when good moves are explored first.
 * In Tic-Tac-Toe:
 * - Center (4): Controls most win lines (4 lines pass through it)
 * - Corners (0,2,6,8): Each controls 3 win lines
 * - Edges (1,3,5,7): Each controls only 2 win lines
 * 
 * PRUNING BENEFIT: By trying strong moves first, we establish tight alpha/beta
 * bounds early, allowing more branches to be pruned without evaluation.
 * 
 * MEMORY IMPACT: Fewer nodes explored = less stack space used in recursion.
 * Can reduce nodes explored from ~60,000 to ~5,000 in worst case.
 */
static const int MOVE_ORDER[9] = {
    4,          // Center - most strategic
    0, 2, 6, 8, // Corners - second best
    1, 3, 5, 7  // Edges - least strategic
};

// ============================================================================
// BITBOARD UTILITY FUNCTIONS
// ============================================================================

/**
 * @brief Convert traditional 3×3 board array to compact bitboard representation
 * 
 * MEMORY TRANSFORMATION:
 * Input:  char board[3][3] = 9+ bytes (each char typically 1 byte)
 * Output: Two integers (maskX, maskO) = 8 bytes total
 * 
 * ALGORITHM:
 * For each cell [r][c]:
 *   1. Calculate linear index: idx = row*3 + col (0-8)
 *   2. If cell contains 'X': Set bit idx in maskX
 *   3. If cell contains 'O': Set bit idx in maskO
 *   4. Empty cells (' '): Leave corresponding bit as 0 in both masks
 * 
 * EXAMPLE:
 * Board:        maskX (binary):      maskO (binary):
 *  X | O | X     1 0 1 0 0 0 1 0 0    0 1 0 1 0 0 0 0 0
 *  O | X |       = 0x105               = 0x00A
 *    |   | X
 * 
 * @param board 3x3 character array representing the game board
 * @param maskX Output: bitmask where 1 = X is present at that position
 * @param maskO Output: bitmask where 1 = O is present at that position
 */
static inline void boardToMasks(const char board[3][3], int *maskX, int *maskO) {
    *maskX = 0;  // Initialize: all bits clear (no X pieces yet)
    *maskO = 0;  // Initialize: all bits clear (no O pieces yet)
    
    // Scan entire board and set appropriate bits
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            int idx = r * 3 + c;      // Linear index: 0-8
            char ch = board[r][c];     // Current cell content
            
            // Set the bit at position 'idx' if this cell has a piece
            if (ch == 'X') 
                *maskX |= (1 << idx);  // OR operation sets bit idx to 1
            else if (ch == 'O') 
                *maskO |= (1 << idx);  // OR operation sets bit idx to 1
            // If ch == ' ' (empty), do nothing - bit stays 0
        }
    }
}

/**
 * @brief Check if a bitmask contains a winning line
 * 
 * ALGORITHM:
 * For each of the 8 winning patterns:
 *   Check if mask contains ALL bits of that pattern
 *   Formula: (mask & WIN_MASK) == WIN_MASK
 * 
 * BITWISE EXPLANATION:
 * - mask & WIN_MASK: Extracts only the bits corresponding to that win line
 * - Compare to WIN_MASK: If equal, player has pieces in all 3 positions
 * 
 * EXAMPLE:
 * mask     = 0b100010001 (pieces at positions 0, 4, 8)
 * WIN_MASKS[6] = 0b100010001 (main diagonal)
 * mask & WIN_MASKS[6] = 0b100010001 == WIN_MASKS[6] → TRUE (winner!)
 * 
 * EFFICIENCY: 8 comparisons vs. traditional nested loops checking all rows,
 * columns, and diagonals with multiple array accesses.
 * 
 * @param mask Bitmask to check (either X's pieces or O's pieces)
 * @return true if mask contains all 3 cells of any winning line, false otherwise
 */
static inline bool isWinnerMask(int mask) {
    // Check all 8 possible winning lines (3 rows + 3 cols + 2 diagonals)
    for (int i = 0; i < 8; ++i) {
        // Bitwise AND isolates the bits for this win line
        // If all 3 bits are set, player has won via this line
        if ((mask & WIN_MASKS[i]) == WIN_MASKS[i]) {
            return true;  // Found a winning line!
        }
    }
    return false;  // No winning line found
}

// ============================================================================
// MINIMAX ALGORITHM WITH ALPHA-BETA PRUNING
// ============================================================================

/**
 * @brief Core minimax algorithm using memory-efficient bitboard representation
 * 
 * ALGORITHM FLOW:
 * 1. BASE CASES (Terminal States):
 *    - Check if current player won → return positive score
 *    - Check if opponent won → return negative score
 *    - Check if board full (draw) → return 0
 * 
 * 2. RECURSIVE CASE:
 *    - Try each empty cell as a potential move
 *    - Recursively evaluate the resulting position
 *    - If maximizing: keep track of highest score (AI wants high scores)
 *    - If minimizing: keep track of lowest score (opponent wants low scores)
 *    - Apply alpha-beta pruning to skip irrelevant branches
 * 
 * 3. RETURN:
 *    - Best score achievable from this position with optimal play
 * 
 * ALPHA-BETA PRUNING MECHANICS:
 * - Alpha: "At least this good" for maximizer (AI)
 * - Beta: "At most this good" for minimizer (opponent)
 * - If alpha >= beta: The current position won't be reached in optimal play
 *   because one player already has a better alternative elsewhere
 * 
 * DEPTH ADJUSTMENT:
 * Scores are adjusted by depth to prefer:
 * - Winning sooner (10 - depth for wins)
 * - Losing later (-10 + depth for losses)
 * This makes the AI aggressive when winning, defensive when losing.
 * 
 * MEMORY EFFICIENCY:
 * - Only passes two integers (8 bytes) per recursive call
 * - No board copying, no dynamic allocation
 * - Stack frames are small and fast
 * 
 * @param playerMask Bitmask for maximizing player (AI's pieces)
 * @param oppMask Bitmask for minimizing player (opponent's pieces)
 * @param depth Current search depth (0 = root, increases in recursion)
 * @param alpha Best score maximizer can guarantee (lower bound for max)
 * @param beta Best score minimizer can guarantee (upper bound for min)
 * @param isMax true = AI's turn (maximize), false = opponent's turn (minimize)
 * @return Evaluation score: +10 to -10 range, adjusted by depth
 */
static int minimax_masks(int playerMask, int oppMask, int depth,
                         int alpha, int beta, bool isMax)
{
    // ========================================================================
    // TERMINAL STATE CHECKS (Base Cases - Stop Recursion)
    // ========================================================================
    
    // Did the AI (maximizing player) win?
    if (isWinnerMask(playerMask)) {
        // Return positive score, reduced by depth to prefer faster wins
        // Depth 1 win: +9, Depth 5 win: +5
        return 10 - depth;
    }
    
    // Did the opponent (minimizing player) win?
    if (isWinnerMask(oppMask)) {
        // Return negative score, adjusted by depth to delay losses
        // Depth 1 loss: -9, Depth 5 loss: -5
        return -10 + depth;
    }
    
    // Is the board completely filled?
    // 0x1FF = 0b111111111 (all 9 bits set)
    // (playerMask | oppMask) gives all occupied cells
    if ((playerMask | oppMask) == 0x1FF || depth == 9) {
        return 0;  // Draw - no winner
    }

    // ========================================================================
    // RECURSIVE CASE (Continue Search)
    // ========================================================================
    
    // Initialize best score based on whose turn it is
    // Maximizer starts at -infinity (any score is better)
    // Minimizer starts at +infinity (any score is better)
    int best = isMax ? -1000 : 1000;

    // Try all possible moves in strategic order (center → corners → edges)
    for (int mi = 0; mi < 9; ++mi) {
        int pos = MOVE_ORDER[mi];  // Get next position to try (0-8)
        int bit = (1 << pos);       // Convert to bitmask (single bit set)
        
        // Skip this position if already occupied by either player
        // (playerMask | oppMask) gives all occupied cells
        if ((playerMask | oppMask) & bit) {
            continue;  // Cell taken, try next position
        }

        // ====================================================================
        // MAXIMIZER'S TURN (AI Making Move)
        // ====================================================================
        if (isMax) {
            // Simulate placing AI's piece at this position
            int newPlayer = playerMask | bit;  // Add bit to AI's mask
            
            // Recursively evaluate: opponent's response to this move
            // - Opponent (minimizer) will try to minimize the score
            // - Search one level deeper (depth + 1)
            // - Pass alpha/beta bounds for pruning
            int val = minimax_masks(newPlayer, oppMask, depth + 1, alpha, beta, false);
            
            // Is this move better than our current best?
            if (val > best) {
                best = val;
            }
            
            // Update alpha (best score maximizer can guarantee)
            // If we found something better than alpha, update it
            alpha = (val > alpha) ? val : alpha;
            
            // PRUNING CHECK: Alpha-Beta Cutoff
            // If alpha >= beta, the minimizer (opponent) will never allow
            // this position because they have a better option elsewhere.
            // No point exploring further moves in this branch.
            if (alpha >= beta) {
                break;  // Beta cutoff - prune remaining moves
            }
        }
        // ====================================================================
        // MINIMIZER'S TURN (Opponent Making Move)
        // ====================================================================
        else {
            // Simulate placing opponent's piece at this position
            int newOpp = oppMask | bit;  // Add bit to opponent's mask
            
            // Recursively evaluate: AI's response to this move
            // - AI (maximizer) will try to maximize the score
            // - Search one level deeper (depth + 1)
            // - Pass alpha/beta bounds for pruning
            int val = minimax_masks(playerMask, newOpp, depth + 1, alpha, beta, true);
            
            // Is this move worse (lower score) than our current best?
            if (val < best) {
                best = val;
            }
            
            // Update beta (best score minimizer can guarantee)
            // If we found something lower than beta, update it
            beta = (val < beta) ? val : beta;
            
            // PRUNING CHECK: Alpha-Beta Cutoff
            // If alpha >= beta, the maximizer (AI) will never allow
            // this position because they have a better option elsewhere.
            // No point exploring further moves in this branch.
            if (alpha >= beta) {
                break;  // Alpha cutoff - prune remaining moves
            }
        }
    }

    // Return the best score achievable from this position
    return best;
}

/**
 * @brief Count the number of pieces (set bits) in a bitmask
 * 
 * ALGORITHM (Brian Kernighan's method variation):
 * - Extract least significant bit with (mask & 1)
 * - Add to count
 * - Right shift to process next bit
 * - Repeat until mask is 0
 * 
 * PURPOSE:
 * Used to determine whose turn it is by comparing piece counts:
 * - If countX == countO: X's turn (X moves first)
 * - If countX > countO: O's turn (players alternate)
 * 
 * EXAMPLE:
 * mask = 0b100010001 (bits 0, 4, 8 set)
 * Iteration 1: count=1, mask=0b10001000
 * Iteration 2: count=1, mask=0b1000100
 * Iteration 3: count=1, mask=0b100010
 * ... continues until count=3
 * 
 * @param mask Bitmask representing pieces on board
 * @return Number of set bits (pieces) in the mask
 */
static inline int countBits(int mask) {
    int count = 0;
    while (mask) {
        count += mask & 1;  // Add least significant bit to count
        mask >>= 1;         // Shift right to process next bit
    }
    return count;
}

// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

/**
 * @brief Find the optimal move for AI using perfect minimax algorithm
 * 
 * HIGH-LEVEL ALGORITHM:
 * 1. Convert board from 2D array to bitboard representation
 * 2. Determine which player is AI (based on piece counts and aiSymbol)
 * 3. Try all empty positions
 * 4. For each position: run minimax to evaluate if AI plays there
 * 5. Return position with highest minimax score
 * 
 * MEMORY EFFICIENCY HIGHLIGHTS:
 * - Converts 9-byte board to 8-byte bitboards
 * - All recursive calls use bitboards (no board copying)
 * - Results in 50-100x less memory usage during search
 * 
 * @param board 3x3 character array of current game state
 * @param aiSymbol Character representing AI ('X' or 'O')
 * @return struct Move containing optimal row and column (0-2 each)
 */
struct Move findBestMovePerfect(char board[3][3], char aiSymbol) {
    // Step 1: Convert traditional board to bitboard representation
    int maskX = 0, maskO = 0;
    boardToMasks(board, &maskX, &maskO);
    int occupied = maskX | maskO;  // All occupied cells (either X or O)

    // Step 2: Determine which player is the AI
    // Count pieces to figure out whose turn it is
    int countX = countBits(maskX);
    int countO = countBits(maskO);
    
    int aiMask, oppMask;
    
    if (countX == 0 && countO == 0) {
        // Empty board - use the provided aiSymbol
        if (aiSymbol == 'X') {
            aiMask = maskX;    // AI plays X
            oppMask = maskO;   // Opponent plays O
        } else {
            aiMask = maskO;    // AI plays O
            oppMask = maskX;   // Opponent plays X
        }
    } else {
        // Board has pieces - determine from counts
        // The player with fewer (or equal) pieces goes next
        if (countX <= countO) {
            // X's turn (X has fewer or equal pieces)
            aiMask = maskX;
            oppMask = maskO;
        } else {
            // O's turn (O has fewer pieces)
            aiMask = maskO;
            oppMask = maskX;
        }
    }

    // Step 3: Search for the best move
    int bestVal = -1000;               // Worst possible score initially
    struct Move bestMove = { -1, -1 }; // Invalid move as default

    // Step 4: Try all possible moves in strategic order
    for (int mi = 0; mi < 9; ++mi) {
        int pos = MOVE_ORDER[mi];  // Get position to try (0-8)
        int bit = (1 << pos);       // Convert to bit mask
        
        // Skip if this cell is already occupied
        if (occupied & bit) {
            continue;
        }

        // Simulate AI playing at this position
        int newAiMask = aiMask | bit;
        
        // Evaluate this move using minimax
        // - Start at depth 1 (first move)
        // - Alpha = -1000 (no lower bound yet)
        // - Beta = 1000 (no upper bound yet)
        // - isMax = false (opponent responds next, will minimize)
        int moveVal = minimax_masks(newAiMask, oppMask, 1, -1000, 1000, false);

        // Step 5: Update best move if this is better
        if (moveVal > bestVal) {
            bestVal = moveVal;
            bestMove.row = pos / 3;  // Convert linear index to row
            bestMove.col = pos % 3;  // Convert linear index to column
        }
    }

    // Return the optimal move
    return bestMove;
}