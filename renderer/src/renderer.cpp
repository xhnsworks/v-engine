#include "pch.h"
#include "renderer.h"
#include "emem.h"
#include "array.h"
#include "camera.h"
#include "sketch.h"
#include "pass_console.h"
#include "gl_utility.h"
#include "shader_buffer.h"
#include "renderable_base.h"
#include "shader_node_generator.h"

#include "tree.h"
#include "math_base.h"
#include "sfloat3.h"
#include "sfloat4.h"

#include "shader_test.h"
#include "shader_object.h"
#include "exception.h"

#include "render_system.h"
#include "lighting_model.h"

#include "clear_sketchbook_pass.h"
#include "gl_lock.h"

/** \brief LightBase树的销毁
 *
 * \param LightBase树
 * \param
 * \return 无返回值
 *
 */
void _light_base_2_tree_dest ( Tree _t )
{
    Iterator iter = Tree_begin ( _t );

    while ( iter ) {
        var data;
        data = Tree_get_value ( iter );
        LightBase2 pt = ( LightBase2 ) data.vptr_var;
        ///LightBase2_delete(pt);
        light_base_2::Delete ( pt );
        iter = Tree_next ( iter );
    }

    Tree_Dest ( _t );
}
/** \brief 在每帧的渲染之前执行的状态
 *
 * \param
 * \param
 * \return 无返回值
 *
 */
light_prototype::light_prototype()
    : first_lighting_pass ( NULL )
    , lighting_pass ( NULL )
{
    light_tree = Tree_new ( Vptr, Vptr, (MALLOC)Ealloc, (MFREE)Efree );
}
light_prototype::~light_prototype()
{
    _light_base_2_tree_dest ( light_tree );
    clear_passes();
}
void light_prototype::clear_passes()
{
    if ( first_lighting_pass ) {
        Pass_delete ( first_lighting_pass );
        first_lighting_pass = NULL;
    }

    if ( lighting_pass ) {
        Pass_delete ( lighting_pass );
        lighting_pass = NULL;
    }
}
void light_prototype::register_light ( LightBase2 lt )
{
    var key, data;
    key.vptr_var = lt;
    data.vptr_var = lt;
    Tree_insert ( light_tree, key, data );
}
LightBase2 dir_light_prototype::new_light()
{
    LightBase2 ret = direction_light::New();
    register_light ( ret );
    return ret;
}
LightBase2 spot_light_prototype::new_light()
{
    LightBase2 ret = spot_light::New();
    register_light ( ret );
    return ret;
}
LightBase2 point_light_prototype::new_light()
{
    LightBase2 ret = point_light::New();
    register_light ( ret );
    return ret;
}
void dir_light_prototype::prepare_lighting_passes ( Renderer *rdr, VertexDecl vtx_decl, ARRAY lighting_decl *decls )
{
    light_state state = {DirectionType, DirectionLightingNode, 1};
    first_lighting_pass = create_lighting_pass_ex3 ( rdr, vtx_decl, &state, decls, false );
    lighting_pass = create_lighting_pass_ex3 ( rdr, vtx_decl, &state, decls, true );
}
void spot_light_prototype::prepare_lighting_passes ( Renderer *rdr, VertexDecl vtx_decl, ARRAY lighting_decl *decls )
{
    light_state state = {SpotType, SpotLightingNode, 1};
    first_lighting_pass = create_lighting_pass_ex3 ( rdr, vtx_decl, &state, decls, false );
    lighting_pass = create_lighting_pass_ex3 ( rdr, vtx_decl, &state, decls, true );
}
void point_light_prototype::prepare_lighting_passes ( Renderer *rdr, VertexDecl vtx_decl, ARRAY lighting_decl *decls )
{
    light_state state = {PointType, PointLightingNode, 1};
    first_lighting_pass = create_lighting_pass_ex3 ( rdr, vtx_decl, &state, decls, false );
    lighting_pass = create_lighting_pass_ex3 ( rdr, vtx_decl, &state, decls, true );
}

void _render_one_frame_begin()
{
    GLLock::Get()->Lock();
    glShadeModel ( GL_SMOOTH );                 // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 4 );   // 4-byte pixel alignment
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable ( GL_BLEND );
    glEnable ( GL_ALPHA_TEST );
    glEnable ( GL_DEPTH_TEST );
    glEnable ( GL_TEXTURE_2D );
    glEnable ( GL_TEXTURE_CUBE_MAP );
    glDepthMask ( GL_TRUE );
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth ( 1.0 );
    glClearStencil ( 0 );
    glDepthFunc ( GL_LEQUAL );
    glCullFace ( GL_BACK );
    ERROR_PROC;
    GLLock::Get()->Unlock();
}

/** \brief 生成材质的Proc函数
 *
 * \param _psb 用于生成材质的像素着色器缓冲
 * \param _id 材质id
 * \return 材质对应的着色器节点
 *
 */

