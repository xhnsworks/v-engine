#include "lighting_pass.h"
#include "pass_console.h"
#include "lighting.h"
#include "array.h"
#include "shader_buffer.h"
#include "shader_node.h"
#include "branch_node.h"
#include "switch_node.h"
#include "estring.h"
#include "texture1d.h"
#include "renderer.h"
#include "tree.h"
#include "eassert.h"
#define POINT_LIGHT_AFFECT_DISTANCE "7.8"

#define USE_DEPTH_MAP

static bool g_display_pos = false;

///=========================create_lighting_shader_node============================///

ShaderNode create_position_decode_node()
{
    ShaderNode ret = ShaderNode_new();

    ShaderNode_set_name(ret, "PositionDecode");
    ShaderNode_set_function(ret,
                            "{\n"
                            "    float x = ( TexCoord.x - 0.5 ) * 2.0;\n"
                            "    float y = ( TexCoord.y - 0.5 ) * 2.0;\n"
                            "    vec4 pos = vec4(x, y, (Enc.r - 0.5) * 2.0, 1.0);\n"
                            "    pos = InvProjectionMatrix * pos;\n"
                            "    pos = vec4(pos.xyz / pos.w, 1.0);\n"
                            "    pos = InvCameraMatrix * pos;\n"
                            "    return pos.xyz;\n"
                            "}\n");
    ShaderNode_add_input_param(ret, Float2_Obj,    "TexCoord", 1);
    ShaderNode_add_input_param(ret, Float4_Obj,    "Enc", 1);
    ShaderNode_add_input_param(ret, Float_Obj,     "Near", 1);
    ShaderNode_add_input_param(ret, Float_Obj,     "Far", 1);
    ShaderNode_add_input_param(ret, Float_Obj,     "Width", 1);
    ShaderNode_add_input_param(ret, Float_Obj,     "Height", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "InvCameraMatrix", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "InvProjectionMatrix", 1);
    ShaderNode_set_return_type(ret, Float3_Obj, 1);

    return ret;
}

ShaderNode create_normal_decode_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "NormalDecode");
    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec3 ret;\n"
                            "    ret.xy = Enc.xy * 2.0 - 1.0;\n"
                            "    ret.z = sqrt(1.0 - dot(ret.xy, ret.xy));\n"
                            "    return ret;\n"
                            "}\n");
    ShaderNode_add_input_param(ret, Float4_Obj, "Enc", 1);
    ShaderNode_set_return_type(ret, Float3_Obj, 1);
    return ret;
}

ShaderNode create_lighting_vertex_shader_node(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }

    const char* prefix = EString_new("v");

    const char* fun_str = EString_new("{\n"
                                      "    vec4 pos = vec4(Position, 1.0);\n"
                                      "    gl_Position = pos;\n"
                                      "    vPosition = pos.xyz;\n");

    for (uint32 i = 0; i < VertexDecl_count(_dec); i++)
    {
        VertexElement ele = VertexDecl_find(_dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        element_type sem_type = get_element_semantic_type(sem);
        const char* sem_str = get_element_semantic_string(sem);

        if (sem_type != EmptyType)
        {
            const char* vary_str = EString_add(prefix, sem_str);

            if (sem != Position)
            {
                const char* tmp0 = EString_new("    ");
                const char* tmp1 = EString_add(tmp0, vary_str);
                const char* tmp2 = EString_add(tmp1, " = ");
                const char* tmp3 = EString_add(tmp2, sem_str);
                const char* tmp4 = EString_add(tmp3, ";\n");
                const char* tmp5 = fun_str;
                fun_str = EString_add(fun_str, tmp4);
                EString_delete(tmp0);
                EString_delete(tmp1);
                EString_delete(tmp2);
                EString_delete(tmp3);
                EString_delete(tmp4);
                EString_delete(tmp5);
            }
            EString_delete(vary_str);
        }
    }

    const char* tmp = fun_str;
    fun_str = EString_add(fun_str, "}\n");
    EString_delete(tmp);

    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "VertexProc");
    ShaderNode_set_function(ret, fun_str);

    EString_delete(fun_str);
    EString_delete(prefix);
    return ret;
}

