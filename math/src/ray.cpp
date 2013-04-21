#include "ray.h"
#include "sfloat3.h"
void Ray_Init_from_begin_end(Ray self, EFloat3* begin, EFloat3* end)
{
    sfloat3 ori = SFloat3_assign_from_efloat3(begin);
    sfloat3 dir = SFloat3_assign_from_efloat3(end);
    dir = SFloat3_sub(dir, ori);
    dir = SFloat3_normalize(dir);
    self->origin = *begin;
    self->direction = SFloat3_convert(dir);
}

void Ray_log(const Ray _self)
{
    elog("##Ray");
    elog("##ori %f, %f, %f", _self->origin.x, _self->origin.y, _self->origin.z);
    elog("##dir %f, %f, %f", _self->direction.x, _self->direction.y, _self->direction.z);
}
