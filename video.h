#pragma once
#include "rect.h"
#include <allegro5/allegro.h>
unsigned long long _FRAMES;

void DrawRoundedRect(Rect r, ALLEGRO_COLOR color);
void DrawOutlinedRect_Dithered(Rect* r, ALLEGRO_COLOR color);
void DrawCone(int x, int y, float angle, float radius, int length, ALLEGRO_COLOR color);
void GetConeVertices(int cx, int cy, int* x1, int* y1, int* x2, int* y2, float angle, float radius, int length);
bool isInsideSector(int x, int y, int cx, int cy, float startX, float startY, float endX, float endY, float length, float radius, float amtRotated);
