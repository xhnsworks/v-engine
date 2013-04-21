#include "triangle.h"
#include "plane.h"
#include "matrix4x4.h"
#include "sfloat3.h"
#include "sfloat4.h"
#include "float_base.h"
#include "math_base.h"

bool Triangle_point_is_in_triangle(Triangle self, EFloat3* point)
{
    sfloat4 v0 = SFloat4(self->vertex0.x, self->vertex0.y, self->vertex0.z, 1.0f);
    sfloat4 v1 = SFloat4(self->vertex1.x, self->vertex1.y, self->vertex1.z, 1.0f);
    sfloat4 v2 = SFloat4(self->vertex2.x, self->vertex2.y, self->vertex2.z, 1.0f);
    sfloat4 sp = SFloat4(point->x, point->y, point->z, 1.0f);

    plane pn;
    Plane_Init_from_triangle(&pn, &self->vertex0, &self->vertex1, &self->vertex2);
    matrix4x4* m = Matrix4x4_new();
    Matrix4x4_from_axis_angle(m, SFloat3_assign_from_efloat3(&pn.normal), -E_PI * 0.5f);

    sfloat4 wall_nor0 = SFloat4_sub(v1, v0);
    sfloat4 wall_nor1 = SFloat4_sub(v2, v1);
    sfloat4 wall_nor2 = SFloat4_sub(v0, v2);

    sfloat3 vec0 = SFloat4_xyz(SFloat4_sub(sp, v0));
    sfloat3 vec1 = SFloat4_xyz(SFloat4_sub(sp, v1));
    sfloat3 vec2 = SFloat4_xyz(SFloat4_sub(sp, v2));

    wall_nor0 = SFloat4_normalize(wall_nor0);
    wall_nor1 = SFloat4_normalize(wall_nor1);
    wall_nor2 = SFloat4_normalize(wall_nor2);

    wall_nor0 = Matrix4x4_mul_float4(m, wall_nor0);
    wall_nor1 = Matrix4x4_mul_float4(m, wall_nor1);
    wall_nor2 = Matrix4x4_mul_float4(m, wall_nor2);

    Matrix4x4_delete(m);

    float d0 = SFloat3_dot(vec0, SFloat4_xyz(wall_nor0));
    float d1 = SFloat3_dot(vec1, SFloat4_xyz(wall_nor1));
    float d2 = SFloat3_dot(vec2, SFloat4_xyz(wall_nor2));

    ///return _is_in_triangle(&v0, &v1, &v2, &sp);
    return d0 < 0.0f && d1 < 0.0f && d2 < 0.0f;
}
