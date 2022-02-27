#pragma once

#include <allegro5/allegro.h>
#define _MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define _MAX(X,Y) ((X) > (Y) ? (X) : (Y))

int GetIndex(int h, int x, int y);
ALLEGRO_MOUSE_STATE GetMouseClamped();
char* readFile(const char* filename);
