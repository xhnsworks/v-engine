#include <omp.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "common.h"
#include "etypes.h"
#include "mem.h"
#include "stack.h"
#include "elog.h"

#include "recycle_bin.h"
#include "vertex_element.h"
#include "vertex_declaration.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

#include "list_test.h"
#include "tree_test.h"
#include "rb_test.h"
#include "matrix4x4.h"
#include "gl_utility.h"
#include "sketch.h"
#include "texture2d.h"
#include "texture_console.h"
#include "pass_console.h"
#include "float3.h"
#include "float4.h"

#include "shader_object.h"
#include "shader_node.h"
#include "shader_buffer.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"

#include "std_material.h"
#include "renderable.h"
#include "pipeline.h"

#include "lighting_pass.h"
#include "std_pass.h"
#include "display_pass.h"
#include "array.h"

static unsigned int g_fps = 0;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void gl_Init(void)
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearStencil(0);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
    /// ATI should be set like this
    ///glDepthFunc(GL_ALWAYS);

    glEnable(GL_DEPTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
}

StdMaterial g_smat0 = {NULL};
StdMaterial g_smat1 = {NULL};
VertexDeclaration g_dec = {NULL};
Matrix4x4 g_mat0 = NULL;
Matrix4x4 g_mat1 = NULL;
Matrix4x4 g_mat = NULL;

