#include "std_pass.h"
#include "pass_console.h"
#include "lighting.h"
#include "array.h"
#include "shader_buffer.h"
#include "shader_node.h"
#include "estring.h"
ShaderNode create_std_vertex_shader_node(VertexDeclaration _dec, bool _use_proj_mat)
{
    if (!VertexDeclaration_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }

    const char* prefix = EString_new("v");
    const char* fun_str = NULL;
    if (_use_proj_mat)
    {

        fun_str = EString_new("{\n"
                              "    vec4 pos = vec4(Position, 1);\n"
                              "    pos = ProjMatrix * pos;\n"

                              "    gl_Position = pos;\n"
                              "    vW = pos.w;\n"
                              "    vDepth = pos.z / pos.w;\n"
                              "    vPosition = Position.xyz;\n"
                              );

    }
    else
    {

        fun_str = EString_new("{\n"
                              "    vec4 pos = vec4(Position, 1);\n"
                              "    gl_Position = pos;\n"
                              "    vW = 1;\n"
                              "    vDepth = pos.z;\n"
                              "    vPosition = Position.xyz;\n");

    }

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

ShaderNode create_normal_depth_encode_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "NormalDepthEncode");
    ShaderNode_set_function(ret,
                            "{\n"
                            "    return vec3(Normal.xy * 0.5 + 0.5, Depth);\n"
                            "}\n");
    ShaderNode_add_input_param(ret, Float3_Obj, "Normal", 1);
    ShaderNode_add_input_param(ret, Float_Obj, "Depth", 1);
    ShaderNode_set_return_type(ret, Float3_Obj, 1);
    return ret;
}

const char* get_render_target_string(render_target _tgt)
{
    switch (_tgt)
    {
    case RenderTarget0:
        return "gl_FragData[0]";
    case RenderTarget1:
        return "gl_FragData[1]";
    case RenderTarget2:
        return "gl_FragData[2]";
    case RenderTarget3:
        return "gl_FragData[3]";
    }
    return NULL;
}

ShaderNode create_std_pixel_shader_node(VertexDeclaration _dec, std_pass_status* _status)
{
    if (!VertexDeclaration_test(_dec) || !VertexDeclaration_test_element_semantic(_dec, Normal))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    const char* normal_proc = NULL;
    const char* depth_proc = NULL;
    const char* color_proc = NULL;
    const char* normal_depth_output = NULL;
    const char* color_output = NULL;
    const char* position_output = NULL;

    normal_proc =
    "    mat3x3 m = mat3x3(vBinormal, vTangent, vNormal);\n"
    "    m = inverse(m);\n"
    "    vec3 nmap = texture2D(NormalMap,  vTexcoord).rgb;\n"
    "    nmap -= vec3(0.5, 0.5, 0.5);\n"
    "    nmap = mul(m, nmap);\n";

    depth_proc =
    ///"    float d = distance(CameraPos, vPosition);\n";
    "    float d = vDepth;\n";

    if (_status->has_color_map)
    {
        color_proc =
        "    vec3 cmap = texture2D(ColorMap,   vTexcoord).rgb;\n";
    }
    else
    {
        color_proc =
        "    vec3 cmap = vec3(1.0, 1.0, 1.0);\n";
    }

    if (_status->has_normal_map)
    {
        normal_depth_output =
        "vec4(nmap.xy * 0.5 + 0.5, vW, vDepth);\n";
    }
    else
    {
        normal_depth_output =
        "vec4(vNormal.xy * 0.5 + 0.5, vW, vDepth);\n";
    }
    color_output =
    "vec4(cmap,                   1.0);\n";
    position_output =
    "vec4(vPosition.xyz,          1.0);\n";

    char func[4 * 1024];
    char procs[4 * 1024];
    char outputs[4 * 1024];

    const char* render_target_strings[4]; const char* render_output_strings[4];
    for (uint i = 0; i < _status->num_render_pipes; i++)
    {
        render_target_strings[i] = get_render_target_string(_status->render_pipes[i].tgt);
        switch (_status->render_pipes[i].out)
        {
        case ColorOutput:
            render_output_strings[i] = color_output;
            break;
        case NormalDepthOutput:
            render_output_strings[i] = normal_depth_output;
            break;
        case PositionOutput:
            render_output_strings[i] = position_output;
            break;
        }
    }

    ///sprintf(procs, "");
    ///sprintf(outputs, "");
    procs[0] = 0x00;
    outputs[0] = 0x00;

    for (uint i = 0; i < _status->num_render_pipes; i++)
    {
        switch (_status->render_pipes[i].out)
        {
        case ColorOutput:
            sprintf(procs, "%s%s", procs, color_proc);
            break;
        case NormalDepthOutput:
            if (_status->has_normal_map)
                sprintf(procs, "%s%s%s", procs, normal_proc, depth_proc);
            else
                sprintf(procs, "%s%s", procs, depth_proc);
            break;
        default:
            break;
        }

        sprintf(outputs, "%s    %s = %s", outputs, render_target_strings[i], render_output_strings[i]);
    }

    sprintf(func, "{\n%s%s}\n", procs, outputs);

    ShaderNode_set_function(ret, func);
    ShaderNode_add_input_param(ret, Float3_Obj, "CameraPos", 1);

    return ret;
}

VertexShaderBuffer create_std_vertex_shader_buffer(VertexDeclaration _dec, bool _use_proj_mat)
{
    if (!VertexDeclaration_test(_dec))
    {
        VertexShaderBuffer ret = {NULL};
        return ret;
    }

    VertexShaderBuffer ret = VertexShaderBuffer_new();
    ShaderBuffer sb = to_ShaderBuffer(ret);
    if (_use_proj_mat)
        ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "ProjMatrix", 1);

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
    ShaderBuffer_add_varying(sb, Float32_Param, "vW");
    ShaderBuffer_add_varying(sb, Float32_Param, "vDepth");

    EString_delete(prefix);
    return ret;
}

PixelShaderBuffer create_std_pixel_shader_buffer(VertexDeclaration _dec, std_pass_status* _status)
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
    ShaderBuffer_add_varying(sb, Float32_Param, "vW");
    ShaderBuffer_add_varying(sb, Float32_Param, "vDepth");

    if (_status->has_normal_map)
        ShaderBuffer_add_uniform(sb, Texture2D_Param, "NormalMap", 1);
    if (_status->has_color_map)
        ShaderBuffer_add_uniform(sb, Texture2D_Param, "ColorMap", 1);
    ShaderBuffer_add_uniform(sb, Float32x3_Param, CAMERA_POSITION, 1);
    ///ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "InvProjMatrix", 1);

    EString_delete(prefix);
    return ret;
}

Pass create_std_pass_from_dec(VertexDeclaration _dec, std_pass_status* _status)
{
    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_dec, true);
    PixelShaderBuffer psb = create_std_pixel_shader_buffer(_dec, _status);

    ShaderNode vsn = create_std_vertex_shader_node(_dec, true);
    ShaderNode psn = create_std_pixel_shader_node(_dec, _status);

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderNode std_ps_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

    ShaderObject cam_pos = ShaderBuffer_find_object(sb, CAMERA_POSITION);
    ShaderNode_add_input_link(std_ps_node, cam_pos, INVALID_ARRAY_INDEX);

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
