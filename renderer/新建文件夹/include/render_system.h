#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#include "etypes.h"
#include "common.h"
#include <windows.h>
#include "pass_declaration.h"
#include "pass.h"

#define USE_DS_RENDER         true
#define MATERIAL_ID_INDEX     0
#define BASE_COLOR_INDEX      1
#define NORMAL_SPECULAR_INDEX 2

#define MATERIAL_ID_FORMAL     RGBA8
#define BASE_COLOR_FORMAT      RGBA8
#define NORMAL_SPECULAR_FORMAT RGBA16

#define DIFFUSE_INDEX          0
#define SPECULAR_INDEX         1

#define DIFFUSE_FORMAT         RGB8
#define SPECULAR_FORMAT        RGB8

API_EXPORT void RenderSystem_Init(HWND h);
API_EXPORT void RenderSystem_Dest();
API_EXPORT void RenderSystem_SwapBuffers(HDC hDC);
API_EXPORT void gl_Init(void);
API_EXPORT void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC);
API_EXPORT void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC);

API_EXPORT VertexDecl RenderSystem_register_vertex_declaration(VertexDecl dec);
///API_EXPORT Pass RenderSystem_register_shadow_pass(VertexDecl dec);
API_EXPORT Pass RenderSystem_register_depth_pass(PassDecl dec);
API_EXPORT Pass RenderSystem_register_blur_pass(VertexDecl dec);
API_EXPORT Pass RenderSystem_register_std_pass(PassDecl dec);

API_EXPORT Texture2D RenderSystem_register_texture2d(const char* filename);

API_EXPORT void RenderSystem_get_window_width_height(int* w, int* h);

#endif
