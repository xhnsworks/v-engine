#include <omp.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "common.h"
#include "etypes.h"
#include "mem.h"
#include "stack.h"
#include "elog.h"

#include "vertex_element.h"
#include "vertex_declaration.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

#include "list_test.h"
#include "tree_test.h"

#include "matrix4x4.h"
#include "gl_utility.h"
#include "sketch.h"
#include "texture2d.h"

#include "pass_console.h"
#include "float3.h"
#include "float4.h"
#include "sfloat3.h"
#include "sfloat4.h"

#include "shader_object.h"
#include "shader_node.h"
#include "shader_buffer.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"

#include "material_prototype.h"
#include "material_instance.h"
#include "renderable.h"
#include "renderer.h"

#include "lighting_pass.h"
#include "std_pass.h"
#include "display_pass.h"
#include "array.h"

#include "render_system.h"

#include "parser.h"

#include "cylinder.h"
#include "locator.h"

#include "plane.h"
#include "triangle.h"

#include "mesh_triangle.h"

#include "branch_node.h"

#include "input_system.h"
#include "camera_utility.h"
#include "exception.h"

#include "octree.h"

#include "collada_loader.h"

#include "line_utility.h"
///#define BASE_DIR "..\\..\\"
#define BASE_DIR "D:\\"
static uint g_fps = 0;
static Ray g_mouse_ray = NULL;
static int g_mouse_x = 0;
static int g_mouse_y = 0;

MaterialInstance g_smat0 = {NULL};
MaterialInstance g_smat1 = {NULL};
MaterialInstance g_smat2 = {NULL};
MaterialInstance g_smat3 = {NULL};
MaterialInstance g_smat4 = {NULL};
MaterialInstance g_smat5 = {NULL};
MaterialInstance g_gay   = {NULL};

MaterialInstance g_cover_smat0 = {NULL};

MaterialInstance g_gui_smat = {NULL};

VertexDecl g_dec = {NULL};
Matrix4x4 g_mat0 = NULL;
Matrix4x4 g_mat1 = NULL;
Matrix4x4 g_mat = NULL;

Mesh g_mesh0 = {NULL};
Mesh g_mesh1 = {NULL};

Renderer g_renderer = NULL;
Renderer g_cover_renderer = NULL;

Renderer g_gui_renderer = NULL;

///ShadowRenderer g_shadow_renderer = NULL;

LineDrawer g_line_drawer = NULL;

CameraUtility g_camera_util = NULL;

bool g_left_alt_key_down = false;

bool g_left_button_down = false;
bool g_right_button_down = false;
bool g_middle_button_down = false;

void default_mouse_input_proc(const input_event event)
{
    if (event.type == MouseMoveEvent)
    {
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
    else if (event.type == MouseAbsolutePositionEvent)
    {
        g_mouse_x = event.info.mouse_info.mouse_abs_pos.x;
        g_mouse_y = event.info.mouse_info.mouse_abs_pos.y + 35;
        ///printf("mouse x %d, y %d\n", g_mouse_x, g_mouse_y);
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
    Plane_Init_from_triangle(&pn, &p0, &p1, &p2);
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

ShaderNode pure_color_material_proc3(PxlSdrBuf _psb, int _id)
{
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    vec3 cmap = texture2D(ColorMap, vTexCoord).rgb;\n"
                            "    gl_FragData[0] = vec4( cmap, 1.0 );"
                            "}\n");
    return psn;
}

ShaderNode pure_lighting_material_proc3(PxlSdrBuf _psb, int _id)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    vec3 lmap = texture2D(DiffuseLightingMap, vTexCoord).rgb;\n"
                            "    gl_FragData[0] = vec4( lmap, 1.0 );"
                            "}\n");

    ShaderBuffer_add_prototype_node(sb, psn);
    return psn;
}

ShaderNode pure_normal_material_proc3(PxlSdrBuf _psb, int _id)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    vec4 lmap = texture2D(NormalTangentMap, vTexCoord).rgba;\n"
                            "    vec3 ret = NormalDecode(lmap);\n"
                            "    gl_FragData[0] = vec4( ret, 1.0 );"
                            "}\n");

    ShaderBuffer_add_prototype_node(sb, psn);
    return psn;
}

