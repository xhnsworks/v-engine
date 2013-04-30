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
#include "shader_log.h"
#include "shader_object_table.h"

ShaderNode create_std_vertex_shader_node(VertexDecl _dec, VtxSdrBuf _vsb, 
										 bool _use_proj_mat, bool _use_world_mat)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
	
    ///bool has_normal = VertexDecl_test_element_semantic(_dec, Normal);
    ///bool has_tangent = VertexDecl_test_element_semantic(_dec, Tangent);
    ///bool has_binormal = VertexDecl_test_element_semantic(_dec, Binormal);
    /// 这里需要仔细推敲
    const char* cam_world_rot_mat_proc = NULL;
    if (_use_world_mat)
        cam_world_rot_mat_proc = "    mat3 CameraWorldRotateMatrix = mat3(CameraWorldMatrix);\n";
    else
        cam_world_rot_mat_proc = "";

    const char* prefix = EString_new("v");
    ///const char* fun_str = NULL;

    _mat4_obj_test(to_ShaderBuffer(_vsb), "RenderableWorldMatrix");
    const char* rend_world_matrix_proc = NULL;
    if (_use_world_mat)
        rend_world_matrix_proc =
        "    mat3 RenderableWorldRotateMatrix = mat3(RenderableWorldMatrix);\n"
        "    pos = RenderableWorldMatrix * pos;\n"
        "    vWorldPosition = pos.xyz;\n";
    else
        rend_world_matrix_proc =
        "    vWorldPosition = Position.xyz;\n";

    const char* cam_world_matrix_proc = NULL;
    const char* cam_proj_matrix_proc = NULL;
    if (_use_proj_mat)
    {
        _mat4_obj_test(to_ShaderBuffer(_vsb), "ProjMatrix");
        cam_proj_matrix_proc = "    pos = ProjMatrix * pos;\n";
    }
    else
    {
        cam_proj_matrix_proc = "";
    }

    if (_use_world_mat)
    {
        _mat4_obj_test(to_ShaderBuffer(_vsb), "CameraWorldMatrix");
        cam_world_matrix_proc = "    pos = CameraWorldMatrix * pos;\n";
    }
    else
        cam_world_matrix_proc = "";

