#include "lighting_model.h"
#include "emem.h"
#include "shader_object.h"
#include "shader_node.h"
#include "pixel_shader_buffer.h"
#include "shader_node_generator.h"
#include "pass_console.h"
#include "lighting_pass.h"
#include "estring.h"
#include "switch_node.h"
#include "shader_log.h"
#include "shadow_emitter.h"
#include "eassert.h"
#include "shader_object_table.h"

LightingModelRendererParamTable LightingModelRendererParamTable_new()
{
    LightingModelRendererParamTable ret = (LightingModelRendererParamTable)SMalloc(sizeof(lighting_model_renderer_param_table));
    ret->param_table = Tree_new(Sint32, String, Ealloc, Efree);
    return ret;
}
void LightingModelRendererParamTable_delete(LightingModelRendererParamTable _self)
{
    Tree_Dest(_self->param_table);
    Mfree(_self);
}

ShaderNode create_point_lighting_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PointLighting");

    char mbuf[STRING_BUFFER_SIZE];

    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "{\n"
             "    vec3 c = vec3(0.0, 0.0, 0.0);\n"
             "    vec3 s = vec3(0.0, 0.0, 0.0);\n"
             "    vec3 realLightDir = normalize(TargetPos - LightPos);\n"

             "    realLightDir = -realLightDir;\n"
             /// diffuse
             "    float d = dot(realLightDir, TargetNor);\n"
             "    d = clamp(d, 0.0, 1.0);\n"
             "    c = LightColor.rgb * d;\n"
             /// specular
             "    vec3 eye_dir = normalize(EyePos - TargetPos);\n"
             "    vec3 r = 2.0 * TargetNor * dot(realLightDir, TargetNor) + realLightDir;\n"
             "    r = normalize(r);\n"
             "    d = dot(r, eye_dir);\n"
             "    d = clamp(d, 0.0, 1.0);\n"
             "    s = pow(d, 16.0) * LightColor.rgb * 0.15;\n"

             "    OutDiffuse = InDiffuse + c * LightingWeight;\n"
             "    OutSpecular = InSpecular + s * LightingWeight;\n"
             "}\n"
            );

    ShaderNode_set_function(ret, mbuf);

    ShaderNode_add_input_param(ret, Float3_Obj, "InDiffuse", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "InSpecular", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "EyePos", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);

    ShaderNode_add_input_param(ret, Float3_Obj, "TargetNor", 1);
    ShaderNode_add_input_param(ret, Float4_Obj, "LightColor", 1);
    ShaderNode_add_input_param(ret, Float_Obj, "AtteCoef", 1);

    ///ShaderNode_add_input_param(ret, Float_Obj, "OuterCosHalfAngle", 1);
    ///ShaderNode_add_input_param(ret, Float_Obj, "InnerCosHalfAngle", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightPos", 1);

    ShaderNode_add_input_param(ret, Float_Obj, "TargetSpecular", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightingWeight", 1);
    ShaderNode_add_output_param(ret, Float3_Obj, "OutDiffuse", 1);
    ShaderNode_add_output_param(ret, Float3_Obj, "OutSpecular", 1);
    return ret;
}

