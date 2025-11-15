#include "screens.h"
#include <stdio.h>
#include "game_state.h"
#include "ui.h"
#include "../Team_B/minimax.h"

void DrawStartScreen(void)
{
    const char* title = "TIC-TAC-TOE";
    int titleSize = 80;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, SCREEN_WIDTH/2 - titleWidth/2, 100, titleSize, colorPrimary);
    
    const char* subtitle = "Classic Strategy Game";
    int subtitleWidth = MeasureText(subtitle, 25);
    DrawText(subtitle, SCREEN_WIDTH/2 - subtitleWidth/2, 190, 25, colorDark);
    
    Rectangle playButton = CreateButton(SCREEN_WIDTH/2, 280, 250, 70);
    Rectangle instructionsButton = CreateButton(SCREEN_WIDTH/2, 370, 250, 70);
    Rectangle fullscreenButton = CreateButton(SCREEN_WIDTH/2, 460, 250, 60);
    
    DrawButton(playButton, "PLAY", colorSecondary);
    DrawButton(instructionsButton, "INSTRUCTIONS", colorPrimary);

    const char* fullscreenText = game.isFullscreen ? "WINDOWED MODE" : "FULLSCREEN";
    DrawButton(fullscreenButton, fullscreenText, colorWarning);
}

void HandleStartScreen(void)
{
    Rectangle playButton = CreateButton(SCREEN_WIDTH/2, 280, 250, 70);
    Rectangle instructionsButton = CreateButton(SCREEN_WIDTH/2, 370, 250, 70);
    Rectangle fullscreenButton = CreateButton(SCREEN_WIDTH/2, 460, 250, 60);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(playButton))
            game.screen = SCREEN_MODE_SELECT;
        else if (IsButtonHovered(instructionsButton))
            game.screen = SCREEN_INSTRUCTIONS;
        else if (IsButtonHovered(fullscreenButton))
        {
            game.isFullscreen = !game.isFullscreen;
            
            if (game.isFullscreen)
            {
                // Store current window size before going fullscreen
                int monitor = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
                SetWindowPosition(0, 0);
                SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
            }
            else
            {
                // Return to windowed mode
                ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
                SetWindowSize(800, 600);
                // Center the window
                int monitor = GetCurrentMonitor();
                int monitorWidth = GetMonitorWidth(monitor);
                int monitorHeight = GetMonitorHeight(monitor);
                SetWindowPosition((monitorWidth - 800) / 2, (monitorHeight - 600) / 2);
            }
        }
    }
}

void DrawModeSelectScreen(void)
{
    const char* title = "SELECT MODE";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, SCREEN_WIDTH/2 - titleWidth/2, 80, 60, colorPrimary);
    
    Rectangle onePlayerButton = CreateButton(SCREEN_WIDTH/2, 250, 300, 80);
    Rectangle twoPlayerButton = CreateButton(SCREEN_WIDTH/2, 370, 300, 80);
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 490, 200, 60);
    
    DrawButton(onePlayerButton, "1 PLAYER", colorSecondary);
    DrawButton(twoPlayerButton, "2 PLAYERS", colorWarning);
    DrawButton(backButton, "BACK", colorDark);
}

void HandleModeSelectScreen(void)
{
    Rectangle onePlayerButton = CreateButton(SCREEN_WIDTH/2, 250, 300, 80);
    Rectangle twoPlayerButton = CreateButton(SCREEN_WIDTH/2, 370, 300, 80);
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 490, 200, 60);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(onePlayerButton))
        {
            game.mode = MODE_ONE_PLAYER;
            game.screen = SCREEN_DIFFICULTY_SELECT;
        }
        else if (IsButtonHovered(twoPlayerButton))
        {
            game.mode = MODE_TWO_PLAYER;
            game.screen = SCREEN_SYMBOL_SELECT_2P;
        }
        else if (IsButtonHovered(backButton))
            game.screen = SCREEN_START;
    }
}

void DrawDifficultySelectScreen(void)
{
    const char* title = "SELECT DIFFICULTY";
    int titleWidth = MeasureText(title, 55);
    DrawText(title, SCREEN_WIDTH/2 - titleWidth/2, 70, 55, colorPrimary);
    
    Rectangle easyButton = CreateButton(SCREEN_WIDTH/2, 200, 280, 75);
    Rectangle mediumButton = CreateButton(SCREEN_WIDTH/2, 305, 280, 75);
    Rectangle hardButton = CreateButton(SCREEN_WIDTH/2, 410, 280, 75);
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 520, 200, 60);
    
    DrawButton(easyButton, "EASY", colorSecondary);
    DrawButton(mediumButton, "MEDIUM", colorWarning);
    DrawButton(hardButton, "HARD", colorAccent);
    DrawButton(backButton, "BACK", colorDark);
}

