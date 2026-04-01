/**
 * @file raylib_stub.c
 * @brief Minimal stubs for raylib functions used by game_state.c
 *
 * These stubs allow the test suite to compile and run without
 * the actual raylib library or a graphics window.
 *
 * We include raylib.h and provide empty implementations for
 * any functions that game_state.c calls.
 */

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// AUDIO STUBS
// ============================================================================

Sound LoadSound(const char *fileName) {
    Sound s = {0};
    (void)fileName;
    return s;
}

void UnloadSound(Sound sound) {
    (void)sound;
}

void PlaySound(Sound sound) {
    (void)sound;
}

void InitAudioDevice(void) {}
void CloseAudioDevice(void) {}
