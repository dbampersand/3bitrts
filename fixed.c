#include "fixed.h"


typeof(ff.f) fadd(fixed f1, fixed f2)
{
    return f1.f + f2.f;
}

typeof(ff.f) fsub(fixed f1, fixed f2)
{
    return f1.f - f2.f;
}
typeof(ff.f) fmul(fixed f1, fixed f2)
{
    return ((FIXED_PROMOTION)f1.f * (FIXED_PROMOTION)f2.f) / (1<<FIXED_PRECISION_BITS);
}
typeof(ff.f) fdiv(fixed f1, fixed f2)
{
    return ((FIXED_PROMOTION)f1.f * (F_1)) / f2.f;
}
typeof(ff.f) ftoi(fixed f)
{
    return f.f >> FIXED_PRECISION_BITS;
}
fixed itofix(int32_t f)
{
    return (fixed){.f = f * F_1};
}
float fixtofloat(fixed f)
{
    return (float)f.f / (float)F_1; 
}
fixed floattofix(float f)
{
    return (fixed){.f = (FIXED_TOTAL_PRECISION)(f * F_1)};
}