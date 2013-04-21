#ifndef RAY_H
#define RAY_H
#include "common.h"
#include "etypes.h"
#include "float_base.h"
struct _ray;
typedef struct _ray* Ray;
typedef struct _ray
{
    EFloat3 origin;
    EFloat3 direction;
} ray;
void FORCE_ALIGN_ARG_POINTER Ray_Init_from_begin_end(Ray self, EFloat3* begin, EFloat3* end);
void Ray_log(const Ray _self);
#endif
