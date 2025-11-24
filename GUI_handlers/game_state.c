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
// GLOBAL VARIABLES
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
// INITIALIZATION AND CLEANUP
// ============================================================================

/**
 * @brief Initializes the game state with default values.
 * Must be called once at the start of the program.
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
    
    game.currentTheme = THEME_DEFAULT;
    game.isFullscreen = false;
    
    // OPTIMIZATION: Initialize string pointer
    game.saveMessage = NULL;
    game.saveMessageTimer = 0.0f;
    
    // OPTIMIZATION: Initialize dynamic move history
    game.moveHistory = NULL;
    game.moveCount = 0;
    game.moveCapacity = 0;
    
    // OPTIMIZATION: Initialize dynamic game history
    game.gameHistory = NULL;
    game.historyLineCount = 0;
    game.historyCapacity = 0;
    game.historyScrollOffset = 0;

    // INITIALIZE SIMULATION
    game.simTimer = 0.0f;
    game.simStep = 0;

    // Load sound effects
    game.sfx.click     = LoadSound("resources/click.ogg"); // Buttons & Inputs
    game.sfx.win = LoadSound("resources/win.ogg");  // Win 1P
    game.sfx.lose       = LoadSound("resources/lose.ogg"); // Lose 1P
    game.sfx.draw      = LoadSound("resources/draw.ogg"); // Draw
    
    ChangeTheme(THEME_DEFAULT);
    ResetBoard();
}

/**
 * @brief OPTIMIZATION: Frees all dynamically allocated memory.
 * Must be called before program exit to prevent memory leaks.
 */
void CleanupGame(void) {
    // Free move history
    if (game.moveHistory != NULL) {
        free(game.moveHistory);
        game.moveHistory = NULL;
    }
    game.moveCount = 0;
    game.moveCapacity = 0;
    
    // Free game history
    if (game.gameHistory != NULL) {
        for (int i = 0; i < game.historyLineCount; i++) {
            free(game.gameHistory[i]);
        }
        free(game.gameHistory);
        game.gameHistory = NULL;
    }
    game.historyLineCount = 0;
    game.historyCapacity = 0;

    // Unload sound effects
    UnloadSound(game.sfx.click);
    UnloadSound(game.sfx.win);
    UnloadSound(game.sfx.lose);
    UnloadSound(game.sfx.draw);
}

// ============================================================================
// BOARD MANAGEMENT
// ============================================================================

/**
 * @brief Resets the game board to its initial state.
 * Clears all cells, resets turn order, and prepares for a new game.
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
// OPTIMIZATION: Lookup table for all 8 winning lines
static const int WIN_LINES[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // rows
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // columns
    {0, 4, 8}, {2, 4, 6}              // diagonals
};

bool CheckWinner(const char (*board)[3])
{
    // Cast 2D array to flat array for easier indexing
    const char *flat = (const char *)board;
    
    // Check all 8 winning lines using lookup table
    for (int line = 0; line < 8; line++) {
        int a = WIN_LINES[line][0];
        int b = WIN_LINES[line][1];
        int c = WIN_LINES[line][2];
        
        if (flat[a] != ' ' && 
            flat[a] == flat[b] && 
            flat[b] == flat[c]) {
            game.winner = flat[a];
            updateWinStatistics();
            return true;
        }
    }
    
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
// OPTIMIZATION: Made board parameter so it can check different boards
bool IsBoardFull(const char (*board)[3])
{
    // Cast to flat array for more efficient iteration
    const char *flat = (const char *)board;     // 9 cells total, faster to loop linearly
    
    // Check if any cell is empty
    for (int i = 0; i < 9; i++) {
        if (flat[i] == ' ') {
            return false;
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
        // Perfect AI: 0% Error Rate
        bestMove = findBestMoveMinimax(game.board, game.aiSymbol, 0);
    } else if (game.difficulty == DIFF_MEDIUM) {
        // Imperfect AI: 20% Forced Randomness
        // This is the "Forced Random Move" strategy
        bestMove = findBestMoveMinimax(game.board, game.aiSymbol, 20);
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
 * @brief Changes the current theme to the specified theme ID.
 * Updates all global color variables to match the new theme.
 * @param newTheme The ID of the theme to switch to.
 */
