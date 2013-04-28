#ifndef SFLOAT3_H
#define SFLOAT3_H
#include "float_base.h"
#include "elog.h"
#include "eassert.h"
#include "common.h"

static inline sfloat3 SFloat3(float _x, float _y, float _z)
{
#ifdef USE_SSE
    return _mm_set_ps(0.0, _z, _y, _x);
#else
#endif
}

static inline sfloat3 SFloat3_assign_from_float(float a)
{
    return SFloat3(a, a, a);
}

static inline sfloat3 SFloat3_assign_from_int3(int x, int y, int z)
{
#ifdef USE_SSE
    __m128i tmp = _mm_set_epi32(0, z, y, x);
    return _mm_cvtepi32_ps(tmp);
#else
#endif
}

static inline sfloat3 SFloat3_assign_from_float3(float3 ft3)
{
#ifdef USE_SSE
    return _mm_set_ps(0.0f, ft3->z, ft3->y, ft3->x);
#else
#endif
}

static inline sfloat3 SFloat3_assign_from_efloat3(EFloat3* ft3)
{
#ifdef USE_SSE
    return _mm_set_ps(0.0f, ft3->z, ft3->y, ft3->x);
#else
#endif
}

static inline sfloat3 SFloat3_assign_from_float_array(float* ft3)
{
#ifdef USE_SSE
	return _mm_set_ps(0.0f, ft3[2], ft3[1], ft3[0]);
#else
#endif
}

static inline float SFloat3_dot(sfloat3 f3_0, sfloat3 f3_1)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3_0);
    SFloat3Assert(f3_1);
  #endif
    __m128 tmp0 = _mm_mul_ps(f3_0, f3_1);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    return *((float*)&tmp0);
#else
#endif
}

static inline sfloat3 SFloat3_dot_sfloat3(sfloat3 f3_0, sfloat3 f3_1)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3_0);
    SFloat3Assert(f3_1);
  #endif
    __m128 tmp0 = _mm_mul_ps(f3_0, f3_1);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_and_ps(tmp0, *((__m128*)g_float3_mask));
    return tmp0;
#else
#endif
}

static inline sfloat3 SFloat3_cross(sfloat3 f3_0, sfloat3 f3_1)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3_0);
    SFloat3Assert(f3_1);
  #endif
    __m128 ea = _mm_shuffle_ps(f3_0, f3_0, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 eb = _mm_shuffle_ps(f3_1, f3_1, _MM_SHUFFLE( 3, 1, 0, 2 ));
    __m128 xa = _mm_mul_ps(ea, eb);
    __m128 tmp2 = _mm_shuffle_ps(f3_0, f3_0, _MM_SHUFFLE( 3, 1, 0, 2 ));
    __m128 tmp3 = _mm_shuffle_ps(f3_1, f3_1, _MM_SHUFFLE( 3, 0, 2, 1 ));
    __m128 xb = _mm_mul_ps(tmp2, tmp3);
    __m128 ret = _mm_sub_ps(xa, xb);
    ret = _mm_and_ps(ret, *((__m128*)g_float3_mask));
    return ret;
#else
#endif
}

static inline sfloat3 SFloat3_add(sfloat3 f3_0, sfloat3 f3_1)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3_0);
    SFloat3Assert(f3_1);
  #endif
    return _mm_add_ps(f3_0, f3_1);
#else
#endif
}

static inline sfloat3 SFloat3_sub(sfloat3 f3_0, sfloat3 f3_1)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3_0);
    SFloat3Assert(f3_1);
  #endif
    return _mm_sub_ps(f3_0, f3_1);
#else
#endif
}

inline sfloat3 SFloat3_mul_float(float _ft, sfloat3 f3)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3);
  #endif
    __m128 tmp1 = _mm_set1_ps(_ft);
    return _mm_mul_ps(f3, tmp1);
#else
#endif
}

inline sfloat3 SFloat3_mul(sfloat3 f3_0, sfloat3 f3_1)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3_0);
    SFloat3Assert(f3_1);
  #endif
    return _mm_mul_ps(f3_0, f3_1);
#else
#endif
}

inline sfloat3 SFloat3_div(sfloat3 f3_0, sfloat3 f3_1)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3_0);
    SFloat3Assert(f3_1);
  #endif
    __m128 ret = _mm_div_ps(f3_0, f3_1);
    ret = _mm_and_ps(ret, *((__m128*)g_float3_mask));
    return ret;
#else
#endif
}

inline sfloat3 SFloat3_normalize(sfloat3 f3)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3);
  #endif
    __m128 tmp0 = f3;
    __m128 tmp1 = tmp0;
    tmp0 = _mm_mul_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_sqrt_ps(tmp0);
    __m128 ret = _mm_div_ps(tmp1, tmp0);
    ret = _mm_and_ps(ret, *((__m128*)g_float3_mask));
    return ret;
#else
#endif
}

inline float SFloat3_length(sfloat3 f3)
{
#ifdef USE_SSE
  #ifdef DEBUG
    SFloat3Assert(f3);
  #endif
    __m128 tmp0 = f3;
    ///__m128 tmp1 = tmp0;
    tmp0 = _mm_mul_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_sqrt_ps(tmp0);

    return *( (float*)&tmp0 );
#else
#endif
}