const char *_get_param_src_str ( param_source _src )
{
    switch ( _src ) {
    case EmptySrc:
        return "EmptySrc";

    case VaryWorldPosition:
        return "VaryWorldPosition";

    case VaryPosition:
        return "VaryPosition";

    case VaryTexCoord:
        return "VaryTexCoord";

    case VaryNormal:
        return "VaryNormal";

    case VaryTangent:
        return "VaryTangent";

    case VaryBinormal:
        return "VaryBinormal";

    case CameraPosition:
        return "CameraPosition";

    case CameraDirection:
        return "CameraDirection";

    case CameraPlaneNear:
        return "CameraPlaneNear";

    case CameraPlaneFar:
        return "CameraPlaneFar";

    case CameraPlaneWidth:
        return "CameraPlaneWidth";

    case CameraPlaneHeight:
        return "CameraPlaneHeight";

    case CameraProjectionMatrix:
        return "CameraProjectionMatrix";

    case CameraWorldMatrix:
        return "CameraWorldMatrix";

    case InvertCameraProjectionMatrix:
        return "InvertCameraProjectionMatrix";

    case InvertCameraWorldMatrix:
        return "InvertCameraProjectionMatrix";

    case LightInnerCos:
        return "LightInnerCos";

    case LightOuterCos:
        return "LightOuterCos";

    case LightPosition:
        return "LightPosition";

    case LightDirection:
        return "LightDirection";

    case LightColor:
        return "LightColor";

    case LightAtteCoef:
        return "LightAtteCoef";

    case LightNumEmitters:
        return "LightNumEmitters";

    case LightProjectionMatrix0:
        return "LightProjectionMatrix0";

    case LightProjectionMatrix1:
        return "LightProjectionMatrix1";

    case LightProjectionMatrix2:
        return "LightProjectionMatrix2";

    case LightProjectionMatrix3:
        return "LightProjectionMatrix3";

    case LightProjectionMatrix4:
        return "LightProjectionMatrix4";

    case LightProjectionMatrix5:
        return "LightProjectionMatrix5";

    case LightWorldMatrix0:
        return "LightWorldMatrix0";

    case LightWorldMatrix1:
        return "LightWorldMatrix1";

    case LightWorldMatrix2:
        return "LightWorldMatrix2";

    case LightWorldMatrix3:
        return "LightWorldMatrix3";

    case LightWorldMatrix4:
        return "LightWorldMatrix4";

    case LightWorldMatrix5:
        return "LightWorldMatrix5";

    case ShadowMap0:
        return "ShadowMap0";

    case ShadowMap1:
        return "ShadowMap1";

    case ShadowMap2:
        return "ShadowMap2";

    case ShadowMap3:
        return "ShadowMap3";

    case ShadowMap4:
        return "ShadowMap4";

    case ShadowMap5:
        return "ShadowMap5";

    case ShadowCubeMap:
        return "ShadowCubeMap";

    case LightInfluence:
        return "LightInfluence";

    case ShadowDirection0:
        return "ShadowDirection0";

    case ShadowDirection1:
        return "ShadowDirection1";

    case ShadowDirection2:
        return "ShadowDirection2";

    case ShadowDirection3:
        return "ShadowDirection3";

    case ShadowDirection4:
        return "ShadowDirection4";

    case ShadowDirection5:
        return "ShadowDirection5";

    case RenderableWorldMatrix:
        return "RenderableWorldMatrix";

    case ColorMap0:
        return "ColorMap0";

    case ColorMap1:
        return "ColorMap1";

    case ColorMap2:
        return "ColorMap2";

    case ColorMap3:
        return "ColorMap3";

    case NormalMap0:
        return "NormalMap0";

    case NormalMap1:
        return "NormalMap1";

    case NormalMap2:
        return "NormalMap2";

    case NormalMap3:
        return "NormalMap3";

    case Plaster:
        return "Plaster";

    case ColorSketch:
        return "ColorSketch";

    case NormalSketch:
        return "NormalSketch";

    case MaterialIDSketch:
        return "MaterialIDSketch";

    case LightingSketch:
        return "LightingSketch";

    case DiffuseLightingSketch:
        return "DiffuseLightingSketch";

    case SpecularLightingSketch:
        return "SpecularLightingSketch";

    case ColorSketchPixelSize:
        return "ColorSketchPixelSize";

    case AmbiColor:
        return "AmbiColor";

    case CurrentMaterialID:
        return "CurrentMaterialID";

    default:
        return "UnknownParamSource";
    }
}

