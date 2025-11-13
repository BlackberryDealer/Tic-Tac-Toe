/*
 * Two-player mode. Uses the shared showBoard and gameStatus helpers
 * so presentation and win detection stay consistent with single-player.
 */

#include <stdio.h>
#include "2_player.h"
#include "gameboard.h"

/*
 * declareWinnerTwoPlayer
 * - Purpose: Print which player won the two-player match.
 */
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

    initialise(board, moves);   // prepare board (initialise should set empty cells)

    int turn = 0;               // even -> Player 1 (HUMANMOVE / 'X'), odd -> Player 2 (COMPUTERMOVE / 'O')
    int row, col;
    int move;
    int moveIndex = 0;          // defensive move counter

    showInstructions();

    while (1) {
        printf("Player %d, enter your move (1-%d): ", (turn % 2) + 1, SIDE * SIDE);

        if (scanf("%d", &move) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {} // clear stdin
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (move < 1 || move > SIDE * SIDE) {
            printf("Invalid input! Please enter a number between 1 and %d.\n", SIDE * SIDE);
            continue;
        }

        /* Convert 1-based move to 0-based board coordinates */
        row = (move - 1) / SIDE;
        col = (move - 1) % SIDE;

        /* Treat a cell as empty if it is neither HUMANMOVE nor COMPUTERMOVE */
        if (board[row][col] == HUMANMOVE || board[row][col] == COMPUTERMOVE) {
            printf("Cell %d is already occupied. Try again.\n", move);
            continue;
        }

        /* Place the current player's symbol using the same symbols as single-player */
        board[row][col] = (turn % 2 == 0) ? HUMANMOVE : COMPUTERMOVE;
        turn++;
        moveIndex++;

        showBoard(board);

        /* Check game status using shared helper */
        GameStatus status = gameStatus(board);
        if (status == GAME_WIN) {
            int last = (turn - 1) % 2; /* 0 -> player1 (HUMANMOVE), 1 -> player2 (COMPUTERMOVE) */
            declareWinnerTwoPlayer(last == 0 ? 1 : 2);
            return;
        }
        if (status == GAME_DRAW) {
            printf("The game is a DRAW\n");
            return;
        }

        /* Defensive safety: if board is full but gameStatus missed it, declare draw */
        if (moveIndex >= SIDE * SIDE) {
            printf("The game is a DRAW\n");
            return;
        }
    }
}