VertexShaderBuffer create_lighting_vertex_shader_buffer(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        VertexShaderBuffer ret = {NULL};
        return ret;
    }

    VertexShaderBuffer ret = VertexShaderBuffer_new();
    ShaderBuffer sb = to_ShaderBuffer(ret);

    const char* prefix = EString_new("v");

    for (uint32 i = 0; i < VertexDecl_count(_dec); i++)
    {
        VertexElement ele = VertexDecl_find(_dec, i);

        element_semantic sem = VertexElement_get_semantic(ele);
        element_type sem_type = get_element_semantic_type(sem);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);

        if (sem_type != EmptyType)
        {
            VertexShaderBuffer_add_attribute(ret, sem, sem_type);
            const char* vary_str = EString_add(prefix, sem_str);
            sint32 src = get_param_source(sem);
            ShaderBuffer_add_varying(sb, pam_type, vary_str, src);
            EString_delete(vary_str);
        }
    }

    EString_delete(prefix);
    return ret;
}

PixelShaderBuffer create_lighting_pixel_shader_buffer(Renderer _rdr, VertexDecl _dec, bool _has_lighting_map)
{
    if (!VertexDecl_test(_dec))
    {
        PixelShaderBuffer ret = {NULL};
        return ret;
    }

    PixelShaderBuffer ret = PixelShaderBuffer_new();
    ShaderBuffer sb = to_ShaderBuffer(ret);

    const char* prefix = EString_new("v");

    for (uint32 i = 0; i < VertexDecl_count(_dec); i++)
    {
        VertexElement ele = VertexDecl_find(_dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);
        const char* vary_str = EString_add(prefix, sem_str);
        sint32 src = get_param_source(sem);
        ShaderBuffer_add_varying(sb, pam_type, vary_str, src);
        EString_delete(vary_str);
    }

    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, NormalSketch, NORMAL_MAP);
    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, Plaster, "DepthMap");
    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, CameraPlaneWidth, "CamWidth");

    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, CameraPlaneHeight, "CamHeight");
    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, CameraNearPlane, "CamNearPlane");
    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, CameraFarPlane, "CamFarPlane");

    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, PointLightPosition, POINT_LIGHT_POSITIOIN);
    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, PointLightColor, POINT_LIGHT_COLOR);
    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, PointLightAtteCoef, POINT_LIGHT_ATTE_COEF);

    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, InvertCameraWorldMatrix, "InvCamWorldMatrix");
    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, InvertCameraProjectionMatrix, "InvCamProjMatrix");
    ShaderBuffer_add_uniform_from_renderer(sb, _rdr, MaterialIDSketch, "MaterialIDMap");

    if (_has_lighting_map)
    {
        ShaderBuffer_add_uniform_from_renderer(sb, _rdr, LightingSketch, "LightingMap");
    }

    EString_delete(prefix);
    return ret;
}

