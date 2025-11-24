/**
 * @file screens.c
 * @brief Implements screen drawing and handling logic.
 */

#include "screens.h"
#include "../Game_algorithms/minimax.h"
#include "game_state.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>


// Responsive Design Helpers
// Scales coordinates to fit the window while maintaining aspect ratio.
#define DESIGN_WIDTH 1280.0f
#define DESIGN_HEIGHT 720.0f

/**
 * @brief Calculates the scale factor for the current window size.
 */
static float CalculateScaleFactor(void) {
  float scaleX = (float)GetScreenWidth() / DESIGN_WIDTH;
  float scaleY = (float)GetScreenHeight() / DESIGN_HEIGHT;
  // Use smaller scale to maintain aspect ratio (adds black bars if needed)
  return (scaleX < scaleY) ? scaleX : scaleY;
}

/**
 * @brief Calculates horizontal centering offset.
 */
static float CalculateHorizontalOffset(void) {
  float scale = CalculateScaleFactor();
  return ((float)GetScreenWidth() - (DESIGN_WIDTH * scale)) / 2.0f;
}

/**
 * @brief Calculates vertical centering offset.
 */
static float CalculateVerticalOffset(void) {
  float scale = CalculateScaleFactor();
  return ((float)GetScreenHeight() - (DESIGN_HEIGHT * scale)) / 2.0f;
}

/**
 * @brief Scales X coordinate.
 */
static float ScaleX(float designX) {
  return CalculateHorizontalOffset() + (designX * CalculateScaleFactor());
}

/**
 * @brief Scales Y coordinate.
 */
static float ScaleY(float designY) {
  return CalculateVerticalOffset() + (designY * CalculateScaleFactor());
}

/**
 * @brief Scales a size value.
 */
static float ScaleSize(float designSize) {
  return designSize * CalculateScaleFactor();
}

// ============================================================================
// START SCREEN
// ============================================================================
void DrawStartScreen(void) {
  // Draw Title
  const char *title = "TIC-TAC-TOE";
  int titleSize = ScaleSize(80);
  int titleWidth = MeasureText(title, titleSize);
  DrawText(title, ScaleX(640) - titleWidth / 2, ScaleY(100), titleSize,
           colorPrimary);

  const char *subtitle = "Classic Strategy Game";
  int subtitleSize = ScaleSize(25);
  int subtitleWidth = MeasureText(subtitle, subtitleSize);
  DrawText(subtitle, ScaleX(640) - subtitleWidth / 2, ScaleY(190), subtitleSize,
           colorLight);

  // Define button rectangles (using the centering helper)
    Rectangle playButton = CreateButton(ScaleX(640), ScaleY(260), ScaleSize(250), ScaleSize(70));
    Rectangle loadButton = CreateButton(ScaleX(640), ScaleY(350), ScaleSize(250), ScaleSize(70));
    Rectangle deleteButton = CreateButton(ScaleX(640 + 170), ScaleY(350), ScaleSize(60), ScaleSize(60));
    Rectangle instructionsButton = CreateButton(ScaleX(640 - 130), ScaleY(440), ScaleSize(250), ScaleSize(70));
    Rectangle historyButton = CreateButton(ScaleX(640 + 130), ScaleY(440), ScaleSize(250), ScaleSize(70));
    Rectangle fullscreenButton = CreateButton(ScaleX(640), ScaleY(530), ScaleSize(250), ScaleSize(60));
    Rectangle themesButton = CreateButton(ScaleX(1280 - 90), ScaleY(720 - 40), ScaleSize(160), ScaleSize(50));

  // Draw the buttons
  DrawButton(&playButton, "PLAY", colorSecondary);
  DrawButton(&loadButton, "LOAD GAME", colorAccent);
  DrawButton(&deleteButton, "DEL", colorWarning);
  DrawButton(&instructionsButton, "INSTRUCTIONS", colorPrimary);
  DrawButton(&historyButton, "HISTORY", colorPrimary);
  DrawButton(&themesButton, "THEMES", colorDark);

  if (game.saveMessageTimer > 0) {
    int msgSize = ScaleSize(20);
    int textWidth = MeasureText(game.saveMessage, msgSize);
    DrawText(game.saveMessage, ScaleX(640) - textWidth / 2, ScaleY(565),
             msgSize, colorAccent);
  }

  // Fullscreen button text changes based on current state
  const char *fullscreenText =
      game.isFullscreen ? "WINDOWED MODE" : "FULLSCREEN";
  DrawButton(&fullscreenButton, fullscreenText, colorWarning);
}

