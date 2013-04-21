#include "pipeline.h"
#include "mem.h"
#include "array.h"
#include "camera.h"
#include "sketch.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "pass_console.h"
#include "float3.h"
typedef struct _renderable
{
    VertexBuffer vtx_buf;
    IndexBuffer idx_buf;
    uint32 tri_cnt;
    StdMaterial material;
} renderable;

static SketchBook* g_lighting_sketch_book_array = NULL;
static LightSlot* g_light_slot_array = NULL;
///=============temporary===============///
///static Pass* g_lighting_pass_array = NULL;
static Pass g_lighting_pass_array[2] = {{NULL}, {NULL}};
static Pass g_display_pass = {NULL};
///=============temporary===============///
static renderable* g_renderable_array = NULL;
static bool g_dirty_flag = true;
uint g_width = 1024;
uint g_height = 768;
RenderablePlane g_render_plane = {NULL};
Matrix4x4 g_proj_matrix = NULL;
Matrix4x4 g_inv_proj_matrix = NULL;

Matrix4x4 g_cam_rota_tran_matrix = NULL;
float g_cam_width = 0.0f;
float g_cam_height = 0.0f;
float g_cam_near_plane = 0.0f;
float g_cam_far_plane = 0.0f;

static Camera g_camera = {NULL};
static EFloat3 g_ambi_color = {0.15f, 0.10f, 0.04f};

void Pipeline_Init()
{
    g_render_plane = RenderablePlane_new();

    SketchBook exec_skh_book = {NULL};
    renderable exec_renderable = {{NULL}, {NULL}};
    LightSlot exec_light_slot = {NULL};

    g_lighting_sketch_book_array = array_new(SketchBook, MAX_SKETCHBOOKS, exec_skh_book);
    g_renderable_array = array_new(renderable, 5, exec_renderable);
    g_light_slot_array = array_new(LightSlot, 5, exec_light_slot);

    g_camera = Camera_new(g_width, g_height);
    Camera_translate(g_camera, 0.0, 0.0, -5.0);
    g_proj_matrix = Camera_get_render_matrix(g_camera);
    g_inv_proj_matrix = Camera_get_inv_render_matrix(g_camera);

    VertexDeclaration pdec = RenderablePlane_get_vertex_declaration(g_render_plane);
    g_lighting_pass_array[0] = create_lighting_pass(pdec, false);
    g_lighting_pass_array[1] = create_lighting_pass(pdec, true);
    g_display_pass = create_display_pass(pdec);
    ///g_display_pass = create_depth_display_pass(pdec);
    ///g_display_pass = create_position_display_pass(pdec);
    ///g_display_pass = create_light_display_pass(pdec);

    g_cam_rota_tran_matrix = Matrix4x4_new();
}

void Pipeline_Dest()
{
    for (uint i = 0; i < array_n(g_lighting_sketch_book_array); i++)
    {
        SketchBook skb = array_safe_get(g_lighting_sketch_book_array, i);
        SketchBook_delete(skb);
    }
    for (uint i = 0; i < array_n(g_light_slot_array); i++)
    {
        LightSlot ls = array_safe_get(g_light_slot_array, i);
        LightSlot_delete(ls);
    }
    for (uint i = 0; i < array_n(g_renderable_array); i++)
    {
        renderable* rb = array_safe_get_ptr(g_renderable_array, i);
        ///VertexBuffer_Dest(to_ptr(rb->vtx_buf));
        ///IndexBuffer_Dest(to_ptr(rb->idx_buf));
        VertexBuffer_delete(rb->vtx_buf);
        IndexBuffer_delete(rb->idx_buf);
        StdMaterial_delete(rb->material);
    }

    ///Matrix4x4_delete(g_proj_matrix);
    array_delete(g_lighting_sketch_book_array);
    array_delete(g_light_slot_array);
    array_delete(g_renderable_array);

    Pass_delete(g_lighting_pass_array[0]);
    Pass_delete(g_lighting_pass_array[1]);
    Pass_delete(g_display_pass);

    Camera_delete(g_camera);

    RenderablePlane_delete(g_render_plane);

    Matrix4x4_delete(g_cam_rota_tran_matrix);
}

PointLight Pipeline_add_point_light()
{
    PointLight ret = PointLight_new();

    uint32 n = array_n(g_light_slot_array);
    LightSlot ls = {NULL};
    if (!n)
    {
        ls = LightSlot_new();
        g_light_slot_array = array_push(g_light_slot_array, ls);
    }
    else
    {
        ls = array_safe_get(g_light_slot_array, n - 1);
        if (LightSlot_get_count(ls) == MAX_LIGHTS_PER_PASS)
        {
            ls = LightSlot_new();
            g_light_slot_array = array_push(g_light_slot_array, ls);
        }
    }
    LightSlot_push_light(ls, to_light(ret));
    return ret;
}

