/**
 * @file screens.c
 * @brief Implements all screen drawing and handling logic
 * * This file contains the implementation for every function declared
 * in `screens.h`. It is the core of the "View" (drawing) and
 * "Controller" (input handling) for the application.
 * * Each screen (`SCREEN_START`, `SCREEN_GAME`, etc.) has two functions:
 * 1. `Draw...Screen()`: Renders the UI for that screen.
 * 2. `Handle...Screen()`: Processes input (e.g., button clicks) for that screen.
 * * This separation keeps the logic clean and organized.
 */

#include "screens.h"
#include <stdio.h>      // For sprintf (in DrawGameScreen)
#include "game_state.h" // Access to the global 'game' state and colors
#include "ui.h"         // For button helper functions
#include "../Team_B/minimax.h" // Access to AI functions (MakeAIMove)
#include <string.h>     // For strncmp (in DrawGameScreen)

// ============================================================================
// START SCREEN
// ============================================================================

/**
 * @brief Draws the main menu screen UI.
 * * This function is purely for rendering. It uses helper functions
 * from `ui.c` (like `CreateButton` and `DrawButton`) to draw the
 * interface. All logic is in `HandleStartScreen`.
 */
void DrawStartScreen(void)
{
    // Draw Title
    const char* title = "TIC-TAC-TOE";
    int titleSize = 80;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, SCREEN_WIDTH/2 - titleWidth/2, 100, titleSize, colorPrimary);
    
    const char* subtitle = "Classic Strategy Game";
    int subtitleWidth = MeasureText(subtitle, 25);
    DrawText(subtitle, SCREEN_WIDTH/2 - subtitleWidth/2, 190, 25, colorLight);
    
    // Define button rectangles (using the centering helper)
    Rectangle playButton = CreateButton(SCREEN_WIDTH/2, 260, 250, 70);
    Rectangle loadButton = CreateButton(SCREEN_WIDTH/2, 350, 250, 70);
    Rectangle instructionsButton = CreateButton(SCREEN_WIDTH/2 - 130, 440, 250, 70);
    Rectangle historyButton      = CreateButton(SCREEN_WIDTH/2 + 130, 440, 250, 70);
    Rectangle fullscreenButton = CreateButton(SCREEN_WIDTH/2, 530, 250, 60);
    Rectangle themesButton = CreateButton(SCREEN_WIDTH - 90, SCREEN_HEIGHT - 40, 160, 50);

    // Draw the buttons
    DrawButton(playButton, "PLAY", colorSecondary);
    DrawButton(loadButton, "LOAD GAME", colorAccent);
    DrawButton(instructionsButton, "INSTRUCTIONS", colorPrimary);
    DrawButton(historyButton, "HISTORY", colorPrimary); // Added
    DrawButton(themesButton, "THEMES", colorDark);
    
    // Fullscreen button text changes based on current state
    const char* fullscreenText = game.isFullscreen ? "WINDOWED MODE" : "FULLSCREEN";
    DrawButton(fullscreenButton, fullscreenText, colorWarning);
}

/**
 * @brief Handles all input for the main menu screen.
 * * This function checks for mouse clicks on the buttons defined
 * in `DrawStartScreen`. When a button is clicked, it changes
 * the game state (e.g., `game.screen = SCREEN_MODE_SELECT`)
 * to navigate to a new screen.
 */
