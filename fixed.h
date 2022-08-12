#pragma once
#include "stdint.h"

#define FIXED_PRECISION_BITS 16
#define FIXED_TOTAL_PRECISION int32_t
#define FIXED_PROMOTION int64_t

#define F_1 (1 << FIXED_PRECISION_BITS)

typedef struct fixed
{
    FIXED_TOTAL_PRECISION f;
} fixed;

//just to be able to use typeof macro 
fixed ff;

typeof(ff.f) fadd(fixed f1, fixed f2);
typeof(ff.f) fsub(fixed f1, fixed f2);
typeof(ff.f) fmul(fixed f1, fixed f2);
typeof(ff.f) fdiv(fixed f1, fixed f2);
typeof(ff.f) ftoi(fixed f);
fixed itofix(int32_t f);
float fixtofloat(fixed f);
fixed floattofix(float f);
