#ifndef FLOAT3_H
#define FLOAT3_H
#include <float_base.h>

extern float3 Float3(float _x, float _y, float _z);
extern float3 Float3_assign(float2 _f2);
extern float Float3_dot(float3 _f3_0, float3 _f3_1);
extern float3 Float3_cross(float3 _f3_0, float3 _f3_1);
extern float3 Float3_sub(float3 _f3_0, float3 _f3_1);
extern float3 Float3_mul_float(float _ft, float3 _f3);
extern float3 Float3_normalize(float3 _f3);
extern float Float3_length(float3 _f3);
extern void Float3_export(float3 _f3, float* _fout);
extern EFloat3 Float3_convert(float3 _f3);

extern void Float3_log(float3 _f3);
#endif
