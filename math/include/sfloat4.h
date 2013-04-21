#ifndef SFLOAT4_H
#define SFLOAT4_H

#include "float_base.h"
#include "elog.h"
#include "eassert.h"

static inline sfloat4 SFloat4(float _x, float _y, float _z, float _w)
{
#ifdef USE_SSE
    return _mm_set_ps(_w, _z, _y, _x);
#else
#endif
}

static inline sfloat4 SFloat4_assign(sfloat3 f3)
{
#ifdef USE_SSE
    ///return _mm_set_ps(1.0f, _f3->z, _f3->y, _f3->x);
    __m128 ret = f3;
    ((float*)&ret)[3] = 1.0f;
    return ret;
#else
#endif
}

static inline float SFloat4_dot(sfloat4 f4_0, sfloat4 f4_1)
{
#ifdef USE_SSE
  #ifdef DEBUG
    EAssert(((float*)&f4_0)[3] == 0.0f, "%s", "float4 error");
    EAssert(((float*)&f4_1)[3] == 0.0f, "%s", "float4 error");
  #endif
    __m128 tmp0 = _mm_and_ps(f4_0, *((__m128*)g_float3_mask));
    __m128 tmp1 = _mm_and_ps(f4_1, *((__m128*)g_float3_mask));

    tmp0 = _mm_mul_ps(tmp0, tmp1);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    return *((float*)&tmp0);
#else
    return _f4_0->x * _f4_1->x + _f4_0->y * _f4_1->y + _f4_0->z * _f4_1->z;
#endif
}

static inline sfloat4 SFloat4_cross(sfloat4 f4_0, sfloat4 f4_1)
{
#ifdef USE_SSE
  #ifdef DEBUG
    EAssert(((float*)&f4_0)[3] == 0.0f, "%s", "float4 error");
    EAssert(((float*)&f4_1)[3] == 0.0f, "%s", "float4 error");
  #endif
    __m128 ea = _mm_shuffle_ps(f4_0, f4_0, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 eb = _mm_shuffle_ps(f4_1, f4_1, _MM_SHUFFLE( 3, 1, 0, 2 ));

    __m128 xa = _mm_mul_ps(ea, eb);
    __m128 tmp0 = _mm_shuffle_ps(f4_0, f4_0, _MM_SHUFFLE( 3, 1, 0, 2 ));
    __m128 tmp1 = _mm_shuffle_ps(f4_1, f4_1, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 xb = _mm_mul_ps(tmp0, tmp1);

    __m128 ret = _mm_sub_ps(xa, xb);
    return ret;
#else
#endif
}

static inline sfloat4 SFloat4_sub(sfloat4 f4_0, sfloat4 f4_1)
{
#ifdef USE_SSE
    __m128 ret = _mm_sub_ps(f4_0, f4_1);
    ((float*)&ret)[3] = 0.0f;
    return ret;
#else
#endif
}

static inline sfloat4 SFloat4_div_float(float ft, sfloat4 f4)
{
#ifdef USE_SSE
    __m128 tmp0 = f4;
    __m128 tmp1 = _mm_set1_ps(ft);
    return _mm_div_ps(tmp0, tmp1);
#else
#endif
}

static inline void SFloat4_export(sfloat4 f4, float* fout)
{
#ifdef USE_SSE
    fout[0] = ((float*)&f4)[0];
    fout[1] = ((float*)&f4)[1];
    fout[2] = ((float*)&f4)[2];
    fout[3] = ((float*)&f4)[3];
#else
#endif
}

static inline sfloat4 SFloat4_normalize(sfloat4 f4)
{
#ifdef USE_SSE
    ///sfloat3 ret = SFloat3_normalize(SFloat4_xyz(f4));
    ///return SFloat4_assign(ret);
    __m128 tmp0 = _mm_and_ps(f4, *((__m128*)g_float3_mask));
    __m128 tmp1 = tmp0;
    tmp0 = _mm_mul_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_sqrt_ps(tmp0);
    __m128 ret = _mm_div_ps(tmp1, tmp0);
    ((float*)&ret)[3] = 1.0f;
    return ret;
#else
#endif
}

static inline sfloat3 SFloat4_xyz(sfloat4 f4)
{
#ifdef USE_SSE
    return _mm_and_ps(f4, *((__m128*)g_float3_mask));
#else
#endif
}

static inline EFloat4 SFloat4_convert(sfloat4 f4)
{
    EFloat4 ret;
    SFloat4_export(f4, (float*)&ret);
    return ret;
}

static inline void SFloat4_print(sfloat4 f4)
{
    printf("##float4 %f %f %f %f\n", ((float*)&f4)[0], ((float*)&f4)[1], ((float*)&f4)[2], ((float*)&f4)[3]);
}

static inline void SFloat4_log(sfloat4 f4)
{
    elog("##float4 %f %f %f %f\n", ((float*)&f4)[0], ((float*)&f4)[1], ((float*)&f4)[2], ((float*)&f4)[3]);
}

static inline void SFloat4_set_x(sfloat4* f4, float x)
{
    ((float*)f4)[0] = x;
}

static inline void SFloat4_set_y(sfloat4* f4, float y)
{
    ((float*)f4)[1] = y;
}

static inline void SFloat4_set_z(sfloat4* f4, float z)
{
    ((float*)f4)[2] = z;
}

static inline void SFloat4_set_w(sfloat4* f4, float w)
{
    ((float*)f4)[3] = w;
}

static inline float SFloat4_get_x(sfloat4* f4)
{
    return ((float*)f4)[0];
}

static inline float SFloat4_get_y(sfloat4* f4)
{
    return ((float*)f4)[1];
}

static inline float SFloat4_get_z(sfloat4* f4)
{
    return ((float*)f4)[2];
}

static inline float SFloat4_get_w(sfloat4* f4)
{
    return ((float*)f4)[3];
}
#endif
