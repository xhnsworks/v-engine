#include "pch.h"
#include "render_system.h"
///#include <GL/gl.h>
#include <pthread.h>
#include "emem.h"
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

#include "resource_group.h"
#include "texture2d_resource.h"
#include "xml_resource.h"

#include "xhn_static_string.hpp"
#include "xhn_set.hpp"

#include "font_renderer.h"

static Tree g_std_pass_tree = NULL;
static Tree g_vtx_dec_tree = NULL;
static Tree g_depth_pass_tree = NULL;
static Tree g_point_depth_pass_tree = NULL;
///static Tree g_shadow_pass_tree = NULL;
static Tree g_blur_pass_tree = NULL;
///static Tree g_tex_tree = NULL;

static xhn::map<ResourceID, Texture2DPtr> g_tex_map;

static int g_window_width = 0;
static int g_window_height = 0;

static ResourceSystem* g_resource_system = NULL;

Texture2DPtr _create_texture_from_file(FILE* file_stream)
{
    Image img = png_load(file_stream);

    if (to_ptr(img))
    {
        euint32 h = Image_get_num_rows(img);
        euint32 w = Image_get_row_width(img);
        pixel_format fmt = Image_get_pixel_format(img);
        Texture2DPtr tex = ENEW Texture2D;
        vptr pxls = Image_get_row(img, 0);
        tex->LoadFromMem(pxls, fmt, w, h, Image_get_size_in_byte(img), true);

        Image_delete(img);

        return tex;
    }

    return NULL;
}

Texture2DPtr _create_texture_from_file(const char* _file_name)
{
    FILE* file_stream = SafeFOpen(_file_name, "rb");
	return _create_texture_from_file(file_stream);
}
#if defined(_WIN32) || defined(_WIN64)
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
#pragma optimize( "", off )
void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
#pragma optimize( "", on )

void RenderSystem_Init(HWND h)
{
    RECT rc;
    GetWindowRect(h, &rc);

    g_window_width = rc.right - rc.left;
    g_window_height = rc.bottom - rc.top;

    glewInit();
    ERROR_PROC;
    Float_Init();

    g_vtx_dec_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_vtx_dec_tree, (KEY_COMPARE)VertexDecl_compare);

    g_depth_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_depth_pass_tree, (KEY_COMPARE)PassDecl_compare);

    g_point_depth_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_depth_pass_tree, (KEY_COMPARE)PassDecl_compare);
/**
    g_shadow_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_data_compare_proc(g_shadow_pass_tree, VertexDecl_compare);
**/
    g_blur_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_blur_pass_tree, (KEY_COMPARE)VertexDecl_compare);

    g_std_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_std_pass_tree, (KEY_COMPARE)PassDecl_compare);

    ///g_tex_tree = Tree_new(String, Vptr, Ealloc, Efree);

	g_resource_system = ENEW ResourceSystem;
	
	ResourceGroup* grp = g_resource_system->GetResourceGroup("BaseGroup");
	grp->RegisterResourceDirectory("..\\test_scene", Public);
	g_resource_system->NewResourceGroup("Texture", "BaseGroup", Public);
	g_resource_system->NewResourceGroup("GUIConfig", "BaseGroup", Public);
	///g_resource_system->NewResourceGroup("FontTexture", "BaseGroup", Public);
	grp = g_resource_system->GetResourceGroup("Texture");
	grp->RegisterResourceImplement(ENEW Texture2DImplement);
	grp->RegisterResourceImplement(ENEW DefaultTexture2DImplement);
    grp = g_resource_system->GetResourceGroup("GUIConfig");
	grp->RegisterResourceImplement(ENEW XMLImplement);
	/**
	grp = g_resource_system->GetResourceGroup("FontTexture");
	grp->RegisterResourceImplement(ENEW Texture2DImplement);
	**/
/**
	FontRenderer* fr = ENEW FontRenderer("..\\test_scene\\Earthbound-Condensed-Bold.otf");
	ComposingStick* cs = ENEW ComposingStick(fr, 256);
	for (wchar_t i = 0; i < 128; i++)
	{
		cs->AllocGlyph(i);
	}
**/
}