#define BUFFER_SIZE 2048
    char mbuf[BUFFER_SIZE];

    int count = snprintf(mbuf, BUFFER_SIZE - 1, "%s%s%s%s%s%s", "{\n"
                         "    vec4 pos = vec4(Position.xyz, 1.0);\n",

                         rend_world_matrix_proc,
                         cam_world_matrix_proc,
                         cam_proj_matrix_proc,

                         cam_world_rot_mat_proc,

                         "    gl_Position = pos;\n"
                         "    vPosition = pos;\n");

    char* str = mbuf + count;
    int remainder = BUFFER_SIZE - 1 - count;

    for (uint32 i = 0; i < VertexDecl_count(_dec); i++)
    {
        VertexElement ele = VertexDecl_find(_dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        ///element_type sem_type = get_element_semantic_type(sem);
        const char* sem_str = get_element_semantic_string(sem);

        if (sem != EmptyType &&
            sem != Position &&
            sem != Normal &&
            sem != Tangent &&
            sem != Binormal)
        {
            const char* vary_str = EString_add(prefix, sem_str);

            count = snprintf(str, remainder, "    %s = %s;\n", vary_str, sem_str);
            str += count;
            remainder -= count;

            EString_delete(vary_str);
        }
        else if (sem == Normal ||
                 sem == Tangent ||
                 sem == Binormal
                 )
        {
            const char* vary_str = EString_add(prefix, sem_str);

            if (_use_world_mat) {
                count = snprintf(str, remainder, "    %s = RenderableWorldRotateMatrix * %s;\n"
                                                 ///"    %s = CameraWorldRotateMatrix * %s;\n"
                                                 "    %s = normalize(%s);\n",
                                                 vary_str, sem_str,
                                                 vary_str, vary_str,
                                                 vary_str, vary_str);
            }
            else {
                count = snprintf(str, remainder, "    %s = %s;\n", vary_str, sem_str);
            }
            str += count;
            remainder -= count;

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
	/// if vertex color is not exist, discard vertex color function...
	if (!VertexDecl_test_element_semantic(_status->dec.dec, Color))
		_status->dec.vtx_color_func = Discard;
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "StdPassPixelProc");

    char normal_specular_output_buf[256];
    char material_id_output_buf[256];

    const char* normal_proc = NULL;
    const char* tangent_proc = NULL;
    const char* color_proc = NULL;
	const char* color_proc2 = NULL;
    const char* vsm_depth_proc =
        "    float depth = vPosition.z / vPosition.w;\n"
		"    depth = depth * 0.5 + 0.5;\n"
		"    float moment1 = depth;\n"
		"    float moment2 = depth * depth;\n"
		"    float dx = dFdx(depth);\n"
		"    float dy = dFdy(depth);\n"
		"    moment2 += 0.25*(dx*dx+dy*dy);\n";

    char point_vsm_depth_proc_buffer[256];
    snprintf(point_vsm_depth_proc_buffer, 255,
             "    vec3 pxlDir = vWorldPosition - %s;\n"
             "    float depth = length(pxlDir);\n"
             "    depth /= %s;\n"
             "    depth = clamp(depth, 0.0, 1.0);\n"
             "    float moment1 = depth;\n"
		     "    float moment2 = depth * depth;\n"
             , LIGHT_POSITION
             , LIGHT_INFLUENCE
             );

    const char* normal_specular_output = NULL;
    const char* material_id_output = NULL;
    const char* color_output = NULL;

    const char* vsm_depth_output = "vec4(moment1, moment2, 0.0, 1.0);\n";


    bool has_normal = VertexDecl_test_element_semantic(_status->dec.dec, Normal);
    bool has_tangent = VertexDecl_test_element_semantic(_status->dec.dec, Tangent);
    bool has_binormal = VertexDecl_test_element_semantic(_status->dec.dec, Binormal);
    bool has_texcoord = VertexDecl_test_element_semantic(_status->dec.dec, TexCoord);

/**
enc = normalize(n.xy) * (sqrt(-n.z*0.5+0.5));
enc = enc*0.5+0.5;
**/

    if (    (_status->dec.usage_tex_flags & NORMAL_MAP_MASK) &&
            has_texcoord &&
            has_normal &&
            has_tangent &&
            has_binormal
            )
    {
        normal_proc =
            "    mat3x3 m = mat3x3(vBinormal, vTangent, vNormal);\n"
            ///"    m = inverse(m);\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
            "    vec3 nmap = texture(NormalMap, vTexCoord).rgb;\n"
#else
			"    vec3 nmap = texture2D(NormalMap, vTexCoord).rgb;\n"
#endif
            ///"    nmap -= vec3(0.5, 0.5, 0.5);\n"
            "    nmap = (nmap - 0.5) * 2.0;\n"
            ///"    nmap *= 2.0;\n"
            "    nmap = m * nmap;\n"
            "    nmap = normalize(nmap);\n"
            "    vec2 enc = normalize(nmap.xy) * (sqrt(-nmap.z * 0.5 + 0.5));\n"
            "    enc = enc * 0.5 + 0.5;\n";
        tangent_proc =
            "    vec3 tmap = vTangent;\n";
    }
    else
    {
        if (has_normal)
            normal_proc = ///"    vec3 nmap = vNormal;\n"
                          "    mat3x3 m = mat3x3(vBinormal, vTangent, vNormal);\n"
                          "    vec3 nmap = vec3(0.49, 0.49, 0.99);\n"
                          "    nmap = (nmap - 0.5) * 2.0;\n"
                          "    nmap = m * nmap;\n"
                          "    nmap = normalize(nmap);\n"
                          "    vec2 enc = normalize(nmap.xy) * (sqrt(-nmap.z * 0.5 + 0.5));\n"
                          "    enc = enc * 0.5 + 0.5;\n";
        else
            normal_proc = "    vec3 nmap = vec3(0.0, 0.0, 0.0);\n"
                          "    vec2 enc = normalize(nmap.xy) * (sqrt(-nmap.z * 0.5 + 0.5));\n"
                          "    enc = enc * 0.5 + 0.5;\n";
        if (has_tangent)
            tangent_proc = "    vec3 tmap = vTangent;\n";
        else
            tangent_proc = "    vec3 tmap = vec3(0.0, 0.0, 0.0);\n";
    }

    if ( (_status->dec.usage_tex_flags & COLOR_MAP_MASK) || (_status->dec.usage_tex_flags & COLOR_SKETCH_MASK) && has_texcoord )
    {
        if (!_status->use_gausscian_blur) {
            color_proc =
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
            "    vec4 cmap = texture(ColorMap, vTexCoord);\n";
#else
			"    vec4 cmap = texture2D(ColorMap, vTexCoord);\n";
#endif
        }
        else {
            color_proc =
/**
            "    vec3 pixels[9];\n"
            "    vec2 offs = PixelSize;\n"
            "    vec2 uv = vTexCoord;\n"
            "    vec3 cmap;\n"

            "    if ( uv.x > PixelSize.x && uv.x < 1.0 - PixelSize.x && uv.y > PixelSize.y && uv.y < 1.0 - PixelSize.y )\n"
            "    {\n"
            "        pixels[0] = texture( ColorMap, vec2(uv.x - offs.x, uv.y - offs.y) ).rgb * 0.062467;\n"
            "        pixels[1] = texture( ColorMap, vec2(uv.x,          uv.y - offs.y) ).rgb * 0.125000;\n"
            "        pixels[2] = texture( ColorMap, vec2(uv.x + offs.x, uv.y - offs.y) ).rgb * 0.062467;\n"

            "        pixels[3] = texture( ColorMap, vec2(uv.x - offs.x, uv.y) ).rgb          * 0.125000;\n"
            "        pixels[4] = texture( ColorMap, vec2(uv.x,          uv.y) ).rgb          * 0.250131;\n"
            "        pixels[5] = texture( ColorMap, vec2(uv.x + offs.x, uv.y) ).rgb          * 0.125000;\n"

            "        pixels[6] = texture( ColorMap, vec2(uv.x - offs.x, uv.y + offs.y) ).rgb * 0.062467;\n"
            "        pixels[7] = texture( ColorMap, vec2(uv.x,          uv.y + offs.y) ).rgb * 0.125000;\n"
            "        pixels[8] = texture( ColorMap, vec2(uv.x + offs.x, uv.y + offs.y) ).rgb * 0.062467;\n"
            "        cmap = pixels[0] + pixels[1] + pixels[2] + pixels[3] + pixels[4] + pixels[5] + pixels[6] + pixels[7] + pixels[8];\n"
            "    }\n"
            "    else\n"
            "    {\n"
            "        cmap = texture( ColorMap, uv ).rgb;\n"
            "    }\n";
**/
            "    vec4 pixels[4];\n"
            "    vec2 offs = PixelSize;\n"
            "    vec2 uv = vTexCoord;\n"
            "    vec4 cmap;\n"

            "    if ( uv.x > PixelSize.x && uv.x < 1.0 - PixelSize.x && uv.y > PixelSize.y && uv.y < 1.0 - PixelSize.y )\n"
            "    {\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
            "        pixels[0] = texture( ColorMap, vec2(uv.x,          uv.y) );\n"
            "        pixels[1] = texture( ColorMap, vec2(uv.x + offs.x, uv.y) );\n"
            "        pixels[2] = texture( ColorMap, vec2(uv.x,          uv.y + offs.y) );\n"
            "        pixels[3] = texture( ColorMap, vec2(uv.x + offs.x, uv.y + offs.y) );\n"
#else
			"        pixels[0] = texture2D( ColorMap, vec2(uv.x,          uv.y) );\n"
			"        pixels[1] = texture2D( ColorMap, vec2(uv.x + offs.x, uv.y) );\n"
			"        pixels[2] = texture2D( ColorMap, vec2(uv.x,          uv.y + offs.y) );\n"
			"        pixels[3] = texture2D( ColorMap, vec2(uv.x + offs.x, uv.y + offs.y) );\n"
#endif
            "        cmap = pixels[0] + pixels[1] + pixels[2] + pixels[3];\n"
            "        cmap /= 4.0;\n"
            "    }\n"
            "    else\n"
            "    {\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
            "        cmap = texture( ColorMap, uv );\n"
#else
			"        cmap = texture2D( ColorMap, uv );\n"
#endif
            "    }\n";
        }
    }
	else
	{
		color_proc = 
		"    vec4 cmap = vec4(1.0, 1.0, 1.0, 1.0);\n";
	}
	
	if (_status->dec.vtx_color_func == Multiply)
	{
		color_proc2 = "    cmap = cmap * vec4(vColor.rgb, 1.0);\n";
	}
	else if (_status->dec.vtx_color_func == Replace)
	{
		color_proc2 = "    cmap = vec4(vColor.rgb, 1.0);\n";
	}
	else
	{
		color_proc2 = "";
	}

    snprintf(normal_specular_output_buf, 255,
            ///"vec4(nmap.xy * 0.5 + 0.5, 0.0, 0.0);\n");
            "vec4(enc, 0.0, 0.0);\n");
    normal_specular_output =
            normal_specular_output_buf;

    snprintf(material_id_output_buf, 255, "vec4(0.0, 0.0, 0.0, MaterialID / 255.0);\n");
    material_id_output = material_id_output_buf;

    color_output =
        "cmap;\n";
    ///position_output =
    ///    "vec4(vPosition, 1.0);\n";

    char func[4 * 1024];
    char procs[4 * 1024];
    char outputs[4 * 1024];

    const char* render_target_strings[4];
    const char* render_output_strings[4];
    for (euint i = 0; i < _status->num_render_pipes; i++)
    {
        render_target_strings[i] = get_render_target_string(_status->render_pipes[i].tgt);
        switch (_status->render_pipes[i].out)
        {
        case ColorOutput:
            render_output_strings[i] = color_output;
            break;
        case NormalSpecularOutput:
            render_output_strings[i] = normal_specular_output;
            break;
        case MaterialIDOutput:
            render_output_strings[i] = material_id_output;
            break;
        case VSMDepthOutput:
            render_output_strings[i] = vsm_depth_output;
            break;
        case PointVSMDepthOutput:
            render_output_strings[i] = vsm_depth_output;
            break;
        ///case PositionOutput:
            ///render_output_strings[i] = position_output;
            ///break;
        }
    }

    procs[0] = 0x00;
    outputs[0] = 0x00;

    for (euint i = 0; i < _status->num_render_pipes; i++)
    {
        switch (_status->render_pipes[i].out)
        {
        case ColorOutput:
            snprintf(procs, 4 * 1024 - 1, "%s%s%s", procs, color_proc, color_proc2);
            break;
        case NormalSpecularOutput:
            snprintf(procs, 4 * 1024 - 1, "%s%s%s", procs, normal_proc, tangent_proc);
            break;
        case VSMDepthOutput:
            snprintf(procs, 4 * 1024 - 1, "%s%s", procs, vsm_depth_proc);
            break;
        case PointVSMDepthOutput:
            snprintf(procs, 4 * 1024 - 1, "%s%s", procs, point_vsm_depth_proc_buffer);
            break;
        default:
            break;
        }

        snprintf(outputs, 4 * 1024 - 1, "%s    %s = %s", outputs, render_target_strings[i], render_output_strings[i]);
    }

    snprintf(func, 4 * 1024 - 1, "{\n%s%s}\n", procs, outputs);

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

    snprintf(func, 4 * 1024 - 1, "{\n    gl_FragData[0] = vec4(%f, %f, %f, 1.0);\n}\n", _color->x, _color->y, _color->z);

    ShaderNode_set_function(ret, func);

    return ret;
}

VtxSdrBuf create_std_vertex_shader_buffer(VertexDecl _dec, bool _use_proj_mat, bool _use_camera_world_mat)
{
    if (!VertexDecl_test(_dec))
    {
        VtxSdrBuf ret = {NULL};
        return ret;
    }

    VtxSdrBuf ret = IVtxSdrBuf._New();
    ///ShaderBuffer sb = to_ShaderBuffer(ret);
    if (_use_proj_mat)
        ///ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "ProjMatrix", 1, CameraProjectionMatrix);
        IVtxSdrBuf.add_uniform((ShaderBuffer)ret, Matrix4x4_Param, "ProjMatrix", 1, CameraProjectionMatrix);
    if (_use_camera_world_mat)
        ///ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "CameraWorldMatrix", 1, CameraWorldMatrix);
        IVtxSdrBuf.add_uniform((ShaderBuffer)ret, Matrix4x4_Param, "CameraWorldMatrix", 1, CameraWorldMatrix);

    ///ShaderBuffer_add_uniform(sb, Matrix4x4_Param, "RenderableWorldMatrix", 1, RenderableWorldMatrix);
    IVtxSdrBuf.add_uniform((ShaderBuffer)ret, Matrix4x4_Param, "RenderableWorldMatrix", 1, RenderableWorldMatrix);

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
            IVtxSdrBuf.add_attribute(ret, sem, sem_type);
            const char* vary_str = EString_add(prefix, sem_str);
            sint32 src = get_param_source(sem);
            ///ShaderBuffer_add_varying(sb, pam_type, vary_str, src);
            IVtxSdrBuf.add_varying((ShaderBuffer)ret, pam_type, vary_str, src);
            EString_delete(vary_str);
            /// 若语义为Position，则添加vWorldPosition的verying
            if (sem == Position)
            {
                const char* vary_str = EString_new("vWorldPosition");
                IVtxSdrBuf.add_varying((ShaderBuffer)ret, Float32x3_Param, vary_str, VaryWorldPosition);
                EString_delete(vary_str);
            }
        }
    }

    ///ShaderBuffer_add_uniform(sb, Float32x3_Param, CAMERA_POSITION, 1, CameraPosition);
    IVtxSdrBuf.add_uniform((ShaderBuffer)ret, Float32x3_Param, CAMERA_POSITION, 1, CameraPosition);

    EString_delete(prefix);
    return ret;
}

