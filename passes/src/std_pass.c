#include "std_pass.h"
#include "pass_console.h"
#include "lighting.h"
#include "array.h"
#include "shader_buffer.h"
#include "shader_node.h"
#include "estring.h"
#include "gl_utility.h"
#include "eassert.h"
#include "shader_test.h"

ShaderNode create_std_vertex_shader_node(VertexDecl _dec, VertexShaderBuffer _vsb, bool _use_proj_mat, bool _use_world_mat)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }

    const char* prefix = EString_new("v");
    const char* fun_str = NULL;

    const char* world_matrix_proc = NULL;
    const char* proj_matrix_proc = NULL;
    if (_use_proj_mat)
    {
        _mat4_obj_test(to_ShaderBuffer(_vsb), "ProjMatrix");
        proj_matrix_proc = "    pos = ProjMatrix * pos;\n";
    }
    else
    {
        proj_matrix_proc = "";
    }

    if (_use_world_mat)
    {
        _mat4_obj_test(to_ShaderBuffer(_vsb), "CameraWorldMatrix");
        world_matrix_proc = "    pos = CameraWorldMatrix * pos;\n";
    }
    else
        world_matrix_proc = "";

#define BUFFER_SIZE 2048
    char mbuf[BUFFER_SIZE];

    int count = snprintf(mbuf, BUFFER_SIZE - 1, "%s%s%s%s", "{\n"
                         "    vec4 pos = vec4(Position, 1.0);\n",
                         world_matrix_proc,
                         proj_matrix_proc,
                         "    gl_Position = pos;\n"
                         "    vPosition = Position.xyz;\n");

    char* str = mbuf + count;
    int remainder = BUFFER_SIZE - 1 - count;

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
                count = snprintf(str, remainder, "    %s = %s;\n", vary_str, sem_str);
                str += count;
                remainder -= count;
            }
            EString_delete(vary_str);
        }
    }

    snprintf(str, remainder, " }\n");

    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "VertexProc");
    ShaderNode_set_function(ret, mbuf);

    EString_delete(prefix);
    return ret;
}

ShaderNode create_normal_depth_encode_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "NormalDepthEncode");
    ShaderNode_set_function(ret,
                            "{\n"
                            "    return vec4(Normal.xy * 0.5 + 0.5, Depth.x, Depth.y);\n"
                            "}\n");
    ShaderNode_add_input_param(ret, Float3_Obj, "Normal", 1);
    ShaderNode_add_input_param(ret, Float2_Obj, "Depth", 1);
    ShaderNode_set_return_type(ret, Float4_Obj, 1);
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

