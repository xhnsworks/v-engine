#include "plane.h"
#include "float_base.h"
#include "sfloat3.h"

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
}

void __attribute__ ((force_align_arg_pointer)) _plane_ray_plane_cross_base(sfloat3* r_dir, sfloat3* r_ori, sfloat3* p_o, sfloat3* p_n, sfloat3* cross_point)
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

EFloat3 Plane_ray_cross(Plane self, Ray ray)
{
    sfloat3 r_dir = SFloat3_assign_from_efloat3(&ray->direction);
    sfloat3 r_ori = SFloat3_assign_from_efloat3(&ray->origin);
    sfloat3 p_o = SFloat3_assign_from_efloat3(&self->origin);
    sfloat3 p_n = SFloat3_assign_from_efloat3(&self->normal);
    sfloat3 cross_point;
    _plane_ray_plane_cross_base(&r_dir, &r_ori, &p_o, &p_n, &cross_point);
    return SFloat3_convert(cross_point);
}