void Renderer::common_init()
{
	RendererBase::CommonInit();

    memset ( &std_sketch_book, 0, sizeof ( std_sketch_book ) );
    memset ( lighting_sketch_book_array, 0, sizeof ( lighting_sketch_book_array ) );

    curt_light = NULL;

    ///VertexDecl pdec = RenderablePlane_get_vertex_declaration ( render_plane );

    dirty_flag = true;
    clear_color_buffer_flag = true;
    clear_depth_buffer_flag = true;
}

Renderer::Renderer ( Renderer *_prev_renderer )
: prev_render_skb(NULL)
, prev_lighting_skb(NULL)
, debug_render_output(NonDebug)
, RendererBase(_prev_renderer)
{
    common_init();
    prev_renderer = _prev_renderer;
    use_deferred_shading = _prev_renderer->use_deferred_shading;
}

Renderer::Renderer ( ViewportPtr view )
: prev_render_skb(NULL)
, prev_lighting_skb(NULL)
, debug_render_output(NonDebug)
, RendererBase(view)
{
    common_init();
    prev_renderer = NULL;
    use_deferred_shading = USE_DS_RENDER;
}

void Renderer::RendererDest()
{
    Dest();

    if ( !prev_renderer ) {
        if ( std_sketch_book ) {
            SketchBook_delete ( std_sketch_book );
        }

        for ( euint i = 0; i < NUM_LIGHTING_SKETCH_BOOKS; i++ ) {
            SketchBook skb = lighting_sketch_book_array[i];

            if ( skb ) {
                SketchBook_delete ( skb );
            }
        }
    }
/**
    Iterator iter = Tree_begin ( param_proc_tree );

    while ( iter ) {
        var data = Tree_get_value ( iter );
        RendererParamEntry entry = ( RendererParamEntry ) data.vptr_var;
        RendererParamEntry_delete ( entry );
        iter = Tree_next ( iter );
    }

    Tree_Dest ( param_proc_tree );
	**/
	STD_NAMESPACE::map<param_source, RendererParamEntry>::iterator iter = param_proc_map.begin();
	STD_NAMESPACE::map<param_source, RendererParamEntry>::iterator end = param_proc_map.end();
	for (; iter != end; iter++) {
        RendererParamEntry entry = iter->second;
		RendererParamEntry_delete ( entry );
	}
}

Renderer::~Renderer()
{
    RendererDest();
}

void Renderer::register_material ( const char *mat_name, SDisplayProc disp_proc, e_draw_mode draw_mode, bool double_sided_flag )
{
    var key, data;
    key.str_var = mat_name;
    Iterator iter = Tree_find ( material_table, key, &data );

    if ( !iter ) {
        VertexDecl dec = RenderablePlane_get_vertex_declaration ( render_plane );
        MaterialPrototype mp;
        {
            SdrNdGen sng = ISdrNdGen.New();
            ISdrNdGen.register_default_nodes ( sng );
            mp = MaterialPrototype_new ( dec, material_count,
                                         disp_proc,
                                         draw_mode, double_sided_flag );
            ISdrNdGen.Delete ( sng );
        }
        data.vptr_var = mp;
        Tree_insert ( material_table, key, data );
        material_count++;
    }
}

esint Renderer::get_material_id ( const char *mat_name )
{
    var key, data;
    key.str_var = mat_name;
    Iterator iter = Tree_find ( material_table, key, &data );

    if ( iter ) {
        MaterialPrototype mp = ( MaterialPrototype ) data.vptr_var;
        return mp->material_id;
    }
    else {
        return -1;
    }
}

