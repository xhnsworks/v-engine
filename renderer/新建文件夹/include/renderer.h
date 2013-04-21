#ifndef RENDERER_H
#define RENDERER_H
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
#include "material_prototype.h"
#include "material_instance.h"
#include "renderable.h"
#include "camera.h"
#include "matrix4x4.h"
#include "float_base.h"
#include "shader_node_generator.h"
#define MAX_SKETCHBOOKS 8
typedef renderer_param_value (*GetRendererParamProc)(Renderer);
typedef struct _renderer_param_entry
{
    GetRendererParamProc get_value_proc;
    param_type type;
    sint array_size;
} renderer_param_entry;
typedef struct _renderer_param_entry* RendererParamEntry;

typedef void (*DrawPointLightShapeProc)(PointLight);

API_EXPORT const char* _get_param_src_str(param_source _src);
API_EXPORT Renderer _Renderer_new(uint x, uint y, uint width, uint height, const char* _file, uint _line);
#define Renderer_new(x, y, w, h) _Renderer_new(x, y, w, h, __FILE__, __LINE__)
API_EXPORT Renderer _Renderer_new_from_prev_renderer(Renderer prev_renderer, const char* _file, uint _line);
#define Renderer_new_from_prev_renderer(p) _Renderer_new_from_prev_renderer(p, __FILE__, __LINE__)
API_EXPORT void Renderer_delete(Renderer _self);
///API_EXPORT PointLight Renderer_add_point_light(Renderer _self);
API_EXPORT LightBase2 Renderer_add_dir_light_2(Renderer _self);
API_EXPORT LightBase2 Renderer_add_spot_light_2(Renderer _self);
API_EXPORT void Renderer_register_material(Renderer _self, const char* mat_name, SDisplayProc disp_proc, draw_mode draw_mode, bool double_sided_flag);
API_EXPORT sint Renderer_get_material_id(Renderer _self, const char* mat_name);
///API_EXPORT void Renderer_append_renderable(Renderer _self, VertexBuffer _vbf, IndexBuffer _idf, uint32 _tri_cnt, MaterialInstance _mat);
API_EXPORT void Renderer_render(Renderer _self);
API_EXPORT void Renderer_clear_sketchbook(Renderer _self);
API_EXPORT Camera Renderer_get_camera(Renderer _self);
API_EXPORT Texture2D Renderer_get_shadow_map(Renderer _self, uint i);
API_EXPORT void Renderer_viewport_refresh(Renderer _self, uint x, uint y, uint width, uint height);

API_EXPORT Renderable _Renderer_new_renderable(Renderer _self, VertexDecl _dec, MaterialInstance _m_inst, mesh_mode _mesh_mode,
                                               const char* _file, uint _line);
#define Renderer_new_renderable(s, d, i, m) _Renderer_new_renderable(s, d, i, m, __FILE__, __LINE__)
API_EXPORT void Renderer_use_renderable(Renderer _self, Renderable _rbl);

API_EXPORT void Renderer_set_clear_color_buffer(Renderer _self, bool flag);
API_EXPORT void Renderer_set_clear_depth_buffer(Renderer _self, bool flag);

///API_EXPORT void Renderer_draw_point_light_shape(Renderer _self, DrawPointLightShapeProc proc);


API_EXPORT void __attribute__ ((force_align_arg_pointer)) Renderer_get_mouse_ray(Renderer _self, int x, int y, EFloat3* ori, EFloat3* dir);

API_EXPORT renderer_param_value Renderer_get_shader_object_value(Renderer _self, sint32 _src);
API_EXPORT void Renderer_register_renderer_param(Renderer _self, sint32 _id, param_type _type, sint _array_size, GetRendererParamProc _proc);
API_EXPORT bool Renderer_is_uniform_param_source(Renderer _self, sint32 _src);
API_EXPORT RendererParamEntry Renderer_get_param_entry(Renderer _self, sint32 _id);
API_EXPORT void shader_object_value_Dest(renderer_param_value _obj);
///============test begin==============///
/**
void Renderer_make_volume_renderable(Renderer _self, Renderable _rbl);
void Renderer_volume_renderable_prev_render(Renderable _rbl);
**/
///============test end================///
#endif
