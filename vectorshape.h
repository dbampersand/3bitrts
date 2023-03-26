#pragma once

#include "point.h"
#include "helperfuncs.h"
#include "allegro5/allegro.h"
#include "colors.h"

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
typedef struct GameObject GameObject;

typedef struct VectorShape
{
    float x;
    float y;

    Point* points;
    int numPoints;

    float extentMinX;
    float extentMinY;
    float extentMaxX;
    float extentMaxY;


    ALLEGRO_BITMAP* generatedSprite;
} VectorShape;

typedef struct Line
{
    int x1; int y1;
    int x2; int y2;
}Line;



void MoveVectorShape(int x, int y, VectorShape v);
VectorShape CreateVectorShape(Point* points, int numPoints, int x, int y);
void DrawVectorShape(VectorShape* v, Color color);
bool ObjectInVectorShape(GameObject* g, VectorShape* v);
bool PointInShape(VectorShape* v, int x, int y);
