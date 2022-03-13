#pragma once
#include "sprite.h"
typedef struct UI
{
    int panel_sprite_index;
    int cursorDefaultIndex;
} UI;   

UI ui;

typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE;

void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void DrawAbilityPortraits(GameObject* selected, int index, int startX, int startY, bool keydown);
void DrawCursor(ALLEGRO_MOUSE_STATE* mouseState, int index, bool clicked);
void LoadCursorSprite(UI* ui, int* index, char* path);