Mesh g_mesh0 = {NULL};
Mesh g_mesh1 = {NULL};

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
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
                          g_width,
                          g_height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, 1);
    SetTimer(hwnd, 1,1000,NULL);

    EnableOpenGL(hwnd, &hDC, &hRC);

    MInit();
    RecycleBin_Init();

    ELog_Init();
    glewInit();
    Float_Init();

    Pipeline_Init();

    glViewport(0,0,g_width,g_height);
    gl_Init();

    g_dec = VertexDeclaration_new();
    VertexElement pos = VertexElement_new(Float32_3, Position);
    VertexElement tex = VertexElement_new(Float32_2, TexCoord);
    VertexElement nor = VertexElement_new(Float32_3, Normal);
    VertexElement tgt = VertexElement_new(Float32_3, Tangent);
    VertexElement bin = VertexElement_new(Float32_3, Binormal);

    VertexDeclaration_add_element(g_dec, pos);
    VertexDeclaration_add_element(g_dec, tex);
    VertexDeclaration_add_element(g_dec, nor);
    VertexDeclaration_add_element(g_dec, tgt);
    VertexDeclaration_add_element(g_dec, bin);

    VertexBuffer vbf = VertexBuffer_new(g_dec);
    IndexBuffer ibf = IndexBuffer_new();

    Mesh mesh = Mesh_new();
    Mesh_load_form_file(mesh, "d:\\test_scene\\mesh_pos.ogl");
    uint32 face_count = Mesh_get_face_count(mesh);

    VertexBuffer_attach_mesh(vbf, mesh);
    IndexBuffer_attach_mesh(ibf, mesh);

    Mesh_delete(mesh);

    g_smat0 = StdMaterial_new(g_dec, "d:\\test_scene\\new.png", "d:\\test_scene\\NMBalls.png");
    Pipeline_append_renderable(vbf, ibf, face_count, g_smat0);
    ///=======================================================================///

    vbf = VertexBuffer_new(g_dec);
    ibf = IndexBuffer_new();

    mesh = Mesh_new();
    Mesh_load_form_file(mesh, "d:\\test_scene\\mesh6.ogl");
    face_count = Mesh_get_face_count(mesh);

    VertexBuffer_attach_mesh(vbf, mesh);
    IndexBuffer_attach_mesh(ibf, mesh);

    Mesh_delete(mesh);

    g_smat1 = StdMaterial_new(g_dec, NULL, NULL);
    Pipeline_append_renderable(vbf, ibf, face_count, g_smat1);

    ///==============create default render data================///
    float4 p0 = Float4(-2.2f, 0.0f, 0.0f, 1.0f);
    float4 p1 = Float4(0.0f, 1.7f, 0.0f, 1.0f);
    float4 p2 = Float4(-3.5f, 0.0f, 0.0f, 1.0f);
    float4 p3 = Float4(1.8f, 0.0f, 0.0f, 1.0f);

    float4 p4 = Float4(0.0f, -1.7f, 0.0f, 1.0f);
    float4 p5 = Float4(0.0f, 2.2f, 0.0f, 1.0f);
    float4 p6 = Float4(1.8f, 0.0f, 0.0f, 1.0f);
    float4 p7 = Float4(0.0f, -1.75f, 0.0f, 1.0f);

    g_mat0 = Matrix4x4_new();
    Matrix4x4_set_z_axis_rotate(g_mat0, 0.03f);
    g_mat1 = Matrix4x4_new();
    Matrix4x4_set_y_axis_rotate(g_mat1, 0.05f);
    g_mat = Matrix4x4_new();
    Matrix4x4_mul_matrix4(g_mat0, g_mat1, g_mat);

    PointLight pl0 = Pipeline_add_point_light();

    PointLight pl1 = Pipeline_add_point_light();
    PointLight pl2 = Pipeline_add_point_light();
    PointLight pl3 = Pipeline_add_point_light();

    ///=======================================================================///
    PointLight_set_position(pl0, -3.0f, 0.0f, 0.0f);
    PointLight_set_color(pl0, 0.5f, 0.1f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl0, 0.8f);
    ///=======================================================================///

    PointLight_set_position(pl1, 0.0f, 4.0f, 0.0f);
    PointLight_set_color(pl1, 0.2f, 0.0f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl1, 0.8f);
    ///=======================================================================///

    ///=======================================================================///
    PointLight_set_position(pl2, -4.0f, 0.0f, 0.0f);
    PointLight_set_color(pl2, 0.0f, 0.3f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl2, 0.8f);

    ///=======================================================================///
    PointLight_set_position(pl3, 2.0f, 0.0f, 0.0f);
    PointLight_set_color(pl3, 0.2f, 0.2f, 0.2f, 0.0f);
    PointLight_set_atte_coef(pl3, 0.8f);
    ///=======================================================================///

    PointLight pl4 = Pipeline_add_point_light();
    PointLight pl5 = Pipeline_add_point_light();
    PointLight pl6 = Pipeline_add_point_light();
    PointLight pl7 = Pipeline_add_point_light();
    ///=======================================================================///
    PointLight_set_position(pl4, 0.0f, -4.0f, 0.0f);
    PointLight_set_color(pl4, 0.0f, 0.1f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl4, 0.8f);
    ///=======================================================================///
    PointLight_set_position(pl5, 0.0f, 4.0f, 0.0f);
    PointLight_set_color(pl5, 0.2f, 0.0f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl5, 0.8f);
    ///=======================================================================///
    ///=======================================================================///
    PointLight_set_position(pl6, 4.0f, 0.0f, 0.0f);
    PointLight_set_color(pl6, 1.0f, 0.3f, 0.0f, 1.0f);
    PointLight_set_atte_coef(pl6, 0.8f);
    ///=======================================================================///
    PointLight_set_position(pl7, 0.0f, -5.0f, 0.0f);
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

                EFloat3 axis = {0.0f, 1.0f, 0.0f};
                Camera cam = Pipeline_get_camera();
                EFloat3 _cam_pos = Camera_get_position(cam);
                float3 cam_pos = Float3(0, 0, 0);

                float3 p3 = Float3(5.5f, 2.0f, 1.0f);
                float3 vec = Float3_sub(p3, cam_pos);
                float depth = Float3_length(vec);
                ///Camera_rotate(cam, axis, 0.001f);
                Matrix4x4 matx = Matrix4x4_new();
                Matrix4x4 maty = Matrix4x4_new();
                Matrix4x4_assign(matx, g_proj_matrix);
                Matrix4x4_assign(maty, g_inv_proj_matrix);
                Matrix4x4_turn(matx);
                Matrix4x4_turn(maty);
                ///Matrix4x4_log(matx);
                ///Matrix4x4_log(maty);
                float4 p = Float4_assign(p3);
                p = Matrix4x4_mul_float4(matx, p);

                ///Float4_log(p);
                float www = p->w;
                p = Float4_div_float(p->w, p);
                ///Float4_log(p);
                p = Float4_mul_float(www, p);
                p = Matrix4x4_mul_float4(maty, p);
                Float4_log(p);

                float x = p->x;
                float y = p->y;
                float w = Camera_get_width(cam);
                float h = Camera_get_height(cam);
                float nnn = Camera_get_near_plane(cam);
                float fff = Camera_get_far_plane(cam);
                float ratio = (fff - nnn) / fff;
                float z = -nnn;
                x = x * w * 0.5f;
                y = y * h * 0.5f;

                float3 pos = Float3(x, y, z);
                Matrix4x4 rota = Camera_get_rota_matrix(cam);
                Matrix4x4 tran = Camera_get_tran_matrix(cam);
                ///Matrix4x4_log(rota);
                ///Matrix4x4_log(tran);
                Matrix4x4 mmm = Matrix4x4_new();
                Matrix4x4_mul_matrix4(rota, tran, mmm);
                ///Matrix4x4_log(mmm);

                ///Float3_log(pos);
                pos = Float3_normalize(pos);
                ///Float3_log(pos);
                pos = Float3_mul_float(depth, pos);

                pos->z = pos->z * ratio;

                float4 p4 = Float4_assign(pos);
                p4 = Matrix4x4_mul_float4(mmm, p4);
                pos = Float4_xyz(p4);

                ///Float3_log(pos);

                Matrix4x4_delete(matx);
                Matrix4x4_delete(maty);
                Matrix4x4_delete(mmm);

                PointLight_set_position(pl0, p0->x, p0->y, p0->z);
                PointLight_set_position(pl1, p1->x, p1->y, p1->z);
                PointLight_set_position(pl2, p2->x, p2->y, p2->z);
                PointLight_set_position(pl3, p3->x, p3->y, p3->z);

                PointLight_set_position(pl4, p4->x, p4->y, p4->z);
                PointLight_set_position(pl5, p5->x, p5->y, p5->z);
                PointLight_set_position(pl6, p6->x, p6->y, p6->z);
                PointLight_set_position(pl7, p7->x, p7->y, p7->z);

                p0 = Matrix4x4_mul_float4(g_mat, p0);
                p1 = Matrix4x4_mul_float4(g_mat, p1);
                p2 = Matrix4x4_mul_float4(g_mat, p2);
                p3 = Matrix4x4_mul_float4(g_mat, p3);

                p4 = Matrix4x4_mul_float4(g_mat, p4);
                p5 = Matrix4x4_mul_float4(g_mat, p5);
                p6 = Matrix4x4_mul_float4(g_mat, p6);
                p7 = Matrix4x4_mul_float4(g_mat, p7);

                Pipeline_render();
                SwapBuffers(hDC);
                g_fps++;

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
        Pipeline_Dest();
        Matrix4x4_delete(g_mat0);
        Matrix4x4_delete(g_mat1);
        Matrix4x4_delete(g_mat);
        VertexDeclaration_delete(g_dec);
        Float_Dest();
        Mlog();
        return 0;

    case WM_TIMER:
        printf("FPS:%d\n", g_fps);
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

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
