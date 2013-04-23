#ifndef SHADER_NODE_H
#define SHADER_NODE_H
#include "common.h"
#include "etypes.h"
#include "shader_object.h"
/**
typedef struct shader_node_
{
    struct _shader_node* self;
} ShaderNode;
**/
typedef struct _shader_node* ShaderNode;

API_EXPORT ShaderNode _ShaderNode_new(const char* _file, uint _line);
API_EXPORT void _ShaderNode_delete(ShaderNode _sn, const char* _file, uint _line);
#define ShaderNode_new() _ShaderNode_new(__FILE__, __LINE__)
#define ShaderNode_delete(s) _ShaderNode_delete(s, __FILE__, __LINE__)
///API_EXPORT void ShaderNode_set_return_type(ShaderNode _sn, shader_object_type _type, uint32 _array_size);
API_EXPORT void _ShaderNode_add_input_param(ShaderNode _sn, shader_object_type _type, const char* _pam_name, uint32 _array_size,
                                            const char* _file, uint _line);
#define ShaderNode_add_input_param(s, t, p, a) _ShaderNode_add_input_param(s, t, p, a, __FILE__, __LINE__)
API_EXPORT void ShaderNode_add_output_param(ShaderNode _sn, shader_object_type _type, const char* _pam_name, uint32 _array_size);
API_EXPORT void _ShaderNode_add_input_link(const char* _file, uint _line, ShaderNode _sn, ShaderObject _so, uint32 _array_index);
#define ShaderNode_add_input_link(sn, so, idx) _ShaderNode_add_input_link(__FILE__, __LINE__, sn, so, idx)
API_EXPORT void ShaderNode_add_output_link(ShaderNode _sn, ShaderObject _so, uint32 _array_index);
///API_EXPORT void ShaderNode_set_result_link(ShaderNode _sn, ShaderObject _so, uint32 _array_index);
API_EXPORT void ShaderNode_set_function(ShaderNode _sn, const char* _func);
API_EXPORT void ShaderNode_clear_links(ShaderNode _sn);
API_EXPORT const char* ShaderNode_compile(ShaderNode _sn);
API_EXPORT const char* ShaderNode_compile_function_declaration(ShaderNode _sn);
API_EXPORT const char* ShaderNode_get_function(ShaderNode _sn);
API_EXPORT void _ShaderNode_set_name(ShaderNode _sn, const char* _name, const char* _file, uint _line);
#define ShaderNode_set_name(s, n) _ShaderNode_set_name(s, n, __FILE__, __LINE__)
API_EXPORT const char* ShaderNode_get_name(ShaderNode _sn);
API_EXPORT ShaderNode _ShaderNode_clone(ShaderNode _sn, const char* _file, uint _line);
#define ShaderNode_clone(s) _ShaderNode_clone(s, __FILE__, __LINE__)
#endif
