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
#include "shader_object_table.h"
#define POINT_LIGHT_AFFECT_DISTANCE 6.8f

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
                            "    Result = pos.xyz;\n"
                            "}\n");
    ShaderNode_add_input_param(ret, Float2_Obj,    "TexCoord", 1);
    ShaderNode_add_input_param(ret, Float4_Obj,    "Enc", 1);
    ShaderNode_add_input_param(ret, Float_Obj,     "Near", 1);
    ShaderNode_add_input_param(ret, Float_Obj,     "Far", 1);
    ShaderNode_add_input_param(ret, Float_Obj,     "Width", 1);
    ShaderNode_add_input_param(ret, Float_Obj,     "Height", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "InvCameraMatrix", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "InvProjectionMatrix", 1);
    ///ShaderNode_set_return_type(ret, Float3_Obj, 1);
	ShaderNode_add_output_param(ret, Float3_Obj, "Result", 1);

    return ret;
}

ShaderNode create_normal_decode_node()
{
/**
float2 fenc = enc*2-1;
n.z = -(dot(fenc,fenc)*2-1);
n.xy = normalize(fenc) * sqrt(1-n.z*n.z);
**/
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "NormalDecode");
    ShaderNode_set_function(ret,
                            "{\n"
                            "    vec3 ret;\n"
                            ///"    ret.xy = Enc.xy * 2.0 - 1.0;\n"
                            ///"    ret.z = sqrt(1.0 - dot(ret.xy, ret.xy));\n"
                            "    vec2 fenc = Enc.xy * 2.0 - 1.0;\n"
                            "    ret.z = -(dot(fenc, fenc) * 2.0 - 1.0);\n"
                            "    ret.xy = normalize(fenc) * sqrt(1.0 - ret.z * ret.z);\n"
                            "    Result = ret;\n"
                            "}\n");
    ShaderNode_add_input_param(ret, Float4_Obj, "Enc", 1);
    ///ShaderNode_set_return_type(ret, Float3_Obj, 1);
	ShaderNode_add_output_param(ret, Float3_Obj, "Result", 1);
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
                                      "    vec4 pos = vec4(Position.xyz, 1.0);\n"
                                      "    gl_Position = pos;\n"
                                      "    vPosition = pos;\n");

    for (euint32 i = 0; i < VertexDecl_count(_dec); i++)
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