ShaderNode create_std_pixel_shader_node(std_pass_status* _status)
{
    if (!VertexDecl_test(_status->dec.dec) || !VertexDecl_test_element_semantic(_status->dec.dec, Position))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    char normal_depth_material_id_output_buf[256];
    char material_id_output_buf[256];

    const char* normal_proc = NULL;
    const char* tangent_proc = NULL;
    const char* color_proc = NULL;
    const char* normal_depth_material_id_output = NULL;
    const char* material_id_output = NULL;
    const char* color_output = NULL;
    const char* position_output = NULL;

    if (VertexDecl_test_element_semantic(_status->dec.dec, TexCoord) &&
            VertexDecl_test_element_semantic(_status->dec.dec, Normal) &&
            VertexDecl_test_element_semantic(_status->dec.dec, Tangent) &&
            VertexDecl_test_element_semantic(_status->dec.dec, Binormal))
    {
        normal_proc =
            "    mat3x3 m = mat3x3(vBinormal, vTangent, vNormal);\n"
            "    m = inverse(m);\n"
            "    vec3 nmap = texture2D(NormalMap, vTexCoord).rgb;\n"
            "    nmap -= vec3(0.5, 0.5, 0.5);\n"
            "    nmap = m * nmap;\n";
        tangent_proc =
            "    vec3 tmap = vTangent;\n";
    }
    else
    {
        normal_proc = "";
        tangent_proc = "";
    }

    if ( (_status->dec.map_flags & COLOR_MAP_MASK) && VertexDecl_test_element_semantic(_status->dec.dec, TexCoord) )
    {
        color_proc =
            "    vec3 cmap = texture2D(ColorMap, vTexCoord).rgb;\n";
    }
    else
    {
        color_proc =
            "    vec3 cmap = vec3(1.0, 1.0, 1.0);\n";
    }
    if (VertexDecl_test_element_semantic(_status->dec.dec, Normal))
    {
        if ( (_status->dec.map_flags & NORMAL_MAP_MASK) && VertexDecl_test_element_semantic(_status->dec.dec, TexCoord) )
        {
            sprintf(normal_depth_material_id_output_buf, "vec4(nmap.xy * 0.5 + 0.5, tmap.xy * 0.5 + 0.5);\n");
            normal_depth_material_id_output =
                normal_depth_material_id_output_buf;
        }
        else
        {
            sprintf(normal_depth_material_id_output_buf, "vec4(vNormal.xy * 0.5 + 0.5, vTangent.xy * 0.5 + 0.5);\n");
            normal_depth_material_id_output =
                normal_depth_material_id_output_buf;
        }
    }
    else
    {
        sprintf(normal_depth_material_id_output_buf, "vec4(0.0, 0.0, 0.0, 0.0);\n");
        normal_depth_material_id_output =
            normal_depth_material_id_output_buf;
    }

    sprintf(material_id_output_buf, "vec4(0.0, 0.0, 0.0, MaterialID / 255.0);\n");
    material_id_output = material_id_output_buf;

    color_output =
        "vec4(cmap, 1.0);\n";
    position_output =
        "vec4(vPosition, 1.0);\n";

    char func[4 * 1024];
    char procs[4 * 1024];
    char outputs[4 * 1024];

    const char* render_target_strings[4];
    const char* render_output_strings[4];
    for (uint i = 0; i < _status->num_render_pipes; i++)
    {
        render_target_strings[i] = get_render_target_string(_status->render_pipes[i].tgt);
        switch (_status->render_pipes[i].out)
        {
        case ColorOutput:
            render_output_strings[i] = color_output;
            break;
        case NormalTangentOutput:
            render_output_strings[i] = normal_depth_material_id_output;
            break;
        case MaterialIDOutput:
            render_output_strings[i] = material_id_output;
            break;
        case PositionOutput:
            render_output_strings[i] = position_output;
            break;
        }
    }

    procs[0] = 0x00;
    outputs[0] = 0x00;

    for (uint i = 0; i < _status->num_render_pipes; i++)
    {
        switch (_status->render_pipes[i].out)
        {
        case ColorOutput:
            sprintf(procs, "%s%s", procs, color_proc);
            break;
        case NormalTangentOutput:
            if (_status->dec.map_flags & NORMAL_MAP_MASK)
                sprintf(procs, "%s%s%s", procs, normal_proc, tangent_proc);
            break;
        default:
            break;
        }

        sprintf(outputs, "%s    %s = %s", outputs, render_target_strings[i], render_output_strings[i]);
    }

    sprintf(func, "{\n%s%s}\n", procs, outputs);

    ShaderNode_set_function(ret, func);
    ShaderNode_add_input_param(ret, Float3_Obj, "CameraPos", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "CameraDir", 1);
    ERROR_PROC;

    return ret;
}

ShaderNode create_color_output_pixel_shader_node(EFloat3* _color)
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "PixelProc");

    char func[4 * 1024];

    sprintf(func, "{\n    gl_FragData[0] = vec4(%f, %f, %f, 1.0);\n}\n", _color->x, _color->y, _color->z);

    ShaderNode_set_function(ret, func);

    return ret;
}

VertexShaderBuffer create_std_vertex_shader_buffer(VertexDecl _dec, bool _use_proj_mat, bool _use_camera_world_mat)
{
    if (!VertexDecl_test(_dec))
    {
        VertexShaderBuffer ret = {NULL};
        return ret;
    }

    VertexShaderBuffer ret = VertexShaderBuffer_new();
    ShaderBuffer sb = to_ShaderBuffer(ret);
    if (_use_proj_mat)
        ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "ProjMatrix", 1, CameraProjectionMatrix);
    if (_use_camera_world_mat)
        ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "CameraWorldMatrix", 1, CameraWorldMatrix);

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

    ///ShaderBuffer_add_varying(sb, Float32x2_Param, "vDepth");
    ShaderBuffer_add_uniform(sb, Float32x3_Param, CAMERA_POSITION, 1, CameraPosition);

    EString_delete(prefix);
    return ret;
}

