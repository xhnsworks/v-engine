#ifndef FLOAT2_H
#define FLOAT2_H
#include "float_base.h"

API_EXPORT float2 Float2(float _x, float _y);
API_EXPORT float Float2_dot(float2 _f2_0, float2 _f2_1);
API_EXPORT float2 Float2_cross(float2 _f2_0, float2 _f2_1);
API_EXPORT EFloat2 Float2_convert(float2 _f2);
#endif