void Pipeline_append_renderable(VertexBuffer _vbf, IndexBuffer _idf, uint32 _tri_cnt, StdMaterial _mat)
{
    renderable rbl = {_vbf, _idf, _tri_cnt, _mat};
    g_renderable_array = array_push(g_renderable_array, rbl);
}

void Pipeline_render()
{
    if (g_dirty_flag)
    {
        /// 销毁素描本
        if (g_lighting_sketch_book_array)
        {
            for (uint32 i = 0; i < array_n(g_lighting_sketch_book_array); i++)
            {
                SketchBook skb = array_safe_get(g_lighting_sketch_book_array, i);
                SketchBook_delete(skb);
            }
            array_delete(g_lighting_sketch_book_array);
            g_lighting_sketch_book_array = NULL;
        }
        /// 建立素描本
        if (g_lighting_sketch_book_array == NULL)
        {
            SketchBook exec_sketch_book = {NULL};
            g_lighting_sketch_book_array = array_new(SketchBook, 5, exec_sketch_book);
        }
        /// 填充素描本
        if (!array_n(g_lighting_sketch_book_array))
        {
            uint32 fill_count = 3;
            for (uint32 i = 0; i < fill_count; i++)
            {
                SketchBook skb = SketchBook_new(g_width, g_height);
                g_lighting_sketch_book_array = array_push(g_lighting_sketch_book_array, skb);
            }
        }
    }

    {
        glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
        // enable /disable features

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glAlphaFunc(GL_GREATER,0.0);
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_DEPTH);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_ALPHA);
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0);
        glDepthFunc(GL_LEQUAL);
    }

    {
        Matrix4x4 rota_mat = Camera_get_rota_matrix(g_camera);
        Matrix4x4 tran_mat = Camera_get_tran_matrix(g_camera);
        Matrix4x4_mul_matrix4(rota_mat, tran_mat, g_cam_rota_tran_matrix);

        g_cam_width = Camera_get_width(g_camera);
        g_cam_height = Camera_get_height(g_camera);
/**
        float mv_mat[16];// get the modelview matrix
        glGetFloatv(GL_MODELVIEW_MATRIX, mv_mat);
        elog("mv");
        elog("%f %f %f %f", mv_mat[0], mv_mat[1], mv_mat[2], mv_mat[3]);
        elog("%f %f %f %f", mv_mat[4], mv_mat[5], mv_mat[6], mv_mat[7]);
        elog("%f %f %f %f", mv_mat[8], mv_mat[9], mv_mat[11], mv_mat[12]);
        elog("%f %f %f %f", mv_mat[13], mv_mat[14], mv_mat[15], mv_mat[16]);
        elog("mv");

        Matrix4x4 mat = Matrix4x4_new();
        Matrix4x4 inv_mat = Matrix4x4_new();
        Matrix4x4_import(mat, mv_mat);
        Matrix4x4_turn(mat);
        Matrix4x4_inverse(mat);
        Matrix4x4_log(mat);
        Matrix4x4_assign(inv_mat, g_proj_matrix);
        Matrix4x4_turn(inv_mat);
        Matrix4x4_inverse(inv_mat);
        Matrix4x4_mul_matrix4(mat, inv_mat, inv_mat);
        Matrix4x4_turn(inv_mat);
        g_inv_proj_matrix = inv_mat;
**/
    }

    SketchBook skb = {NULL};
    Pass std_pass = {NULL};
    skb = array_safe_get(g_lighting_sketch_book_array, 0);