void HandleStartScreen(void)
{
    // Re-create button rectangles to check for collisions
    // These *must* match the ones in DrawStartScreen
    Rectangle playButton = CreateButton(SCREEN_WIDTH/2, 260, 250, 70);
    Rectangle loadButton = CreateButton(SCREEN_WIDTH/2, 350, 250, 70);
    Rectangle instructionsButton = CreateButton(SCREEN_WIDTH/2 - 130, 440, 250, 70);
    Rectangle historyButton      = CreateButton(SCREEN_WIDTH/2 + 130, 440, 250, 70);
    Rectangle fullscreenButton = CreateButton(SCREEN_WIDTH/2, 530, 250, 60);
    Rectangle themesButton = CreateButton(SCREEN_WIDTH - 90, SCREEN_HEIGHT - 40, 160, 50);

    // Check for a click *once*
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Check which button was clicked
        if (IsButtonHovered(playButton))
        {
            game.screen = SCREEN_MODE_SELECT; // Navigate to mode select
        }
        else if (IsButtonHovered(loadButton))
        {
            // Try to load a game.
            // If `LoadGame()` is successful (returns true),
            // jump directly into the game screen.
            if (LoadGame())
            {
                game.screen = SCREEN_GAME;
            }
            // else: LoadGame() returned false (no file/corrupt),
            // so we do nothing and stay on the start screen.
        }
        else if (IsButtonHovered(themesButton))
        {
            game.screen = SCREEN_THEME_SELECT; // Navigate to theme select
        }
        else if (IsButtonHovered(instructionsButton))
        {
            game.screen = SCREEN_INSTRUCTIONS; // Navigate to instructions
        }
        else if (IsButtonHovered(historyButton))
        {
            LoadGameHistory(); // Read the history file
            game.screen = SCREEN_HISTORY;
        }
        else if (IsButtonHovered(fullscreenButton))
        {
            // Handle fullscreen toggle
            game.isFullscreen = !game.isFullscreen;
            
            if (game.isFullscreen)
            {
                // Go to borderless windowed fullscreen
                int monitor = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
                SetWindowPosition(0, 0);
                SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
            }
            else
            {
                // Return to windowed mode
                ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
                SetWindowSize(1280, 720);
                // Center the window
                int monitor = GetCurrentMonitor();
                int monitorWidth = GetMonitorWidth(monitor);
                int monitorHeight = GetMonitorHeight(monitor);
                SetWindowPosition((monitorWidth - 1280) / 2, (monitorHeight - 720) / 2);
            }
        }
    }
}

// ============================================================================
// MODE SELECT SCREEN
// ============================================================================

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
            game.screen = SCREEN_DIFFICULTY_SELECT; // 1P needs difficulty
        }
        else if (IsButtonHovered(twoPlayerButton))
        {
            game.mode = MODE_TWO_PLAYER;
            game.screen = SCREEN_SYMBOL_SELECT_2P; // 2P skips difficulty
        }
        else if (IsButtonHovered(backButton))
            game.screen = SCREEN_START;
    }
}

// ============================================================================
// DIFFICULTY SELECT SCREEN
// ============================================================================

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
            game.screen = SCREEN_MODE_SELECT; // Go back to mode select
    }
}

// ============================================================================
// SYMBOL SELECT SCREEN
// ============================================================================

/**
 * @brief Draws the 'X' or 'O' selection screen.
 * * This function is reused for both 1P and 2P modes.
 * The `isPlayer1` parameter just changes the title text.
 */
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

/**
 * @brief Handles symbol selection for both 1P and 2P modes.
 * * This function sets the player symbols (`game.humanSymbol`, `game.aiSymbol`)
 * based on the selection and game mode.
 * After selection, it *always* calls `ResetBoard()` to set up the
 * board correctly (e.g., if 'O' is chosen, AI must go first)
 * and then navigates to the main game screen.
 */
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
                game.humanSymbol = 'X'; // P1 is X
                // P2 will be 'O' by default
            }
            ResetBoard(); // CRITICAL: Reset board *after* setting symbols
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
                game.humanSymbol = 'O'; // P1 is O
                // P2 will be 'X' by default
            }
            ResetBoard(); // CRITICAL: Reset board *after* setting symbols
            game.screen = SCREEN_GAME;
        }
        else if (IsButtonHovered(backButton))
        {
            // Go back to the correct previous screen
            if (isPlayer1)
                game.screen = SCREEN_MODE_SELECT; // 2P mode goes back to Mode Select
            else
                game.screen = SCREEN_DIFFICULTY_SELECT; // 1P mode goes back to Difficulty
        }
    }
}

// ============================================================================
// INSTRUCTIONS SCREEN
// ============================================================================

void DrawInstructionsScreen(void)
{
    const char* title = "INSTRUCTIONS";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, SCREEN_WIDTH/2 - titleWidth/2, 50, 60, colorPrimary);
    
    // Calculate centered positions for the instruction box
    // This makes it responsive to different window sizes.
    float boxWidth = 600;
    float boxHeight = 380;
    float boxX = (SCREEN_WIDTH / 2) - (boxWidth / 2);
    float boxY = 130;
    float textX = boxX + 20;

    // Draw the instruction box
    DrawRectangleRec((Rectangle){boxX, boxY, boxWidth, boxHeight}, colorLight);
    DrawRectangleLinesEx((Rectangle){boxX, boxY, boxWidth, boxHeight}, 3, colorPrimary);
    
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
        "   3 in a row, the game is a 'draw'.",
        "",
        "Click on an empty square to place your symbol!"
    };
    
   int yPos = boxY + 20; // Start text Y-position relative to the box
    for (int i = 0; i < 13; i++)
    {
        DrawText(instructions[i], textX, yPos, 22, colorDark);
        yPos += 28; // Move down for the next line
    }
    
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 540, 200, 50);
    DrawButton(backButton, "BACK", colorPrimary);
}

