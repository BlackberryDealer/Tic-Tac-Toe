#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define COMPUTER 1
#define HUMAN 2
#define SIDE 3
#define COMPUTERMOVE 'O'
#define HUMANMOVE 'X'

// ---------------- Intelligent Moves start (Team_B minimax implementation)
#include "Team_B/minimax_core.h"

// -----------------------------------Intelligent Moves end

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
    printf("Choose a cell numbered from 1 to 9 as below "
           "and play\n\n");

    printf("\t\t\t 1 | 2 | 3 \n");
    printf("\t\t\t--------------\n");
    printf("\t\t\t 4 | 5 | 6 \n");
    printf("\t\t\t--------------\n");
    printf("\t\t\t 7 | 8 | 9 \n\n");

    printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n\n");
}

// Function to initialise the game
void initialise(char board[][SIDE], int moves[])
{
    srand(time(NULL));

    // Initially, the board is empty
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

// Function to declare the winner of the game
void declareWinner(int whoseTurn)
{
    if (whoseTurn == COMPUTER)
        printf("COMPUTER has won\n");
    else
        printf("HUMAN has won\n");
}

// Function to check if any row is crossed with the same
// player's move
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

// Function to check if any column is crossed with the same
// player's move
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

// Function to check if any diagonal is crossed with the
// same player's move
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

// Function to check if the game is over
int gameOver(char board[][SIDE])
{
    return (rowCrossed(board) || columnCrossed(board)
            || diagonalCrossed(board));
}

// Function to play Tic-Tac-Toe
void playTicTacToe(int whoseTurn)
{
    // A 3*3 Tic-Tac-Toe board for playing
    char board[SIDE][SIDE];
    int moves[SIDE * SIDE];

    // Initialise the game
    initialise(board, moves);

    // Show the instructions before playing
    showInstructions();

    int moveIndex = 0, x, y;
    int difficulty = DIFF_HARD; // default

    // Ask player for difficulty level once at start
    printf("Select AI difficulty: 0=EASY, 1=MEDIUM, 2=HARD (default 2): ");
    if (scanf("%d", &difficulty) != 1 || difficulty < 0 || difficulty > 2) {
        difficulty = DIFF_HARD;
        // clear stdin if needed
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
    }

    // Keep playing until the game is over or it is a draw
    while (!gameOver(board) && moveIndex != SIDE * SIDE) {
        if (whoseTurn == COMPUTER) {

            char tempBoard[3][3];
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == 'X') {
                        tempBoard[i][j] = 'x';
                    }
                    else if (board[i][j] == 'O') {
                        tempBoard[i][j] = 'o';
                    }
                    else {
                        tempBoard[i][j] = '_';
                    }
                }
            }
            // Ensure minimax globals reflect that the AI is playing 'o' and human is 'x'
            player = 'o';
            opponent = 'x';
            struct Move thisMove = findBestMoveWithDifficulty(tempBoard, difficulty);
            x = thisMove.row;
            y = thisMove.col;

            board[x][y] = COMPUTERMOVE;
            printf("COMPUTER has put a %c in cell %d %d\n",
                   COMPUTERMOVE, x, y);
            showBoard(board);
            moveIndex++;
            whoseTurn = HUMAN;
        }
        else if (whoseTurn == HUMAN) {
            int move;
            printf("Enter your move (1-9): ");
            scanf("%d", &move);
            if (move < 1 || move > 9) {
                printf("Invalid input! Please enter a "
                       "number between 1 and 9.\n");
                continue;
            }
            x = (move - 1) / SIDE;
            y = (move - 1) % SIDE;
            if (board[x][y] == ' ') {
                board[x][y] = HUMANMOVE;
                showBoard(board);
                moveIndex++;
                if (gameOver(board)) {
                    declareWinner(HUMAN);
                    return;
                }
                whoseTurn = COMPUTER;
            }
            else {
                printf("Cell %d is already occupied. Try "
                       "again.\n",
                       move);
            }
        }
    }

    // If the game has drawn
    if (!gameOver(board) && moveIndex == SIDE * SIDE)
        printf("It's a draw\n");
    else {
        // Toggling the user to declare the actual winner
        if (whoseTurn == COMPUTER)
            whoseTurn = HUMAN;
        else if (whoseTurn == HUMAN)
            whoseTurn = COMPUTER;

        // Declare the winner
        declareWinner(whoseTurn);
    }
}

// Driver program
int main()
{
    // Let us play the game with COMPUTER starting first
    playTicTacToe(COMPUTER);

    return 0;
}