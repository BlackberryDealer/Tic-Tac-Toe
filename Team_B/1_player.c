/*
 * playOnePlayer: thin wrapper that uses the modular playTicTacToe
 * implementation (computer vs human) from game.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "1_player.h"
#include "gameboard.h"
#include "minimax.h"

#define EMPTY ' '   // single definition for an empty board cell

// Declare winner depending on whoseTurn (COMPUTER or HUMAN)
void declareWinner(int whoseTurn)
{
    if (whoseTurn == COMPUTER)
        printf("COMPUTER has won!!!\n");
    else
        printf("HUMAN has won!!!\n");
}

void playOnePlayer()
{
    // Game board and moves array
    char board[SIDE][SIDE];
    int moves[SIDE * SIDE];

    // Initialize empty board and moves list
    initialise(board, moves);
    showInstructions();

    // Ask user to choose AI difficulty
    int difficulty;
    while (1) {
        printf("Choose AI difficulty:\n");
        printf("1. Perfect (always optimal)\n");
        printf("2. Imperfect (can make mistakes)\n");
        printf("3. Model (trained ML, can make different mistakes)\n"); 
    printf("\nEnter choice: ");
        if (scanf("%d", &difficulty) != 1) {
            // Clear invalid input buffer
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Please enter 1 or 2.\n");
            continue;
        }
        if (difficulty == 1 || difficulty == 2 || difficulty == 3) break;
        printf("Invalid choice. Please enter 1,  2 or 3.\n");
    }

    // Track move index and whose turn it is
    int moveIndex = 0, x, y;
    int whoseTurn = COMPUTER;  // Computer always starts first

    // Main game loop
    while (1) {
        if (whoseTurn == COMPUTER) {
            // Convert board to tempBoard for minimax functions
            // Use SIDE so this code remains consistent if board size changes
            char tempBoard[SIDE][SIDE];
            for (int i = 0; i < SIDE; i++) {
                for (int j = 0; j < SIDE; j++) {
                    if (board[i][j] == 'X') tempBoard[i][j] = 'X';
                    else if (board[i][j] == 'O') tempBoard[i][j] = 'O';
                    else tempBoard[i][j] = EMPTY; // empty cell
                }
            }

            // Select move depending on difficulty chosen
            struct Move thisMove;
            if (difficulty == 1)
                thisMove = findBestMovePerfect(tempBoard);   // Perfect AI
            else if (difficulty == 2)
                thisMove = findBestMoveImperfect(tempBoard); // Imperfect AI
            else if (difficulty == 3)
                thisMove = findBestMoveModel(tempBoard);
            // Apply computer move to board
            x = thisMove.row;
            y = thisMove.col;

            // Defensive check: if minimax returned invalid move, pick first empty cell
            if (x < 0 || x >= SIDE || y < 0 || y >= SIDE || board[x][y] != EMPTY) {
                int found = 0;
                for (int i = 0; i < SIDE && !found; i++) {
                    for (int j = 0; j < SIDE; j++) {
                        if (board[i][j] == EMPTY) {
                            x = i; y = j; found = 1; break;
                        }
                    }
                }
                if (!found) { // no empty cell, declare draw and return
                    printf("It's a DRAW\n");
                    return;
                }
            }

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
        } else { // HUMAN turn
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
