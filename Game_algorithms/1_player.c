/**
 * @file 1_player.c
 * @brief Single-player game mode implementation
 * 
 * This module implements the one-player game mode where a human
 * plays against an AI opponent. Supports multiple AI difficulty levels
 * (Perfect, Imperfect, Model-based).
 */

#include <stdio.h>
#include <stdlib.h>
#include "1_player.h"
#include "gameboard.h"
#include "minimax.h"

// ============================================================================
// CONSTANTS
// ============================================================================

#define EMPTY ' '   /**< Character representing an empty board cell */

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

/**
 * @brief Display winner announcement
 * 
 * @param whoseTurn COMPUTER (1) or HUMAN (2) - the winner
 */
void declareWinner(int whoseTurn)
{
    if (whoseTurn == COMPUTER)
        printf("COMPUTER has won!!!\n");
    else
        printf("HUMAN has won!!!\n");
}

// ============================================================================
// MAIN GAME FUNCTION
// ============================================================================

void playOnePlayer()
{
    // Initialize game board and moves array
    char board[SIDE][SIDE];
    int moves[SIDE * SIDE];
    initialise(board, moves);
    showInstructions();

    // Prompt user for AI difficulty selection
    int difficulty;
    while (1) {
        printf("Choose AI difficulty:\n");
        printf("1. Perfect (always optimal)\n");
        printf("2. Imperfect (can make mistakes)\n");
        printf("3. Model (trained ML, can make different mistakes)\n"); 
        printf("\nEnter choice: ");
        
        if (scanf("%d", &difficulty) != 1) {
            // Clear invalid input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Please enter 1, 2, or 3.\n");
            continue;
        }
        if (difficulty == 1 || difficulty == 2 || difficulty == 3) {
            break;
        }
        printf("Invalid choice. Please enter 1, 2, or 3.\n");
    }

    // Initialize game state
    int moveIndex = 0, x, y;
    int whoseTurn = COMPUTER;  // Computer always starts first

    // ========================================================================
    // MAIN GAME LOOP
    // ========================================================================
    while (1) {
        if (whoseTurn == COMPUTER) {
            // ================================================================
            // COMPUTER'S TURN (AI MOVE)
            // ================================================================
            
            // Convert board to format expected by minimax functions
            // Use SIDE constant for consistency if board size changes
            char tempBoard[SIDE][SIDE];
            for (int i = 0; i < SIDE; i++) {
                for (int j = 0; j < SIDE; j++) {
                    if (board[i][j] == 'X') {
                        tempBoard[i][j] = 'X';
                    } else if (board[i][j] == 'O') {
                        tempBoard[i][j] = 'O';
                    } else {
                        tempBoard[i][j] = EMPTY;
                    }
                }
            }

            // Select AI algorithm based on difficulty
            struct Move thisMove;
            if (difficulty == 1) {
                // Perfect AI - unbeatable, uses alpha-beta pruning
                thisMove = findBestMovePerfect(tempBoard, COMPUTERMOVE);
            } else if (difficulty == 2) {
                // Imperfect AI - makes mistakes, depth-limited
                thisMove = findBestMoveImperfect(tempBoard, COMPUTERMOVE);
            } else if (difficulty == 3) {
                // Model-based AI - uses logistic regression
                thisMove = findBestMoveModel(tempBoard);
            }
            
            // Extract move coordinates
            x = thisMove.row;
            y = thisMove.col;

            // Defensive check: if AI returned invalid move, pick first empty cell
            if (x < 0 || x >= SIDE || y < 0 || y >= SIDE || board[x][y] != EMPTY) {
                int found = 0;
                for (int i = 0; i < SIDE && !found; i++) {
                    for (int j = 0; j < SIDE; j++) {
                        if (board[i][j] == EMPTY) {
                            x = i;
                            y = j;
                            found = 1;
                            break;
                        }
                    }
                }
                if (!found) {
                    // No empty cell - declare draw
                    printf("It's a DRAW\n");
                    return;
                }
            }

            // Apply computer move
            board[x][y] = COMPUTERMOVE;
            printf("COMPUTER has put a %c in cell %d %d\n", COMPUTERMOVE, x, y);
            showBoard(board);
            moveIndex++;

            // Check game status after computer move
            GameStatus status = gameStatus(board);
            if (status == GAME_WIN) {
                declareWinner(COMPUTER);
                return;
            } else if (status == GAME_DRAW) {
                printf("It's a DRAW\n");
                return;
            }

            // Switch turn to human
            whoseTurn = HUMAN;
        } else {
            // ================================================================
            // HUMAN'S TURN
            // ================================================================
            int move;
            printf("Enter your move (1-9): ");
            if (scanf("%d", &move) != 1) {
                // Clear invalid input buffer
                int c; while ((c = getchar()) != '\n' && c != EOF) {}
                printf("Invalid input!\n");
                continue;
            }
            if (move < 1 || move > SIDE * SIDE) {
                printf("Invalid input! Please enter a number between 1 and %d.\n", SIDE * SIDE);
                continue;
            }

            // Convert user input (1–9) to board coordinates
            x = (move - 1) / SIDE;
            y = (move - 1) % SIDE;

            // Place human move if cell is empty
            if (board[x][y] == EMPTY) {
                board[x][y] = HUMANMOVE;
                showBoard(board);
                moveIndex++;

                // Check game status after human move
                GameStatus status = gameStatus(board);
                if (status == GAME_WIN) {
                    declareWinner(HUMAN);
                    return;
                } else if (status == GAME_DRAW) {
                    printf("It's a DRAW\n");
                    return;
                }

                // Switch turn back to computer
                whoseTurn = COMPUTER;
            } else {
                printf("Cell %d is already occupied. Try again.\n", move);
            }
        }

        // Safety check: if all cells filled but no winner, declare draw
        if (moveIndex >= SIDE * SIDE) {
            printf("It's a DRAW\n");
            return;
        }
    }
}
