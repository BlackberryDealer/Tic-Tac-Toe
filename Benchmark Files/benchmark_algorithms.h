// Benchmark-specific Minimax implementations (not used in actual game)

#ifndef BENCHMARK_ALGORITHMS_H
#define BENCHMARK_ALGORITHMS_H

#include "../Game_algorithms/minimax.h"

// ============================================================================
// ARRAY-BASED MINIMAX IMPLEMENTATIONS
// ============================================================================

/**
 * Array-based Minimax WITH Alpha-Beta Pruning
 *
 * Uses traditional 2D array to represent the board instead of bitboard.
 * Includes Alpha-Beta pruning optimization for faster search.
 *
 * @param board      3x3 character array (' ' = empty, 'X' or 'O' = occupied)
 * @param aiSymbol   The symbol AI is playing as ('X' or 'O')
 * @param errorRate  Percentage (0-100) chance AI makes a random move instead
 *                   of optimal move. Use 0 for perfect play, >0 for imperfect.
 * @return Move structure containing {row, col} of best move
 */
struct Move findBestMoveMinimax_NoBitboard(char board[3][3], char aiSymbol,
                                           int errorRate);

/**
 * Array-based Minimax WITHOUT any optimizations (slowest)
 *
 * Uses traditional 2D array with NO Alpha-Beta pruning.
 * Used to benchmark the performance impact of pruning optimization.
 *
 * @param board     3x3 character array (' ' = empty, 'X' or 'O' = occupied)
 * @param aiSymbol  The symbol AI is playing as ('X' or 'O')
 * @return Move structure containing {row, col} of best move
 */
struct Move findBestMoveMinimax_NoBitboard_NoPruning(char board[3][3],
                                                     char aiSymbol);

// ============================================================================
// BITBOARD-BASED MINIMAX IMPLEMENTATIONS
// ============================================================================

/**
 * Bitboard-based Minimax WITHOUT Alpha-Beta Pruning
 *
 * Uses efficient bitboard representation but NO Alpha-Beta pruning.
 * Used to benchmark the performance impact of pruning on bitboard version.
 *
 * @param board     3x3 character array (' ' = empty, 'X' or 'O' = occupied)
 *                  Converted internally to bitboard representation
 * @param aiSymbol  The symbol AI is playing as ('X' or 'O')
 * @return Move structure containing {row, col} of best move
 */
struct Move findBestMoveMinimax_NoAlphaBeta(char board[3][3], char aiSymbol);

/**
 * Bitboard-based Minimax WITH Alpha-Beta Pruning (Production version)
 *
 * Copy of production Minimax.c implementation for stack measurement.
 * Uses efficient bitboard representation with Alpha-Beta pruning.
 *
 * @param board     3x3 character array (' ' = empty, 'X' or 'O' = occupied)
 * @param aiSymbol  The symbol AI is playing as ('X' or 'O')
 * @return Move structure containing {row, col} of best move
 */
struct Move findBestMoveMinimax_Bitboard(char board[3][3], char aiSymbol);

#endif
