#ifndef CAMERA_H
#define CAMERA_H
#include "common.h"
#include "etypes.h"
#include "matrix4x4.h"
struct _camera;
typedef struct camera_
{
    struct _camera* self;
} Camera;
extern Camera Camera_new(uint _width, uint _height);
extern void Camera_delete(Camera _self);
extern void Camera_updata(Camera _self);
extern Matrix4x4 Camera_get_render_matrix(Camera _self);
extern Matrix4x4 Camera_get_inv_render_matrix(Camera _self);
extern void Camera_translate(Camera _self, float _x, float _y, float _z);
extern void Camera_rotate(Camera _self, EFloat3 _axis, float _rad);
extern EFloat3 Camera_get_position(Camera _self);
extern EFloat3 Camera_get_direction(Camera _self);
extern Matrix4x4 Camera_get_rota_matrix(Camera _self);
extern Matrix4x4 Camera_get_tran_matrix(Camera _self);
extern float Camera_get_near_plane(Camera _self);
extern float Camera_get_far_plane(Camera _self);
extern float Camera_get_width(Camera _self);
extern float Camera_get_height(Camera _self);
#endif