void HandleInstructionsScreen(void)
{
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 540, 200, 50);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsButtonHovered(backButton))
        game.screen = SCREEN_START; // Always go back to the start screen
}

// ============================================================================
// GAME SCREEN
// ============================================================================

/**
 * @brief Draws the main game screen (scores, board, buttons).
 * * This is one of the most complex draw functions as it has to
 * reflect many different states:
 * - Current turn (Human vs. AI, P1 vs. P2)
 * - The `game.board[3][3]` array (drawing X's and O's)
 * - The hover effect over empty cells
 * - The "Game Saved!" message
 */
void DrawGameScreen(void)
{
    // --- 1. Draw Header & Scores ---
    const char* header = (game.mode == MODE_ONE_PLAYER) ? "YOU vs AI" : "PLAYER 1 vs PLAYER 2";
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
    DrawText(scoreText, SCREEN_WIDTH/2 - scoreWidth/2, 80, 22, colorLight);
    
    // --- 2. Draw Current Turn Text ---
if (!game.gameOver)
{
    char turnText[64]; // Changed to a char array to support sprintf

    if (game.mode == MODE_ONE_PLAYER)
    {
        sprintf(turnText, game.aiTurn ? "AI's Turn" : "Your Turn");
    }
    else // 2P Mode
    {
        // This is the new logic.
        // 'game.humanSymbol' stores Player 1's choice.
        char p1Symbol = game.humanSymbol;
        char p2Symbol = (game.humanSymbol == 'X') ? 'O' : 'X';

        if (game.currentPlayer == p1Symbol)
        {
            sprintf(turnText, "Player 1's Turn (%c)", p1Symbol);
        }
        else
        {
            sprintf(turnText, "Player 2's Turn (%c)", p2Symbol);
        }
    }

    int turnWidth = MeasureText(turnText, 28);
    // Use a different color if it's the AI's turn
    Color turnColor = game.aiTurn ? colorAccent : colorSecondary;
    DrawText(turnText, SCREEN_WIDTH/2 - turnWidth/2, 120, 28, turnColor);
}
    
    // --- 3. Draw the Game Board & Pieces ---
    float boardSize = 360;
    float boardX = SCREEN_WIDTH/2 - boardSize/2;
    float boardY = 180;
    float cellSize = boardSize / 3;
    
    // Draw grid background
    DrawRectangleRec((Rectangle){boardX - 10, boardY - 10, boardSize + 20, boardSize + 20}, colorLight);
    
    // Draw grid lines
    for (int i = 1; i < 3; i++)
    {
        DrawLineEx((Vector2){boardX + i * cellSize, boardY},
                   (Vector2){boardX + i * cellSize, boardY + boardSize},
                   5, colorDark);
        DrawLineEx((Vector2){boardX, boardY + i * cellSize},
                   (Vector2){boardX + boardSize, boardY + i * cellSize},
                   5, colorDark);
    }
    
    // Draw X's and O's by iterating over the game state
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            float x = boardX + j * cellSize + cellSize/2; // Cell center x
            float y = boardY + i * cellSize + cellSize/2; // Cell center y
            
            if (game.board[i][j] == 'X')
            {
                float offset = cellSize * 0.25f; // Size of the X
                DrawLineEx((Vector2){x - offset, y - offset},
                           (Vector2){x + offset, y + offset}, 8, colorPrimary);
                DrawLineEx((Vector2){x + offset, y - offset},
                           (Vector2){x - offset, y + offset}, 8, colorPrimary);
            }
            else if (game.board[i][j] == 'O')
            {
                DrawRing((Vector2){x, y}, cellSize * 0.25f, cellSize * 0.3f, 0, 360, 32, colorAccent);
            }
            
            // Draw hover effect
            // Show only if game is not over, it's not the AI's turn,
            // and the cell is empty.
            if (!game.gameOver && !game.aiTurn && game.board[i][j] == ' ')
            {
                Rectangle cell = {boardX + j * cellSize, boardY + i * cellSize, 
                                 cellSize, cellSize};
                if (CheckCollisionPointRec(GetMousePosition(), cell))
                {
                    // Draw semi-transparent gray box
                    DrawRectangle(cell.x, cell.y, cell.width, cell.height, 
                                 (Color){100, 100, 100, 50});
                }
            }
        }
    }
    
    // --- 4. Draw Save Message (if timer is active) ---
    if (game.saveMessageTimer > 0)
    {
        int textWidth = MeasureText(game.saveMessage, 20);
        // Show "ERROR" in red (colorAccent), "Saved!" in green (colorSecondary)
        Color msgColor = (strncmp(game.saveMessage, "ERROR", 5) == 0) ? colorAccent : colorSecondary;
        DrawText(game.saveMessage, SCREEN_WIDTH / 2 - textWidth / 2, 555, 20, msgColor);
    }

    // --- 5. Draw Undo Button (conditionally) ---
    // Only draw the button if there is a move to undo
    if (game.moveCount > 0)
    {
        // Positioned above the grid, on the right side
        Rectangle undoButton = CreateButton(SCREEN_WIDTH/2 + 270, 215, 120, 50);
        DrawButton(undoButton, "UNDO", colorAccent);
    }
    
    // --- 5. Draw Bottom Buttons ---
    Rectangle restartButton = CreateButton(SCREEN_WIDTH/2 - 170, 595, 140, 50);
    Rectangle saveButton = CreateButton(SCREEN_WIDTH/2, 595, 140, 50);
    Rectangle menuButton = CreateButton(SCREEN_WIDTH/2 + 170, 595, 140, 50);
    
    DrawButton(restartButton, "RESTART", colorWarning);
    DrawButton(saveButton, "SAVE", colorSecondary);
    DrawButton(menuButton, "MENU", colorDark);
}

