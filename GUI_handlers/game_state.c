/**
 * @file game_state.c
 * @brief Implementation of game state management functions
 * * This file implements all game state operations including initialization,
 * board management, win checking, and theme management.
 */

#include "game_state.h"
#include "../Game_algorithms/minimax.h" // For AI move functions
#include <stdio.h> // For file I/O (fopen, fwrite, fread, fclose)
#include <string.h> // For sprintf

/**
 * @brief Static definition of all available themes.
 * * This array stores the color palettes for every theme. The `ThemeID` enum
 * is used as a direct index into this array (e.g., `allThemes[THEME_DARK]`).
 * Defining them here keeps all theme data neatly organized in one place.
 */
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

// ============================================================================
// GLOBAL VARIABLE DEFINITIONS
// ============================================================================

/** * Global UI color variables.
 * These are the *actual* colors used by all drawing functions.
 * `ChangeTheme` works by copying colors from `allThemes` into these variables.
 * This decouples drawing logic from specific theme data.
 */
Color colorPrimary = {0};
Color colorSecondary = {0};
Color colorAccent = {0};
Color colorWarning = {0};
Color colorBackground = {0};
Color colorDark = {0};
Color colorLight = {0};

/** * Global game state instance.
 * This is the single, global "singleton" object that holds all game
 * state. It is initialized to all-zeros by default, and then
 * properly set by `InitGame()`.
 */
GameState game = {0};

// ============================================================================
// GAME INITIALIZATION FUNCTIONS
// ============================================================================

/**
 * @brief Initializes the game state for the first launch.
 * * This function is called *once* at startup by `main.c`.
 * It sets all game parameters to their default values, applies the
 * default theme, and resets the board.
 */
void InitGame(void)
{
    // Initialize screen and game mode
    game.screen = SCREEN_START;
    game.mode = MODE_ONE_PLAYER;
    game.difficulty = DIFF_MEDIUM;
    
    // Initialize player symbols
    game.humanSymbol = 'X';
    game.aiSymbol = 'O';
    game.currentPlayer = 'X';
    
    // Initialize game status
    game.gameOver = false;
    game.winner = ' ';
    game.aiTurn = false;
    game.aiMoveTimer = 0.5f; // Default AI "thinking" time
    
    // Initialize statistics
    game.player1Wins = 0;
    game.player2Wins = 0;
    game.draws = 0;
    
    // Initialize UI settings
    game.isFullscreen = false;
    game.saveMessageTimer = 0.0f; // Timer starts at 0 (hidden)
    game.saveMessage[0] = '\0';   // Clear save message buffer
    game.moveCount = 0;           // Initialize move history counter
    game.historyLineCount = 0;    // Initialize history line count
    ChangeTheme(THEME_DEFAULT);   // Apply the default theme
    
    // Initialize board
    ResetBoard();
}

/**
 * @brief Resets the board for a new game.
 * * This is called by `InitGame` but also by the "Play Again" button.
 * It's a separate function because we often want to start a new
 * round *without* resetting scores, theme, or game mode.
 */
void ResetBoard(void)
{
    // Clear all board cells
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game.board[i][j] = ' ';
        }
    }
    
    // Reset game state
    game.currentPlayer = game.humanSymbol; // Human (or P1) always starts
    game.gameOver = false;
    game.winner = ' ';
    
    // If human is 'O' in 1-player mode, the AI ('X') must go first.
    game.aiTurn = (game.mode == MODE_ONE_PLAYER && game.humanSymbol == 'O');
    game.aiMoveTimer = 0.5f;  // Delay before AI makes its first move
    game.saveMessageTimer = 0.0f; // Hide any save messages
    game.moveCount = 0;           // Reset move history for new game
}

// ============================================================================
// GAME STATUS CHECKING FUNCTIONS
// ============================================================================

// Forward declaration for helper function
static void updateWinStatistics(void);

/**
 * @brief Checks all 8 win conditions (3 rows, 3 cols, 2 diagonals).
 * * This function is called after every move. If a winner is found,
 * it sets `game.winner` and calls the `updateWinStatistics` helper
 * before returning `true`.
 */
bool CheckWinner(void)
{
    // Check all rows for three in a row
    for (int i = 0; i < 3; i++) {
        if (game.board[i][0] != ' ' &&
            game.board[i][0] == game.board[i][1] &&
            game.board[i][1] == game.board[i][2]) {
            game.winner = game.board[i][0];
            updateWinStatistics();
            return true;
        }
    }
    
    // Check all columns for three in a row
    for (int i = 0; i < 3; i++) {
        if (game.board[0][i] != ' ' && 
            game.board[0][i] == game.board[1][i] && 
            game.board[1][i] == game.board[2][i]) {
            game.winner = game.board[0][i];
            updateWinStatistics();
            return true;
        }
    }
    
    // Check main diagonal (top-left to bottom-right)
    if (game.board[0][0] != ' ' && 
        game.board[0][0] == game.board[1][1] && 
        game.board[1][1] == game.board[2][2]) {
        game.winner = game.board[0][0];
        updateWinStatistics();
        return true;
    }
    
    // Check anti-diagonal (top-right to bottom-left)
    if (game.board[0][2] != ' ' && 
        game.board[0][2] == game.board[1][1] && 
        game.board[1][1] == game.board[2][0]) {
        game.winner = game.board[0][2];
        updateWinStatistics();
        return true;
    }
    
    // No winner found
    return false;
}

