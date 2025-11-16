#include "game_state.h"
#include "../Team_B/minimax.h"
#include <stdio.h> // --- New for file saving ---

// --- NEW allThemes array ---

Theme allThemes[THEME_COUNT] = {
    [THEME_DEFAULT] = {
        .name = "Default",
        .primary = {52, 152, 219, 255},      // Blue
        .secondary = {46, 204, 113, 255},    // Green
        .accent = {231, 76, 60, 255},         // Red
        .warning = {241, 196, 15, 255},       // Yellow
        .background = {236, 240, 241, 255},  // Light Gray
        .dark = {44, 62, 80, 255},           // Dark Blue-Gray (Grid/Text)
        .light = {255, 255, 255, 255}         // White
    },
    [THEME_DARK] = {
    .name = "Dark",
    .primary = {52, 152, 219, 255},      // Blue
    .secondary = {46, 204, 113, 255},    // Green
    .accent = {231, 76, 60, 255},         // Red
    .warning = {241, 196, 15, 255},       // Yellow
    .background = {30, 40, 50, 255},       // Very Dark Blue (Page BG)
    .dark = {44, 62, 80, 255},           // Dark Blue-Gray (Text on light boxes)
    .light = {236, 240, 241, 255}         // Light Gray (Text on dark BG)
},
    [THEME_FOREST] = {
        .name = "Forest",
        .primary = {46, 139, 87, 255},        // Sea Green
        .secondary = {85, 107, 47, 255},     // Dark Olive
        .accent = {139, 69, 19, 255},         // Brown (Saddle)
        .warning = {218, 165, 32, 255},       // Goldenrod
        .background = {240, 248, 240, 255},  // Pale Honeydew
        .dark = {34, 54, 34, 255},           // Dark Forest Green (Grid/Text)
        .light = {255, 255, 255, 255}         // White
    },
    [THEME_SPACE] = {
        .name = "Space",
        .primary = {142, 68, 173, 255},      // Purple
        .secondary = {52, 73, 94, 255},       // Grayish Blue
        .accent = {241, 196, 15, 255},        // Yellow (Star)
        .warning = {231, 76, 60, 255},        // Red (Nebula)
        .background = {20, 25, 40, 255},       // Near Black (Page BG)
        .dark = {44, 62, 80, 255},           // Dark Blue-Gray (Text on light boxes)
        .light = {220, 220, 255, 255}         // Light Blue-White (Text on dark BG)
    },
    [THEME_AQUATIC] = {
        .name = "Aquatic",
        .primary = {26, 188, 156, 255},      // Turquoise
        .secondary = {52, 152, 219, 255},    // Blue
        .accent = {243, 156, 18, 255},       // Orange (Clownfish)
        .warning = {241, 196, 15, 255},       // Yellow (Sand)
        .background = {235, 250, 255, 255},  // Very Light Cyan
        .dark = {0, 80, 100, 255},           // Deep Teal (Grid/Text)
        .light = {255, 255, 255, 255}         // White
    }
};

// UI Colors definitions
Color colorPrimary = {0};
Color colorSecondary = {0};
Color colorAccent = {0};
Color colorWarning = {0};
Color colorBackground = {0};
Color colorDark = {0};
Color colorLight = {0};

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
    game.saveMessageTimer = 0.0f; // --- ADD THIS ---
    game.saveMessage[0] = '\0';   // --- ADD THIS ---
    ChangeTheme(THEME_DEFAULT);
    ResetBoard();
}

void ResetBoard(void)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            game.board[i][j] = ' ';
    
    game.currentPlayer = game.humanSymbol;
    game.gameOver = false;
    game.winner = ' ';
    game.aiTurn = (game.mode == MODE_ONE_PLAYER && game.humanSymbol == 'O');
    game.aiMoveTimer = 0.5f;
    game.saveMessageTimer = 0.0f; // --- ADD THIS ---
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
        bestMove = findBestMovePerfect(game.board, game.aiSymbol);
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
// --- ADD THIS ENTIRE FUNCTION ---
void ChangeTheme(ThemeID newTheme)
{
    if (newTheme >= THEME_COUNT) newTheme = THEME_DEFAULT;

    game.currentTheme = newTheme;

    // Copy the theme's colors into the global variables
    // that all your draw functions already use.
    colorPrimary = allThemes[newTheme].primary;
    colorSecondary = allThemes[newTheme].secondary;
    colorAccent = allThemes[newTheme].accent;
    colorWarning = allThemes[newTheme].warning;
    colorBackground = allThemes[newTheme].background;
    colorDark = allThemes[newTheme].dark;
    colorLight = allThemes[newTheme].light;
}

// --- NEW FUNCTION FOR FILE SAVING ---
void SaveGame(void)
{
    FILE* file = fopen("save.dat", "wb");
    if (file == NULL) {
        sprintf(game.saveMessage, "ERROR: Save Failed!");
        game.saveMessageTimer = 2.0f; // Show message for 2 seconds
        return;
    }

    size_t itemsWritten = fwrite(&game, sizeof(GameState), 1, file);
    fclose(file);

    if (itemsWritten != 1) {
        sprintf(game.saveMessage, "ERROR: Write Failed!");
    } else {
        sprintf(game.saveMessage, "Game Saved!");
    }
    game.saveMessageTimer = 2.0f; // Show message for 2 seconds
}

// --- ADD THIS ENTIRE FUNCTION ---
// In Team_A/game_state.c

bool LoadGame(void)
{
    // 1. Remember the user's currently active theme
    ThemeID currentThemeBeforeLoad = game.currentTheme;

    FILE* file = fopen("save.dat", "rb");
    if (file == NULL) {
        return false; // No file
    }

    // 2. Read into a temporary variable, NOT the real 'game'
    GameState tempGame;
    size_t itemsRead = fread(&tempGame, sizeof(GameState), 1, file);
    fclose(file);

    if (itemsRead != 1) {
        // 3. The file was empty or corrupted. Abort.
        // This is what stops the crash.
        return false;
    }

    // 4. Success! Now we can safely update the real 'game'
    game = tempGame;
    
    // 5. Restore the user's active theme (as you wanted)
    game.currentTheme = currentThemeBeforeLoad;
    
    // 6. Re-apply the colors for the active theme
    ChangeTheme(game.currentTheme);
    
    return true;
}
