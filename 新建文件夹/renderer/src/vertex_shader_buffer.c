#include "vertex_shader_buffer.h"
#include "shader_buffer.h"
#include "mem.h"
#include "estring.h"
typedef struct _vertex_shader_buffer
{
    shader_buffer base;
    char* attribute;
} vertex_shader_buffer;
///==============================VertexShaderBuffer========================================///
vertex_shader_buffer* VertexShaderBuffer_Init(struct _vertex_shader_buffer* _buf, const char* _file, uint _line)
{
    vertex_shader_buffer* ret = (vertex_shader_buffer*)ShaderBuffer_Init((struct _shader_buffer*)_buf, _file, _line);
    ret->attribute = EString_new("///attribute///\n");

    ShaderBuffer sb = {(struct _shader_buffer*)_buf};

    ShaderObject vertex_pos_in = ShaderObject_new(Float4_Obj, "gl_Vertex", 1);
    ShaderObject vertex_pos_out = ShaderObject_new(Float4_Obj, "gl_Position", 1);

    ShaderBuffer_push_shader_object(sb, vertex_pos_in);
    ShaderBuffer_push_shader_object(sb, vertex_pos_out);
    return ret;
}

void VertexShaderBuffer_Dest(struct _vertex_shader_buffer* _buf, const char* _file, uint _line)
{
    EString_delete(_buf->attribute);
}

VertexShaderBuffer _VertexShaderBuffer_new(const char* _file, uint _line)
{
    VertexShaderBuffer ret;
    ret.self = _Malloc(sizeof(vertex_shader_buffer), _file, _line);
    ret.self = VertexShaderBuffer_Init(ret.self, __FILE__, __LINE__);
    return ret;
}

void _VertexShaderBuffer_delete(VertexShaderBuffer _vsb, const char* _file, uint _line)
{
    VertexShaderBuffer_Dest(_vsb.self, _file, _line);
    ShaderBuffer sb = to_ShaderBuffer(_vsb);
    ShaderBuffer_Dest(sb.self, __FILE__, __LINE__);
    Mfree(_vsb.self);
}

void VertexShaderBuffer_print(VertexShaderBuffer _vsb)
{
    printf(_vsb.self->base.output);
}
ShaderObject VertexShaderBuffer_add_attribute(VertexShaderBuffer _vsb, element_semantic _sem, element_type _type)
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
    char* tmp0 = EString_new("attribute ");
    char* tmp1 = EString_add(tmp0, type);
    char* tmp2 = EString_add(tmp1, " ");
    char* tmp3 = EString_add(tmp2, attr);
    char* tmp4 = EString_add(tmp3, ";\n");
    EString_delete(tmp0);
    EString_delete(tmp1);
    EString_delete(tmp2);
    EString_delete(tmp3);
    tmp1 = _vsb.self->attribute;
    _vsb.self->attribute = EString_add(_vsb.self->attribute, tmp4);
    EString_delete(tmp1);
    EString_delete(tmp4);

    ShaderBuffer sb = to_ShaderBuffer(_vsb);
    ShaderBuffer_push_shader_object(sb, ret);
    return ret;
}
void VertexShaderBuffer_release(VertexShaderBuffer _vsb)
{
    ShaderBuffer_release(&_vsb.self->base);
    char* tmp0 = EString_new("///shader///\n");
    char* tmp1 = EString_add(tmp0, _vsb.self->attribute);
    char* tmp2 = EString_add(tmp1, _vsb.self->base.output);
    char* tmp3 = _vsb.self->base.output;
    _vsb.self->base.output = tmp2;
    EString_delete(tmp0);
    EString_delete(tmp1);
    EString_delete(tmp3);
}
