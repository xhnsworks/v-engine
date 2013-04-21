/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：camera.h
* 文件描述：摄像机对象
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
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
struct _line_drawer;
API_EXPORT Camera Camera_new(uint _width, uint _height);
API_EXPORT void Camera_delete(Camera _self);
API_EXPORT void Camera_updata(Camera _self);
API_EXPORT void Camera_draw_shape(Camera _self, struct _line_drawer* _drawer);

API_EXPORT void Camera_set_proj_matrix(Camera _self, matrix4x4* _mat);
API_EXPORT void Camera_set(Camera _self, bool _is_orthogonal, float _width, float _height, float _near, float _far);
API_EXPORT void Camera_set_proj_scale(Camera _self, float x_scale, float y_scale);

API_EXPORT matrix4x4* Camera_get_render_matrix(Camera _self);
API_EXPORT matrix4x4* Camera_get_inv_render_matrix(Camera _self);
API_EXPORT matrix4x4* Camera_get_world_matrix(Camera _self);
API_EXPORT matrix4x4* Camera_get_inv_world_matrix(Camera _self);
///API_EXPORT matrix4x4* Camera_get_turned_render_matrix(Camera _self);
///API_EXPORT matrix4x4* Camera_get_inv_render_matrix(Camera _self);
API_EXPORT void Camera_translate(Camera _self, float _x, float _y, float _z);
API_EXPORT void Camera_rotate(Camera _self, EFloat3 _axis, float _rad);
API_EXPORT EFloat3 Camera_get_position(Camera _self);
API_EXPORT EFloat3 Camera_get_direction(Camera _self);
API_EXPORT void Camera_look_at(Camera _self, EFloat3 _pt);
API_EXPORT void Camera_set_direction(Camera _self, EFloat3 _dir);
API_EXPORT void Camera_set_position(Camera _self, EFloat3 _pt);
///API_EXPORT matrix4x4* Camera_get_rota_matrix(Camera _self);
///API_EXPORT matrix4x4* Camera_get_tran_matrix(Camera _self);
API_EXPORT float Camera_get_plane_near(Camera _self);
API_EXPORT float Camera_get_plane_far(Camera _self);
API_EXPORT float Camera_get_width(Camera _self);
API_EXPORT float Camera_get_height(Camera _self);

API_EXPORT float Camera_get_hori_angle(Camera _self);
API_EXPORT float Camera_get_vert_angle(Camera _self);
API_EXPORT void Camera_set_FOV(Camera _self, float degrees);
API_EXPORT void Camera_set_radian(Camera _self, float radian);

API_EXPORT bool Camera_is_orthogonal(Camera _self);

///API_EXPORT void Camera_get_width_height(Camera _self, float hori_angle, float vert_angle);

API_EXPORT void Camera_refresh(Camera _self, uint _width, uint _height);

API_EXPORT EFloat3 FORCE_ALIGN_ARG_POINTER Camera_screen_position_to_world_position(Camera _self, float x, float y);
#endif
