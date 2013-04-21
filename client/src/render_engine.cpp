extern "C" {
#include <GL/glew.h>
#include <GL/gl.h>
#include "mem.h"
#include "elog.h"
#include "renderer.h"
#include "render_system.h"
#include "sfloat4.h"
#include "sfloat3.h"
#include "camera_utility.h"
#include "plane.h"
#include "triangle.h"
#include "shader_buffer.h"
#include "input_system.h"
}

static Ray g_mouse_ray = NULL;
CameraUtility g_camera_util = NULL;

static int g_mouse_x = 0;
static int g_mouse_y = 0;

bool g_left_alt_key_down = false;

bool g_left_button_down = false;
bool g_right_button_down = false;
bool g_middle_button_down = false;

void default_mouse_input_proc(const input_event event)
{
    if (event.type == MouseMoveEvent)
    {
        g_mouse_x += event.info.mouse_info.mouse_move_info.x;
        g_mouse_y += event.info.mouse_info.mouse_move_info.y;

        SetCursorPos(g_mouse_x, g_mouse_y);

        if (g_left_alt_key_down && g_right_button_down)
        {
            sfloat3 vec = SFloat3( (float)event.info.mouse_info.mouse_move_info.x,
                                   (float)event.info.mouse_info.mouse_move_info.y,
                                   0.0f );
            float dist = SFloat3_length(vec);
            if (event.info.mouse_info.mouse_move_info.x < 0)
                dist = -dist;
            CameraUtility_dolly_shot(g_camera_util, dist);
        }
        else if (g_left_alt_key_down && g_middle_button_down)
        {
            CameraUtility_pan(g_camera_util, (float)event.info.mouse_info.mouse_move_info.x, (float)event.info.mouse_info.mouse_move_info.y);
        }
        else if (g_left_alt_key_down && g_left_button_down)
        {
            CameraUtility_rotate(g_camera_util, (float)-event.info.mouse_info.mouse_move_info.x, 0.0f);
        }
    }
    else if (event.type == MouseButtonDownEvent)
    {
        if (event.info.mouse_info.mouse_button_info == LeftButton)
            g_left_button_down = true;
        else if (event.info.mouse_info.mouse_button_info == RightButton)
            g_right_button_down = true;
        else if (event.info.mouse_info.mouse_button_info == MiddleButton)
            g_middle_button_down = true;
    }
    else if (event.type == MouseButtonUpEvent)
    {
        if (event.info.mouse_info.mouse_button_info == LeftButton)
            g_left_button_down = false;
        else if (event.info.mouse_info.mouse_button_info == RightButton)
            g_right_button_down = false;
        else if (event.info.mouse_info.mouse_button_info == MiddleButton)
            g_middle_button_down = false;
    }
}

void default_keyboard_input_proc(const input_event event)
{
    if (event.type == KeyDownEvent)
    {
        printf("key down %d\n", event.info.key_info);
        if (event.info.key_info == 56)
        {
            g_left_alt_key_down = true;
        }
    }
    else if (event.type == KeyUpEvent)
    {
        printf("key up %d\n", event.info.key_info);
        if (event.info.key_info == 56)
        {
            g_left_alt_key_down = false;
        }
    }
}

renderable_proc_result default_renderable_proc(Matrix4x4 mat, float* pos0, float* pos1, float* pos2)
{
    sfloat4 p0 = SFloat4(pos0[0], pos0[1], pos0[2], 1.0f);
    sfloat4 p1 = SFloat4(pos1[0], pos1[1], pos1[2], 1.0f);
    sfloat4 p2 = SFloat4(pos2[0], pos2[1], pos2[2], 1.0f);
    Matrix4x4 inv_mat = Matrix4x4_new();
    Matrix4x4_assign(inv_mat, mat);
    Matrix4x4_inverse(inv_mat);

    p0 = Matrix4x4_mul_float4(inv_mat, p0);
    p1 = Matrix4x4_mul_float4(inv_mat, p1);
    p2 = Matrix4x4_mul_float4(inv_mat, p2);

    Matrix4x4_delete(inv_mat);
    plane pn;
    Plane_Init_from_triangle(&pn, (EFloat3*)&p0, (EFloat3*)&p1, (EFloat3*)&p2);
    EFloat3 cross_point = Plane_ray_cross(&pn, g_mouse_ray);
    triangle tri = {SFloat3_convert(SFloat4_xyz(p0)),
                    SFloat3_convert(SFloat4_xyz(p1)),
                    SFloat3_convert(SFloat4_xyz(p2))
                   };
    bool c = Triangle_point_is_in_triangle(&tri, &cross_point);
    if (c)
    {
        printf("##cross point %f %f %f\n", cross_point.x, cross_point.y, cross_point.z);
        return proc_end;
    }
    else
        return proc_continue;
}