/**
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glOrtho(-1,1,1,-1,-20,20);
    }
    **/
    for (uint32 i = 0; i < array_n(g_renderable_array); i++)
    {
        glEnable(GL_DEPTH_TEST);
        SketchBook_draw_begin(skb, 3);
/**
        float mv_mat[16];// get the modelview matrix
                glGetFloatv(GL_MODELVIEW_MATRIX, mv_mat);
                elog("mv");
                elog("%f %f %f %f", mv_mat[0], mv_mat[1], mv_mat[2], mv_mat[3]);
                elog("%f %f %f %f", mv_mat[4], mv_mat[5], mv_mat[6], mv_mat[7]);
                elog("%f %f %f %f", mv_mat[8], mv_mat[9], mv_mat[11], mv_mat[12]);
                elog("%f %f %f %f", mv_mat[13], mv_mat[14], mv_mat[15], mv_mat[16]);
                elog("mv");
**/
        if (i == 0)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderable rbl = array_safe_get(g_renderable_array, i);

        std_pass = StdMaterial_get_std_pass(rbl.material);

        Texture2D col_tex = StdMaterial_get_color_texture(rbl.material);
        Texture2D nor_tex = StdMaterial_get_normal_texture(rbl.material);

        Pass_set_uniform_param_mat4x4(std_pass, "ProjMatrix", g_proj_matrix);
        if (to_ptr(col_tex))
            Pass_set_uniform_param_tex2d(std_pass, "ColorMap", col_tex);
        if (to_ptr(nor_tex))
            Pass_set_uniform_param_tex2d(std_pass, "NormalMap", nor_tex);

        EFloat3 pos = Camera_get_position(g_camera);
        Pass_set_uniform_param_float3(std_pass, CAMERA_POSITION, pos);

        Pass_render(std_pass, rbl.vtx_buf, rbl.idx_buf, rbl.tri_cnt * 3);

        SketchBook_draw_end(skb);
    }

    SketchBook curt_skb = g_lighting_sketch_book_array[1];
    SketchBook prev_skb = g_lighting_sketch_book_array[2];

    for (uint32 i = 0; i < array_n(g_light_slot_array); i++)
    {
        LightSlot light_slot = array_safe_get(g_light_slot_array, i);
        if (i != 0)
        {
            Pass lighting_pass = g_lighting_pass_array[1];

            SketchBook_draw_begin(curt_skb, 1);
            glDisable(GL_DEPTH_TEST);

            Pass_set_uniform_param_tex2d(lighting_pass, "NormalMap", SketchBook_get_sketch(skb, 1));
            Pass_set_uniform_param_tex2d(lighting_pass, "PositionMap", SketchBook_get_sketch(skb, 2));
            Pass_set_uniform_param_tex2d(lighting_pass, "LightingMap", SketchBook_get_sketch(prev_skb, 0));

            Pass_set_uniform_param_mat4x4(lighting_pass, "InvCamRotaTranProjMatrix", g_inv_proj_matrix);
            Pass_set_uniform_param_float(lighting_pass, "CamWidth", g_cam_width);
            Pass_set_uniform_param_float(lighting_pass, "CamHeight", g_cam_height);
            Pass_set_uniform_param_float(lighting_pass, "CamNearPlane", g_cam_near_plane);
            Pass_set_uniform_param_float(lighting_pass, "CamFarPlane", g_cam_near_plane);

            LightSlot_apply(light_slot, lighting_pass);
            Pass_render_plane(lighting_pass, g_render_plane);

            SketchBook_draw_end(curt_skb);
        }
        else
        {
            Pass lighting_pass = g_lighting_pass_array[0];

            SketchBook_draw_begin(curt_skb, 1);
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Pass_set_uniform_param_tex2d(lighting_pass, "NormalMap", SketchBook_get_sketch(skb, 1));
            Pass_set_uniform_param_tex2d(lighting_pass, "PositionMap", SketchBook_get_sketch(skb, 2));

            Pass_set_uniform_param_mat4x4(lighting_pass, "InvCamRotaTranProjMatrix", g_inv_proj_matrix);
            Pass_set_uniform_param_float(lighting_pass, "CamWidth", g_cam_width);
            Pass_set_uniform_param_float(lighting_pass, "CamHeight", g_cam_height);
            Pass_set_uniform_param_float(lighting_pass, "CamNearPlane", g_cam_near_plane);
            Pass_set_uniform_param_float(lighting_pass, "CamFarPlane", g_cam_near_plane);

            LightSlot_apply(light_slot, lighting_pass);
            Pass_render_plane(lighting_pass, g_render_plane);

            SketchBook_draw_end(curt_skb);
        }

        SketchBook tmp = prev_skb;
        prev_skb = curt_skb;
        curt_skb = tmp;
    }

    glDisable(GL_DEPTH_TEST);
    Pass_set_uniform_param_tex2d(g_display_pass, "ColorMap", SketchBook_get_sketch(skb, 0));
    Pass_set_uniform_param_tex2d(g_display_pass, "LightingMap", SketchBook_get_sketch(prev_skb, 0));
    Pass_set_uniform_param_tex2d(g_display_pass, "NormalMap", SketchBook_get_sketch(skb, 1));
    Pass_set_uniform_param_tex2d(g_display_pass, "PositionMap", SketchBook_get_sketch(skb, 2));
    Pass_set_uniform_param_float3(g_display_pass, "AmbiColor", g_ambi_color);
    Pass_render_plane(g_display_pass, g_render_plane);

    g_dirty_flag = false;
}

Camera Pipeline_get_camera()
{
    return g_camera;
}