void HandleDifficultySelectScreen(void)
{
    Rectangle easyButton = CreateButton(SCREEN_WIDTH/2, 200, 280, 75);
    Rectangle mediumButton = CreateButton(SCREEN_WIDTH/2, 305, 280, 75);
    Rectangle hardButton = CreateButton(SCREEN_WIDTH/2, 410, 280, 75);
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 520, 200, 60);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(easyButton))
        {
            game.difficulty = DIFF_EASY;
            game.screen = SCREEN_SYMBOL_SELECT_1P;
        }
        else if (IsButtonHovered(mediumButton))
        {
            game.difficulty = DIFF_MEDIUM;
            game.screen = SCREEN_SYMBOL_SELECT_1P;
        }
        else if (IsButtonHovered(hardButton))
        {
            game.difficulty = DIFF_HARD;
            game.screen = SCREEN_SYMBOL_SELECT_1P;
        }
        else if (IsButtonHovered(backButton))
            game.screen = SCREEN_MODE_SELECT;
    }
}

void DrawSymbolSelectScreen(bool isPlayer1)
{
    const char* title = isPlayer1 ? "PLAYER 1: CHOOSE SYMBOL" : "CHOOSE YOUR SYMBOL";
    int titleWidth = MeasureText(title, 50);
    DrawText(title, SCREEN_WIDTH/2 - titleWidth/2, 80, 50, colorPrimary);
    
    Rectangle xButton = CreateButton(SCREEN_WIDTH/2 - 130, 280, 180, 180);
    Rectangle oButton = CreateButton(SCREEN_WIDTH/2 + 130, 280, 180, 180);
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 500, 200, 60);
    
    DrawButton(xButton, "X", colorPrimary);
    DrawButton(oButton, "O", colorAccent);
    DrawButton(backButton, "BACK", colorDark);
}

void HandleSymbolSelectScreen(bool isPlayer1)
{
    Rectangle xButton = CreateButton(SCREEN_WIDTH/2 - 130, 280, 180, 180);
    Rectangle oButton = CreateButton(SCREEN_WIDTH/2 + 130, 280, 180, 180);
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 500, 200, 60);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(xButton))
        {
            if (game.mode == MODE_ONE_PLAYER)
            {
                game.humanSymbol = 'X';
                game.aiSymbol = 'O';
            }
            else
            {
                game.humanSymbol = 'X';
            }
            ResetBoard();
            game.screen = SCREEN_GAME;
        }
        else if (IsButtonHovered(oButton))
        {
            if (game.mode == MODE_ONE_PLAYER)
            {
                game.humanSymbol = 'O';
                game.aiSymbol = 'X';
            }
            else
            {
                game.humanSymbol = 'O';
            }
            ResetBoard();
            game.screen = SCREEN_GAME;
        }
        else if (IsButtonHovered(backButton))
        {
            if (isPlayer1)
                game.screen = SCREEN_MODE_SELECT;
            else
                game.screen = SCREEN_DIFFICULTY_SELECT;
        }
    }
}


void DrawInstructionsScreen(void)
{
    const char* title = "INSTRUCTIONS";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, SCREEN_WIDTH/2 - titleWidth/2, 50, 60, colorPrimary);
    
    DrawRectangleRec((Rectangle){100, 130, 600, 380}, colorLight);
    DrawRectangleLinesEx((Rectangle){100, 130, 600, 380}, 3, colorPrimary);
    
    const char* instructions[] = {
        "HOW TO PLAY:",
        "",
        "1. Players take turns placing their symbol (X or O)",
        "   on a 3x3 grid.",
        "",
        "2. The first player to get 3 of their symbols in",
        "   a row (horizontally, vertically, or diagonally)",
        "   wins the game.",
        "",
        "3. If all 9 squares are filled and no player has",
        "   3 in a row, the game is a draw.",
        "",
        "Click on an empty square to place your symbol!"
    };
    
    int yPos = 150;
    for (int i = 0; i < 13; i++)
    {
        DrawText(instructions[i], 120, yPos, 22, colorDark);
        yPos += 28;
    }
    
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 540, 200, 50);
    DrawButton(backButton, "BACK", colorPrimary);
}

