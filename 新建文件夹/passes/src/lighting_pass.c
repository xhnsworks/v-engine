#include "lighting_pass.h"
#include "pass_console.h"
#include "lighting.h"
#include "array.h"
#include "shader_buffer.h"
#include "shader_node.h"
#include "estring.h"
#include "texture1d.h"
#define POINT_LIGHT_AFFECT_DISTANCE "6.5"
typedef struct _light_slot
{
    Light* light_array;
} light_slot;

LightSlot LightSlot_new()
{
    LightSlot ret;
    Light exec_light = {NULL};
    ret.self = (light_slot*)Malloc(sizeof(light_slot));
    ret.self->light_array = array_new(Light, 5, exec_light);
    return ret;
}

void LightSlot_delete(LightSlot _ls)
{
    for (uint i = 0; i < array_n(_ls.self->light_array); i++)
    {
        Light lt = _ls.self->light_array[i];
        Light_delete(lt);
    }
    array_delete(_ls.self->light_array);
    Mfree(_ls.self);
}

void LightSlot_push_light(LightSlot _ls, Light _lt)
{
    _ls.self->light_array = array_push(_ls.self->light_array, _lt);
}

Light LightSlot_get_light(LightSlot _ls, uint32 _i)
{
    return array_safe_get(_ls.self->light_array, _i);
}

uint32 LightSlot_get_count(LightSlot _ls)
{
    return array_n(_ls.self->light_array);
}

void LightSlot_apply(LightSlot _ls, Pass _pas)
{
    EFloat3 pos_array[MAX_LIGHTS_PER_PASS];
    EFloat4 col_array[MAX_LIGHTS_PER_PASS];
    EFloat  ac_array [MAX_LIGHTS_PER_PASS];
    int     pt_count = 0;

    for (uint32 i = 0; i < array_n(_ls.self->light_array); i++)
    {
        Light l = array_safe_get(_ls.self->light_array, i);
        light_type t = Light_get_type(l);
        switch (t)
        {
            case Point:
            {
                PointLight pl = to_point_light(l);
                pos_array[pt_count] = PointLight_get_position(pl);
                col_array[pt_count] = PointLight_get_color(pl);
                ac_array[pt_count] = PointLight_get_atte_coef(pl);
                pt_count++;
            }
            break;
            case Direction:
            case Spot:
            break;
        }
    }

    Pass_set_uniform_param_float3s(_pas, POINT_LIGHT_POSITIOIN_ARRAY, pos_array, MAX_LIGHTS_PER_PASS);
    Pass_set_uniform_param_float4s(_pas, POINT_LIGHT_COLOR_ARRAY, col_array, MAX_LIGHTS_PER_PASS);
    Pass_set_uniform_param_floats(_pas, POINT_LIGHT_ATTE_COEF_ARRAY, ac_array, MAX_LIGHTS_PER_PASS);
    Pass_set_uniform_param_int(_pas, POINT_LIGHT_COUNT, pt_count);
}

void LightSlot_make_param_textures(LightSlot _ls, Texture1D* _pt_tex, Texture1D* _spt_tex, Texture1D* _dt_tex)
{
    point_light_render_data exec_point_render_data =
    {0.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 0.0f,
     0.0f};

    point_light_render_data* point_light_buffer = array_new(point_light_render_data, 5, exec_point_render_data);

    for (uint32 i = 0; i < array_n(_ls.self->light_array); i++)
    {
        Light l = array_safe_get(_ls.self->light_array, i);
        light_type t = Light_get_type(l);
        switch (t)
        {
            case Point:
            {
                PointLight pl = to_point_light(l);

                point_light_render_data* render_data = PointLight_get_render_data(pl);
                point_light_buffer = array_push(point_light_buffer, *render_data);
            }
            break;
            case Direction:
            break;
            case Spot:
            break;
        }
    }
    if (!*_pt_tex)
    {
        *_pt_tex = Texture1D_new();
        Texture1D_create(*_pt_tex, RGBA32F, array_n(point_light_buffer));
    }

    Texture1D_load_from_mem(*_pt_tex, point_light_buffer, RGBA32F, array_n(point_light_buffer));
    array_delete(point_light_buffer);
}

///=========================create_lighting_shader_node============================///

