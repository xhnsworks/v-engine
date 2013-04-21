#ifndef SHADER_BUFFER_H
#define SHADER_BUFFER_H
#include "common.h"
#include "etypes.h"
#include "shader_object.h"
#include "shader_node.h"
#include "tree.h"
struct _shader_buffer;
typedef struct _shader_buffer
{
    Tree shader_node_prototype_tree;
    ShaderNode* shader_node_reference_table;
    ///ShaderNode* shader_node_table;
    ///ShaderObject* main_var_table;
    Tree shader_object_tree;

    char* function;
    char* main;
    char* uniform;
    char* varying;
    char* output;
} shader_buffer;
typedef struct shader_buffer_
{
    struct _shader_buffer* self;
} ShaderBuffer;
extern shader_buffer* ShaderBuffer_Init(struct _shader_buffer* _buf, const char* _file, uint _line);
extern void ShaderBuffer_Dest(struct _shader_buffer* _buf, const char* _file, uint _line);
extern void ShaderBuffer_release(struct _shader_buffer* _buf);
extern ShaderObject ShaderBuffer_add_varying(ShaderBuffer _sb, param_type _type, const char* _vary);
extern ShaderObject ShaderBuffer_add_uniform(ShaderBuffer _sb, param_type _type, const char* _unif, uint32 _array_size);
extern ShaderObject ShaderBuffer_new_object(ShaderBuffer _sb, shader_object_type _type, const char* _name, uint32 _array_size);
extern void ShaderBuffer_add_prototype_node(ShaderBuffer _sb, ShaderNode _sn);
///extern void ShaderBuffer_add_reference_node(ShaderBuffer _sb, ShaderNode _sn);
extern ShaderNode _ShaderBuffer_add_reference_node(ShaderBuffer _sb, const char* _name, const char* _file, uint _line);
#define ShaderBuffer_add_reference_node(s, n) _ShaderBuffer_add_reference_node(s, n, __FILE__, __LINE__)
extern ShaderObject ShaderBuffer_find_object(ShaderBuffer _sb, const char* _name);

extern void ShaderBuffer_print_shader_object(ShaderBuffer _sb);
extern void ShaderBuffer_push_shader_object(ShaderBuffer _sb, ShaderObject _so);

extern const char* get_element_type_string(element_type _type);
extern const char* get_element_semantic_string(element_semantic _sem);
extern element_type get_element_semantic_type(element_semantic _sem);
extern param_type get_element_param_type(element_semantic _sem);
extern const char* get_param_type_string(param_type _type);
extern const char* get_shader_object_string(shader_object_type _type);
/// 所有ShaderObject的销毁都由ShaderBuffer管理

///======================///
extern void ref_init();
extern void ref_insert(ShaderNode _node, ShaderBuffer _buf);
extern void ref_earse(ShaderNode _node);
extern void ref_log();
#endif
