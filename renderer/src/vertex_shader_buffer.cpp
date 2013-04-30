#include "pch.h"
#include "vertex_shader_buffer.h"
#include "shader_buffer.h"
#include "emem.h"
#include "estring.h"
typedef struct _vertex_shader_buffer
{
    shader_buffer base;
    char* attribute;
} vertex_shader_buffer;
///==============================VtxSdrBuf========================================///
vertex_shader_buffer* VtxSdrBuf_Init(struct _vertex_shader_buffer* _buf, const char* _file, euint _line)
{
    vertex_shader_buffer* ret = (vertex_shader_buffer*)ShaderBuffer_Init((struct _shader_buffer*)_buf, _file, _line);
    ret->attribute = (char*)EString_new("///attribute///\n");

    ShaderBuffer sb = {(struct _shader_buffer*)_buf};

    ShaderObject vertex_pos_in = ShaderObject_new(Float4_Obj, "gl_Vertex", 1);
    ShaderObject vertex_pos_out = ShaderObject_new(Float4_Obj, "gl_Position", 1);

    ShaderBuffer_push_shader_object(sb, vertex_pos_in);
    ShaderBuffer_push_shader_object(sb, vertex_pos_out);
    return ret;
}

void VtxSdrBuf_Dest(struct _vertex_shader_buffer* _buf, const char* _file, euint _line)
{
    EString_delete(_buf->attribute);
}

VtxSdrBuf _VtxSdrBuf_new(const char* _file, euint _line)
{
    VtxSdrBuf ret;
    ret = (struct _vertex_shader_buffer*)_Malloc(sizeof(vertex_shader_buffer), _file, _line);
    ret = VtxSdrBuf_Init(ret, __FILE__, __LINE__);
    return ret;
}

void _VtxSdrBuf_delete(VtxSdrBuf _vsb, const char* _file, euint _line)
{
    VtxSdrBuf_Dest(_vsb, _file, _line);
    ShaderBuffer sb = to_ShaderBuffer(_vsb);
    ShaderBuffer_Dest(sb, __FILE__, __LINE__);
    Mfree(_vsb);
}

void VtxSdrBuf_print(VtxSdrBuf _vsb)
{
    printf(_vsb->base.output);
}
ShaderObject VtxSdrBuf_add_attribute(VtxSdrBuf _vsb, element_semantic _sem, element_type _type)
{
    const char* attr = get_element_semantic_string(_sem);
    const char* type = get_element_type_string(_type);
    ShaderObject ret = {NULL};
    if (attr == NULL || type == NULL)
    {
        return ret;
    }

    switch (_type)
    {
    case Float32_1:
        ret = ShaderObject_new(Float_Obj, attr, 1);
        break;
    case Float32_2:
        ret = ShaderObject_new(Float2_Obj, attr, 1);
        break;
    case Float32_3:
        ret = ShaderObject_new(Float3_Obj, attr, 1);
        break;
    case Float32_4:
        ret = ShaderObject_new(Float4_Obj, attr, 1);
        break;
    default:
        return ret;
    };
    const char* tmp0 = EString_new("attribute ");
    const char* tmp1 = EString_add(tmp0, type);
    const char* tmp2 = EString_add(tmp1, " ");
    const char* tmp3 = EString_add(tmp2, attr);
    const char* tmp4 = EString_add(tmp3, ";\n");
    EString_delete(tmp0);
    EString_delete(tmp1);
    EString_delete(tmp2);
    EString_delete(tmp3);
    tmp1 = _vsb->attribute;
    _vsb->attribute = (char*)EString_add(_vsb->attribute, tmp4);
    EString_delete(tmp1);
    EString_delete(tmp4);

    ShaderBuffer sb = to_ShaderBuffer(_vsb);
    ShaderBuffer_push_shader_object(sb, ret);
    return ret;
}
void VtxSdrBuf_complete(VtxSdrBuf _vsb)
{
    ShaderBuffer_complete(&_vsb->base);
    const char* tmp0 = EString_new("///shader///\n");
    const char* tmp1 = EString_add(tmp0, _vsb->attribute);
    const char* tmp2 = EString_add(tmp1, _vsb->base.output);
    const char* tmp3 = _vsb->base.output;
    _vsb->base.output = (char*)tmp2;
    EString_delete(tmp0);
    EString_delete(tmp1);
    EString_delete(tmp3);
}
