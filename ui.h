#pragma once
#include "sprite.h"
typedef struct ALLEGRO_FONT ALLEGRO_FONT;
typedef struct UI
{
    int panel_sprite_index;
    int cursorDefaultIndex;
    int cursorCastingIndex;
    int cursorAttackIndex;

    ALLEGRO_FONT* font;

} UI;   

UI ui;

typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE;
typedef struct Ability Ability;

void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState);
bool DrawAbilityPortraits(GameObject* selected, Ability* heldAbility, int index, int startX, int startY, bool keydown, ALLEGRO_MOUSE_STATE* mouseState);
void DrawCursor(ALLEGRO_MOUSE_STATE* mouseState, int index, bool clicked);
void LoadCursorSprite(UI* ui, int* index, char* path);
void DrawDescriptionBox(char* description, int padding, ALLEGRO_FONT* f, int x, int y, int wTextbox);
 