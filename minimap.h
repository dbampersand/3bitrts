#pragma once

#include "allegro5/allegro.h"
#include "video.h"

typedef struct Minimap
{
    int x;
} Minimap;

extern Minimap minimap;

void DrawMinimap(MouseState mouseState);