void HandleStartScreen(void) {
  if (game.saveMessageTimer > 0)
    game.saveMessageTimer -= GetFrameTime();

  // Re-create button rectangles to check for collisions
    Rectangle playButton = CreateButton(ScaleX(640), ScaleY(260), ScaleSize(250), ScaleSize(70));
    Rectangle loadButton = CreateButton(ScaleX(640), ScaleY(350), ScaleSize(250), ScaleSize(70));
    Rectangle deleteButton = CreateButton(ScaleX(640 + 170), ScaleY(350), ScaleSize(60), ScaleSize(60));
    Rectangle instructionsButton = CreateButton(ScaleX(640 - 130), ScaleY(440), ScaleSize(250), ScaleSize(70));
    Rectangle historyButton = CreateButton(ScaleX(640 + 130), ScaleY(440), ScaleSize(250), ScaleSize(70));
    Rectangle fullscreenButton = CreateButton(ScaleX(640), ScaleY(530), ScaleSize(250), ScaleSize(60));
    Rectangle themesButton = CreateButton(ScaleX(1280 - 90), ScaleY(720 - 40), ScaleSize(160), ScaleSize(50));

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (IsButtonHovered(&playButton)) {
      PlaySound(game.sfx.click);
      game.screen = SCREEN_MODE_SELECT;
    } else if (IsButtonHovered(&loadButton)) {
      if (LoadGame()) {
        PlaySound(game.sfx.click);
        game.screen = SCREEN_GAME;
      }
    } else if (IsButtonHovered(&deleteButton)) {
      PlaySound(game.sfx.click);
      DeleteSaveGame();
      // Note: We stay on the same screen, but the "Save Deleted" message will
      // appear
    } else if (IsButtonHovered(&themesButton)) {
      PlaySound(game.sfx.click);
      game.screen = SCREEN_THEME_SELECT;
    } else if (IsButtonHovered(&instructionsButton)) {
      PlaySound(game.sfx.click);
      game.screen = SCREEN_INSTRUCTIONS;
    } else if (IsButtonHovered(&historyButton)) {
      PlaySound(game.sfx.click);
      LoadGameHistory();
      game.screen = SCREEN_HISTORY;
    } else if (IsButtonHovered(&fullscreenButton)) {
      PlaySound(game.sfx.click);
      game.isFullscreen = !game.isFullscreen;
      if (game.isFullscreen) {
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        SetWindowPosition(0, 0);
        SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
      } else {
        ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
        SetWindowSize(1280, 720);
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
void DrawModeSelectScreen(void) {
  const char *title = "SELECT MODE";
  int titleSize = ScaleSize(60);
  int titleWidth = MeasureText(title, titleSize);
  DrawText(title, ScaleX(640) - titleWidth / 2, ScaleY(80), titleSize,
           colorPrimary);

  Rectangle onePlayerButton =
      CreateButton(ScaleX(640), ScaleY(250), ScaleSize(300), ScaleSize(80));
  Rectangle twoPlayerButton =
      CreateButton(ScaleX(640), ScaleY(370), ScaleSize(300), ScaleSize(80));
  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(490), ScaleSize(200), ScaleSize(60));

  DrawButton(&onePlayerButton, "1 PLAYER", colorSecondary);
  DrawButton(&twoPlayerButton, "2 PLAYERS", colorWarning);
  DrawButton(&backButton, "BACK", colorDark);
}

void HandleModeSelectScreen(void) {
  Rectangle onePlayerButton =
      CreateButton(ScaleX(640), ScaleY(250), ScaleSize(300), ScaleSize(80));
  Rectangle twoPlayerButton =
      CreateButton(ScaleX(640), ScaleY(370), ScaleSize(300), ScaleSize(80));
  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(490), ScaleSize(200), ScaleSize(60));

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (IsButtonHovered(&onePlayerButton)) {
      PlaySound(game.sfx.click);
      game.mode = MODE_ONE_PLAYER;
      game.screen = SCREEN_DIFFICULTY_SELECT;
    } else if (IsButtonHovered(&twoPlayerButton)) {
      PlaySound(game.sfx.click);
      game.mode = MODE_TWO_PLAYER;
      game.screen = SCREEN_SYMBOL_SELECT_2P;
    } else if (IsButtonHovered(&backButton)) {
      PlaySound(game.sfx.click);
      game.screen = SCREEN_START;
    }
  }
}

// ============================================================================
// DIFFICULTY SELECT SCREEN
// ============================================================================
void DrawDifficultySelectScreen(void) {
  const char *title = "SELECT DIFFICULTY";
  int titleSize = ScaleSize(55);
  int titleWidth = MeasureText(title, titleSize);
  DrawText(title, ScaleX(640) - titleWidth / 2, ScaleY(70), titleSize,
           colorPrimary);

    Rectangle easyButton = CreateButton(ScaleX(640), ScaleY(200), ScaleSize(280), ScaleSize(75));
    Rectangle mediumButton = CreateButton(ScaleX(640), ScaleY(305), ScaleSize(280), ScaleSize(75));
    Rectangle hardButton = CreateButton(ScaleX(640), ScaleY(410), ScaleSize(280), ScaleSize(75));
    Rectangle backButton = CreateButton(ScaleX(640), ScaleY(520), ScaleSize(200), ScaleSize(60));

  DrawButton(&easyButton, "EASY", colorSecondary);
  DrawButton(&mediumButton, "MEDIUM", colorWarning);
  DrawButton(&hardButton, "HARD", colorAccent);
  DrawButton(&backButton, "BACK", colorDark);
}