ShaderNode create_spot_lighting_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "SpotLighting");

    char mbuf[STRING_BUFFER_SIZE];

    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "{\n"
             "    vec3 c = vec3(0.0, 0.0, 0.0);\n"
             "    vec3 s = vec3(0.0, 0.0, 0.0);\n"
             "    vec3 realLightDir = normalize(TargetPos - LightPos);\n"
             "    float d = dot(normalize(LightDir), realLightDir);\n"
             "    d = clamp(d, 0.0, 1.0);\n"

             ///"    float range = (1.0 - OuterCosHalfAngle) - (1.0 - InnerCosHalfAngle);\n"

             "    if (d > OuterCosHalfAngle)\n"
             "    {\n"
             "        float weight = 1.0 + (d - InnerCosHalfAngle) / (1.0 - InnerCosHalfAngle);\n"
             "        weight = clamp(weight, 0.0, 1.0);\n"
             ///"        LightDir = -LightDir;\n"
             "        realLightDir = -realLightDir;\n"
             /// diffuse
             "        d = dot(realLightDir, TargetNor);\n"
             "        d = clamp(d, 0.0, 1.0);\n"
             "        c = LightColor.rgb * d * weight;\n"
             /// specular
             "        vec3 eye_dir = normalize(EyePos - TargetPos);\n"
             "        vec3 r = 2.0 * TargetNor * dot(realLightDir, TargetNor) + realLightDir;\n"
             "        r = normalize(r);\n"
             "        d = dot(r, eye_dir);\n"
             "        d = clamp(d, 0.0, 1.0);\n"
             "        s = pow(d, 16.0) * LightColor.rgb * 0.15 * weight;\n"
             "    }\n"
             "    OutDiffuse = InDiffuse + c * LightingWeight;\n"
             "    OutSpecular = InSpecular + s * LightingWeight;\n"
             "}\n"
            );

    ShaderNode_set_function(ret, mbuf);

    ShaderNode_add_input_param(ret, Float3_Obj, "InDiffuse", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "InSpecular", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "EyePos", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);

    ShaderNode_add_input_param(ret, Float3_Obj, "TargetNor", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightDir", 1);
    ShaderNode_add_input_param(ret, Float4_Obj, "LightColor", 1);
    ShaderNode_add_input_param(ret, Float_Obj, "AtteCoef", 1);

    ShaderNode_add_input_param(ret, Float_Obj, "OuterCosHalfAngle", 1);
    ShaderNode_add_input_param(ret, Float_Obj, "InnerCosHalfAngle", 1);
    ///ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightPos", 1);

    ShaderNode_add_input_param(ret, Float_Obj, "TargetSpecular", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightingWeight", 1);
    ShaderNode_add_output_param(ret, Float3_Obj, "OutDiffuse", 1);
    ShaderNode_add_output_param(ret, Float3_Obj, "OutSpecular", 1);
    return ret;
}

ShaderNode create_direction_lighting_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "DirectionLighting");

    char mbuf[STRING_BUFFER_SIZE];

    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "{\n"
             /// diffuse
             "    LightDir = -LightDir;\n"
             "    float d = dot(LightDir, TargetNor);\n"
             "    d = clamp(d, 0.0, 1.0);\n"
             "    vec3 c = LightColor.rgb * d;\n"
             /// specular
             "    vec3 eye_dir = normalize(EyePos - TargetPos);\n"
             "    vec3 r = 2.0 * TargetNor * dot(LightDir, TargetNor) + LightDir;\n"
             "    r = normalize(r);\n"
             "    d = dot(r, eye_dir);\n"
             "    d = clamp(d, 0.0, 1.0);\n"
             "    vec3 s = pow(d, 16.0) * LightColor.rgb * 0.15;\n"
             /// output
             "    OutDiffuse = InDiffuse + c * LightingWeight;\n"
             "    OutSpecular = InSpecular + s * LightingWeight;\n"
             "}\n");
    ShaderNode_set_function(ret, mbuf);

    ShaderNode_add_input_param(ret, Float3_Obj, "InDiffuse", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "InSpecular", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "EyePos", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);

    ShaderNode_add_input_param(ret, Float3_Obj, "TargetNor", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightDir", 1);
    ShaderNode_add_input_param(ret, Float4_Obj, "LightColor", 1);
    ShaderNode_add_input_param(ret, Float_Obj, "AtteCoef", 1);

    ShaderNode_add_input_param(ret, Float_Obj, "TargetSpecular", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightingWeight", 1);
    ShaderNode_add_output_param(ret, Float3_Obj, "OutDiffuse", 1);
    ShaderNode_add_output_param(ret, Float3_Obj, "OutSpecular", 1);
    return ret;
}

ShaderNode create_diffuse_specular_out_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "DiffuseSpecularOutput");

    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             "{\n"
             "    gl_FragData[0] = vec4(InDiffuse, 1.0);\n"
             "    gl_FragData[1] = vec4(InSpecular, 1.0);\n"
             "}\n");

    ShaderNode_set_function(ret, mbuf);

    ShaderNode_add_input_param(ret, Float3_Obj, "InDiffuse", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "InSpecular", 1);
    return ret;
}

