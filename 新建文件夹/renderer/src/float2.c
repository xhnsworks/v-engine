#include "float2.h"

float2 Float2(float _x, float _y)
{
#ifdef USE_SSE
    __m128* ret = __m128_alloc();
    *ret = _mm_set_ps(0.0, 0.0, _y, _x);
    return (float2)ret;
#else
#endif
}

float Float2_dot(float2 _f2_0, float2 _f2_1)
{
#ifdef USE_SSE
    __m128 tmp0 = _mm_loadu_ps((float*)_f2_0);
    __m128 tmp1 = _mm_loadu_ps((float*)_f2_1);
    tmp0 = _mm_and_ps(tmp0, *g_float2_mask);
    tmp1 = _mm_and_ps(tmp1, *g_float2_mask);
    tmp0 = _mm_mul_ps(tmp0, tmp1);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    return *((float*)&tmp0);
#else
#endif
}

float2 Float2_cross(float2 _f2_0, float2 _f2_1)
{
#ifdef USE_SSE
    __m128 tmp0 = _mm_loadu_ps((float*)_f2_0);
    __m128 tmp1 = _mm_loadu_ps((float*)_f2_1);
    tmp0 = _mm_and_ps(tmp0, *g_float2_mask);
    tmp1 = _mm_and_ps(tmp1, *g_float2_mask);
    __m128 ea = _mm_shuffle_ps(tmp0, tmp0, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 eb = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE( 3, 1, 0, 2 ));
    __m128 xa = _mm_mul_ps(ea, eb);
    __m128 tmp2 = _mm_shuffle_ps(tmp0, tmp0, _MM_SHUFFLE( 3, 1, 0, 2 ));
    __m128 tmp3 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 xb = _mm_mul_ps(tmp2, tmp3);
    __m128* ret = __m128_alloc();
    *ret = _mm_sub_ps(xa, xb);
    return (float2)ret;
#else
#endif
}

EFloat2 Float2_convert(float2 _f2)
{
    return *_f2;
}
