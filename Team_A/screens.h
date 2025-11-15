#ifndef SCREENS_H
#define SCREENS_H

// Screen drawing functions
void DrawStartScreen(void);
void DrawModeSelectScreen(void);
void DrawDifficultySelectScreen(void);
void DrawSymbolSelectScreen(bool isPlayer1);
void DrawInstructionsScreen(void);
void DrawGameScreen(void);
void DrawGameOverScreen(void);

// Screen input handling functions
void HandleStartScreen(void);
void HandleModeSelectScreen(void);
void HandleDifficultySelectScreen(void);
void HandleSymbolSelectScreen(bool isPlayer1);
void HandleInstructionsScreen(void);
void HandleGameScreen(void);
void HandleGameOverScreen(void);

#endif // SCREENS_H
