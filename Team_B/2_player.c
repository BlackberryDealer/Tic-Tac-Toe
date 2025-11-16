/**
 * @file 2_player.c
 * @brief Two-player game mode implementation
 * 
 * This module implements the two-player game mode where two humans
 * take turns playing against each other on the same device.
 * Uses shared board display and status checking functions for consistency.
 * 
 * @author Team B
 * @date 2025
 */

#include <stdio.h>
#include "2_player.h"
#include "gameboard.h"

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

/**
 * @brief Display winner announcement for two-player mode
 * 
 * @param playerNumber Player number (1 or 2) who won
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

// ============================================================================
// MAIN GAME FUNCTION
// ============================================================================

void playTwoPlayer()
{
    // Initialize game board
    char board[SIDE][SIDE];
    int moves[SIDE * SIDE];
    initialise(board, moves);
    showInstructions();

    // Initialize game state
    int turn = 0;        // Even = Player 1 (X), Odd = Player 2 (O)
    int row, col;
    int move;
    int moveIndex = 0;   // Defensive move counter

    // ========================================================================
    // MAIN GAME LOOP
    // ========================================================================
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