PxlSdrBuf create_std_pixel_shader_buffer(std_pass_status* _status)
{
    if (!VertexDecl_test(_status->dec.dec))
    {
        PxlSdrBuf ret = {NULL};
        return ret;
    }

    PxlSdrBuf ret = IPxlSdrBuf._New();

    const char* prefix = EString_new("v");

    for (uint32 i = 0; i < VertexDecl_count(_status->dec.dec); i++)
    {
        VertexElement ele = VertexDecl_find(_status->dec.dec, i);
        element_semantic sem = VertexElement_get_semantic(ele);
        const char* sem_str = get_element_semantic_string(sem);
        param_type pam_type = get_element_param_type(sem);
        const char* vary_str = EString_add(prefix, sem_str);
        sint32 src = get_param_source(sem);
        IPxlSdrBuf.add_varying((ShaderBuffer)ret, pam_type, vary_str, src);
        EString_delete(vary_str);
        if (sem == Position)
        {
            const char* vary_str = EString_new("vWorldPosition");
            IPxlSdrBuf.add_varying((ShaderBuffer)ret, Float32x3_Param, vary_str, VaryWorldPosition);
            EString_delete(vary_str);
        }
    }

    if (_status->dec.usage_tex_flags & NORMAL_MAP_MASK)
        IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, NORMAL_MAP, 1, NormalMap0);
    if (_status->dec.usage_tex_flags & COLOR_MAP_MASK)
        IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, COLOR_MAP, 1, ColorMap0);
    if (_status->dec.usage_tex_flags & COLOR_SKETCH_MASK)
        IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Texture2D_Param, COLOR_MAP, 1, ColorSketch);
    if (_status->use_gausscian_blur)
        IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32x2_Param, PIXEL_SIZE, 1, ColorSketchPixelSize);
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32x3_Param, CAMERA_POSITION, 1, CameraPosition);
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32x3_Param, CAMERA_DIRECTION, 1, CameraDirection);
    IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32_Param, MATERIAL_ID, 1, CurrentMaterialID);

    for (euint i = 0; i < _status->num_render_pipes; i++)
    {
        if (_status->render_pipes[i].out == PointVSMDepthOutput)
        {
            IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32x3_Param, LIGHT_POSITION, 1, LightPosition);
            IPxlSdrBuf.add_uniform((ShaderBuffer)ret, Float32_Param, LIGHT_INFLUENCE, 1, LightInfluence);
            break;
        }
    }

    EString_delete(prefix);
    return ret;
}

