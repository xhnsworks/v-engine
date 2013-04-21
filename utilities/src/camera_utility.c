#include "camera_utility.h"
#include "mem.h"
#include "sfloat3.h"
#include "sfloat4.h"
typedef struct _camera_utility
{
    Camera cam;
    float dolly_speed;
    float pan_speed;
    float rotate_speed;
} camera_utility;

CameraUtility CameraUtility_new(Camera cam)
{
    CameraUtility ret = Malloc(sizeof(camera_utility));
    ret->cam = cam;
    ret->dolly_speed = 0.01f;
    ret->pan_speed = 0.01f;
    ret->rotate_speed = 0.01f;
    return ret;
}

void CameraUtility_delete(CameraUtility self)
{
    Mfree(self);
}

void CameraUtility_dolly_shot(CameraUtility self, float dist)
{
    EFloat3 dir = Camera_get_direction(self->cam);
    sfloat3 s_dir = SFloat3_assign_from_efloat3(&dir);
    s_dir = SFloat3_mul_float(dist * self->dolly_speed, s_dir);
    Camera_translate(self->cam, SFloat3_get_x(&s_dir), SFloat3_get_y(&s_dir), SFloat3_get_z(&s_dir));
}

void CameraUtility_pan(CameraUtility self, float x, float y)
{
    EFloat3 dir = Camera_get_direction(self->cam);
    sfloat4 s_dir = SFloat4_assign(SFloat3_assign_from_efloat3(&dir));
    SFloat4_set_y(&s_dir, 0.0f);
    s_dir = SFloat4_normalize(s_dir);
    Matrix4x4 m = Matrix4x4_new();
    Matrix4x4_set_right_y_axis(m);
    sfloat3 right = SFloat4_xyz(Matrix4x4_mul_float4(m, s_dir));
    Matrix4x4_delete(m);

    sfloat3 up = SFloat3(0.0f, 1.0f, 0.0f);
    right = SFloat3_mul_float(x * self->pan_speed, right);
    up = SFloat3_mul_float(y * self->pan_speed, up);
    sfloat3 disp = SFloat3_add(right, up);
    Camera_translate(self->cam, SFloat3_get_x(&disp), SFloat3_get_y(&disp), SFloat3_get_z(&disp));
}

void CameraUtility_rotate(CameraUtility self, float x, float y)
{
    EFloat3 y_axis = {0.0f, 1.0f, 0.0f};

    EFloat3 dir = Camera_get_direction(self->cam);
    sfloat4 s_dir = SFloat4_assign(SFloat3_assign_from_efloat3(&dir));
    SFloat4_set_y(&s_dir, 0.0f);
    s_dir = SFloat4_normalize(s_dir);
    Matrix4x4 m = Matrix4x4_new();
    Matrix4x4_set_right_y_axis(m);
    sfloat3 right = SFloat4_xyz(Matrix4x4_mul_float4(m, s_dir));
    Matrix4x4_delete(m);

    EFloat3 x_axis = SFloat3_convert(right);
    Camera_rotate(self->cam, x_axis, y * self->rotate_speed);
    Camera_rotate(self->cam, y_axis, x * self->rotate_speed);
}
