#pragma once
#include "stdbool.h"

typedef struct Point
{
    float x; float y;
} Point;
typedef struct PointI
{
    int x; int y;
} PointI;

bool PointIsEq(Point p1, Point p2);
bool PointIntIsEq(PointI p1, PointI p2);
