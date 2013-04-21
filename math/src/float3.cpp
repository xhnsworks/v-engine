#include "float3.h"
#include "elog.h"
#include "eassert.h"

union float3_ptr
{
    float3  ft3;
    __m128* m128;
    float*  ft128;
    ref_ptr r_ptr;
};

float3 Float3(float _x, float _y, float _z)
{
#ifdef USE_SSE
    __m128* ret = __m128_alloc();
    *ret = _mm_set_ps(0.0, _z, _y, _x);
    return (float3)ret;
#else
#endif
}

API_EXPORT float3 Float3_assign(float2 _f2)
{
#ifdef USE_SSE
    __m128* ret = __m128_alloc();
    *ret = _mm_set_ps(0.0, 0.0, _f2->y, _f2->y);
    return (float3)ret;
#else
#endif
}

float Float3_dot(float3 _f3_0, float3 _f3_1)
{
#ifdef USE_SSE
    union float3_ptr f3_0, f3_1;
    f3_0.ft3 = _f3_0;
    f3_1.ft3 = _f3_1;
  #ifdef DEBUG
    EAssert(f3_0.r_ptr % 16 == 0, "%s", "float3 error");
    EAssert(f3_1.r_ptr % 16 == 0, "%s", "float3 error");

    EAssert(f3_0.ft128[3] == 0.0f, "%s", "float3 error");
    EAssert(f3_1.ft128[3] == 0.0f, "%s", "float3 error");
  #endif
    __m128 tmp0 = _mm_mul_ps(*f3_0.m128, *f3_1.m128);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    return *((float*)&tmp0);
#else
#endif
}

float3 Float3_cross(float3 _f3_0, float3 _f3_1)
{
#ifdef USE_SSE
    union float3_ptr f3_0, f3_1;
    f3_0.ft3 = _f3_0;
    f3_1.ft3 = _f3_1;
  #ifdef DEBUG
    EAssert(f3_0.r_ptr % 16 == 0, "%s", "float3 error");
    EAssert(f3_1.r_ptr % 16 == 0, "%s", "float3 error");

    EAssert(f3_0.ft128[3] == 0.0f, "%s", "float3 error");
    EAssert(f3_1.ft128[3] == 0.0f, "%s", "float3 error");
  #endif
    __m128 ea = _mm_shuffle_ps(*f3_0.m128, *f3_0.m128, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 eb = _mm_shuffle_ps(*f3_1.m128, *f3_1.m128, _MM_SHUFFLE( 3, 1, 0, 2 ));
    __m128 xa = _mm_mul_ps(ea, eb);
    __m128 tmp2 = _mm_shuffle_ps(*f3_0.m128, *f3_0.m128, _MM_SHUFFLE( 3, 1, 0, 2 ));
    __m128 tmp3 = _mm_shuffle_ps(*f3_1.m128, *f3_1.m128, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 xb = _mm_mul_ps(tmp2, tmp3);
    __m128* ret = __m128_alloc();
    *ret = _mm_sub_ps(xa, xb);
    *ret = _mm_and_ps(*ret, *((__m128*)g_float3_mask));
    return (float3)ret;
#else
#endif
}

float3 Float3_add(float3 _f3_0, float3 _f3_1)
{
#ifdef USE_SSE
    union float3_ptr f3_0, f3_1;
    f3_0.ft3 = _f3_0;
    f3_1.ft3 = _f3_1;
  #ifdef DEBUG
    EAssert(f3_0.r_ptr % 16 == 0, "%s", "float3 error");
    EAssert(f3_1.r_ptr % 16 == 0, "%s", "float3 error");

    EAssert(f3_0.ft128[3] == 0.0f, "%s", "float3 error");
    EAssert(f3_1.ft128[3] == 0.0f, "%s", "float3 error");
  #endif
    __m128* ret = __m128_alloc();
    *ret = _mm_add_ps(*f3_0.m128, *f3_1.m128);
    return (float3)ret;
#else
#endif
}

float3 Float3_sub(float3 _f3_0, float3 _f3_1)
{
#ifdef USE_SSE
    union float3_ptr f3_0, f3_1;
    f3_0.ft3 = _f3_0;
    f3_1.ft3 = _f3_1;
  #ifdef DEBUG
    EAssert(f3_0.r_ptr % 16 == 0, "%s", "float3 error");
    EAssert(f3_1.r_ptr % 16 == 0, "%s", "float3 error");

    EAssert(f3_0.ft128[3] == 0.0f, "%s", "float3 error");
    EAssert(f3_1.ft128[3] == 0.0f, "%s", "float3 error");
  #endif
    __m128* ret = __m128_alloc();
    *ret = _mm_sub_ps(*f3_0.m128, *f3_1.m128);
    return (float3)ret;
#else
#endif
}

float3 Float3_mul_float(float _ft, float3 _f3)
{
#ifdef USE_SSE
    union float3_ptr f3;
    f3.ft3 = _f3;
  #ifdef DEBUG
    EAssert(f3.r_ptr % 16 == 0, "%s", "float3 error");
    EAssert(f3.ft128[3] == 0.0f, "%s", "float3 error");
  #endif
    __m128 tmp1 = _mm_set1_ps(_ft);
    __m128* ret = __m128_alloc();
    *ret = _mm_mul_ps(*f3.m128, tmp1);
    return (float3)ret;
#else
#endif
}

float3 Float3_normalize(float3 _f3)
{
#ifdef USE_SSE
    union float3_ptr f3;
    f3.ft3 = _f3;
  #ifdef DEBUG
    EAssert(f3.r_ptr % 16 == 0, "%s", "float3 error");
    EAssert(f3.ft128[3] == 0.0f, "%s", "float3 error");
  #endif
    __m128 tmp0 = *f3.m128;
    __m128 tmp1 = tmp0;
    tmp0 = _mm_mul_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_sqrt_ps(tmp0);
    __m128* ret = __m128_alloc();
    *ret = _mm_div_ps(tmp1, tmp0);
    *ret = _mm_and_ps(*ret, *((__m128*)g_float3_mask));
    return (float3)ret;
#else
#endif
}

float Float3_length(float3 _f3)
{
#ifdef USE_SSE
    union float3_ptr f3;
    f3.ft3 = _f3;
  #ifdef DEBUG
    EAssert(f3.r_ptr % 16 == 0, "%s", "float3 error");
    EAssert(f3.ft128[3] == 0.0f, "%s", "float3 error");
  #endif
    __m128 tmp0 = *f3.m128;
    __m128 tmp1 = tmp0;
    tmp0 = _mm_mul_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_sqrt_ps(tmp0);

    return *( (float*)&tmp0 );
#else
#endif
}

void Float3_export(float3 _f3, float* _fout)
{
#ifdef USE_SSE
    _fout[0] = ((float*)_f3)[0];
    _fout[1] = ((float*)_f3)[1];
    _fout[2] = ((float*)_f3)[2];
#else
#endif
}

EFloat3 Float3_convert(float3 _f3)
{
    return *_f3;
}

void Float3_print(float3 _f3)
{
    printf("x %f y %f z %f\n", _f3->x, _f3->y, _f3->z);
}

void Float3_log(float3 _f3)
{
    elog("x %f y %f z %f\n", _f3->x, _f3->y, _f3->z);
}
