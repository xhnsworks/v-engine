#ifndef VERTEX_SHADER_BUFFER_H
#define VERTEX_SHADER_BUFFER_H
#include <common.h>
#include <etypes.h>
#include <shader_object.h>
struct _vertex_shader_buffer;
typedef struct vertex_shader_buffer_
{
    struct _vertex_shader_buffer* self;
} VertexShaderBuffer;

extern VertexShaderBuffer _VertexShaderBuffer_new(const char* _file, uint _line);
extern void _VertexShaderBuffer_delete(VertexShaderBuffer _vsb, const char* _file, uint _line);
#define VertexShaderBuffer_new() _VertexShaderBuffer_new(__FILE__, __LINE__)
#define VertexShaderBuffer_delete(p) _VertexShaderBuffer_delete(p, __FILE__, __LINE__)
extern ShaderObject VertexShaderBuffer_add_attribute(VertexShaderBuffer _vsb, element_semantic _sem, element_type _type);
extern void VertexShaderBuffer_release(VertexShaderBuffer _vsb);
extern void VertexShaderBuffer_print(VertexShaderBuffer _vsb);
#endif
