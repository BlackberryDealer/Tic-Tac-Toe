#include "ui.h"
#include "game_state.h"

Rectangle CreateButton(float x, float y, float width, float height)
{
    return (Rectangle){x - width/2, y - height/2, width, height};
}

bool IsButtonHovered(Rectangle button)
{
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, button);
}

void DrawButton(Rectangle button, const char* text, Color color)
{
    Color drawColor = color;
    if (IsButtonHovered(button))
    {
        drawColor.r = (color.r + 50 > 255) ? 255 : color.r + 50;
        drawColor.g = (color.g + 50 > 255) ? 255 : color.g + 50;
        drawColor.b = (color.b + 50 > 255) ? 255 : color.b + 50;
    }
    
    DrawRectangleRec(button, drawColor);
    DrawRectangleLinesEx(button, 3, colorDark);
    
    int textWidth = MeasureText(text, 30);
    DrawText(text, button.x + (button.width - textWidth)/2, 
             button.y + (button.height - 30)/2, 30, colorLight);
}