ShaderNode create_position_decode_node()
{
                /**
                p = Float4_mul_float(www, p);
                p = Matrix4x4_mul_float4(maty, p);
                **/
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PositionDecode");
    ShaderNode_set_function(ret,
                            "{\n"
                            "    float w = Enc.z;\n"
                            "    float d = Enc.w;\n"
                            "    float x = (Texcoord.x - 0.5);\n"
                            "    float y = (Texcoord.y - 0.5);\n"
                            "    vec4 ret = vec4(x, y, d, 1.0) * w;\n"
                            "    ret = InvCameraMatrix * ret;\n"
                            ///"    ret.z -= 0.1;\n"
                            "    return vec4(ret.xyz, 1.0);\n"
                            "}\n");
    ShaderNode_add_input_param(ret, Float2_Obj,    "Texcoord", 1);
    ShaderNode_add_input_param(ret, Float4_Obj,    "Enc", 1);

    ShaderNode_add_input_param(ret, Float_Obj,     "Width", 1);
    ShaderNode_add_input_param(ret, Float_Obj,     "Height", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "InvCameraMatrix", 1);
    ShaderNode_set_return_type(ret, Float4_Obj, 1);
    return ret;
}

ShaderNode create_normal_decode_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "NormalDecode");
    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec4 ret;\n"
                            "    ret.xy = Enc.xy * 2.0 - 1.0;\n"
                            "    ret.z = sqrt(1.0 - dot(ret.xy, ret.xy));\n"
                            "    ret.w = 1.0;\n"
                            "    return ret;\n"
                            "}\n");
    ShaderNode_add_input_param(ret, Float4_Obj, "Enc", 1);
    ShaderNode_set_return_type(ret, Float4_Obj, 1);
    return ret;
}

ShaderNode create_lighting_vertex_shader_node(VertexDeclaration _dec)
{
    if (!VertexDeclaration_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }

    const char* prefix = EString_new("v");

    const char* fun_str = EString_new("{\n"
                                      "    vec4 pos = vec4(Position, 1);\n"
                                      "    gl_Position = pos;\n"
                                      "    vPosition = pos.xyz;\n");

    for (uint32 i = 0; i < VertexDeclaration_count(_dec); i++)
    {
        VertexElement ele = VertexDeclaration_find(_dec, i);
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

VertexShaderBuffer create_lighting_vertex_shader_buffer(VertexDeclaration _dec)
{
    if (!VertexDeclaration_test(_dec))
    {
        VertexShaderBuffer ret = {NULL};
        return ret;
    }

    VertexShaderBuffer ret = VertexShaderBuffer_new();
    ShaderBuffer sb = to_ShaderBuffer(ret);

    const char* prefix = EString_new("v");

    for (uint32 i = 0; i < VertexDeclaration_count(_dec); i++)
    {
        VertexElement ele = VertexDeclaration_find(_dec, i);

        element_semantic sem = VertexElement_get_semantic(ele);
        element_type sem_type = get_element_semantic_type(sem);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);

        if (sem_type != EmptyType)
        {
            VertexShaderBuffer_add_attribute(ret, sem, sem_type);
            const char* vary_str = EString_add(prefix, sem_str);
            ShaderBuffer_add_varying(sb, pam_type, vary_str);
            EString_delete(vary_str);
        }
    }

    EString_delete(prefix);
    return ret;
}

PixelShaderBuffer create_lighting_pixel_shader_buffer(VertexDeclaration _dec, bool _has_lighting_map)
{
    if (!VertexDeclaration_test(_dec))
    {
        PixelShaderBuffer ret = {NULL};
        return ret;
    }

    PixelShaderBuffer ret = PixelShaderBuffer_new();
    ShaderBuffer sb = to_ShaderBuffer(ret);

    const char* prefix = EString_new("v");

    for (uint32 i = 0; i < VertexDeclaration_count(_dec); i++)
    {
        VertexElement ele = VertexDeclaration_find(_dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);
        const char* vary_str = EString_add(prefix, sem_str);
        ShaderBuffer_add_varying(sb, pam_type, vary_str);
        EString_delete(vary_str);
    }

    ShaderBuffer_add_uniform(sb, Texture2D_Param, "NormalMap", 1);
    ShaderBuffer_add_uniform(sb, Texture2D_Param, "PositionMap", 1);

    ShaderBuffer_add_uniform(sb, Float32_Param, "CamWidth", 1);
    ShaderBuffer_add_uniform(sb, Float32_Param, "CamHeight", 1);
    ShaderBuffer_add_uniform(sb, Float32_Param, "CamNearPlane", 1);
    ShaderBuffer_add_uniform(sb, Float32_Param, "CamFarPlane", 1);

    ShaderBuffer_add_uniform(sb, Float32x3_Param, POINT_LIGHT_POSITIOIN_ARRAY, MAX_LIGHTS_PER_PASS);
    ShaderBuffer_add_uniform(sb, Float32x4_Param, POINT_LIGHT_COLOR_ARRAY, MAX_LIGHTS_PER_PASS);
    ShaderBuffer_add_uniform(sb, Float32_Param, POINT_LIGHT_ATTE_COEF_ARRAY, MAX_LIGHTS_PER_PASS);
    ShaderBuffer_add_uniform(sb, Int_Param, POINT_LIGHT_COUNT, 1);

    ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "InvCamRotaTranProjMatrix", 1);

    if (_has_lighting_map)
    {
        ShaderBuffer_add_uniform(sb, Texture2D_Param, "LightingMap", 1);
    }

    EString_delete(prefix);
    return ret;
}

