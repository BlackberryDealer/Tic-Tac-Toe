/*
 * Two-player mode. Uses the shared showBoard and gameOver helpers
 * so presentation and win detection stay consistent with single-player.
 */

#include <stdio.h>
#include "2_player.h"
#include "gameboard.h"

// Two-player specific winner declaration
void declareWinnerTwoPlayer(int playerNumber)
{
    if (playerNumber == 1)
        printf("Player 1 has won!!!\n");
    else if (playerNumber == 2)
        printf("Player 2 has won!!!\n");
    else
        printf("Player %d has won!!!\n", playerNumber);
}

void playTwoPlayer()
{
    char board[SIDE][SIDE];
    int moves[SIDE * SIDE];
    initialise(board, moves);

    int turn = 0;
    int row, col;
    int move;

    showInstructions();

    while (1) {
        printf("Player %d, enter your move (1-9): ", (turn % 2) + 1);
        if (scanf("%d", &move) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (move < 1 || move > 9) {
            printf("Invalid input! Please enter a number between 1 and 9.\n");
            continue;
        }

        row = (move - 1) / SIDE;
        col = (move - 1) % SIDE;

        if (board[row][col] != ' ') {
            printf("Cell %d is already occupied. Try again.\n", move);
            continue;
        }

        board[row][col] = (turn % 2 == 0) ? 'X' : 'O';
        turn++;

        showBoard(board);

        GameStatus status = gameStatus(board);
        if (status == GAME_WIN) {
            int last = (turn - 1) % 2; /* 0 -> player1 (X), 1 -> player2 (O) */
            declareWinnerTwoPlayer(last == 0 ? 1 : 2);
            return;
        }
        if (status == GAME_DRAW) {
            printf("The game is a DRAW\n");
            return;
        }

        /* otherwise continue to next player's turn */
    }
}
