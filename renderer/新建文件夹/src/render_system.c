#include "render_system.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <pthread.h>
#include "mem.h"
#include "elog.h"
#include "shader_log.h"
#include "float_base.h"
#include "gl_utility.h"
#include "tree.h"
#include "vertex_element.h"
#include "vertex_declaration.h"
#include "pass_declaration.h"
#include "pass.h"
#include "std_pass.h"
#include "image.h"
#include "png_load.h"
#include "shadow_emitter.h"

static Tree g_std_pass_tree = NULL;
static Tree g_vtx_dec_tree = NULL;
static Tree g_depth_pass_tree = NULL;
///static Tree g_shadow_pass_tree = NULL;
static Tree g_blur_pass_tree = NULL;
static Tree g_tex_tree = NULL;

static int g_window_width = 0;
static int g_window_height = 0;

Texture2D _create_texture_from_file(const char* _file_name)
{
    Image img = png_load(_file_name);

    if (to_ptr(img))
    {
        uint32 h = Image_get_num_rows(img);
        uint32 w = Image_get_row_width(img);
        pixel_format fmt = Image_get_pixel_format(img);
        Texture2D tex = Texture2D_new();
        vptr pxls = Image_get_row(img, 0);
        Texture2D_load_from_mem(tex, pxls, fmt, w, h, Image_get_size_in_byte(img), true);

        Image_delete(img);

        return tex;
    }

    return NULL;
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
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
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

void RenderSystem_Init(HWND h)
{
    RECT rc;
    GetWindowRect(h, &rc);

    g_window_width = rc.right - rc.left;
    g_window_height = rc.bottom - rc.top;

    MInit();
    ELog_Init();
    ShaderLog_Init();

    glewInit();
    ERROR_PROC;
    Float_Init();

    g_vtx_dec_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_data_compare_proc(g_vtx_dec_tree, VertexDecl_compare);

    g_depth_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_data_compare_proc(g_depth_pass_tree, PassDecl_compare);
/**
    g_shadow_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_data_compare_proc(g_shadow_pass_tree, VertexDecl_compare);
**/
    g_blur_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_data_compare_proc(g_blur_pass_tree, VertexDecl_compare);

    g_std_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_data_compare_proc(g_std_pass_tree, PassDecl_compare);

    g_tex_tree = Tree_new(String, Vptr, Ealloc, Efree);
}

void RenderSystem_Dest()
{
    Iterator iter = Tree_begin(g_vtx_dec_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        VertexDecl_delete(data.vptr_var);
        iter = Tree_next(iter);
    }
    Tree_Dest(g_vtx_dec_tree);

    iter = Tree_begin(g_depth_pass_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        Pass_delete(data.vptr_var);
        iter = Tree_next(iter);
    }
    Tree_Dest(g_depth_pass_tree);
/**
    iter = Tree_begin(g_shadow_pass_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        Pass_delete(data.vptr_var);
        iter = Tree_next(iter);
    }
    Tree_Dest(g_shadow_pass_tree);
**/
    iter = Tree_begin(g_std_pass_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        Pass_delete(data.vptr_var);
        iter = Tree_next(iter);
    }
    Tree_Dest(g_std_pass_tree);

    iter = Tree_begin(g_tex_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        Texture2D_delete(data.vptr_var);
        iter = Tree_next(iter);
    }
    Tree_Dest(g_tex_tree);
}

void gl_Init(void)
{
    ERROR_PROC;
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    ERROR_PROC;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
    // enable /disable features
    ERROR_PROC;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ERROR_PROC;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ERROR_PROC;
    glEnable(GL_BLEND);
    ERROR_PROC;
    glAlphaFunc(GL_GREATER,0.0);
    ERROR_PROC;
    glEnable(GL_ALPHA_TEST);
    ERROR_PROC;
    glEnable(GL_DEPTH_TEST);
    ERROR_PROC;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ERROR_PROC;
    glClearStencil(0);
    ERROR_PROC;
    glClearDepth(1.0);
    ERROR_PROC;
    glDepthFunc(GL_LEQUAL);
    ERROR_PROC;
    /// ATI should be set like this
    ///glDepthFunc(GL_ALWAYS);
    ERROR_PROC;
    glEnable(GL_TEXTURE_2D);
    ERROR_PROC;
    glEnable(GL_CULL_FACE);
    ERROR_PROC;
    glDepthMask(GL_TRUE);
    ERROR_PROC;
}

void RenderSystem_SwapBuffers(HDC hDC)
{
    SwapBuffers(hDC);
}

VertexDecl RenderSystem_register_vertex_declaration(VertexDecl dec)
{
    var key, data;
    key.vptr_var = dec;

    Iterator iter = Tree_find(g_vtx_dec_tree, key, &data);
    if (iter)
        return data.vptr_var;
    else
    {
        VertexDecl ret = VertexDecl_clone(dec);
        key.vptr_var = data.vptr_var = ret;
        Tree_insert(g_vtx_dec_tree, key, data);
        return ret;
    }
}

Pass RenderSystem_register_depth_pass(PassDecl dec)
{
    var key, data;
    key.vptr_var = dec;

    Iterator iter = Tree_find(g_depth_pass_tree, key, &data);
    if (iter)
    {
        Pass ret = {data.vptr_var};
        return ret;
    }
    else
    {
        PassDecl tmp = PassDecl_clone(dec);
        key.vptr_var = tmp;

        std_pass_status status;
        ///status.material_id = Renderer_get_material_id(_renderer, mat_name);
        ///status.has_color_map = has_col_map;
        ///status.has_normal_map = has_nor_map;
        memcpy(&status.dec, tmp, sizeof(pass_decl));
        status.use_proj_mat = true;
        status.use_world_mat = true;
        status.use_gausscian_blur = false;

        status.num_render_pipes = 1;

        status.render_pipes[0].out = VSMDepthOutput;
        status.render_pipes[0].tgt = RenderTarget0;

        Pass ret = create_std_pass_from_dec(&status, true);
        data.vptr_var = to_ptr(ret);

        Tree_insert(g_depth_pass_tree, key, data);
        Mfree(tmp);
        return ret;
    }
}

Pass RenderSystem_register_blur_pass(VertexDecl dec)
{
    var key, data;
    key.vptr_var = dec;

    Iterator iter = Tree_find(g_blur_pass_tree, key, &data);
    if (iter)
    {
        Pass ret = {data.vptr_var};
        return ret;
    }
    else
    {
        pass_decl pd;
        pd.dec = dec;
        pd.usage_tex_flags = COLOR_SKETCH_MASK;
        PassDecl tmp = PassDecl_clone(&pd);

        std_pass_status status;
        memcpy(&status.dec, tmp, sizeof(pass_decl));
        status.use_proj_mat = false;
        status.use_world_mat = false;
        status.use_gausscian_blur = true;

        status.num_render_pipes = 1;

        status.render_pipes[0].out = ColorOutput;
        status.render_pipes[0].tgt = RenderTarget0;

        Pass ret = create_std_pass_from_dec(&status, true);
        data.vptr_var = to_ptr(ret);

        Tree_insert(g_blur_pass_tree, key, data);
        Mfree(tmp);
        return ret;
    }
}

Pass RenderSystem_register_std_pass(PassDecl dec)
{
    var key, data;
    key.vptr_var = dec;

    Iterator iter = Tree_find(g_std_pass_tree, key, &data);
    if (iter)
    {
        Pass ret = {data.vptr_var};
        return ret;
    }
    else
    {
        PassDecl tmp = PassDecl_clone(dec);
        key.vptr_var = tmp;

        std_pass_status status;
        ///status.material_id = Renderer_get_material_id(_renderer, mat_name);
        ///status.has_color_map = has_col_map;
        ///status.has_normal_map = has_nor_map;
        memcpy(&status.dec, tmp, sizeof(pass_decl));
        status.use_proj_mat = true;
        status.use_world_mat = true;
        status.use_gausscian_blur = false;

        status.num_render_pipes = 3;

        status.render_pipes[MATERIAL_ID_INDEX].out = MaterialIDOutput;
        status.render_pipes[BASE_COLOR_INDEX].out = ColorOutput;
        status.render_pipes[NORMAL_SPECULAR_INDEX].out = NormalSpecularOutput;

        status.render_pipes[0].tgt = RenderTarget0;
        status.render_pipes[1].tgt = RenderTarget1;
        status.render_pipes[2].tgt = RenderTarget2;

        Pass ret = create_std_pass_from_dec(&status, true);
        data.vptr_var = to_ptr(ret);

        Tree_insert(g_std_pass_tree, key, data);
        Mfree(tmp);
        return ret;
    }
}

Texture2D RenderSystem_register_texture2d(const char* filename)
{
    var key, data;
    key.str_var = filename;
    if (Tree_find(g_tex_tree, key, &data))
    {
        return data.vptr_var;
    }
    else
    {
        Texture2D tex = _create_texture_from_file(filename);
        data.vptr_var = tex;
        Tree_insert(g_tex_tree, key, data);
        return tex;
    }
}

void RenderSystem_get_window_width_height(int* w, int* h)
{
    *w = g_window_width;
    *h = g_window_height;
}
