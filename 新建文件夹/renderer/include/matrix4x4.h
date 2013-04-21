#ifndef MATRIX4X4_H
#define MATRIX4X4_H
#include "float_base.h"
#include "stack.h"
struct _matrix4x4;
typedef struct _matrix4x4* Matrix4x4;
extern Matrix4x4 _Matrix4x4_new(const char* _file, uint _line);
extern void _Matrix4x4_delete(Matrix4x4 _mat, const char* _file, uint _line);
#define Matrix4x4_new() _Matrix4x4_new(__FILE__, __LINE__)
#define Matrix4x4_delete(m) _Matrix4x4_delete(m, __FILE__, __LINE__)
extern void Matrix4x4_Dest(Matrix4x4 _mat);
extern void Matrix4x4_set_translate(Matrix4x4 _mat, float x, float y, float z);
extern float3 Matrix4x4_get_translate(Matrix4x4 _mat);
extern void Matrix4x4_set_zero(Matrix4x4 _mat);
extern void Matrix4x4_set_one(Matrix4x4 _mat);
extern void Matrix4x4_export(Matrix4x4 _mat, float* _ret);
extern void Matrix4x4_import(Matrix4x4 _mat, float* _src);
extern void Matrix4x4_mul_matrix4(Matrix4x4 m1, Matrix4x4 m2, Matrix4x4 result);
extern void Matrix4x4_assign(Matrix4x4 _dst, Matrix4x4 _src);
///extern void Matrix4x4_projection(Matrix4x4 _mat, float _left, float _right, float _top, float _bottom, float _near, float _far);
extern void Matrix4x4_projection(Matrix4x4 _mat, float _width, float _height, float _near, float _far);
extern void Matrix4x4_inverse(Matrix4x4 _mat);
extern void Matrix4x4_print(Matrix4x4 _mat);
extern void Matrix4x4_log(Matrix4x4 _mat);
extern void Matrix4x4_turn(Matrix4x4 _mat);
extern float4 Matrix4x4_mul_float4(Matrix4x4 _mat, float4 _ft);
extern void Matrix4x4_set_x_axis_rotate(Matrix4x4 _mat, float _r);
extern void Matrix4x4_set_y_axis_rotate(Matrix4x4 _mat, float _r);
extern void Matrix4x4_set_z_axis_rotate(Matrix4x4 _mat, float _r);
extern void Matrix4x4_from_axis_angle(Matrix4x4 _mat, float3 _axis, float _radian);

extern void Matrix4x4_mul_matrix4_test();

typedef struct matrix4x4_array_
{
    struct _matrix4x4* self;
} Matrix4x4Array;

extern Matrix4x4Array Matrix4x4Array_new();
extern void Matrix4x4Array_delete(Matrix4x4Array _m4_array);
extern void Matrix4x4Array_push(Matrix4x4Array _m4_array, Matrix4x4 _mt4);
extern Matrix4x4 Matrix4x4Array_pop(Matrix4x4Array _m4_array);
extern Matrix4x4 Matrix4x4Array_get(Matrix4x4Array _m4_array, uint32 _i);
extern uint32 Matrix4x4Array_get_size(Matrix4x4Array _m4_array);
extern float* Matrix4x4Array_get_ptr(Matrix4x4Array _m4_array);
extern void Matrix4x4Array_export(Matrix4x4Array _m4_array, float* _ret);
#endif
