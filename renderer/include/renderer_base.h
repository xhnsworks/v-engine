#ifndef RENDERER_BASE_H
#define RENDERER_BASE_H
#include "common.h"
#include "etypes.h"
#include "emem.hpp"
#include "sketch.h"
#include "matrix4x4.h"
#include "pass.h"
#include "camera.h"
#include "material_prototype.h"
#include "lighting.h"
#include "renderable.h"
#include "xhn_set.hpp"

struct Viewport : public RefObject
{
	euint x;
	euint y;
    euint width;
	euint height;
	xhn::SmartPtr<Viewport> parent;
	Viewport()
		: x(0)
		, y(0)
		, width(1024)
		, height(768)
	{}
};
typedef xhn::SmartPtr<Viewport> ViewportPtr;
class API_EXPORT RenderableSorter : public RefObject
{
public:
	virtual void Sort(STD_NAMESPACE::set<Renderable>& used_renderable_set, STD_NAMESPACE::list<Renderable>& sorted_renderable_list) = 0;
};
typedef xhn::SmartPtr<RenderableSorter> RenderableSorterPtr;
class DefaultRenderableSorter : public RenderableSorter
{
public:
	virtual ~DefaultRenderableSorter() {}
    virtual void Sort(STD_NAMESPACE::set<Renderable>& used_renderable_set, STD_NAMESPACE::list<Renderable>& sorted_renderable_list);
};
class API_EXPORT RendererBase : public MemObject
{
protected:
    RenderablePlane render_plane;
	matrix4x4* default_rend_matrix;

	Tree param_proc_tree;

	Tree material_table;
    esint material_count;

	STD_NAMESPACE::set<Renderable> used_renderable_set;
	STD_NAMESPACE::set<Renderable> unused_renderable_set;
	STD_NAMESPACE::list<Renderable> sorted_renderable_list;

	RenderableSorterPtr renderable_sorter;

    Pass display_pass;
    Pass clear_sketchbook_pass;

    euint x;
    euint y;
    euint width;
    euint height;
    Camera camera_base;
public:
    matrix4x4* curt_rend_world_matrix;

    MaterialInstance curt_mat_inst;
    MaterialPrototype curt_mat_proto;

    Texture2DPtr curt_plaster;
    Texture2DPtr curt_color_sketch;
    Texture2DPtr curt_normal_sketch;
    Texture2DPtr curt_lighting_sketch;
    Texture2DPtr curt_material_id_sketch;

    Texture2DPtr curt_diffuse_lighting_sketch;
    Texture2DPtr curt_specular_lighting_sketch;

    LightBase2 curt_light;
public:
    Camera curt_render_cam;
private:
    bool own_camera;
	bool own_material_table;
private:
	void _renderable_tree_dest( Tree _t );
	void _renderable_set_dest ( STD_NAMESPACE::set<Renderable>& _s );
protected:
    RendererBase ( ViewportPtr view, RenderableSorter* sorter = NULL );
    RendererBase ( RendererBase *prev_renderer, bool inherit_material_table = false, RenderableSorter* sorter = NULL );
	void CommonInit();
    void Dest();
public:
    Renderable new_renderable ( VertexDecl _dec, MaterialInstance _m_inst, e_mesh_mode _mesh_mode );
    void use_renderable ( Renderable _rbl );

	void FORCE_ALIGN_ARG_POINTER get_mouse_ray ( int _x, int _y, EFloat3 *ori, EFloat3 *dir );
    
    virtual bool is_uniform_param_source ( sint32 _src ) = 0;
	virtual Texture2DPtr get_shadow_map ( euint i ) = 0;
    virtual TextureCube get_shadow_cube_map() = 0;
	virtual renderer_param_value get_shader_object_value ( sint32 _src ) = 0;
	virtual void prepare_renderable(Renderable _rbl) = 0;

	inline euint get_width() {
		return width;
	}
	inline euint get_height() {
		return height;
	}
    inline RenderablePlane get_render_plane() {
        return render_plane;
    }
};
typedef struct _renderer_param_entry *RendererParamEntry;
typedef renderer_param_value ( *GetRendererParamProc ) ( RendererBase * );
API_EXPORT RendererParamEntry RendererParamEntry_new ( GetRendererParamProc _proc, param_type _type, esint _array_size );
API_EXPORT void RendererParamEntry_delete ( RendererParamEntry _self );
#endif