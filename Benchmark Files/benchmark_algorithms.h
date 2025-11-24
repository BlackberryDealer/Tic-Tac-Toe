/**
 * @file benchmark_algorithms.h
 * @brief Benchmark-specific Minimax Implementations
 *
 * This file contains variations of the Minimax algorithm used solely for
 * benchmarking purposes (e.g., array-based, no alpha-beta pruning).
 * These are NOT used in the actual game.
 */

#ifndef BENCHMARK_ALGORITHMS_H
#define BENCHMARK_ALGORITHMS_H

#include "../Game_algorithms/minimax.h" // For struct Move

// ============================================================================
// ARRAY-BASED IMPLEMENTATIONS
// ============================================================================

/**
 * @brief Find the best move using standard array-based Minimax (No Bitboard).
 * * Uses Alpha-Beta Pruning.
 * @param board 3x3 game board (char array)
 * @param aiSymbol The symbol the AI is playing ('X' or 'O')
 * @param errorRate Percentage chance (0-100) to make a random random move.
 * @return Move structure with the best move coordinates
 */
struct Move findBestMoveMinimax_NoBitboard(char board[3][3], char aiSymbol,
                                           int errorRate);

/**
 * @brief Find the best move using standard array-based Minimax WITHOUT Pruning.
 * * Used for benchmarking comparison (Slowest).
 * @param board 3x3 game board (char array)
 * @param aiSymbol The symbol the AI is playing ('X' or 'O')
 * @return Move structure with the best move coordinates
 */
struct Move findBestMoveMinimax_NoBitboard_NoPruning(char board[3][3],
                                                     char aiSymbol);

// ============================================================================
// BITBOARD-BASED IMPLEMENTATIONS (UNOPTIMIZED)
// ============================================================================

/**
 * @brief Find the best move using Minimax WITHOUT Alpha-Beta Pruning.
 * * Used for benchmarking to show the efficiency gain of pruning.
 * @param board 3x3 game board (char array)
 * @param aiSymbol The symbol the AI is playing ('X' or 'O')
 * @return Move structure with the best move coordinates
 */
struct Move findBestMoveMinimax_NoAlphaBeta(char board[3][3], char aiSymbol);

#endif // BENCHMARK_ALGORITHMS_H
