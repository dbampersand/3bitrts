#include "vectorshape.h"
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "allegro5/allegro.h"

ALLEGRO_BITMAP* btest;

void DrawVectorShape(VectorShape* v)
{
    
}

void MoveVectorShape(int x, int y, VectorShape v)
{

}
bool CastRay(float x, float y, Line l)
{
    const float _EPSILON = 0.00001f;

    float minX = _MIN(l.x1,l.x2);
    float minY = _MIN(l.y1,l.y2);
    float maxX = _MAX(l.x1,l.x2);
    float maxY = _MAX(l.y1,l.y2);

    
    if (l.y1 > l.y2)
    {
        float tempx = l.x1;
        float tempy = l.y1;

        l.x1 = l.x2;
        l.y1 = l.y2;

        l.x2 = tempx;
        l.y2 = tempy;

    }


    if (y == l.y1 || y == l.y2) 
        y += _EPSILON;
    if (y < l.y1 || y > l.y2) 
        return false;
    if (x >= maxX)
        return false;
    if (x < minX)  
        return true;
    float slopeOne; 
    float slopeTwo;
    if (l.x1 != l.x2)
        slopeOne = (l.y2 - l.y1) / (l.x2 - l.x1);
    else
        slopeOne = FLT_MAX;

    if (l.x1 != x)
        slopeTwo = (y - l.y1) / (x - l.x1);
    else
        slopeTwo = FLT_MAX;
    if (slopeTwo >= slopeOne)
        return true;
    else
        return false;
}
bool PointInShape(VectorShape* v, int x, int y)
{
    int numIntersections = 0;
    for (int i = 0; i < v->numPoints; i++)
    {
        Line l;
        l.x1 = v->points[i].x + v->x;
        l.y1 = v->points[i].y + v->y;

        Point point2;
        if (i == v->numPoints-1)
        {
            point2 = v->points[0];
        }
        else
        {
            point2 = v->points[i+1];
        }
        l.x2 = point2.x + v->x;
        l.y2 = point2.y + v->y;

        if (CastRay(x,y,l))
            numIntersections++;

    }
    return (numIntersections % 2 != 0);
}

ALLEGRO_BITMAP* GenerateVectorShapeBitmap(VectorShape* v)
{
    ALLEGRO_BITMAP* before = al_get_target_bitmap();
    int w = v->extentMaxX - v->extentMinX;
    int h = v->extentMaxY - v->extentMinY;

    ALLEGRO_BITMAP* b = al_create_bitmap(w,h);
    al_lock_bitmap(b,ALLEGRO_PIXEL_FORMAT_ANY,0);
    al_set_target_bitmap(b);
    for (int x = v->extentMinX + v->x; x < v->extentMaxX + v->x; x++)
    {
        for (int y = v->extentMinY + v->y; y < v->extentMaxY + v->y; y++)
        {
            if (PointInShape(v,x,y))
            {
                al_put_blended_pixel(x,y,(ALLEGRO_COLOR){255,255,255,255});
            }
        }
    }
    btest = b;
    al_unlock_bitmap(b);
    al_set_target_bitmap(before);

}
VectorShape CreateVectorShape(Point* points, int numPoints, int x, int y)
{
    VectorShape v = {0};
    for (int i = 0; i < numPoints; i++)
    {
        v.extentMinX = _MIN(v.extentMinX,points[i].x);
        v.extentMinY = _MIN(v.extentMinY,points[i].y);
        v.extentMaxX = _MAX(v.extentMaxX,points[i].x);
        v.extentMaxY = _MAX(v.extentMaxY,points[i].y);
    }
    v.points = calloc(numPoints,sizeof(v.points[0]));
    memcpy(v.points,points,numPoints * sizeof(v.points[0]));
    v.x = x;
    v.y = y;
    v.numPoints = numPoints;

    GenerateVectorShapeBitmap(&v);
}