void HandleInstructionsScreen(void)
{
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 540, 200, 50);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsButtonHovered(backButton))
        game.screen = SCREEN_START;
}

void DrawGameScreen(void)
{
    const char* header;
    if (game.mode == MODE_ONE_PLAYER)
        header = "YOU vs AI";
    else
        header = "PLAYER 1 vs PLAYER 2";
    
    int headerWidth = MeasureText(header, 40);
    DrawText(header, SCREEN_WIDTH/2 - headerWidth/2, 30, 40, colorPrimary);
    
    char scoreText[64];
    if (game.mode == MODE_ONE_PLAYER)
        sprintf(scoreText, "You: %d  |  AI: %d  |  Draws: %d", 
                game.player1Wins, game.player2Wins, game.draws);
    else
        sprintf(scoreText, "P1: %d  |  P2: %d  |  Draws: %d", 
                game.player1Wins, game.player2Wins, game.draws);
    
    int scoreWidth = MeasureText(scoreText, 22);
    DrawText(scoreText, SCREEN_WIDTH/2 - scoreWidth/2, 80, 22, colorDark);
    
    if (!game.gameOver)
    {
        const char* turnText;
        if (game.mode == MODE_ONE_PLAYER)
        {
            turnText = game.aiTurn ? "AI's Turn" : "Your Turn";
        }
        else
        {
            turnText = (game.currentPlayer == game.humanSymbol) ? 
                       "Player 1's Turn" : "Player 2's Turn";
        }
        
        int turnWidth = MeasureText(turnText, 28);
        Color turnColor = game.aiTurn ? colorAccent : colorSecondary;
        DrawText(turnText, SCREEN_WIDTH/2 - turnWidth/2, 120, 28, turnColor);
    }
    
    float boardSize = 360;
    float boardX = SCREEN_WIDTH/2 - boardSize/2;
    float boardY = 180;
    float cellSize = boardSize / 3;
    
    DrawRectangleRec((Rectangle){boardX - 10, boardY - 10, boardSize + 20, boardSize + 20}, colorLight);
    
    for (int i = 1; i < 3; i++)
    {
        DrawLineEx((Vector2){boardX + i * cellSize, boardY},
                   (Vector2){boardX + i * cellSize, boardY + boardSize},
                   5, colorDark);
        DrawLineEx((Vector2){boardX, boardY + i * cellSize},
                   (Vector2){boardX + boardSize, boardY + i * cellSize},
                   5, colorDark);
    }
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            float x = boardX + j * cellSize + cellSize/2;
            float y = boardY + i * cellSize + cellSize/2;
            
            if (game.board[i][j] == 'X')
            {
                float offset = cellSize * 0.25f;
                DrawLineEx((Vector2){x - offset, y - offset},
                          (Vector2){x + offset, y + offset}, 8, colorPrimary);
                DrawLineEx((Vector2){x + offset, y - offset},
                          (Vector2){x - offset, y + offset}, 8, colorPrimary);
            }
            else if (game.board[i][j] == 'O')
            {
                DrawRing((Vector2){x, y}, cellSize * 0.25f, cellSize * 0.3f, 0, 360, 32, colorAccent);
            }
            
            if (!game.gameOver && !game.aiTurn && game.board[i][j] == ' ')
            {
                Rectangle cell = {boardX + j * cellSize, boardY + i * cellSize, 
                                 cellSize, cellSize};
                if (CheckCollisionPointRec(GetMousePosition(), cell))
                {
                    DrawRectangle(cell.x, cell.y, cell.width, cell.height, 
                                 (Color){100, 100, 100, 50});
                }
            }
        }
    }
    
    Rectangle menuButton = CreateButton(SCREEN_WIDTH/2, 565, 180, 50);
    DrawButton(menuButton, "MENU", colorDark);
}

