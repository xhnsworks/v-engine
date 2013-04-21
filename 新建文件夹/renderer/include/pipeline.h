#ifndef PIPELINE_H
#define PIPELINE_H
#include "common.h"
#include "etypes.h"
#include "estring.h"
#include "vertex_element.h"
#include "pass.h"
#include "sketch.h"
#include "display_pass.h"
#include "lighting.h"
#include "lighting_pass.h"
#include "std_pass.h"
#include "vertex_buffer.h"
#include "std_material.h"
#include "camera.h"
#define MAX_SKETCHBOOKS 8

extern RenderablePlane g_render_plane;
extern uint32 g_width;
extern uint32 g_height;
extern Matrix4x4 g_proj_matrix;
extern Matrix4x4 g_inv_proj_matrix;
extern void Pipeline_Init();
extern void Pipeline_Dest();
extern PointLight Pipeline_add_point_light();
extern void Pipeline_append_renderable(VertexBuffer _vbf, IndexBuffer _idf, uint32 _tri_cnt, StdMaterial _mat);
extern void Pipeline_render();
extern Camera Pipeline_get_camera();
#endif
