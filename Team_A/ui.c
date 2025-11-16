/**
 * @file ui.c
 * @brief Implementation of UI helper functions
 * 
 * This file implements reusable UI components for creating interactive
 * buttons with hover effects and consistent styling.
 */

#include "ui.h"
#include "game_state.h"

// ============================================================================
// BUTTON CREATION AND INTERACTION
// ============================================================================

Rectangle CreateButton(float x, float y, float width, float height)
{
    // Create rectangle centered at (x, y)
    return (Rectangle){x - width/2, y - height/2, width, height};
}

bool IsButtonHovered(Rectangle button)
{
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, button);
}

void DrawButton(Rectangle button, const char* text, Color color)
{
    // Determine button color (brighten on hover)
    Color drawColor = color;
    if (IsButtonHovered(button)) {
        // Brighten each color component by 50 (clamped to 255)
        drawColor.r = (color.r + 50 > 255) ? 255 : color.r + 50;
        drawColor.g = (color.g + 50 > 255) ? 255 : color.g + 50;
        drawColor.b = (color.b + 50 > 255) ? 255 : color.b + 50;
    }
    
    // Draw button background
    DrawRectangleRec(button, drawColor);
    
    // Draw button border
    DrawRectangleLinesEx(button, 3, colorDark);
    
    // Draw centered text
    int textWidth = MeasureText(text, 30);
    DrawText(text, 
             button.x + (button.width - textWidth)/2, 
             button.y + (button.height - 30)/2, 
             30, colorLight);
}