void lighting_template(LightState _lt_state, PxlSdrBuf _psb, SdrNdGen _sng, CircuitBoard _cb, bool _has_lighting_map)
{
    ShaderObject nor = IPxlSdrBuf.new_object((ShaderBuffer)_psb, Float4_Obj, "Normal", 1);
    ShaderObject depth = IPxlSdrBuf.new_object((ShaderBuffer)_psb, Float4_Obj, "Depth", 1);
    ShaderObject tgt_pos = IPxlSdrBuf.new_object((ShaderBuffer)_psb, Float3_Obj, "TargetPosition", 1);
    ShaderObject tgt_nor = IPxlSdrBuf.new_object((ShaderBuffer)_psb, Float3_Obj, "TargetNormal", 1);

    ShaderObject diffuse = IPxlSdrBuf.new_object((ShaderBuffer)_psb, Float3_Obj, "DiffuseValue", 1);
    ShaderObject specular = IPxlSdrBuf.new_object((ShaderBuffer)_psb, Float3_Obj, "SpecularValue", 1);
    ShaderObject view_light_dir = IPxlSdrBuf.new_object((ShaderBuffer)_psb, Float3_Obj, "ViewLightDir", 1);
    ShaderObject light_weight = IPxlSdrBuf.new_object((ShaderBuffer)_psb, Float3_Obj, "LightingWeight", 1);

    uint _num_shadow_map = _lt_state->num_shadow_emitters;
    EAssert(_num_shadow_map <= 6, "%s", "num shadow map is more than 6");

	ShaderObject shadow_cube_map = {NULL};

    ShaderObject shadow_maps[6];
    ShaderObject light_world_mats[6];
    ShaderObject light_proj_mats[6];
    ShaderObject shadow_dirs[6];

    if (_lt_state->type != Point)
    {
        for (uint i = 0; i < _num_shadow_map; i++)
        {
            char shadow_map[256];
            char light_world_matrix[256];
            char light_proj_matrix[256];
            char shadow_dir[256];
            snprintf(shadow_map, 255, "%s%d", SHADOW_MAP, i);
            snprintf(light_world_matrix, 255, "%s%d", LIGHT_WORLD_MATRIX, i);
            snprintf(light_proj_matrix, 255, "%s%d", LIGHT_PROJ_MATRIX, i);
            snprintf(shadow_dir, 255, "%s%d", SHADOW_DIRECTION, i);
            /**
            IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, ShadowMap0 + i, shadow_map);
            IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightWorldMatrix0 + i, light_world_matrix);
            IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightProjectionMatrix0 + i, light_proj_matrix);
            IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, ShadowDirection0 + i, shadow_dir);
            **/
            shadow_maps[i] = IPxlSdrBuf.find_object((ShaderBuffer)_psb, shadow_map);
            light_world_mats[i] = IPxlSdrBuf.find_object((ShaderBuffer)_psb, light_world_matrix);
            light_proj_mats[i] = IPxlSdrBuf.find_object((ShaderBuffer)_psb, light_proj_matrix);
            shadow_dirs[i] = IPxlSdrBuf.find_object((ShaderBuffer)_psb, shadow_dir);
        }
    }
    else
    {
        shadow_cube_map = IPxlSdrBuf.find_object((ShaderBuffer)_psb, SHADOW_CUBE_MAP);
    }

    ShaderObject nor_map = IPxlSdrBuf.find_object((ShaderBuffer)_psb, NORMAL_MAP);
    ShaderObject depth_map = IPxlSdrBuf.find_object((ShaderBuffer)_psb, DEPTH_MAP);
    ShaderObject tex_crd = IPxlSdrBuf.find_object((ShaderBuffer)_psb, TEXCOORD);
    ShaderObject width =  IPxlSdrBuf.find_object((ShaderBuffer)_psb, CAMERA_PLANE_WIDTH);
    ShaderObject height =  IPxlSdrBuf.find_object((ShaderBuffer)_psb, CAMERA_PLANE_HEIGHT);
    ShaderObject near_plane =  IPxlSdrBuf.find_object((ShaderBuffer)_psb, CAMERA_PLANE_NEAR);
    ShaderObject far_plane =  IPxlSdrBuf.find_object((ShaderBuffer)_psb, CAMERA_PLANE_FAR);

    ShaderObject inv_cam_world_mat =  IPxlSdrBuf.find_object((ShaderBuffer)_psb, INVERT_CAMERA_WORLD_MATRIX);
    ShaderObject inv_cam_proj_mat =  IPxlSdrBuf.find_object((ShaderBuffer)_psb, INVERT_CAMERA_PROJECTION_MATRIX);

    ShaderObject cam_world_mat = IPxlSdrBuf.find_object((ShaderBuffer)_psb, CAMERA_WORLD_MATRIX);
    ShaderObject cam_pos = IPxlSdrBuf.find_object((ShaderBuffer)_psb, CAMERA_POSITION);

    ShaderObject light_pos = IPxlSdrBuf.find_object((ShaderBuffer)_psb, LIGHT_POSITION);
    ShaderObject light_dir = IPxlSdrBuf.find_object((ShaderBuffer)_psb, LIGHT_DIRECTION);
    ShaderObject light_col = IPxlSdrBuf.find_object((ShaderBuffer)_psb, LIGHT_COLOR);
    ShaderObject light_ac = IPxlSdrBuf.find_object((ShaderBuffer)_psb, LIGHT_ATTE_COEF);

    ShaderObject light_inner_cos = IPxlSdrBuf.find_object((ShaderBuffer)_psb, LIGHT_INNER_COS);
    ShaderObject light_outer_cos = IPxlSdrBuf.find_object((ShaderBuffer)_psb, LIGHT_OUTER_COS);

    ShaderObject light_influence = IPxlSdrBuf.find_object((ShaderBuffer)_psb, LIGHT_INFLUENCE);

    if (_has_lighting_map)
    {
        ShaderNode sampler_node = ISdrNdGen.add_reference_node_2(_sng, _cb, MapSampleNode);
        ShaderObject diffuse_map = IPxlSdrBuf.find_object((ShaderBuffer)_psb, DIFFUSE_LIGHTING_MAP);
        ShaderNode_add_input_link(sampler_node, diffuse_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, diffuse, INVALID_ARRAY_INDEX);
	
        sampler_node = ISdrNdGen.add_reference_node_2(_sng, _cb, MapSampleNode);
        ShaderObject specular_map = IPxlSdrBuf.find_object((ShaderBuffer)_psb, SPECULAR_LIGHTING_MAP);
        ShaderNode_add_input_link(sampler_node, diffuse_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, specular, INVALID_ARRAY_INDEX);
    }
    else
    {
        ShaderNode clear_node = ISdrNdGen.add_reference_node_2(_sng, _cb, ClearLightingValueNode);
        ShaderNode_set_result_link(clear_node, diffuse, INVALID_ARRAY_INDEX);
        clear_node = ISdrNdGen.add_reference_node_2(_sng, _cb, ClearLightingValueNode);
        ShaderNode_set_result_link(clear_node, specular, INVALID_ARRAY_INDEX);
    }
    {
        ShaderNode sampler_node = ISdrNdGen.add_reference_node_2(_sng, _cb, MapSampleNodeRGBA);
        ShaderNode_add_input_link(sampler_node, nor_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, nor, INVALID_ARRAY_INDEX);
        sampler_node = ISdrNdGen.add_reference_node_2(_sng, _cb, MapSampleNodeRGBA);
        ShaderNode_add_input_link(sampler_node, depth_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, depth, INVALID_ARRAY_INDEX);
    }
    {
        ///ShaderNode decode_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(pos_decode_node));
        ShaderNode decode_node = ISdrNdGen.add_reference_node_2(_sng, _cb, PositionDecodeNode);
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
        if (_lt_state->type != Point)
        {
            if (_num_shadow_map == 1)
            {
                ShaderNode shadow_test_node = ISdrNdGen.add_reference_node_2(_sng, _cb, Shadow2DMapTestNode);
                /**
                ShaderNode_add_input_param(ret, Texture2D_Obj, "DepthMap", 1);
                ShaderNode_add_input_param(ret, Float3_Obj, "PixelWorldPosition", 1);
                ShaderNode_add_input_param(ret, Matrix4x4_Obj, "LightWorldMatrix", 1);
                ShaderNode_add_input_param(ret, Matrix4x4_Obj, "LightProjectionMatrix", 1);
                ShaderNode_add_input_param(ret, Float3_Obj, "LightPosition", 1);
                ShaderNode_add_input_param(ret, Float3_Obj, "LightDirection", 1);
                **/
                ShaderNode_add_input_link(shadow_test_node, shadow_maps[0], INVALID_ARRAY_INDEX);
                ShaderNode_add_input_link(shadow_test_node, tgt_pos, INVALID_ARRAY_INDEX);
                ShaderNode_add_input_link(shadow_test_node, light_world_mats[0], INVALID_ARRAY_INDEX);
                ShaderNode_add_input_link(shadow_test_node, light_proj_mats[0], INVALID_ARRAY_INDEX);
                ShaderNode_add_input_link(shadow_test_node, light_pos, INVALID_ARRAY_INDEX);
                ShaderNode_add_input_link(shadow_test_node, shadow_dirs[0], INVALID_ARRAY_INDEX);
                ShaderNode_set_result_link(shadow_test_node, light_weight, INVALID_ARRAY_INDEX);
            }
            else if (_num_shadow_map == 0)
            {
                ShaderNode white_screen = ISdrNdGen.add_reference_node_2(_sng, _cb, WhiteScreenNode);
                ShaderNode_set_result_link(white_screen, light_weight, INVALID_ARRAY_INDEX);
            }
        }
        else
        {
            ShaderNode shadow_test_node = ISdrNdGen.add_reference_node_2(_sng, _cb, ShadowCubeMapTestNode);

            ShaderNode_add_input_link(shadow_test_node, shadow_cube_map, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(shadow_test_node, tgt_pos, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(shadow_test_node, light_pos, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(shadow_test_node, light_influence, INVALID_ARRAY_INDEX);
            ShaderNode_set_result_link(shadow_test_node, light_weight, INVALID_ARRAY_INDEX);
        }
    }
    {
        ///ShaderNode decode_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(nor_decode_node));
        ShaderNode decode_node = ISdrNdGen.add_reference_node_2(_sng, _cb, NormalDecodeNode);
        ShaderNode_add_input_link(decode_node, nor, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(decode_node, tgt_nor, INVALID_ARRAY_INDEX);
    }
    {
        ///ShaderNode pt_array_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(point_array_lighting_node));
        component_index idx = {1, CompZ};
        ShaderObject spe = ShaderObject_get_component(nor, idx, 0);

        ShaderNode lighting_node = ISdrNdGen.add_reference_node_2(_sng, _cb, _lt_state->lighting_node_id);
        if (_lt_state->type == Direction)
        {
            /**
            ShaderNode_add_input_param(ret, Float3_Obj, "InDiffuse", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "InSpecular", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "EyePos", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);

            ShaderNode_add_input_param(ret, Float3_Obj, "TargetNor", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "LightDir", 1);
            ShaderNode_add_input_param(ret, Float4_Obj, "LightColor", 1);
            ShaderNode_add_input_param(ret, Float_Obj, "AtteCoef", 1);

            ShaderNode_add_input_param(ret, Float_Obj, "TargetSpecular", 1);
            ShaderNode_add_input_param(ret, Float_Obj, "LightingWeight", 1);
            ShaderNode_add_output_param(ret, Float3_Obj, "OutDiffuse", 1);
            ShaderNode_add_output_param(ret, Float3_Obj, "OutSpecular", 1);
            **/
            ShaderNode_add_input_link(lighting_node, diffuse, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, specular, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, cam_pos, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, tgt_pos, INVALID_ARRAY_INDEX);

            ShaderNode_add_input_link(lighting_node, tgt_nor, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_dir, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_col, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_ac, INVALID_ARRAY_INDEX);

            ShaderNode_add_input_link(lighting_node, spe, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_weight, INVALID_ARRAY_INDEX);
            ShaderNode_add_output_link(lighting_node, diffuse, INVALID_ARRAY_INDEX);
            ShaderNode_add_output_link(lighting_node, specular, INVALID_ARRAY_INDEX);
        }
        else if (_lt_state->type == Spot)
        {
            /**
            ShaderNode_add_input_param(ret, Float3_Obj, "InDiffuse", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "InSpecular", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "EyePos", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);

            ShaderNode_add_input_param(ret, Float3_Obj, "TargetNor", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "LightDir", 1);
            ShaderNode_add_input_param(ret, Float4_Obj, "LightColor", 1);
            ShaderNode_add_input_param(ret, Float_Obj, "AtteCoef", 1);

            /// sin相当于1.0 - cos，外半径sin值大于内半径sin值，这样便于理解些
            ShaderNode_add_input_param(ret, Float_Obj, "OuterSinHalfAngle", 1);
            ShaderNode_add_input_param(ret, Float_Obj, "InnerSinHalfAngle", 1);
            ///ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "LightPos", 1);

            ShaderNode_add_input_param(ret, Float_Obj, "TargetSpecular", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "LightingWeight", 1);
            ShaderNode_add_output_param(ret, Float3_Obj, "OutDiffuse", 1);
            ShaderNode_add_output_param(ret, Float3_Obj, "OutSpecular", 1);
            **/
            ShaderNode_add_input_link(lighting_node, diffuse, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, specular, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, cam_pos, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, tgt_pos, INVALID_ARRAY_INDEX);

            ShaderNode_add_input_link(lighting_node, tgt_nor, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_dir, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_col, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_ac, INVALID_ARRAY_INDEX);

            ShaderNode_add_input_link(lighting_node, light_outer_cos, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_inner_cos, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_pos, INVALID_ARRAY_INDEX);

            ShaderNode_add_input_link(lighting_node, spe, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_weight, INVALID_ARRAY_INDEX);
            ShaderNode_add_output_link(lighting_node, diffuse, INVALID_ARRAY_INDEX);
            ShaderNode_add_output_link(lighting_node, specular, INVALID_ARRAY_INDEX);
        }
        else if (_lt_state->type == Point)
        {
            /**
            ShaderNode_add_input_param(ret, Float3_Obj, "InDiffuse", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "InSpecular", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "EyePos", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);

            ShaderNode_add_input_param(ret, Float3_Obj, "TargetNor", 1);
            ShaderNode_add_input_param(ret, Float4_Obj, "LightColor", 1);
            ShaderNode_add_input_param(ret, Float_Obj, "AtteCoef", 1);

            ShaderNode_add_input_param(ret, Float_Obj, "OuterCosHalfAngle", 1);
            ShaderNode_add_input_param(ret, Float_Obj, "InnerCosHalfAngle", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "LightPos", 1);

            ShaderNode_add_input_param(ret, Float_Obj, "TargetSpecular", 1);
            ShaderNode_add_input_param(ret, Float3_Obj, "LightingWeight", 1);
            ShaderNode_add_output_param(ret, Float3_Obj, "OutDiffuse", 1);
            ShaderNode_add_output_param(ret, Float3_Obj, "OutSpecular", 1);
            **/
            ShaderNode_add_input_link(lighting_node, diffuse, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, specular, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, cam_pos, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, tgt_pos, INVALID_ARRAY_INDEX);

            ShaderNode_add_input_link(lighting_node, tgt_nor, INVALID_ARRAY_INDEX);
            ///ShaderNode_add_input_link(lighting_node, light_dir, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_col, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_ac, INVALID_ARRAY_INDEX);

            ///ShaderNode_add_input_link(lighting_node, light_outer_cos, INVALID_ARRAY_INDEX);
            ///ShaderNode_add_input_link(lighting_node, light_inner_cos, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_pos, INVALID_ARRAY_INDEX);

            ShaderNode_add_input_link(lighting_node, spe, INVALID_ARRAY_INDEX);
            ShaderNode_add_input_link(lighting_node, light_weight, INVALID_ARRAY_INDEX);
            ShaderNode_add_output_link(lighting_node, diffuse, INVALID_ARRAY_INDEX);
            ShaderNode_add_output_link(lighting_node, specular, INVALID_ARRAY_INDEX);
        }
    }
    {
        ShaderNode diffuse_specular_output_node = ISdrNdGen.add_reference_node_2(_sng, _cb, DiffuseSpecularOutputNode);
        ShaderNode_add_input_link(diffuse_specular_output_node, diffuse, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(diffuse_specular_output_node, specular, INVALID_ARRAY_INDEX);
    }
}

PxlSdrBuf create_lighting_pixel_shader_buffer2(Renderer* _rdr, VertexDecl _dec, uint _num_shadow_map, light_type type, bool _has_lighting_map)
{
    /**
    if (!VertexDecl_test(_dec))
    {
        PxlSdrBuf ret = {NULL};
        return ret;
    }
    **/
    EAssert(VertexDecl_test(_dec), "%s", "vertex decl is invalid");
    EAssert(_num_shadow_map <= 6, "%s", "num shadow map is more than 6");

    PxlSdrBuf ret = IPxlSdrBuf._New();
    ///ShaderBuffer sb = to_ShaderBuffer(ret);

    const char* prefix = EString_new("v");

    for (uint32 i = 0; i < VertexDecl_count(_dec); i++)
    {
        VertexElement ele = VertexDecl_find(_dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);
        const char* vary_str = EString_add(prefix, sem_str);
        sint32 src = get_param_source(sem);
        IPxlSdrBuf.add_varying((ShaderBuffer)ret, pam_type, vary_str, src);
        EString_delete(vary_str);
    }

    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, NormalSketch, NORMAL_MAP);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, Plaster, DEPTH_MAP);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, CameraPlaneWidth, CAMERA_PLANE_WIDTH);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, CameraPlaneHeight, CAMERA_PLANE_HEIGHT);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, CameraPlaneNear, CAMERA_PLANE_NEAR);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, CameraPlaneFar, CAMERA_PLANE_FAR);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, CameraPosition, CAMERA_POSITION);

    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightPosition, LIGHT_POSITION);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightDirection, LIGHT_DIRECTION);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightColor, LIGHT_COLOR);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightAtteCoef, LIGHT_ATTE_COEF);

    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightInnerCos, LIGHT_INNER_COS);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightOuterCos, LIGHT_OUTER_COS);

    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, InvertCameraWorldMatrix, INVERT_CAMERA_WORLD_MATRIX);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, InvertCameraProjectionMatrix, INVERT_CAMERA_PROJECTION_MATRIX);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, CameraWorldMatrix, CAMERA_WORLD_MATRIX);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, CameraProjectionMatrix, CAMERA_PROJECTION_MATRIX);
    IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, MaterialIDSketch, "MaterialIDMap");

    if (_has_lighting_map)
    {
        IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, DiffuseLightingSketch, DIFFUSE_LIGHTING_MAP);
        IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, SpecularLightingSketch, SPECULAR_LIGHTING_MAP);
    }

    if (type != Point)
    {
        for (uint i = 0; i < _num_shadow_map; i++)
        {
            char shadow_map[256];
            char light_world_matrix[256];
            char light_proj_matrix[256];
            char shadow_dir[256];
            snprintf(shadow_map, 255, "%s%d", SHADOW_MAP, i);
            snprintf(light_world_matrix, 255, "%s%d", LIGHT_WORLD_MATRIX, i);
            snprintf(light_proj_matrix, 255, "%s%d", LIGHT_PROJ_MATRIX, i);
            snprintf(shadow_dir, 255, "%s%d", SHADOW_DIRECTION, i);
            IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, ShadowMap0 + i, shadow_map);
            IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightWorldMatrix0 + i, light_world_matrix);
            IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightProjectionMatrix0 + i, light_proj_matrix);
            IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, ShadowDirection0 + i, shadow_dir);
        }
    }
    else
    {
        IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, ShadowCubeMap, SHADOW_CUBE_MAP);
        IPxlSdrBuf.add_uniform_from_renderer((ShaderBuffer)ret, _rdr, LightInfluence, LIGHT_INFLUENCE);
    }
    EString_delete(prefix);
    return ret;
}