void HandleDifficultySelectScreen(void)
{
    Rectangle easyButton = CreateButton(ScaleX(640), ScaleY(200), ScaleSize(280), ScaleSize(75));
    Rectangle mediumButton = CreateButton(ScaleX(640), ScaleY(305), ScaleSize(280), ScaleSize(75));
    Rectangle hardButton = CreateButton(ScaleX(640), ScaleY(410), ScaleSize(280), ScaleSize(75));
    Rectangle backButton = CreateButton(ScaleX(640), ScaleY(520), ScaleSize(200), ScaleSize(60));

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (IsButtonHovered(&easyButton)) {
      PlaySound(game.sfx.click);
      game.difficulty = DIFF_EASY;
      game.screen = SCREEN_SYMBOL_SELECT_1P;
    } else if (IsButtonHovered(&mediumButton)) {
      PlaySound(game.sfx.click);
      game.difficulty = DIFF_MEDIUM;
      game.screen = SCREEN_SYMBOL_SELECT_1P;
    } else if (IsButtonHovered(&hardButton)) {
      PlaySound(game.sfx.click);
      game.difficulty = DIFF_HARD;
      game.screen = SCREEN_SYMBOL_SELECT_1P;
    } else if (IsButtonHovered(&backButton)) {
      PlaySound(game.sfx.click);
      game.screen = SCREEN_MODE_SELECT;
    }
  }
}

// ============================================================================
// SYMBOL SELECT SCREEN
// ============================================================================
void DrawSymbolSelectScreen(bool isPlayer1) {
  const char *title =
      isPlayer1 ? "PLAYER 1: CHOOSE SYMBOL" : "CHOOSE YOUR SYMBOL";
  int titleSize = ScaleSize(50);
  int titleWidth = MeasureText(title, titleSize);
  DrawText(title, ScaleX(640) - titleWidth / 2, ScaleY(80), titleSize,
           colorPrimary);

  Rectangle xButton = CreateButton(ScaleX(640 - 130), ScaleY(280),
                                   ScaleSize(180), ScaleSize(180));
  Rectangle oButton = CreateButton(ScaleX(640 + 130), ScaleY(280),
                                   ScaleSize(180), ScaleSize(180));
  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(500), ScaleSize(200), ScaleSize(60));

  DrawButton(&xButton, "X", colorPrimary);
  DrawButton(&oButton, "O", colorAccent);
  DrawButton(&backButton, "BACK", colorDark);
}

void HandleSymbolSelectScreen(bool isPlayer1) {
  Rectangle xButton = CreateButton(ScaleX(640 - 130), ScaleY(280),
                                   ScaleSize(180), ScaleSize(180));
  Rectangle oButton = CreateButton(ScaleX(640 + 130), ScaleY(280),
                                   ScaleSize(180), ScaleSize(180));
  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(500), ScaleSize(200), ScaleSize(60));

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (IsButtonHovered(&xButton)) {
      PlaySound(game.sfx.click);
      if (game.mode == MODE_ONE_PLAYER) {
        game.humanSymbol = 'X';
        game.aiSymbol = 'O';
      } else {
        game.humanSymbol = 'X';
      }
      ResetBoard();
      game.screen = SCREEN_GAME;
    } else if (IsButtonHovered(&oButton)) {
      PlaySound(game.sfx.click);
      if (game.mode == MODE_ONE_PLAYER) {
        game.humanSymbol = 'O';
        game.aiSymbol = 'X';
      } else {
        game.humanSymbol = 'O';
      }
      ResetBoard();
      game.screen = SCREEN_GAME;
    } else if (IsButtonHovered(&backButton)) {
      PlaySound(game.sfx.click);
      if (isPlayer1) {
        game.screen = SCREEN_MODE_SELECT;
      } else {
        game.screen = SCREEN_DIFFICULTY_SELECT;
      }
    }
  }
}

// ============================================================================
// INSTRUCTIONS SCREEN
// ============================================================================
void DrawInstructionsScreen(void) {
  const char *title = "INSTRUCTIONS";
  int titleSize = ScaleSize(60);
  int titleWidth = MeasureText(title, titleSize);
  DrawText(title, ScaleX(640) - titleWidth / 2, ScaleY(50), titleSize,
           colorPrimary);

  float boxWidth = ScaleSize(600);
  float boxHeight = ScaleSize(380);
  float boxX = ScaleX(640) - boxWidth / 2;
  float boxY = ScaleY(130);
  float textX = boxX + ScaleSize(20);

  DrawRectangleRec((Rectangle){boxX, boxY, boxWidth, boxHeight}, colorLight);
  DrawRectangleLinesEx((Rectangle){boxX, boxY, boxWidth, boxHeight},
                       ScaleSize(3), colorPrimary);

  const char *instructions[] = {
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
      "Click on an empty square to place your symbol!"};

  float yPos = boxY + ScaleSize(20);
  int textSize = ScaleSize(22);
  for (int i = 0; i < 13; i++) {
    DrawText(instructions[i], textX, yPos, textSize, colorDark);
    yPos += ScaleSize(28);
  }

  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(540), ScaleSize(200), ScaleSize(50));
  DrawButton(&backButton, "BACK", colorPrimary);
}

