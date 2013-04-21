#ifndef SHADER_NODE_H
#define SHADER_NODE_H
#include <common.h>
#include <etypes.h>
#include <shader_object.h>
struct _shader_node;
typedef struct shader_node_
{
    struct _shader_node* self;
} ShaderNode;

extern ShaderNode _ShaderNode_new(const char* _file, uint _line);
extern void _ShaderNode_delete(ShaderNode _sn, const char* _file, uint _line);
#define ShaderNode_new() _ShaderNode_new(__FILE__, __LINE__)
#define ShaderNode_delete(s) _ShaderNode_delete(s, __FILE__, __LINE__)
extern void ShaderNode_set_return_type(ShaderNode _sn, shader_object_type _type, uint32 _array_size);
extern void ShaderNode_add_input_param(ShaderNode _sn, shader_object_type _type, const char* _pam_name, uint32 _array_size);
extern void ShaderNode_add_output_param(ShaderNode _sn, shader_object_type _type, const char* _pam_name, uint32 _array_size);
extern void ShaderNode_add_input_link(ShaderNode _sn, ShaderObject _so, uint32 _array_index);
extern void ShaderNode_add_output_link(ShaderNode _sn, ShaderObject _so, uint32 _array_index);
extern void ShaderNode_set_result_link(ShaderNode _sn, ShaderObject _so, uint32 _array_index);
extern void ShaderNode_set_function(ShaderNode _sn, const char* _func);
extern void ShaderNode_clear_links(ShaderNode _sn);
extern const char* ShaderNode_compile(ShaderNode _sn);
extern const char* ShaderNode_compile_function_declaration(ShaderNode _sn);
extern const char* ShaderNode_get_function(ShaderNode _sn);
extern void _ShaderNode_set_name(ShaderNode _sn, const char* _name, const char* _file, uint _line);
#define ShaderNode_set_name(s, n) _ShaderNode_set_name(s, n, __FILE__, __LINE__)
extern const char* ShaderNode_get_name(ShaderNode _sn);
extern ShaderNode _ShaderNode_clone(ShaderNode _sn, const char* _file, uint _line);
#define ShaderNode_clone(s) _ShaderNode_clone(s, __FILE__, __LINE__)
#endif
