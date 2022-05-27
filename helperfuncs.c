#include "helperfuncs.h"
#include "sprite.h"
#include <math.h>
#include "rect.h"
#include "gameobject.h"
#include "video.h"
#include "gamestate.h"
#include "colors.h"
#include "allegro5/allegro_primitives.h"

float clamp(float f, float min, float max)
{
    return f < min ? min : f > max ? max : f;
}
bool CircleInRect(float cx, float cy, float rad, Rect r)
{
   float closestX = clamp(cx,r.x,r.x+r.w);
   float closestY = clamp(cy,r.y,r.y+r.h);

   float dx = closestX - cx;
   float dy = closestY - cy;

   return (dx * dx + dy * dy) <= rad * rad;
}

int GetIndex(int h, int x, int y)
{
    return (h*x)+y;
}
bool MoveTo(float* x, float* y, float toX, float toY, float speed, float delta)
{

        float moveX = toX - *x;
        float moveY = toY - *y;

        float dist = sqrt(moveX*moveX+moveY*moveY);

        if (dist <= DIST_DELTA)
        {
            *x = toX;
            *y = toY;
            return true;
        }
        float dX = (moveX / dist * speed) * delta;
        float dY = (moveY / dist * speed) * delta;

        *x += dX;
        *y += dY;
        return false;

}
ALLEGRO_MOUSE_STATE GetMouseClamped()
{
    ALLEGRO_MOUSE_STATE mouse;
    al_get_mouse_state(&mouse);
    int beforeX = mouse.x;
    int beforeY = mouse.y;
    mouse.x = mouse.x/(float)_RENDERSIZE;
    mouse.y = mouse.y/(float)_RENDERSIZE;

    if (mouse.x + MOUSECURSORSIZE > (_SCREEN_SIZE))
    {
        mouse.x = _SCREEN_SIZE - MOUSECURSORSIZE;

        //This function is giving very weird results
        //al_set_mouse_xy(display, x-MOUSECURSORSIZE*_RENDERSIZE, beforeY);
    }
    if (mouse.y > (_SCREEN_SIZE-MOUSECURSORSIZE))
    {
        mouse.y = 255 - MOUSECURSORSIZE;
    }
    if (mouse.x < 0)
        mouse.x = 0;
   if (mouse.y < 0)
        mouse.y = 0;

    return mouse;
    
}