void HandleInstructionsScreen(void) {
  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(540), ScaleSize(200), ScaleSize(50));
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsButtonHovered(&backButton)) {
    PlaySound(game.sfx.click); // <--- SOUND ADDED
    game.screen = SCREEN_START;
  }
}

// ============================================================================
// GAME SCREEN (FIXED LOGIC)
// ============================================================================
void DrawGameScreen(void) {
  // --- 1. Draw Header & Scores ---
  const char *header =
      (game.mode == MODE_ONE_PLAYER) ? "YOU vs AI" : "PLAYER 1 vs PLAYER 2";
  int headerSize = ScaleSize(40);
  int headerWidth = MeasureText(header, headerSize);
  DrawText(header, ScaleX(640) - headerWidth / 2, ScaleY(30), headerSize,
           colorPrimary);

  char scoreText[64];
  if (game.mode == MODE_ONE_PLAYER) {
    sprintf(scoreText, "You: %d | AI: %d | Draws: %d", game.player1Wins,
            game.player2Wins, game.draws);
  } else {
    sprintf(scoreText, "P1: %d | P2: %d | Draws: %d", game.player1Wins,
            game.player2Wins, game.draws);
  }

  int scoreSize = ScaleSize(22);
  int scoreWidth = MeasureText(scoreText, scoreSize);
  DrawText(scoreText, ScaleX(640) - scoreWidth / 2, ScaleY(80), scoreSize,
           colorLight);

  // --- 2. Draw Current Turn Text ---
  if (!game.gameOver) {
    char turnText[64];
    if (game.mode == MODE_ONE_PLAYER) {
      sprintf(turnText, game.aiTurn ? "AI's Turn" : "Your Turn");
    } else {
      char p1Symbol = game.humanSymbol;
      char p2Symbol = (game.humanSymbol == 'X') ? 'O' : 'X';
      if (game.currentPlayer == p1Symbol) {
        sprintf(turnText, "Player 1's Turn (%c)", p1Symbol);
      } else {
        sprintf(turnText, "Player 2's Turn (%c)", p2Symbol);
      }
    }

    int turnSize = ScaleSize(28);
    int turnWidth = MeasureText(turnText, turnSize);
    Color turnColor = game.aiTurn ? colorAccent : colorSecondary;
    DrawText(turnText, ScaleX(640) - turnWidth / 2, ScaleY(120), turnSize,
             turnColor);
  }

  // --- 3. Draw the Game Board & Pieces ---
  float boardSize = ScaleSize(360);
  float boardX = ScaleX(640) - boardSize / 2;
  float boardY = ScaleY(180);
  float cellSize = boardSize / 3;

  // Draw grid background
  DrawRectangleRec((Rectangle){boardX - ScaleSize(10), boardY - ScaleSize(10),
                               boardSize + ScaleSize(20),
                               boardSize + ScaleSize(20)},
                   colorLight);

  // Draw grid lines
  for (int i = 1; i < 3; i++) {
    DrawLineEx((Vector2){boardX + i * cellSize, boardY},
               (Vector2){boardX + i * cellSize, boardY + boardSize},
               ScaleSize(5), colorDark);
    DrawLineEx((Vector2){boardX, boardY + i * cellSize},
               (Vector2){boardX + boardSize, boardY + i * cellSize},
               ScaleSize(5), colorDark);
  }

  // Draw X's and O's by iterating over the game state
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      float x = boardX + j * cellSize + cellSize / 2;
      float y = boardY + i * cellSize + cellSize / 2;

      if (game.board[i][j] == 'X') {
        float offset = cellSize * 0.25f;
        DrawLineEx((Vector2){x - offset, y - offset},
                   (Vector2){x + offset, y + offset}, ScaleSize(8),
                   colorPrimary);
        DrawLineEx((Vector2){x + offset, y - offset},
                   (Vector2){x - offset, y + offset}, ScaleSize(8),
                   colorPrimary);
      } else if (game.board[i][j] == 'O') {
        DrawRing((Vector2){x, y}, cellSize * 0.25f, cellSize * 0.3f, 0, 360, 32,
                 colorAccent);
      }

      // Draw hover effect
      if (!game.gameOver && !game.aiTurn && game.board[i][j] == ' ') {
        Rectangle cell = {boardX + j * cellSize, boardY + i * cellSize,
                          cellSize, cellSize};
        if (CheckCollisionPointRec(GetMousePosition(), cell)) {
          DrawRectangle(cell.x, cell.y, cell.width, cell.height,
                        (Color){100, 100, 100, 50});
        }
      }
    }
  }

  // --- NEW: Draw Winning Line Highlight ---
  if (game.gameOver && game.winner != ' ') {
    Vector2 lineStart = {0};
    Vector2 lineEnd = {0};
    bool foundWinningLine = false;

    // Check rows
    for (int i = 0; i < 3; i++) {
      if (game.board[i][0] == game.winner && game.board[i][1] == game.winner &&
          game.board[i][2] == game.winner) {
        lineStart = (Vector2){boardX, boardY + i * cellSize + cellSize / 2};
        lineEnd =
            (Vector2){boardX + boardSize, boardY + i * cellSize + cellSize / 2};
        foundWinningLine = true;
        break;
      }
    }

    // Check columns
    if (!foundWinningLine) {
      for (int j = 0; j < 3; j++) {
        if (game.board[0][j] == game.winner &&
            game.board[1][j] == game.winner &&
            game.board[2][j] == game.winner) {
          lineStart = (Vector2){boardX + j * cellSize + cellSize / 2, boardY};
          lineEnd = (Vector2){boardX + j * cellSize + cellSize / 2,
                              boardY + boardSize};
          foundWinningLine = true;
          break;
        }
      }
    }

    // Check diagonals
    if (!foundWinningLine) {
      if (game.board[0][0] == game.winner && game.board[1][1] == game.winner &&
          game.board[2][2] == game.winner) {
        lineStart = (Vector2){boardX, boardY};
        lineEnd = (Vector2){boardX + boardSize, boardY + boardSize};
        foundWinningLine = true;
      } else if (game.board[0][2] == game.winner &&
                 game.board[1][1] == game.winner &&
                 game.board[2][0] == game.winner) {
        lineStart = (Vector2){boardX + boardSize, boardY};
        lineEnd = (Vector2){boardX, boardY + boardSize};
        foundWinningLine = true;
      }
    }

    if (foundWinningLine) {
      DrawLineEx(lineStart, lineEnd, ScaleSize(15), colorWarning);
    }
  }

  // --- 4. Draw Save Message (if timer is active) ---
  if (game.saveMessageTimer > 0) {
    int msgSize = ScaleSize(20);
    int textWidth = MeasureText(game.saveMessage, msgSize);
    Color msgColor = (strncmp(game.saveMessage, "ERROR", 5) == 0)
                         ? colorAccent
                         : colorSecondary;
    DrawText(game.saveMessage, ScaleX(640) - textWidth / 2, ScaleY(550),
             msgSize, msgColor);
  }

  // --- 5. Draw Undo Button (conditionally) ---
  if (game.moveCount > 0) {
    Rectangle undoButton = CreateButton(ScaleX(640 + 270), ScaleY(215),
                                        ScaleSize(120), ScaleSize(50));
    DrawButton(&undoButton, "UNDO", colorAccent);
  }

  // --- 6. Draw Bottom Buttons ---
  Rectangle restartButton = CreateButton(ScaleX(640 - 170), ScaleY(595),
                                         ScaleSize(140), ScaleSize(50));
  Rectangle saveButton =
      CreateButton(ScaleX(640), ScaleY(595), ScaleSize(140), ScaleSize(50));
  Rectangle menuButton = CreateButton(ScaleX(640 + 170), ScaleY(595),
                                      ScaleSize(140), ScaleSize(50));

  DrawButton(&restartButton, "RESTART", colorWarning);
  DrawButton(&saveButton, "SAVE", colorSecondary);
  DrawButton(&menuButton, "MENU", colorDark);
}

