#include "fixed_point.h"

static q32 saturate(int64_t x)
{
    if (x > INT32_MAX) return Q32_MAX;
    if (x < INT32_MIN) return Q32_MIN;
    return (q32)x;
}

q32 q32_from_int(int32_t x)
{
    return saturate((int64_t)x * Q32_ONE);
}

q32 q32_add(q32 a, q32 b)
{
    return saturate((int64_t) a + b);
}

q32 q32_sub(q32 a, q32 b)
{
    return saturate((int64_t) a - b);
}

q32 q32_mul(q32 a, q32 b)
{
    int64_t p = (int64_t)a * b;
    return saturate(p >> Q32_FRAC_BITS);
}

q32 q32_div(q32 a, q32 b)
{
    if (b==0) {
        return (a >= 0) ? Q32_MAX : Q32_MIN;
    }
    int64_t n = (int64_t)a * Q32_ONE;
    return saturate( n / b);
}