VtxSdrBuf create_lighting_vertex_shader_buffer(VertexDecl _dec);
Pass create_lighting_pass_ex3(Renderer* _rdr, VertexDecl _dec, LightState _lt_state, lighting_decl* _decls, bool _has_lighting_map)
{
    char mbuf[STRING_BUFFER_SIZE];

    VtxSdrBuf vsb = create_lighting_vertex_shader_buffer(_dec);
    PxlSdrBuf psb = create_lighting_pixel_shader_buffer2(_rdr, _dec, _lt_state->num_shadow_emitters, _lt_state->type, _has_lighting_map);

    ShaderNode vsn = create_lighting_vertex_shader_node(_dec);

    IVtxSdrBuf.add_prototype_node((ShaderBuffer)vsb, vsn);
    IVtxSdrBuf.add_reference_node((ShaderBuffer)vsb, ShaderNode_get_name(vsn));

    ShaderBuffer sb = to_ShaderBuffer(psb);

    SdrNdGen sng = ISdrNdGen.New();
    ISdrNdGen.register_default_nodes(sng);
    ISdrNdGen.register_shader_node(sng, DirectionLightingNode, create_direction_lighting_node);
    ISdrNdGen.register_shader_node(sng, SpotLightingNode, create_spot_lighting_node);
    ISdrNdGen.register_shader_node(sng, DiffuseSpecularOutputNode, create_diffuse_specular_out_node);
    ISdrNdGen.register_shader_node(sng, Shadow2DMapTestNode, create_shadow2d_test_node);
    ISdrNdGen.register_shader_node(sng, ShadowCubeMapTestNode, create_shadowcube_test_node);
    ISdrNdGen.register_shader_node(sng, WhiteScreenNode, create_white_screen_node);
    ISdrNdGen.register_shader_node(sng, PointLightingNode, create_point_lighting_node);
    ISdrNdGen.attach_all_prototype_nodes(sng, sb);

    ShaderObject mat_id_tex = IPxlSdrBuf.find_object((ShaderBuffer)psb, "MaterialIDMap");
    ShaderObject uv = IPxlSdrBuf.find_object((ShaderBuffer)psb, "vTexCoord");
    ShaderObject mat_id_pxl = IPxlSdrBuf.new_object((ShaderBuffer)psb, Float4_Obj, "MaterialIDPixel", 1);

    ShaderNode rgba_map_sample_node = ISdrNdGen.add_reference_node_1(sng, MapSampleNodeRGBA);
    ShaderNode_add_input_link(rgba_map_sample_node, mat_id_tex, INVALID_ARRAY_INDEX);
    ShaderNode_add_input_link(rgba_map_sample_node, uv, INVALID_ARRAY_INDEX);
    ShaderNode_add_output_link(rgba_map_sample_node, mat_id_pxl, INVALID_ARRAY_INDEX);

    component_index comp_idx;
    comp_idx.num_comps = 1;
    comp_idx.comps[0] = CompW;
    ShaderObject mat_id = ShaderObject_get_component(mat_id_pxl, comp_idx, INVALID_ARRAY_INDEX);
    ShaderObject int_mat_id = ShaderObject_float_to_int(mat_id, 255.0f);
#ifdef USE_SWITCH
    SwitchNode sn = SwitchNode_new();
    SwitchNode_set_switch_object(sn, int_mat_id);
#else
	BranchNode bn = BranchNode_new();
	ShaderObject null_sdr_obj = {NULL};
	ShaderObject* tag_obj_buf = array_new(ShaderObject, 5, null_sdr_obj);
#endif
    Tree inv_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);

    /// 提取出光照处理函数，将函数指针作为key，
    /// 从std::map<lighting_proc, std::set<material_id>>的tree里面找出std::set<material_id>，
    /// 如找不到则创建并插入一个空的std::set<material_id>
    /// 最后向找到的或新创建出来的std::set<material_id>插入material_id
    uint n = array_n(_decls);
    for (uint i = 0; i < n; i++)
    {
        LightingProc2 light_proc = _decls[i].light_proc;
        var key, data;
        Tree tag_set = NULL;
        key.vptr_var = (vptr)light_proc;
        if (Tree_find(inv_tree, key, &data))
            tag_set = (Tree)data.vptr_var;
        else
        {
            tag_set = Tree_new(Sint32, Sint32, Ealloc, Efree);
            key.vptr_var = (vptr)light_proc;
            data.vptr_var = tag_set;
            Tree_insert(inv_tree, key, data);
        }
        var set_value;
        set_value.sint32_var = _decls[i].mat_id;
        Tree_insert(tag_set, set_value, set_value);
    }

    /// 遍历inv_tree，收集每一个条目下的material_id
    /// 将条目添加到对应的material_id构成的case分支标签当中
    Iterator inv_iter = Tree_begin(inv_tree);
    while (inv_iter)
    {
        var key = Tree_get_key(inv_iter);
        var data = Tree_get_value(inv_iter);
		LightingProc2 light_proc = (LightingProc2)key.vptr_var;
        Tree tag_set = (Tree)data.vptr_var;
#ifdef USE_SWITCH
        sint* tags = array_new(sint, Tree_count(tag_set), -1);
        Iterator tag_iter = Tree_begin(tag_set);
        while (tag_iter)
        {
            var data = Tree_get_value(tag_iter);
            apush(tags, data.sint32_var);
            tag_iter = Tree_next(tag_iter);
        }

        CircuitBoard cb = SwitchNode_add_branch_block(sn, tags);
        light_proc(_lt_state, psb, sng, cb, _has_lighting_map);

		array_delete(tags);
#else
		Iterator tag_iter = Tree_begin(tag_set);
		while (tag_iter)
		{
			var data = Tree_get_value(tag_iter);
			///apush(tags, data.sint32_var);
			ShaderObject so = ShaderObject_new_from_sint(data.sint32_var);
			CircuitBoard cb = BranchNode_add_branch(bn, int_mat_id, Equal, so);
			light_proc(_lt_state, psb, sng, cb, _has_lighting_map);
			apush(tag_obj_buf, so);
			tag_iter = Tree_next(tag_iter);
		}
#endif
        inv_iter = Tree_next(inv_iter);
    }