static inline void SFloat3_export(sfloat3 f3, float* fout)
{
#ifdef USE_SSE
    fout[0] = ((float*)&f3)[0];
    fout[1] = ((float*)&f3)[1];
    fout[2] = ((float*)&f3)[2];
#else
#endif
}

static inline void SFloat3_export_int(sfloat3 f3, int* iout)
{
#ifdef USE_SSE
    #ifdef DEBUG
        SFloat3Assert(f3);
    #endif
    __m128i tmp0 = _mm_cvttps_epi32(f3);
    iout[0] = ((int*)&tmp0)[0];
    iout[1] = ((int*)&tmp0)[1];
    iout[2] = ((int*)&tmp0)[2];
#else
#endif
}

typedef struct _sfloat3_compare_result
{
    bool x_cmp_ret;
    bool y_cmp_ret;
    bool z_cmp_ret;
} sfloat3_compare_result;

static inline sfloat3_compare_result SFloat3_less(sfloat3 a, sfloat3 b)
{
#ifdef USE_SSE
    __m128 tmp0 = _mm_cmplt_ps(a, b);
    float* fp = (float*)&tmp0;
    sfloat3_compare_result ret;
    ret.x_cmp_ret = *((bool*)&fp[0]);
    ret.y_cmp_ret = *((bool*)&fp[1]);
    ret.z_cmp_ret = *((bool*)&fp[2]);
    return ret;
#else
#endif
}

static inline sfloat3_compare_result SFloat3_less_equal(sfloat3 a, sfloat3 b)
{
#ifdef USE_SSE
    __m128 tmp0 = _mm_cmple_ps(a, b);
    float* fp = (float*)&tmp0;
    sfloat3_compare_result ret;
    ret.x_cmp_ret = *((bool*)&fp[0]);
    ret.y_cmp_ret = *((bool*)&fp[1]);
    ret.z_cmp_ret = *((bool*)&fp[2]);
    return ret;
#else
#endif
}

static inline sfloat3_compare_result SFloat3_greater(sfloat3 a, sfloat3 b)
{
#ifdef USE_SSE
    __m128 tmp0 = _mm_cmpgt_ps(a, b);
    float* fp = (float*)&tmp0;
    sfloat3_compare_result ret;
    ret.x_cmp_ret = *((bool*)&fp[0]);
    ret.y_cmp_ret = *((bool*)&fp[1]);
    ret.z_cmp_ret = *((bool*)&fp[2]);
    return ret;
#else
#endif
}

static inline sfloat3_compare_result SFloat3_greater_equal(sfloat3 a, sfloat3 b)
{
#ifdef USE_SSE
    __m128 tmp0 = _mm_cmpge_ps(a, b);
    float* fp = (float*)&tmp0;
    sfloat3_compare_result ret;
    ret.x_cmp_ret = *((bool*)&fp[0]);
    ret.y_cmp_ret = *((bool*)&fp[1]);
    ret.z_cmp_ret = *((bool*)&fp[2]);
    return ret;
#else
#endif
}

static inline sfloat3_compare_result SFloat3_equal(sfloat3 a, sfloat3 b)
{
#ifdef USE_SSE
    __m128 tmp0 = _mm_cmpeq_ps(a, b);
    float* fp = (float*)&tmp0;
    sfloat3_compare_result ret;
    ret.x_cmp_ret = *((bool*)&fp[0]);
    ret.y_cmp_ret = *((bool*)&fp[1]);
    ret.z_cmp_ret = *((bool*)&fp[2]);
    return ret;
#else
#endif
}

static inline void SFloat3_print(sfloat3 f3)
{
    printf("x %f y %f z %f\n", ((float*)&f3)[0], ((float*)&f3)[1], ((float*)&f3)[2]);
}

static inline void SFloat3_log(sfloat3 f3)
{
    elog("x %f y %f z %f\n", ((float*)&f3)[0], ((float*)&f3)[1], ((float*)&f3)[2]);
}

static inline EFloat3 SFloat3_convert(sfloat3 f3)
{
    EFloat3 ret;
    SFloat3_export(f3, (float*)&ret);
    return ret;
}

static inline float SFloat3_get_x(const sfloat3* f3)
{
    return ((float*)f3)[0];
}

static inline float SFloat3_get_y(const sfloat3* f3)
{
    return ((float*)f3)[1];
}

static inline float SFloat3_get_z(const sfloat3* f3)
{
    return ((float*)f3)[2];
}

static inline sfloat3 SFloat3_cale_mid_point(sfloat3 a, sfloat3 b)
{
	sfloat3 tmp0 = SFloat3_sub(a, b);
	sfloat3 tmp1 = SFloat3_assign_from_float(0.5f);
	tmp0 = SFloat3_mul(tmp0, tmp1);
	tmp0 = SFloat3_add(tmp0, b);
	return tmp0;
}
#ifdef __cplusplus
struct MakeSFloat3
{
	inline sfloat3 operator() (float _x, float _y, float _z) {
		return SFloat3(_x, _y, _z);
	}
};
#endif
#endif