void HandleGameScreen(void) {
  // 1. Update Timer
  if (game.saveMessageTimer > 0)
    game.saveMessageTimer -= GetFrameTime();

  // 2. Get Mouse Status ONCE per frame to avoid conflict
  bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
  Vector2 mousePos = GetMousePosition();

  // 3. Define Buttons
  Rectangle restartButton = CreateButton(ScaleX(640 - 170), ScaleY(595),
                                         ScaleSize(140), ScaleSize(50));
  Rectangle saveButton =
      CreateButton(ScaleX(640), ScaleY(595), ScaleSize(140), ScaleSize(50));
  Rectangle menuButton = CreateButton(ScaleX(640 + 170), ScaleY(595),
                                      ScaleSize(140), ScaleSize(50));
  Rectangle undoButton = CreateButton(ScaleX(640 + 270), ScaleY(215),
                                      ScaleSize(120), ScaleSize(50));

  // 4. Handle Mouse Clicks (Buttons OR Board)
  if (mouseClicked) {
    if (IsButtonHovered(&restartButton)) {
      PlaySound(game.sfx.click);
      ResetBoard();
      return;
    } else if (game.moveCount > 0 && IsButtonHovered(&undoButton)) {
      PlaySound(game.sfx.click);
      if (game.moveCount > 0) {
        game.moveCount--;
        memcpy(game.board, game.moveHistory[game.moveCount].board,
               sizeof(game.board));
        game.currentPlayer = game.moveHistory[game.moveCount].currentPlayer;
        game.aiTurn = game.moveHistory[game.moveCount].aiTurn;
        game.gameOver = false;
        game.winner = ' ';
        game.saveMessageTimer = 0.0f;
      }
      return;
    } else if (IsButtonHovered(&saveButton)) {
      PlaySound(game.sfx.click);
      SaveGame();
      return;
    } else if (IsButtonHovered(&menuButton)) {
      PlaySound(game.sfx.click);
      game.screen = SCREEN_START;
      return;
    }

    // --- CHECK BOARD INPUT ---
    // Only allowed if game is NOT over AND it's NOT the AI's turn
    if (!game.gameOver && !(game.mode == MODE_ONE_PLAYER && game.aiTurn)) {
      float boardSize = ScaleSize(360);
      float boardX = ScaleX(640) - boardSize / 2;
      float boardY = ScaleY(180);
      float cellSize = boardSize / 3;

      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          Rectangle cell = {boardX + j * cellSize, boardY + i * cellSize,
                            cellSize, cellSize};

          if (CheckCollisionPointRec(mousePos, cell) &&
              game.board[i][j] == ' ') {
            // Undo Saving
            if (game.moveCount >= game.moveCapacity) {
              int newCapacity =
                  (game.moveCapacity == 0) ? 4 : game.moveCapacity * 2;
              MoveSnapshot *newHistory = (MoveSnapshot *)realloc(
                  game.moveHistory, newCapacity * sizeof(MoveSnapshot));
              if (newHistory != NULL) {
                game.moveHistory = newHistory;
                game.moveCapacity = newCapacity;
              } else
                goto skip_move_save;
            }
            memcpy(game.moveHistory[game.moveCount].board, game.board,
                   sizeof(game.board));
            game.moveHistory[game.moveCount].currentPlayer = game.currentPlayer;
            game.moveHistory[game.moveCount].aiTurn = game.aiTurn;
            game.moveCount++;

          skip_move_save:

            // Place Symbol & Sound
            game.board[i][j] = game.currentPlayer;
            PlaySound(game.sfx.click);

            // Check Win/Draw
            if (CheckWinner(game.board) || IsBoardFull(game.board)) {
              if (!game.gameOver)
                AppendGameToHistory();
              game.gameOver = true;
              game.screen = SCREEN_GAME_OVER;

              if (game.winner == ' ') {
                PlaySound(game.sfx.draw);
              } else {
                PlaySound(game.sfx.win);
              }
              return;
            }

            // Switch Turn
            if (game.mode == MODE_ONE_PLAYER) {
              game.aiTurn = true;
              game.aiMoveTimer = 0.5f;
            } else {
              game.currentPlayer = (game.currentPlayer == 'X') ? 'O' : 'X';
            }

            // IMPORTANT: Return immediately to stop processing this click
            return;
          }
        }
      }
    }
  }

  // 5. Handle AI Turn Logic (Runs automatically)
  if (!game.gameOver && game.mode == MODE_ONE_PLAYER && game.aiTurn) {
    game.aiMoveTimer -= GetFrameTime();
    if (game.aiMoveTimer <= 0) {
      MakeAIMove();
      PlaySound(game.sfx.click);

      game.aiTurn = false;
      if (CheckWinner(game.board) || IsBoardFull(game.board)) {
        if (!game.gameOver)
          AppendGameToHistory();
        game.gameOver = true;
        game.screen = SCREEN_GAME_OVER;

        if (game.winner == ' ') {
          PlaySound(game.sfx.draw);
        } else {
          PlaySound(game.sfx.lose);
        }
      }
    }
  }
}

