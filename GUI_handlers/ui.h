/**
 * @file ui.h
 * @brief User interface helper functions for Tic-Tac-Toe
 * * This module provides reusable UI components and utilities for creating
 * interactive buttons and UI elements. Functions handle button creation,
 * hover detection, and rendering with consistent styling.
 * * This is a "toolkit" file to make `screens.c` cleaner.
 */

#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <stdbool.h>

// ============================================================================
// UI HELPER FUNCTIONS
// ============================================================================

/**
 * @brief Create a button rectangle centered at the given coordinates
 * * This is a convenience function. Instead of manually calculating
 * `(x - width/2)`, we can just pass the desired *center* coordinates,
 * which is a more intuitive way to design UIs.
 * * @param x Center X coordinate of the button
 * @param y Center Y coordinate of thebutton
 * @param width Width of the button
 * @param height Height of the button
 * @return Rectangle structure representing the button bounds
 */
Rectangle CreateButton(float x, float y, float width, float height);

/**
 * @brief Check if the mouse cursor is hovering over a button
 * * A simple wrapper for Raylib's `CheckCollisionPointRec`.
 * This makes the code in `screens.c` more readable
 * (e.g., `if (IsButtonHovered(myButton))` vs.
 * `if (CheckCollisionPointRec(GetMousePosition(), myButton))`).
 * * @param button The button rectangle to check
 * @return true if mouse is over the button, false otherwise
 */
bool IsButtonHovered(const Rectangle *button);

/**
 * @brief Draw a button with text and hover effect
 * * This function encapsulates all the drawing logic for a standard
 * button. It handles:
 * 1. Drawing the button rectangle
 * 2. Brightening the color on hover
 * 3. Drawing a border
 * 4. Drawing the text *centered* inside the button
 * * This ensures all buttons in the game have a consistent look and feel.
 * * @param button The button rectangle to draw
 * @param text The text to display on the button
 * @param color The base color of the button
 */
void DrawButton(const Rectangle *button, const char* text, Color color);

#endif // UI_H