void Renderer::prepare_sketchbooks()
{
    if ( !prev_renderer ) {
        elog ( "%s", "STD SKETCHBOOK INIT BEGIN:" );
        {
            if ( std_sketch_book ) {
                SketchBook_delete ( std_sketch_book );
            }

            SketchBookConfig cfg = SketchBookConfig_new();
            SketchBookConfig_set_size ( cfg, width, height );

            SketchBookConfig_set_sketch_format ( cfg, MATERIAL_ID_INDEX,     MATERIAL_ID_FORMAL );
            SketchBookConfig_set_sketch_format ( cfg, BASE_COLOR_INDEX,      BASE_COLOR_FORMAT );
            SketchBookConfig_set_sketch_format ( cfg, NORMAL_SPECULAR_INDEX, NORMAL_SPECULAR_FORMAT );

            SketchBookConfig_set_plaster_format ( cfg, DEPTH32F );
            SketchBook skb = SketchBook_new ( cfg );
            SketchBookConfig_delete ( cfg );
            std_sketch_book = skb;

            Texture2DPtr tex_mat_id = SketchBook_get_sketch ( skb, MATERIAL_ID_INDEX );
            Texture2DPtr tex_color = SketchBook_get_sketch ( skb, BASE_COLOR_INDEX );
            Texture2DPtr tex_normal = SketchBook_get_sketch ( skb, NORMAL_SPECULAR_INDEX );
            elog ( "        MATERIAL ID SKETCH ID %d", tex_mat_id->GetID() );
            elog ( "        BASE COLOR SKETCH ID %d", tex_color->GetID() );
            elog ( "        NORMAL SPECULAR SKETCH ID %d", tex_normal->GetID() );
            elog ( "%s", "STD SKETCHBOOK INIT END:" );
        }

        shadow_state.prepare ( get_render_plane() );

        elog ( "%s", "LIGHTING SKETCHBOOK INIT BEGIN:" );

        for ( euint i = 0; i < NUM_LIGHTING_SKETCH_BOOKS; i++ ) {
            SketchBook skb = lighting_sketch_book_array[i];

            if ( to_ptr ( skb ) ) {
                SketchBook_delete ( skb );
            }
        }

        for ( euint32 i = 0; i < NUM_LIGHTING_SKETCH_BOOKS; i++ ) {
            elog ( "    SKETCH BOOK %d", i );
            SketchBookConfig cfg = SketchBookConfig_new();
            SketchBookConfig_set_size ( cfg, width, height );

            SketchBookConfig_set_sketch_format ( cfg, DIFFUSE_INDEX,  DIFFUSE_FORMAT );
            SketchBookConfig_set_sketch_format ( cfg, SPECULAR_INDEX, SPECULAR_FORMAT );

            SketchBook skb = SketchBook_new ( cfg );
            SketchBookConfig_delete ( cfg );
            lighting_sketch_book_array[i] = skb;

            Texture2DPtr tex_diffuse = SketchBook_get_sketch ( skb, DIFFUSE_INDEX );
            Texture2DPtr tex_specular = SketchBook_get_sketch ( skb, SPECULAR_INDEX );
            elog ( "        DIFFUSE INDEX SKETCH ID %d", tex_diffuse->GetID() );
            elog ( "        SPECULAR INDEX SKETCH ID %d", tex_specular->GetID() );
        }

        elog ( "%s", "LIGHTING SKETCHBOOK INIT END:" );
    }
    else {
        std_sketch_book = prev_renderer->std_sketch_book;

        for ( euint i = 0; i < NUM_LIGHTING_SKETCH_BOOKS; i++ ) {
            lighting_sketch_book_array[i] = prev_renderer->lighting_sketch_book_array[i];
        }
    }
}
void Renderer::prepare_display_passes()
{
    if ( display_pass ) {
        Pass_delete ( display_pass );
    }

	if (debug_render_output == NonDebug) {
		Iterator mat_iter = Tree_begin ( material_table );
		material_decl null_mat_decl = {NULL, -1};
		material_decl *decls = array_new ( material_decl, (euint32)Tree_count ( material_table ), null_mat_decl );

		while ( mat_iter ) {
			var data = Tree_get_value ( mat_iter );
			MaterialPrototype mp = ( MaterialPrototype ) data.vptr_var;
			material_decl d = {mp->disp_proc, mp->material_id};
			apush ( decls, d );
			mat_iter = Tree_next ( mat_iter );
		}

		VertexDecl vtx_decl = RenderablePlane_get_vertex_declaration ( render_plane );
		display_pass = create_display_pass_ex2 ( vtx_decl, decls );

		array_delete ( decls );
	}
	else {
		VertexDecl vtx_decl = RenderablePlane_get_vertex_declaration ( render_plane );
		display_pass = create_debug_display_pass(vtx_decl, debug_render_output);
	}
}
void Renderer::prepare_lighting_passes()
{
    VertexDecl vtx_decl = RenderablePlane_get_vertex_declaration ( render_plane );
    lighting_decl null_lt_decl = {NULL, -1};
    lighting_decl *decls = array_new ( lighting_decl, (euint32)Tree_count ( material_table ), null_lt_decl );
    Iterator mat_iter = Tree_begin ( material_table );

    while ( mat_iter ) {
        var data = Tree_get_value ( mat_iter );
        MaterialPrototype mp = ( MaterialPrototype ) data.vptr_var;
        lighting_decl d = { ( LightingProc2 ) lighting_template, mp->material_id};
        apush ( decls, d );
        mat_iter = Tree_next ( mat_iter );
    }

    dir_light_ptype.prepare_lighting_passes ( this, vtx_decl, decls );
    spot_light_ptype.prepare_lighting_passes ( this, vtx_decl, decls );
    point_light_ptype.prepare_lighting_passes ( this, vtx_decl, decls );

    array_delete ( decls );
}

