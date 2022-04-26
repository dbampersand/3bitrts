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
float extra=0;

bool PointInTri(int x0, int y0, int x1, int y1, int x2, int y2, int pX, int pY)
{
    //RotatePoint(&x1,&y1,x0,y0,extra);
    //RotatePoint(&x2,&y2,x0,y0,extra);

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

    al_draw_line(x1,y1,x2,y2,HEAL,1);
    al_draw_line(x2,y2,x0,y0,HEAL,1);
    al_draw_line(x0,y0,x1,y1,HEAL,1);

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

    //special case for when one of the lines crosses over 360degrees and the other doesnt
    //rotate all points back
    //this doesn't feel like  a good way to do this
    //also doesn't work r > 180
    //TODO: fix?
   /* if (angle2 + 2*rad >= 2*M_PI && (angle3 - 2*rad <= rad))
    {
        RotatePoint(&x,&y,cx,cy,(-rad));
        RotatePoint(&startX,&startY,cx,cy,(-rad));
        RotatePoint(&endX,&endY,cx,cy,(-rad));
        angle = (atan2(y-cy,x-cx));
        angle2 = (atan2(startY-cy,startX-cx));
        angle3 = (atan2(endY-cy,endX-cx));
        angle = Normalise(angle, 0, 2*M_PI);
        angle2 = Normalise(angle2,0, 2*M_PI);
        angle3 = Normalise(angle3,0, 2*M_PI);
        if (angle2 < angle3) 
        {
            float temp = angle2;
            angle2 = angle3;
            angle3 = temp;
        }
        al_draw_line(cx,cy,startX,startY,HEAL,1);
        al_draw_line(cx,cy,endX,endY,HEAL,1);

        //float temp = angle2;
        //angle2 = angle3;
        //angle3 = temp;

    }*/

    if (dX*dX+dY*dY <= length*length)
    {
        if ((angle <= angle2 && angle >= angle3) || (!isClockwise(startX,startY,x,y) && isClockwise(endX,endY,x,y)))
        {
            //return true;
        }
        float newX = startX-cx; float newY = startY-cy;
        float newX2 = endX-cx; float newY2 = endY-cy;
        Normalize(&newX,&newY);
        Normalize(&newX2,&newY2);
        newX*=length;
        newY*=length;

        newX2*=length;
        newY2*=length;
        newX += startX;
        newY += startY;
        newX2 += endX;
        newY2 += endY;


        float midX = startX;
        float midY = startY;
        //Normalize(&midX,&midY);
        //midX*=radius;
        //midY*=radius;

        float fdsfds = RadToDeg(radius);
       // float midAngle = atan2(endY-startY,endX-startX)- amtRotated  - DegToRad(60+45);// - DegToRad(45) - rad; 
        float midAngle = M_PI - ((atan2(endY-startY,endX-startX)) + DegToRad(90) - radius/2.0) + amtRotated;
        
        midAngle = Normalise(midAngle, 0, 2*M_PI);

        RotatePointF(&midX,&midY,cx,cy,(midAngle+extra));
        float midXNrm = midX;
        float midYNrm = midY;
        //Normalize(&midXNrm,&midYNrm);
       // midXNrm+=radius*2;
        //midYNrm+=radius*2;
        //midX += midXNrm;
        //midY += midYNrm;
        
        printf("%f,%f\n",midAngle,extra);
        //bool tri1 = PointInTri(cx,cy,newX,newY,midX,midY,x,y);
        //bool tri2 =  PointInTri(cx,cy,newX2,newY2,midX,midY,x,y);

        al_draw_line(cx,cy,midX,midY,POISON,1);

        //PointInTri(cx,cy,newX,newY,newX2,newY2,x,y));
        //if (tri1 || tri2)
        {
            printf("gg");
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
void DrawCone(int x, int y, float angle, float radius, int length)
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
    if (al_key_down(&a,ALLEGRO_KEY_Y))
    {
        extra+=0.1f;
    }
    if (al_key_down(&a,ALLEGRO_KEY_T))
    {
        extra-=0.1f;
    }
    //al_draw_triangle(x,y,x2,y2,x3,y3,FRIENDLY,1);
    al_draw_line(x,y,x2,y2,FRIENDLY,1);
    al_draw_line(x,y,x3,y3,FRIENDLY,1);

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
        CircleSegment(x,y,l,angle2,angle3,FRIENDLY,length); 
    else
        CircleSegment(x,y,l,angle2,angle3,ENEMY,length);
    printf("\n");

}