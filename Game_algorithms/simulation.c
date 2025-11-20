/**
 * @file simulation.c
 * @brief Benchmark simulation for AI difficulty modes
 *
 * This program benchmarks the 3 AI difficulty modes (Perfect, Imperfect, Model)
 * against a shallow depth minimax AI (the Imperfect AI, depth=5).
 * It strictly alternates starting players to ensure a fair difficulty assessment.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameboard.h"
#include "minimax.h"

// Configuration
#define NUM_GAMES 100000  // Must be even for equal start distribution
#define TEST_AI_SYMBOL 'X'
#define BENCHMARK_AI_SYMBOL 'O'

/**
 * @brief Play a single game between Test AI and Benchmark AI
 * 
 * @param test_ai_mode 1=Perfect, 2=Imperfect, 3=Model
 * @param test_ai_starts 1 if Test AI (X) moves first, 0 if Benchmark AI (O) moves first
 * @return 1 if Test AI wins, -1 if Benchmark AI wins, 0 for Draw
 */
int play_game(int test_ai_mode, int test_ai_starts) {
    // Initialize local board copy to prevent state leakage
    char board[3][3];
    int moves[9]; // Required by initialise but not used here
    initialise(board, moves);
    
    // Determine who moves first based on the function argument
    char current_turn = test_ai_starts ? TEST_AI_SYMBOL : BENCHMARK_AI_SYMBOL;
    
    while (gameStatus(board) == GAME_ONGOING) {
        struct Move move = {-1, -1};
        
        // Create a temporary board for AI functions (preserves original board safety)
        char tempBoard[3][3];
        for(int r=0; r<3; r++) {
            for(int c=0; c<3; c++) {
                tempBoard[r][c] = board[r][c];
            }
        }

        if (current_turn == TEST_AI_SYMBOL) {
            // --- TEST AI TURN (Playing as X) ---
            if (test_ai_mode == 1) {
                move = findBestMovePerfect(tempBoard, TEST_AI_SYMBOL);
            } else if (test_ai_mode == 2) {
                move = findBestMoveImperfect(tempBoard, TEST_AI_SYMBOL);
            } else if (test_ai_mode == 3) {
                // Model AI is optimized for 'X'
                move = findBestMoveModel(tempBoard);
            }
        } else {
            // --- BENCHMARK AI TURN (Playing as O) ---
            // Uses Imperfect AI (Shallow Depth Minimax) as the benchmark
            move = findBestMoveImperfect(tempBoard, BENCHMARK_AI_SYMBOL);
        }
        
        // Apply the move
        if (move.row >= 0 && move.row < 3 && move.col >= 0 && move.col < 3 && board[move.row][move.col] == ' ') {
            board[move.row][move.col] = current_turn;
        } else {
            // Fallback for invalid moves (fills first empty slot)
            int found = 0;
            for (int r = 0; r < 3 && !found; r++) {
                for (int c = 0; c < 3; c++) {
                    if (board[r][c] == ' ') {
                        board[r][c] = current_turn;
                        found = 1;
                        break;
                    }
                }
            }
        }
        
        // Switch turn
        current_turn = (current_turn == 'X') ? 'O' : 'X';
    }
    
    // Determine Result
    GameStatus status = gameStatus(board);
    if (status == GAME_WIN) {
        // If the game ended in a win, the "current_turn" has already switched to the loser
        // So the winner is the previous player.
        // Actually, gameStatus checks the board. We need to see who has 3 in a row.
        
        // Helper check specifically for symbols
        int x_wins = 0, o_wins = 0;
        // Check rows/cols/diags manually to be sure or use helper logic if gameStatus doesn't return winner
        // Since gameStatus returns GAME_WIN, we check who actually won.
        // (Re-using simple win check logic for safety as gameStatus return value is generic)
        
        // Check if Test AI (X) won
        for(int i=0; i<3; i++) {
            if(board[i][0]=='X' && board[i][1]=='X' && board[i][2]=='X') x_wins=1;
            if(board[0][i]=='X' && board[1][i]=='X' && board[2][i]=='X') x_wins=1;
        }
        if(board[0][0]=='X' && board[1][1]=='X' && board[2][2]=='X') x_wins=1;
        if(board[0][2]=='X' && board[1][1]=='X' && board[2][0]=='X') x_wins=1;

        if(x_wins) return 1;
        else return -1; // If GAME_WIN and X didn't win, O must have won
    }
    
    return 0; // Draw
}

int main() {
    srand(time(NULL)); // Seed RNG for Imperfect AI randomness
    
    // Ensure even number of games for equal starts
    int total_games = NUM_GAMES;
    if (total_games % 2 != 0) total_games++;

    printf("================================================================\n");
    printf("AI DIFFICULTY BENCHMARK SIMULATION\n");
    printf("----------------------------------------------------------------\n");
    printf("Opponent: Shallow Depth Minimax (Imperfect AI, Depth=5)\n");
    printf("Total Games: %d (%d starts each)\n", total_games, total_games/2);
    printf("Test AI: 'X' | Benchmark AI: 'O'\n");
    printf("================================================================\n\n");
    
    const char* mode_names[] = {"", "Perfect (Hard)", "Imperfect (Medium)", "Model (Easy)"};
    
    for (int mode = 1; mode <= 3; mode++) {
        int wins = 0;
        int losses = 0;
        int draws = 0;
        
        for (int i = 0; i < total_games; i++) {
            // i % 2 == 0 -> test_ai_starts = 1 (Test AI moves first)
            // i % 2 == 1 -> test_ai_starts = 0 (Benchmark AI moves first)
            int test_ai_starts = ((i % 2) == 0);
            
            int result = play_game(mode, test_ai_starts);
            
            if (result == 1) wins++;
            else if (result == -1) losses++;
            else draws++;
        }
        
        printf("Mode: %-20s\n", mode_names[mode]);
        printf("--------------------------------\n");
        printf("Wins:   %4d (%5.1f%%)\n", wins, (double)wins/total_games*100.0);
        printf("Losses: %4d (%5.1f%%)\n", losses, (double)losses/total_games*100.0);
        printf("Draws:  %4d (%5.1f%%)\n", draws, (double)draws/total_games*100.0);
        printf("\n");
    }
    
    return 0;
}