void ChangeTheme(ThemeID newTheme) {
    if (newTheme >= THEME_COUNT) {
        newTheme = THEME_DEFAULT;
    }

    // Update current theme
    game.currentTheme = newTheme;

    // Copy theme colors to globals
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
void SaveGame(void) {
    FILE* file = fopen("save.dat", "wb");
    if (file == NULL) {
        // OPTIMIZATION: Direct string assignment instead of strcpy
        game.saveMessage = "ERROR: Save Failed!";
        game.saveMessageTimer = 2.0f;
        return;
    }

    fwrite(&game, sizeof(GameState), 1, file);
    fclose(file);

    // OPTIMIZATION: Direct string assignment instead of strcpy
    game.saveMessage = "Game Saved!";
    game.saveMessageTimer = 2.0f;
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

       // Reset dynamic pointers to prevent crashes
    // (Pointers saved to disk are invalid when loaded back)
    game.moveHistory = NULL;
    game.moveCount = 0;
    game.moveCapacity = 0;
    game.gameHistory = NULL;
    game.historyLineCount = 0;
    game.historyCapacity = 0;
    
    // 5. Restore the user's active theme (as requested)
    game.currentTheme = currentThemeBeforeLoad;
    
    // 6. Re-apply the colors for the active theme
    // This is necessary because the global Color... variables
    // are not part of the GameState struct and were not loaded.
    ChangeTheme(game.currentTheme);

    // 7. Restore sound effects
    game.sfx.click     = LoadSound("resources/click.ogg");
    game.sfx.win       = LoadSound("resources/win.ogg");
    game.sfx.lose      = LoadSound("resources/lose.ogg");
    game.sfx.draw      = LoadSound("resources/draw.ogg");
    
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
 * @brief OPTIMIZATION: Loads game history from file using dynamic allocation.
 * Reads "game_history.txt" and allocates memory for each line.
 * Grows the array dynamically as needed using realloc.
 */
void LoadGameHistory(void) {
    game.historyLineCount = 0;
    
    FILE* file = fopen("game_history.txt", "r");
    if (file == NULL) {
        return;
    }
    
    char buffer[128];
    while (fgets(buffer, 128, file) != NULL) {
        // OPTIMIZATION: Expand capacity if needed using realloc
        if (game.historyLineCount >= game.historyCapacity) {
            int newCapacity = (game.historyCapacity == 0) ? 10 : game.historyCapacity * 2;
            char **newHistory = (char **)realloc(game.gameHistory, newCapacity * sizeof(char *));
            
            if (newHistory == NULL) break;  // Allocation failed
            
            game.gameHistory = newHistory;
            game.historyCapacity = newCapacity;
        }
        
        // Remove newline character
        buffer[strcspn(buffer, "\n")] = 0;
        
        // OPTIMIZATION: Allocate exact size needed for this string
        game.gameHistory[game.historyLineCount] = (char *)malloc(strlen(buffer) + 1);
        if (game.gameHistory[game.historyLineCount] != NULL) {
            strcpy(game.gameHistory[game.historyLineCount], buffer);
            game.historyLineCount++;
        }
    }
    
    fclose(file);
}

/**
 * @brief OPTIMIZATION: Clears all game history from file and memory.
 * Frees all dynamically allocated strings and resets counters.
 */
void ClearGameHistory(void) {
    FILE* file = fopen("game_history.txt", "w");
    if (file != NULL) {
        fclose(file);
    }
    
    // OPTIMIZATION: Free all allocated strings
    if (game.gameHistory != NULL) {
        for (int i = 0; i < game.historyLineCount; i++) {
            free(game.gameHistory[i]);
        }
        free(game.gameHistory);
        game.gameHistory = NULL;
    }
    game.historyLineCount = 0;
    game.historyCapacity = 0;
    game.historyScrollOffset = 0;
}


void DeleteSaveGame(void)
{
    // Try to delete "save.dat"
    if (remove("save.dat") == 0) 
    {
        game.saveMessage = "Save Deleted!";
        game.saveMessageTimer = 2.0f;
    } 
    else 
    {
        game.saveMessage = "No Save Found!";
        game.saveMessageTimer = 2.0f;
    }
}