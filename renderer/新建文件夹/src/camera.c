#include "camera.h"
#include "float3.h"
#include "float4.h"
#include "sfloat3.h"
#include "sfloat4.h"
#include "matrix4x4.h"
#include "mem.h"
#include "renderer.h"
#include "line_utility.h"
#include "math_base.h"
typedef struct _camera
{
    float ratio;
    float plane_far;
    float plane_near;
    float width;
    float height;
    uint pixel_width;
    uint pixel_height;
    Matrix4x4 cam_rota_mat;
    Matrix4x4 cam_tran_mat;
    Matrix4x4 cam_proj_mat;
    Matrix4x4 render_mat;
    Matrix4x4 inv_render_mat;
    ///Matrix4x4 inv_cam_rota_mat;
    ///Matrix4x4 inv_cam_tran_mat;

    Matrix4x4 cam_world_mat;
    Matrix4x4 inv_cam_world_mat;

    Matrix4x4 unturned_cam_world_mat;
    Matrix4x4 unturned_inv_cam_world_mat;

    ///Matrix4x4 turned_render_mat;
    Matrix4x4 inv_cam_proj_mat;
    ///Matrix4x4 inv_render_mat;
} camera;

void Camera_Dest(struct _camera* _self)
{

    Matrix4x4_delete(_self->cam_rota_mat);
    Matrix4x4_delete(_self->cam_tran_mat);
    Matrix4x4_delete(_self->cam_proj_mat);
    Matrix4x4_delete(_self->render_mat);
    Matrix4x4_delete(_self->inv_render_mat);

    ///Matrix4x4_delete(_self->inv_cam_rota_mat);
    ///Matrix4x4_delete(_self->inv_cam_tran_mat);

    ///Matrix4x4_delete(_self->turned_render_mat);
    Matrix4x4_delete(_self->inv_cam_proj_mat);
    ///Matrix4x4_delete(_self->inv_render_mat);
    Matrix4x4_delete(_self->cam_world_mat);
    Matrix4x4_delete(_self->inv_cam_world_mat);

    Matrix4x4_delete(_self->unturned_cam_world_mat);
    Matrix4x4_delete(_self->unturned_inv_cam_world_mat);

    Mfree(_self);
}

Camera Camera_Init(struct _camera* _self, uint _width, uint _height)
{
    _self->ratio = (float)_width / (float)_height;
    _self->plane_far = 50.0;
    _self->plane_near = 1.0;
    ///_self->width = (float)_width / (float)g_width;
    ///_self->height = (float)_height / (float)g_width;
    _self->width = 1.0f;
    _self->height = (float)_height / (float)_width;
    ///_self->height = 1.0f;
    ///float r = 3.14 * 0.5;
    ///_self->width *= r;
    ///_self->height *= r;
    _self->pixel_width = _width;
    _self->pixel_height = _height;

    Matrix4x4_set_one(_self->cam_rota_mat);
    ///Matrix4x4_assign(_self->inv_cam_rota_mat, _self->cam_rota_mat);
    ///Matrix4x4_inverse(_self->inv_cam_rota_mat);
    Matrix4x4_set_translate(_self->cam_tran_mat, 0.0, 0.0, 0.0);
    ///Matrix4x4_assign(_self->inv_cam_tran_mat, _self->cam_tran_mat);
    ///Matrix4x4_inverse(_self->inv_cam_tran_mat);

    if (_width && _height)
        Matrix4x4_projection(_self->cam_proj_mat, _self->width, _self->height, _self->plane_near, _self->plane_far);
    else
        Matrix4x4_set_one(_self->cam_proj_mat);

    Camera ret = {_self};
    Camera_updata(ret);

    return ret;
}

Camera Camera_new(uint _width, uint _height)
{
    camera* cam = Malloc(sizeof(camera));
    cam->cam_tran_mat = Matrix4x4_new();
    cam->cam_rota_mat = Matrix4x4_new();
    cam->cam_proj_mat = Matrix4x4_new();
    cam->render_mat = Matrix4x4_new();
    cam->inv_render_mat = Matrix4x4_new();

    ///cam->inv_cam_tran_mat = Matrix4x4_new();
    ///cam->inv_cam_rota_mat = Matrix4x4_new();

    cam->cam_world_mat = Matrix4x4_new();
    cam->inv_cam_world_mat = Matrix4x4_new();

    cam->unturned_cam_world_mat = Matrix4x4_new();
    cam->unturned_inv_cam_world_mat = Matrix4x4_new();

    ///cam->turned_render_mat = Matrix4x4_new();
    cam->inv_cam_proj_mat = Matrix4x4_new();
    ///cam->inv_render_mat = Matrix4x4_new();
    return Camera_Init(cam, _width, _height);
}

