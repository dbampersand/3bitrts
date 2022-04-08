#pragma once
#include "sprite.h"
typedef struct UI
{
    int panel_sprite_index;
    int cursorDefaultIndex;
    int cursorCastingIndex;
    int cursorAttackIndex;

} UI;   

UI ui;

typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE;
typedef struct Ability Ability;

void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void DrawAbilityPortraits(GameObject* selected, Ability* heldAbility, int index, int startX, int startY, bool keydown);
void DrawCursor(ALLEGRO_MOUSE_STATE* mouseState, int index, bool clicked);
void LoadCursorSprite(UI* ui, int* index, char* path);