ShaderNode pure_tangent_material_proc3(PxlSdrBuf _psb, int _id)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    vec4 lmap = texture2D(NormalTangentMap, vTexCoord).rgba;\n"
                            "    lmap.xy = lmap.zw;\n"
                            "    vec3 ret = NormalDecode(lmap);\n"
                            "    gl_FragData[0] = vec4( ret, 1.0 );"
                            "}\n");

    ShaderBuffer_add_prototype_node(sb, psn);
    return psn;
}

ShaderNode red_material_proc3(PxlSdrBuf _psb, int _id)
{
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "RedPixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    gl_FragData[0] = vec4( 1.0, 0.0, 0.0, 1.0 );"
                            "}\n");
    return psn;
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


///////////////////////////////////////////////////////////////////////////////

HHOOK g_hhkLowLevelKybd = NULL;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    FILE *stream ;
    EAssert((stream = freopen("dump.txt", "w", stderr)) != NULL, "%s", "cant open dump file");

    LoadLibraryA("backtrace.dll");

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;
#define W_Width  1440
#define W_Height 900

    if (!RegisterClassEx(&wcex))
        return 0;

    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          W_Width,
                          W_Height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, 1);
    SetTimer(hwnd, 1,1000,NULL);

    EnableOpenGL(hwnd, &hDC, &hRC);

    TRY(0)

    RenderSystem_Init(hwnd);

    sfloat3 nor = SFloat3(-0.318, 0.692, 0.648);
    sfloat3 enc = SFloat3(-0.318, 0.692, 0.0);
    sfloat3 half = SFloat3(0.5, 0.5, 0.0);
    sfloat3 one = SFloat3(1.0, 1.0, 0.0);
    sfloat3 sqrt_z = SFloat3_assign_from_float(sqrt(-0.648*0.5+0.5));
/**
enc = normalize(n.xy) * (sqrt(-n.z*0.5+0.5));
enc = enc*0.5+0.5;
**/
    enc = SFloat3_normalize(enc);
    enc = SFloat3_mul(enc, sqrt_z);
    enc = SFloat3_mul_float(0.5, enc);
    enc = SFloat3_add(enc, half);
/**
float2 fenc = enc*2-1;
n.z = -(dot(fenc,fenc)*2-1);
n.xy = normalize(fenc) * sqrt(1-n.z*n.z);
**/
    sfloat3 fn = SFloat3_mul_float(2.0, enc);
    fn = SFloat3_sub(fn, one);
    float n_z = -(SFloat3_dot(fn, fn) * 2.0 - 1.0);
    sfloat3 n_xy = SFloat3_mul_float(sqrt(1.0 - n_z * n_z), SFloat3_normalize(fn));
    elog("%s", "##dec");
    SFloat3_log(n_xy);
    elog("%f", n_z);
/**
// optimized
// enc4 is float4, with .rg containing encoded normal
float4 nn =
    enc4*float4(2,2,0,0) +
    float4(-1,-1,1,-1);
float l = dot(nn.xyz,-nn.xyw);
nn.z = l;
nn.xy *= sqrt(l);
n = nn.xyz * 2 + float3(0,0,-1);
**/
    ///input_Init(hwnd);
    printf("octtree node size %d\n", sizeof(octree_node));
    Octree ot = Octree_new(0.25f, 2);
    Octree_print(ot);

    ray r;
    EFloat3 b = {1.0f, 0.0f, 0.194f};
    EFloat3 e = {-1.0f, 0.0f, 0.194f};
    Ray_Init_from_begin_end(&r, &b, &e);
    plane p;
    p.origin.x = 0.0f; p.origin.y = 0.0f; p.origin.z = 0.0f;
    p.normal.x = 0.799f; p.normal.y = 0.503f; p.normal.z = 0.318f;
    EFloat3 c = Plane_ray_cross(&p, &r);
    printf("cross %f %f %f\n", c.x, c.y, c.z);
    ///getchar();

    uint gui_render_width = 200;
    g_renderer = Renderer_new(gui_render_width, 0, W_Width - gui_render_width, W_Height);
    g_cover_renderer = Renderer_new_from_prev_renderer(g_renderer);
    ///g_cover_renderer = Renderer_new(gui_render_width, 0, W_Width - gui_render_width, W_Height);

    g_gui_renderer = Renderer_new(0, 0, gui_render_width, W_Height);

    Camera cam = Renderer_get_camera(g_gui_renderer);
    Matrix4x4 orthogonal_matrix = Matrix4x4_new();
    Matrix4x4_set_one(orthogonal_matrix);
    Matrix4x4_set_translate(orthogonal_matrix, 0.0f, 0.0f, 1.0f);
    Camera_set_proj_matrix(cam, orthogonal_matrix);
    Matrix4x4_delete(orthogonal_matrix);

    SDisplayProc s;
    s.proc = red_material_proc3;
    Renderer_register_material(g_renderer, "red_material", s, Shaded, false);

    s.proc = pure_color_material_proc3;
    Renderer_register_material(g_renderer, "pure_color", s, Shaded, false);
    s.proc = pure_lighting_material_proc3;
    Renderer_register_material(g_renderer, "pure_lighting", s, Shaded, false);
    s.proc = pure_normal_material_proc3;
    Renderer_register_material(g_renderer, "pure_normal", s, Shaded, false);
    s.proc = pure_tangent_material_proc3;
    Renderer_register_material(g_renderer, "pure_tangent", s, Shaded, false);

    s.proc = red_material_proc3;
    Renderer_register_material(g_cover_renderer, "red_material", s, Shaded, true);

    g_line_drawer = ILnDwr.New(g_cover_renderer);

    Renderer_register_material(g_gui_renderer, "red_material", s, Shaded, true);
    ///g_smat1 = MaterialInstance_new(g_renderer, g_dec, "default_material", NULL, NULL);
    gl_Init();
    ERROR_PROC;

    g_dec = VertexDecl_new_from_string("pf3"
                                       "tf2"
                                       "nf3"
                                       "Tf3"
                                       "Bf3");

    VertexDecl_new_from_string("pf3"
                               "tf2"
                               "nf3"
                               "Tf3"
                               "Bf3");