/**
 * @brief Helper function to update win statistics.
 * * This logic is separated from `CheckWinner` to keep that function
 * clean and focused on just *finding* the win. This function handles
 * the "what to do *after* a win" logic, which differs based on game mode.
 */
static void updateWinStatistics(void)
{
    if (game.mode == MODE_ONE_PLAYER) {
        // In one-player mode, 'player1Wins' is human, 'player2Wins' is AI
        if (game.winner == game.humanSymbol) {
            game.player1Wins++;
        } else {
            game.player2Wins++;
        }
    } else {
        // In two-player mode, 'player1Wins' is X, 'player2Wins' is O
        if (game.winner == 'X') {
            game.player1Wins++;
        } else {
            game.player2Wins++;
        }
    }
}

/**
 * @brief Checks if the board is full (a draw condition).
 * * Iterates over the board. If any empty cell (' ') is found,
 * the board is not full, and it returns `false`.
 * If no empty cells are found, it's a draw, so it increments
 * the draw counter and returns `true`.
 */
bool IsBoardFull(void)
{
    // Check if any cell is empty
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game.board[i][j] == ' ') {
                return false; // Found an empty cell, not full
            }
        }
    }
    
    // Board is full - no winner, it's a draw
    game.winner = ' '; // Ensure winner is ' ' for draw
    game.draws++;
    return true;
}

// ============================================================================
// AI MOVE EXECUTION
// ============================================================================

/**
 * @brief Acts as a dispatcher to call the correct AI function.
 * * This high-level function reads `game.difficulty` and selects
 * the appropriate AI move-finding algorithm from the external
 * `minimax.h` library. This keeps `game_state.c` clean and
 * independent of the complex AI logic itself.
 */
void MakeAIMove(void)
{
    struct Move bestMove;
    
    // Select AI algorithm based on difficulty setting
    if (game.difficulty == DIFF_HARD) {
        // Perfect AI - unbeatable, uses alpha-beta pruning
        bestMove = findBestMovePerfect(game.board, game.aiSymbol);
    } else if (game.difficulty == DIFF_MEDIUM) {
        // Imperfect AI - makes mistakes, depth-limited minimax
        bestMove = findBestMoveImperfect(game.board, game.aiSymbol);
    } else {  // DIFF_EASY
        // Model-based AI - uses logistic regression evaluation
        bestMove = findBestMoveModel(game.board);
    }
    
    // Apply AI move to board if valid
    if (bestMove.row != -1 && bestMove.col != -1) {
        game.board[bestMove.row][bestMove.col] = game.aiSymbol;
        game.currentPlayer = game.humanSymbol;  // Switch back to human
    }
}

// ============================================================================
// THEME MANAGEMENT
// ============================================================================

/**
 * @brief Applies a new theme to the game.
 * * This is the core of the theme system. It validates the theme ID,
 * updates the `game.currentTheme` state, and then *copies* all colors
 * from the `allThemes` array into the global `Color` variables.
 * Every drawing function uses these global colors, so they will
 * all update on the next frame.
 */
void ChangeTheme(ThemeID newTheme)
{
    // Validate theme ID (prevent out-of-bounds access)
    if (newTheme >= THEME_COUNT) {
        newTheme = THEME_DEFAULT;
    }

    // Update current theme
    game.currentTheme = newTheme;

    // Copy theme colors to global variables used by all draw functions
    colorPrimary = allThemes[newTheme].primary;
    colorSecondary = allThemes[newTheme].secondary;
    colorAccent = allThemes[newTheme].accent;
    colorWarning = allThemes[newTheme].warning;
    colorBackground = allThemes[newTheme].background;
    colorDark = allThemes[newTheme].dark;
    colorLight = allThemes[newTheme].light;
}

// ============================================================================
// SAVE/LOAD FUNCTIONS
// ============================================================================

/**
 * @brief Saves the *entire* GameState struct to a binary file.
 * * This uses `fwrite` to write the raw bytes of the `game` struct
 * directly to "save.dat". This is simple but has drawbacks:
 * 1. Not portable (byte order, struct padding).
 * 2. Fragile: If `GameState` struct changes, old saves are invalid.
 * * It also sets a UI message (`game.saveMessage`) to give the user
 * feedback on the operation.
 */
