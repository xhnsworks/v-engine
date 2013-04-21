#include "display_pass.h"
#include "pass_console.h"
#include "lighting.h"
#include "array.h"
#include "shader_buffer.h"
#include "shader_node.h"
#include "std_pass.h"
#include "estring.h"
ShaderNode create_display_pixel_shader_node(VertexDeclaration _dec)
{
    if (!VertexDeclaration_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec3 cmap = texture2D(ColorMap, vTexcoord).rgb;\n"
                            "    vec3 lmap = texture2D(LightingMap, vTexcoord).rgb;\n"
                            "    gl_FragData[0] = vec4( clamp( cmap + AmbiColor, 0.0, 1.0 ) * lmap, 1.0 );"
                            "}\n");

    return ret;
}

ShaderNode create_display_normal_map_node(VertexDeclaration _dec)
{
    if (!VertexDeclaration_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec3 nmap = texture2D(NormalMap, vTexcoord).rgb;\n"
                            "    vec3 n;\n"
                            "    n.xy = nmap.xy * 2.0 - 1.0;\n"
                            "    n.z = sqrt(1.0 - dot(n.xy, n.xy));\n"
                            "    gl_FragData[0] = vec4( n, 1.0 );"
                            "}\n");

    return ret;
}

ShaderNode create_display_lighting_map_node(VertexDeclaration _dec)
{
    if (!VertexDeclaration_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec3 lmap = texture2D(LightingMap, vTexcoord).rgb;\n"
                            "    gl_FragData[0] = vec4( lmap, 1.0 );"
                            "}\n");

    return ret;
}

ShaderNode create_display_depth_map_node(VertexDeclaration _dec)
{
    if (!VertexDeclaration_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec4 nmap = texture2D(NormalMap, vTexcoord);\n"
                            "    float depth = nmap.w * 0.1;\n"
                            "    gl_FragData[0] = vec4( depth, depth, depth, 1.0 );"
                            "}\n");

    return ret;
}

ShaderNode create_display_position_map_node(VertexDeclaration _dec)
{
    if (!VertexDeclaration_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec3 pmap = texture2D(PositionMap, vTexcoord).rgb;\n"
                            "    gl_FragData[0] = vec4( pmap, 1.0 );"
                            "}\n");

    return ret;
}

ShaderNode create_clear_buffer_node(VertexDeclaration _dec)
{
    if (!VertexDeclaration_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    gl_FragData[0] = vec4( 0.0, 0.0, 0.0, 0.0 );"
                            "    gl_FragData[1] = vec4( 0.0, 0.0, 0.0, 0.0 );"
                            "    gl_FragData[2] = vec4( 0.0, 0.0, 0.0, 0.0 );"
                            "}\n");

    return ret;
}

PixelShaderBuffer create_display_pixel_shader_buffer(VertexDeclaration _dec)
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
    ShaderBuffer_add_uniform(sb, Texture2D_Param, "ColorMap", 1);
    ShaderBuffer_add_uniform(sb, Texture2D_Param, "PositionMap", 1);
    ShaderBuffer_add_uniform(sb, Texture2D_Param, "NormalMap", 1);
    ShaderBuffer_add_uniform(sb, Texture2D_Param, "LightingMap", 1);
    ShaderBuffer_add_uniform(sb, Float32x3_Param, "AmbiColor", 1);

    EString_delete(prefix);

    return ret;
}

PixelShaderBuffer create_clear_buffer_buffer(VertexDeclaration _dec)
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

    EString_delete(prefix);

    return ret;
}

Pass create_display_pass(VertexDeclaration _dec)
{
    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_dec, false);
    PixelShaderBuffer psb = create_display_pixel_shader_buffer(_dec);
    ShaderNode vsn = create_std_vertex_shader_node(_dec, false);
    ShaderNode psn = create_display_pixel_shader_node(_dec);

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    VertexShaderBuffer_release(vsb);
    PixelShaderBuffer_release(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);

    Shader_load_from_string(auto_vs, sb.self->output, VertexShader);

    sb = to_ShaderBuffer(psb);
    Shader_load_from_string(auto_ps, sb.self->output, PixelShader);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    ShaderNode_delete(psn);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}

Pass create_normal_display_pass(VertexDeclaration _dec)
{
    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_dec, false);
    PixelShaderBuffer psb = create_display_pixel_shader_buffer(_dec);
    ShaderNode vsn = create_std_vertex_shader_node(_dec, false);
    ShaderNode psn = create_display_normal_map_node(_dec);

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    VertexShaderBuffer_release(vsb);
    PixelShaderBuffer_release(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);
    Shader_load_from_string(auto_vs, sb.self->output, VertexShader);

    sb = to_ShaderBuffer(psb);
    Shader_load_from_string(auto_ps, sb.self->output, PixelShader);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    ShaderNode_delete(psn);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}

Pass create_depth_display_pass(VertexDeclaration _dec)
{
    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_dec, false);
    PixelShaderBuffer psb = create_display_pixel_shader_buffer(_dec);
    ShaderNode vsn = create_std_vertex_shader_node(_dec, false);
    ShaderNode psn = create_display_depth_map_node(_dec);

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    VertexShaderBuffer_release(vsb);
    PixelShaderBuffer_release(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);

    Shader_load_from_string(auto_vs, sb.self->output, VertexShader);

    sb = to_ShaderBuffer(psb);
    Shader_load_from_string(auto_ps, sb.self->output, PixelShader);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    ShaderNode_delete(psn);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}

Pass create_light_display_pass(VertexDeclaration _dec)
{
    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_dec, false);
    PixelShaderBuffer psb = create_display_pixel_shader_buffer(_dec);
    ShaderNode vsn = create_std_vertex_shader_node(_dec, false);
    ShaderNode psn = create_display_lighting_map_node(_dec);

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    VertexShaderBuffer_release(vsb);
    PixelShaderBuffer_release(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);

    Shader_load_from_string(auto_vs, sb.self->output, VertexShader);

    sb = to_ShaderBuffer(psb);
    Shader_load_from_string(auto_ps, sb.self->output, PixelShader);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    ShaderNode_delete(psn);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}

Pass create_position_display_pass(VertexDeclaration _dec)
{
    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_dec, false);
    PixelShaderBuffer psb = create_display_pixel_shader_buffer(_dec);
    ShaderNode vsn = create_std_vertex_shader_node(_dec, false);
    ShaderNode psn = create_display_position_map_node(_dec);

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    VertexShaderBuffer_release(vsb);
    PixelShaderBuffer_release(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);

    Shader_load_from_string(auto_vs, sb.self->output, VertexShader);

    sb = to_ShaderBuffer(psb);
    Shader_load_from_string(auto_ps, sb.self->output, PixelShader);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    ShaderNode_delete(psn);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}

Pass create_clear_buffer_pass(VertexDeclaration _dec)
{
    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_dec, false);
    PixelShaderBuffer psb = create_clear_buffer_buffer(_dec);
    ShaderNode vsn = create_std_vertex_shader_node(_dec, false);
    ShaderNode psn = create_clear_buffer_node(_dec);

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    VertexShaderBuffer_release(vsb);
    PixelShaderBuffer_release(psb);
    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);
    Shader_load_from_string(auto_vs, sb.self->output, VertexShader);
    sb = to_ShaderBuffer(psb);

    Shader_load_from_string(auto_ps, sb.self->output, PixelShader);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    ShaderNode_delete(vsn);
    ShaderNode_delete(psn);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}
