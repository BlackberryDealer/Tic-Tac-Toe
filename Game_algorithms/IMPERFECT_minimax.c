/**
 * @file IMPERFECT_minimax.c
 * @brief Medium Difficulty AI - Smart but Beatable
 * 
 * This is the MEDIUM difficulty AI. It plays reasonably well but makes
 * deliberate mistakes so humans can beat it.
 * 
 * How it works:
 * - It only thinks 5 moves ahead (instead of looking at every possibility)
 * - It doesn't always pick the absolute best move (introduces variability)
 * - Sometimes it overlooks winning opportunities or defensive moves
 * 
 * This uses a "bitboard" representation (binary numbers) internally to make
 * the calculations super fast. Think of it as a compact way to represent
 * which cells have X's and which have O's.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "minimax.h"
#include "model_minimax.h"
#include "minimax_utils.h"


// ============================================================================
// IMPERFECT MINIMAX ALGORITHM
// ============================================================================

/**
 * @brief Imperfect minimax with depth limiting and randomization
 * 
 * Explores possible moves but only thinks 5 moves ahead (beatable).
 * Randomizes move order for unpredictability.
 * Scoring: +10 (AI wins), -10 (AI loses), 0 (draw/uncertain).
 * Depth is subtracted from wins to prefer faster victories.
 *
 * @param playerMask Bitmask for maximizing player (AI)
 * @param oppMask Bitmask for minimizing player (human)
 * @param depth Current search depth (0 = start)
 * @param isMax true if AI's turn, false if opponent's turn
 * @return Score: positive for AI win, negative for human win, 0 for draw
 */
// Imperfect minimax upgraded to use alpha-beta pruning for efficiency.
// Logic preserved: depth cutoff at 5 and randomized move order remain.
static int minimax_masks(int playerMask, int oppMask, int depth,
                         int alpha, int beta, bool isMax) {
    /*
     * NOTE: This imperfect variant keeps its own minimax implementation
     * because it enforces a depth cutoff (depth >= 5) and intentionally
     * preserves randomized move ordering to keep the AI "imperfect".
     * We added alpha-beta pruning for efficiency but kept the original
     * cutoff and randomness so gameplay behavior is unchanged.
     */
    // Terminal checks (same as before)
    if (isWinnerMask(playerMask)) return 10 - depth;
    if (isWinnerMask(oppMask)) return -10 + depth;
    if ((playerMask | oppMask) == 0x1FF || depth >= 5) return 0;

    int best = isMax ? -1000 : 1000;
    int moves[9], count = 0;

    // Collect available cells
    for (int i = 0; i < 9; ++i) {
        if (!((playerMask | oppMask) & (1 << i))) moves[count++] = i;
    }

    // Shuffle to keep AI imperfect (randomness preserved)
    for (int i = count - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = moves[i]; moves[i] = moves[j]; moves[j] = tmp;
    }

    for (int i = 0; i < count; ++i) {
        int pos = moves[i];
        int bit = (1 << pos);

        if (isMax) {
            int val = minimax_masks(playerMask | bit, oppMask, depth + 1, alpha, beta, false);
            if (val > best) best = val;
            if (val > alpha) alpha = val;
            if (alpha >= beta) break; // beta cutoff
        } else {
            int val = minimax_masks(playerMask, oppMask | bit, depth + 1, alpha, beta, true);
            if (val < best) best = val;
            if (val < beta) beta = val;
            if (alpha >= beta) break; // alpha cutoff
        }
    }

    return best;
}


// ============================================================================
// PUBLIC API FUNCTION
// ============================================================================

/**
 * @brief Find best move (medium). Uses depth-limited minimax.
 *
 * Compact behavior:
 * - Convert board to bitboards
 * - Map aiSymbol to the correct mask (so AI can be 'X' or 'O')
 * - Evaluate every empty cell with minimax_masks (depth limit = 5)
 * - Return the best-scoring move or {-1,-1} if none
 */
struct Move findBestMoveImperfect(char board[3][3], char aiSymbol) {
    int maskX = 0, maskO = 0;
    boardToMasks(board, &maskX, &maskO);

    // Determine which player is the AI
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

    int occupied = aiMask | oppMask;
    struct Move bestMove = { -1, -1 };
    int bestVal = -1000;

    // Collect empty cells (guarantees we try all available moves)
    int emptyCells[9];
    int nEmpty = 0;
    for (int i = 0; i < 9; ++i) {
        if (!((occupied) & (1 << i))) emptyCells[nEmpty++] = i;
    }

    if (nEmpty == 0) return bestMove; // board full

    // Shuffle to introduce variety (keeps AI imperfect)
    for (int i = nEmpty - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = emptyCells[i]; emptyCells[i] = emptyCells[j]; emptyCells[j] = tmp;
    }

    // Evaluate each possible AI move: place AI bit, ask minimax what follows
    for (int k = 0; k < nEmpty; ++k) {
        int pos = emptyCells[k];
        int bit = (1 << pos);

    // Simulate AI placing at pos: pass (aiMask | bit) as the maximizing mask
    // Use alpha-beta initial bounds (-1000, 1000)
    int score = minimax_masks(aiMask | bit, oppMask, 1, -1000, 1000, false);

        if (score > bestVal) {
            bestVal = score;
            bestMove.row = pos / 3;
            bestMove.col = pos % 3;
        }
    }

    return bestMove;
}
