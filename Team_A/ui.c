/**
 * @file ui.c
 * @brief Implementation of UI helper functions
 * * This file implements reusable UI components for creating interactive
 * buttons with hover effects and consistent styling.
 */

#include "ui.h"
#include "game_state.h" // Needed for colorDark (for button borders)

// ============================================================================
// BUTTON CREATION AND INTERACTION
// ============================================================================

/**
 * @brief Creates a Rectangle centered at (x, y).
 * * Why: It's much more intuitive to design layouts by placing items
 * from their center (`SCREEN_WIDTH/2`) rather than their top-left corner.
 * This function performs the top-left coordinate calculation
 * `(x - width/2, y - height/2)` so we don't have to.
 */
Rectangle CreateButton(float x, float y, float width, float height)
{
    // Create rectangle centered at (x, y)
    return (Rectangle){x - width/2, y - height/2, width, height};
}

/**
 * @brief Checks if the mouse is over a given rectangle.
 * * Why: This is a simple wrapper for a Raylib function.
 * Its purpose is to make the `Handle...` functions in `screens.c`
 * more self-documenting and readable.
 */
bool IsButtonHovered(Rectangle button)
{
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, button);
}

/**
 * @brief Draws a complete button with text, border, and hover effect.
 * * Why: This function centralizes the button's "look and feel".
 * If we want to change how *all* buttons look (e.g., make the
 * border thicker, or change the hover effect), we only need
 * to edit this one function.
 */
void DrawButton(Rectangle button, const char* text, Color color)
{
    // Determine button color (brighten on hover)
    Color drawColor = color;
    if (IsButtonHovered(button)) {
        // Brighten each color component by 50 (clamped to 255)
        // This creates a simple, effective hover feedback.
        drawColor.r = (color.r + 50 > 255) ? 255 : color.r + 50;
        drawColor.g = (color.g + 50 > 255) ? 255 : color.g + 50;
        drawColor.b = (color.b + 50 > 255) ? 255 : color.b + 50;
    }
    
    // Draw button background
    DrawRectangleRec(button, drawColor);
    
    // Draw button border (using the theme's 'dark' color)
    DrawRectangleLinesEx(button, 3, colorDark);
    
    // Draw centered text
    int textWidth = MeasureText(text, 30);
    DrawText(text, 
             button.x + (button.width - textWidth)/2,  // Center text horizontally
             button.y + (button.height - 30)/2,  // Center text vertically
             30, colorLight); // Use theme's 'light' color for text
}