void Renderer::render_std_passes()
{
    prev_render_skb = std_sketch_book;
    Pass std_pass = {NULL};

	if ( use_deferred_shading )
        glViewport ( 0, 0, width, height );
	else
        glViewport ( x, y, width, height );

    if ( use_deferred_shading ) {
        SketchBook_bind_framebuffer ( prev_render_skb );
        SketchBook_bind_renderbuffer();
    }

    euint32 clear_flag = 0;

    if ( clear_color_buffer_flag ) {
        clear_flag |= GL_COLOR_BUFFER_BIT;
    }

    if ( clear_depth_buffer_flag ) {
        clear_flag |= GL_DEPTH_BUFFER_BIT;
    }

    if ( clear_flag ) {
        glClear ( clear_flag );
    }

    glDepthMask ( GL_TRUE );

    if ( use_deferred_shading ) {
        SketchBook_unbind_framebuffer();
        SketchBook_unbind_renderbuffer();
    }

	renderable_sorter->Sort(used_renderable_set, sorted_renderable_list);
	STD_NAMESPACE::list<Renderable>::iterator r_iter = sorted_renderable_list.begin();
	STD_NAMESPACE::list<Renderable>::iterator end = sorted_renderable_list.end();
    euint count = 0;

	for (; r_iter != end; r_iter++ ) {
        var key, data;

		Renderable rbl = *r_iter;

        if ( !rbl->world_matrix ) {
            throw_exception ( RenderableWorldMatrixIsNull, "renderable world matrix is null" );
            continue;
        }

        curt_rend_world_matrix = rbl->world_matrix;

        const char *mat_name = rbl->material->GetMaterialName().c_str();

        key.str_var = mat_name;
        Iterator iter = Tree_find ( material_table, key, &data );

        if ( !iter ) {
            throw_exception ( MaterialNotExist, "material not exist" );
            continue;
        }

        MaterialPrototype mp = ( MaterialPrototype ) data.vptr_var;

        if ( mp->draw_mode == Shaded ) {
            glPolygonMode ( GL_FRONT, GL_FILL );
        }
        else {
            glPolygonMode ( GL_FRONT, GL_LINE );
        }

        ERROR_PROC;

        if ( mp->double_sided_flag ) {
            glDisable ( GL_CULL_FACE );
        }
        else {
            glEnable ( GL_CULL_FACE );
        }

        ERROR_PROC;

        if ( use_deferred_shading ) {
            SketchBook_draw_begin ( prev_render_skb, 3 );
        }

        prepare_renderable ( rbl );
        std_pass = rbl->std_pass;

        curt_mat_proto = mp;
        curt_mat_inst = rbl->material.get();
        Pass_auto_set_uniform_params ( std_pass, this, false );

        euint32 face_count = IndexBuffer_get_num_faces ( rbl->idx_buf );
        e_mesh_mode mode = IndexBuffer_get_mesh_mode ( rbl->idx_buf );

		euint32 clip_plane_count = 0;
		xhn::vector<plane>::iterator cpIter = clip_planes.begin();
		xhn::vector<plane>::iterator cpEnd = clip_planes.end();
		for (; cpIter != cpEnd; cpIter++, clip_plane_count++) {
			plane& p = *cpIter;
            GLPlane glplane = Plane_to_glplane(&p); 
			glClipPlane(GL_CLIP_PLANE0 + clip_plane_count, glplane.GetPointer());
			glEnable(GL_CLIP_PLANE0 + clip_plane_count);
		}
		cpIter = rbl->clip_planes.begin();
		cpEnd = rbl->clip_planes.end();
		for (; cpIter != cpEnd; cpIter++, clip_plane_count++) {
			plane& p = *cpIter;
			GLPlane glplane = Plane_to_glplane(&p); 
			glClipPlane(GL_CLIP_PLANE0 + clip_plane_count, glplane.GetPointer());
			glEnable(GL_CLIP_PLANE0 + clip_plane_count);
		}
		
        if ( mode == Triangular ) {
            Pass_render ( std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 3, mode );
        }
        else {
            Pass_render ( std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 2, mode );
        }

		for (euint32 i = 0; i < clip_plane_count; i++) {
			glDisable(GL_CLIP_PLANE0 + i);
		}
		
        ERROR_PROC;
		
        if ( use_deferred_shading ) {
            SketchBook_draw_end ( prev_render_skb );
            ERROR_PROC;
        }

        count++;
    }
}

