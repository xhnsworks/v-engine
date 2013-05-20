#ifndef PLANE_H
#define PLANE_H
#include "common.h"
#include "etypes.h"
#include "float_base.h"
#include "sfloat3.h"
#include "ray.h"
struct _plane;
typedef struct _plane* Plane;

typedef struct _plane
{
    EFloat3 origin;
    EFloat3 normal;
} plane;

struct GLPlane
{
	double x;
	double y;
	double z;
	double w;
    GLPlane(const EFloat3& nor, float w)
		: x(nor.x)
		, y(nor.y)
		, z(nor.z)
		, w(w)
	{}
	GLPlane(const sfloat3& nor, float w)
		: x(SFloat3_get_x(&nor))
		, y(SFloat3_get_y(&nor))
		, z(SFloat3_get_z(&nor))
		, w(w)
	{}
	inline double* GetPointer() {
        return &x;
	}
};

API_EXPORT Plane FORCE_ALIGN_ARG_POINTER Plane_Init_from_triangle(Plane self, EFloat3* pos0, EFloat3* pos1, EFloat3* pos2);
API_EXPORT EFloat3 FORCE_ALIGN_ARG_POINTER Plane_ray_cross(Plane self, Ray ray);
API_EXPORT GLPlane FORCE_ALIGN_ARG_POINTER Plane_to_glplane(Plane self);
#endif