void pure_color_material_proc(VertexDecl dec, PixelShaderBuffer psb, material_input input)
{
    if (!VertexDecl_test(dec))
    {
        return;
    }
    ShaderNode psn = ShaderNode_new();
    ShaderNode_set_name(psn, "PixelProc");

    ShaderNode_set_function(psn,
                            "{\n"
                            "    vec3 cmap = texture2D(ColorMap, vTexCoord).rgb;\n"
                            "    gl_FragData[0] = vec4( cmap, 1.0 );"
                            "}\n");
    ShaderBuffer sb;
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    ShaderNode_delete(psn);
}

void pure_lighting_material_proc(VertexDecl dec, PixelShaderBuffer psb, material_input input)
{
    if (!VertexDecl_test(dec))
    {
        return;
    }
    ShaderNode psn = ShaderNode_new();
    ShaderNode_set_name(psn, "PixelProc");

    ShaderNode_set_function(psn,
                            "{\n"
                            "    vec3 lmap = texture2D(LightingMap, vTexCoord).rgb;\n"
                            "    gl_FragData[0] = vec4( lmap, 1.0 );"
                            "}\n");
    ShaderBuffer sb;
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    ShaderNode_delete(psn);
}

void white_material_proc(VertexDecl dec, PixelShaderBuffer psb, material_input input)
{
    if (!VertexDecl_test(dec))
    {
        return;
    }
    ShaderNode psn = ShaderNode_new();
    ShaderNode_set_name(psn, "WhitePixelProc");

    ShaderNode_set_function(psn,
                            "{\n"
                            "    gl_FragData[0] = vec4( 1.0, 1.0, 1.0, 1.0 );"
                            "}\n");
    ShaderBuffer sb;
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    ShaderNode_delete(psn);
}

void red_material_proc(VertexDecl dec, PixelShaderBuffer psb, material_input input)
{
    if (!VertexDecl_test(dec))
    {
        return;
    }
    ShaderNode psn = ShaderNode_new();
    ShaderNode_set_name(psn, "WhitePixelProc");

    ShaderNode_set_function(psn,
                            "{\n"
                            "    gl_FragData[0] = vec4( 1.0, 0.0, 0.0, 1.0 );"
                            "}\n");
    ShaderBuffer sb;
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    ShaderNode_delete(psn);
}

void green_material_proc(VertexDecl dec, PixelShaderBuffer psb, material_input input)
{
    if (!VertexDecl_test(dec))
    {
        return;
    }
    ShaderNode psn = ShaderNode_new();
    ShaderNode_set_name(psn, "WhitePixelProc");

    ShaderNode_set_function(psn,
                            "{\n"
                            "    gl_FragData[0] = vec4( 0.0, 1.0, 0.0, 1.0 );"
                            "}\n");
    ShaderBuffer sb;
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    ShaderNode_delete(psn);
}

void blue_material_proc(VertexDecl dec, PixelShaderBuffer psb, material_input input)
{
    if (!VertexDecl_test(dec))
    {
        return;
    }
    ShaderNode psn = ShaderNode_new();
    ShaderNode_set_name(psn, "WhitePixelProc");

    ShaderNode_set_function(psn,
                            "{\n"
                            "    gl_FragData[0] = vec4( 0.0, 0.0, 1.0, 1.0 );"
                            "}\n");
    ShaderBuffer sb;
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    ShaderNode_delete(psn);
}

#include "render_engine.hpp"

Renderable RenderEngine::s_pointLightsShape = NULL;

void RenderEngine::DrawPointLightShapeProc(PointLight pl)
{
    EFloat3 pos = PointLight_get_position(pl);
    Mesh mesh = create_locator(&pos, 0.2f);
    Renderable_add_mesh(s_pointLightsShape, mesh);
    Mesh_delete(mesh);
}

RenderEngine::~RenderEngine()
{
    Clear();
}

