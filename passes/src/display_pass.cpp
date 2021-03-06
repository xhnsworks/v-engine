#include "display_pass.h"
#include "pass_console.h"
#include "lighting.h"
#include "array.h"
#include "shader_buffer.h"
#include "branch_node.h"
#include "shader_node.h"
#include "std_pass.h"
#include "estring.h"
#include "eassert.h"

#include "shader_object_table.h"

#include "shader_node_generator.h"
#include "shader_log.h"

ShaderNode create_material_id_test_node(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "MaterialIDTest");

    ShaderNode_add_input_param(ret, Int_Obj, "CurrentMaterialID", 1);
    ShaderNode_add_input_param(ret, Float4_Obj, "MaterialIDPixel", 1);
    ShaderNode_set_function(ret,
                            "{\n"
                            "    int CurrentPixelMaterialID = int(MaterialIDPixel.w * 255.0);\n"
                            "    if (CurrentMaterialID != CurrentPixelMaterialID)\n"
                            "    {\n"
                            "        discard;\n"
                            "    }\n"
                            "}\n");
    return ret;
}

ShaderNode create_display_pixel_shader_node(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                            "    vec3 cmap = texture(ColorMap, vTexCoord).rgb;\n"
                            "    vec3 lmap = texture(DiffuseLightingMap, vTexCoord).rgb;\n"
#else
							"    vec3 cmap = texture2D(ColorMap, vTexCoord).rgb;\n"
							"    vec3 lmap = texture2D(DiffuseLightingMap, vTexCoord).rgb;\n"
#endif
                            "    gl_FragData[0] = vec4( clamp(cmap * lmap, 0.0, 1.0), 1.0 );"
                            "}\n");

    return ret;
}

ShaderNode create_display_normal_map_node(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                            "    vec3 nmap = texture(NormalMap, vTexCoord).rgb;\n"
#else
							"    vec3 nmap = texture2D(NormalMap, vTexCoord).rgb;\n"
#endif
                            "    vec3 n;\n"
                            "    n.xy = nmap.xy * 2.0 - 1.0;\n"
                            "    n.z = sqrt(1.0 - dot(n.xy, n.xy));\n"
                            "    gl_FragData[0] = vec4( n, 1.0 );"
                            "}\n");

    return ret;
}

ShaderNode create_display_lighting_map_node(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                            "    vec3 lmap = texture(DiffuseLightingMap, vTexCoord).rgb;\n"
#else
							"    vec3 lmap = texture2D(DiffuseLightingMap, vTexCoord).rgb;\n"
#endif
                            "    gl_FragData[0] = vec4( lmap, 1.0 );"
                            "}\n");

    return ret;
}

ShaderNode create_display_depth_map_node(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                            "    vec3 nmap = texture(NormalMap, vTexCoord).rgb;\n"
#else
							"    vec3 nmap = texture2D(NormalMap, vTexCoord).rgb;\n"
#endif
                            "    float depth = nmap.z * 0.05;\n"
                            "    gl_FragData[0] = vec4( depth, depth, depth, 1.0 );"
                            "}\n");

    return ret;
}

ShaderNode create_color_output_node(VertexDecl _dec, shader_object_type _col_type, render_target _tgt)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "ColorOutput");

#define BUFFER_SIZE (4 * 1024)
    char mbuf[BUFFER_SIZE];
    char* str = mbuf;
    int remainder = BUFFER_SIZE - 1;
    int offs = 0;

    const char* render_target_str = get_render_target_string(_tgt);
    
    shader_object_type type = _col_type;

    if      (Float_Obj == type)
        offs = snprintf(str, remainder, "{    \n%s = vec4(Color, Color, Color, 1.0);\n}", render_target_str);
    else if (Float2_Obj == type)
        offs = snprintf(str, remainder, "{    \n%s = vec4(Color, 1.0, 1.0);\n}", render_target_str);
    else if (Float3_Obj == type)
        offs = snprintf(str, remainder, "{    \n%s = vec4(Color, 1.0);\n}", render_target_str);
    else if (Float4_Obj == type)
        offs = snprintf(str, remainder, "{    \n%s = Color;\n}", render_target_str);
    else
        EAssert(1, "%s", "shader type error!");

    ShaderNode_set_function(ret, mbuf);
    ShaderNode_add_input_param(ret, type, "Color", 1);
    return ret;
}

