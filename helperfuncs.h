#pragma once

#include <allegro5/allegro.h>
#include "rect.h"
#define _MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define _MAX(X,Y) ((X) > (Y) ? (X) : (Y))

int GetIndex(int h, int x, int y);
ALLEGRO_MOUSE_STATE GetMouseClamped();
char* readFile(const char* filename);
bool MoveTo(float* x, float* y, float toX, float toY, float speed, float delta);
void MoveAngle(float* x, float* y, float dx, float dy, float speed, float delta);   
bool CircleInRect(float cx, float cy, float rad, Rect r);
void Normalize(float* x, float* y);
double RandRange(double min, double max);
float dot(float x1, float y1, float x2, float y2);
float dist(float x1, float y1, float x2, float y2);
bool GetLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float* out_x, float* out_y);
