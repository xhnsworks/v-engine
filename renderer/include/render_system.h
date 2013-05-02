#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#include "common.h"
#include "etypes.h"
#include "pass_declaration.h"
#include "pass.h"
#include "xml_resource.h"
#include "xhn_static_string.hpp"
#include "texture2d.h"
#define USE_DS_RENDER         true
#define MATERIAL_ID_INDEX     1
#define BASE_COLOR_INDEX      0
#define NORMAL_SPECULAR_INDEX 2

#define MATERIAL_ID_FORMAL     RGBA8
#define BASE_COLOR_FORMAT      RGBA8
#define NORMAL_SPECULAR_FORMAT RGBA16

#define DIFFUSE_INDEX          0
#define SPECULAR_INDEX         1

#define DIFFUSE_FORMAT         RGB8
#define SPECULAR_FORMAT        RGB8

typedef enum _depth_pass_type
{
    NormalDepthPass,
    PointDepthPass
} depth_pass_type;
#if defined(_WIN32) || defined(_WIN64)
API_EXPORT void RenderSystem_Init(HWND h);
API_EXPORT void RenderSystem_Dest();
API_EXPORT void RenderSystem_SwapBuffers(HDC hDC);
API_EXPORT void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC);
API_EXPORT void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC);
#else
API_EXPORT void RenderSystem_Init(euint32 viewWidth, euint32 viewHeight);
API_EXPORT void RenderSystem_Dest();
#endif
API_EXPORT void gl_Init(void);
API_EXPORT VertexDecl RenderSystem_register_vertex_declaration(VertexDecl dec);
API_EXPORT Pass RenderSystem_register_depth_pass(PassDecl dec, depth_pass_type type);
API_EXPORT Pass RenderSystem_register_blur_pass(VertexDecl dec);
API_EXPORT Pass RenderSystem_register_std_pass(PassDecl dec);

API_EXPORT Texture2DPtr RenderSystem_register_texture2d(const xhn::static_string filename, const xhn::static_string res_group);
API_EXPORT Texture2DPtr RenderSystem_new_texture2d(const xhn::static_string filename, const xhn::static_string res_group);
API_EXPORT Texture2DPtr RenderSystem_get_texture2d(const xhn::static_string filename, const xhn::static_string res_group);
API_EXPORT XMLResourcePtr RenderSystem_load_gui_config(const xhn::static_string filename);

API_EXPORT void RenderSystem_get_window_width_height(int* w, int* h);

#endif