/**
    sfloat3 a = SFloat3(1.0f, 0.0f, 2.0f);
    sfloat3 b = SFloat3(2.0f, 0.0f, 1.0f);
    sfloat3_compare_result ret = SFloat3_equal(a, b);
    printf("compare result %d %d %d\n", ret.x_cmp_ret, ret.y_cmp_ret, ret.z_cmp_ret);
**/
    ray mouse_ray;
    memset(&mouse_ray, 0, sizeof(mouse_ray));
    g_mouse_ray = &mouse_ray;

    g_camera_util = CameraUtility_new(Renderer_get_camera(g_renderer));
    ///g_camera_util = CameraUtility_new(Renderer_get_camera(g_gui_renderer));

    RECT rc;
    GetWindowRect(hwnd, &rc);
    ///g_mouse_x = rc.left;
    ///g_mouse_y = rc.top;
    g_mouse_x = 0;
    g_mouse_y = 0;

    InputSystem_Init(hwnd);
    InputSystem_register_input_listen(default_mouse_input_proc);
    InputSystem_register_input_listen(default_keyboard_input_proc);

    ///VertexBuffer vbf = VertexBuffer_new(g_dec);
    ///IndexBuffer ibf = IndexBuffer_new(Triangular);

    Renderable locator;

    {
        char path0[MAX_PATH];
        char path1[MAX_PATH];
        char path2[MAX_PATH];
        char path3[MAX_PATH];

        snprintf(path0, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\new.png");
        snprintf(path1, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\Normal-Map.png");
        snprintf(path2, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\GAZER789_Diffuse.png");
        snprintf(path3, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\GAZER789_Normal-1.png");

        g_smat0 = MaterialInstance_new("default_material", path0, path1);
        g_smat1 = MaterialInstance_new("default_material", NULL, NULL);
        g_smat2 = MaterialInstance_new("red_material", NULL, NULL);
        g_smat3 = MaterialInstance_new("pure_lighting", path0, path1);
        g_smat4 = MaterialInstance_new("pure_normal", path0, path1);
        g_smat5 = MaterialInstance_new("pure_tangent", NULL, NULL);
        g_gay   = MaterialInstance_new("default_material", path2, path3);

        g_cover_smat0 = MaterialInstance_new("red_material", NULL, NULL);

        g_gui_smat = MaterialInstance_new("red_material", NULL, NULL);
    }
    /**
        {
            locator = Renderer_new_renderable(g_renderer, g_dec, g_smat2, Segment);
            Renderer_use_renderable(g_renderer, locator);
        }
        **/
    {
        locator = Renderer_new_renderable(g_cover_renderer, g_dec, g_smat2, Segment);
        Renderer_use_renderable(g_cover_renderer, locator);

        ///g_line_drawer->line_strip_rabl = locator;
    }
    Renderable gui_plane;
    {
        gui_plane = Renderer_new_renderable(g_gui_renderer, g_dec, g_gui_smat, Triangular);
        Renderer_use_renderable(g_gui_renderer, gui_plane);
    }
    /**
    {
        char path[MAX_PATH];
        snprintf(path, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\mesh6.ogl");
        Mesh mesh = Mesh_new();
        Mesh_load_form_file(mesh, path);

        Renderable r = Renderer_new_renderable(g_renderer, g_dec, g_smat3, Triangular);
        Renderable_add_mesh(r, mesh);
        Renderer_use_renderable(g_renderer, r);

        Mesh_delete(mesh);
    }
    **/
     {
        Mesh mesh = NULL;
        {
            ColladaState colSt = ColladaState_new();
            ColladaState_load_mesh(colSt, "D:\\test_scene\\test4.dae");
            ColladaState_log(colSt);
            mesh = ColladaState_create_mesh(colSt);
        }

        char path[MAX_PATH];
        snprintf(path, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\mesh_pos_unit1.ogl");
        ///mesh = Mesh_new();
        ///Mesh_load_form_file(mesh, path);
        ///Mesh mesh = load_collada_mesh("D:\\test_scene\\test1.dae");

        Renderable r = Renderer_new_renderable(g_renderer, g_dec, g_smat3, Triangular);
        Renderable_add_mesh(r, mesh);
        Renderer_use_renderable(g_renderer, r);

        Mesh_delete(mesh);
    }
    {
        Mesh mesh = NULL;
        {
            ColladaState colSt = ColladaState_new();
            ColladaState_load_mesh(colSt, "D:\\test_scene\\gay.dae");
            ColladaState_log(colSt);
            mesh = ColladaState_create_mesh(colSt);
        }

        char path[MAX_PATH];
        snprintf(path, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\mesh_pos_unit1.ogl");
        ///mesh = Mesh_new();
        ///Mesh_load_form_file(mesh, path);
        ///Mesh mesh = load_collada_mesh("D:\\test_scene\\test1.dae");

        Renderable r = Renderer_new_renderable(g_renderer, g_dec, g_gay, Triangular);
        Renderable_add_mesh(r, mesh);
        Renderer_use_renderable(g_renderer, r);

        Matrix4x4 mat = Renderable_get_world_matrix(r);
        Matrix4x4 rot_mat = Matrix4x4_new();
        Matrix4x4_set_x_axis_rotate(rot_mat, 3.14 * 0.3);
        Matrix4x4_set_scale(mat, 10.5, 10.5, 10.5);
        Matrix4x4_mul_matrix4(rot_mat, mat, mat);

        Mesh_delete(mesh);

    }
    ///==============create default render data================///
    sfloat4 p0 = SFloat4(-1.2f, 0.0f, 0.5f, 1.0f);
    sfloat4 p1 = SFloat4(1.0f, 0.0f, 1.5f, 1.0f);
    sfloat4 p2 = SFloat4(-2.5f, 0.0f, 0.1f, 1.0f);
    sfloat4 p3 = SFloat4(1.8f, 0.0f, 0.0f, 1.0f);

    sfloat4 p4 = SFloat4(1.0f, 0.0f, -1.6f, 1.0f);
    sfloat4 p5 = SFloat4(1.0f, 0.0f, 1.7f, 1.0f);
    sfloat4 p6 = SFloat4(1.8f, 0.0f, 0.9f, 1.0f);
    sfloat4 p7 = SFloat4(-1.0f, 0.0f, 0.3f, 1.0f);

    Matrix4x4 scl = Matrix4x4_new();
    Matrix4x4_set_scale(scl, 1.5, 1.5, 1.5);
    p0 = Matrix4x4_mul_float4(scl, p0);
    p1 = Matrix4x4_mul_float4(scl, p1);
    p2 = Matrix4x4_mul_float4(scl, p2);
    p3 = Matrix4x4_mul_float4(scl, p3);

    p4 = Matrix4x4_mul_float4(scl, p4);
    p5 = Matrix4x4_mul_float4(scl, p5);
    p6 = Matrix4x4_mul_float4(scl, p6);
    p7 = Matrix4x4_mul_float4(scl, p7);

    Matrix4x4_delete(scl);

    g_mat = Matrix4x4_new();
    Matrix4x4_set_y_axis_rotate(g_mat, 0.01f);

    p0 = Matrix4x4_mul_float4(g_mat, p0);
    p1 = Matrix4x4_mul_float4(g_mat, p1);
    p2 = Matrix4x4_mul_float4(g_mat, p2);
    p3 = Matrix4x4_mul_float4(g_mat, p3);

    p4 = Matrix4x4_mul_float4(g_mat, p4);
    p5 = Matrix4x4_mul_float4(g_mat, p5);
    p6 = Matrix4x4_mul_float4(g_mat, p6);
    p7 = Matrix4x4_mul_float4(g_mat, p7);

    Renderer_add_dir_light_2(g_renderer);
/**
    PointLight pl0 = Renderer_add_point_light(g_renderer);
    PointLight pl1 = Renderer_add_point_light(g_renderer);
    PointLight pl2 = Renderer_add_point_light(g_renderer);
    PointLight pl3 = Renderer_add_point_light(g_renderer);
    **/
    Camera light_cam = Camera_new(512, 512);
    /**
    g_shadow_renderer = ShadowRenderer_new(light_cam);
    {
        char path[MAX_PATH];
        snprintf(path, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\mesh_pos_unit1.ogl");
        Mesh mesh = Mesh_new();
        Mesh_load_form_file(mesh, path);

        Renderable r = ShadowRenderer_new_renderable(g_shadow_renderer, g_dec, g_smat3, Triangular);
        Renderable_add_mesh(r, mesh);
        ShadowRenderer_use_renderable(g_shadow_renderer, r);

        Mesh_delete(mesh);
    }
    {
        char path[MAX_PATH];
        snprintf(path, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\mesh6.ogl");
        Mesh mesh = Mesh_new();
        Mesh_load_form_file(mesh, path);

        Renderable r = ShadowRenderer_new_renderable(g_shadow_renderer, g_dec, g_smat1, Triangular);
        Renderable_add_mesh(r, mesh);
        ShadowRenderer_use_renderable(g_shadow_renderer, r);

        Mesh_delete(mesh);
    }
    **/
/**
    ///=======================================================================///
    PointLight_set_color(pl0, 0.05f, 0.01f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl0, 0.8f);
    ///=======================================================================///
    PointLight_set_color(pl1, 0.02f, 0.0f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl1, 0.8f);
    ///=======================================================================///
    PointLight_set_color(pl2, 0.0f, 0.03f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl2, 0.8f);
    ///=======================================================================///
    PointLight_set_color(pl3, 0.02f, 0.02f, 0.02f, 0.0f);
    PointLight_set_atte_coef(pl3, 0.8f);
    ///=======================================================================///
**/
/**
    PointLight pl4 = Renderer_add_point_light(g_renderer);
    PointLight pl5 = Renderer_add_point_light(g_renderer);
    PointLight pl6 = Renderer_add_point_light(g_renderer);
    PointLight pl7 = Renderer_add_point_light(g_renderer);
    ///=======================================================================///
    PointLight_set_color(pl4, 0.0f, 0.1f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl4, 0.8f);
    ///=======================================================================///
    PointLight_set_color(pl5, 0.2f, 0.0f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl5, 0.8f);
    ///=======================================================================///
    PointLight_set_color(pl6, 1.0f, 0.3f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl6, 0.8f);
    ///=======================================================================///
    PointLight_set_color(pl7, 0.8f, 0.8f, 0.8f, 1.0f);
    PointLight_set_atte_coef(pl7, 0.8f);
    ///=======================================================================///
**/
    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            {
                EFloat3 axis = {0.0f, 1.0f, 0.0f};
                ///Camera cam = Renderer_get_camera(g_renderer);
                ///Camera_rotate(cam, axis, -0.001);
                {
/**
                    Renderable_clear(locator);

                    sfloat3 ori = SFloat3_assign_from_efloat3(&g_mouse_ray->origin);
                    sfloat3 dir = SFloat3_assign_from_efloat3(&g_mouse_ray->direction);

                    dir = SFloat3_mul_float(-2.5f, dir);
                    ori = SFloat3_add(ori, dir);

                    EFloat3 center = SFloat3_convert(ori);

                    Mesh mesh = create_locator(&center, 0.2f);
                    Renderable_add_mesh(locator, mesh);
                    Mesh_delete(mesh);

                    sfloat3 begin = ori;
                    sfloat3 vec_x = SFloat3(1.0f, 0.0f, 0.0f);
                    sfloat3 end = SFloat3_add(begin, vec_x);

                    EFloat3 ebegin = SFloat3_convert(begin);
                    EFloat3 eend = SFloat3_convert(end);
                    EFloat3 enor = {0.0f, 0.0f, 0.0f};
                    EFloat4 ecol = {1.0f, 0.0f, 1.0f, 1.0f};
                    line_def def = {ebegin, ecol, enor, eend, ecol, enor};
**/
                    ///ILnDwr.add_line(g_line_drawer, &def);

                    EFloat3 edir = g_mouse_ray->direction;
                    ///LineDrawer_draw_circle(g_line_drawer, &ebegin, &edir, 1.0f);
                    ///LineDrawer_draw_translate_manipulator(g_line_drawer, &ebegin, &edir, 1.0f);
                }
                {
                    Renderable_clear(gui_plane);
                    {
                        Mesh mesh = create_plane_mesh();
                        Renderable_add_mesh(gui_plane, mesh);
                        Mesh_delete(mesh);
                    }
                }
                {
                    EFloat4 color = {1.0f, 1.0f, 0.0f, 1.0};
                    Octree_set_color(ot, color);
                    Octree_touch(ot, g_mouse_ray, g_line_drawer);

                    Octree_draw(ot, g_line_drawer);
                }

                ///Renderable_process(r, default_renderable_proc);
/**
                PointLight_set_position_eft3(pl0, SFloat3_convert(p0));
                PointLight_set_position_eft3(pl1, SFloat3_convert(p1));
                PointLight_set_position_eft3(pl2, SFloat3_convert(p2));
                PointLight_set_position_eft3(pl3, SFloat3_convert(p3));

                PointLight_set_position_eft3(pl4, SFloat3_convert(p4));
                PointLight_set_position_eft3(pl5, SFloat3_convert(p5));
                PointLight_set_position_eft3(pl6, SFloat3_convert(p6));
                PointLight_set_position_eft3(pl7, SFloat3_convert(p7));
**/
                p0 = Matrix4x4_mul_float4(g_mat, p0);
                p1 = Matrix4x4_mul_float4(g_mat, p1);
                p2 = Matrix4x4_mul_float4(g_mat, p2);
                p3 = Matrix4x4_mul_float4(g_mat, p3);

                p4 = Matrix4x4_mul_float4(g_mat, p4);
                p5 = Matrix4x4_mul_float4(g_mat, p5);
                p6 = Matrix4x4_mul_float4(g_mat, p6);
                p7 = Matrix4x4_mul_float4(g_mat, p7);

                ILnDwr.update(g_line_drawer);

                Renderer_render(g_renderer);
                ///Renderer_render(g_cover_renderer);

                ///Renderer_render(g_gui_renderer);
                ///Renderer_clear_sketchbook(g_renderer);

                ///ShadowRenderer_render(g_shadow_renderer);
                SwapBuffers(hDC);
                g_fps++;

                for (int i = 0; i < 128; i++)
                {
                    InputSystem_update();
                }
                Renderer_get_mouse_ray(g_renderer, g_mouse_x, g_mouse_y, &g_mouse_ray->origin, &g_mouse_ray->direction);
                ///Mesh_print_mesh_count();
                ///Renderer_get_mouse_ray(g_gui_renderer, g_mouse_x, g_mouse_y, &g_mouse_ray->origin, &g_mouse_ray->direction);
            }
        }
    }

    CATCH(0)
    {
        printf("exce %s\n", GET_EXCE);
        getchar();
        exit(1);
    }

    DisableOpenGL(hwnd, hDC, hRC);

    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
        Renderer_delete(g_renderer);
        Renderer_delete(g_cover_renderer);
        ///ShadowRenderer_delete(g_shadow_renderer);
        Renderer_delete(g_gui_renderer);
        Matrix4x4_delete(g_mat0);
        Matrix4x4_delete(g_mat1);
        Matrix4x4_delete(g_mat);

        MaterialInstance_delete(g_smat0);
        MaterialInstance_delete(g_smat1);
        MaterialInstance_delete(g_smat2);
        MaterialInstance_delete(g_smat3);

        MaterialInstance_delete(g_cover_smat0);
        MaterialInstance_delete(g_gui_smat);

        CameraUtility_delete(g_camera_util);

        Float_Dest();
        RenderSystem_Dest();
        Mlog();

        ///UnhookWindowsHookEx(g_hhkLowLevelKybd);
        return 0;

    case WM_TIMER:
        elog("##FPS %d", g_fps);
        g_fps = 0;
        break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
    }
    break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