void Camera_delete(Camera _self)
{
    Camera_Dest(_self.self);
}

static inline sfloat3 _Camera_get_position(Camera _self)
{
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    return SFloat4_xyz(pos);
}
static inline sfloat3 _Camera_get_direction(Camera _self)
{
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    sfloat4 dir = SFloat4(0.0f, 0.0f, -1.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    dir = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, dir);
    dir = SFloat4_sub(dir, pos);
    dir = SFloat4_normalize(dir);
    return SFloat4_xyz(dir);
}
static inline sfloat3 _Camera_get_right(Camera _self)
{
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    sfloat4 right = SFloat4(1.0f, 0.0f, 0.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    right = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, right);
    right = SFloat4_sub(right, pos);
    right = SFloat4_normalize(right);
    return SFloat4_xyz(right);
}

static inline sfloat3 _Camera_get_up(Camera _self)
{
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    sfloat4 up = SFloat4(0.0f, 1.0f, 0.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    up = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, up);
    up = SFloat4_sub(up, pos);
    up = SFloat4_normalize(up);
    return SFloat4_xyz(up);
}

static inline sfloat3 _Camera_screen_position_to_world_position(Camera _self, float x, float y)
{
     sfloat4 z = SFloat4(0.0f, 0.0f, 0.0f, SELF.plane_near);
     z = Matrix4x4_mul_float4(SELF.inv_cam_proj_mat, z);
     sfloat4 pos = SFloat4(x , y, -SFloat4_get_z(&z), 1.0f);
     pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
     return SFloat4_xyz(pos);
}

static inline sfloat3 _Camera_screen_position_to_world_position_2(Camera _self, float x, float y, float depth)
{
 /**
"    float x = ( TexCoord.x - 0.5 ) * 2.0;\n"
"    float y = ( TexCoord.y - 0.5 ) * 2.0;\n"
"    vec4 pos = vec4(x, y, (Enc.r - 0.5) * 2.0, 1.0);\n"
"    pos = InvProjectionMatrix * pos;\n"
"    pos = vec4(pos.xyz / pos.w, 1.0);\n"
"    pos = InvCameraMatrix * pos;\n"
"    return pos.xyz;\n"
**/
     sfloat4 pos = SFloat4(x, y, depth, 1.0f);
     pos = Matrix4x4_mul_float4(SELF.inv_cam_proj_mat, pos);
     pos = SFloat4_div_float(SFloat4_get_w(&pos), pos);
     pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
     return SFloat4_xyz(pos);
}

void Camera_updata(Camera _self)
{
    ///Matrix4x4_assign(SELF.inv_cam_rota_mat, SELF.cam_rota_mat);
    ///Matrix4x4_inverse(SELF.inv_cam_rota_mat);

    ///Matrix4x4_assign(SELF.inv_cam_tran_mat, SELF.cam_tran_mat);
    ///Matrix4x4_inverse(SELF.inv_cam_tran_mat);

    Matrix4x4_mul_matrix4(SELF.cam_tran_mat, SELF.cam_rota_mat, SELF.cam_world_mat);
    ///Matrix4x4_mul_matrix4(SELF.inv_cam_rota_mat, SELF.inv_cam_tran_mat, SELF.inv_cam_world_mat);
    Matrix4x4_assign(SELF.inv_cam_world_mat, SELF.cam_world_mat);
    Matrix4x4_inverse(SELF.inv_cam_world_mat);

    Matrix4x4_assign(SELF.unturned_cam_world_mat, SELF.cam_world_mat);
    Matrix4x4_assign(SELF.unturned_inv_cam_world_mat, SELF.inv_cam_world_mat);

    Matrix4x4_turn(SELF.cam_world_mat);
    Matrix4x4_turn(SELF.inv_cam_world_mat);

    Matrix4x4_assign(SELF.render_mat, SELF.cam_proj_mat);
    Matrix4x4_assign(SELF.inv_cam_proj_mat, SELF.cam_proj_mat);
    Matrix4x4_inverse(SELF.inv_cam_proj_mat);
    Matrix4x4_assign(SELF.inv_render_mat, SELF.inv_cam_proj_mat);
    Matrix4x4_turn(SELF.render_mat);
    Matrix4x4_turn(SELF.inv_render_mat);
}

