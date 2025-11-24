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
bool IsButtonHovered(const Rectangle *button)
{
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, *button);
}

/**
 * @brief Draws a complete button with text, border, and hover effect.
 * * Why: This function centralizes the button's "look and feel".
 * If we want to change how *all* buttons look (e.g., make the
 * border thicker, or change the hover effect), we only need
 * to edit this one function.
 */
void DrawButton(const Rectangle *button, const char* text, Color color)
{
    Color drawColor = color;
    if (IsButtonHovered(button)) {
        drawColor.r = (color.r + 50 > 255) ? 255 : color.r + 50;
        drawColor.g = (color.g + 50 > 255) ? 255 : color.g + 50;
        drawColor.b = (color.b + 50 > 255) ? 255 : color.b + 50;
    }
    
    // Use -> instead of . for pointer access
    DrawRectangleRec(*button, drawColor);
    DrawRectangleLinesEx(*button, 3, colorDark);
    
    int textWidth = MeasureText(text, 30);
    DrawText(text, 
             button->x + (button->width - textWidth)/2,
             button->y + (button->height - 30)/2,
             30, colorLight);
}