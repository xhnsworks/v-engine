#include "pch.h"
#include "renderer_base.h"
#include "renderable_base.h"
#include "shader_test.h"
#include "render_system.h"
#include "clear_sketchbook_pass.h"
#include "sfloat3.h"

void DefaultRenderableSorter::Sort(STD_NAMESPACE::set<Renderable>& used_renderable_set, STD_NAMESPACE::list<Renderable>& sorted_renderable_list)
{
	sorted_renderable_list.clear();
	STD_NAMESPACE::set<Renderable>::iterator iter = used_renderable_set.begin();
	STD_NAMESPACE::set<Renderable>::iterator end = used_renderable_set.end();
	for (; iter != end; iter++)
	{
		Renderable rbl = *iter;
		sorted_renderable_list.push_back(rbl);
	}
}

void RendererBase::_renderable_set_dest ( STD_NAMESPACE::set<Renderable>& _s )
{
	STD_NAMESPACE::set<Renderable>::iterator iter = _s.begin();
    STD_NAMESPACE::set<Renderable>::iterator end = _s.end();
	for (; iter != end; iter++) {
        renderable *rdl = *iter;
        VertexBuffer_delete ( rdl->vtx_buf );
        IndexBuffer_delete ( rdl->idx_buf );
        Matrix4x4_delete ( rdl->world_matrix );
		
		delete rdl;
    }
	_s.clear();
}

ShaderNode default_material_proc3 ( PxlSdrBuf _psb, int _id )
{
    ShaderBuffer sb = to_ShaderBuffer ( _psb );

    _tex_2d_obj_test ( sb, "ColorMap" );
    _tex_2d_obj_test ( sb, "DiffuseLightingMap" );
    _tex_2d_obj_test ( sb, "SpecularLightingMap" );
    _vec2_obj_test ( sb, "vTexCoord" );

    ShaderNode psn = ShaderNode_new();

    char mbuf[STRING_BUFFER_SIZE];
    snprintf ( mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id );
    ShaderNode_set_name ( psn, mbuf );

    ShaderNode_set_function ( psn,
                              "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                              "    vec3 cmap = texture(ColorMap, vTexCoord).rgb;\n"
                              "    vec3 dmap = texture(DiffuseLightingMap, vTexCoord).rgb;\n"
                              "    vec3 smap = texture(SpecularLightingMap, vTexCoord).rgb;\n"
#else
							  "    vec3 cmap = texture2D(ColorMap, vTexCoord).rgb;\n"
							  "    vec3 dmap = texture2D(DiffuseLightingMap, vTexCoord).rgb;\n"
							  "    vec3 smap = texture2D(SpecularLightingMap, vTexCoord).rgb;\n"
#endif
                              "    gl_FragData[0] = vec4( clamp(cmap * dmap + smap, 0.0, 1.0), 1.0 );"
                              "}\n" );

    return psn;
}

#include "renderer_param_procs.h"

RendererParamEntry RendererParamEntry_new ( GetRendererParamProc _proc, param_type _type, esint32 _array_size )
{
    RendererParamEntry ret = ( RendererParamEntry ) SMalloc ( sizeof ( renderer_param_entry ) );
    ret->get_value_proc = _proc;
    ret->type = _type;
    ret->array_size = _array_size;
    return ret;
}
void RendererParamEntry_delete ( RendererParamEntry _self )
{
    Mfree ( _self );
}