void Camera_draw_shape(Camera _self, struct _line_drawer* _drawer)
{
    sfloat3 center = _Camera_screen_position_to_world_position_2(_self, 0.0f, 0.0f, -1.0f);
    sfloat3 pos = _Camera_get_position(_self);
    sfloat3 near_lower_left = _Camera_screen_position_to_world_position_2(_self, -1.0f, -1.0f, -1.0f);
    sfloat3 near_upper_left = _Camera_screen_position_to_world_position_2(_self, -1.0f, 1.0f, -1.0f);
    sfloat3 near_lower_right = _Camera_screen_position_to_world_position_2(_self, 1.0f, -1.0f, -1.0f);
    sfloat3 near_upper_right = _Camera_screen_position_to_world_position_2(_self, 1.0f, 1.0f, -1.0f);

    sfloat3 far_lower_left = _Camera_screen_position_to_world_position_2(_self, -1.0f, -1.0f, 0.0f);
    sfloat3 far_upper_left = _Camera_screen_position_to_world_position_2(_self, -1.0f, 1.0f, 0.0f);
    sfloat3 far_lower_right = _Camera_screen_position_to_world_position_2(_self, 1.0f, -1.0f, 0.0f);
    sfloat3 far_upper_right = _Camera_screen_position_to_world_position_2(_self, 1.0f, 1.0f, 0.0f);

    float far = Camera_get_plane_far(_self);
    float near = Camera_get_plane_near(_self);

    float w = -2.0 * far * near / (far - near) * 10.0;
    /**
    far_lower_left = SFloat3_mul_float(w, far_lower_left);
    far_upper_left = SFloat3_mul_float(w, far_upper_left);
    far_lower_right = SFloat3_mul_float(w, far_lower_right);
    far_upper_right = SFloat3_mul_float(w, far_upper_right);
    **/
    void _draw_red_line(sfloat3 begin, sfloat3 end, float w)
    {
        line_def def;
        EFloat4 color = {1.0f, 0.0f, 0.0f, 1.0f};
        EFloat3 nor = {0.0f, 0.0f, 0.0f};
        def.begin = SFloat3_convert(begin);
        def.begin_color = color;
        def.begin_normal = nor;
        sfloat3 dir = SFloat3_sub(end, begin);
        dir = SFloat3_mul_float(w, dir);
        end = SFloat3_add(dir, end);
        def.end = SFloat3_convert(end);
        def.end_color = color;
        def.end_normal = nor;
        ILnDwr.add_line(_drawer, &def);
    }
    ///_draw_red_line(far_upper_left, far_lower_right);
    ///_draw_red_line(far_lower_left, far_upper_right);
    _draw_red_line(near_lower_left, near_upper_left, 0.0);
    _draw_red_line(near_lower_right, near_upper_right, 0.0);
    _draw_red_line(near_lower_left, near_lower_right, 0.0);
    _draw_red_line(near_upper_left, near_upper_right, 0.0);

    _draw_red_line(far_lower_left, far_upper_left, 0.0);
    _draw_red_line(far_lower_right, far_upper_right, 0.0);
    _draw_red_line(far_lower_left, far_lower_right, 0.0);
    _draw_red_line(far_upper_left, far_upper_right, 0.0);

    _draw_red_line(near_lower_left, far_lower_left, -w);
    _draw_red_line(near_lower_right, far_lower_right, -w);
    _draw_red_line(near_upper_left, far_upper_left, -w);
    _draw_red_line(near_upper_right, far_upper_right, -w);

    _draw_red_line(near_lower_left, pos, 0.0);
    _draw_red_line(near_lower_right, pos, 0.0);
    _draw_red_line(near_upper_left, pos, 0.0);
    _draw_red_line(near_upper_right, pos, 0.0);
/**
    sfloat3 w = SFloat3_sub(near_lower_right, near_lower_left);
    sfloat3 n = SFloat3_sub(center, pos);
    float width = SFloat3_length(w);
    float near = SFloat3_length(n);
    elog("virt w %f real w %f", Camera_get_width(_self), width);
    elog("virt n %f real n %f", Camera_get_plane_near(_self), near);
**/
}

void Camera_set_width(Camera _self, float _width)
{
    SELF.width = _width;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.plane_near, SELF.plane_far);
    Camera_updata(_self);
}