void RenderSystem_SwapBuffers(HDC hDC)
{
    SwapBuffers(hDC);
}
#else
void RenderSystem_Init(euint32 viewWidth, euint32 viewHeight)
{
    ///RECT rc;
    ///GetWindowRect(h, &rc);
    
    g_window_width = viewWidth;
    g_window_height = viewHeight;
    
    ERROR_PROC;
    Float_Init();
    
    g_vtx_dec_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_vtx_dec_tree, (KEY_COMPARE)VertexDecl_compare);
    
    g_depth_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_depth_pass_tree, (KEY_COMPARE)PassDecl_compare);
    
    g_point_depth_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_depth_pass_tree, (KEY_COMPARE)PassDecl_compare);
    /**
     g_shadow_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
     Tree_set_data_compare_proc(g_shadow_pass_tree, VertexDecl_compare);
     **/
    g_blur_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_blur_pass_tree, (KEY_COMPARE)VertexDecl_compare);
    
    g_std_pass_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Tree_set_key_compare_proc(g_std_pass_tree, (KEY_COMPARE)PassDecl_compare);
    
    ///g_tex_tree = Tree_new(String, Vptr, Ealloc, Efree);
    
	g_resource_system = ENEW ResourceSystem;
	
	ResourceGroup* grp = g_resource_system->GetResourceGroup("BaseGroup");
#if defined(_WIN32) || defined(_WIN64)
	grp->RegisterResourceDirectory("..\\test_scene", Public);
#elif defined(__APPLE__)
    grp->RegisterResourceDirectory("../../../../../test_scene", Public);
#endif
	g_resource_system->NewResourceGroup("Texture", "BaseGroup", Public);
	g_resource_system->NewResourceGroup("GUIConfig", "BaseGroup", Public);
	///g_resource_system->NewResourceGroup("FontTexture", "BaseGroup", Public);
	grp = g_resource_system->GetResourceGroup("Texture");
	grp->RegisterResourceImplement(ENEW Texture2DImplement);
	grp->RegisterResourceImplement(ENEW DefaultTexture2DImplement);
    grp = g_resource_system->GetResourceGroup("GUIConfig");
	grp->RegisterResourceImplement(ENEW XMLImplement);
}
#endif
void RenderSystem_Dest()
{
    Iterator iter = Tree_begin(g_vtx_dec_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        VertexDecl_delete((VertexDecl)data.vptr_var);
        iter = Tree_next(iter);
    }
    Tree_Dest(g_vtx_dec_tree);
    
    iter = Tree_begin(g_depth_pass_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        Pass_delete((Pass)data.vptr_var);
        iter = Tree_next(iter);
    }
    Tree_Dest(g_depth_pass_tree);
    
    iter = Tree_begin(g_point_depth_pass_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        Pass_delete((Pass)data.vptr_var);
        iter = Tree_next(iter);
    }
    Tree_Dest(g_point_depth_pass_tree);
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
        Pass_delete((Pass)data.vptr_var);
        iter = Tree_next(iter);
    }
    Tree_Dest(g_std_pass_tree);
    /**
     iter = Tree_begin(g_tex_tree);
     while (iter)
     {
     var data = Tree_get_value(iter);
     Texture2D_delete((Texture2D)data.vptr_var);
     iter = Tree_next(iter);
     }
     Tree_Dest(g_tex_tree);
     **/
	g_tex_map.clear();
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
    glEnable(GL_TEXTURE_CUBE_MAP);
    ERROR_PROC;
    glEnable(GL_CULL_FACE);
    ERROR_PROC;
    glDepthMask(GL_TRUE);
    ERROR_PROC;
}
VertexDecl RenderSystem_register_vertex_declaration(VertexDecl dec)
{
    var key, data;
    key.vptr_var = dec;

    Iterator iter = Tree_find(g_vtx_dec_tree, key, &data);
    if (iter)
        return (VertexDecl)data.vptr_var;
    else
    {
        VertexDecl ret = VertexDecl_clone(dec);
        key.vptr_var = data.vptr_var = ret;
        Tree_insert(g_vtx_dec_tree, key, data);
        return ret;
    }
}

