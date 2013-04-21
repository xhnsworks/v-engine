#ifndef PLANE_H
#define PLANE_H
#include "common.h"
#include "etypes.h"
#include "float_base.h"
#include "ray.h"
struct _plane;
typedef struct _plane* Plane;

typedef struct _plane
{
    EFloat3 origin;
    EFloat3 normal;
} plane;

API_EXPORT Plane FORCE_ALIGN_ARG_POINTER Plane_Init_from_triangle(Plane self, EFloat3* pos0, EFloat3* pos1, EFloat3* pos2);
API_EXPORT EFloat3 FORCE_ALIGN_ARG_POINTER Plane_ray_cross(Plane self, Ray ray);
#endif