ShaderNode create_point_array_lighting_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PointArrayLighting");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec3 ret = vec3(0.0, 0.0, 0.0);\n"

                            "    for (int i = 0; i < LightCount; i++)\n"
                            "    {\n"

                            "        vec3 LightPos = LightPosArray[i];\n"
                            "        vec4 LightColor = LightColorArray[i];\n"
                            "        float LightAtteCoef = LightAtteCoefArray[i];\n"

                            "        vec3 lit_dir = LightPos - TargetPos.xyz;\n"

                            "        float a = length(lit_dir);\n"
                            "        lit_dir = normalize(lit_dir);\n"

                            "        a = clamp( ("POINT_LIGHT_AFFECT_DISTANCE" - a) * (1.0 - LightAtteCoef), 0.0, 1.0);\n"
                            "        a = pow(a, 5);\n"

                            "        float d = dot(lit_dir, TargetNor.xyz);\n"
                            "        d = clamp(d, 0.0, 1.0);\n"

                            "        vec3 c = LightColor.rgb * d * a;\n"

                            "        ret += c;\n"

                            "    }\n"

                            "    return vec4(ret, 1.0);\n"
                            "}\n");

    ShaderNode_add_input_param(ret, Float4_Obj, "TargetPos", 1);
    ShaderNode_add_input_param(ret, Float4_Obj, "TargetNor", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightPosArray", MAX_LIGHTS_PER_PASS);
    ShaderNode_add_input_param(ret, Float4_Obj, "LightColorArray", MAX_LIGHTS_PER_PASS);
    ShaderNode_add_input_param(ret, Float_Obj, "LightAtteCoefArray", MAX_LIGHTS_PER_PASS);
    ShaderNode_add_input_param(ret, Int_Obj, "LightCount", 1);

    ShaderNode_set_return_type(ret, Float4_Obj, 1);
    return ret;
}

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
/**
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
**/
ShaderNode create_output_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "Output");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    gl_FragData[0] = vec4(LightingValue.xyz, 1);\n"
                            "}\n");

    ShaderNode_add_input_param(ret, Float4_Obj, "LightingValue", 1);
    return ret;
}

