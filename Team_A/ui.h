/**
 * @file ui.h
 * @brief User interface helper functions for Tic-Tac-Toe
 * 
 * This module provides reusable UI components and utilities for creating
 * interactive buttons and UI elements. Functions handle button creation,
 * hover detection, and rendering with consistent styling.
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
 * 
 * @param x Center X coordinate of the button
 * @param y Center Y coordinate of the button
 * @param width Width of the button
 * @param height Height of the button
 * @return Rectangle structure representing the button bounds
 */
Rectangle CreateButton(float x, float y, float width, float height);

/**
 * @brief Check if the mouse cursor is hovering over a button
 * 
 * @param button The button rectangle to check
 * @return true if mouse is over the button, false otherwise
 */
bool IsButtonHovered(Rectangle button);

/**
 * @brief Draw a button with text and hover effect
 * 
 * Renders a button with the specified text and color. The button
 * automatically brightens when hovered. Includes border and centered text.
 * 
 * @param button The button rectangle to draw
 * @param text The text to display on the button
 * @param color The base color of the button
 */
void DrawButton(Rectangle button, const char* text, Color color);

#endif // UI_H