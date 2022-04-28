#include "video.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "colors.h"
#include "helperfuncs.h"
#include <float.h>
#include "point.h"
unsigned long long _FRAMES = 0;

void DrawRoundedRect(Rect r, ALLEGRO_COLOR color)
{
    //horizontals
    al_draw_line(r.x,r.y,r.x+r.w-1,r.y,color,1);
    al_draw_line(r.x,r.y+r.h,r.x+r.w-1,r.y+r.h,color,1);

    //verticals
    al_draw_line(r.x,r.y+1,r.x,r.y+r.h,color,1);
    al_draw_line(r.x+r.h,r.y+1,r.x+r.h,r.y+r.h,color,1);



}
void DrawOutlinedRect_Dithered(Rect* r, ALLEGRO_COLOR color)
{
    if (!r) return;
    
    for (int y = r->y+2; y < r->y+r->h; y++)
    {
        if (y%2==0)
        {
            al_draw_pixel(r->x,y,color);
            al_draw_pixel(r->x+r->w,y,color);
        }

    }
    for (int x = r->x; x < r->x+r->w+1; x++)
    {
        if (x%2==1)
        {
            al_draw_pixel(x,r->y,color);
            al_draw_pixel(x,r->y+r->h,color);
        }

    }

    //Draw corner details
    al_draw_pixel(r->x,r->y,color);
    al_draw_pixel(r->x+1,r->y,color);
    al_draw_pixel(r->x,r->y+1,color);

    al_draw_pixel(r->x+r->w,r->y,color);
    al_draw_pixel(r->x+r->w-1,r->y,color);
    al_draw_pixel(r->x+r->w,r->y+1,color);

    al_draw_pixel(r->x,r->y+r->h,color);
    al_draw_pixel(r->x,r->y+r->h-1,color);
    al_draw_pixel(r->x+1,r->y+r->h,color);

    al_draw_pixel(r->x+r->w,r->y+r->h,color);
    al_draw_pixel(r->x+r->w-1,r->y+r->h,color);
    al_draw_pixel(r->x+r->w,r->y+r->h-1,color);



    //}
}
int GetBarycentricCoord(int x0, int y0, int x1, int y1, int pX, int pY)
{
    return (y0 - y1) * pX + (x1 - x0) * pY + x0 * y1 - x1 * y0;
}
void SwapPoint(int* x0, int* y0, int* x1, int* y1)
{
    int temp = *x1;
    *x1 = *x0;
    *x0 = temp;
    temp = *y1;
    *y1 = *y0;
    *y0 = temp;
}

bool PointInTri(int x0, int y0, int x1, int y1, int x2, int y2, int pX, int pY)
{

    if (y0 > y1)
    {
        SwapPoint(&x0,&y0,&x1,&y1);
    }
    if (y0 > y2)
    {
        SwapPoint(&x0,&y0,&x2,&y2);
    }

    if (y1 > y2)
    {
        SwapPoint(&x1,&y1,&x2,&y2);
    }
    #ifdef DEBUG
        al_draw_line(x1,y1,x2,y2,HEAL,1);
        al_draw_line(x2,y2,x0,y0,HEAL,1);
        al_draw_line(x0,y0,x1,y1,HEAL,1);
    #endif
    float bary12 = GetBarycentricCoord(x1,y1,x2,y2,x0,y0);
    float bary20 = GetBarycentricCoord(x2,y2,x0,y0,x1,y1);
    float bary01 = GetBarycentricCoord(x0,y0,x1,y1,x2,y2);

    float alpha = GetBarycentricCoord(x1,y1,x2,y2,pX,pY) / bary12;
    float beta = GetBarycentricCoord(x2,y2,x0,y0,pX,pY) / bary20;
    float gamma = GetBarycentricCoord(x0,y0,x1,y1,pX,pY) / bary01;


    return (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1);
}