void Camera_set_height(Camera _self, float _height)
{
    SELF.height = _height;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.plane_near, SELF.plane_far);
    Camera_updata(_self);
}

void Camera_set_near(Camera _self, float _near)
{
    SELF.plane_near = _near;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.plane_near, SELF.plane_far);
    Camera_updata(_self);
}

void Camera_set_far(Camera _self, float _far)
{
    SELF.plane_far = _far;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.plane_near, SELF.plane_far);
    Camera_updata(_self);
}

void Camera_set(Camera _self, float _width, float _height, float _near, float _far)
{
    SELF.width = _width;
    SELF.height = _height;
    SELF.plane_near = _near;
    SELF.plane_far = _far;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.plane_near, SELF.plane_far);
    Camera_updata(_self);
}

void Camera_set_proj_matrix(Camera _self, Matrix4x4 _mat)
{
    Matrix4x4_assign(SELF.cam_proj_mat, _mat);
    Camera_updata(_self);
}

void Camera_set_rota_matrix(Camera _self, Matrix4x4 _mat)
{
    Matrix4x4_assign(SELF.cam_rota_mat, _mat);
    Camera_updata(_self);
}

Matrix4x4 Camera_get_render_matrix(Camera _self)
{
    return SELF.render_mat;
}

Matrix4x4 Camera_get_inv_render_matrix(Camera _self)
{
    return SELF.inv_render_mat;
}

Matrix4x4 Camera_get_world_matrix(Camera _self)
{
    return SELF.cam_world_mat;
}

Matrix4x4 Camera_get_inv_world_matrix(Camera _self)
{
    return SELF.inv_cam_world_mat;
}

void Camera_translate(Camera _self, float _x, float _y, float _z)
{
    Matrix4x4 tran_mat = Matrix4x4_new();
    Matrix4x4_set_translate(tran_mat, -_x, -_y, -_z);
    Matrix4x4_mul_matrix4(SELF.cam_tran_mat, tran_mat, SELF.cam_tran_mat);
    Matrix4x4_delete(tran_mat);
    Camera_updata(_self);
}

void Camera_rotate(Camera _self, EFloat3 _axis, float _rad)
{
    sfloat3 axis = SFloat3(_axis.x, _axis.y, _axis.z);
    Matrix4x4 mat = Matrix4x4_new();
    Matrix4x4_from_axis_angle(mat, axis, -_rad);
    Matrix4x4_mul_matrix4(SELF.cam_rota_mat, mat, SELF.cam_rota_mat);
    Matrix4x4_delete(mat);
    Camera_updata(_self);
}

EFloat3 Camera_get_position(Camera _self)
{
    /**
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    return SFloat3_convert(pos);
    **/
    return SFloat3_convert(_Camera_get_position(_self));
}

EFloat3 Camera_get_direction(Camera _self)
{
    /**
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    sfloat4 dir = SFloat4(0.0f, 0.0f, 1.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    dir = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, dir);
    dir = SFloat4_sub(dir, pos);
    dir = SFloat4_normalize(dir);
    return SFloat3_convert(dir);
    **/
    return SFloat3_convert(_Camera_get_direction(_self));
}

void Camera_look_at(Camera _self, EFloat3 _pt)
{
    sfloat3 tgt_dir = SFloat3_assign_from_efloat3(&_pt);
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    tgt_dir = SFloat3_sub(tgt_dir, SFloat4_xyz(pos));
    tgt_dir = SFloat3_normalize(tgt_dir);

    sfloat4 src_dir4 = SFloat4(0.0f, 0.0f, -1.0f, 1.0f);
    src_dir4 = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, src_dir4);
    sfloat3 src_dir = SFloat3_sub(SFloat4_xyz(src_dir4), SFloat4_xyz(pos));
    src_dir = SFloat3_normalize(src_dir);

    sfloat3 axis = SFloat3_cross(src_dir, tgt_dir);
    axis = SFloat3_normalize(axis);

    float dot = SFloat3_dot(src_dir, tgt_dir);
    float rad = acosf(dot);
    if (fabs(rad) > 0.0001f)
        Camera_rotate(_self, SFloat3_convert(axis), rad);
}

void Camera_set_direction(Camera _self, EFloat3 _dir)
{
    sfloat3 tgt = SFloat3_assign_from_efloat3(&_dir);
    sfloat3 pos = SFloat3(0.0f, 0.0f, 0.0f);
    pos = Matrix4x4_mul_float3(SELF.unturned_inv_cam_world_mat, pos);
    tgt = SFloat3_add(tgt, pos);
    Camera_look_at(_self, SFloat3_convert(tgt));
}

