///extern "C" {
#include <omp.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "common.h"
#include "etypes.h"
#include "emem.h"
#include "stack.h"
#include "elog.h"

#include "vertex_element.h"
#include "vertex_declaration.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

///#include "list_test.h"
///#include "tree_test.h"

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

///#include "cylinder.h"
///#include "locator.h"

#include "plane.h"
#include "triangle.h"

#include "input_system.h"
#include "camera_utility.h"
///}
///#include "render_engine.hpp"

///#define BASE_DIR "..\\..\\"
#define BASE_DIR "D:\\"
static uint g_fps = 0;

MaterialInstance g_smat0 = {NULL};
MaterialInstance g_smat1 = {NULL};
MaterialInstance g_smat2 = {NULL};
MaterialInstance g_smat3 = {NULL};

MaterialInstance g_cover_smat0 = {NULL};

VertexDecl g_dec = {NULL};
Matrix4x4 g_mat0 = NULL;
Matrix4x4 g_mat1 = NULL;
Matrix4x4 g_mat = NULL;

Mesh g_mesh0 = {NULL};
Mesh g_mesh1 = {NULL};

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
    ///VertexDeclaration_testx();

    FILE *stream ;
    if((stream = freopen("dump.txt", "w", stderr)) == NULL)
        exit(-1);
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


    if (!RegisterClassEx(&wcex))
        return 0;

    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1280,
                          720,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, 1);
    SetTimer(hwnd, 1,1000,NULL);

    ///input_Init(hwnd);

    EnableOpenGL(hwnd, &hDC, &hRC);

    RenderSystem_Init(hwnd);

    RenderEngine pipline;
    pipline.Init(0, 0, 1280, 720, hwnd);
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

    {
        char path0[MAX_PATH];
        char path1[MAX_PATH];

        snprintf(path0, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\new.png");
        snprintf(path1, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\NMBalls.png");

        g_smat0 = MaterialInstance_new("default_material", path0, path1);
        g_smat1 = MaterialInstance_new("default_material", NULL, NULL);
        g_smat2 = MaterialInstance_new("red_material", NULL, NULL);
        g_smat3 = MaterialInstance_new("pure_lighting", path0, path1);

        g_cover_smat0 = MaterialInstance_new("red_material", NULL, NULL);
    }

    {
        char path[MAX_PATH];
        snprintf(path, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\mesh_pos_unit1.ogl");
        Mesh mesh = Mesh_new();
        Mesh_load_form_file(mesh, path);

        Renderable r = Renderer_new_renderable(pipline.m_mainRenderer, g_dec, g_smat3, Triangular);
        Renderable_add_mesh(r, mesh);
        Renderer_use_renderable(pipline.m_mainRenderer, r);
    }
    {
        char path[MAX_PATH];
        snprintf(path, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\mesh6.ogl");
        Mesh mesh = Mesh_new();
        Mesh_load_form_file(mesh, path);

        Renderable r = Renderer_new_renderable(pipline.m_mainRenderer, g_dec, g_smat1, Triangular);
        Renderable_add_mesh(r, mesh);
        Renderer_use_renderable(pipline.m_mainRenderer, r);
    }
    {
        Mesh mesh = create_cylinder_mesh(1.0f, 1.0f, 6);

        Renderable r = Renderer_new_renderable(pipline.m_mainRenderer, g_dec, g_smat2, Triangular);
        Renderable_add_mesh(r, mesh);
        Renderer_use_renderable(pipline.m_mainRenderer, r);
    }

    ///==============create default render data================///
    sfloat4 p0 = SFloat4(-1.2f, 0.0f, 0.3f, 1.0f);
    sfloat4 p1 = SFloat4(1.0f, 0.0f, 1.5f, 1.0f);
    sfloat4 p2 = SFloat4(-1.5f, 0.0f, 0.1f, 1.0f);
    sfloat4 p3 = SFloat4(1.8f, 0.0f, 0.0f, 1.0f);

    sfloat4 p4 = SFloat4(1.0f, 0.0f, -1.6f, 1.0f);
    sfloat4 p5 = SFloat4(1.0f, 0.0f, 1.7f, 1.0f);
    sfloat4 p6 = SFloat4(1.8f, 0.0f, 0.9f, 1.0f);
    sfloat4 p7 = SFloat4(-1.0f, 0.0f, 0.3f, 1.0f);

    g_mat = Matrix4x4_new();

    Matrix4x4_set_y_axis_rotate(g_mat, 0.001f);

    PointLight pl0 = Renderer_add_point_light(pipline.m_mainRenderer);
    PointLight pl1 = Renderer_add_point_light(pipline.m_mainRenderer);
    PointLight pl2 = Renderer_add_point_light(pipline.m_mainRenderer);
    PointLight pl3 = Renderer_add_point_light(pipline.m_mainRenderer);

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

    PointLight pl4 = Renderer_add_point_light(pipline.m_mainRenderer);
    PointLight pl5 = Renderer_add_point_light(pipline.m_mainRenderer);
    PointLight pl6 = Renderer_add_point_light(pipline.m_mainRenderer);
    PointLight pl7 = Renderer_add_point_light(pipline.m_mainRenderer);
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

                PointLight_set_position_ft3(pl0, p0);
                PointLight_set_position_ft3(pl1, p1);
                PointLight_set_position_ft3(pl2, p2);
                PointLight_set_position_ft3(pl3, p3);

                PointLight_set_position_ft3(pl4, p4);
                PointLight_set_position_ft3(pl5, p5);
                PointLight_set_position_ft3(pl6, p6);
                PointLight_set_position_ft3(pl7, p7);

                p0 = Matrix4x4_mul_float4(g_mat, p0);
                p1 = Matrix4x4_mul_float4(g_mat, p1);
                p2 = Matrix4x4_mul_float4(g_mat, p2);
                p3 = Matrix4x4_mul_float4(g_mat, p3);

                p4 = Matrix4x4_mul_float4(g_mat, p4);
                p5 = Matrix4x4_mul_float4(g_mat, p5);
                p6 = Matrix4x4_mul_float4(g_mat, p6);
                p7 = Matrix4x4_mul_float4(g_mat, p7);

                pipline.Render(hDC);
            }
        }
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
        ///Renderer_delete(g_renderer);
        Matrix4x4_delete(g_mat0);
        Matrix4x4_delete(g_mat1);
        Matrix4x4_delete(g_mat);
        VertexDecl_delete(g_dec);
        Float_Dest();
        Mlog();

        ///UnhookWindowsHookEx(g_hhkLowLevelKybd);
        return 0;

    case WM_TIMER:
        /**
                input_update();
                {
                    int x, y;
                    input_get_mouse_pos(&x, &y);
                    Renderer_get_mouse_ray(g_renderer, x, y, &g_mouse_ray->origin, &g_mouse_ray->direction);
                }
        **/
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
