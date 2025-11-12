#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameboard.h"
#include "PERFECT_minimax.h"

// Function to display the game board
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

// Function to show the instructions
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

// Function to initialise the game
void initialise(char board[][SIDE], int moves[])
{
    srand((unsigned)time(NULL));

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

// Function to check if any row is crossed with the same player's move
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

// Function to check if any column is crossed with the same player's move
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

// Function to check if any diagonal is crossed with the same player's move
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

/* helper: check if board is full (no spaces) */
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

/* return GAME_WIN if any win-line exists,
   GAME_DRAW if board full with no winner,
   GAME_ONGOING otherwise */
GameStatus gameStatus(char board[][SIDE])
{
    if (rowCrossed(board) || columnCrossed(board) || diagonalCrossed(board))
        return GAME_WIN;

    if (isBoardFull(board))
        return GAME_DRAW;

    return GAME_ONGOING;
}

