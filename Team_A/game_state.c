#include "game_state.h"
#include "../Team_B/minimax_core.h"

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
    game.difficulty = 2;
    game.humanSymbol = 'x';
    game.aiSymbol = 'o';
    game.currentPlayer = 'x';
    game.gameOver = false;
    game.winner = '_';
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
            game.board[i][j] = '_';
    
    game.currentPlayer = 'x';
    game.gameOver = false;
    game.winner = '_';
    game.aiTurn = (game.mode == MODE_ONE_PLAYER && game.humanSymbol == 'o');
    game.aiMoveTimer = 0.5f;
}

bool CheckWinner(void)
{
    int result = evaluate(game.board);
    
    if (result == 10)
    {
        game.winner = player;
        if (game.mode == MODE_ONE_PLAYER)
        {
            if (player == game.humanSymbol)
                game.player1Wins++;
            else
                game.player2Wins++;
        }
        else
        {
            if (player == game.humanSymbol)
                game.player1Wins++;
            else
                game.player2Wins++;
        }
        return true;
    }
    else if (result == -10)
    {
        game.winner = opponent;
        if (game.mode == MODE_ONE_PLAYER)
        {
            if (opponent == game.humanSymbol)
                game.player1Wins++;
            else
                game.player2Wins++;
        }
        else
        {
            if (opponent == game.humanSymbol)
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
    if (!isMovesLeft(game.board))
    {
        game.winner = '_';
        game.draws++;
        return true;
    }
    return false;
}

void MakeAIMove(void)
{
    struct Move bestMove = findBestMoveWithDifficulty(game.board, game.difficulty);
    
    if (bestMove.row != -1 && bestMove.col != -1)
    {
        game.board[bestMove.row][bestMove.col] = game.aiSymbol;
        game.currentPlayer = game.humanSymbol;
    }
}