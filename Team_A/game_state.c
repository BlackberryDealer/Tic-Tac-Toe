#include "game_state.h"
#include "../Team_B/minimax.h"

// UI Colors definitions
Color colorPrimary = {52, 152, 219, 255};
Color colorSecondary = {46, 204, 113, 255};
Color colorAccent = {231, 76, 60, 255};
Color colorWarning = {241, 196, 15, 255};
Color colorBackground = {236, 240, 241, 255};
Color colorDark = {44, 62, 80, 255};
Color colorLight = {255, 255, 255, 255};

// Global game state definition
GameState game = {0};

void InitGame(void)
{
    game.screen = SCREEN_START;
    game.mode = MODE_ONE_PLAYER;
    game.difficulty = DIFF_MEDIUM;
    game.humanSymbol = 'X';
    game.aiSymbol = 'O';
    game.currentPlayer = 'X';
    game.gameOver = false;
    game.winner = ' ';
    game.aiTurn = false;
    game.aiMoveTimer = 0.0f;
    game.player1Wins = 0;
    game.player2Wins = 0;
    game.draws = 0;
    game.isFullscreen = false;
    ResetBoard();
}

void ResetBoard(void)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            game.board[i][j] = ' ';
    
    game.currentPlayer = 'X';
    game.gameOver = false;
    game.winner = ' ';
    game.aiTurn = (game.mode == MODE_ONE_PLAYER && game.humanSymbol == 'O');
    game.aiMoveTimer = 0.5f;
}

// Check for winner on the board
bool CheckWinner(void)
{
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (game.board[i][0] != ' ' && 
            game.board[i][0] == game.board[i][1] && 
            game.board[i][1] == game.board[i][2]) {
            game.winner = game.board[i][0];
            if (game.mode == MODE_ONE_PLAYER) {
                if (game.winner == game.humanSymbol)
                    game.player1Wins++;
                else
                    game.player2Wins++;
            } else {
                if (game.winner == 'X')
                    game.player1Wins++;
                else
                    game.player2Wins++;
            }
            return true;
        }
    }
    
    // Check columns
    for (int i = 0; i < 3; i++) {
        if (game.board[0][i] != ' ' && 
            game.board[0][i] == game.board[1][i] && 
            game.board[1][i] == game.board[2][i]) {
            game.winner = game.board[0][i];
            if (game.mode == MODE_ONE_PLAYER) {
                if (game.winner == game.humanSymbol)
                    game.player1Wins++;
                else
                    game.player2Wins++;
            } else {
                if (game.winner == 'X')
                    game.player1Wins++;
                else
                    game.player2Wins++;
            }
            return true;
        }
    }
    
    // Check diagonals
    if (game.board[0][0] != ' ' && 
        game.board[0][0] == game.board[1][1] && 
        game.board[1][1] == game.board[2][2]) {
        game.winner = game.board[0][0];
        if (game.mode == MODE_ONE_PLAYER) {
            if (game.winner == game.humanSymbol)
                game.player1Wins++;
            else
                game.player2Wins++;
        } else {
            if (game.winner == 'X')
                game.player1Wins++;
            else
                game.player2Wins++;
        }
        return true;
    }
    
    if (game.board[0][2] != ' ' && 
        game.board[0][2] == game.board[1][1] && 
        game.board[1][1] == game.board[2][0]) {
        game.winner = game.board[0][2];
        if (game.mode == MODE_ONE_PLAYER) {
            if (game.winner == game.humanSymbol)
                game.player1Wins++;
            else
                game.player2Wins++;
        } else {
            if (game.winner == 'X')
                game.player1Wins++;
            else
                game.player2Wins++;
        }
        return true;
    }
    
    return false;
}

bool IsBoardFull(void)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game.board[i][j] == ' ')
                return false;
        }
    }
    game.winner = ' ';
    game.draws++;
    return true;
}

void MakeAIMove(void)
{
    struct Move bestMove;
    
    // Select AI based on difficulty (Team B's AI functions)
    // DIFF_HARD = 1 (Perfect), DIFF_MEDIUM = 2 (Imperfect), DIFF_EASY = 3 (Model)
    // Board format is now standardized, so we can pass it directly
    if (game.difficulty == DIFF_HARD)
        bestMove = findBestMovePerfect(game.board);
    else if (game.difficulty == DIFF_MEDIUM)
        bestMove = findBestMoveImperfect(game.board);
    else // DIFF_EASY
        bestMove = findBestMoveModel(game.board);
    
    // Apply AI move to board
    if (bestMove.row != -1 && bestMove.col != -1) {
        game.board[bestMove.row][bestMove.col] = game.aiSymbol;
        game.currentPlayer = game.humanSymbol;
    }
}