void Camera_set_position(Camera _self, EFloat3 _pt)
{
    sfloat3 tgt = SFloat3_assign_from_efloat3(&_pt);
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    EFloat3 disp = SFloat3_convert(SFloat3_sub(tgt, SFloat4_xyz(pos)));
    Camera_translate(_self, disp.x, disp.y, disp.z);
}
/**
Matrix4x4 Camera_get_rota_matrix(Camera _self)
{
    return SELF.inv_cam_rota_mat;
}

Matrix4x4 Camera_get_tran_matrix(Camera _self)
{
    return SELF.inv_cam_tran_mat;
}
**/
float Camera_get_plane_near(Camera _self)
{
    return SELF.plane_near;
}
float Camera_get_plane_far(Camera _self)
{
    return SELF.plane_far;
}
float Camera_get_width(Camera _self)
{
    return SELF.width;
}
float Camera_get_height(Camera _self)
{
    return SELF.height;
}
float Camera_get_hori_angle(Camera _self)
{
    sfloat3 pos = _Camera_get_position(_self);
    sfloat3 near_left = _Camera_screen_position_to_world_position_2(_self, -1.0f, 0.0f, -1.0f);
    sfloat3 near_right = _Camera_screen_position_to_world_position_2(_self, 1.0f, 0.0f, -1.0f);

    sfloat3 vec_left = SFloat3_sub(near_left, pos);
    sfloat3 vec_right = SFloat3_sub(near_right, pos);
    vec_left = SFloat3_normalize(vec_left);
    vec_right = SFloat3_normalize(vec_right);
    float cos_a = SFloat3_dot(vec_left, vec_right);
    return acosf(cos_a);
}
float Camera_get_vert_angle(Camera _self)
{
    sfloat3 pos = _Camera_get_position(_self);
    sfloat3 near_top = _Camera_screen_position_to_world_position_2(_self, 0.0f, 1.0f, -1.0f);
    sfloat3 near_bottom = _Camera_screen_position_to_world_position_2(_self, 0.0f, -1.0f, -1.0f);

    sfloat3 vec_top = SFloat3_sub(near_top, pos);
    sfloat3 vec_bottom = SFloat3_sub(near_bottom, pos);
    vec_top = SFloat3_normalize(vec_top);
    vec_bottom = SFloat3_normalize(vec_bottom);
    float cos_a = SFloat3_dot(vec_top, vec_bottom);
    return acosf(cos_a);
}

void Camera_set_radian(Camera _self, float radian)
{
    float half_radian = 0.5 * radian;
    float n = Camera_get_plane_near(_self);
    float f = Camera_get_plane_far(_self);
    float ratio = tanf(half_radian);
    float w = 2.0 * n / ( (- (f + n) / (f - n)) -1.0 );
    w *= ratio;
    float h = w;
    Matrix4x4 mat = Matrix4x4_new();
    Matrix4x4_projection(mat, w, h, n, f);
    Camera_set_proj_matrix(_self, mat);
    Matrix4x4_delete(mat);
}

void Camera_set_degrees(Camera _self, float degrees)
{
    float radian = DegreesToRadian(degrees);
    Camera_set_radian(_self, radian);
}

void Camera_refresh(Camera _self, uint _width, uint _height)
{
    Camera_Init(_self.self, _width, _height);
}
EFloat3 Camera_screen_position_to_world_position(Camera _self, float x, float y)
{
    ///sfloat4 pos;
    float half_pixel_width = (float)(SELF.pixel_width >> 1);
    float half_pixel_height = (float)(SELF.pixel_height >> 1);
    float pixel_width = (float)(SELF.pixel_width);
    float pixel_height = (float)(SELF.pixel_height);
    float width = SELF.width;
    float height = SELF.height;
    x = (x - half_pixel_width) / pixel_width;
    y = (y - half_pixel_height) / pixel_height;

    x *= -width;
    y *= height;
/**
    sfloat4 z = SFloat4(0.0f, 0.0f, 0.0f, SELF.plane_near);
    z = Matrix4x4_mul_float4(SELF.inv_cam_proj_mat, z);
    pos = SFloat4(x , y, -SFloat4_get_z(&z), 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);

    return SFloat3_convert(SFloat4_xyz(pos));
    **/
    return SFloat3_convert(_Camera_screen_position_to_world_position(_self, x, y));
}
