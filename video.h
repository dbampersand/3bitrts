#pragma once
#include "rect.h"
#include <allegro5/allegro.h>
unsigned long long _FRAMES;
    
void DrawRoundedRect(Rect r, ALLEGRO_COLOR color);
void DrawOutlinedRect_Dithered(Rect* r, ALLEGRO_COLOR color);
