#include "helperfuncs.h"
#include "sprite.h"
#include <math.h>
#include "rect.h"
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
    
    if (beforeX > 255*_RENDERSIZE)
        mouse.x = 255;
    if (beforeY > 255*_RENDERSIZE)
        mouse.y = 255;
    if (beforeX < 0)
        mouse.x = 0;
   if (beforeY < 0)
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
double RandRange(double min, double max)
{
   return ((max - min) * ((double)rand() / RAND_MAX)) + min;
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