#ifdef USE_SWITCH
    IPxlSdrBuf.add_branch_node((ShaderBuffer)psb, (BranchNode)sn);
#else
	IPxlSdrBuf.add_branch_node((ShaderBuffer)psb, bn);
#endif
    IVtxSdrBuf.complete(vsb);
    IPxlSdrBuf.complete(psb);
#ifdef USE_SWITCH
    SwitchNode_delete(sn);
#else
	BranchNode_delete(bn);
	uint num_objs = array_n(tag_obj_buf);
	for (uint i = 0; i < num_objs; i++)
	{
		ShaderObject_delete(tag_obj_buf[i]);
	}
#endif
    /// 清除用过的inv_tree
    inv_iter = Tree_begin(inv_tree);
    while (inv_iter)
    {
        var data = Tree_get_value(inv_iter);
        Tree tag_set = (Tree)data.vptr_var;
        Tree_Dest(tag_set);
        inv_iter = Tree_next(inv_iter);
    }
    Tree_Dest(inv_tree);
    ShaderObject_delete(mat_id);
    ShaderObject_delete(int_mat_id);

    ShaderNode_delete(vsn);
    ISdrNdGen.Delete(sng);

    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);

    ///snprintf(mbuf, STRING_BUFFER_SIZE - 1, "#version 400\n%s", sb.self->output);
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             GLSL_VERSION
             "%s", sb->output);
    Shader_load_from_string(auto_vs, mbuf, VertexShader);
    slog(LightingPassLog, "%s", mbuf);

    sb = to_ShaderBuffer(psb);

    ///snprintf(mbuf, STRING_BUFFER_SIZE - 1, "#version 400\n%s", sb.self->output);
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
             GLSL_VERSION
             "%s", sb->output);
    Shader_load_from_string(auto_ps, mbuf, PixelShader);
    slog(LightingPassLog, "%s", mbuf);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    Tree vtx_param_src_tree = IVtxSdrBuf.sell_param_source_object_tree((ShaderBuffer)vsb);
    Tree pxl_param_src_tree = IPxlSdrBuf.sell_param_source_object_tree((ShaderBuffer)psb);

    Pass_buy_vertex_param_source_tree(ret, vtx_param_src_tree);
    Pass_buy_pixel_param_source_tree(ret, pxl_param_src_tree);

    IVtxSdrBuf._Delete(vsb);
    IPxlSdrBuf._Delete(psb);
    return ret;
}
