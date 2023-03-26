#pragma once

#include "point.h"
#include "helperfuncs.h"

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

typedef struct VectorShape
{
    float x;
    float y;

    Point* points;
    int numPoints;

    int extentMinX;
    int extentMinY;
    int extentMaxX;
    int extentMaxY;


    ALLEGRO_BITMAP* generatedSprite;
} VectorShape;

typedef struct Line
{
    int x1; int y1;
    int x2; int y2;
}Line;


extern ALLEGRO_BITMAP* btest;

void MoveVectorShape(int x, int y, VectorShape v);
VectorShape CreateVectorShape(Point* points, int numPoints, int x, int y);
