#include "plane.h"
#include "float_base.h"
#include "sfloat3.h"
#include "array.h"
#include "math.h"
Plane Plane_Init_from_triangle(Plane self, EFloat3* pos0, EFloat3* pos1, EFloat3* pos2)
{
    sfloat3 vtmp0 = SFloat3_assign_from_efloat3(pos0);
    sfloat3 vtmp1 = SFloat3_assign_from_efloat3(pos1);
    sfloat3 vtmp2 = SFloat3_assign_from_efloat3(pos2);
    vtmp1 = SFloat3_sub(vtmp1, vtmp0);
    vtmp2 = SFloat3_sub(vtmp2, vtmp0);
    ///self->normal = SFloat3_convert(SFloat3_cross(vtmp1, vtmp2));
    sfloat3 normal = SFloat3_cross(vtmp1, vtmp2);
    normal = SFloat3_normalize(normal);
    self->normal = SFloat3_convert(normal);
    self->origin = *pos0;
	return self;
}

void FORCE_ALIGN_ARG_POINTER _ray_plane_cross_base(sfloat3* r_dir, sfloat3* r_ori, sfloat3* p_o, sfloat3* p_n, sfloat3* cross_point)
{
    sfloat3 temp3 = SFloat3_sub(*p_o, *r_ori);
    temp3 = SFloat3_mul(temp3, *p_n);

    float a, b;
    a = SFloat3_dot(temp3, *p_n);

    sfloat3 temp1 = SFloat3(a, a, a);
    b = SFloat3_dot(*r_dir, *p_n);
    sfloat3 temp2 = SFloat3(b, b, b);
    temp1 = SFloat3_div(temp1, temp2);
    temp1 = SFloat3_mul(temp1, *r_dir);
    *cross_point = SFloat3_add(*r_ori, temp1);
}

bool FORCE_ALIGN_ARG_POINTER _ray_plane_cross_base2(sfloat3* r_dir, sfloat3* r_ori, sfloat3* p_o, sfloat3* p_n, sfloat3* cross_point)
{
    float d = -SFloat3_dot( *r_dir, *p_n );
    if (fabsf(d) < 0.0001f)
    {
        return false;
    }
    sfloat3 ray_ori_vec_in_plane_space   = SFloat3_sub( *r_ori, *p_o );
    sfloat3 ray_ori_proj_to_plane_normal = SFloat3_dot_sfloat3( ray_ori_vec_in_plane_space, *p_n );

    sfloat3 cos_a     = SFloat3_assign_from_float( d );
    sfloat3 cross_len = SFloat3_div( ray_ori_proj_to_plane_normal, cos_a );

    sfloat3 c_pt = SFloat3_mul( cross_len, *r_dir );
    c_pt         = SFloat3_add( c_pt, *r_ori );

    *cross_point = c_pt;
    return true;
}

EFloat3 Plane_ray_cross(Plane self, Ray ray)
{
    sfloat3 r_dir = SFloat3_assign_from_efloat3(&ray->direction);
    sfloat3 r_ori = SFloat3_assign_from_efloat3(&ray->origin);
    sfloat3 p_o = SFloat3_assign_from_efloat3(&self->origin);
    sfloat3 p_n = SFloat3_assign_from_efloat3(&self->normal);
    sfloat3 cross_point;
    if (_ray_plane_cross_base2(&r_dir, &r_ori, &p_o, &p_n, &cross_point))
        return SFloat3_convert(cross_point);
    else {
        EFloat3 ret(fexce, 0.0f, 0.0f);
        return ret;
    }
}

GLPlane Plane_to_glplane(Plane self)
{
    sfloat3 nor = SFloat3_assign_from_efloat3(&self->normal);
	sfloat3 pos = SFloat3_assign_from_efloat3(&self->origin);
	float d = - SFloat3_dot(nor, pos);
	return GLPlane(self->normal, d);
}