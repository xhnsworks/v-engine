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

void RendererBase::_renderable_tree_dest ( Tree _t )
{
    Iterator iter = Tree_begin ( _t );

    while ( iter ) {
        var data;
        data = Tree_get_value ( iter );
        renderable *rdl = ( renderable * ) data.vptr_var;
        VertexBuffer_delete ( rdl->vtx_buf );
        IndexBuffer_delete ( rdl->idx_buf );
		/**
        Iterator mesh_iter = List_begin ( rdl->mesh_list );

        while ( mesh_iter ) {
            var data = List_get_value ( mesh_iter );
            Mesh mesh = ( Mesh ) data.vptr_var;
            Mesh_delete ( mesh );
            mesh_iter = List_next ( mesh_iter );
        }
		**/
        ///List_Dest ( rdl->mesh_list );
        Matrix4x4_delete ( rdl->world_matrix );
        MaterialInstance_delete ( rdl->material );
        ///Mfree ( rdl );
        iter = Tree_next ( iter );

		delete rdl;
    }

    Tree_Dest ( _t );
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
        MaterialInstance_delete ( rdl->material );

		delete rdl;
    }
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

RendererParamEntry RendererParamEntry_new ( GetRendererParamProc _proc, param_type _type, esint _array_size )
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
	param_proc_tree = Tree_new ( Sint32, Vptr, (MALLOC)Ealloc, (MFREE)Efree );
    {
        var key, data;
        key.sint32_var = CameraPosition;
        data.vptr_var = RendererParamEntry_new ( getCameraPositionProc, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = CameraDirection;
        data.vptr_var = RendererParamEntry_new ( getCameraDirectionProc, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = CameraPlaneNear;
        data.vptr_var = RendererParamEntry_new ( getCameraPlaneNearProc, Float32_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = CameraPlaneFar;
        data.vptr_var = RendererParamEntry_new ( getCameraPlaneFarProc, Float32_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = CameraPlaneWidth;
        data.vptr_var = RendererParamEntry_new ( getCameraPlaneWidthProc, Float32_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = CameraPlaneHeight;
        data.vptr_var = RendererParamEntry_new ( getCameraPlaneHeightProc, Float32_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = CameraProjectionMatrix;
        data.vptr_var = RendererParamEntry_new ( getCameraProjectionMatrixProc, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = CameraWorldMatrix;
        data.vptr_var = RendererParamEntry_new ( getCameraWorldMatrixProc, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = RenderableWorldMatrix;
        data.vptr_var = RendererParamEntry_new ( getRenderableWorldMatrixProc, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = InvertCameraProjectionMatrix;
        data.vptr_var = RendererParamEntry_new ( getInvertCameraProjectionMatrixProc, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = InvertCameraWorldMatrix;
        data.vptr_var = RendererParamEntry_new ( getInvertCameraWorldMatrixProc, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = CurrentMaterialID;
        data.vptr_var = RendererParamEntry_new ( getCurrentMaterialIDProc, Float32_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ColorMap0;
        data.vptr_var = RendererParamEntry_new ( getColorMap0Proc, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = NormalMap0;
        data.vptr_var = RendererParamEntry_new ( getNormalMap0Proc, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = Plaster;
        data.vptr_var = RendererParamEntry_new ( getPlasterProc, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ColorSketch;
        data.vptr_var = RendererParamEntry_new ( getColorSketchProc, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = NormalSketch;
        data.vptr_var = RendererParamEntry_new ( getNormalSketchProc, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = MaterialIDSketch;
        data.vptr_var = RendererParamEntry_new ( getMaterialIDSketchProc, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightingSketch;
        data.vptr_var = RendererParamEntry_new ( getLightingSketchProc, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightInnerCos;
        data.vptr_var = RendererParamEntry_new ( getLightInnerCos, Float32_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightOuterCos;
        data.vptr_var = RendererParamEntry_new ( getLightOuterCos, Float32_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightPosition;
        data.vptr_var = RendererParamEntry_new ( getLightPosition, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightDirection;
        data.vptr_var = RendererParamEntry_new ( getLightDirection, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightColor;
        data.vptr_var = RendererParamEntry_new ( getLightColor, Float32x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightAtteCoef;
        data.vptr_var = RendererParamEntry_new ( getLightAtteCoef, Float32_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightNumEmitters;
        data.vptr_var = RendererParamEntry_new ( getLightNumShadowEmitters, Int_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightProjectionMatrix0;
        data.vptr_var = RendererParamEntry_new ( getLightProjMatrix0, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightProjectionMatrix1;
        data.vptr_var = RendererParamEntry_new ( getLightProjMatrix1, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightProjectionMatrix2;
        data.vptr_var = RendererParamEntry_new ( getLightProjMatrix2, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightProjectionMatrix3;
        data.vptr_var = RendererParamEntry_new ( getLightProjMatrix3, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightProjectionMatrix4;
        data.vptr_var = RendererParamEntry_new ( getLightProjMatrix4, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightProjectionMatrix5;
        data.vptr_var = RendererParamEntry_new ( getLightProjMatrix5, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightWorldMatrix0;
        data.vptr_var = RendererParamEntry_new ( getLightWorldMatrix0, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightWorldMatrix1;
        data.vptr_var = RendererParamEntry_new ( getLightWorldMatrix1, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightWorldMatrix2;
        data.vptr_var = RendererParamEntry_new ( getLightWorldMatrix2, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightWorldMatrix3;
        data.vptr_var = RendererParamEntry_new ( getLightWorldMatrix3, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightWorldMatrix4;
        data.vptr_var = RendererParamEntry_new ( getLightWorldMatrix4, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightWorldMatrix5;
        data.vptr_var = RendererParamEntry_new ( getLightWorldMatrix5, Matrix4x4_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowMap0;
        data.vptr_var = RendererParamEntry_new ( getShadowMap0, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowMap1;
        data.vptr_var = RendererParamEntry_new ( getShadowMap1, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowMap2;
        data.vptr_var = RendererParamEntry_new ( getShadowMap2, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowMap3;
        data.vptr_var = RendererParamEntry_new ( getShadowMap3, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowMap4;
        data.vptr_var = RendererParamEntry_new ( getShadowMap4, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowMap5;
        data.vptr_var = RendererParamEntry_new ( getShadowMap5, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowCubeMap;
        data.vptr_var = RendererParamEntry_new ( getShadowCubeMap, TextureCube_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = LightInfluence;
        data.vptr_var = RendererParamEntry_new ( getLightInfluence, Float32_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowDirection0;
        data.vptr_var = RendererParamEntry_new ( getShadowDir0, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowDirection1;
        data.vptr_var = RendererParamEntry_new ( getShadowDir1, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowDirection2;
        data.vptr_var = RendererParamEntry_new ( getShadowDir2, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowDirection3;
        data.vptr_var = RendererParamEntry_new ( getShadowDir3, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowDirection4;
        data.vptr_var = RendererParamEntry_new ( getShadowDir4, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ShadowDirection5;
        data.vptr_var = RendererParamEntry_new ( getShadowDir5, Float32x3_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = DiffuseLightingSketch;
        data.vptr_var = RendererParamEntry_new ( getDiffuseLightingSketchProc, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = SpecularLightingSketch;
        data.vptr_var = RendererParamEntry_new ( getSpecularLightingSketchProc, Texture2D_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );

        key.sint32_var = ColorSketchPixelSize;
        data.vptr_var = RendererParamEntry_new ( getColorSketchPixelSizeProc, Float32x2_Param, 1 );
        Tree_insert ( param_proc_tree, key, data );
    }

    Iterator ppt_iter = Tree_begin ( param_proc_tree );
    elog ( "%s", "########" );

    while ( ppt_iter ) {
        var key = Tree_get_key ( ppt_iter );
        const char *key_str = _get_param_src_str ( ( param_source ) key.sint32_var );
        elog ( "%s", key_str );
        ppt_iter = Tree_next ( ppt_iter );
    }

    elog ( "%s", "########" );

    for ( euint i = 0; i < MaxParamSource; i++ ) {
        var key, data;
        key.sint32_var = i;

        if ( !Tree_find ( param_proc_tree, key, &data ) ) {
            const char *key_str = _get_param_src_str ( ( param_source ) key.sint32_var );
            elog ( "param source %s cant find", key_str );
        }
        else {
            RendererParamEntry rpe = ( RendererParamEntry ) data.vptr_var;
            elog ( "source %s, proc %x, type %d", _get_param_src_str ( ( param_source ) key.sint32_var ), rpe->get_value_proc, rpe->type, rpe->array_size );
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

Renderable RendererBase::new_renderable ( VertexDecl _dec, MaterialInstance _m_inst, e_mesh_mode _mesh_mode )
{
    ///Renderable ret = ( Renderable ) SMalloc ( sizeof ( renderable ) );
    Renderable ret = ENEW renderable;
    ret->vtx_dec = RenderSystem_register_vertex_declaration ( _dec );
    ret->vtx_buf = VertexBuffer_new ( _dec );
    ret->idx_buf = IndexBuffer_new ( _mesh_mode );
    ///ret->mesh_list = List_new ( Vptr, Ealloc, Efree );
    ret->material = MaterialInstance_clone ( _m_inst );
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
	/**
    var key, data;
    key.vptr_var = _rbl;
    Iterator iter = Tree_find ( unused_renderable_tree, key, &data );

    if ( iter ) {
        key.vptr_var = _rbl;
        data.vptr_var = _rbl;
        Tree_insert ( used_renderable_tree, key, data );
        Tree_remove ( unused_renderable_tree, key );
    }
	**/
	STD_NAMESPACE::set<Renderable>::iterator iter = unused_renderable_set.find(_rbl);
	if (iter != unused_renderable_set.end())
	{
		used_renderable_set.insert(_rbl);
		unused_renderable_set.erase(iter);
	}
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
