#pragma once

#include "point.h"
#include "helperfuncs.h"
#include "allegro5/allegro.h"
#include "colors.h"

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
typedef struct GameObject GameObject;
typedef struct Area
{
    Point* points;
    int numPoints;
} Area;
typedef struct VectorShape
{
    float x;
    float y;

    Point* points;
    int numPoints;

    struct VectorShape* cutoutAreas;
    int numCutOutAreas;


    float extentMinX;
    float extentMinY;
    float extentMaxX;
    float extentMaxY;


    ALLEGRO_BITMAP* generatedSprite;

    float angle;
    float offsetX; float offsetY;
    
} VectorShape;

typedef struct Line
{
    float x1; float y1;
    float x2; float y2;
}Line;



void MoveVectorShape(int x, int y, VectorShape v);
VectorShape CreateVectorShape(Point* points, int numPoints, int x, int y);
void DrawVectorShape(VectorShape* v, Color color);
bool ObjectInVectorShape(GameObject* g, VectorShape* v);
bool PointInShape(VectorShape* v, int x, int y, float angle);
void NOTArea(VectorShape* v, Point* points, int numPoints);
ALLEGRO_BITMAP* GenerateVectorShapeBitmap(VectorShape* v, VectorShape* parent, bool not);