void RenderEngine::Clear()
{
    if (m_mainRenderer)
    {
        Renderer_delete(m_mainRenderer);
        m_mainRenderer = NULL;
    }
    if (m_coverRenderer)
    {
        Renderer_delete(m_coverRenderer);
        m_coverRenderer = NULL;
    }
}

void RenderEngine::Init(uint x, uint y, uint width, uint height, HWND hwnd)
{
    Clear();
    m_mainRenderer = Renderer_new(x, y, width, height);
    m_coverRenderer = Renderer_new_from_prev_renderer(m_mainRenderer);

    Renderer_register_material(m_mainRenderer, "white_material", white_material_proc, Shaded, true);
    Renderer_register_material(m_mainRenderer, "red_material", red_material_proc, Shaded, true);
    Renderer_register_material(m_mainRenderer, "green_material", green_material_proc, Shaded, true);
    Renderer_register_material(m_mainRenderer, "blue_material", blue_material_proc, Shaded, true);
    Renderer_register_material(m_mainRenderer, "pure_color", pure_color_material_proc, Shaded, true);
    Renderer_register_material(m_mainRenderer, "pure_lighting", pure_lighting_material_proc, Shaded, true);

    Renderer_register_material(m_coverRenderer, "white_material", white_material_proc, Shaded, true);
    Renderer_register_material(m_coverRenderer, "red_material", red_material_proc, Shaded, true);

    VertexDecl dec  = VertexDecl_new_from_string(
                          "pf3"
                          "tf2"
                          "nf3"
                          "Tf3"
                          "Bf3");

    MaterialInstance inst = MaterialInstance_new("red_material", NULL, NULL);
    m_locator = Renderer_new_renderable(m_coverRenderer, dec, inst, Segment);
    Renderer_use_renderable(m_coverRenderer, m_locator);

    if (!s_pointLightsShape)
    {
        inst = MaterialInstance_new("white_material", NULL, NULL);
        s_pointLightsShape = Renderer_new_renderable(m_coverRenderer, dec, inst, Segment);
        Renderer_use_renderable(m_coverRenderer, s_pointLightsShape);
    }

    ///ray mouse_ray;
    ///memset(&mouse_ray, 0, sizeof(mouse_ray));
    ///g_mouse_ray = &mouse_ray;
    g_mouse_ray = (Ray)Malloc(sizeof(ray));

    g_camera_util = CameraUtility_new(Renderer_get_camera(m_mainRenderer));

    RECT rc;
    GetWindowRect(hwnd, &rc);
    g_mouse_x = rc.left;
    g_mouse_y = rc.top;

    InputSystem_Init(hwnd);
    InputSystem_register_input_listen(default_mouse_input_proc);
    InputSystem_register_input_listen(default_keyboard_input_proc);
}

void RenderEngine::Render(HDC hDC)
{
    EFloat3 axis = {0.0f, 1.0f, 0.0f};
    Camera cam = Renderer_get_camera(m_coverRenderer);
    {
        Renderable_clear(m_locator);

        sfloat3 ori = SFloat3_assign_from_efloat3(&g_mouse_ray->origin);
        sfloat3 dir = SFloat3_assign_from_efloat3(&g_mouse_ray->direction);

        dir = SFloat3_mul_float(-2.5f, dir);
        ori = SFloat3_add(ori, dir);

        EFloat3 center = SFloat3_convert(ori);

        Mesh mesh = create_locator(&center, 0.2f);
        Renderable_add_mesh(m_locator, mesh);
        Mesh_delete(mesh);
    }
    {
        Renderable_clear(s_pointLightsShape);
        Renderer_draw_point_light_shape(m_mainRenderer, DrawPointLightShapeProc);
    }

    Renderer_render(m_mainRenderer);
    Renderer_render(m_coverRenderer);
    SwapBuffers(hDC);

    InputSystem_update();
    Renderer_get_mouse_ray(m_mainRenderer, g_mouse_x, g_mouse_y, &g_mouse_ray->origin, &g_mouse_ray->direction);
}

Ray RenderEngine::GetMouseRay()
{
    return g_mouse_ray;
}

MaterialInstance RenderEngine::GetMaterialInst(VertexDecl decl, const char* mat_name, const char* color_tex_name, const char* normal_tex_name)
{
    return MaterialInstance_new(mat_name, color_tex_name, normal_tex_name);
}