PixelShaderBuffer create_std_pixel_shader_buffer(std_pass_status* _status)
{
    if (!VertexDecl_test(_status->dec.dec))
    {
        PixelShaderBuffer ret = {NULL};
        return ret;
    }

    PixelShaderBuffer ret = PixelShaderBuffer_new();
    ShaderBuffer sb = to_ShaderBuffer(ret);

    const char* prefix = EString_new("v");

    for (uint32 i = 0; i < VertexDecl_count(_status->dec.dec); i++)
    {
        VertexElement ele = VertexDecl_find(_status->dec.dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);
        const char* vary_str = EString_add(prefix, sem_str);
        sint32 src = get_param_source(sem);
        ShaderBuffer_add_varying(sb, pam_type, vary_str, src);
        EString_delete(vary_str);
    }
    ///ShaderBuffer_add_varying(sb, Float32x2_Param, "vDepth");

    if (_status->dec.map_flags | NORMAL_MAP_MASK)
        ShaderBuffer_add_uniform(sb, Texture2D_Param, "NormalMap", 1, NormalMap0);
    if (_status->dec.map_flags | COLOR_MAP_MASK)
        ShaderBuffer_add_uniform(sb, Texture2D_Param, "ColorMap", 1, ColorMap0);
    ShaderBuffer_add_uniform(sb, Float32x3_Param, CAMERA_POSITION, 1, CameraPosition);
    ShaderBuffer_add_uniform(sb, Float32x3_Param, CAMERA_DIRECTION, 1, CameraDirection);
    ShaderBuffer_add_uniform(sb, Float32_Param, "MaterialID", 1, CurrentMaterialID);
    ///ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "InvProjMatrix", 1);

    EString_delete(prefix);
    return ret;
}

Pass create_std_pass_from_dec(std_pass_status* _status)
{
    char mbuf[STRING_BUFFER_SIZE];
    ERROR_PROC;
    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_status->dec.dec, true, true);
    ERROR_PROC;
    PixelShaderBuffer psb = create_std_pixel_shader_buffer(_status);
    ERROR_PROC;

    ShaderNode vsn = create_std_vertex_shader_node(_status->dec.dec, vsb, true, true);
    ERROR_PROC;
    ShaderNode psn = create_std_pixel_shader_node(_status);
    ERROR_PROC;

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderNode std_ps_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));
    ERROR_PROC;

    ShaderObject cam_pos = ShaderBuffer_find_object(sb, CAMERA_POSITION);
    ShaderObject cam_dir = ShaderBuffer_find_object(sb, CAMERA_DIRECTION);
    ShaderNode_add_input_link(std_ps_node, cam_pos, INVALID_ARRAY_INDEX);
    ShaderNode_add_input_link(std_ps_node, cam_dir, INVALID_ARRAY_INDEX);
    ERROR_PROC;

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

    Tree vtx_param_src_tree = ShaderBuffer_sell_param_source_object_tree(to_ShaderBuffer(vsb));
    Tree pxl_param_src_tree = ShaderBuffer_sell_param_source_object_tree(to_ShaderBuffer(psb));

    Pass_buy_vertex_param_source_tree(ret, vtx_param_src_tree);
    Pass_buy_pixel_param_source_tree(ret, pxl_param_src_tree);

    ShaderNode_delete(vsn);
    ShaderNode_delete(psn);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}

Pass create_color_output_pass(std_pass_status* _status, EFloat3* _color)
{
    char mbuf[STRING_BUFFER_SIZE];

    VertexShaderBuffer vsb = create_std_vertex_shader_buffer(_status->dec.dec, false, false);
    PixelShaderBuffer psb = create_std_pixel_shader_buffer(_status);

    ShaderNode vsn = create_std_vertex_shader_node(_status->dec.dec, vsb, false, false);
    ShaderNode psn = create_color_output_pixel_shader_node(_color);

    ShaderBuffer sb;
    sb = to_ShaderBuffer(vsb);
    ShaderBuffer_add_prototype_node(sb, vsn);
    ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(vsn));
    sb = to_ShaderBuffer(psb);
    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderNode std_ps_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(psn));

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
    ShaderNode_delete(psn);
    VertexShaderBuffer_delete(vsb);
    PixelShaderBuffer_delete(psb);
    return ret;
}
