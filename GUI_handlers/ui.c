/**
 * @file ui.c
 * @brief Implementation of UI helper functions.
 */

#include "ui.h"
#include "game_state.h" // Needed for colorDark (for button borders)

// ============================================================================
// BUTTON CREATION AND INTERACTION
// ============================================================================

/**
 * @brief Creates a Rectangle centered at (x, y).
 */
Rectangle CreateButton(float x, float y, float width, float height) {
  // Create rectangle centered at (x, y)
  return (Rectangle){x - width / 2, y - height / 2, width, height};
}

/**
 * @brief Checks if the mouse is over a button.
 */
bool IsButtonHovered(const Rectangle *button) {
  Vector2 mousePos = GetMousePosition();
  return CheckCollisionPointRec(mousePos, *button);
}

/**
 * @brief Draws a button with text and hover effect.
 */
void DrawButton(const Rectangle *button, const char *text, Color color) {
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
  DrawText(text, button->x + (button->width - textWidth) / 2,
           button->y + (button->height - 30) / 2, 30, colorLight);
}