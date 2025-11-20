/**
 * @file gameboard.c
 * @brief Game board display and status checking implementation
 * 
 * This file implements console-based board display and game status
 * checking functions. Used by both console and GUI versions of the game.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameboard.h"
#include "minimax.h"

// ============================================================================
// BOARD DISPLAY FUNCTIONS
// ============================================================================

void showBoard(char board[][SIDE])
{
    printf("\n\n");
    printf("\t\t\t %c | %c | %c \n", board[0][0],
           board[0][1], board[0][2]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t %c | %c | %c \n", board[1][0],
           board[1][1], board[1][2]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t %c | %c | %c \n\n", board[2][0],
           board[2][1], board[2][2]);
}

void showInstructions()
{
    printf("\t\t\t Tic-Tac-Toe\n\n");
    printf("Choose a cell numbered from 1 to 9 as below and play\n\n");

    printf("\t\t\t 1 | 2 | 3 \n");
    printf("\t\t\t--------------\n");
    printf("\t\t\t 4 | 5 | 6 \n");
    printf("\t\t\t--------------\n");
    printf("\t\t\t 7 | 8 | 9 \n\n");

    printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t\n\n");
}

// ============================================================================
// BOARD INITIALIZATION
// ============================================================================

void initialise(char board[][SIDE], int moves[])
{

    // Initially, the board is empty (use space for printing)
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++)
            board[i][j] = ' ';
    }

    // Fill the moves with numbers
    for (int i = 0; i < SIDE * SIDE; i++)
        moves[i] = i;

    // Randomize the moves
    for (int i = 0; i < SIDE * SIDE; i++) {
        int randIndex = rand() % (SIDE * SIDE);
        int temp = moves[i];
        moves[i] = moves[randIndex];
        moves[randIndex] = temp;
    }
}

// ============================================================================
// WIN CONDITION CHECKING FUNCTIONS
// ============================================================================

int rowCrossed(char board[][SIDE])
{
    for (int i = 0; i < SIDE; i++) {
        if (board[i][0] == board[i][1]
            && board[i][1] == board[i][2]
            && board[i][0] != ' ')
            return 1;
    }
    return 0;
}

int columnCrossed(char board[][SIDE])
{
    for (int i = 0; i < SIDE; i++) {
        if (board[0][i] == board[1][i]
            && board[1][i] == board[2][i]
            && board[0][i] != ' ')
            return 1;
    }
    return 0;
}

int diagonalCrossed(char board[][SIDE])
{
    if ((board[0][0] == board[1][1]
         && board[1][1] == board[2][2]
         && board[0][0] != ' ')
        || (board[0][2] == board[1][1]
            && board[1][1] == board[2][0]
            && board[0][2] != ' '))
        return 1;

    return 0;
}

/**
 * @brief Check if board is completely filled (no empty cells)
 * 
 * @param board 3x3 game board array
 * @return true if board is full, false otherwise
 */
static bool isBoardFull(char board[][SIDE])
{
    for (int r = 0; r < SIDE; r++) {
        for (int c = 0; c < SIDE; c++) {
            if (board[r][c] == ' ')
                return false;
        }
    }
    return true;
}

GameStatus gameStatus(char board[][SIDE])
{
    if (rowCrossed(board) || columnCrossed(board) || diagonalCrossed(board))
        return GAME_WIN;

    if (isBoardFull(board))
        return GAME_DRAW;

    return GAME_ONGOING;
}