ShaderNode create_display_position_map_node(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                            "    vec3 pmap = texture(PositionMap, vTexCoord).rgb;\n"
#else
							"    vec3 pmap = texture2D(PositionMap, vTexCoord).rgb;\n"
#endif
                            "    gl_FragData[0] = vec4( pmap, 1.0 );"
                            "}\n");

    return ret;
}

PxlSdrBuf create_display_pixel_shader_buffer(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        PxlSdrBuf ret = {NULL};
        return ret;
    }

    PxlSdrBuf ret = IPxlSdrBuf._New();
    ///ShaderBuffer sb = to_ShaderBuffer(ret);

    const char* prefix = EString_new("v");

    for (euint32 i = 0; i < VertexDecl_count(_dec); i++)
    {
        VertexElement ele = VertexDecl_find(_dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);
        const char* vary_str = EString_add(prefix, sem_str);
        esint32 src = get_param_source(sem);
        IPxlSdrBuf.add_varying((ShaderBuffer)ret, pam_type, vary_str, src);
        EString_delete(vary_str);
    }
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, COLOR_MAP, 1, ColorSketch);
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, NORMAL_MAP, 1, NormalSketch);
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, "MaterialIDMap", 1, MaterialIDSketch);
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, DEPTH_MAP, 1, Plaster);
    
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, "DiffuseLightingMap", 1, DiffuseLightingSketch);
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, "SpecularLightingMap", 1, SpecularLightingSketch);

	IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32_Param, CAMERA_PLANE_WIDTH, 1, CameraPlaneWidth);
	IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32_Param, CAMERA_PLANE_HEIGHT, 1, CameraPlaneHeight);
	IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32_Param, CAMERA_PLANE_NEAR, 1, CameraPlaneNear);
	IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32_Param, CAMERA_PLANE_FAR, 1, CameraPlaneFar);

	IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Matrix4x4_Param, INVERT_CAMERA_WORLD_MATRIX, 1, InvertCameraWorldMatrix);
	IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Matrix4x4_Param, INVERT_CAMERA_PROJECTION_MATRIX, 1, InvertCameraProjectionMatrix);

    EString_delete(prefix);

    return ret;
}

PxlSdrBuf create_display_texture_pixel_shader_buffer(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        PxlSdrBuf ret = {NULL};
        return ret;
    }

    PxlSdrBuf ret = IPxlSdrBuf._New();

    const char* prefix = EString_new("v");

    for (euint32 i = 0; i < VertexDecl_count(_dec); i++)
    {
        VertexElement ele = VertexDecl_find(_dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);
        const char* vary_str = EString_add(prefix, sem_str);
        esint32 src = get_param_source(sem);
        IPxlSdrBuf.add_varying((ShaderBuffer)ret, pam_type, vary_str, src);
        EString_delete(vary_str);
    }
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, "ColorMap", 1, ColorMap0);
    EString_delete(prefix);
    return ret;
}

PxlSdrBuf create_clear_buffer_buffer(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        PxlSdrBuf ret = {NULL};
        return ret;
    }

    PxlSdrBuf ret = IPxlSdrBuf._New();

    const char* prefix = EString_new("v");

    for (euint32 i = 0; i < VertexDecl_count(_dec); i++)
    {
        VertexElement ele = VertexDecl_find(_dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);
        const char* vary_str = EString_add(prefix, sem_str);
        esint32 src = get_param_source(sem);
        IPxlSdrBuf.add_varying((ShaderBuffer)ret, pam_type, vary_str, src);
        EString_delete(vary_str);
    }

    EString_delete(prefix);

    return ret;
}