// ============================================================================
// GAME OVER SCREEN
// ============================================================================
void DrawGameOverScreen(void) {
  // Draw semi-transparent panel at TOP of screen
  float panelWidth = ScaleSize(500);
  float panelHeight = ScaleSize(200);
  float panelX = ScaleX(640) - panelWidth / 2;
  float panelY = ScaleY(20);

  DrawRectangleRec((Rectangle){panelX, panelY, panelWidth, panelHeight},
                   (Color){44, 62, 80, 230});
  DrawRectangleLinesEx((Rectangle){panelX, panelY, panelWidth, panelHeight},
                       ScaleSize(4), colorPrimary);

  // Result text
  const char *resultText;
  Color resultColor;

  if (game.winner == ' ') {
    resultText = "IT'S A DRAW!";
    resultColor = colorWarning;
  } else if (game.mode == MODE_ONE_PLAYER) {
    if (game.winner == game.humanSymbol) {
      resultText = "YOU WIN!";
      resultColor = colorSecondary;
    } else {
      resultText = "AI WINS!";
      resultColor = colorAccent;
    }
  } else {
    char p1Symbol = game.humanSymbol;
    if (game.winner == p1Symbol) {
      resultText =
          (p1Symbol == 'X') ? "PLAYER 1 (X) WINS!" : "PLAYER 1 (O) WINS!";
      resultColor = colorSecondary;
    } else {
      resultText =
          (p1Symbol == 'X') ? "PLAYER 2 (O) WINS!" : "PLAYER 2 (X) WINS!";
      resultColor = colorSecondary;
    }
  }

  int resultSize = ScaleSize(45);
  int resultWidth = MeasureText(resultText, resultSize);
  DrawText(resultText, ScaleX(640) - resultWidth / 2, panelY + ScaleSize(30),
           resultSize, resultColor);

  // Score display
  char scoreText[128];
  if (game.mode == MODE_ONE_PLAYER) {
    sprintf(scoreText, "You: %d | AI: %d | Draws: %d", game.player1Wins,
            game.player2Wins, game.draws);
  } else {
    sprintf(scoreText, "P1: %d | P2: %d | Draws: %d", game.player1Wins,
            game.player2Wins, game.draws);
  }

  int scoreSize = ScaleSize(18);
  int scoreWidth = MeasureText(scoreText, scoreSize);
  DrawText(scoreText, ScaleX(640) - scoreWidth / 2, panelY + ScaleSize(90),
           scoreSize, colorLight);

  // Buttons
  Rectangle playAgainButton =
      CreateButton(ScaleX(640 - 130), panelY + ScaleSize(150), ScaleSize(220),
                   ScaleSize(45));
  Rectangle menuButton =
      CreateButton(ScaleX(640 + 130), panelY + ScaleSize(150), ScaleSize(220),
                   ScaleSize(45));

  DrawButton(&playAgainButton, "PLAY AGAIN", colorSecondary);
  DrawButton(&menuButton, "MAIN MENU", colorDark);
}