void Renderer::shadow_render ( Renderable rbl, SketchBook curt_skb, sketch_type shadow_type )
{
    if ( !rbl->world_matrix ) {
        throw_exception ( RenderableWorldMatrixIsNull, "renderable world matrix is null" );
        return;
    }

    curt_rend_world_matrix = rbl->world_matrix;

    const char *mat_name = rbl->material->GetMaterialName().c_str();

    var key, data;
    key.str_var = mat_name;
    Iterator iter = Tree_find ( material_table, key, &data );

    if ( !iter ) {
        throw_exception ( MaterialNotExist, "material not exist" );
        return;
    }

    MaterialPrototype mp = ( MaterialPrototype ) data.vptr_var;

    if ( mp->draw_mode == Shaded ) {
        glPolygonMode ( GL_FRONT, GL_FILL );
    }
    else {
        glPolygonMode ( GL_FRONT, GL_LINE );
    }

    ERROR_PROC;

    if ( mp->double_sided_flag ) {
        glDisable ( GL_CULL_FACE );
    }
    else {
        glEnable ( GL_CULL_FACE );
    }

    ERROR_PROC;

    SketchBook_draw_begin ( curt_skb, 1 );

    Renderable_depth_prev_render ( rbl );
    Pass std_pass = NULL;

    switch ( shadow_type ) {
    case NormalDepthPass:
        std_pass = rbl->depth_pass;;
        break;

    case PointDepthPass:
        std_pass = rbl->point_depth_pass;;
        break;
    }

    curt_mat_proto = mp;
    curt_mat_inst = rbl->material.get();

    Pass_auto_set_uniform_params ( std_pass, this, false );

    euint32 face_count = IndexBuffer_get_num_faces ( rbl->idx_buf );
    e_mesh_mode mode = IndexBuffer_get_mesh_mode ( rbl->idx_buf );

    if ( mode == Triangular ) {
        Pass_render ( std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 3, mode );
    }
    else {
        Pass_render ( std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 2, mode );
    }

    ERROR_PROC;
    SketchBook_draw_end ( curt_skb );
    ERROR_PROC;
}

void Renderer::lighting_render ( Pass lighting_pass, SketchBook curt_skb )
{
    SketchBook_draw_begin ( curt_skb, 2 );

    Pass_auto_set_uniform_params ( lighting_pass, this, false );

    ERROR_PROC;
    Pass_render_plane ( lighting_pass, render_plane );
    ERROR_PROC;
    SketchBook_draw_end ( curt_skb );
    ERROR_PROC;
}

void Renderer::prepare_shadow_maps ( LightBase2 pl, light_prototype *prototype )
{
    curt_light = pl;
    /// here render shadow map
    sketch_type shadow_type = pl->get_shadow_type();

    if ( shadow_type == Sketch2D ) {
        Camera light_cam = pl->get_camera ( 0 );

        if ( to_ptr ( light_cam ) ) {
            curt_render_cam = light_cam;

			renderable_sorter->Sort(used_renderable_set, sorted_renderable_list);
			STD_NAMESPACE::list<Renderable>::iterator rbl_iter = sorted_renderable_list.begin();
			STD_NAMESPACE::list<Renderable>::iterator end = sorted_renderable_list.end();
            SketchBook skb = shadow_state.depth_sketch_book;

            clear_sketch_book ( skb );

			for (; rbl_iter != end; rbl_iter++) {
                euint32 width = SketchBookConfig_get_width ( shadow_state.depth_sketch_cfg );
                euint32 height = SketchBookConfig_get_height ( shadow_state.depth_sketch_cfg );

				Renderable rbl = *rbl_iter;
                glViewport ( 0, 0, width, height );
                glEnable ( GL_DEPTH_TEST );
                ERROR_PROC;
                glEnable ( GL_STENCIL_TEST );
                ERROR_PROC;
                shadow_render ( rbl, skb, shadow_type );
            }
			curt_rend_world_matrix = default_rend_matrix;
			var key, data;
			key.str_var = "default_material";
			if (Tree_find(material_table, key, &data)) {
				curt_mat_proto = (MaterialPrototype)data.vptr_var;
			}
            shadow_state.depth_map_blur ( this );
        }
    }
    else if ( shadow_type == Sketch2DBox ) {
        /// 阴影是cube map，需要渲染6次
        for ( euint i = 0; i < 6; i++ ) {
            Camera light_cam = pl->get_camera ( i );

            if ( to_ptr ( light_cam ) ) {
                curt_render_cam = light_cam;
				renderable_sorter->Sort(used_renderable_set, sorted_renderable_list);
				STD_NAMESPACE::list<Renderable>::iterator rbl_iter = sorted_renderable_list.begin();
				STD_NAMESPACE::list<Renderable>::iterator end = sorted_renderable_list.end();
                /// 先渲染到2D的sketch中，再将2Dsketch模糊到cubemap中
                SketchBook skb = shadow_state.depth_sketch_book;
                ///SketchCube skc = _self->depth_sketch_cube;

                clear_sketch_book ( skb );

                ///while ( rbl_iter ) {
				for (; rbl_iter != end; rbl_iter++) {
                    euint32 width = shadow_state.blurred_depth_sketch_cube_size;
                    euint32 height = shadow_state.blurred_depth_sketch_cube_size;

					Renderable rbl = *rbl_iter;
                    glViewport ( 0, 0, width, height );
                    glEnable ( GL_DEPTH_TEST );
                    ERROR_PROC;
                    glEnable ( GL_STENCIL_TEST );
                    ERROR_PROC;
                    shadow_render ( rbl, skb, shadow_type );
                }
				curt_rend_world_matrix = default_rend_matrix;
				var key, data;
			    key.str_var = "default_material";
			    if (Tree_find(material_table, key, &data)) {
				    curt_mat_proto = (MaterialPrototype)data.vptr_var;
			    }
                shadow_state.depth_map_blur_to_cube_map ( this, i );
            }
        }
    }

    curt_render_cam = camera_base;
}