Pass create_display_texture_pass(VertexDecl _dec)
{
    char mbuf[STRING_BUFFER_SIZE];

    VtxSdrBuf vsb = create_std_vertex_shader_buffer(_dec, false, false);
    PxlSdrBuf psb = create_display_texture_pixel_shader_buffer(_dec);

    ShaderNode vsn = create_std_vertex_shader_node(_dec, vsb, false, false);
    ShaderNode map_sample_node = create_map_sample_node_rgba();
    ShaderNode color_output_node = create_color_output_node(_dec, Float4_Obj, RenderTarget0);

    IVtxSdrBuf.add_prototype_node((ShaderBuffer)vsb, vsn);
    IVtxSdrBuf.add_reference_node((ShaderBuffer)vsb, ShaderNode_get_name(vsn));

    IPxlSdrBuf.add_prototype_node((ShaderBuffer)psb, map_sample_node);
    IPxlSdrBuf.add_prototype_node((ShaderBuffer)psb, color_output_node);

    ShaderObject temp = IPxlSdrBuf.new_object((ShaderBuffer)psb, Float4_Obj, "Temp", 1);

    {
        ShaderNode ref_map_sample_node = IPxlSdrBuf.add_reference_node((ShaderBuffer)psb, ShaderNode_get_name(map_sample_node));
        ShaderNode_add_input_link(ref_map_sample_node,
                                  IPxlSdrBuf.find_object((ShaderBuffer)psb, "ColorMap"),
                                  INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(ref_map_sample_node,
                                  IPxlSdrBuf.find_object((ShaderBuffer)psb, "vTexCoord"),
                                  INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(ref_map_sample_node, temp, INVALID_ARRAY_INDEX);
    }
    {
        ShaderNode ref_color_output_node = IPxlSdrBuf.add_reference_node((ShaderBuffer)psb, ShaderNode_get_name(color_output_node));
        ShaderNode_add_input_link(ref_color_output_node, temp, INVALID_ARRAY_INDEX);
    }

    IVtxSdrBuf.complete(vsb);
    IPxlSdrBuf.complete(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();

    ShaderBuffer sb = to_ShaderBuffer(vsb);

#ifdef MARK_GLSL_VERSION
	snprintf(mbuf, STRING_BUFFER_SIZE - 1,
		"#version %d%d0\n%s", GLSL_MAIN_VERSION, GLSL_SUB_VERSION, sb->output);
#else
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "%s", sb->output);
#endif
    Shader_load_from_string(auto_vs, mbuf, VertexShader);
    sb = to_ShaderBuffer(psb);

#ifdef MARK_GLSL_VERSION
	snprintf(mbuf, STRING_BUFFER_SIZE - 1,
		"#version %d%d0\n%s", GLSL_MAIN_VERSION, GLSL_SUB_VERSION, sb->output);
#else
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "%s", sb->output);
#endif
    Shader_load_from_string(auto_ps, mbuf, PixelShader);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    ShaderNode_delete(map_sample_node);
    ShaderNode_delete(color_output_node);

    IVtxSdrBuf._Delete(vsb);
    IPxlSdrBuf._Delete(psb);
    return ret;
}

#include "shader_test.h"
void default_material_proc_ex(PxlSdrBuf _psb, BranchNode _bn)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);

    _tex_2d_obj_test(sb, "ColorMap");
    _tex_2d_obj_test(sb, "DiffuseLightingMap");
    _vec2_obj_test(sb, "vTexCoord");

    ShaderNode psn = ShaderNode_new();

    ShaderNode_set_name(psn, "DefaultPixelProc");

    ShaderNode_set_function(psn,
                            "{\n"
                            "    discard;\n"
                            "}\n");

    IPxlSdrBuf.add_prototype_node((ShaderBuffer)_psb, psn);

    CircuitBoard cb = BranchNode_set_else_branch(_bn);
    CircuitBoard_add_reference_node(cb, psn);

    ShaderNode_delete(psn);
}

