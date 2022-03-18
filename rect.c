#include "rect.h"
#include "vector2.h"
#include "helperfuncs.h"
#include <math.h>
bool CheckIntersect(Rect r1, Rect r2)
{
    if (r1.x < r2.x + r2.w && r1.x + r1.w > r2.x) 
        if (r1.y < r2.y + r2.h && r1.h + r1.y > r2.y) 
             return true;
    return false;
}
void GetRectIntersectBound(float x, float y, float w, float h, float x2, float y2, float w2, float h2, float* resultX, float* resultY, float* resultW, float* resultH)
{
    float Amin, Amax, Bmin, Bmax;
    
    Amin = x;
    Amax = Amin + w;
    Bmin = x2;
    Bmax = Bmin + w2;
    Amin = Bmin > Amin ? Bmin : Amin;
    *resultX = Amin;

    Bmax = Bmax < Amax ? Bmax : Amax;
    *resultW = Amax - Amin;

    Amin = y;
    Amax = Amin + h;
    Bmin = y2;
    Bmax = Bmin + h2;
    Amin = Bmin > Amin ? Bmin : Amin;
    *resultY = Amin;

    Bmax = Bmax < Amax ? Bmax : Amax;
    *resultH = Amax - Amin;
}
bool PointInRect(float x, float y, Rect r)
{
    if (x > r.x && x < r.x + r.w)
    {
        if (y > r.y && y < r.y + r.h)
        {
            return true;
        }
    }
    return false;
}
Rect UnionRect(Rect r1, Rect r2)
{
    float lowestx = _MIN(r1.x,r2.x);
    float lowesty = _MIN(r1.y,r2.y);
    float right = _MAX(r1.x+r1.w,r2.x+r2.w);
    float bottom = _MAX(r1.y+r1.h,r2.y+r2.h);

    return (Rect){lowestx,lowesty,right-lowestx,bottom-lowesty};
}
bool RectsTouch(Rect r1, Rect r2, float delta)
{
    Rect unioned = UnionRect(r1,r2);
    if (unioned.w < r1.w+r2.w+delta)
    {
        if (unioned.h < r1.h+r2.h+delta)
        {
            return true;
        }
    }
    return false;
}