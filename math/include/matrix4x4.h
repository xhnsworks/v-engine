#ifndef MATRIX4X4_H
#define MATRIX4X4_H
#include "float_base.h"
#include "stack.h"
#ifdef USE_SSE
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#else
typedef struct __m128_t_
{
	float x, y, z, w;
} __m128;
typedef struct __m64_t_
{
	int x, y;
} __m64;
#endif
struct matrix4x4
{
#ifdef USE_SSE
	__m128 m0;
	__m128 m1;
	__m128 m2;
	__m128 m3;
	matrix4x4()
	{
		m0 = _mm_setzero_ps();
		m1 = _mm_setzero_ps();
		m2 = _mm_setzero_ps();
		m3 = _mm_setzero_ps();
	}
	matrix4x4(const matrix4x4& mat)
	{
		m0 = mat.m0;
		m1 = mat.m1;
		m2 = mat.m2;
		m3 = mat.m3;
	}
    matrix4x4& operator = (const matrix4x4& mat)
	{
		m0 = mat.m0;
		m1 = mat.m1;
		m2 = mat.m2;
		m3 = mat.m3;
		return *this;
	}
#else
	float m[4][4];
	matrix4x4()
	{
		memset(m, 0, sizeof(m));
	}
	matrix4x4(const matrix4x4& mat)
	{
		memcpy(m, mat.m, sizeof(m));
	}
	matrix4x4& operator = (const matrix4x4& mat)
	{
		memcpy(m, mat.m, sizeof(m));
		return *this;
	}
#endif
};

typedef matrix4x4* Matrix4x4;
matrix4x4* _Matrix4x4_new(const char* _file, euint _line);
void _Matrix4x4_delete(matrix4x4* _mat, const char* _file, euint _line);
#define Matrix4x4_new() _Matrix4x4_new(__FILE__, __LINE__)
#define Matrix4x4_delete(m) _Matrix4x4_delete(m, __FILE__, __LINE__)
void Matrix4x4_Dest(matrix4x4* _mat);
void Matrix4x4_set_translate(matrix4x4* _mat, float x, float y, float z);
void Matrix4x4_set_as_translate(matrix4x4* _mat, float x, float y, float z);
EFloat3 Matrix4x4_get_translate(matrix4x4* _mat);
void Matrix4x4_set_zero(matrix4x4* _mat);
void Matrix4x4_set_one(matrix4x4* _mat);
void Matrix4x4_export(matrix4x4* _mat, float* _ret);
void Matrix4x4_import(matrix4x4* _mat, float* _src);
void Matrix4x4_blend(matrix4x4* _mat);
void Matrix4x4_mul_matrix4(const matrix4x4* m1, const matrix4x4* m2, matrix4x4* result);
void Matrix4x4_assign(matrix4x4* _dst, const matrix4x4* _src);
///API_EXPORT void Matrix4x4_projection(matrix4x4* _mat, float _left, float _right, float _top, float _bottom, float _near, float _far);
void FORCE_ALIGN_ARG_POINTER Matrix4x4_projection(matrix4x4* _mat, float _width, float _height, float _near, float _far);
void FORCE_ALIGN_ARG_POINTER Matrix4x4_orthogonal(matrix4x4* _mat, float _width, float _height, float _near, float _far);
void Matrix4x4_proj_scale(matrix4x4* _mat, float x_scale, float y_scale);
void Matrix4x4_inverse(matrix4x4* _mat);
void Matrix4x4_print(matrix4x4* _mat);
void Matrix4x4_log(matrix4x4* _mat);
void Matrix4x4_turn(matrix4x4* _mat);
sfloat4 FORCE_ALIGN_ARG_POINTER Matrix4x4_mul_float4(const matrix4x4* _mat, sfloat4 _ft);
sfloat3 FORCE_ALIGN_ARG_POINTER Matrix4x4_mul_float3(const matrix4x4* _mat, sfloat3 _ft);
void Matrix4x4_set_x_axis_rotate(matrix4x4* _mat, float _r);
void Matrix4x4_set_y_axis_rotate(matrix4x4* _mat, float _r);
void Matrix4x4_set_z_axis_rotate(matrix4x4* _mat, float _r);
void Matrix4x4_set_scale(matrix4x4* _mat, float x, float y, float z);
void FORCE_ALIGN_ARG_POINTER Matrix4x4_from_axis_angle(matrix4x4* _mat, sfloat3 _axis, float _radian);
void FORCE_ALIGN_ARG_POINTER Matrix4x4_from_vec_to_vec(matrix4x4* _mat, sfloat3 src_vec, sfloat3 dst_vec);

void Matrix4x4_set_right_z_axis(matrix4x4* _mat);
void Matrix4x4_set_right_y_axis(matrix4x4* _mat);
void Matrix4x4_set_right_x_axis(matrix4x4* _mat);

void Matrix4x4_mul_matrix4_test();
/**
typedef struct matrix4x4_array_
{
    struct _matrix4x4* self;
} Matrix4x4Array;

Matrix4x4Array Matrix4x4Array_new();
void Matrix4x4Array_delete(Matrix4x4Array _m4_array);
void Matrix4x4Array_push(Matrix4x4Array _m4_array, matrix4x4* _mt4);
matrix4x4* Matrix4x4Array_pop(Matrix4x4Array _m4_array);
matrix4x4* Matrix4x4Array_get(Matrix4x4Array _m4_array, uint32 _i);
uint32 Matrix4x4Array_get_size(Matrix4x4Array _m4_array);
float* Matrix4x4Array_get_ptr(Matrix4x4Array _m4_array);
void Matrix4x4Array_export(Matrix4x4Array _m4_array, float* _ret);
**/
#endif