Pass create_std_pass_from_dec(std_pass_status* _status, bool write_log)
{
    char mbuf[STRING_BUFFER_SIZE];
    ERROR_PROC;
    VtxSdrBuf vsb = create_std_vertex_shader_buffer(_status->dec.dec, _status->use_proj_mat, _status->use_world_mat);
    ERROR_PROC;
    PxlSdrBuf psb = create_std_pixel_shader_buffer(_status);
    ERROR_PROC;

    ShaderNode vsn = create_std_vertex_shader_node(_status->dec.dec, vsb, _status->use_proj_mat, _status->use_world_mat);
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

    IVtxSdrBuf.complete(vsb);
    IPxlSdrBuf.complete(psb);

    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    sb = to_ShaderBuffer(vsb);
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
        "#version %d%d0\n %s", GLSL_MAIN_VERSION, GLSL_SUB_VERSION, sb->output);
    Shader_load_from_string(auto_vs, mbuf, VertexShader);
    if (write_log)
        slog(StdPassLog, "%s", mbuf);

    sb = to_ShaderBuffer(psb);
    ///sprintf(mbuf, "#version 140\n#extension GL_ARB_gpu_shader5 : enable\n%s", sb.self->output);
	snprintf(mbuf, STRING_BUFFER_SIZE - 1,
		"#version %d%d0\n %s", GLSL_MAIN_VERSION, GLSL_SUB_VERSION, sb->output);
    Shader_load_from_string(auto_ps, mbuf, PixelShader);
    if (write_log)
        slog(StdPassLog, "%s", mbuf);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    Tree vtx_param_src_tree = ShaderBuffer_sell_param_source_object_tree(to_ShaderBuffer(vsb));
    Tree pxl_param_src_tree = ShaderBuffer_sell_param_source_object_tree(to_ShaderBuffer(psb));

    Pass_buy_vertex_param_source_tree(ret, vtx_param_src_tree);
    Pass_buy_pixel_param_source_tree(ret, pxl_param_src_tree);

    ShaderNode_delete(vsn);
    ShaderNode_delete(psn);
    IVtxSdrBuf._Delete(vsb);
    IPxlSdrBuf._Delete(psb);
    return ret;
}
