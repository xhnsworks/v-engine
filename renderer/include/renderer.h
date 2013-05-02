/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：renderer.h
* 文件描述：延迟着色渲染器对象
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef RENDERER_H
#define RENDERER_H
#include "common.h"
#include "etypes.h"
#include "emem.h"
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
#include "shadow_renderer.h"
#include "renderer_base.h"

#define MAX_SKETCHBOOKS 8
#define NUM_LIGHTING_SKETCH_BOOKS 2

typedef struct _renderer_param_entry {
    GetRendererParamProc get_value_proc;
    param_type type;
    esint array_size;
} renderer_param_entry;

typedef void ( *DrawPointLightShapeProc ) ( PointLight );

class Renderer;
class light_prototype : public MemObject
{
public:
    Tree light_tree;
    Pass first_lighting_pass;
    Pass lighting_pass;
public:
    light_prototype();
    ~light_prototype();
    void clear_passes();
    void register_light ( LightBase2 lt );
    virtual LightBase2 new_light() = 0;
    virtual void prepare_lighting_passes ( Renderer *rdr, VertexDecl vtx_decl, ARRAY lighting_decl *decls ) = 0;
};

class dir_light_prototype : public light_prototype
{
public:
    virtual LightBase2 new_light();
    virtual void prepare_lighting_passes ( Renderer *rdr, VertexDecl vtx_decl, ARRAY lighting_decl *decls );
};

class spot_light_prototype : public light_prototype
{
public:
    virtual LightBase2 new_light();
    virtual void prepare_lighting_passes ( Renderer *rdr, VertexDecl vtx_decl, ARRAY lighting_decl *decls );
};

class point_light_prototype : public light_prototype
{
public:
    virtual LightBase2 new_light();
    virtual void prepare_lighting_passes ( Renderer *rdr, VertexDecl vtx_decl, ARRAY lighting_decl *decls );
};

API_EXPORT class Renderer : public RendererBase
{
    friend class light_prototype;
private:
    Renderer *prev_renderer;
	DebugRenderOutput debug_render_output;

    SketchBook std_sketch_book;
    SketchBook lighting_sketch_book_array[NUM_LIGHTING_SKETCH_BOOKS];

    ShadowRenderer shadow_state;

    dir_light_prototype dir_light_ptype;
    spot_light_prototype spot_light_ptype;
    point_light_prototype point_light_ptype;

    SketchBook prev_render_skb;
    SketchBook prev_lighting_skb;

    bool dirty_flag;

    bool clear_color_buffer_flag;
    bool clear_depth_buffer_flag;

    bool use_deferred_shading;
public:
    void common_init();
    Renderer ( ViewportPtr view );
    Renderer ( Renderer *prev_renderer );
	~Renderer();
    void RendererDest();

    void clear_sketchbook();
private:
    void prepare_sketchbooks();
    void prepare_display_passes();
    void prepare_lighting_passes();
    void render_std_passes();
	void lighting_render ( Pass lighting_pass, SketchBook curt_skb );

    void prepare_shadow_maps ( LightBase2 pl, light_prototype *prototype );
    void shadow_render ( Renderable rbl, SketchBook curt_skb, sketch_type shadow_type );
    void all_lighting_render ( light_prototype &prototype, SketchBook curt_lighting_skb, euint *light_count );
	void prepare_renderable(Renderable _rbl);

	void lighting();

    esint get_material_id ( const char *mat_name );
protected:
    virtual void set_pre_lighting_state() {}
public:
    virtual void render();
    void register_material ( const char *mat_name, SDisplayProc disp_proc, e_draw_mode draw_mode, bool double_sided_flag );
    Renderable new_renderable ( VertexDecl _dec, MaterialInstance _m_inst, e_mesh_mode _mesh_mode );
    void viewport_refresh ( euint _x, euint _y, euint _width, euint _height );
   
    inline LightBase2 add_dir_light_2() {
        return dir_light_ptype.new_light();
    }
    inline LightBase2 add_spot_light_2() {
        return spot_light_ptype.new_light();
    }
    inline LightBase2 add_point_light_2() {
        return point_light_ptype.new_light();
    }
	inline void set_clear_color_buffer ( bool flag ) {
		clear_color_buffer_flag = flag;
	}
	inline void set_clear_depth_buffer ( bool flag ) {
		clear_depth_buffer_flag = flag;
	}
	inline Camera get_camera() {
		return curt_render_cam;
	}
	inline void set_deferred_shading(bool enable) {
		use_deferred_shading = enable;
	}

    renderer_param_value get_shader_object_value ( esint32 _src );
	renderer_param_value get_shader_object_value ( RendererBase* rdr, esint32 _src );
    void register_renderer_param ( esint32 _id, param_type _type, esint _array_size, GetRendererParamProc _proc );
    bool is_uniform_param_source ( esint32 _src );
    RendererParamEntry get_param_entry ( esint32 _id );

	virtual Texture2DPtr get_shadow_map ( euint i ) {
        return shadow_state.get_shadow_map ( i );
    }
    virtual TextureCube get_shadow_cube_map() {
        return shadow_state.get_shadow_cube_map();
    }
	void set_debug_output(DebugRenderOutput dro);
};

API_EXPORT const char *_get_param_src_str ( param_source _src );
API_EXPORT void shader_object_value_Dest ( renderer_param_value _obj );

#endif