Pass create_debug_display_pass(VertexDecl _dec, DebugRenderOutput _db_rdr_out)
{
    VtxSdrBuf vsb = create_std_vertex_shader_buffer(_dec, false, false);
    PxlSdrBuf psb = create_display_pixel_shader_buffer(_dec);
    ShaderNode vsn = create_std_vertex_shader_node(_dec, vsb, false, false);

    ShaderBuffer sb = to_ShaderBuffer(psb);
    SdrNdGen sng = ISdrNdGen.New();
    ISdrNdGen.register_default_nodes(sng);
    ISdrNdGen.attach_all_prototype_nodes(sng, sb);

	ShaderObject tex;
    
	ShaderObject tex_crd = IPxlSdrBuf.find_object(to_ShaderBuffer(psb), TEXCOORD);
	switch (_db_rdr_out) 
	{
    case ColorDebug:
		tex = ShaderBuffer_find_object(to_ShaderBuffer(psb), COLOR_MAP);
		{
			ShaderObject pixel = IPxlSdrBuf.new_object(to_ShaderBuffer(psb), Float3_Obj, "Pixel", 1);
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, MapSampleNode);
				ShaderNode_add_input_link(node, tex, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, tex_crd, INVALID_ARRAY_INDEX);
				ShaderNode_add_output_link(node, pixel, INVALID_ARRAY_INDEX);
			}
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, LightingOutputNode);
				ShaderNode_add_input_link(node, pixel, INVALID_ARRAY_INDEX);
			}
		}
		break;
	case NormalDebug:
		tex = IPxlSdrBuf.find_object(to_ShaderBuffer(psb), NORMAL_MAP);
		{
			ShaderObject pixel_rgba = IPxlSdrBuf.new_object(to_ShaderBuffer(psb),
                                                            Float4_Obj,
                                                            "PixelRGBA",
                                                            1);
			ShaderObject pixel_rgb = IPxlSdrBuf.new_object(to_ShaderBuffer(psb),
                                                           Float3_Obj,
                                                           "PixelRGB",
                                                           1);
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, MapSampleNodeRGBA);
				ShaderNode_add_input_link(node, tex, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, tex_crd, INVALID_ARRAY_INDEX);
				ShaderNode_add_output_link(node, pixel_rgba, INVALID_ARRAY_INDEX);
			}
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, NormalDecodeNode);
				ShaderNode_add_input_link(node, pixel_rgba, INVALID_ARRAY_INDEX);
                ShaderNode_add_output_link(node, pixel_rgb, INVALID_ARRAY_INDEX);
			}
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, LightingOutputNode);
				ShaderNode_add_input_link(node, pixel_rgb, INVALID_ARRAY_INDEX);
			}
		}
		break;
	case PositionDebug:
		tex = IPxlSdrBuf.find_object(to_ShaderBuffer(psb), DEPTH_MAP);
		{
			ShaderObject pixel_rgba = IPxlSdrBuf.new_object(to_ShaderBuffer(psb),
                                                            Float4_Obj,
                                                            "PixelRGBA",
                                                            1);
			ShaderObject tgt_pos = IPxlSdrBuf.new_object(to_ShaderBuffer(psb),
                                                         Float3_Obj,
                                                         "PixelRGB",
                                                         1);
			ShaderObject width =  IPxlSdrBuf.find_object(to_ShaderBuffer(psb),
                                                         CAMERA_PLANE_WIDTH);
			ShaderObject height =  IPxlSdrBuf.find_object(to_ShaderBuffer(psb),
                                                          CAMERA_PLANE_HEIGHT);
			ShaderObject near_plane =  IPxlSdrBuf.find_object(to_ShaderBuffer(psb),
                                                              CAMERA_PLANE_NEAR);
			ShaderObject far_plane =  IPxlSdrBuf.find_object(to_ShaderBuffer(psb),
                                                             CAMERA_PLANE_FAR);
			ShaderObject inv_cam_world_mat =
            IPxlSdrBuf.find_object(to_ShaderBuffer(psb),
                                   INVERT_CAMERA_WORLD_MATRIX);
			ShaderObject inv_cam_proj_mat =
            IPxlSdrBuf.find_object(to_ShaderBuffer(psb),
                                   INVERT_CAMERA_PROJECTION_MATRIX);
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, MapSampleNodeRGBA);
				ShaderNode_add_input_link(node, tex, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, tex_crd, INVALID_ARRAY_INDEX);
				ShaderNode_add_output_link(node, pixel_rgba, INVALID_ARRAY_INDEX);
			}
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, PositionDecodeNode);
				ShaderNode_add_input_link(node, tex_crd, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, pixel_rgba, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, near_plane, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, far_plane, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, width, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, height, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, inv_cam_world_mat, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, inv_cam_proj_mat, INVALID_ARRAY_INDEX);
				
				ShaderNode_add_output_link(node, tgt_pos, INVALID_ARRAY_INDEX);
			}
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, LightingOutputNode);
				ShaderNode_add_input_link(node, tgt_pos, INVALID_ARRAY_INDEX);
			}
		}
		break;
	case DepthDebug:
		tex = IPxlSdrBuf.find_object(to_ShaderBuffer(psb), DEPTH_MAP);
		{
			ShaderObject pixel = IPxlSdrBuf.new_object(to_ShaderBuffer(psb), Float3_Obj, "Pixel", 1);
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, MapSampleNode);
				ShaderNode_add_input_link(node, tex, INVALID_ARRAY_INDEX);
				ShaderNode_add_input_link(node, tex_crd, INVALID_ARRAY_INDEX);
				ShaderNode_add_output_link(node, pixel, INVALID_ARRAY_INDEX);
			}
			{
				ShaderNode node = ISdrNdGen.add_reference_node_1(sng, LightingOutputNode);
				ShaderNode_add_input_link(node, pixel, INVALID_ARRAY_INDEX);
			}
		}
		break;
	default:
		return NULL;
	}
	

    IVtxSdrBuf.add_prototype_node((ShaderBuffer)vsb, vsn);
    IVtxSdrBuf.add_reference_node((ShaderBuffer)vsb, ShaderNode_get_name(vsn));

    IVtxSdrBuf.complete(vsb);
    IPxlSdrBuf.complete(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();

    char mbuf[STRING_BUFFER_SIZE];
    sb = to_ShaderBuffer(vsb);

#ifdef MARK_GLSL_VERSION
	snprintf(mbuf, STRING_BUFFER_SIZE - 1,
		"#version %d%d0\n%s", GLSL_MAIN_VERSION, GLSL_SUB_VERSION, sb->output);
#else
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "%s", sb->output);
#endif
    Shader_load_from_string(auto_vs, mbuf, VertexShader);

    sb = to_ShaderBuffer(psb);