void HandleGameOverScreen(void) {
  float panelY = ScaleY(20);
  Rectangle playAgainButton =
      CreateButton(ScaleX(640 - 130), panelY + ScaleSize(150), ScaleSize(220),
                   ScaleSize(45));
  Rectangle menuButton =
      CreateButton(ScaleX(640 + 130), panelY + ScaleSize(150), ScaleSize(220),
                   ScaleSize(45));

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (IsButtonHovered(&playAgainButton)) {
      PlaySound(game.sfx.click); // <--- SOUND ADDED
      ResetBoard();
      game.screen = SCREEN_GAME;
    } else if (IsButtonHovered(&menuButton)) {
      PlaySound(game.sfx.click); // <--- SOUND ADDED
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
void DrawThemeSelectScreen(void) {
  const char *title = "SELECT THEME";
  int titleSize = ScaleSize(60);
  int titleWidth = MeasureText(title, titleSize);
  DrawText(title, ScaleX(640) - titleWidth / 2, ScaleY(60), titleSize,
           colorPrimary);

  Rectangle defaultButton =
      CreateButton(ScaleX(640), ScaleY(150), ScaleSize(280), ScaleSize(60));
  Rectangle darkButton =
      CreateButton(ScaleX(640), ScaleY(220), ScaleSize(280), ScaleSize(60));
  Rectangle forestButton =
      CreateButton(ScaleX(640), ScaleY(290), ScaleSize(280), ScaleSize(60));
  Rectangle spaceButton =
      CreateButton(ScaleX(640), ScaleY(360), ScaleSize(280), ScaleSize(60));
  Rectangle aquaticButton =
      CreateButton(ScaleX(640), ScaleY(430), ScaleSize(280), ScaleSize(60));
  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(520), ScaleSize(200), ScaleSize(60));

  DrawButton(&defaultButton, "DEFAULT", colorSecondary);
  DrawButton(&darkButton, "DARK", colorSecondary);
  DrawButton(&forestButton, "FOREST", colorSecondary);
  DrawButton(&spaceButton, "SPACE", colorSecondary);
  DrawButton(&aquaticButton, "AQUATIC", colorSecondary);
  DrawButton(&backButton, "BACK", colorDark);

  if (game.currentTheme == THEME_DEFAULT)
    DrawRectangleLinesEx(defaultButton, ScaleSize(5), colorAccent);
  if (game.currentTheme == THEME_DARK)
    DrawRectangleLinesEx(darkButton, ScaleSize(5), colorAccent);
  if (game.currentTheme == THEME_FOREST)
    DrawRectangleLinesEx(forestButton, ScaleSize(5), colorAccent);
  if (game.currentTheme == THEME_SPACE)
    DrawRectangleLinesEx(spaceButton, ScaleSize(5), colorAccent);
  if (game.currentTheme == THEME_AQUATIC)
    DrawRectangleLinesEx(aquaticButton, ScaleSize(5), colorAccent);
}

void HandleThemeSelectScreen(void) {
  Rectangle defaultButton =
      CreateButton(ScaleX(640), ScaleY(150), ScaleSize(280), ScaleSize(60));
  Rectangle darkButton =
      CreateButton(ScaleX(640), ScaleY(220), ScaleSize(280), ScaleSize(60));
  Rectangle forestButton =
      CreateButton(ScaleX(640), ScaleY(290), ScaleSize(280), ScaleSize(60));
  Rectangle spaceButton =
      CreateButton(ScaleX(640), ScaleY(360), ScaleSize(280), ScaleSize(60));
  Rectangle aquaticButton =
      CreateButton(ScaleX(640), ScaleY(430), ScaleSize(280), ScaleSize(60));
  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(520), ScaleSize(200), ScaleSize(60));

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (IsButtonHovered(&defaultButton)) {
      PlaySound(game.sfx.click);
      ChangeTheme(THEME_DEFAULT);
    } else if (IsButtonHovered(&darkButton)) {
      PlaySound(game.sfx.click);
      ChangeTheme(THEME_DARK);
    } else if (IsButtonHovered(&forestButton)) {
      PlaySound(game.sfx.click);
      ChangeTheme(THEME_FOREST);
    } else if (IsButtonHovered(&spaceButton)) {
      PlaySound(game.sfx.click);
      ChangeTheme(THEME_SPACE);
    } else if (IsButtonHovered(&aquaticButton)) {
      PlaySound(game.sfx.click);
      ChangeTheme(THEME_AQUATIC);
    } else if (IsButtonHovered(&backButton)) {
      PlaySound(game.sfx.click);
      game.screen = SCREEN_START;
    }
  }
}

