/*
 * playOnePlayer: thin wrapper that uses the modular playTicTacToe
 * implementation (computer vs human) from game.c
 */

#include <stdio.h>
#include "1_player.h"
#include "gameboard.h"
#include "PERFECT_minimax.h"

// One-player specific winner declaration (COMPUTER vs HUMAN)
void declareWinner(int whoseTurn)
{
    if (whoseTurn == COMPUTER)
        printf("COMPUTER has won\n");
    else
        printf("HUMAN has won\n");
}

void playOnePlayer()
{
    // Single-player loop: COMPUTER vs HUMAN (computer starts)
    char board[SIDE][SIDE];
    int moves[SIDE * SIDE];

    initialise(board, moves);
    showInstructions();

    int moveIndex = 0, x, y;
    int whoseTurn = COMPUTER;

    while (1) {
        if (whoseTurn == COMPUTER) {
            char tempBoard[3][3];
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == 'X') tempBoard[i][j] = 'X';
                    else if (board[i][j] == 'O') tempBoard[i][j] = 'O';
                    else tempBoard[i][j] = '_';
                }
            }
            struct Move thisMove = findBestMove(tempBoard);
            x = thisMove.row;
            y = thisMove.col;

            board[x][y] = COMPUTERMOVE;
            printf("COMPUTER has put a %c in cell %d %d\n", COMPUTERMOVE, x, y);
            showBoard(board);
            moveIndex++;

            // check status after computer move
            GameStatus status = gameStatus(board);
            if (status == GAME_WIN) {
                declareWinner(COMPUTER);
                return;
            } else if (status == GAME_DRAW) {
                printf("It's a draw\n");
                return;
            }

            whoseTurn = HUMAN;
        } else { // HUMAN
            int move;
            printf("Enter your move (1-9): ");
            if (scanf("%d", &move) != 1) {
                int c; while ((c = getchar()) != '\n' && c != EOF) {}
                printf("Invalid input!\n");
                continue;
            }
            if (move < 1 || move > 9) {
                printf("Invalid input! Please enter a number between 1 and 9.\n");
                continue;
            }
            x = (move - 1) / SIDE;
            y = (move - 1) % SIDE;
            if (board[x][y] == ' ') {
                board[x][y] = HUMANMOVE;
                showBoard(board);
                moveIndex++;

                // check status after human move
                GameStatus status = gameStatus(board);
                if (status == GAME_WIN) {
                    declareWinner(HUMAN);
                    return;
                } else if (status == GAME_DRAW) {
                    printf("It's a draw\n");
                    return;
                }

                whoseTurn = COMPUTER;
            } else {
                printf("Cell %d is already occupied. Try again.\n", move);
            }
        }

        // safety: in case gameStatus wasn't reached for some reason
        if (moveIndex >= SIDE * SIDE) {
            printf("It's a draw\n");
            return;
        }
    }
}