///
/// 第一光照管线输入只有法线图和坐标图，没有光照图
/// 输出有光照图
/// 以后的光照管线输入均有光照图
///
Pass create_lighting_pass(VertexDeclaration _dec, bool _has_lighting_map)
{
    ShaderNode null_node = {NULL};
    ///ShaderObject null_obj = {NULL};
    ShaderNode* node_array = array_new(ShaderNode, 20, null_node);
    ///ShaderObject* obj_array = array_new(ShaderObject, 5, null_obj);

    VertexShaderBuffer vsb = create_lighting_vertex_shader_buffer(_dec);
    PixelShaderBuffer psb = create_lighting_pixel_shader_buffer(_dec, _has_lighting_map);

    ShaderNode vsn = create_lighting_vertex_shader_node(_dec);

    ShaderNode pos_nor_sampler_node = create_map_sample4_node();
    ShaderNode nor_decode_node = create_normal_decode_node();
    ShaderNode pos_decode_node = create_position_decode_node();
    ShaderNode point_array_lighting_node = create_point_array_lighting_node();
    ShaderNode clear_lighting_value_node = create_clear_lighting_value_node();
    ///ShaderNode lighting_accumulator_node = create_lighting_accumulator_node();
    ShaderNode output_node = create_output_node();

    node_array = array_push(node_array, vsn);
    node_array = array_push(node_array, pos_nor_sampler_node);
    node_array = array_push(node_array, nor_decode_node);
    node_array = array_push(node_array, pos_decode_node);
    node_array = array_push(node_array, point_array_lighting_node);
    node_array = array_push(node_array, clear_lighting_value_node);
    ///node_array = array_push(node_array, lighting_accumulator_node);
    node_array = array_push(node_array, output_node);

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);

    ShaderBuffer_add_prototype_node(sb, pos_nor_sampler_node);
    ShaderBuffer_add_prototype_node(sb, nor_decode_node);
    ShaderBuffer_add_prototype_node(sb, pos_decode_node);

    ShaderBuffer_add_prototype_node(sb, point_array_lighting_node);
    ShaderBuffer_add_prototype_node(sb, clear_lighting_value_node);
    ///ShaderBuffer_add_prototype_node(sb, lighting_accumulator_node);
    ShaderBuffer_add_prototype_node(sb, output_node);

    ShaderObject tgt_pos = ShaderBuffer_new_object(sb, Float4_Obj, "TargetPosition", 1);
    ShaderObject tgt_nor = ShaderBuffer_new_object(sb, Float4_Obj, "TargetNormal", 1);
    ShaderObject lt_value = ShaderBuffer_new_object(sb, Float4_Obj, "LightingValue", 1);

    ShaderObject nor_map = ShaderBuffer_find_object(sb, NORMAL_MAP);
    ShaderObject pos_map = ShaderBuffer_find_object(sb, POSITION_MAP);
    ShaderObject tex_crd = ShaderBuffer_find_object(sb, TEXCOORD);

    ShaderObject width =  ShaderBuffer_find_object(sb, "CamWidth");
    ShaderObject height =  ShaderBuffer_find_object(sb, "CamHeight");
    ShaderObject near_plane =  ShaderBuffer_find_object(sb, "CamNearPlane");
    ShaderObject far_plane =  ShaderBuffer_find_object(sb, "CamFarPlane");

    ShaderObject inv_cam_mat =  ShaderBuffer_find_object(sb, "InvCamRotaTranProjMatrix");

    ShaderObject pt_pos_array = ShaderBuffer_find_object(sb, POINT_LIGHT_POSITIOIN_ARRAY);
    ShaderObject pt_col_array = ShaderBuffer_find_object(sb, POINT_LIGHT_COLOR_ARRAY);
    ShaderObject pt_ac_array = ShaderBuffer_find_object(sb, POINT_LIGHT_ATTE_COEF_ARRAY);
    ShaderObject pt_cnt = ShaderBuffer_find_object(sb, POINT_LIGHT_COUNT);

    if (_has_lighting_map)
    {
        ShaderNode sampler_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(pos_nor_sampler_node));
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
        ShaderNode sampler_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(pos_nor_sampler_node));
        ShaderNode_add_input_link(sampler_node, pos_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, tgt_pos, INVALID_ARRAY_INDEX);
    }
    {
        ShaderNode sampler_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(pos_nor_sampler_node));
        ShaderNode_add_input_link(sampler_node, nor_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, tgt_nor, INVALID_ARRAY_INDEX);
    }
/**
    {

        ShaderNode decode_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(pos_decode_node));
        ///ShaderNode_add_input_param(ret, Float2_Obj,    "Texcoord", 1);
        ///ShaderNode_add_input_param(ret, Float3_Obj,    "Enc", 1);
        ///ShaderNode_add_input_param(ret, Float_Obj,     "Width", 1);
        ///ShaderNode_add_input_param(ret, Float_Obj,     "Height", 1);
        ///ShaderNode_add_input_param(ret, Matrix4x4_Obj, "InvCameraMatrix", 1);
        ShaderNode_add_input_link(decode_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, tgt_nor, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, width, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, height, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, inv_cam_mat, INVALID_ARRAY_INDEX);

        ///ShaderNode_set_result_link(decode_node, tgt_pos, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(decode_node, lt_value, INVALID_ARRAY_INDEX);
    }
**/
    {
        ShaderNode decode_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(nor_decode_node));
        ShaderNode_add_input_link(decode_node, tgt_nor, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(decode_node, tgt_nor, INVALID_ARRAY_INDEX);
    }
    {

        ShaderNode pt_array_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(point_array_lighting_node));
        ShaderNode_add_input_link(pt_array_node, tgt_pos, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, tgt_nor, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, pt_pos_array, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, pt_col_array, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, pt_ac_array, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_array_node, pt_cnt, INVALID_ARRAY_INDEX);

        ShaderNode_set_result_link(pt_array_node, lt_value, INVALID_ARRAY_INDEX);
    }

    {
        ShaderNode out_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(output_node));
        ShaderNode_add_input_link(out_node, lt_value, INVALID_ARRAY_INDEX);
    }

    VertexShaderBuffer_release(vsb);
    PixelShaderBuffer_release(psb);

    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);
    ///
    elog(sb.self->output);
    ///
    Shader_load_from_string(auto_vs, sb.self->output, VertexShader);
    sb = to_ShaderBuffer(psb);

    Shader_load_from_string(auto_ps, sb.self->output, PixelShader);

    ///
    elog(sb.self->output);
    ///

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);
    for (uint i = 0; i < array_n(node_array); i++)
    {
        ShaderNode_delete(node_array[i]);
    }
    /**
    for (uint i = 0; i < array_n(obj_array); i++)
    {
        ShaderObject_delete(obj_array[i]);
    }
    **/
    array_delete(node_array);
    ///array_delete(obj_array);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}