ShaderNode create_point_array_lighting_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PointArrayLighting");


    const char* light_proc = NULL;
    if (MAX_LIGHTS_PER_PASS > 1)
    {
        light_proc =
            "        vec3 LightPos = LightPosArray[i];\n"
            "        vec4 LightColor = LightColorArray[i];\n"
            "        float LightAtteCoef = LightAtteCoefArray[i];\n";
    }
    else
    {
        light_proc =
            "        vec3 LightPos = LightPosArray;\n"
            "        vec4 LightColor = LightColorArray;\n"
            "        float LightAtteCoef = LightAtteCoefArray;\n";
    }

    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "{\n"
             "    vec3 ret = Lighting;\n"
             ///"    for (int i = 0; i < LightCount; i++)\n"
             "    {\n"
             "%s"
             "        vec3 lit_dir = LightPos - TargetPos;\n"
             "        float a = length(lit_dir);\n"
             "        lit_dir = normalize(lit_dir);\n"
             "        a = clamp( ("POINT_LIGHT_AFFECT_DISTANCE" - a) * (1.0 - LightAtteCoef), 0.0, 1.0);\n"
             "        a = pow(a, 5);\n"
             "        float d = dot(lit_dir, TargetNor);\n"
             "        d = clamp(d, 0.0, 1.0);\n"
             "        vec3 c = LightColor.rgb * d * a;\n"
             "        ret += c;\n"
             "    }\n"
             "    return ret;\n"
             "}\n",
             light_proc);

    ShaderNode_set_function(ret, mbuf);

    ShaderNode_add_input_param(ret, Float3_Obj, "Lighting", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "TargetNor", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightPosArray", MAX_LIGHTS_PER_PASS);
    ShaderNode_add_input_param(ret, Float4_Obj, "LightColorArray", MAX_LIGHTS_PER_PASS);
    ShaderNode_add_input_param(ret, Float_Obj, "LightAtteCoefArray", MAX_LIGHTS_PER_PASS);
    ///ShaderNode_add_input_param(ret, Int_Obj, "LightCount", 1);

    ShaderNode_set_return_type(ret, Float3_Obj, 1);
    return ret;
}
/**
ShaderNode create_point_lighting_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PointLighting");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec3 lit_dir = normalize(LightPos - TargetPos);\n"
                            "    float d = dot(lit_dir, TargetNor);\n"
                            "    d = -d;\n"
                            "    vec3 c = LightColor.rgb * d * 5.0f;\n"
                            "    return c.rgb;\n"
                            "}\n");

    ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "TargetNor", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightPos", 1);
    ShaderNode_add_input_param(ret, Float4_Obj, "LightColor", 1);
    ShaderNode_add_input_param(ret, Float_Obj, "AtteCoef", 1);

    ShaderNode_set_return_type(ret, Float3_Obj, 1);
    return ret;
}
**/
ShaderNode create_lighting_accumulator_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "LightingAccumulator");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    return LightingValue0 + LightingValue1;\n"
                            "}\n");

    ShaderNode_add_input_param(ret, Float3_Obj, "LightingValue0", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightingValue1", 1);
    ShaderNode_set_return_type(ret, Float3_Obj, 1);
    return ret;
}

ShaderNode create_lighting_output_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "Output");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    gl_FragData[0] = vec4(LightingValue, 1.0);\n"
                            "}\n");

    ShaderNode_add_input_param(ret, Float3_Obj, "LightingValue", 1);
    return ret;
}

