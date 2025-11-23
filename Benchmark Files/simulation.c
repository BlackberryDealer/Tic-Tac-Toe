/**
 * @file simulation.c
 * @brief Benchmark simulation for AI difficulty modes
 *
 * This program benchmarks the 3 AI difficulty modes (Perfect, Imperfect, Model)
 * against a shallow depth minimax AI (the Imperfect AI, depth=5).
 * It strictly alternates starting players to ensure a fair difficulty assessment.
 *
 * COMPILATION:
 * gcc -o "Benchmark Files\simulation.exe" "Benchmark Files\simulation.c" "GUI_handlers\game_state.c" "Game_algorithms\Minimax.c" "Game_algorithms\model_minimax.c" "Game_algorithms\minimax_utils.c" -Ilib\raylib -Llib\raylib -lraylib -lopengl32 -lgdi32 -lwinmm -static -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../GUI_handlers/game_state.h"
#include "../Game_algorithms/minimax.h"

// Configuration
#define NUM_GAMES 10000  // Must be even for equal start distribution
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
    
    // Clear the board manually
    for(int r=0; r<3; r++) {
        for(int c=0; c<3; c++) {
            board[r][c] = ' ';
        }
    }
    
    // Reset global game winner for this game context
    game.winner = ' ';
    
    // Determine who moves first based on the function argument
    char current_turn = test_ai_starts ? TEST_AI_SYMBOL : BENCHMARK_AI_SYMBOL;
    
    // Loop until game over (Win or Draw)
    // CheckWinner returns true if there is a winner and sets game.winner
    // IsBoardFull returns true if draw and sets game.winner = ' '
    while (!CheckWinner(board) && !IsBoardFull(board)) {
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
                move = findBestMoveMinimax(tempBoard, TEST_AI_SYMBOL, 0);
            } else if (test_ai_mode == 2) {
                move = findBestMoveMinimax(tempBoard, TEST_AI_SYMBOL, 20);
            } else if (test_ai_mode == 3) {
                // Model AI is optimized for 'X'
                move = findBestMoveModel(tempBoard);
            }
        } else {
            // --- BENCHMARK AI TURN (Playing as O) ---
            // Uses Imperfect AI with 10% chance of play error as the benchmark
            move = findBestMoveMinimax(tempBoard, BENCHMARK_AI_SYMBOL, 10);
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
    
    // Determine Result based on game.winner set by CheckWinner/IsBoardFull
    if (game.winner == TEST_AI_SYMBOL) return 1;
    if (game.winner == BENCHMARK_AI_SYMBOL) return -1;
    
    return 0; // Draw
}

int main() {
    srand(time(NULL)); // Seed RNG for Imperfect AI randomness
    
    // Initialize minimal game state required for CheckWinner/IsBoardFull
    game.mode = MODE_TWO_PLAYER; // Use 2P mode so stats update for X and O generically
    
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
