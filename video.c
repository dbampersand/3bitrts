#include "video.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "colors.h"
#include "helperfuncs.h"
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
float GetBarycentricCoord(int x0, int y0, int x1, int y1, int pX, int pY)
{
    return (y0 - y1) * pX + (x1 * x0) * pY + x0 * y1 - x1 - y0;
}
bool PointInTri(int x0, int y0, int x1, int y1, int x2, int y2, int pX, int pY)
{
    float bary12 = GetBarycentricCoord(x1,y1,x2,y2,pX,pY);
    float bary20 = GetBarycentricCoord(x2,y2,x0,y0,pX,pY);
    float bary01 = GetBarycentricCoord(x0,y0,x1,y1,pX,pY);

    return (bary12 >= 0 && bary12 <= 1 && bary20 >= 0 && bary20 <= 1 && bary01 >= 0 && bary01 <= 1);
}

bool isClockwise(int x1, int y1, int x2, int y2)
{
    return -x1*y2 + y1*x2 > 0;
}
bool isInsideSector(int x, int y, int cx, int cy, int startX, int startY, int endX, int endY, float radius) 
{
    radius = radius * 180/M_PI;
    float dX =  cx - x;
    float dY = cy - y;

    if (dX*dX+dY*dY <= radius*radius)
    {
        return true;
    }
    return false;
}
void CircleSegment(int xc, int yc, float radius, float start, float end, ALLEGRO_COLOR col)
{
    float theta = start;
    float inc = 0.01f;
    while (theta <= end) {
        float xn = xc + radius * sin(theta);
        float yn = yc + radius * cos(theta);
        al_draw_pixel(xn, yn, col);
        theta = theta + inc;
    }

}
void DrawCone(int x, int y, float angle, float radius, int length)
{
    angle -= 135;
    int x2 = x + length; int y2 = y + length;
    int x3 = x + length; int y3 = y + length;

    radius = radius * M_PI/180.0f;
    angle = angle * M_PI/180.0f;
    float l_r = length * M_PI/180.0f;

    RotatePoint(&x2,&y2,x,y, -radius/2.0f+angle);
    RotatePoint(&x3,&y3,x,y, radius/2.0f+angle);


    ALLEGRO_KEYBOARD_STATE a;
    al_get_keyboard_state(&a);
    if (al_key_down(&a,ALLEGRO_KEY_Y))
    {
        printf("gg");
    }
    //al_draw_triangle(x,y,x2,y2,x3,y3,FRIENDLY,1);
    al_draw_line(x,y,x2,y2,FRIENDLY,1);
    al_draw_line(x,y,x3,y3,FRIENDLY,1);

    //float angle2 = atan(x2/(float)y2) - radius/2.0f;
    //float angle3 = atan(x3/(float)y3) + radius/2.0f;
    float angle2 = atan2(y-y2,x-x2);
    float angle3 = atan2(y-y3,x-x3);
    angle2+=1.578;
    angle3+=1.578;

    //angle2 -= angle;
    //angle3 -= angle;

    ALLEGRO_MOUSE_STATE m = GetMouseClamped();
    int startX = x2; int startY = y2;
    int endX = x3; int endY = y3;
    if (startY > endY)
    {
        int tempX = startX;
        int tempY = startY;

        startX = endX;
        startY = endY;

        endX = tempX;
        endY = tempY;

    }
    float distX = x - x2;
    float distY = y - y2;

    float l = sqrt(distX*distX+distY*distY);
   // if (isInsideSector(m.x,m.y,x,y,startX,startY,endX,endY,radius))
     //   CircleSegment(x,y,radius+(length*sqrt(2))-1,angle2,angle3,FRIENDLY);
    //else
    CircleSegment(x,y,l,angle2,angle3,ENEMY);


}