void default_point_lighting_proc(PixelShaderBuffer _pxl_sdr_buf, ShaderNodeGenerator _sdr_node_gtr, bool _has_lighting_map)
{
    ShaderBuffer sb = to_ShaderBuffer(_pxl_sdr_buf);
    ShaderNode rgb_sampler_node = ShaderNodeGenerator_generate_shader_node(_sdr_node_gtr, MapSampleNode);
    ShaderNode rgba_sampler_node = ShaderNodeGenerator_generate_shader_node(_sdr_node_gtr, MapSampleNodeRGBA);

    ShaderNode nor_decode_node = ShaderNodeGenerator_generate_shader_node(_sdr_node_gtr, NormalDecodeNode);
    ShaderNode pos_decode_node = ShaderNodeGenerator_generate_shader_node(_sdr_node_gtr, PositionDecodeNode);
    ShaderNode point_array_lighting_node = ShaderNodeGenerator_generate_shader_node(_sdr_node_gtr, PointLightingNode);
    ShaderNode clear_lighting_value_node = ShaderNodeGenerator_generate_shader_node(_sdr_node_gtr, ClearLightingValueNode);
    ShaderNode lighting_accumulator_node = ShaderNodeGenerator_generate_shader_node(_sdr_node_gtr, LightingAccumulatorNode);
    ShaderNode output_node = ShaderNodeGenerator_generate_shader_node(_sdr_node_gtr, LightingOutputNode);

    ShaderBuffer_add_prototype_node(sb, rgb_sampler_node);
    ShaderBuffer_add_prototype_node(sb, rgba_sampler_node);
    ShaderBuffer_add_prototype_node(sb, nor_decode_node);
    ShaderBuffer_add_prototype_node(sb, pos_decode_node);
    ShaderBuffer_add_prototype_node(sb, point_array_lighting_node);
    ShaderBuffer_add_prototype_node(sb, clear_lighting_value_node);
    ShaderBuffer_add_prototype_node(sb, lighting_accumulator_node);
    ShaderBuffer_add_prototype_node(sb, output_node);

    ShaderObject nor = ShaderBuffer_new_object(sb, Float4_Obj, "Normal", 1);
    ShaderObject depth = ShaderBuffer_new_object(sb, Float4_Obj, "Depth", 1);
    ShaderObject tgt_pos = ShaderBuffer_new_object(sb, Float3_Obj, "TargetPosition", 1);
    ShaderObject tgt_nor = ShaderBuffer_new_object(sb, Float3_Obj, "TargetNormal", 1);
    ShaderObject lt_value = ShaderBuffer_new_object(sb, Float3_Obj, "LightingValue", 1);
    ShaderObject nor_map = ShaderBuffer_find_object(sb, NORMAL_MAP);
    ShaderObject depth_map = ShaderBuffer_find_object(sb, DEPTH_MAP);
    ShaderObject tex_crd = ShaderBuffer_find_object(sb, TEXCOORD);
    ShaderObject width =  ShaderBuffer_find_object(sb, "CamWidth");
    ShaderObject height =  ShaderBuffer_find_object(sb, "CamHeight");
    ShaderObject near_plane =  ShaderBuffer_find_object(sb, "CamNearPlane");
    ShaderObject far_plane =  ShaderBuffer_find_object(sb, "CamFarPlane");

    ShaderObject inv_cam_world_mat =  ShaderBuffer_find_object(sb, "InvCamWorldMatrix");
    ShaderObject inv_cam_proj_mat =  ShaderBuffer_find_object(sb, "InvCamProjMatrix");

    ShaderObject pt_pos_array = ShaderBuffer_find_object(sb, POINT_LIGHT_POSITIOIN);
    ShaderObject pt_col_array = ShaderBuffer_find_object(sb, POINT_LIGHT_COLOR);
    ShaderObject pt_ac_array = ShaderBuffer_find_object(sb, POINT_LIGHT_ATTE_COEF);

    if (_has_lighting_map)
    {
        ShaderNode sampler_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(rgb_sampler_node));
        ShaderObject lt_map = ShaderBuffer_find_object(sb, LIGHTING_MAP);
        ShaderNode_add_input_link(sampler_node, lt_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, lt_value, INVALID_ARRAY_INDEX);
    }
    else
    {
        ShaderNode clear_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(clear_lighting_value_node));
        ShaderNode_set_result_link(clear_node, lt_value, INVALID_ARRAY_INDEX);
    }
    {
        ShaderNode sampler_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(rgba_sampler_node));
        ShaderNode_add_input_link(sampler_node, nor_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, nor, INVALID_ARRAY_INDEX);
        sampler_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(rgba_sampler_node));
        ShaderNode_add_input_link(sampler_node, depth_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, depth, INVALID_ARRAY_INDEX);
    }
    {
        ShaderNode decode_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(pos_decode_node));
        ShaderNode_add_input_link(decode_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, depth, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, near_plane, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, far_plane, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, width, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, height, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, inv_cam_world_mat, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, inv_cam_proj_mat, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(decode_node, tgt_pos, INVALID_ARRAY_INDEX);
    }
    {
        ShaderNode decode_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(nor_decode_node));
        ShaderNode_add_input_link(decode_node, nor, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(decode_node, tgt_nor, INVALID_ARRAY_INDEX);
    }
    {

        ShaderNode pt_array_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(point_array_lighting_node));
        ShaderNode_add_input_link(pt_array_node, lt_value, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, tgt_pos, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, tgt_nor, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, pt_pos_array, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, pt_col_array, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, pt_ac_array, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(pt_array_node, lt_value, INVALID_ARRAY_INDEX);
    }
    {
        ShaderNode out_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(output_node));
        if (g_display_pos)
            ShaderNode_add_input_link(out_node, tgt_pos, INVALID_ARRAY_INDEX);
        else
            ShaderNode_add_input_link(out_node, lt_value, INVALID_ARRAY_INDEX);
    }
}
///
///

