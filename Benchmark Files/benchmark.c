// gcc -o benchmark benchmark.c benchmark_algorithms.c
// ../Game_algorithms/Minimax.c ../Game_algorithms/minimax_utils.c
// ../Game_algorithms/model_minimax.c -I../Game_algorithms -lm

#include "benchmark_algorithms.h"
#include "minimax.h"
#include "model_minimax.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// ============================================================================
// DYNAMIC STACK TRACKING SYSTEM
// ============================================================================

typedef struct {
    void* stack_base;
    size_t max_stack_used;
    bool enabled;
} StackTracker;

// Global stack tracker
static StackTracker g_stack_tracker = {NULL, 0, false};

/**
 * Enable stack tracking for calibration phase
 * Call this before running a single calibration iteration
 */
void enable_stack_tracking() {
    void* dummy;
    g_stack_tracker.stack_base = &dummy;
    g_stack_tracker.max_stack_used = 0;
    g_stack_tracker.enabled = true;
}

/**
 * Disable stack tracking for timing benchmarks
 * Call this before running performance tests
 */
void disable_stack_tracking() {
    g_stack_tracker.enabled = false;
}

/**
 * Get the measured stack usage from last calibration
 */
size_t get_measured_stack_usage() {
    return g_stack_tracker.max_stack_used;
}

/**
 * Track current stack usage (called from instrumented functions)
 * This has overhead, so only call during calibration phase
 */