void HandleGameScreen(void)
{
    if (game.gameOver)
        return;
    
    if (game.mode == MODE_ONE_PLAYER && game.aiTurn)
    {
        game.aiMoveTimer -= GetFrameTime();
        if (game.aiMoveTimer <= 0)
        {
            MakeAIMove();
            game.aiTurn = false;
            
            if (CheckWinner() || IsBoardFull())
            {
                game.gameOver = true;
                game.screen = SCREEN_GAME_OVER;
            }
        }
        return;
    }
    
    Rectangle menuButton = CreateButton(SCREEN_WIDTH/2, 565, 180, 50);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(menuButton))
        {
            game.screen = SCREEN_START;
            return;
        }
        
        float boardSize = 360;
        float boardX = SCREEN_WIDTH/2 - boardSize/2;
        float boardY = 180;
        float cellSize = boardSize / 3;
        
        Vector2 mousePos = GetMousePosition();
        
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                Rectangle cell = {boardX + j * cellSize, boardY + i * cellSize,
                                 cellSize, cellSize};
                
                if (CheckCollisionPointRec(mousePos, cell) && game.board[i][j] == ' ')
                {
                    game.board[i][j] = game.currentPlayer;
                    
                    if (CheckWinner() || IsBoardFull())
                    {
                        game.gameOver = true;
                        game.screen = SCREEN_GAME_OVER;
                        return;
                    }
                    
                    if (game.mode == MODE_ONE_PLAYER)
                    {
                        game.aiTurn = true;
                        game.aiMoveTimer = 0.5f;
                    }
                    else
                    {
                        game.currentPlayer = (game.currentPlayer == 'X') ? 'O' : 'X';
                    }
                    break;
                }
            }
        }
    }
}

void DrawGameOverScreen(void)
{
    // Draw semi-transparent panel at TOP of screen (not covering board)
    float panelWidth = 500;
    float panelHeight = 200;
    float panelX = SCREEN_WIDTH/2 - panelWidth/2;
    float panelY = 20;  // Near top, not center
    
    // Semi-transparent background
    DrawRectangleRec((Rectangle){panelX, panelY, panelWidth, panelHeight}, (Color){44, 62, 80, 230});
    DrawRectangleLinesEx((Rectangle){panelX, panelY, panelWidth, panelHeight}, 4, colorPrimary);
    
    // Result text
    const char* resultText;
    Color resultColor;
    
    if (game.winner == ' ')
    {
        resultText = "IT'S A DRAW!";
        resultColor = colorWarning;
    }
    else if (game.mode == MODE_ONE_PLAYER)
    {
        if (game.winner == game.humanSymbol)
        {
            resultText = "YOU WIN!";
            resultColor = colorSecondary;
        }
        else
        {
            resultText = "AI WINS!";
            resultColor = colorAccent;
        }
    }
    else
    {
        if (game.winner == game.humanSymbol)
        {
            resultText = "PLAYER 1 WINS!";
            resultColor = colorSecondary;
        }
        else
        {
            resultText = "PLAYER 2 WINS!";
            resultColor = colorSecondary;
        }
    }
    
    int resultWidth = MeasureText(resultText, 45);
    DrawText(resultText, SCREEN_WIDTH/2 - resultWidth/2, panelY + 30, 45, resultColor);
    
    // Score display
    char scoreText[128];
    if (game.mode == MODE_ONE_PLAYER)
        sprintf(scoreText, "You: %d | AI: %d | Draws: %d",
                game.player1Wins, game.player2Wins, game.draws);
    else
        sprintf(scoreText, "P1: %d | P2: %d | Draws: %d",
                game.player1Wins, game.player2Wins, game.draws);
    
    int scoreWidth = MeasureText(scoreText, 18);
    DrawText(scoreText, SCREEN_WIDTH/2 - scoreWidth/2, panelY + 90, 18, colorLight);
    
    // Buttons - horizontal layout at bottom of panel
    Rectangle playAgainButton = CreateButton(SCREEN_WIDTH/2 - 130, panelY + 150, 220, 45);
    Rectangle menuButton = CreateButton(SCREEN_WIDTH/2 + 130, panelY + 150, 220, 45);
    
    DrawButton(playAgainButton, "PLAY AGAIN", colorSecondary);
    DrawButton(menuButton, "MAIN MENU", colorDark);
}

void HandleGameOverScreen(void)
{
    float panelY = 20;  // Match the position from DrawGameOverScreen
    
    Rectangle playAgainButton = CreateButton(SCREEN_WIDTH/2 - 130, panelY + 150, 220, 45);
    Rectangle menuButton = CreateButton(SCREEN_WIDTH/2 + 130, panelY + 150, 220, 45);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(playAgainButton))
        {
            ResetBoard();
            game.screen = SCREEN_GAME;
        }
        else if (IsButtonHovered(menuButton))
        {
            game.screen = SCREEN_START;
            game.player1Wins = 0;
            game.player2Wins = 0;
            game.draws = 0;
        }
    }
}