// ============================================================================
// HISTORY SCREEN
// ============================================================================
void DrawHistoryScreen(void) {
  const char *title = "GAME HISTORY";
  int titleSize = ScaleSize(60);
  int titleWidth = MeasureText(title, titleSize);
  DrawText(title, ScaleX(640) - titleWidth / 2, ScaleY(50), titleSize,
           colorPrimary);

  // Define the History Box
  float boxWidth = ScaleSize(700);
  float boxHeight = ScaleSize(450);
  float boxX = ScaleX(640) - boxWidth / 2;
  float boxY = ScaleY(130);

  DrawRectangleRec((Rectangle){boxX, boxY, boxWidth, boxHeight}, colorLight);
  DrawRectangleLinesEx((Rectangle){boxX, boxY, boxWidth, boxHeight},
                       ScaleSize(3), colorPrimary);

  if (game.historyLineCount == 0) {
    const char *msg = "No game history found. Go play a game!";
    int msgSize = ScaleSize(24);
    int msgWidth = MeasureText(msg, msgSize);
    DrawText(msg, ScaleX(640) - msgWidth / 2, ScaleY(300), msgSize, colorDark);
  } else {
    // --- SCROLLING LOGIC ---
    int textSize = ScaleSize(20);
    int lineHeight = ScaleSize(25);
    int padding = ScaleSize(15);

    // Calculate how many lines fit in the box
    int maxVisibleLines = (boxHeight - (padding * 2)) / lineHeight;

    // Clamp the scroll offset to valid bounds (0 to max_scroll)
    int maxScroll = game.historyLineCount - maxVisibleLines;
    if (maxScroll < 0)
      maxScroll = 0;

    if (game.historyScrollOffset < 0)
      game.historyScrollOffset = 0;
    if (game.historyScrollOffset > maxScroll)
      game.historyScrollOffset = maxScroll;

    // Draw visible lines
    float textX = boxX + padding;
    float textY = boxY + padding;

    for (int i = 0; i < maxVisibleLines; i++) {
      int lineIndex = game.historyScrollOffset + i;
      if (lineIndex >= game.historyLineCount)
        break;

      DrawText(game.gameHistory[lineIndex], textX, textY + (i * lineHeight),
               textSize, colorDark);
    }

    // Draw Scrollbar (Visual Feedback)
    if (game.historyLineCount > maxVisibleLines) {
      float scrollbarWidth = ScaleSize(10);
      float scrollbarHeight =
          boxHeight * ((float)maxVisibleLines / game.historyLineCount);
      float scrollbarX = boxX + boxWidth - scrollbarWidth - ScaleSize(5);

      // Calculate scrollbar Y position based on percentage scrolled
      float scrollPercent = (float)game.historyScrollOffset / maxScroll;
      float scrollbarY = boxY + (scrollPercent * (boxHeight - scrollbarHeight));

      DrawRectangle(scrollbarX, scrollbarY, scrollbarWidth, scrollbarHeight,
                    colorAccent);
    }
  }

  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(620), ScaleSize(200), ScaleSize(50));
  DrawButton(&backButton, "BACK", colorPrimary);

  if (game.historyLineCount > 0) {
    Rectangle clearButton = CreateButton(ScaleX(640 + 245), ScaleY(160),
                                         ScaleSize(200), ScaleSize(50));
    DrawButton(&clearButton, "CLEAR", colorAccent);
  }
}

void HandleHistoryScreen(void) {
  Rectangle backButton =
      CreateButton(ScaleX(640), ScaleY(620), ScaleSize(200), ScaleSize(50));
  Rectangle clearButton = CreateButton(ScaleX(640 + 245), ScaleY(160),
                                       ScaleSize(200), ScaleSize(50));

  // Handle Button Clicks
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (IsButtonHovered(&backButton)) {
      PlaySound(game.sfx.click); // <--- SOUND ADDED
      game.screen = SCREEN_START;
      game.historyScrollOffset = 0;
    } else if (game.historyLineCount > 0 && IsButtonHovered(&clearButton)) {
      PlaySound(game.sfx.click); // <--- SOUND ADDED
      ClearGameHistory();
      game.historyScrollOffset = 0;
    }
  }

  // Handle Mouse Wheel Scrolling
  float wheelMove = GetMouseWheelMove();
  if (wheelMove != 0) {
    // Scroll speed: 1 line per wheel click (invert because wheel up is
    // positive)
    game.historyScrollOffset -= (int)wheelMove;

    // Note: Clamping happens in Draw function to keep logic simple,
    // but we can also clamp here for strict correctness.
    if (game.historyScrollOffset < 0)
      game.historyScrollOffset = 0;
  }
}
