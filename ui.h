#pragma once
#include "sprite.h"
typedef struct UI
{
    int panel_sprite_index;
} UI;   

UI ui;

void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void DrawAbilityPortraits(GameObject* selected, int index, int startX, int startY, bool keydown);