/**
 * @brief Handles all logic for the main game screen.
 * * This is the most complex handler. It is responsible for:
 * 1. Counting down the save message timer.
 * 2. Handling clicks on Restart, Undo, Save, and Menu (always).
 * 3. (If game not over) Running the AI move timer and triggering MakeAIMove.
 * 4. (If human turn) Handling clicks on the 3x3 game board.
 * 5. After a move, checking for a win/draw and changing state.
 */
void HandleGameScreen(void)
{
    // 1. Update save message timer
    if (game.saveMessageTimer > 0)
    {
        game.saveMessageTimer -= GetFrameTime(); // GetFrameTime is time since last frame
    }

    // 2. Handle Human Button Input (Restart, Undo, Save, Menu)
    // This logic runs *even if the game is over* so the user can
    // undo their losing move or restart.
    
    // Define button rectangles (MUST match DrawGameScreen)
    // Define button rectangles (MUST match DrawGameScreen)
    Rectangle restartButton = CreateButton(SCREEN_WIDTH/2 - 170, 595, 140, 50);
    Rectangle saveButton    = CreateButton(SCREEN_WIDTH/2, 595, 140, 50);
    Rectangle menuButton    = CreateButton(SCREEN_WIDTH/2 + 170, 595, 140, 50);
    Rectangle undoButton    = CreateButton(SCREEN_WIDTH/2 + 270, 215, 120, 50);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(restartButton))
        {
            ResetBoard(); // Just reset the board, keep settings
            return; // Action taken, end this frame's logic
        }
        else if (game.moveCount > 0 && IsButtonHovered(undoButton)) // Only check if undo is possible
        {
            // --- HANDLE UNDO ---
            if (game.moveCount > 0)
            {
                game.moveCount--; // Go back to the previous state index
                
                // Restore the board and turn info from history
                memcpy(game.board, game.moveHistory[game.moveCount].board, sizeof(game.board));
                game.currentPlayer = game.moveHistory[game.moveCount].currentPlayer;
                game.aiTurn = game.moveHistory[game.moveCount].aiTurn;
                
                // The game is no longer over
                game.gameOver = false;
                game.winner = ' ';
                
                // Hide any messages
                game.saveMessageTimer = 0.0f;
            }
            return; // Action taken, end this frame's logic
        }
        else if (IsButtonHovered(saveButton))
        {
            SaveGame(); // Save the game, but continue playing
            return; // Action taken, end this frame's logic
        }
        else if (IsButtonHovered(menuButton))
        {
            game.screen = SCREEN_START; // Go to menu
            return; // Action taken, end this frame's logic
        }
        // If no button was clicked, we will proceed to check the game board.
    }
    
    // 3. Stop all game logic (AI turns, board clicks) if game is over
    if (game.gameOver)
        return;
    
    // 4. Handle AI Turn Logic
    if (game.mode == MODE_ONE_PLAYER && game.aiTurn)
    {
        // This timer adds a small delay before the AI moves.
        // This makes the AI *feel* more natural, like it's "thinking".
        game.aiMoveTimer -= GetFrameTime();
        if (game.aiMoveTimer <= 0)
        {
            MakeAIMove();   // AI logic function from game_state.c
            game.aiTurn = false; // AI turn is over
            
            // Check if the AI's move won the game
        if (CheckWinner() || IsBoardFull())
        {
            if (!game.gameOver) // Only append to history on the first frame it's over
            {
                AppendGameToHistory();
            }
            game.gameOver = true;
            game.screen = SCREEN_GAME_OVER; // Go to game over state
        }
        }
        return; // IMPORTANT: If it's the AI's turn, skip all human input
    }
    
    // 5. Handle Human Board Clicks (only if no button was pressed)
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        float boardSize = 360;
        float boardX = SCREEN_WIDTH/2 - boardSize/2;
        float boardY = 180;
        float cellSize = boardSize / 3;
        
        Vector2 mousePos = GetMousePosition();
        
        // Iterate over the 3x3 grid
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                Rectangle cell = {boardX + j * cellSize, boardY + i * cellSize,
                                 cellSize, cellSize};
                
                // Check if mouse clicked this cell AND if it's empty
                if (CheckCollisionPointRec(mousePos, cell) && game.board[i][j] == ' ')
                {
                    // --- SAVE STATE FOR UNDO ---
                    // We only save right before a human move is made.
                    // This works for both 1P (undoes human+AI) and 2P (undoes last move).
                    if (game.moveCount < 9) 
                    {
                        memcpy(game.moveHistory[game.moveCount].board, game.board, sizeof(game.board));
                        game.moveHistory[game.moveCount].currentPlayer = game.currentPlayer;
                        game.moveHistory[game.moveCount].aiTurn = game.aiTurn;
                        game.moveCount++;
                    }
                    // --- END SAVE STATE ---

                    // Place the current player's symbol
                    game.board[i][j] = game.currentPlayer;
                    
                    // 6. Check for Win/Draw
                    if (CheckWinner() || IsBoardFull())
                    {
                        if (!game.gameOver) // Only append to history on the first frame it's over
                        {
                            AppendGameToHistory();
                        }
                        game.gameOver = true;
                        game.screen = SCREEN_GAME_OVER;
                        return; // Game over, stop processing
                    }
                    
                    // 7. Switch Turns
                    if (game.mode == MODE_ONE_PLAYER)
                    {
                        game.aiTurn = true; // Set to AI's turn
                        game.aiMoveTimer = 0.5f; // Reset AI "thinking" timer
                    }
                    else // 2P mode
                    {
                        game.currentPlayer = (game.currentPlayer == 'X') ? 'O' : 'X';
                    }
                    break; // Move was made, stop checking cells
                }
            }
        }
    }
}

