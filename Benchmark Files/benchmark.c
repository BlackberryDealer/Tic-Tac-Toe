// Compilation command:
// gcc -o benchmark benchmark.c ../Game_algorithms/Minimax.c
// ../Game_algorithms/minimax_utils.c ../Game_algorithms/model_minimax.c
// -I../Game_algorithms -lm

#include "benchmark_algorithms.h"
#include "minimax.h"
#include "minimax_utils.h"
#include "model_minimax.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Helper to print board
void printBoard(char board[3][3]) {
  for (int i = 0; i < 3; i++) {
    printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
    if (i < 2)
      printf("---|---|---\n");
  }
  printf("\n");
}

int main() {
  printf("==================================================\n");
  printf("TIC-TAC-TOE AI BENCHMARK SUITE\n");
  printf("==================================================\n");
  printf("Running all benchmarks...\n");
  printf("Please wait (Total time: approx 2-3 minutes)\n");
  printf("==================================================\n\n");

  // Setup Empty Board (Worst Case for Search)
  char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
  struct Move move;
  clock_t start, end;
  double cpu_time_1, cpu_time_2;

  // Standardized iterations: 1,000 is the "Goldilocks" number.
  int iterations = 1000;

  // ====================================================================
  // BENCHMARK 1: ARRAY (NO OPTIMISATIONS) vs ARRAY (ALPHA-BETA)
  // ====================================================================
  printf("\n");
  printf("##################################################\n");
  printf("BENCHMARK 1: Array (No Optimisations) vs Array (Alpha-Beta)\n");
  printf("##################################################\n");
  printf("Iterations: %d\n\n", iterations);

  // TEST A: Array (No Optimisations)
  printf("Testing Array (No Optimisations)...\n");
  start = clock();
  for (int i = 0; i < iterations; i++) {
    move = findBestMoveMinimax_NoBitboard_NoPruning(board, 'X');
  }
  end = clock();
  cpu_time_1 = ((double)(end - start)) / CLOCKS_PER_SEC;
  double avg_time_1_b1 = cpu_time_1 / iterations;

  // TEST B: Array (Alpha-Beta)
  printf("Testing Array (Alpha-Beta)...\n");
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

  printf("Array (No Optimisations):\n");
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_1_b1);

  printf("Array (Alpha-Beta):\n");
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_2_b1);

  printf("SPEEDUP: %.2fx Faster with Alpha-Beta\n",
         avg_time_1_b1 / avg_time_2_b1);
  printf("========================================\n");

  // ====================================================================
  // BENCHMARK 2: BITBOARD (ALPHA-BETA) vs ARRAY (ALPHA-BETA)
  // ====================================================================
  printf("\n\n");
  printf("##################################################\n");
  printf("BENCHMARK 2: Bitboard (Alpha-Beta) vs Array (Alpha-Beta)\n");
  printf("##################################################\n");
  printf("Iterations: %d\n\n", iterations);

  // TEST A: Bitboard (Alpha-Beta)
  printf("Testing Bitboard (Alpha-Beta)...\n");
  start = clock();
  for (int i = 0; i < iterations; i++) {
    move = findBestMoveMinimax(board, 'X', 0);
  }
  end = clock();
  cpu_time_1 = ((double)(end - start)) / CLOCKS_PER_SEC;
  double avg_time_1_b2 = cpu_time_1 / iterations;

  // TEST B: Array (Alpha-Beta)
  printf("Testing Array (Alpha-Beta)...\n");
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

  printf("Bitboard (Alpha-Beta):\n");
  printf("  State Memory Size: %zu bytes\n", sizeof(int) * 2);
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_1_b2);

  printf("Array (Alpha-Beta):\n");
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
  printf("##################################################\n");
  printf("BENCHMARK 3: Bitboard (Alpha-Beta) vs Array (No Optimisations)\n");
  printf("##################################################\n");
  printf("Iterations: %d\n\n", iterations);

  // TEST A: Bitboard (Alpha-Beta)
  printf("Testing Bitboard (Alpha-Beta)...\n");
  start = clock();
  for (int i = 0; i < iterations; i++) {
    move = findBestMoveMinimax(board, 'X', 0);
  }
  end = clock();
  cpu_time_1 = ((double)(end - start)) / CLOCKS_PER_SEC;
  double avg_time_1_b3 = cpu_time_1 / iterations;

  // TEST B: Array (No Optimisations)
  printf("Testing Array (No Optimisations)...\n");
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

  printf("Bitboard (Alpha-Beta):\n");
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_1_b3);

  printf("Array (No Optimisations):\n");
  printf("  Avg Time per Move: %.6f s\n\n", avg_time_2_b3);

  printf("SPEEDUP: %.2fx Faster with Bitboard + Alpha-Beta\n",
         avg_time_2_b3 / avg_time_1_b3);
  printf("========================================\n");

  // Final summary
  printf("\n\n");
  printf("==================================================\n");
  printf("ALL BENCHMARKS COMPLETE\n");
  printf("==================================================\n");

  return 0;
}
