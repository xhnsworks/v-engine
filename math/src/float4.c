#include "float4.h"
#include "elog.h"
#include "eassert.h"

union float4_ptr
{
    float4  float4;
    __m128* m128;
    float*  float128;
    ref_ptr ref_ptr;
};

float4 Float4(float _x, float _y, float _z, float _w)
{
#ifdef USE_SSE
    __m128* ret = __m128_alloc();
    *ret = _mm_set_ps(_w, _z, _y, _x);
    return (float4)ret;
#else
#endif
}

float4 Float4_assign(float3 _f3)
{
#ifdef USE_SSE
    __m128* ret = __m128_alloc();
    *ret = _mm_set_ps(1.0f, _f3->z, _f3->y, _f3->x);
    return (float4)ret;
#else
#endif
}

float Float4_dot(float4 _f4_0, float4 _f4_1)
{
#ifdef USE_SSE
    union float4_ptr f4_0, f4_1;
    f4_0.float4 = _f4_0;
    f4_1.float4 = _f4_1;
  #ifdef DEBUG
    EAssert(f4_0.ref_ptr % 16 == 0, "float4 error");
    EAssert(f4_1.ref_ptr % 16 == 0, "float4 error");

    EAssert(f4_0.float128[3] == 0.0f, "float4 error");
    EAssert(f4_1.float128[3] == 0.0f, "float4 error");
  #endif
    __m128 tmp0 = _mm_and_ps(*f4_0.m128, *((__m128*)g_float3_mask));
    __m128 tmp1 = _mm_and_ps(*f4_1.m128, *((__m128*)g_float3_mask));

    tmp0 = _mm_mul_ps(tmp0, tmp1);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    return *((float*)&tmp0);
#else
    return _f4_0->x * _f4_1->x + _f4_0->y * _f4_1->y + _f4_0->z * _f4_1->z;
#endif
}

float4 Float4_cross(float4 _f4_0, float4 _f4_1)
{
#ifdef USE_SSE
    union float4_ptr f4_0, f4_1;
    f4_0.float4 = _f4_0;
    f4_1.float4 = _f4_1;
  #ifdef DEBUG
    EAssert(f4_0.ref_ptr % 16 == 0, "float4 error");
    EAssert(f4_1.ref_ptr % 16 == 0, "float4 error");

    EAssert(f4_0.float128[3] == 0.0f, "float4 error");
    EAssert(f4_1.float128[3] == 0.0f, "float4 error");
  #endif
    __m128 ea = _mm_shuffle_ps(*f4_0.m128, *f4_0.m128, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 eb = _mm_shuffle_ps(*f4_1.m128, *f4_1.m128, _MM_SHUFFLE( 3, 1, 0, 2 ));

    __m128 xa = _mm_mul_ps(ea, eb);
    __m128 tmp0 = _mm_shuffle_ps(*f4_0.m128, *f4_0.m128, _MM_SHUFFLE( 3, 1, 0, 2 ));
    __m128 tmp1 = _mm_shuffle_ps(*f4_1.m128, *f4_1.m128, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 xb = _mm_mul_ps(tmp0, tmp1);

    __m128* ret = __m128_alloc();
    *ret = _mm_sub_ps(xa, xb);
    return (float4)ret;
#else
#endif
}

float4 Float4_div_float(float _ft, float4 _f4)
{
#ifdef USE_SSE
    __m128 tmp0 = _mm_loadu_ps((float*)_f4);
    __m128 tmp1 = _mm_set1_ps(_ft);
    __m128* ret = __m128_alloc();
    *ret = _mm_div_ps(tmp0, tmp1);
    return (float4)ret;
#else
#endif
}

void Float4_export(float4 _f4, float* _fout)
{
#ifdef USE_SSE
    _fout[0] = ((float*)_f4)[0];
    _fout[1] = ((float*)_f4)[1];
    _fout[2] = ((float*)_f4)[2];
    _fout[3] = ((float*)_f4)[3];
#else
#endif
}

float3 Float4_xyz(float4 _f4)
{
#ifdef USE_SSE
    union float4_ptr f4;
    f4.float4 = _f4;
    *f4.m128 = _mm_and_ps(*f4.m128, *((__m128*)g_float3_mask));
    return (float3)_f4;
#else
#endif
}

EFloat4 Float4_convert(float4 _f4)
{
    return *_f4;
}

void Float4_print(float4 _f4)
{
    printf("##float4 %f %f %f %f\n", ((float*)_f4)[0], ((float*)_f4)[1], ((float*)_f4)[2], ((float*)_f4)[3]);
}

void Float4_log(float4 _f4)
{
    elog("##float4 %f %f %f %f\n", ((float*)_f4)[0], ((float*)_f4)[1], ((float*)_f4)[2], ((float*)_f4)[3]);
}
