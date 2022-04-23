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
void DrawCone(int x, int y, float angle, float radius, int length)
{
    int x2 = x + length; int y2 = y + length;
    int x3 = x + length; int y3 = y + length;

    RotatePoint(&x2,&y2,x,y, angle-radius/2.0f);
    RotatePoint(&x3,&y3,x,y, angle);



    al_draw_triangle(x,y,x2,y2,x3,y3,FRIENDLY,1);
}