void default_lighting_proc_ex(PixelShaderBuffer _psb, BranchNode _bn)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);

    ShaderNode psn = ShaderNode_new();

    ShaderNode_set_name(psn, "DefaultLightingProc");

    ShaderNode_set_function(psn,
                            "{\n"
                            "    discard;\n"
                            "}\n");

    ShaderBuffer_add_prototype_node(sb, psn);

    CircuitBoard cb = BranchNode_set_else_branch(_bn);
    CircuitBoard_add_reference_node(cb, psn);

    ShaderNode_delete(psn);
}

#define USE_SWITCH_NODE

Pass create_lighting_pass_ex2(Renderer _rdr, VertexDecl _dec, lighting_decl* _decls, bool _has_lighting_map)
{
    char mbuf[STRING_BUFFER_SIZE];

    ///ShaderNode null_node = {NULL};
    ///ShaderNode* node_array = array_new(ShaderNode, 20, null_node);

    VertexShaderBuffer vsb = create_lighting_vertex_shader_buffer(_dec);
    PixelShaderBuffer psb = create_lighting_pixel_shader_buffer(_rdr, _dec, _has_lighting_map);

    ShaderNode vsn = create_lighting_vertex_shader_node(_dec);
    ///apush(node_array, vsn);
    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));

    sb = to_ShaderBuffer(psb);

    ShaderNodeGenerator sng = ShaderNodeGenerator_new();
    ShaderNodeGenerator_register_default_nodes(sng);
    ShaderNodeGenerator_attach_all_prototype_nodes(sng, sb);

    ShaderObject mat_id_tex = ShaderBuffer_find_object(sb, "MaterialIDMap");
    ShaderObject uv = ShaderBuffer_find_object(sb, "vTexCoord");
    ShaderObject mat_id_pxl = ShaderBuffer_new_object(sb, Float4_Obj, "MaterialIDPixel", 1);

    ShaderNode rgba_map_sample_node = ShaderNodeGenerator_add_reference_node(sng, MapSampleNodeRGBA);
    ShaderNode_add_input_link(rgba_map_sample_node, mat_id_tex, INVALID_ARRAY_INDEX);
    ShaderNode_add_input_link(rgba_map_sample_node, uv, INVALID_ARRAY_INDEX);
    ShaderNode_add_output_link(rgba_map_sample_node, mat_id_pxl, INVALID_ARRAY_INDEX);

    component_index comp_idx;
    comp_idx.num_comps = 1;
    comp_idx.comps[0] = CompW;
    ShaderObject mat_id = ShaderObject_get_component(mat_id_pxl, comp_idx, INVALID_ARRAY_INDEX);
    ShaderObject int_mat_id = ShaderObject_float_to_int(mat_id, 255.0f);
#ifndef USE_SWITCH_NODE
    BranchNode bn = BranchNode_new();

    uint n = array_n(_decls);
    for (uint i = 0; i < n; i++)
    {
        ShaderObject imme = ShaderBuffer_new_immediate_int_object(sb, _decls[i].mat_id);
        CircuitBoard cb = BranchNode_add_branch(bn, int_mat_id, Equal, imme);
        _decls[i].light_proc(psb, sng, cb, _has_lighting_map);
    }
    default_lighting_proc_ex(psb, bn);
    ShaderBuffer_add_branch_node(sb, bn);