char* readFile(const char* filename) {
    ALLEGRO_FILE* rw = al_fopen(filename, "rb");
    if (rw == NULL) return NULL;

    int64_t res_size = al_fsize(rw);
    char* res = (char*)malloc(res_size + 1);

    int64_t nb_read_total = 0, nb_read = 1;
    char* buf = res;
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = al_fread(rw,buf,1);
        nb_read_total += nb_read;
        buf += nb_read;
    }
    al_fclose(rw);
    if (nb_read_total != res_size) {
        free(res);
        return NULL;
    }

    res[nb_read_total] = '\0';
    return res;
}
void lerpValue(float* x, float tX, float t)
{
    *x = (1.0f - t) * *x + t * tX;
    if (t >= 1.0f)
    {
        *x = tX;
    }
}
void MoveAngle(float* x, float* y, float dx, float dy, float speed, float delta)
{
    *x += dx * delta * speed;
    *y += dy * delta * speed;

}
void Normalize(float* x, float* y)
{
    float mag = sqrt((*x**x)+(*y**y));
    *x /= mag;
    *y /= mag;
}
float magnitude(float* x, float* y)
{
    return  sqrt((*x**x)+(*y**y));
}
double RandRange(double min, double max)
{
   return ((max - min) * ((double)rand() / RAND_MAX)) + min;
}    
bool LineIntersectsObj(GameObject* g, float xLine, float yLine, float x2Line, float y2Line)
{   
    typedef struct line { 
        float x; float y; float x2; float y2;
    } line; 

    int w = GetWidth(g);
    int h = GetHeight(g);

    line top = {g->position.x,g->position.y,g->position.x + w,g->position.y}; 
    line right = {g->position.x + w, g->position.y, g->position.x + w, g->position.y + h}; 
    line bottom = {g->position.x, g->position.y + h, g->position.x + w, g->position.y + h}; 
    line left = {g->position.x,g->position.y,g->position.x,g->position.y+h}; 

    float intersectX;
    float intersectY;

    if (GetLineIntersection(top.x,top.y,top.x2,top.y2,xLine,yLine,x2Line,y2Line,&intersectX,&intersectY))
        return true;
    if (GetLineIntersection(right.x,right.y,right.x2,right.y2,xLine,yLine,x2Line,y2Line,&intersectX,&intersectY))
        return true;
    if (GetLineIntersection(bottom.x,bottom.y,bottom.x2,bottom.y2,xLine,yLine,x2Line,y2Line,&intersectX,&intersectY))
        return true;
    if (GetLineIntersection(left.x,left.y,left.x2,left.y2,xLine,yLine,x2Line,y2Line,&intersectX,&intersectY))
        return true;

    return false;
}
//note: taken from https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
bool GetLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float* out_x, float* out_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;    
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     
    s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        *out_x = p0_x + (t * s1_x);
        *out_y = p0_y + (t * s1_y);
        return true;
    }

    return false; 
}
float dot(float x1, float y1, float x2, float y2)
{
    return (x1*x2)+(y1*y2);
}
float dist(float x1, float y1, float x2, float y2)
{
    float moveX = x2 - x1;
    float moveY = y2 - y1;

    return sqrt(moveX*moveX+moveY*moveY);
}
void ClampToRadius(float* x, float* y, float cx, float cy, float radius)
{

    float distX = *x - cx; 
    float distY = *y - cy;
    float dist = sqrt(distX*distX+distY*distY);
            

    *x = *x - cx;
    *y = *y - cy;
    Normalize(x,y); 
    *x = cx + *x*_MIN(radius,dist);
    *y = cy + *y*_MIN(radius,dist);
}
bool PointInCircle(float x, float y, float cx, float cy, float radius)
{
    
    return (dist(x,y,cx,cy) < radius);
}
float cotan(float i)
{
    return 1 / tan(i);
}
void RotatePoint(int* x, int* y, int cx, int cy, float angle)
{
    *x -= cx;
    *y -= cy;

    int xN = *x * cos(angle) - *y * sin(angle);
    int yN = *y * cos(angle) + *x * sin(angle);

    *x = xN + cx;
    *y = yN + cy;
}
void RotatePointF(float* x, float* y, float cx, float cy, float angle)
{
    *x -= cx;
    *y -= cy;

    int xN = *x * cos(angle) - *y * sin(angle);
    int yN = *y * cos(angle) + *x * sin(angle);

    *x = xN + cx;
    *y = yN + cy;

}
float Normalise(float v, float start, float end)
{
    float w = end-start;
    float offset = v - start;

    return (offset - (floor(offset/w)*w))+start;
}
int sign(float j)
{
    if (j < 0)
        return -1;
    else
        return 1;
}
float DegToRad(float deg)
{
    return deg * M_PI/180.0f;
}
float RadToDeg(float rad)
{
    return rad * 180/M_PI;
}
float CircleRectDist(int cx, int cy, float radius, Rect r)
{
    Point cd;
    cd.x = abs(cx - r.x);
    cd.y = abs(cy - r.y);

    if (cd.x > (r.w/2.0f + radius)) { return false; }
    if (cd.y > (r.h/2.0f + radius)) { return false; }

    if (cd.x <= (r.w/2.0f)) { return true; } 
    if (cd.y <= (r.h/2.0f)) { return true; }

    float cornerdistx = cd.x-r.w/2.0f;
    float cornerdisty = cd.y-r.h/2.0f;

    return (cornerdistx*cornerdistx + cornerdisty*cornerdisty <= radius*radius);
}
int NumDigits(int i)
{
    if (i == 0)
        return 1;
    return (int)ceil(log10(i))+1;
}