#ifdef MARK_GLSL_VERSION
	snprintf(mbuf, STRING_BUFFER_SIZE - 1,
		"#version %d%d0\n%s", GLSL_MAIN_VERSION, GLSL_SUB_VERSION, sb->output);
#else
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "%s", sb->output);
#endif
    Shader_load_from_string(auto_ps, mbuf, PixelShader);
	slog(PostPassLog, "%s", mbuf);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);

    Tree vtx_param_src_tree = IVtxSdrBuf.sell_param_source_object_tree((ShaderBuffer)vsb);
    Tree pxl_param_src_tree = IPxlSdrBuf.sell_param_source_object_tree((ShaderBuffer)psb);
    Pass_buy_vertex_param_source_tree(ret, vtx_param_src_tree);
    Pass_buy_pixel_param_source_tree(ret, pxl_param_src_tree);

    IVtxSdrBuf._Delete(vsb);
    IPxlSdrBuf._Delete(psb);
	ISdrNdGen.Delete(sng);
    return ret;
}

Pass create_display_pass_ex2(VertexDecl _dec, material_decl* _mat_decls)
{
    VtxSdrBuf vsb = create_std_vertex_shader_buffer(_dec, false, false);
    PxlSdrBuf psb = create_display_pixel_shader_buffer(_dec);
    ShaderNode vsn = create_std_vertex_shader_node(_dec, vsb, false, false);
    
    ShaderBuffer sb = to_ShaderBuffer(psb);
    SdrNdGen sng = ISdrNdGen.New();
    ISdrNdGen.register_default_nodes(sng);
    ISdrNdGen.attach_all_prototype_nodes(sng, sb);

    IVtxSdrBuf.add_prototype_node((ShaderBuffer)vsb, vsn);
    IVtxSdrBuf.add_reference_node((ShaderBuffer)vsb, ShaderNode_get_name(vsn));

    ShaderObject materialID_pixel = IPxlSdrBuf.new_object((ShaderBuffer)psb,
                                                          Float4_Obj,
                                                          "MaterialIDPixel",
                                                          1);

    {
        ShaderNode ref_map_sample_node = ISdrNdGen.add_reference_node_1(sng,
                                                                        MapSampleNodeRGBA);
        ShaderNode_add_input_link(ref_map_sample_node,
                                  IPxlSdrBuf.find_object((ShaderBuffer)psb,
                                                         "MaterialIDMap"),
                                  INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(ref_map_sample_node,
                                  IPxlSdrBuf.find_object((ShaderBuffer)psb,
                                                         "vTexCoord"),
                                  INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(ref_map_sample_node,
                                   materialID_pixel,
                                   INVALID_ARRAY_INDEX);
    }

    BranchNode bn = BranchNode_new();

    component_index comp_idx;
    comp_idx.num_comps = 1;
    comp_idx.comps[0] = CompW;
    ShaderObject mat_id = ShaderObject_get_component(materialID_pixel, comp_idx, INVALID_ARRAY_INDEX);
    ShaderObject int_mat_id = ShaderObject_float_to_int(mat_id, 255.0f);

    euint32 n = array_n(_mat_decls);
    for (euint32 i = 0; i < n; i++)
    {
        material_decl decl = _mat_decls[i];
        ShaderBuffer sb = to_ShaderBuffer(psb);

        BranchNode _bn = bn;
        ShaderObject _id_obj = int_mat_id;
        int _id = decl.mat_id;

        ShaderNode psn = decl.disp_proc(psb, decl.mat_id);

        ShaderBuffer_add_prototype_node(sb, psn);

        ShaderObject imme_so = ShaderBuffer_new_immediate_int_object(sb, _id);
        CircuitBoard cb = BranchNode_add_branch(_bn, _id_obj, Equal, imme_so);
        CircuitBoard_add_reference_node(cb, psn);

        ShaderNode_delete(psn);
    }

    default_material_proc_ex(psb, bn);

    ShaderObject_delete(mat_id);
    ShaderObject_delete(int_mat_id);

    IPxlSdrBuf.add_branch_node((ShaderBuffer)psb, bn);

    IVtxSdrBuf.complete(vsb);
    IPxlSdrBuf.complete(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();

    char mbuf[STRING_BUFFER_SIZE];
    sb = to_ShaderBuffer(vsb);

#ifdef MARK_GLSL_VERSION
	snprintf(mbuf, STRING_BUFFER_SIZE - 1,
		"#version %d%d0\n%s", GLSL_MAIN_VERSION, GLSL_SUB_VERSION, sb->output);
#else
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "%s", sb->output);
#endif
    Shader_load_from_string(auto_vs, mbuf, VertexShader);

    sb = to_ShaderBuffer(psb);

#ifdef MARK_GLSL_VERSION
	snprintf(mbuf, STRING_BUFFER_SIZE - 1,
		"#version %d%d0\n%s", GLSL_MAIN_VERSION, GLSL_SUB_VERSION, sb->output);
#else
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "%s", sb->output);
#endif
    Shader_load_from_string(auto_ps, mbuf, PixelShader);
	slog(PostPassLog, "%s", mbuf);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    BranchNode_delete(bn);

    Tree vtx_param_src_tree = IVtxSdrBuf.sell_param_source_object_tree((ShaderBuffer)vsb);
    Tree pxl_param_src_tree = IPxlSdrBuf.sell_param_source_object_tree((ShaderBuffer)psb);
    Pass_buy_vertex_param_source_tree(ret, vtx_param_src_tree);
    Pass_buy_pixel_param_source_tree(ret, pxl_param_src_tree);

    IVtxSdrBuf._Delete(vsb);
    IPxlSdrBuf._Delete(psb);
    return ret;
}

Pass create_clear_buffer_pass(VertexDecl _dec, SdrNdGen _shader_node_gen)
{
    VtxSdrBuf vsb = create_std_vertex_shader_buffer(_dec, false, false);
    PxlSdrBuf psb = create_clear_buffer_buffer(_dec);
    ShaderNode vsn = create_std_vertex_shader_node(_dec, vsb, false, false);

    ShaderBuffer sb;

    IVtxSdrBuf.add_prototype_node((ShaderBuffer)vsb, vsn);
    IVtxSdrBuf.add_reference_node((ShaderBuffer)vsb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);

    ISdrNdGen.attach_all_prototype_nodes(_shader_node_gen, sb);
    ISdrNdGen.add_reference_node_1(_shader_node_gen, ClearSketchNode);

    IVtxSdrBuf.complete(vsb);
    IPxlSdrBuf.complete(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);

    char mbuf[STRING_BUFFER_SIZE];
#ifdef MARK_GLSL_VERSION
	snprintf(mbuf, STRING_BUFFER_SIZE - 1,
		"#version %d%d0\n%s",
        GLSL_MAIN_VERSION,
        GLSL_SUB_VERSION,
        sb->output);
#else
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "%s",
             sb->output);
#endif
    Shader_load_from_string(auto_vs, mbuf, VertexShader);

    sb = to_ShaderBuffer(psb);
#ifdef MARK_GLSL_VERSION
	snprintf(mbuf, STRING_BUFFER_SIZE - 1,
		"#version %d%d0\n%s",
        GLSL_MAIN_VERSION,
        GLSL_SUB_VERSION,
        sb->output);
#else
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "%s", sb->output);
#endif
    Shader_load_from_string(auto_ps, mbuf, PixelShader);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    IVtxSdrBuf._Delete(vsb);
    IPxlSdrBuf._Delete(psb);
    return ret;
}