void Renderer::all_lighting_render ( light_prototype &prototype, SketchBook curt_lighting_skb, euint *light_count )
{
    Iterator light_iter = Tree_begin ( prototype.light_tree );

    while ( light_iter ) {
        var data = Tree_get_value ( light_iter );

        LightBase2 pl = ( LightBase2 ) data.vptr_var;

		curt_light = pl;

        prepare_shadow_maps ( pl, &prototype );
		///shadow_renderer->prepare_shadow_maps(pl, &prototype);

        glViewport ( 0, 0, width, height );
        glDisable ( GL_DEPTH_TEST );
        ERROR_PROC;
        glDisable ( GL_STENCIL_TEST );
        ERROR_PROC;

        if ( *light_count != 0 ) {
            curt_diffuse_lighting_sketch = SketchBook_get_sketch ( prev_lighting_skb, 0 );
            curt_specular_lighting_sketch = SketchBook_get_sketch ( prev_lighting_skb, 1 );

            lighting_render ( prototype.lighting_pass, curt_lighting_skb );
        }
        else {
            lighting_render ( prototype.first_lighting_pass, curt_lighting_skb );
        }

        SketchBook tmp = prev_lighting_skb;
        prev_lighting_skb = curt_lighting_skb;
        curt_lighting_skb = tmp;

        light_iter = Tree_next ( light_iter );
        ( *light_count ) ++;

        glDisable ( GL_DEPTH_TEST );
        ERROR_PROC;
        glDisable ( GL_STENCIL_TEST );
        ERROR_PROC;
    }
}

void Renderer::prepare_renderable(Renderable _rbl)
{
    Renderable_prev_render(_rbl);
}

void Renderer::lighting()
{
    curt_normal_sketch = SketchBook_get_sketch ( prev_render_skb, NORMAL_SPECULAR_INDEX );
    curt_plaster = SketchBook_get_plaster ( prev_render_skb );
    curt_material_id_sketch = SketchBook_get_sketch ( prev_render_skb, MATERIAL_ID_INDEX );

    {
        SketchBook curt_lighting_skb = lighting_sketch_book_array[0];
        prev_lighting_skb = lighting_sketch_book_array[1];

        curt_material_id_sketch = SketchBook_get_sketch ( prev_render_skb, MATERIAL_ID_INDEX );

        euint light_count = 0;

        all_lighting_render ( dir_light_ptype, curt_lighting_skb, &light_count );
        all_lighting_render ( spot_light_ptype, curt_lighting_skb, &light_count );
        all_lighting_render ( point_light_ptype, curt_lighting_skb, &light_count );
    }
}
void Renderer::render()
{
    TRY ( 0 )

    _render_one_frame_begin();

    ///if ( !Tree_count ( used_renderable_tree ) ) {
	if ( ! used_renderable_set.size() ) {
        return;
    }

    if ( dirty_flag ) {
        prepare_sketchbooks();
        prepare_display_passes();
        prepare_lighting_passes();
    }

    render_std_passes();

    if ( use_deferred_shading ) {
		if (debug_render_output == NonDebug)
            lighting();
        glViewport ( x, y, width, height );

        if ( !prev_renderer ) {
            glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
            glClearDepth ( 1.0 );
            glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        }

		curt_mat_proto = NULL;
		curt_material_id_sketch = SketchBook_get_sketch ( prev_render_skb, MATERIAL_ID_INDEX );
		curt_color_sketch = SketchBook_get_sketch ( prev_render_skb, BASE_COLOR_INDEX );
		curt_normal_sketch = SketchBook_get_sketch ( prev_render_skb, NORMAL_SPECULAR_INDEX );
		curt_plaster = SketchBook_get_plaster ( prev_render_skb );
		if (prev_lighting_skb) {
			curt_lighting_sketch = SketchBook_get_sketch ( prev_lighting_skb, 0 );
			curt_diffuse_lighting_sketch = SketchBook_get_sketch ( prev_lighting_skb, 0 );
			curt_specular_lighting_sketch = SketchBook_get_sketch ( prev_lighting_skb, 1 );
		}
		else {
            curt_lighting_sketch = NULL;
            curt_diffuse_lighting_sketch = NULL;
            curt_specular_lighting_sketch = NULL;
		}

		Pass_auto_set_uniform_params ( display_pass, this, false );

		Pass_render_plane ( display_pass, render_plane );
    }

    dirty_flag = false;
    CATCH ( 0 ) {
        dirty_flag = false;
    }
}

