#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "minimax.h"
#include "minimax_utils.h"
#include "model_minimax.h"

// Helper to print board
void printBoard(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
        if (i < 2) printf("---|---|---\n");
    }
    printf("\n");
}

int main() {
    printf("==================================================\n");
    printf("TIC-TAC-TOE AI BENCHMARK\n");
    printf("==================================================\n\n");

    // 1. Setup Empty Board (Worst Case for Search)
    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    clock_t start, end;
    double cpu_time_used;
    struct Move move;

    int iterations = 10000;
    
    // ========================================================================
    // TEST 1: MINIMAX (HARD / PERFECT)
    // ========================================================================
    printf("TEST 1: Minimax (Hard/Perfect) - errorRate = 0\n");
    printf("----------------------------------------------\n");
    
    g_max_depth_reached = 0; // Reset depth counter
    start = clock();
    
    for(int i=0; i<iterations; i++) {
        move = findBestMoveMinimax(board, 'X', 0);
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Best Move: (%d, %d)\n", move.row, move.col);
    printf("Total Time (%d runs): %.6f seconds\n", iterations, cpu_time_used);
    printf("Avg Time per Move: %.8f seconds\n", cpu_time_used / iterations);
    printf("Max Recursion Depth: %d\n", g_max_depth_reached);
    printf("Est. Stack Usage: ~%d bytes (assuming 64 bytes/frame)\n\n", g_max_depth_reached * 64);


    // ========================================================================
    // TEST 2: MINIMAX (MEDIUM / IMPERFECT)
    // ========================================================================
    printf("TEST 2: Minimax (Medium/Imperfect) - errorRate = 20\n");
    printf("---------------------------------------------------\n");
    
    g_max_depth_reached = 0; // Reset depth counter
    start = clock();
    
    for(int i=0; i<iterations; i++) {
        move = findBestMoveMinimax(board, 'X', 20);
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Best Move: (%d, %d)\n", move.row, move.col);
    printf("Total Time (%d runs): %.6f seconds\n", iterations, cpu_time_used);
    printf("Avg Time per Move: %.8f seconds\n", cpu_time_used / iterations);
    printf("Max Recursion Depth: %d\n", g_max_depth_reached);
    printf("Est. Stack Usage: ~%d bytes\n\n", g_max_depth_reached * 64);


    // ========================================================================
    // TEST 3: LOGISTIC REGRESSION (EASY / MODEL)
    // ========================================================================
    printf("TEST 3: Logistic Regression (Easy/Model)\n");
    printf("----------------------------------------\n");
    
    start = clock();
    
    for(int i=0; i<iterations; i++) {
        move = findBestMoveModel(board);
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Best Move: (%d, %d)\n", move.row, move.col);
    printf("Total Time (%d runs): %.6f seconds\n", iterations, cpu_time_used);
    printf("Avg Time per Move: %.8f seconds\n", cpu_time_used / iterations);
    printf("Max Recursion Depth: 0 (Iterative)\n");
    printf("Est. Stack Usage: Constant (Low)\n\n");

    return 0;
}
