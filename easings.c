#include "easings.h"
#include <math.h>


#ifndef M_PI
    #define M_PI 3.14159265358979323846f
#endif
float easeOutBack(float x) 
{
    const float c1 = 1.70158f;
    const float c3 = c1 + 1;

    return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}
float easeOutSine(float x)
{
  return sin((x * M_PI) / 2.0f);
}

float easeInOutBack(float x) 
{
    const float c1 = 1.70158f;
    const float c2 = c1 * 1.525f;
    
    if (x < 0.5f)
    {
        return (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2.0f;
    }
    else
    {
        return (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2.0f;
    }
}
float easeOutCubic(float x)
{
    return 1 - pow(1 - x, 3);
}   
float easeInOutSine(float x)
{
    return -(cos(M_PI * x) - 1) / 2.0f;
}

float EaseOutQuint(float x) {
    if (x > 1) return 1;
    return 1 - pow(1 - x, 5);
}
float easeInCirc(float x) {
    return 1 - sqrt(1 - pow(x, 2));
}
float easeOutCirc(float x){
    return sqrt(1 - pow(x - 1, 2));
}
float easeInOutQuint(float x)
{
    if (x < 0.5f)
    {
        return 16 * x * x * x * x * x;
    }
    return 1 - pow(-2 * x + 2, 5) / 2.0f;
}
float easeOutExpo(float x)
{
    if (x >= 1)
        return 1;
    else return (1 - pow(2, -10 * x));
}
float EaseInOutCubic(float f)
{
    if (f < 0.5f)
    {
        return 4 * f * f * f;
    }
    else
    {
        return  1 - pow(-2 * f + 2, 3) / 2.0f;
    }
}