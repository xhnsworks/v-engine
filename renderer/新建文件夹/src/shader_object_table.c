#include "common.h"
#include "etypes.h"
#include "shader_object_table.h"
#include "shader_buffer.h"
#include "shader_object.h"
sint32 get_param_source(element_semantic sem)
{
    switch (sem)
    {
        case Position:
        return VaryPosition;
        case TexCoord:
        return VaryTexCoord;
        case Normal:
        return VaryNormal;
        case Tangent:
        return VaryTangent;
        case Binormal:
        return VaryBinormal;
        default:
        return EmptySrc;
    }
}

ShaderObject get_param_source_shader_object(ShaderBuffer _sb, sint32 _src)
{
    ShaderObject ret = {NULL};
    var key, data;
    key.uint64_var = _src;
    Iterator iter = Tree_find(_sb.self->param_src_obj_tree, key, &data);
    if (iter)
        ret.self = data.vptr_var;
    return ret;
}

/// 解压节点数据输入节点必须是该节点的第一个参数
typedef struct _shader_object_decompression_process
{
    ShaderNode proc_node;
} shader_obj_dec_proc;

typedef struct _semantic_real_type_compression_type_decompression_process_entry
{
    element_semantic sem;
    shader_object_type real_type;
    shader_object_type comp_type;
} semantic_entry;

semantic_entry semantic_table[] =
{
    {Normal, Float3_Obj, Float2_Obj},
};
/**
shader_object_type _get_semantic_object_type(element_semantic _sem)
{
}
**/
#define MAX_SHADER_OBJECTS 16
typedef struct _shader_table
{
    uint num_shader_object;
    element_semantic table[16];
} shader_table;