void Renderer::clear_sketchbook()
{
    Pass clear_sketchbook_pass = NULL;

    if ( prev_renderer ) {
        clear_sketchbook_pass = prev_renderer->clear_sketchbook_pass;
    }
    else {
        clear_sketchbook_pass = clear_sketchbook_pass;
    }

    SketchBook render_skb = std_sketch_book;

    SketchBook_draw_begin ( render_skb, 3 );

    glShadeModel ( GL_SMOOTH );                 // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 4 );   // 4-byte pixel alignment
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable ( GL_BLEND );

    glEnable ( GL_TEXTURE_2D );
    glEnable ( GL_TEXTURE_CUBE_MAP );
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth ( 1.0 );
    glClearStencil ( 0 );
    glDepthFunc ( GL_LEQUAL );
    glCullFace ( GL_BACK );
    ERROR_PROC;

    glDisable ( GL_DEPTH_TEST );
    glDisable ( GL_STENCIL_TEST );
    ERROR_PROC;
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glDepthMask ( GL_FALSE );
    glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glViewport ( x, y, width, height );
    Pass_render_plane ( clear_sketchbook_pass, render_plane );
    SketchBook_draw_end ( render_skb );
}

void Renderer::add_clip_plane(const plane& p)
{
    clip_planes.push_back(p);
}

void Renderer::clear_clip_planes()
{
    clip_planes.clear();
}

Renderable Renderer::new_renderable ( VertexDecl _dec, MaterialInstance* _m_inst, e_mesh_mode _mesh_mode )
{
    const char *mat_name = _m_inst->GetMaterialName().c_str();
    var key, data;
    key.str_var = mat_name;
    Iterator iter = Tree_find ( material_table, key, &data );

    if ( !iter ) {
        char mbuf[STRING_BUFFER_SIZE];
        snprintf ( mbuf, STRING_BUFFER_SIZE - 1, "file %s line %d material not exist", __FILE__, __LINE__ );
        throw_exception ( MaterialNotExist, mbuf );
    }

    return RendererBase::new_renderable ( _dec, _m_inst, _mesh_mode );
}

void Renderer::viewport_refresh ( euint32 _x, euint32 _y, euint32 _width, euint32 _height )
{
    Camera cam = curt_render_cam;

    Camera_refresh ( cam, width, height );

    x = _x;
    y = _y;
    width = _width;
    height = _height;
    dirty_flag = true;
}

void shader_object_value_Dest ( renderer_param_value _obj )
{
    if ( _obj.value ) {
        Mfree ( _obj.value );
    }
}

bool Renderer::is_uniform_param_source ( esint32 _src )
{
    ///var key, data;
    ///key.sint32_var = _src;

    ///if ( Tree_find ( param_proc_tree, key, &data ) ) {
	if (param_proc_map.find((param_source)_src) != param_proc_map.end()) {
        return true;
    }
    else {
        return false;
    }
}

renderer_param_value Renderer::get_shader_object_value ( RendererBase* rdr, esint32 _src )
{
	///var key, data;
    ///key.sint32_var = _src;

	STD_NAMESPACE::map<param_source, RendererParamEntry>::iterator iter = param_proc_map.find((param_source)_src);
    ///if ( Tree_find ( param_proc_tree, key, &data ) ) {
	if (iter != param_proc_map.end()) {
        RendererParamEntry entry = iter->second;
        return entry->get_value_proc ( rdr );
    }
    else {
        renderer_param_value null_ret = {NULL};
        return null_ret;
    }
}

renderer_param_value Renderer::get_shader_object_value ( esint32 _src )
{
    return get_shader_object_value(this, _src);
}

void Renderer::register_renderer_param ( esint32 _id, param_type _type, esint32 _array_size, GetRendererParamProc _proc )
{
    ///var key, data;
    ///key.sint32_var = _id;
    ///data.vptr_var = RendererParamEntry_new ( _proc, _type, _array_size );
    ///Tree_insert ( param_proc_tree, key, data );
	param_proc_map.insert(STD_NAMESPACE::make_pair((param_source)_id, RendererParamEntry_new ( _proc, _type, _array_size )));
}
RendererParamEntry Renderer::get_param_entry ( esint32 _id )
{
    ///var key, data;
    ///key.sint32_var = _id;

    STD_NAMESPACE::map<param_source, RendererParamEntry>::iterator iter = param_proc_map.find((param_source)_id);
    ///if ( Tree_find ( param_proc_tree, key, &data ) ) {
	if (iter != param_proc_map.end()) {
        RendererParamEntry entry = iter->second;
        return entry;
    }
    else {
        return NULL;
    }
}

void Renderer::set_debug_output(DebugRenderOutput dro)
{
	if (dro != debug_render_output) {
		debug_render_output = dro;
		dirty_flag = true;
	}
}