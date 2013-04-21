#include "shadow_emitter.h"
#include "sketch.h"
#include "matrix4x4.h"
#include "camera.h"
#include "mem.h"
#include "estring.h"
#include "pass_console.h"

ShaderNode create_shadow2d_test_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "Shadow2DTest");
    ShaderNode_add_input_param(ret, Texture2D_Obj, "DepthMap", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "PixelWorldPosition", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "LightWorldMatrix", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "LightProjectionMatrix", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightPosition", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightDirection", 1);
    ShaderNode_set_return_type(ret, Bool_Obj, 1);

    const char* func =
    "{\n"
    "    vec3 pixel_dir = PixelWorldPosition - LightPosition;\n"
    "    pixel_dir = normalize(pixel_dir);\n"
    "    if ( dot(pixel_dir, LightDirection) > 0.0 )\n"
    "        return false;\n"
    "    vec4 pos = LightWorldMatrix * vec4(PixelWorldPosition, 1.0);\n"
    "    pos = LightProjectionMatrix * pos;\n"
    "    pos /= pos.w;\n"
    "    vec2 uv = pos.xy;\n"
    "    uv = uv * 0.5 + vec2(0.5, 0.5);\n"
    "    float depth = pos.z;\n"
    "    depth = clamp(depth, 0.0, 1.0);\n"
    "    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)\n"
    "        return false;\n"
    "    float depthMapPixel = texture2D(DepthMap, uv).r;\n"
    "    if (depth > depthMapPixel)\n"
    "        return true;\n"
    "    else\n"
    "        return false;\n"
    "}\n";

    ShaderNode_set_function(ret, func);
    return ret;
}

ShaderNode create_shadow_dispaly_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "ShadowProc");
    ShaderNode_add_input_param(ret, Bool_Obj, "ShadowTestResult", 1);
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
    "{\n"
    "    if (true == ShadowTestResult)\n"
    "        %s = vec4(0.0, 0.0, 0.0, 1.0);\n"
    "    else\n"
    "        %s = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n",
    get_render_target_string(RenderTarget0), get_render_target_string(RenderTarget0));
    ShaderNode_set_function(ret, mbuf);
    return ret;
}

PixelShaderBuffer create_shadow2d_pixel_shader_buffer(VertexDecl _dec)
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
    ShaderBuffer_add_uniform(sb, Texture2D_Param, "UnifDepthMap", 1, Plaster);
    ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "UnifLightWorldMatrix", 1, LightWorldMatrix);
    ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "UnifLightProjectionMatrix", 1, LightProjectionMatrix);
    ShaderBuffer_add_uniform(sb, Float32x3_Param, CAMERA_POSITION, 1, LightPosition);
    ShaderBuffer_add_uniform(sb, Float32x3_Param, CAMERA_DIRECTION, 1, LightDirection);
    EString_delete(prefix);
    return ret;
}

Pass create_shadow_pass(VertexDecl _dec)
{
    char mbuf[STRING_BUFFER_SIZE];

    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_dec, true, true);
    PixelShaderBuffer psb = create_shadow2d_pixel_shader_buffer(_dec);

    ShaderNode vsn = create_std_vertex_shader_node(_dec, vsb, true, true);
    ShaderNode shadow2d_test_node = create_shadow2d_test_node();
    ShaderNode shadow_dispaly_node = create_shadow_dispaly_node();

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, shadow2d_test_node);
    ShaderBuffer_add_prototype_node(sb, shadow_dispaly_node);

    ShaderObject shadow_test_result = ShaderBuffer_new_object(sb, Bool_Obj, "ShadowTestResult", 1);
    ShaderObject depth_map = ShaderBuffer_find_object(sb, "UnifDepthMap");
    ShaderObject light_world_matrix = ShaderBuffer_find_object(sb, "UnifLightWorldMatrix");
    ShaderObject light_proj_matrix = ShaderBuffer_find_object(sb, "UnifLightProjectionMatrix");
    ShaderObject light_cam_pos = ShaderBuffer_find_object(sb, CAMERA_POSITION);
    ShaderObject light_cam_dir = ShaderBuffer_find_object(sb, CAMERA_DIRECTION);
    ShaderObject pixel_world_pos = ShaderBuffer_find_object(sb, "vPosition");
    {
        ShaderNode ref_shadow2d_test_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(shadow2d_test_node));
        ShaderNode_add_input_link(ref_shadow2d_test_node, depth_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(ref_shadow2d_test_node, pixel_world_pos, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(ref_shadow2d_test_node, light_world_matrix, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(ref_shadow2d_test_node, light_proj_matrix, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(ref_shadow2d_test_node, light_cam_pos, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(ref_shadow2d_test_node, light_cam_dir, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(ref_shadow2d_test_node, shadow_test_result, INVALID_ARRAY_INDEX);
    }
    {
        ShaderNode ref_shadow_dispaly_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(shadow_dispaly_node));
        ShaderNode_add_input_link(ref_shadow_dispaly_node, shadow_test_result, INVALID_ARRAY_INDEX);
    }

    VertexShaderBuffer_release(vsb);
    PixelShaderBuffer_release(psb);

    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);
    sprintf(mbuf, "#version 120\n%s", sb.self->output);
    Shader_load_from_string(auto_vs, mbuf, VertexShader);
    ///
    elog(mbuf);
    ///
    sb = to_ShaderBuffer(psb);
    sprintf(mbuf, "#version 140\n#extension GL_ARB_gpu_shader5 : enable\n%s", sb.self->output);
    Shader_load_from_string(auto_ps, mbuf, PixelShader);
    ///
    elog(mbuf);
    ///
    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    ShaderNode_delete(shadow2d_test_node);
    ShaderNode_delete(shadow_dispaly_node);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}
