#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <stdbool.h>

// UI helper functions
Rectangle CreateButton(float x, float y, float width, float height);
bool IsButtonHovered(Rectangle button);
void DrawButton(Rectangle button, const char* text, Color color);

#endif // UI_H