/**
 * @file game_state.c
 * @brief Implementation of game state management functions
 * 
 * This file implements all game state operations including initialization,
 * board management, win checking, and theme management.
 */

#include "game_state.h"
#include "../Team_B/minimax.h"

// ============================================================================
// THEME DEFINITIONS
// ============================================================================

/**
 * @brief Array of all available color themes
 * 
 * Each theme provides a complete color palette for the UI.
 * Themes are indexed by ThemeID enum values.
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
        .background = {30, 40, 50, 255},       // Very Dark Blue
        .dark = {236, 240, 241, 255},     // Light Gray (Grid/Text)
        .light = {255, 255, 255, 255}         // White
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
        .background = {20, 25, 40, 255},       // Near Black
        .dark = {220, 220, 255, 255},     // Light Blue-White (Grid/Text)
        .light = {255, 255, 255, 255}         // White
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

/** Global UI color variables (initialized to black, updated by ChangeTheme) */
Color colorPrimary = {0};
Color colorSecondary = {0};
Color colorAccent = {0};
Color colorWarning = {0};
Color colorBackground = {0};
Color colorDark = {0};
Color colorLight = {0};

/** Global game state instance (initialized to zero) */
GameState game = {0};

// ============================================================================
// GAME INITIALIZATION FUNCTIONS
// ============================================================================

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
    game.aiMoveTimer = 0.5f;
    
    // Initialize statistics
    game.player1Wins = 0;
    game.player2Wins = 0;
    game.draws = 0;
    
    // Initialize UI settings
    game.isFullscreen = false;
    ChangeTheme(THEME_DEFAULT);
    
    // Initialize board
    ResetBoard();
}

void ResetBoard(void)
{
    // Clear all board cells
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game.board[i][j] = ' ';
        }
    }
    
    // Reset game state
    game.currentPlayer = game.humanSymbol;
    game.gameOver = false;
    game.winner = ' ';
    
    // If human is O in one-player mode, AI goes first
    game.aiTurn = (game.mode == MODE_ONE_PLAYER && game.humanSymbol == 'O');
    game.aiMoveTimer = 0.5f;  // Delay before AI makes move
}

// ============================================================================
// GAME STATUS CHECKING FUNCTIONS
// ============================================================================

// Forward declaration for helper function
static void updateWinStatistics(void);

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
    
    return false;
}

/**
 * @brief Helper function to update win statistics
 * 
 * Updates player1Wins or player2Wins based on the winner and game mode.
 * This is separated to avoid code duplication in CheckWinner().
 */
static void updateWinStatistics(void)
{
    if (game.mode == MODE_ONE_PLAYER) {
        // In one-player mode, track human vs AI
        if (game.winner == game.humanSymbol) {
            game.player1Wins++;
        } else {
            game.player2Wins++;
        }
    } else {
        // In two-player mode, track X vs O
        if (game.winner == 'X') {
            game.player1Wins++;
        } else {
            game.player2Wins++;
        }
    }
}

bool IsBoardFull(void)
{
    // Check if any cell is empty
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game.board[i][j] == ' ') {
                return false;
            }
        }
    }
    
    // Board is full - no winner, it's a draw
    game.winner = ' ';
    game.draws++;
    return true;
}

// ============================================================================
// AI MOVE EXECUTION
// ============================================================================

void MakeAIMove(void)
{
    struct Move bestMove;
    
    // Select AI algorithm based on difficulty setting
    // DIFF_HARD = 1 (Perfect minimax), DIFF_MEDIUM = 2 (Imperfect minimax), 
    // DIFF_EASY = 3 (Model-based)
    if (game.difficulty == DIFF_HARD) {
        // Perfect AI - unbeatable, uses alpha-beta pruning
        bestMove = findBestMovePerfect(game.board, game.aiSymbol);
    } else if (game.difficulty == DIFF_MEDIUM) {
        // Imperfect AI - makes mistakes, depth-limited minimax
        bestMove = findBestMoveImperfect(game.board);
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