// ============================================================================
// GAME OVER SCREEN
// ============================================================================

/**
 * @brief Draws the game over panel.
 * * This function is designed to be drawn *on top of* `DrawGameScreen`.
 * It creates a semi-transparent panel that sits near the top of the
 * screen, displaying the result and the final scores.
 */
void DrawGameOverScreen(void)
{
    // Draw semi-transparent panel at TOP of screen
    float panelWidth = 500;
    float panelHeight = 200;
    float panelX = SCREEN_WIDTH/2 - panelWidth/2;
    float panelY = 20;  // Near top, not center
    
    // Semi-transparent background
    DrawRectangleRec((Rectangle){panelX, panelY, panelWidth, panelHeight}, (Color){44, 62, 80, 230});
    DrawRectangleLinesEx((Rectangle){panelX, panelY, panelWidth, panelHeight}, 4, colorPrimary);
    
    // --- 1. Result text ---
    const char* resultText;
    Color resultColor;
    
    if (game.winner == ' ') // Draw
    {
        resultText = "IT'S A DRAW!";
        resultColor = colorWarning;
    }
    else if (game.mode == MODE_ONE_PLAYER) // 1P Mode
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
    else // 2P Mode
{
    char p1Symbol = game.humanSymbol; // Player 1's chosen symbol

    if (game.winner == p1Symbol)
    {
        // Player 1 wins
        resultText = (p1Symbol == 'X') ? "PLAYER 1 (X) WINS!" : "PLAYER 1 (O) WINS!";
        resultColor = colorSecondary;
    }
    else
    {
        // Player 2 wins
        resultText = (p1Symbol == 'X') ? "PLAYER 2 (O) WINS!" : "PLAYER 2 (X) WINS!";
        resultColor = colorSecondary;
    }
}
    
    int resultWidth = MeasureText(resultText, 45);
    DrawText(resultText, SCREEN_WIDTH/2 - resultWidth/2, panelY + 30, 45, resultColor);
    
    // --- 2. Score display ---
    char scoreText[128];
    if (game.mode == MODE_ONE_PLAYER)
        sprintf(scoreText, "You: %d | AI: %d | Draws: %d",
                game.player1Wins, game.player2Wins, game.draws);
    else
        sprintf(scoreText, "P1: %d | P2: %d | Draws: %d",
                game.player1Wins, game.player2Wins, game.draws);
    
    int scoreWidth = MeasureText(scoreText, 18);
    DrawText(scoreText, SCREEN_WIDTH/2 - scoreWidth/2, panelY + 90, 18, colorLight);
    
    // --- 3. Buttons ---
    Rectangle playAgainButton = CreateButton(SCREEN_WIDTH/2 - 130, panelY + 150, 220, 45);
    Rectangle menuButton = CreateButton(SCREEN_WIDTH/2 + 130, panelY + 150, 220, 45);
    
    DrawButton(playAgainButton, "PLAY AGAIN", colorSecondary);
    DrawButton(menuButton, "MAIN MENU", colorDark);
}

