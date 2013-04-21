#include "cone.h"

/**
#include "Cone4f.h"
#include "Cube4f.h"
#include "Line4f.h"
#include "Plane4f.h"
#include "Ray4f.h"
#include "Triangle4f.h"
bool Cone4f::rayConeCross(Ray4f& r)
{
    //ALIGN_Vector4f(ro_co);
    ALIGN_Value(Vector4f, ro_co);
    ro_co = r.origin - origin;
    float ft0 = r.direction * direction;
    float ft1 = ro_co * direction;
    float ft2 = direction * direction;
    float ft3 = r.direction * ro_co;
    float ft4 = ro_co * ro_co;

    float a, b, c;
    a = radian_dot*radian_dot*ft2 - ft0*ft0;
    b = 2.0 * (radian_dot*radian_dot*ft3 - ft0*ft1);
    c = radian_dot*radian_dot*ft4 - ft1*ft1;

    float fx = b * b - 4.0 * a * c;
    if (fx >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
**/
/**
int Cone4f::rayConeCross(Ray4f& r, Vector4f* p0, Vector4f* p1)
{
    //ALIGN_Vector4f(ro_co);
    ALIGN_Value3(Vector4f, ro_co, c0, c1);
    ro_co = r.origin - origin;
    float ft0 = r.direction * direction;
    float ft1 = ro_co * direction;
    float ft2 = direction * direction;
    float ft3 = r.direction * ro_co;
    float ft4 = ro_co * ro_co;

    float a, b, c;
    a = radian_dot*radian_dot*ft2 - ft0*ft0;
    b = 2.0 * (radian_dot*radian_dot*ft3 - ft0*ft1);
    c = radian_dot*radian_dot*ft4 - ft1*ft1;

    float fx = b * b - 4.0 * a * c;
    if (fx >= 0)
    {
        float x0, x1;

        x0 = (-b + sqrt(fx)) / (2.0 * a);
        x1 = (-b - sqrt(fx)) / (2.0 * a);

        c0 = r.direction.mul(x0);
        c0 = c0 + r.origin;
        c0 = c0 - origin;

        c1 = r.direction.mul(x1);
        c1 = c1 + r.origin;
        c1 = c1 - origin;
        float d0 = c0 * r.direction;
        float d1 = c1 * r.direction;
        if (d0 > 0 && d1 > 0)
        {
            *p0 = c0;
            *p1 = c1;
            return 2;
        }
        else if (d0 > 0 && d1 <= 0)
        {
            *p0 = c0;
            return 1;
        }
        else if (d0 <= 0 && d1 > 0)
        {
            *p0 = c1;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (fx == 0)
    {
        float x;
        x = (-b) / (2.0 * a);

        c0 = r.direction.mul(x);
        c0 = c0 + r.origin;
        c0 = c0 - origin;
        float d = c0 * r.direction;
        if (d > 0)
        {
            *p0 = c0;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
**/
int Cone::intersect(Ray _ray, sfloat3* p0, sfloat3* p1)
{
	///ro_co = r.origin - origin;
    ///float ft0 = r.direction * direction;
    ///float ft1 = ro_co * direction;
    ///float ft2 = direction * direction;
    ///float ft3 = r.direction * ro_co;
    ///float ft4 = ro_co * ro_co;

	sfloat3 r_ori = SFloat3_assign_from_efloat3(&_ray->origin);
	sfloat3 r_dir = SFloat3_assign_from_efloat3(&_ray->direction);
	sfloat3 ro_co = SFloat3_sub(r_ori, ori);

	float ft0 = SFloat3_dot(r_dir, dir);
    float ft1 = SFloat3_dot(ro_co, dir);
    float ft2 = SFloat3_dot(dir, dir);
    float ft3 = SFloat3_dot(r_dir, ro_co);
    float ft4 = SFloat3_dot(ro_co, ro_co);

	float a, b, c;
    a = radian_dot * radian_dot * ft2 - ft0 * ft0;
    b = 2.0f * (radian_dot* radian_dot * ft3 - ft0 * ft1);
    c = radian_dot * radian_dot * ft4 - ft1 * ft1;

    float fx = b * b - 4.0f * a * c;
    if (fx >= 0)
    {
        float x0, x1;

        x0 = (-b + sqrtf(fx)) / (2.0f * a);
        x1 = (-b - sqrtf(fx)) / (2.0f * a);

		///c0 = r.direction.mul(x0);
        ///c0 = c0 + r.origin;
        ///c0 = c0 - origin;

        ///c1 = r.direction.mul(x1);
        ///c1 = c1 + r.origin;
        ///c1 = c1 - origin;
        ///float d0 = c0 * r.direction;
        ///float d1 = c1 * r.direction;

		sfloat3 c0 = SFloat3_mul_float(x0, r_dir);
        c0 = SFloat3_add(c0, r_ori);
        sfloat3 cc0 = SFloat3_sub(c0, ori);

        sfloat3 c1 = SFloat3_mul_float(x1, r_dir);
        c1 = SFloat3_add(c1, r_ori);
        sfloat3 cc1 = SFloat3_sub(c1, ori);
        float d0 = SFloat3_dot(cc0, dir);
        float d1 = SFloat3_dot(cc1, dir);
        if (d0 > 0 && d1 > 0)
        {
            *p0 = c0;
            *p1 = c1;
            return 2;
        }
        else if (d0 > 0 && d1 <= 0)
        {
            *p0 = c0;
            return 1;
        }
        else if (d0 <= 0 && d1 > 0)
        {
            *p0 = c1;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (fx == 0)
    {
        float x;
        x = (-b) / (2.0f * a);

		///c0 = r.direction.mul(x);
        ///c0 = c0 + r.origin;
        ///c0 = c0 - origin;
        ///float d = c0 * r.direction;

        sfloat3 c0 = SFloat3_mul_float(x, r_dir);
        c0 = SFloat3_add(c0, r_ori);
        c0 = SFloat3_sub(c0, ori);
        float d = SFloat3_dot(c0, r_dir);
        if (d > 0)
        {
            *p0 = c0;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