VtxSdrBuf create_lighting_vertex_shader_buffer(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        VtxSdrBuf ret = {NULL};
        return ret;
    }

    VtxSdrBuf ret = IVtxSdrBuf._New();
    ///ShaderBuffer sb = to_ShaderBuffer(ret);

    const char* prefix = EString_new("v");

    for (euint32 i = 0; i < VertexDecl_count(_dec); i++)
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
            esint32 src = get_param_source(sem);
            IVtxSdrBuf.add_varying((ShaderBuffer)ret, pam_type, vary_str, src);
            EString_delete(vary_str);
        }
    }

    EString_delete(prefix);
    return ret;
}
/**
PxlSdrBuf create_lighting_pixel_shader_buffer(Renderer _rdr, VertexDecl _dec, bool _has_lighting_map)
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
        IPxlSdrBuf.add_varying(ret, pam_type, vary_str, src);
        EString_delete(vary_str);
    }

    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, NormalSketch, NORMAL_MAP);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, Plaster, DEPTH_MAP);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, CameraPlaneWidth, CAMERA_PLANE_WIDTH);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, CameraPlaneHeight, CAMERA_PLANE_HEIGHT);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, CameraPlaneNear, CAMERA_PLANE_NEAR);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, CameraPlaneFar, CAMERA_PLANE_FAR);

    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, PointLightPosition, POINT_LIGHT_POSITIOIN);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, PointLightColor, POINT_LIGHT_COLOR);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, PointLightAtteCoef, POINT_LIGHT_ATTE_COEF);

    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, InvertCameraWorldMatrix, INVERT_CAMERA_WORLD_MATRIX);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, InvertCameraProjectionMatrix, INVERT_CAMERA_PROJECTION_MATRIX);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, CameraWorldMatrix, CAMERA_WORLD_MATRIX);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, CameraProjectionMatrix, CAMERA_PROJECTION_MATRIX);
    IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, MaterialIDSketch, "MaterialIDMap");

    if (_has_lighting_map)
    {
        IPxlSdrBuf.add_uniform_from_renderer(ret, _rdr, LightingSketch, "LightingMap");
    }

    EString_delete(prefix);
    return ret;
}
**/
ShaderNode create_to_camera_space_node()
{
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "ToCameraSpace");

    ShaderNode_set_function(ret, "{\n"
                                 "    mat3 m = mat3(CamWorldMat);\n"
                                 "    vec3 ret = m * Dir;\n"
                                 "    Result = ret;\n"
                                 "}\n");
    ShaderNode_add_input_param(ret, Float3_Obj, "Dir", 1);
    ShaderNode_add_input_param(ret, Matrix4x4_Obj, "CamWorldMat", 1);
    ///ShaderNode_add_input_param(ret, Matrix4x4_Obj, "CamProjMat", 1);
    ///ShaderNode_set_return_type(ret, Float3_Obj, 1);
	ShaderNode_add_output_param(ret, Float3_Obj, "Result", 1);
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
             "        a = clamp( (%f - a) * (1.0 - LightAtteCoef), 0.0, 1.0);\n"
             "        a = pow(a, 5.0);\n"
             "        float d = dot(lit_dir, TargetNor);\n"
             "        d = clamp(d, 0.0, 1.0);\n"
             "        vec3 c = LightColor.rgb * d * a;\n"
             "        ret += c;\n"
             "    }\n"
             "    Result = ret;\n"
             "}\n",
             light_proc
             , POINT_LIGHT_AFFECT_DISTANCE);

    ShaderNode_set_function(ret, mbuf);

    ShaderNode_add_input_param(ret, Float3_Obj, "Lighting", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "TargetPos", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "TargetNor", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightPosArray", MAX_LIGHTS_PER_PASS);
    ShaderNode_add_input_param(ret, Float4_Obj, "LightColorArray", MAX_LIGHTS_PER_PASS);
    ShaderNode_add_input_param(ret, Float_Obj, "LightAtteCoefArray", MAX_LIGHTS_PER_PASS);
    ShaderNode_add_input_param(ret, Float_Obj, "TargetSpecular", 1);
    ///ShaderNode_add_input_param(ret, Int_Obj, "LightCount", 1);

    ///ShaderNode_set_return_type(ret, Float3_Obj, 1);
	ShaderNode_add_output_param(ret, Float3_Obj, "Result", 1);
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
                            "    Result = LightingValue0 + LightingValue1;\n"
                            "}\n");

    ShaderNode_add_input_param(ret, Float3_Obj, "LightingValue0", 1);
    ShaderNode_add_input_param(ret, Float3_Obj, "LightingValue1", 1);
    ///ShaderNode_set_return_type(ret, Float3_Obj, 1);
	ShaderNode_add_output_param(ret, Float3_Obj, "Result", 1);
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

#define USE_SWITCH_NODE

ParamTable ParamTable_new()
{
    ParamTable ret;
    ret.param_entries = Tree_new(String, Sint32, (MALLOC)Ealloc, (MFREE)Efree);
    return ret;
}
void ParamTable_delete(ParamTable _self)
{
    Tree_Dest(_self.param_entries);
    ///Efree(_self.param_entries);
}
void ParamTable_add_entry(ParamTable _self, const char* _name, esint32 _src)
{
    var key, data;
    key.str_var = _name;
    data.sint32_var = _src;
    Tree_insert(_self.param_entries, key, data);
}
void ParamTable_get_all_entrys(ParamTable _self, param_entry* _result)
{
    Iterator iter = Tree_begin(_self.param_entries);
    euint i = 0;
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
