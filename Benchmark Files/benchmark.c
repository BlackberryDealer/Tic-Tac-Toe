// gcc -o benchmark benchmark.c benchmark_algorithms.c
// ../Game_algorithms/Minimax.c ../Game_algorithms/minimax_utils.c
// ../Game_algorithms/model_minimax.c -I../Game_algorithms -lm

#include "benchmark_algorithms.h"
#include "minimax.h"
#include "model_minimax.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  printf("==================================================\n");
  printf("TIC-TAC-TOE AI BENCHMARK SUITE\n");
  printf("==================================================\n");
  printf("Running all benchmarks...\n");
  printf("==================================================\n\n");

  // Setup Empty Board (Worst Case for Search)
  char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
  struct Move move;
  clock_t start, end;
  double cpu_time_1, cpu_time_2;

  int iterations = 1000;     // For optimization benchmarks
  int ai_iterations = 10000; // For AI difficulty benchmarks

  // ====================================================================
  // PART 1: AI DIFFICULTY BENCHMARKS
  // ====================================================================
  printf("\n");
  printf("##################################################\n");
  printf("PART 1: AI DIFFICULTY BENCHMARKS\n");
  printf("##################################################\n");
  printf("Testing different AI difficulty levels\n");
  printf("Iterations: %d\n\n", ai_iterations);

  // TEST 1: LOGISTIC REGRESSION (EASY / MODEL)
  printf("Testing Easy Mode (Logistic Regression AI Model)...\n");
  start = clock();
  for (int i = 0; i < ai_iterations; i++) {
    move = findBestMoveModel(board);
  }
  end = clock();
  double time_easy = ((double)(end - start)) / CLOCKS_PER_SEC;

  printf("\n");
  printf("========================================\n");
  printf("RESULTS: Easy Mode (Logistic Regression AI Model)\n");
  printf("========================================\n\n");
  printf("Best Move: (%d, %d)\n", move.row, move.col);
  printf("Total Time (%d runs): %.6f s\n", ai_iterations, time_easy);
  printf("Avg Time per Move: %.8f s\n", time_easy / ai_iterations);
  printf("Max Recursion Depth: 0 (Iterative)\n");
  printf("Est. Stack Usage: Constant (Low)\n");
  printf("========================================\n\n");

  // TEST 2: MINIMAX BITBOARD (MEDIUM / IMPERFECT)
  printf("Testing Medium Mode (Imperfect Minimax AI)...\n");
  g_max_depth_reached = 0;
  start = clock();
  for (int i = 0; i < ai_iterations; i++) {
    move = findBestMoveMinimax(board, 'X', 20);
  }
  end = clock();
  double time_medium = ((double)(end - start)) / CLOCKS_PER_SEC;

  printf("\n");
  printf("========================================\n");
  printf("RESULTS: Medium Mode (Imperfect Minimax AI)\n");
  printf("========================================\n\n");
  printf("Best Move: (%d, %d)\n", move.row, move.col);
  printf("Total Time (%d runs): %.6f s\n", ai_iterations, time_medium);
  printf("Avg Time per Move: %.8f s\n", time_medium / ai_iterations);
  printf("Max Recursion Depth: %d\n", g_max_depth_reached);
  printf("Est. Stack Usage: ~%d bytes\n", g_max_depth_reached * 64);
  printf("========================================\n\n");

  // TEST 3: MINIMAX BITBOARD (HARD / PERFECT)
  printf("Testing Hard Mode (Perfect Minimax AI)...\n");
  g_max_depth_reached = 0;
  start = clock();
  for (int i = 0; i < ai_iterations; i++) {
    move = findBestMoveMinimax(board, 'X', 0);
  }
  end = clock();
  double time_hard = ((double)(end - start)) / CLOCKS_PER_SEC;

  printf("\n");
  printf("========================================\n");
  printf("RESULTS: Hard Mode (Perfect Minimax AI)\n");
  printf("========================================\n\n");
  printf("Best Move: (%d, %d)\n", move.row, move.col);
  printf("Total Time (%d runs): %.6f s\n", ai_iterations, time_hard);
  printf("Avg Time per Move: %.8f s\n", time_hard / ai_iterations);
  printf("Max Recursion Depth: %d\n", g_max_depth_reached);
  printf("Est. Stack Usage: ~%d bytes\n", g_max_depth_reached * 64);
  printf("========================================\n\n");

  // ====================================================================
  // PART 2: MINIMAX OPTIMIZATION BENCHMARKS
  // ====================================================================
  printf("\n\n");
  printf("##################################################\n");
  printf("PART 2: MINIMAX OPTIMIZATION BENCHMARKS\n");
  printf("##################################################\n");
  printf("Comparing different Minimax implementations\n");
  printf("Iterations: %d\n\n", iterations);

  // ====================================================================
  // BENCHMARK 1: ARRAY (NO OPTIMISATIONS) vs ARRAY (ALPHA-BETA)
  // ====================================================================
  printf("\n");
  printf("--------------------------------------------------\n");
  printf("BENCHMARK 1: Algorithmic Optimization Impact\n");
  printf("--------------------------------------------------\n");
  printf("Comparing: Minimax Array (No Optimisations) vs Minimax Array "
         "(Alpha-Beta)\n\n");

  // TEST A: Minimax Array (No Optimisations)
  printf("Testing Minimax Array (No Optimisations)...\n");
  start = clock();
  for (int i = 0; i < iterations; i++) {
    move = findBestMoveMinimax_NoBitboard_NoPruning(board, 'X');
  }
  end = clock();
  cpu_time_1 = ((double)(end - start)) / CLOCKS_PER_SEC;
  double avg_time_1_b1 = cpu_time_1 / iterations;

  // TEST B: Minimax Array (Alpha-Beta)
  printf("Testing Minimax Array (Alpha-Beta)...\n");
  start = clock();
  for (int i = 0; i < iterations; i++) {
    move = findBestMoveMinimax_NoBitboard(board, 'X', 0);
  }
  end = clock();
  cpu_time_2 = ((double)(end - start)) / CLOCKS_PER_SEC;
  double avg_time_2_b1 = cpu_time_2 / iterations;

  printf("\n");
  printf("========================================\n");
  printf("RESULTS:\n");
  printf("========================================\n\n");

  printf("Minimax Array (No Optimisations):\n");
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_1_b1);

  printf("Minimax Array (Alpha-Beta):\n");
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_2_b1);

  printf("SPEEDUP: %.2fx Faster with Alpha-Beta Pruning\n",
         avg_time_1_b1 / avg_time_2_b1);
  printf("========================================\n");

  // ====================================================================
  // BENCHMARK 2: BITBOARD (ALPHA-BETA) vs ARRAY (ALPHA-BETA)
  // ====================================================================
  printf("\n\n");
  printf("--------------------------------------------------\n");
  printf("BENCHMARK 2: Data Structure Efficiency\n");
  printf("--------------------------------------------------\n");
  printf("Comparing: Minimax Bitboard (Alpha-Beta) vs Minimax Array "
         "(Alpha-Beta)\n\n");

  // TEST A: Minimax Bitboard (Alpha-Beta)
  printf("Testing Minimax Bitboard (Alpha-Beta)...\n");
  start = clock();
  for (int i = 0; i < iterations; i++) {
    move = findBestMoveMinimax(board, 'X', 0);
  }
  end = clock();
  cpu_time_1 = ((double)(end - start)) / CLOCKS_PER_SEC;
  double avg_time_1_b2 = cpu_time_1 / iterations;

  // TEST B: Minimax Array (Alpha-Beta)
  printf("Testing Minimax Array (Alpha-Beta)...\n");
  start = clock();
  for (int i = 0; i < iterations; i++) {
    move = findBestMoveMinimax_NoBitboard(board, 'X', 0);
  }
  end = clock();
  cpu_time_2 = ((double)(end - start)) / CLOCKS_PER_SEC;
  double avg_time_2_b2 = cpu_time_2 / iterations;

  printf("\n");
  printf("========================================\n");
  printf("RESULTS:\n");
  printf("========================================\n\n");

  printf("Minimax Bitboard (Alpha-Beta):\n");
  printf("  State Memory Size: %zu bytes\n", sizeof(int) * 2);
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_1_b2);

  printf("Minimax Array (Alpha-Beta):\n");
  printf("  State Memory Size: %zu bytes\n", sizeof(char) * 9);
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_2_b2);

  if (cpu_time_1 < cpu_time_2)
    printf("SPEEDUP: %.2fx (Bitboard is Faster)\n",
           avg_time_2_b2 / avg_time_1_b2);
  else
    printf("SPEEDUP: %.2fx (Array is Faster)\n", avg_time_1_b2 / avg_time_2_b2);

  printf("========================================\n");

  // ====================================================================
  // BENCHMARK 3: BITBOARD (ALPHA-BETA) vs ARRAY (NO OPTIMISATIONS)
  // ====================================================================
  printf("\n\n");
  printf("--------------------------------------------------\n");
  printf("BENCHMARK 3: Maximum Performance Comparison\n");
  printf("--------------------------------------------------\n");
  printf("Comparing: Minimax Bitboard (Alpha-Beta) vs Minimax Array (No "
         "Optimisations)\n\n");

  // TEST A: Minimax Bitboard (Alpha-Beta)
  printf("Testing Minimax Bitboard (Alpha-Beta)...\n");
  start = clock();
  for (int i = 0; i < iterations; i++) {
    move = findBestMoveMinimax(board, 'X', 0);
  }
  end = clock();
  cpu_time_1 = ((double)(end - start)) / CLOCKS_PER_SEC;
  double avg_time_1_b3 = cpu_time_1 / iterations;

  // TEST B: Minimax Array (No Optimisations)
  printf("Testing Minimax Array (No Optimisations)...\n");
  start = clock();
  for (int i = 0; i < iterations; i++) {
    move = findBestMoveMinimax_NoBitboard_NoPruning(board, 'X');
  }
  end = clock();
  cpu_time_2 = ((double)(end - start)) / CLOCKS_PER_SEC;
  double avg_time_2_b3 = cpu_time_2 / iterations;

  printf("\n");
  printf("========================================\n");
  printf("RESULTS:\n");
  printf("========================================\n\n");

  printf("Minimax Bitboard (Alpha-Beta):\n");
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_1_b3);

  printf("Minimax Array (No Optimisations):\n");
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_2_b3);

  printf("SPEEDUP: %.2fx (Fastest vs Slowest Implementation)\n",
         avg_time_2_b3 / avg_time_1_b3);
  printf("========================================\n");

  // Final summary
  printf("\n\n");
  printf("==================================================\n");
  printf("ALL BENCHMARKS COMPLETE\n");
  printf("==================================================\n");

  return 0;
}
