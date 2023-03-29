#include "vectorshape.h"
#include "player.h"
#include "colors.h"
#include "rect.h"
#include "gameobject.h"

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"


void DrawVectorShape(VectorShape* v, Color color)
{
    int w = abs(v->extentMaxX) + abs(v->extentMinX);
    int h = abs(v->extentMaxY) + abs(v->extentMinY);


    float xScreen = v->x ; 
    float yScreen = v->y;

    ToScreenSpace(&xScreen,&yScreen);

    float cx = w/2;
    float cy = h/2;
    

    float dx = xScreen + v->offsetX;
    float dy = yScreen + v->offsetY;
    RotatePointF(&dx,&dy,v->x,v->y,v->angle);

    DEBUG_P4.x = cx + v->x;
    DEBUG_P4.y = cy + v->y;


    al_draw_tinted_rotated_bitmap(v->generatedSprite,GetColor(color,0),cx,cy,dx,dy,v->angle,0);

}   
bool ObjectInVectorShape(GameObject* g, VectorShape* v)
{
    if (!g || !v)  
        return false;
    #define COLLISION_SHAPE_NUM_POINTS 5.0f

    Rect r = GetObjRect(g);

    float moveX = r.w / COLLISION_SHAPE_NUM_POINTS;
    float moveY = r.h / COLLISION_SHAPE_NUM_POINTS;

    for (float x = 0; x < COLLISION_SHAPE_NUM_POINTS; x++)
    {
        for (float y = 0; y < COLLISION_SHAPE_NUM_POINTS; y++)
        {
            float xCheck = r.x + (x * moveX);
            float yCheck = r.y + (y * moveY);
            if (PointInShape(v,xCheck,yCheck))
                return true;
        }
    } 
    return false;
}
void MoveVectorShape(int x, int y, VectorShape v)
{

}
bool CastRay(float x, float y, Line l)
{
    const float _EPSILON = 0.001f;

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

    float xScreen = v->x + v->points[0].x; 
    float yScreen = v->y +  v->points[0].y;

    float cx = v->x;
    float cy = v->y;

    //RotatePointF(&cx,&cy,v->x,v->y,v->angle);

    for (int i = 0; i < v->numPoints; i++)
    {
        Line l;
        l.x1 = v->points[i].x + v->x;
        l.y1 = v->points[i].y + v->y;

        RotatePointF(&l.x1,&l.y1,cx,cy,v->angle);
        /*
        if (i == 0)
        {
            DEBUG_P1.x = l.x1;
            DEBUG_P1.y = l.y1;

        }
        if (i == 1)
        {
            DEBUG_P2.x = l.x1;
            DEBUG_P2.y = l.y1;
        }
        if (i == 2)
        {
            DEBUG_P3.x = l.x1;
            DEBUG_P3.y = l.y1;
        }*/


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
        RotatePointF(&l.x2,&l.y2,cx,cy,v->angle);

        if (CastRay(x,y,l))
            numIntersections++;

    }
    return (numIntersections % 2 != 0);
}

ALLEGRO_BITMAP* GenerateVectorShapeBitmap(VectorShape* v)
{
    ALLEGRO_BITMAP* before = al_get_target_bitmap();
    int w = abs(v->extentMaxX) + abs(v->extentMinX);
    int h = abs(v->extentMaxY) + abs(v->extentMinY);


    ALLEGRO_BITMAP* b = al_create_bitmap(w,h);

    al_set_target_bitmap(b);
    al_lock_bitmap(b,ALLEGRO_PIXEL_FORMAT_ANY,0);

    int yOffset = v->extentMinY;
    
    for (int x = v->extentMinX; x < w; x++)
    {
        for (int y = v->extentMinY; y < h; y++)
        {
            float x2 = x + v->x;
            float y2 = y + v->y;

            if (PointInShape(v,x2,y2))
            {
                al_put_blended_pixel(x - v->extentMinX,y+abs(yOffset),al_map_rgb(255,255,255));
            }
        }
    }
    v->angle = 0;

    al_unlock_bitmap(b);
    al_set_target_bitmap(before);

    v->generatedSprite = b;

}
VectorShape CreateVectorShape(Point* points, int numPoints, int x, int y)
{
    VectorShape v = {0};
    v.extentMinX = FLT_MAX;
    v.extentMinY = FLT_MAX;
    v.extentMaxX = -FLT_MAX;
    v.extentMaxY = -FLT_MAX;

    v.offsetX = 0;
    v.offsetY = 0;


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
    v.angle = 0;

    GenerateVectorShapeBitmap(&v);
    return v;
}