bool isClockwise(int x1, int y1, int x2, int y2)
{
    return -x1*y2 + y1*x2 > 0;
}
Rect GetTriBoundingBox(int x0, int y0, int x1, int y1, int x2, int y2)
{
    Rect r;
    r.x = _MIN(_MIN(x0,x1),x2);
    r.y = _MIN(_MIN(y0,y1),y2);

    r.w = _MAX(_MAX(x0,x1),x2) - r.x;
    r.y = _MAX(_MAX(y0,y1),y2) - r.y;
    return r;
}
bool isInsideSector(int x, int y, int cx, int cy, float startX, float startY, float endX, float endY, float length, float radius, float amtRotated) 
{
    float dX = cx - x;
    float dY = cy - y;

    double angle = (atan2(y-cy,x-cx));
    double angle2 = (atan2(startY-cy,startX-cx));
    double angle3 = (atan2(endY-cy,endX-cx));

    angle = Normalise(angle, 0, 2*M_PI);
    angle2 = Normalise(angle2,   0, 2*M_PI);
    angle3 = Normalise(angle3, 0, 2*M_PI);
    double rad = DegToRad(radius);
    rad = Normalise(rad, 0, 2*M_PI);

    if (angle2 < angle3)
    {
        float temp = angle2;
        angle2 = angle3;
        angle3 = temp;
    }


    if (dX*dX+dY*dY <= length*length)
    {
        float newX = startX-cx; float newY = startY-cy;
        float newX2 = endX-cx; float newY2 = endY-cy;
        Normalize(&newX,&newY);
        Normalize(&newX2,&newY2);
        newX*=10000;
        newY*=10000;

        newX2*=10000;
        newY2*=10000;
        newX += startX;
        newY += startY;
        newX2 += endX;
        newY2 += endY;


        float midX = startX;
        float midY = startY;

        float midAngle = M_PI - ((atan2(endY-startY,endX-startX)) + DegToRad(90) - radius/2.0) + amtRotated;
        
        midAngle = Normalise(midAngle, 0, 2*M_PI);

        RotatePointF(&midX,&midY,cx,cy,(midAngle));
        float midXNrm = midX - cx;
        float midYNrm = midY - cy;
        Normalize(&midXNrm,&midYNrm);
        midXNrm*=10000;
        midYNrm*=10000;
        midX += midXNrm;
        midY += midYNrm;
        
        bool tri1 = PointInTri(cx,cy,newX,newY,midX,midY,x,y);
        bool tri2 =  PointInTri(cx,cy,newX2,newY2,midX,midY,x,y);
        #ifdef DEBUG
            al_draw_line(cx,cy,midX,midY,POISON,1);
        #endif
        if (tri1 || tri2)
        {
            return true;
        }

    }
    return false;
}

void CircleSegment(int xc, int yc, float radius, float start, float end, ALLEGRO_COLOR col, float length)
{
    
    start = Normalise(start,0, M_PI*2);
    end = Normalise(end, 0, M_PI*2);

    int sign = start < end ? 1 : -1;

    float inc = 1 /  radius;
    if (sign == -1)
    {
        float temp = start;
        end -= radius * M_PI/180.0f - (length*M_PI/180.0f);
        float rad = (360-radius) * M_PI/180.0f;
        start -= rad + (length*M_PI/180.0f);
    }

    float theta = start;

    if (theta <= end)
    {
        while (theta <= end) {
            float xn; float yn;
            xn = xc + radius * (sin(theta));
            yn = yc + radius * (cos(theta));
            al_draw_pixel(xn, yn, col);
            theta += inc;
        }
    }
    else if (theta >= end)
    {
        while (theta >= end) {
            float xn; float yn;
            xn = xc + radius * (sin(theta));
            yn = yc + radius * (cos(theta));
            al_draw_pixel(xn, yn, col);
            theta += inc * -1;
        }

    }

}