#else
    SwitchNode sn = SwitchNode_new();
    SwitchNode_set_switch_object(sn, int_mat_id);
    Tree inv_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);

    uint n = array_n(_decls);
    for (uint i = 0; i < n; i++)
    {
        LightingProc2 light_proc = _decls[i].light_proc;
        var key, data;
        Tree tag_set = NULL;
        key.vptr_var = light_proc;
        if (Tree_find(inv_tree, key, &data))
            tag_set = data.vptr_var;
        else
        {
            tag_set = Tree_new(Sint32, Sint32, Ealloc, Efree);
            key.vptr_var = light_proc;
            data.vptr_var = tag_set;
            Tree_insert(inv_tree, key, data);
        }
        var set_value;
        set_value.sint32_var = _decls[i].mat_id;
        Tree_insert(tag_set, set_value, set_value);
    }

    Iterator inv_iter = Tree_begin(inv_tree);
    while (inv_iter)
    {
        var key = Tree_get_key(inv_iter);
        var data = Tree_get_value(inv_iter);
        LightingProc2 light_proc = key.vptr_var;
        Tree tag_set = data.vptr_var;

        sint* tags = array_new(sint, Tree_count(tag_set), -1);
        Iterator tag_iter = Tree_begin(tag_set);
        while (tag_iter)
        {
            var data = Tree_get_value(tag_iter);
            apush(tags, data.sint32_var);
            tag_iter = Tree_next(tag_iter);
        }

        CircuitBoard cb = SwitchNode_add_branch_block(sn, tags);
        light_proc(psb, sng, cb, _has_lighting_map);

        array_delete(tags);

        inv_iter = Tree_next(inv_iter);
    }

    ShaderBuffer_add_branch_node(sb, sn);
#endif
    VertexShaderBuffer_release(vsb);
    PixelShaderBuffer_release(psb);

#ifndef USE_SWITCH_NODE
    BranchNode_delete(bn);
#else
    SwitchNode_delete(sn);

    inv_iter = Tree_begin(inv_tree);
    while (inv_iter)
    {
        var data = Tree_get_value(inv_iter);
        Tree tag_set = data.vptr_var;
        Tree_Dest(tag_set);
        ///Efree(tag_set);
        inv_iter = Tree_next(inv_iter);
    }
    Tree_Dest(inv_tree);
    ///Efree(inv_tree);
#endif
    ShaderObject_delete(mat_id);
    ShaderObject_delete(int_mat_id);

    ShaderNode_delete(vsn);
    ShaderNodeGenerator_delete(sng);

    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);
    elog(sb.self->output);
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "#version 400\n%s", sb.self->output);
    Shader_load_from_string(auto_vs, mbuf, VertexShader);
    ///
    ///elog("#release#");
    ///elog(mbuf);
    ///elog("#release#");
    ///
    sb = to_ShaderBuffer(psb);
    ///elog(sb.self->output);
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "#version 400\n%s", sb.self->output);
    Shader_load_from_string(auto_ps, mbuf, PixelShader);
    ///
    elog("#release#");
    elog(mbuf);
    elog("#release#");
    ///

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    Tree vtx_param_src_tree = ShaderBuffer_sell_param_source_object_tree(to_ShaderBuffer(vsb));
    Tree pxl_param_src_tree = ShaderBuffer_sell_param_source_object_tree(to_ShaderBuffer(psb));

    Pass_buy_vertex_param_source_tree(ret, vtx_param_src_tree);
    Pass_buy_pixel_param_source_tree(ret, pxl_param_src_tree);

    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}

ParamTable ParamTable_new()
{
    ParamTable ret;
    ret.param_entries = Tree_new(String, Sint32, Ealloc, Efree);
    return ret;
}
void ParamTable_delete(ParamTable _self)
{
    Tree_Dest(_self.param_entries);
    ///Efree(_self.param_entries);
}
void ParamTable_add_entry(ParamTable _self, const char* _name, sint32 _src)
{
    var key, data;
    key.str_var = _name;
    data.sint32_var = _src;
    Tree_insert(_self.param_entries, key, data);
}
void ParamTable_get_all_entrys(ParamTable _self, param_entry* _result)
{
    Iterator iter = Tree_begin(_self.param_entries);
    uint i = 0;
    while (iter)
    {
        var key = Tree_get_key(iter);
        var data = Tree_get_value(iter);
        _result[i].name = (const char*)key.str_var;
        _result[i].source = data.sint32_var;
        iter = Tree_next(iter);
        i++;
    }
}
