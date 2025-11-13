/*
 * playOnePlayer: thin wrapper that uses the modular playTicTacToe
 * implementation (computer vs human) from game.c
 */

#include <stdio.h>
#include "1_player.h"
#include "gameboard.h"
#include "minimax.h"

// Declare winner depending on whoseTurn (COMPUTER or HUMAN)
void declareWinner(int whoseTurn)
{
    if (whoseTurn == COMPUTER)
        printf("COMPUTER has won\n");
    else
        printf("HUMAN has won\n");
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
    printf("Choose AI difficulty:\n");
    printf("1. Perfect (always optimal)\n");
    printf("2. Imperfect (can make mistakes)\n");
    printf("Enter choice: ");
    scanf("%d", &difficulty);

    // Track move index and whose turn it is
    int moveIndex = 0, x, y;
    int whoseTurn = COMPUTER;  // Computer always starts first

    // Main game loop
    while (1) {
        if (whoseTurn == COMPUTER) {
            // Convert board to tempBoard for minimax functions
            char tempBoard[3][3];
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == 'X') tempBoard[i][j] = 'X';
                    else if (board[i][j] == 'O') tempBoard[i][j] = 'O';
                    else tempBoard[i][j] = '_'; // empty cell
                }
            }

            // Select move depending on difficulty chosen
            struct Move thisMove;
            if (difficulty == 1)
                thisMove = findBestMovePerfect(tempBoard);   // Perfect AI
            else
                thisMove = findBestMoveImperfect(tempBoard); // Imperfect AI

            // Apply computer move to board
            x = thisMove.row;
            y = thisMove.col;
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
                printf("It's a draw\n");
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
            if (move < 1 || move > 9) {
                printf("Invalid input! Please enter a number between 1 and 9.\n");
                continue;
            }

            // Convert user input (1–9) to board coordinates
            x = (move - 1) / SIDE;
            y = (move - 1) % SIDE;

            // Place human move if cell is empty
            if (board[x][y] == ' ') {
                board[x][y] = HUMANMOVE;
                showBoard(board);
                moveIndex++;

                // Check game status after human move
                GameStatus status = gameStatus(board);
                if (status == GAME_WIN) {
                    declareWinner(HUMAN);
                    return;
                } else if (status == GAME_DRAW) {
                    printf("It's a draw\n");
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
            printf("It's a draw\n");
            return;
        }
    }
}