//TODO: this isn't an accurate function.
//right now it will return true just when the center or corners are intersected
//does this need fixed? Unlikely for a cone to be *that* small radius
//either way, there should be a more efficient way to do this
bool RectInCone(Rect r, int cx, int cy, float angle, float radius, float length)
{
    float dx = _MIN(cx-r.x,cx-r.x+r.w);
    float dy = _MIN(cy-r.y,cy-r.y+r.h);
    if (CircleRectDist(cx,cy,length,r))
    {      
        Point rc = (Point){r.x+r.w/2.0f,r.y+r.h/2.0f};
        Point tl = (Point){r.x,r.y};
        Point tr = (Point){r.x+r.w,r.y};
        Point bl = (Point){r.x,r.y+r.h};
        Point br = (Point){r.x+r.h,r.y+r.h};
        
        int startX; int startY; int endX; int endY;
        GetConeVertices(cx,cy,&startX,&startY,&endX,&endY,angle,radius,length);
        if (isInsideSector(rc.x, rc.y,cx,cy,startX,startY,endX,endY,length,radius,angle) ||
            isInsideSector(tl.x, tl.y,cx,cy,startX,startY,endX,endY,length,radius,angle) ||
            isInsideSector(tr.x, tr.y,cx,cy,startX,startY,endX,endY,length,radius,angle) ||
            isInsideSector(bl.x, bl.y,cx,cy,startX,startY,endX,endY,length,radius,angle) ||
            isInsideSector(br.x, br.y,cx,cy,startX,startY,endX,endY,length,radius,angle))
        {
            return true;
        }
    }
    return false;
}
void GetConeVertices(int cx, int cy, int* x1, int* y1, int* x2, int* y2, float angle, float radius, int length)
{
    angle -= 45;
    *x1 = cx + length; *y1 = cy + length;
    *x2 = cx + length; *y2 = cy + length;

    radius = radius * M_PI/180.0f;
    angle = angle * M_PI/180.0f;
    float l_r = length * M_PI/180.0f;

    RotatePoint(x1,y1,cx,cy, -radius/2.0f+angle);
    RotatePoint(x2,y2,cx,cy, radius/2.0f+angle);

}
void DrawCone(int x, int y, float angle, float radius, int length, ALLEGRO_COLOR color)
{

    angle -= 45;
    int x2 = x + length; int y2 = y + length;
    int x3 = x + length; int y3 = y + length;

    radius = radius * M_PI/180.0f;
    angle = angle * M_PI/180.0f;
    float l_r = length * M_PI/180.0f;

    RotatePoint(&x2,&y2,x,y, -radius/2.0f+angle);
    RotatePoint(&x3,&y3,x,y, radius/2.0f+angle);


    ALLEGRO_KEYBOARD_STATE a;
    al_get_keyboard_state(&a);

    al_draw_line(x,y,x2,y2,color,1);
    al_draw_line(x,y,x3,y3,color,1);

    float angle2 = atan2(y-y2,x-x2);
    float angle3 = atan2(y-y3,x-x3);
    angle2 -= (angle+(90*M_PI/180.0f)) * 2;
    angle3 -= (angle+(90*M_PI/180.0f)) * 2;

    if (angle2 > angle3)
    {
        //float temp = angle2;
        //angle2 = angle3;
        //angle3 = temp;
        //angle3 = -angle3;
    }
    //angle2 -= angle;
    //angle3 -= angle;

    ALLEGRO_MOUSE_STATE m = GetMouseClamped();

    int startX = x2; int startY = y2;
    int endX = x3; int endY = y3;

    if (startY > endY)
    {
        int tempX = startX;
        int tempY = startY;

        // startX = endX;
            //startY = endY;

            //endX = tempX;
            //endY = tempY;

    }


    float distX = x - x2;
    float distY = y - y2;

    float l = sqrt(distX*distX+distY*distY);
    if (isInsideSector(m.x,m.y,x,y,x2,y2,x3,y3,l,radius,angle+DegToRad(45)))
        CircleSegment(x,y,l,angle2,angle3,color,length); 
    else
        CircleSegment(x,y,l,angle2,angle3,color,length);
    printf("\n");

}