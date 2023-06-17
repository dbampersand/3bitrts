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
#include <math.h>

typedef struct PointSortStruct
{
    float x;
    float y;
    float a;
}PointSortStruct;

int SortPoints(const void* a, const void* b)
{
    PointSortStruct p1 = *(PointSortStruct*)a;
    PointSortStruct p2 = *(PointSortStruct*)b;
    return p2.a - p1.a;
}

Point GetBarycentre(Point* points, int numPoints)
{
    float x=0; float y=0;
    for (int i = 0; i < numPoints; i++)
    {
        x += points[i].x;
        y += points[i].y;
    }
    return (Point){x/numPoints,y/numPoints};
}   

void SortAntiClockwise(Point** points, int numPoints)
{
    Point center = GetBarycentre(*points,numPoints);

    PointSortStruct p[numPoints];
    Point* pointD = *points;
    for (int i = 0; i < numPoints; i++)
    {   
        p[i].x = pointD[i].x;
        p[i].y = pointD[i].y;
        p[i].a = atan2f(pointD[i].y - center.y, pointD[i].x - center.x);
    }
    qsort(p,numPoints,sizeof(PointSortStruct),SortPoints);

    for (int i = 0; i < numPoints; i++)
    {   
        pointD[i].x = p[i].x;
        pointD[i].y = p[i].y;
    }


}
void DrawVectorShape(VectorShape* v, Color color)
{
    int w = fabsf(v->extentMaxX) + fabsf(v->extentMinX);
    int h = fabsf(v->extentMaxY) + fabsf(v->extentMinY);
    

    float xScreen = v->x ; 
    float yScreen = v->y;

    if (fabsf(v->extentMinY) > fabsf(v->extentMaxY))
        yScreen -= v->extentMaxY;
    if (fabsf(v->extentMinY) < fabsf(v->extentMaxY))
        yScreen -= v->extentMinY;
    if (fabsf(v->extentMinX) > fabsf(v->extentMaxX))
        xScreen -= v->extentMaxX;
    if (fabsf(v->extentMinX) < fabsf(v->extentMaxX))
        xScreen -= v->extentMinX;




    float cx = w/2;
    float cy = h/2;
    
    if (fabsf(v->extentMinY) > fabsf(v->extentMaxY))
    {
        cy = h;
    }
    if (fabsf(v->extentMinY) < fabsf(v->extentMaxY))
    {
        cy = 0;
    }
    if (fabsf(v->extentMinX) > fabsf(v->extentMaxX))
    {
        cx = w;
    }
    if (fabsf(v->extentMinX) < fabsf(v->extentMaxX))
    {
        cx = 0;
    }




    float dx = xScreen + v->offsetX;
    float dy = yScreen + v->offsetY;
    
    float rotX = v->x; float rotY = v->y;

    RotatePointF(&dx,&dy,rotX,rotY,v->angle);

    //DEBUG_P4.x = cx + v->x;
    //DEBUG_P4.y = cy + v->y;
    ToScreenSpace(&dx,&dy);


    al_draw_tinted_rotated_bitmap(v->generatedSprite,GetColor(color,0),cx,cy,dx,dy,v->angle,0);

}   
bool ObjectInVectorShape(GameObject* g, VectorShape* v)
{
    if (!g || !v)  
        return false;
    #define COLLISION_SHAPE_NUM_POINTS 5.0f

    Rect r = GetObjRect(g);

    int w = fabsf(v->extentMaxX) + fabsf(v->extentMinX);
    int h = fabsf(v->extentMaxY) + fabsf(v->extentMinY);


    float moveX = r.w / COLLISION_SHAPE_NUM_POINTS;
    float moveY = r.h / COLLISION_SHAPE_NUM_POINTS;

    for (float x = 0; x < COLLISION_SHAPE_NUM_POINTS; x++)
    {
        for (float y = 0; y < COLLISION_SHAPE_NUM_POINTS; y++)
        {
            float xCheck = r.x + (x * moveX);
            float yCheck = r.y + (y * moveY);
            if (PointInShape(v,xCheck,yCheck,v->angle,w,h))
                return true;
        }
    } 
    return false;
}
void MoveVectorShape(int x, int y, VectorShape* v)
{
}
bool CastRay(float x, float y, Line l)
{
    const float _EPSILON = 0.001f;

    float minX = _MIN(l.x1,l.x2);
    float maxX = _MAX(l.x1,l.x2);

    
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
bool PointInShape(VectorShape* v, int x, int y, float angle, float w, float h)
{
    //if (!PointInRect(x,y,(Rect){v->x + v->extentMinX, v->y + v->extentMinY,w,h}))
      //  return false;
    for (int i = 0; i < v->numCutOutAreas; i++)
    {
        int w2 = fabsf(v->cutoutAreas[i].extentMinX) + fabsf(v->cutoutAreas[i].extentMaxX);
        int h2 = fabsf(v->cutoutAreas[i].extentMinY) + fabsf(v->cutoutAreas[i].extentMaxY);

        if (PointInShape(&v->cutoutAreas[i],x,y,angle,w,h))
        {
            return false;
        }
    }
    int numIntersections = 0;

    float cx = v->x;
    float cy = v->y;

    //RotatePointF(&cx,&cy,v->x,v->y,v->angle);

    for (int i = 0; i < v->numPoints; i++)
    {
        Line l;
        l.x1 = v->points[i].x + v->x;
        l.y1 = v->points[i].y + v->y;

        RotatePointF(&l.x1,&l.y1,cx,cy,angle);
        
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

        RotatePointF(&l.x2,&l.y2,cx,cy,angle);

        if (i == 0)
        {
            DEBUG_P1.x = l.x1;
            DEBUG_P1.y = l.y1;

        }
        if (i == 1)
        {
            DEBUG_P2.x = l.x2;
            DEBUG_P2.y = l.y2;
        }
        if (i == 2)
        {
            DEBUG_P3.x = cx;
            DEBUG_P3.y = cy;
        }


        if (CastRay(x,y,l))
            numIntersections++;

    }
    return (numIntersections % 2 != 0);
}
void NOTArea(VectorShape* v, Point* points, int numPoints)
{
    if (!v->cutoutAreas)
    {
        v->cutoutAreas = calloc(1,sizeof(VectorShape));
        v->numCutOutAreas = 1;
    }
    else
    {
        v->numCutOutAreas++;
        v->cutoutAreas = realloc(v->cutoutAreas,v->numCutOutAreas * sizeof(VectorShape));
    }
    v->cutoutAreas[v->numCutOutAreas - 1] = CreateVectorShape(points,numPoints,v->x,v->y);
    GenerateVectorShapeBitmap(&v->cutoutAreas[v->numCutOutAreas-1],v,true);
}

ALLEGRO_BITMAP* GenerateVectorShapeBitmap(VectorShape* v, VectorShape* parent, bool not)
{
    ALLEGRO_BITMAP* before = al_get_target_bitmap();
    int w = fabsf(v->extentMaxX) + fabsf(v->extentMinX);
    int h = fabsf(v->extentMaxY) + fabsf(v->extentMinY);

    int yOffset = parent->extentMinY;


    ALLEGRO_BITMAP* b;
    if (parent && parent->generatedSprite)
    {
        b = parent->generatedSprite;
    }
    else
        b = al_create_bitmap(w,h);
    

    al_set_target_bitmap(b);
   // al_lock_bitmap(b,ALLEGRO_PIXEL_FORMAT_ANY,0);

    ALLEGRO_COLOR col;
    if (not)
        col = al_map_rgba(0,0,0,0);
    else
        col = al_map_rgb(255,255,255);
    /*
    for (int x = parent->extentMinX; x < w; x++)
    {
        for (int y = parent->extentMinY; y < h; y++)
        {
            float x2 = x + parent->x;   
            float y2 = y + parent->y;

            if (PointInShape(v,x2,y2,0,w,h))
            {
                al_put_pixel(x - parent->extentMinX,y + abs(yOffset),col);
            }
        }
    }*/

    int op; int src; int dst;
    al_get_blender(&op, &src, &dst);


    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
    Point vertexes[v->numPoints];
    for (int i = 0; i < v->numPoints; i++)
    {
        vertexes[i].x = v->points[i].x-parent->extentMinX;
        vertexes[i].y = v->points[i].y+abs(yOffset);
    }
    al_draw_filled_polygon((void*)vertexes,v->numPoints,col);
    //v->angle = 0;

    //al_unlock_bitmap(b);
    al_set_target_bitmap(before);

    al_set_blender(op, src, dst);


    v->generatedSprite = b;
    return b;

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
    SortAntiClockwise(&v.points,numPoints);
    v.x = x;
    v.y = y;
    v.numPoints = numPoints;
    v.angle = 0;

    GenerateVectorShapeBitmap(&v,&v,false);
    return v;
}