/**
 * @brief Handles input for the game over panel.
 * * This function only runs when `game.screen == SCREEN_GAME_OVER`.
 * It checks for clicks on "Play Again" or "Main Menu".
 * Note: `HandleGameScreen` is *not* running at this time.
 */
void HandleGameOverScreen(void)
{
    float panelY = 20;  // Must match the Y pos from DrawGameOverScreen
    
    Rectangle playAgainButton = CreateButton(SCREEN_WIDTH/2 - 130, panelY + 150, 220, 45);
    Rectangle menuButton = CreateButton(SCREEN_WIDTH/2 + 130, panelY + 150, 220, 45);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(playAgainButton))
        {
            // Play Again: Just reset the board and go back to game screen
            ResetBoard();
            game.screen = SCREEN_GAME;
        }
        else if (IsButtonHovered(menuButton))
        {
            // Main Menu: Go to start screen AND reset all stats
            game.screen = SCREEN_START;
            game.player1Wins = 0;
            game.player2Wins = 0;
            game.draws = 0;
        }
    }
}

// ============================================================================
// THEME SELECT SCREEN
// ============================================================================

void DrawThemeSelectScreen(void)
{
    const char* title = "SELECT THEME";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, SCREEN_WIDTH/2 - titleWidth/2, 60, 60, colorPrimary);

    // Define buttons
    Rectangle defaultButton = CreateButton(SCREEN_WIDTH/2, 150, 280, 60);
    Rectangle darkButton = CreateButton(SCREEN_WIDTH/2, 220, 280, 60);
    Rectangle forestButton = CreateButton(SCREEN_WIDTH/2, 290, 280, 60);
    Rectangle spaceButton = CreateButton(SCREEN_WIDTH/2, 360, 280, 60);
    Rectangle aquaticButton = CreateButton(SCREEN_WIDTH/2, 430, 280, 60);
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 520, 200, 60);

    // Draw buttons
    DrawButton(defaultButton, "DEFAULT", colorSecondary);
    DrawButton(darkButton, "DARK", colorSecondary);
    DrawButton(forestButton, "FOREST", colorSecondary);
    DrawButton(spaceButton, "SPACE", colorSecondary);
    DrawButton(aquaticButton, "AQUATIC", colorSecondary);
    DrawButton(backButton, "BACK", colorDark);

    // Add a highlight rectangle to the *currently selected* theme
    // This provides clear visual feedback to the user.
    if (game.currentTheme == THEME_DEFAULT) DrawRectangleLinesEx(defaultButton, 5, colorAccent);
    if (game.currentTheme == THEME_DARK) DrawRectangleLinesEx(darkButton, 5, colorAccent);
    if (game.currentTheme == THEME_FOREST) DrawRectangleLinesEx(forestButton, 5, colorAccent);
    if (game.currentTheme == THEME_SPACE) DrawRectangleLinesEx(spaceButton, 5, colorAccent);
    if (game.currentTheme == THEME_AQUATIC) DrawRectangleLinesEx(aquaticButton, 5, colorAccent);
}