void SaveGame(void)
{
    FILE* file = fopen("save.dat", "wb"); // "wb" = Write Binary
    if (file == NULL) {
        sprintf(game.saveMessage, "ERROR: Save Failed!");
        game.saveMessageTimer = 2.0f; // Show message for 2 seconds
        return;
    }

    // Write the entire 'game' struct (1 item of size GameState) to the file
    size_t itemsWritten = fwrite(&game, sizeof(GameState), 1, file);
    fclose(file);

    if (itemsWritten != 1) {
        sprintf(game.saveMessage, "ERROR: Write Failed!");
    } else {
        sprintf(game.saveMessage, "Game Saved!");
    }
    game.saveMessageTimer = 2.0f; // Show message for 2 seconds
}

/**
 * @brief Loads the GameState from "save.dat".
 * * This function contains critical safety logic to prevent crashes.
 * * 1. It remembers the user's *current* theme.
 * 2. It reads the file into a *temporary* `tempGame` struct, NOT the
 * live `game` struct.
 * 3. It *validates* the read (`itemsRead == 1`). If the file is empty or
 * corrupt, it aborts, leaving the current game state untouched.
 * 4. Only on success does it copy `tempGame` into the global `game`.
 * 5. It *restores* the user's original theme (a design choice).
 * 6. It *must* call `ChangeTheme` to re-apply the theme's colors,
 * as the global `Color` variables are not part of the saved struct.
 */
bool LoadGame(void)
{
    // 1. Remember the user's currently active theme
    ThemeID currentThemeBeforeLoad = game.currentTheme;

    FILE* file = fopen("save.dat", "rb"); // "rb" = Read Binary
    if (file == NULL) {
        return false; // No save file found
    }

    // 2. Read into a temporary variable, NOT the real 'game'
    GameState tempGame;
    size_t itemsRead = fread(&tempGame, sizeof(GameState), 1, file);
    fclose(file);

    // 3. CRITICAL: Check if the read was successful.
    // If itemsRead != 1, the file was empty or corrupted. Abort.
    // This check prevents a crash from loading bad data.
    if (itemsRead != 1) {
        return false;
    }

    // 4. Success! Now we can safely update the real 'game'
    game = tempGame;
    
    // 5. Restore the user's active theme (as requested)
    game.currentTheme = currentThemeBeforeLoad;
    
    // 6. Re-apply the colors for the active theme
    // This is necessary because the global Color... variables
    // are not part of the GameState struct and were not loaded.
    ChangeTheme(game.currentTheme);
    
    return true;
}

/**
 * @brief Appends the result of the completed game to "game_history.txt".
 * * This function is called once when a game ends.
 */
void AppendGameToHistory(void)
{
    // "a" mode = append (or create if it doesn't exist)
    FILE* file = fopen("game_history.txt", "a");
    if (file == NULL) {
        return; // Failed to open file, do nothing
    }

    char summary[128];

    if (game.mode == MODE_ONE_PLAYER)
    {
        // Get difficulty as a string
        const char* diff = "Unknown";
        if (game.difficulty == DIFF_HARD) diff = "Hard";
        if (game.difficulty == DIFF_MEDIUM) diff = "Medium";
        if (game.difficulty == DIFF_EASY) diff = "Easy";

        // Format the summary string
        if (game.winner == ' ') {
            sprintf(summary, "1P vs AI (%s): Draw", diff);
        } else if (game.winner == game.humanSymbol) {
            sprintf(summary, "1P vs AI (%s): You Win", diff);
        } else {
            sprintf(summary, "1P vs AI (%s): AI Wins", diff);
        }
    }
    else // MODE_TWO_PLAYER
    {
        if (game.winner == ' ') {
            sprintf(summary, "2P (Human vs Human): Draw");
        } else if (game.winner == 'X') {
            sprintf(summary, "2P (Human vs Human): Player 1 (X) Wins");
        } else {
            sprintf(summary, "2P (Human vs Human): Player 2 (O) Wins");
        }
    }

    fprintf(file, "%s\n", summary);
    fclose(file);
}

/**
 * @brief Loads the game history from "game_history.txt" into the state.
 * * Reads up to the first 20 lines from the file to be displayed.
 */
void LoadGameHistory(void)
{
    game.historyLineCount = 0;
    FILE* file = fopen("game_history.txt", "r"); // "r" mode = read
    if (file == NULL) {
        return; // No file, so historyLineCount stays 0
    }

    // Read up to 20 lines
    while (game.historyLineCount < 20 && 
           fgets(game.gameHistory[game.historyLineCount], 128, file) != NULL)
    {
        // Remove the newline character (\n) from the end of the line
        game.gameHistory[game.historyLineCount][strcspn(game.gameHistory[game.historyLineCount], "\n")] = 0;
        
        game.historyLineCount++;
    }

    fclose(file);
}

/**
 * @brief Clears all game history by truncating the file.
 * * It opens the file in "w" (write) mode, which immediately
 * * clears all its contents, and then closes it.
 * * It also resets the in-memory line count so the screen updates.
 */
void ClearGameHistory(void)
{
    // "w" mode = write (this truncates the file to 0 bytes)
    FILE* file = fopen("game_history.txt", "w");
    if (file != NULL) {
        fclose(file);
    }
    
    // Also clear the history currently loaded into memory
    game.historyLineCount = 0;
}