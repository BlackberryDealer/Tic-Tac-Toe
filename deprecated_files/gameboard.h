// /**
//  * @file gameboard.h
//  * @brief Game board utilities and status checking for Tic-Tac-Toe
//  * 
//  * This module provides functions for displaying the game board,
//  * checking game status (win/draw/ongoing), and initializing the board.
//  * Used by both console and GUI versions of the game.
//  */

// #ifndef GAME_H
// #define GAME_H

// #include <stdbool.h>

// // ============================================================================
// // CONSTANTS
// // ============================================================================

// #define COMPUTER 1       /**< Player identifier for computer/AI */
// #define HUMAN 2          /**< Player identifier for human player */
// #define SIDE 3           /**< Board dimension (3x3 grid) */
// #define COMPUTERMOVE 'O' /**< Symbol used by computer/AI */
// #define HUMANMOVE 'X'    /**< Symbol used by human player */

// // ============================================================================
// // DATA STRUCTURES
// // ============================================================================

// /**
//  * @enum GameStatus
//  * @brief Represents the current state of the game
//  */
// typedef enum {
//     GAME_ONGOING = 0,    /**< Game is still in progress */
//     GAME_WIN = 1,        /**< A player has won */
//     GAME_DRAW = 2        /**< Game ended in a draw */
// } GameStatus;

// // ============================================================================
// // FUNCTION DECLARATIONS
// // ============================================================================

// /**
//  * @brief Display the game board in console format
//  * 
//  * Prints a formatted 3x3 grid showing the current board state.
//  * 
//  * @param board 3x3 game board array
//  */
// void showBoard(char board[][SIDE]);

// /**
//  * @brief Display game instructions in console format
//  * 
//  * Shows the numbered cell layout and basic game rules.
//  */
// void showInstructions(void);

// /**
//  * @brief Initialize the game board and moves array
//  * 
//  * Clears the board to empty state and randomizes the moves array
//  * for potential use in move selection algorithms.
//  * 
//  * @param board 3x3 game board array to initialize
//  * @param moves Array to store randomized move indices
//  */
// void initialise(char board[][SIDE], int moves[]);

// /**
//  * @brief Check if any row has three matching symbols
//  * 
//  * @param board 3x3 game board array
//  * @return 1 if a row is crossed, 0 otherwise
//  */
// int rowCrossed(char board[][SIDE]);

// /**
//  * @brief Check if any column has three matching symbols
//  * 
//  * @param board 3x3 game board array
//  * @return 1 if a column is crossed, 0 otherwise
//  */
// int columnCrossed(char board[][SIDE]);

// /**
//  * @brief Check if any diagonal has three matching symbols
//  * 
//  * @param board 3x3 game board array
//  * @return 1 if a diagonal is crossed, 0 otherwise
//  */
// int diagonalCrossed(char board[][SIDE]);

// /**
//  * @brief Legacy function to check if game is over
//  * 
//  * @deprecated Use gameStatus() instead for more precise status
//  * @param board 3x3 game board array
//  * @return Non-zero if game is over, 0 otherwise
//  */
// int gameOver(char board[][SIDE]);

// /**
//  * @brief Get precise game status (win/draw/ongoing)
//  * 
//  * Comprehensive check that determines if the game has a winner,
//  * is a draw, or is still ongoing.
//  * 
//  * @param board 3x3 game board array
//  * @return GameStatus enum value indicating current game state
//  */
// GameStatus gameStatus(char board[][SIDE]);

// #endif // GAME_H