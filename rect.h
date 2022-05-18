#pragma once
#include <stdbool.h>
#include "vector2.h"
typedef struct Rect 
{
    int x; int y; int w; int h;
} Rect;

bool CheckIntersect(Rect r1, Rect r2);
void GetRectIntersectBound(float x, float y, float w, float h, float x2, float y2, float w2, float h2, float* resultX, float* resultY, float* resultW, float* resultH);
bool PointInRect(float x, float y, Rect r);
Rect UnionRectR(Rect r1, Rect r2);
bool RectsTouch(Rect r1, Rect r2, float delta);