void RendererBase::CommonInit()
{
    {
		param_proc_map.insert(STD_NAMESPACE::make_pair(CameraPosition, RendererParamEntry_new ( getCameraPositionProc, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(CameraDirection, RendererParamEntry_new ( getCameraDirectionProc, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(CameraPlaneNear, RendererParamEntry_new ( getCameraPlaneNearProc, Float32_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(CameraPlaneFar, RendererParamEntry_new ( getCameraPlaneFarProc, Float32_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(CameraPlaneWidth, RendererParamEntry_new ( getCameraPlaneWidthProc, Float32_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(CameraPlaneHeight, RendererParamEntry_new ( getCameraPlaneHeightProc, Float32_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(CameraProjectionMatrix, RendererParamEntry_new ( getCameraProjectionMatrixProc, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(CameraWorldMatrix, RendererParamEntry_new ( getCameraWorldMatrixProc, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(RenderableWorldMatrix, RendererParamEntry_new ( getRenderableWorldMatrixProc, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(InvertCameraProjectionMatrix, RendererParamEntry_new ( getInvertCameraProjectionMatrixProc, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(InvertCameraWorldMatrix, RendererParamEntry_new ( getInvertCameraWorldMatrixProc, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(CurrentMaterialID, RendererParamEntry_new ( getCurrentMaterialIDProc, Float32_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ColorMap0, RendererParamEntry_new ( getColorMap0Proc, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(NormalMap0, RendererParamEntry_new ( getNormalMap0Proc, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(Plaster, RendererParamEntry_new ( getPlasterProc, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ColorSketch, RendererParamEntry_new ( getColorSketchProc, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(NormalSketch, RendererParamEntry_new ( getNormalSketchProc, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(MaterialIDSketch, RendererParamEntry_new ( getMaterialIDSketchProc, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightingSketch, RendererParamEntry_new ( getLightingSketchProc, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightInnerCos, RendererParamEntry_new ( getLightInnerCos, Float32_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightOuterCos, RendererParamEntry_new ( getLightOuterCos, Float32_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightPosition, RendererParamEntry_new ( getLightPosition, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightDirection, RendererParamEntry_new ( getLightDirection, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightColor, RendererParamEntry_new ( getLightColor, Float32x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightAtteCoef, RendererParamEntry_new ( getLightAtteCoef, Float32_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightNumEmitters, RendererParamEntry_new ( getLightNumShadowEmitters, Int_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightProjectionMatrix0, RendererParamEntry_new ( getLightProjMatrix0, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightProjectionMatrix1, RendererParamEntry_new ( getLightProjMatrix1, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightProjectionMatrix2, RendererParamEntry_new ( getLightProjMatrix2, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightProjectionMatrix3, RendererParamEntry_new ( getLightProjMatrix3, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightProjectionMatrix4, RendererParamEntry_new ( getLightProjMatrix4, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightProjectionMatrix5, RendererParamEntry_new ( getLightProjMatrix5, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightWorldMatrix0, RendererParamEntry_new ( getLightWorldMatrix0, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightWorldMatrix1, RendererParamEntry_new ( getLightWorldMatrix1, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightWorldMatrix2, RendererParamEntry_new ( getLightWorldMatrix2, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightWorldMatrix3, RendererParamEntry_new ( getLightWorldMatrix3, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightWorldMatrix4, RendererParamEntry_new ( getLightWorldMatrix4, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightWorldMatrix5, RendererParamEntry_new ( getLightWorldMatrix5, Matrix4x4_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowMap0, RendererParamEntry_new ( getShadowMap0, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowMap1, RendererParamEntry_new ( getShadowMap1, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowMap2, RendererParamEntry_new ( getShadowMap2, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowMap3, RendererParamEntry_new ( getShadowMap3, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowMap4, RendererParamEntry_new ( getShadowMap4, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowMap5, RendererParamEntry_new ( getShadowMap5, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowCubeMap, RendererParamEntry_new ( getShadowCubeMap, TextureCube_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(LightInfluence, RendererParamEntry_new ( getLightInfluence, Float32_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowDirection0, RendererParamEntry_new ( getShadowDir0, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowDirection1, RendererParamEntry_new ( getShadowDir1, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowDirection2, RendererParamEntry_new ( getShadowDir2, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowDirection3, RendererParamEntry_new ( getShadowDir3, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowDirection4, RendererParamEntry_new ( getShadowDir4, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ShadowDirection5, RendererParamEntry_new ( getShadowDir5, Float32x3_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(DiffuseLightingSketch, RendererParamEntry_new ( getDiffuseLightingSketchProc, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(SpecularLightingSketch, RendererParamEntry_new ( getSpecularLightingSketchProc, Texture2D_Param, 1 )));
		param_proc_map.insert(STD_NAMESPACE::make_pair(ColorSketchPixelSize, RendererParamEntry_new ( getColorSketchPixelSizeProc, Float32x2_Param, 1 )));
    }

	STD_NAMESPACE::map<param_source, RendererParamEntry>::iterator ppt_iter = param_proc_map.begin();
	elog ( "%s", "########" );

	while ( ppt_iter != param_proc_map.end() ) {
		const char *key_str = _get_param_src_str ( ( param_source ) ppt_iter->first );
		elog ( "%s", key_str );
		ppt_iter++;
	}

	elog ( "%s", "########" );

    for ( esint32 i = 0; i < MaxParamSource; i++ ) {
        ppt_iter = param_proc_map.find((param_source)i);

		if (ppt_iter == param_proc_map.end()) {
			const char *key_str = _get_param_src_str ( ( param_source ) i );
            elog ( "param source %s cant find", key_str );
        }
        else {
            ///RendererParamEntry rpe = ( RendererParamEntry ) data.vptr_var;
			RendererParamEntry rpe = ppt_iter->second;
#if BIT_WIDTH == 32
            elog ( "source %s, proc %x, type %d, size %d", _get_param_src_str ( ( param_source ) ppt_iter->first ), rpe->get_value_proc, rpe->type, rpe->array_size );
#elif BIT_WIDTH == 64
            elog ( "source %s, proc %llx, type %d, size %d", _get_param_src_str ( ( param_source ) ppt_iter->first ), (ref_ptr)rpe->get_value_proc, rpe->type, rpe->array_size );
#endif
        }
    }
	elog ( "%s", "########" );
}
RendererBase::RendererBase ( ViewportPtr view, RenderableSorter* sorter )
{
	if (sorter)
		renderable_sorter = sorter;
	else
		renderable_sorter = ENEW DefaultRenderableSorter;

	default_rend_matrix = Matrix4x4_new();
	Matrix4x4_set_one(default_rend_matrix);
    render_plane = RenderablePlane_new();
    VertexDecl pdec = RenderablePlane_get_vertex_declaration ( render_plane );

    display_pass = NULL;
    clear_sketchbook_pass = create_clear_sketchbook_pass ( pdec );

    camera_base = Camera_new ( view->width, view->height );
    x = view->x;
    y = view->y;
    width = view->width;
    height = view->height;
    Camera_translate ( camera_base, 0.0, 0.0, 1.5f );

    curt_render_cam = camera_base;

    ///used_renderable_tree = Tree_new ( Vptr, Vptr, Ealloc, Efree );
    ///unused_renderable_tree = Tree_new ( Vptr, Vptr, Ealloc, Efree );

    material_table = Tree_new ( String, Vptr, (MALLOC)Ealloc, (MFREE)Efree );
    var key, data;
    key.str_var = "default_material";
    MaterialPrototype mp;
    {
        SdrNdGen sng = ISdrNdGen.New();
        ISdrNdGen.register_default_nodes ( sng );

        SDisplayProc s;
        s.proc = default_material_proc3;
        mp = MaterialPrototype_new ( pdec, 2,
                                     s,
                                     Shaded, false );
        ISdrNdGen.Delete ( sng );
    }
    data.vptr_var = mp;
    Tree_insert ( material_table, key, data );
    material_count = 3;

    own_camera = true;
    own_material_table = true;
}

RendererBase::RendererBase ( RendererBase *prev_renderer, bool inherit_material_table, RenderableSorter* sorter )
{
	if (sorter)
		renderable_sorter = sorter;
	else
		renderable_sorter = ENEW DefaultRenderableSorter;

	default_rend_matrix = Matrix4x4_new();
	Matrix4x4_set_one(default_rend_matrix);
    render_plane = RenderablePlane_new();
    VertexDecl pdec = RenderablePlane_get_vertex_declaration ( render_plane );

    display_pass = NULL;
    clear_sketchbook_pass = NULL;

    camera_base = prev_renderer->camera_base;
    x = prev_renderer->x;
    y = prev_renderer->y;
    width = prev_renderer->width;
    height = prev_renderer->height;

    curt_render_cam = camera_base;

    ///used_renderable_tree = Tree_new ( Vptr, Vptr, Ealloc, Efree );
    ///unused_renderable_tree = Tree_new ( Vptr, Vptr, Ealloc, Efree );

    if ( !inherit_material_table ) {
        material_table = Tree_new ( String, Vptr, (MALLOC)Ealloc, (MFREE)Efree );
        var key, data;
        key.str_var = "default_material";
        MaterialPrototype mp;
        {
            SdrNdGen sng = ISdrNdGen.New();
            ISdrNdGen.register_default_nodes ( sng );

            SDisplayProc s;
            s.proc = default_material_proc3;
            mp = MaterialPrototype_new ( pdec, 2,
                                         s,
                                         Shaded, false );
            ISdrNdGen.Delete ( sng );
        }
        data.vptr_var = mp;
        Tree_insert ( material_table, key, data );
        material_count = 3;
        own_material_table = true;
    }

    else {
        material_table = prev_renderer->material_table;
        material_count = prev_renderer->material_count;
        own_material_table = false;
    }

    own_camera = false;
}

void RendererBase::Dest()
{
	Matrix4x4_delete(default_rend_matrix);
    RenderablePlane_delete ( render_plane );

    if ( display_pass ) {
        Pass_delete ( display_pass );
    }

    if ( clear_sketchbook_pass ) {
        Pass_delete ( clear_sketchbook_pass );
    }
/**
    _renderable_tree_dest ( used_renderable_tree );
    _renderable_tree_dest ( unused_renderable_tree );
**/
	_renderable_set_dest(used_renderable_set);
	_renderable_set_dest(unused_renderable_set);
    if ( own_camera ) {
        Camera_delete ( camera_base );
    }

    if ( own_material_table ) {
        Iterator iter = Tree_begin ( material_table );

        while ( iter ) {
            var data = Tree_get_value ( iter );
            MaterialPrototype mp = ( MaterialPrototype ) data.vptr_var;
            MaterialPrototype_delete ( mp );
            iter = Tree_next ( iter );
        }

        Tree_Dest ( material_table );
    }
}

Renderable RendererBase::new_renderable ( VertexDecl _dec, MaterialInstance* _m_inst, e_mesh_mode _mesh_mode )
{
    ///Renderable ret = ( Renderable ) SMalloc ( sizeof ( renderable ) );
    Renderable ret = ENEW renderable;
    ret->vtx_dec = RenderSystem_register_vertex_declaration ( _dec );
    ret->vtx_buf = VertexBuffer_new ( _dec );
    ret->idx_buf = IndexBuffer_new ( _mesh_mode );
    ///ret->mesh_list = List_new ( Vptr, Ealloc, Efree );
    ret->material = _m_inst->Clone();
    ret->std_pass_dirty_flag = true;
    ret->depth_pass_dirty_flag = true;
    ret->shadow_pass_dirty_flag = true;
    matrix4x4* mat = Matrix4x4_new();

    Matrix4x4_set_one ( mat );
    ret->world_matrix = mat;
    ret->ower = ( Renderer * ) this;
    ///var key, data;
    ///key.vptr_var = ret;
    ///data.vptr_var = ret;
    ///Tree_insert ( unused_renderable_tree, key, data );
	unused_renderable_set.insert(ret);
    return ret;
}

void RendererBase::use_renderable ( Renderable _rbl )
{
	STD_NAMESPACE::set<Renderable>::iterator iter = unused_renderable_set.find(_rbl);
	if (iter != unused_renderable_set.end())
	{
		used_renderable_set.insert(_rbl);
		unused_renderable_set.erase(iter);
	}
}

void RendererBase::clear_renderables()
{
    _renderable_set_dest(used_renderable_set);
	_renderable_set_dest(unused_renderable_set);
}

void RendererBase::get_mouse_ray ( int _x, int _y, EFloat3 *ori, EFloat3 *dir )
{
    int window_width, window_height;
    RenderSystem_get_window_width_height ( &window_width, &window_height );

    int totel_x = _x - ( int ) x;
    int totel_y = _y - ( int ) y;

    *ori = Camera_screen_position_to_world_position ( curt_render_cam, ( float ) totel_x, ( float ) totel_y );
    sfloat3 s_ori = SFloat3_assign_from_efloat3 ( ori );
    EFloat3 cam_pos = Camera_get_position ( curt_render_cam );
    sfloat3 s_cam_pos = SFloat3_assign_from_efloat3 ( &cam_pos );

	if (!Camera_is_orthogonal(curt_render_cam)) {
		sfloat3 s_dir = SFloat3_sub ( s_ori, s_cam_pos );
		s_dir = SFloat3_normalize ( s_dir );
		*dir = SFloat3_convert ( s_dir );
	}
	else {
		dir->x = 0.0f;
		dir->y = 0.0f;
		dir->z = 1.0f;
	}
}
