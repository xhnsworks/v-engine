#ifndef FLOAT4_H
#define FLOAT4_H
#include <float_base.h>

API_EXPORT float4 Float4(float _x, float _y, float _z, float _w);
API_EXPORT float4 Float4_assign(float3 _f3);
API_EXPORT float Float4_dot(float4 _f4_0, float4 _f4_1);
API_EXPORT float4 Float4_cross(float4 _f4_0, float4 _f4_1);
API_EXPORT float4 Float4_div_float(float _ft, float4 _f4);
API_EXPORT void Float4_print(float4 _f4);
API_EXPORT void Float4_log(float4 _f4);
API_EXPORT void Float4_export(float4 _f4, float* _fout);
API_EXPORT float3 Float4_xyz(float4 _f4);

API_EXPORT EFloat4 Float4_convert(float4 _f4);
#endif
