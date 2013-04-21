#ifndef CONE_H
#define CONE_H
/**
#include "Vector4f.h"
#include "matrix4x4*.h"
#include "geometry_types.h"

class Cone4f
{
public:
    Vector4f origin;
    Vector4f direction;
    float angle;
    float radian;
    float radian_dot;

    inline Cone4f()
    {
        angle = 30.0;
        radian = pi * angle / 180.0;
        radian_dot = cos(radian);
    }

    inline Cone4f(float a)
    {
        angle = a;
        radian = pi * angle / 180.0;
        radian_dot = cos(radian);
    }

    inline Cone4f(Vector4f& o, Vector4f& d, float a)
    {
        origin = o;
        direction = d;
        angle = a;
        radian = pi * angle / 180.0;
        radian_dot = cos(radian);
    }

    inline void setOrigin(float x, float y, float z)
    {
        origin.setr(x, y, z, 1.0);
    }

    inline void setDirection(float x, float y, float z)
    {
        direction.setr(x, y, z, 1.0);
        direction.normalize();
    }

    inline void setAngle(float a)
    {
        angle = a;
        radian = pi * angle / 180.0;
        radian_dot = cos(radian);
    }

    bool rayConeCross(Ray4f& r);
    int rayConeCross(Ray4f& r, Vector4f* p0, Vector4f* p1);
    int rayConeCross(Ray4f& r, float* x0, float* x1);
    bool lineConeCross(Line4f& l);
    int lineConeCross(Line4f& l, Vector4f* p0, Vector4f* p1);
    bool lineConeCrossAsSegment(Line4f& l);
    bool lineConeCrossAsSegment(Line4f_Ref& l);
    bool coneCubeCross(Cube4f& cube);
    bool coneTriangleCross(Triangle4f& tri);
    bool coneTriangleCross(Triangle4f_Ref& tri);
};
**/
#include "common.h"
#include "etypes.h"
#include "sfloat3.h"
#include "ray.h"
#include "math_base.h"
#include <math.h>
class Cone
{
public:
	sfloat3 ori;
	sfloat3 dir;
	float angle;
    float radian;
    float radian_dot;
	inline Cone(sfloat3& o, sfloat3& d, float a)
    {
        ori = o;
        dir = d;
        angle = a;
        radian = E_PI * angle / 180.0f;
        radian_dot = cosf(radian);
    }
	inline Cone(float a)
    {
        angle = a;
        radian = E_PI * angle / 180.0f;
        radian_dot = cosf(radian);
		ori = SFloat3(0.0f, 0.0f, 0.0f);
		dir = SFloat3(0.0f, 0.0f, 1.0f);
    }
	int intersect(Ray _ray, sfloat3* p0, sfloat3* p1);
};
#endif