/**
 * @brief Handles clicks on the theme selection buttons.
 * * When a theme button is clicked, this function calls `ChangeTheme()`.
 * `ChangeTheme` instantly updates the global color variables,
 * so the UI will update to the new colors on the very next frame.
 */
void HandleThemeSelectScreen(void)
{
    Rectangle defaultButton = CreateButton(SCREEN_WIDTH/2, 150, 280, 60);
    Rectangle darkButton = CreateButton(SCREEN_WIDTH/2, 220, 280, 60);
    Rectangle forestButton = CreateButton(SCREEN_WIDTH/2, 290, 280, 60);
    Rectangle spaceButton = CreateButton(SCREEN_WIDTH/2, 360, 280, 60);
    Rectangle aquaticButton = CreateButton(SCREEN_WIDTH/2, 430, 280, 60);
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 520, 200, 60);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(defaultButton))
        {
            ChangeTheme(THEME_DEFAULT);
        }
        else if (IsButtonHovered(darkButton))
        {
            ChangeTheme(THEME_DARK);
        }
        else if (IsButtonHovered(forestButton))
        {
            ChangeTheme(THEME_FOREST);
        }
        else if (IsButtonHovered(spaceButton))
        {
            ChangeTheme(THEME_SPACE);
        }
        else if (IsButtonHovered(aquaticButton))
        {
            ChangeTheme(THEME_AQUATIC);
        }
        else if (IsButtonHovered(backButton))
        {
            game.screen = SCREEN_START; // Go back to start menu
        }
    }
}

// ============================================================================
// HISTORY SCREEN
// ============================================================================

void DrawHistoryScreen(void)
{
    const char* title = "GAME HISTORY";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, SCREEN_WIDTH/2 - titleWidth/2, 50, 60, colorPrimary);

    // Draw the instruction box
    float boxWidth = 700;
    float boxHeight = 450;
    float boxX = (SCREEN_WIDTH / 2) - (boxWidth / 2);
    float boxY = 130;
    float textX = boxX + 20;

    DrawRectangleRec((Rectangle){boxX, boxY, boxWidth, boxHeight}, colorLight);
    DrawRectangleLinesEx((Rectangle){boxX, boxY, boxWidth, boxHeight}, 3, colorPrimary);

    if (game.historyLineCount == 0)
    {
        // Show message if no history
        const char* msg = "No game history found. Go play a game!";
        int msgWidth = MeasureText(msg, 24);
        DrawText(msg, SCREEN_WIDTH/2 - msgWidth/2, 300, 24, colorDark);
    }
    else
    {
        // Display each line of history
        int yPos = boxY + 15; // Start text Y-position
        for (int i = 0; i < game.historyLineCount; i++)
        {
            DrawText(game.gameHistory[i], textX, yPos, 20, colorDark);
            yPos += 22; // Move down for the next line
        }
    }

   // --- Draw Buttons ---
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 620, 200, 50);
    DrawButton(backButton, "BACK", colorPrimary);

    // Only draw the "CLEAR" button if there is history to clear
    if (game.historyLineCount > 0)
    {
        Rectangle clearButton = CreateButton(SCREEN_WIDTH/2 + 245, 160, 200, 50);
        DrawButton(clearButton, "CLEAR", colorAccent); // Use Accent color
    }
}

void HandleHistoryScreen(void)
{
    // Define rectangles for both buttons
    Rectangle backButton = CreateButton(SCREEN_WIDTH/2, 620, 200, 50);
    Rectangle clearButton = CreateButton(SCREEN_WIDTH/2 + 245, 160, 200, 50);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsButtonHovered(backButton))
        {
            game.screen = SCREEN_START; // Go back to menu
        }
        // Only check the clear button if history exists (matches the draw logic)
        else if (game.historyLineCount > 0 && IsButtonHovered(clearButton))
        {
            ClearGameHistory();
            // The screen will automatically update on the next frame
            // to show "No game history found."
        }
    }
}