void track_stack_usage() {
    if (!g_stack_tracker.enabled) return;
    
    void* current_stack;
    size_t used = (size_t)((char*)g_stack_tracker.stack_base - (char*)&current_stack);
    
    if (used > g_stack_tracker.max_stack_used) {
        g_stack_tracker.max_stack_used = used;
    }
}

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

  // Memory analysis for Easy mode
  int easy_frame_size = sizeof(int) * 3 + sizeof(double) * 9;  // Simple iterative function
  
  printf("\n");
  printf("========================================\n");
  printf("RESULTS: Easy Mode (Logistic Regression AI Model)\n");
  printf("========================================\n\n");
  printf("Best Move: (%d, %d)\n", move.row, move.col);
  printf("Total Time (%d runs): %.6f s\n", ai_iterations, time_easy);
  printf("Avg Time per Move: %.8f s\n", time_easy / ai_iterations);
  printf("========================================\n\n");

  // TEST 2: MINIMAX BITBOARD (MEDIUM / IMPERFECT)
  printf("Testing Medium Mode (Imperfect Minimax AI)...\n");
  
  // Calibrate stack usage
  enable_stack_tracking();
  g_max_depth_reached = 0;
  move = findBestMoveMinimax(board, 'X', 20);
  size_t medium_stack = get_measured_stack_usage();
  int medium_depth = g_max_depth_reached;
  disable_stack_tracking();
  
  // Run timing benchmarks
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
  printf("========================================\n\n");

  // TEST 3: MINIMAX BITBOARD (HARD / PERFECT)
  printf("Testing Hard Mode (Perfect Minimax AI)...\n");
  
  // Calibrate stack usage
  enable_stack_tracking();
  g_max_depth_reached = 0;
  move = findBestMoveMinimax(board, 'X', 0);
  size_t hard_stack = get_measured_stack_usage();
  int hard_depth = g_max_depth_reached;
  disable_stack_tracking();
  
  // Run timing benchmarks
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

  // DYNAMIC MEMORY MEASUREMENT
  printf("=== MEMORY ANALYSIS ===\n");
  printf("Measuring actual stack usage...\n");
  
  // Calibrate without alpha-beta
  enable_stack_tracking();
  move = findBestMoveMinimax_NoBitboard_NoPruning(board, 'X');
  size_t stack_no_ab = get_measured_stack_usage();
  disable_stack_tracking();
  
  // Calibrate with alpha-beta
  enable_stack_tracking();
  move = findBestMoveMinimax_NoBitboard(board, 'X', 0);
  size_t stack_with_ab = get_measured_stack_usage();
  disable_stack_tracking();
  
  printf("\nWithout Alpha-Beta:\n");
  printf("  Measured stack usage: %zu bytes\n", stack_no_ab);
  printf("  Typical max depth: 9 levels (full tree)\n");
  printf("  Frame size: ~%zu bytes\n\n", stack_no_ab / 9);
  
  printf("With Alpha-Beta:\n");
  printf("  Measured stack usage: %zu bytes\n", stack_with_ab);
  printf("  Typical max depth: 5 levels (pruned tree)\n");
  printf("  Frame size: ~%zu bytes\n", stack_with_ab / 5);
  
  long long diff = (long long)stack_no_ab - (long long)stack_with_ab;
  if (diff >= 0) {
      printf("  Memory saved: %lld bytes (%.1f%% reduction)\n\n",
             diff, 100.0 * (float)diff / (float)stack_no_ab);
  } else {
      printf("  Memory impact: %+lld bytes (%.1f%% increase)\n",
             diff, 100.0 * (float)(-diff) / (float)stack_no_ab);
  }
  
  printf("=======================\n\n");


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

  // MEMORY CALIBRATION
  printf("=== MEMORY ANALYSIS ===\n");
  printf("Calibrating stack usage...\n");
  
  // Calculate state sizes (portable via sizeof)
  size_t state_bitboard = sizeof(int) * 2;
  size_t state_array = sizeof(char) * 9;
  
  // Calibrate bitboard
  enable_stack_tracking();
  move = findBestMoveMinimax_Bitboard(board, 'X');
  size_t stack_bitboard = get_measured_stack_usage();
  disable_stack_tracking();
  
  // Calibrate array
  enable_stack_tracking();
  move = findBestMoveMinimax_NoBitboard(board, 'X', 0);
  size_t stack_array = get_measured_stack_usage();
  disable_stack_tracking();
  
  printf("\nBitboard representation:\n");
  printf("  State size: %zu bytes (2 x int)\n", state_bitboard);
  printf("  Frame size: ~%zu bytes\n", stack_bitboard / 5);
  printf("  Measured stack usage: %zu bytes (5 x %zu)\n\n", 
         stack_bitboard, stack_bitboard / 5);
  
  printf("Array representation:\n");
  printf("  State size: %zu bytes (9 x char)\n", state_array);
  printf("  Frame size: ~%zu bytes\n", stack_array / 5);
  printf("  Measured stack usage: %zu bytes (5 x %zu)\n\n",
         stack_array, stack_array / 5);
  
  printf("Comparison:\n");
  printf("  State memory saved: %zu byte (%.1f%% reduction)\n",
         state_array - state_bitboard,
         100.0 * (float)(state_array - state_bitboard) / (float)state_array);
  printf("  Stack memory saved: %d bytes (%.1f%% reduction)\n",
         (int)(stack_array - stack_bitboard),
         100.0 * (float)(stack_array - stack_bitboard) / (float)stack_array);
  printf("=======================\n\n");

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

  // DYNAMIC MEMORY MEASUREMENT
  printf("=== MEMORY ANALYSIS ===\n");
  printf("Measuring actual stack usage...\n");
  
  // Calibrate optimized (bitboard + alpha-beta)
  enable_stack_tracking();
  move = findBestMoveMinimax_Bitboard(board, 'X');
  size_t stack_optimized = get_measured_stack_usage();
  disable_stack_tracking();
  
  // Calibrate baseline (array, no alpha-beta)
  enable_stack_tracking();
  move = findBestMoveMinimax_NoBitboard_NoPruning(board, 'X');
  size_t stack_baseline = get_measured_stack_usage();
  disable_stack_tracking();
  
  size_t state_optimized = sizeof(int) * 2;
  size_t state_baseline = sizeof(char) * 9;
  
  printf("\nFully Optimized (Bitboard + Alpha-Beta):\n");
  printf("  State size: %zu bytes (2 x int)\n", state_optimized);
  printf("  Frame size: ~%zu bytes\n", stack_optimized / 5);
  printf("  Typical max depth: 5 levels\n");
  printf("  Measured stack usage: %zu bytes (5 x %zu)\n\n",
         stack_optimized, stack_optimized / 5);
  
  printf("Baseline (Array, No Optimizations):\n");
  printf("  State size: %zu bytes (9 x char)\n", state_baseline);
  printf("  Frame size: ~%zu bytes\n", stack_baseline / 9);
  printf("  Typical max depth: 9 levels\n");
  printf("  Measured stack usage: %zu bytes (9 x %zu)\n\n",
         stack_baseline, stack_baseline / 9);
  
  printf("Total Improvement:\n");
  printf("  State memory saved: %zu bytes (%.1f%% reduction)\n",
         state_baseline - state_optimized,
         100.0 * (float)(state_baseline - state_optimized) / (float)state_baseline);
          
  long long stack_diff = (long long)stack_baseline - (long long)stack_optimized;
  if (stack_diff >= 0) {
      printf("  Stack memory saved: %lld bytes (%.1f%% reduction)\n",
             stack_diff, 100.0 * (float)stack_diff / (float)stack_baseline);
  } else {
      printf("  Stack memory impact: %+lld bytes (%.1f%% increase)\n",
             stack_diff, 100.0 * (float)(-stack_diff) / (float)stack_baseline);
  }
  printf("=======================\n\n");

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