Pass RenderSystem_register_depth_pass(PassDecl dec, depth_pass_type type)
{
    var key, data;
    key.vptr_var = dec;
    Tree depth_pass_tree = NULL;
    switch (type)
    {
    case NormalDepthPass:
        depth_pass_tree = g_depth_pass_tree;
        break;
    case PointDepthPass:
        depth_pass_tree = g_point_depth_pass_tree;
        break;
    }

    Iterator iter = Tree_find(depth_pass_tree, key, &data);
    if (iter)
    {
        Pass ret = {(Pass)data.vptr_var};
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
        switch (type)
        {
        case NormalDepthPass:
            status.render_pipes[0].out = VSMDepthOutput;
            break;
        case PointDepthPass:
            status.render_pipes[0].out = PointVSMDepthOutput;
            break;
        }
        status.render_pipes[0].tgt = RenderTarget0;

        Pass ret = create_std_pass_from_dec(&status, true);
        data.vptr_var = to_ptr(ret);

        Tree_insert(depth_pass_tree, key, data);
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
        Pass ret = {(Pass)data.vptr_var};
        return ret;
    }
    else
    {
        pass_decl pd;
        pd.dec = dec;
        pd.usage_tex_flags = COLOR_SKETCH_MASK;
		pd.vtx_color_func = Discard;
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
        Pass ret = (Pass)data.vptr_var;
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

Texture2DPtr RenderSystem_register_texture2d(const xhn::static_string filename, const xhn::static_string res_group)
{
	ResourceID id = xhn::make_pair(filename, res_group);
	xhn::map<ResourceID, Texture2DPtr>::iterator iter = g_tex_map.find(id);
	if (iter != g_tex_map.end())
		return iter->second;
	else {
		ResourceGroup* resGrp = g_resource_system->GetResourceGroup(res_group);
		ResourcePtr res = resGrp->Load(filename);
		if (TestResourcePtr(res)) {
			Texture2DResource* tex_res = res->DynamicCast<Texture2DResource>();
			Texture2DPtr tex = tex_res->GetTexture();
			g_tex_map.insert(xhn::make_pair(id, tex));
			return tex;
		}
		else
			return NULL;
	}
}

Texture2DPtr RenderSystem_new_texture2d(const xhn::static_string filename, const xhn::static_string res_group)
{
	ResourceID id = xhn::make_pair(filename, res_group);
	xhn::map<ResourceID, Texture2DPtr>::iterator iter = g_tex_map.find(id);
	if (iter != g_tex_map.end())
		return iter->second;
	else {
		ResourceGroup* resGrp = g_resource_system->GetResourceGroup(res_group);
		ResourcePtr res = resGrp->New(filename);
		if (TestResourcePtr(res)) {
			Texture2DResource* tex_res = res->DynamicCast<Texture2DResource>();
			Texture2DPtr tex = tex_res->GetTexture();
			g_tex_map.insert(xhn::make_pair(id, tex));
			return tex;
		}
		else
			return NULL;
	}
}

Texture2DPtr RenderSystem_get_texture2d(const xhn::static_string filename, const xhn::static_string res_group)
{
	ResourceID id = xhn::make_pair(filename, res_group);
	xhn::map<ResourceID, Texture2DPtr>::iterator iter = g_tex_map.find(id);
	if (iter != g_tex_map.end())
		return iter->second;
	else
        return NULL;
}

XMLResourcePtr RenderSystem_load_gui_config(const xhn::static_string filename)
{
	ResourceGroup* resGrp = g_resource_system->GetResourceGroup("GUIConfig");
	ResourcePtr res = resGrp->Load(filename);

	XMLResourcePtr ret(res->DynamicCast<XMLResource>());
	return ret;
}

void RenderSystem_get_window_width_height(int* w, int* h)
{
    *w = g_window_width;
    *h = g_window_height;
}
