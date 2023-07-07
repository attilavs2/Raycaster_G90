//---
// fixed: 48:16 fixed-point arithmetic
//---
//vient de https://gitea.planet-casio.com/Slyvtt/OutRun/src/branch/master/src
//
#pragma once
#include <stdint.h>

typedef int32_t fixed_t;
/* Standard arithmetic. */

static inline fixed_t fmul(fixed_t left, fixed_t right)
{
    /* Generally optimized by the compiler to use dmuls.l and xtrct */
    int64_t p = (int64_t)left * (int64_t)right;
    return (int32_t)(p >> 16);
}

static inline fixed_t fdiv(fixed_t left, fixed_t right)
{
    /* Pretty slow */
    int64_t d = (int64_t)left << 16;
    return d / right;
}

#define fix(x) ((int)((x) * 65536))

static inline fixed_t fixdouble(double constant)
{
    return (fixed_t)(constant * 65536);
}

static inline fixed_t fixfloat(float constant)
{
    return (fixed_t)(constant * 65536);
}

static inline fixed_t fdec(fixed_t f)
{
    return f & 0xffff;
}

static inline int ffloor(fixed_t f)
{
    return f >> 16;
}

static inline int fceil(fixed_t f)
{
    return (f + 0xffff) >> 16;
}

static inline int fround(fixed_t f)
{
    return (f + 0x8000) >> 16;
}

static inline float f2float(fixed_t f)
{
    return (float)f / 65536;
}

static inline double f2double(fixed_t f)
{
    return (double)f / 65536;
}

static inline double f2int(fixed_t f)
{
    return (int)f / 65536;
}

static inline fixed_t feasein(fixed_t x)
{
    return fmul(x, x);
}

static inline fixed_t fease(fixed_t x)
{
    if(x <= fix(0.5)) {
        return 2 * fmul(x, x);
    }
    else {
        x = fix(1) - x;
        return fix(1) - 2 * fmul(x, x);
    }
}
