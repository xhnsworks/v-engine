#ifndef SHADER_BUFFER_H
#define SHADER_BUFFER_H
#include "common.h"
#include "etypes.h"
#include "shader_object.h"
#include "shader_node.h"
#include "branch_node.h"
#include "tree.h"
#include "circuit_board.h"
struct _shader_buffer;
typedef struct _shader_buffer
{
    Tree shader_node_prototype_tree;
    ///ShaderNode* shader_node_reference_table;
    Tree shader_object_tree;
    ShaderObject* imme_shader_objects;
    s_circuit_board circuit_board;
    /// key: sint32, data: ShaderObject
    Tree param_src_obj_tree;
    char* function;
    char* main;
    char* uniform;
    char* varying;
    char* output;
} shader_buffer;
/**
typedef struct shader_buffer_
{
    struct _shader_buffer* self;
} ShaderBuffer;
**/
class Renderer;
typedef struct _shader_buffer* ShaderBuffer;
API_EXPORT shader_buffer* ShaderBuffer_Init(struct _shader_buffer* _buf, const char* _file, uint _line);
API_EXPORT void ShaderBuffer_Dest(struct _shader_buffer* _buf, const char* _file, uint _line);
API_EXPORT void ShaderBuffer_complete(struct _shader_buffer* _buf);
API_EXPORT ShaderObject ShaderBuffer_add_varying(ShaderBuffer _sb, param_type _type, const char* _vary, sint32 _src);
API_EXPORT ShaderObject _ShaderBuffer_add_uniform(ShaderBuffer _sb, param_type _type, const char* _unif, uint32 _array_size, sint32 _src,
                                                  const char* _file, uint _line);
#define ShaderBuffer_add_uniform(s, t, u, as, ps) _ShaderBuffer_add_uniform(s, t, u, as, ps, __FILE__, __LINE__)

API_EXPORT ShaderObject _ShaderBuffer_add_uniform_from_renderer(ShaderBuffer _self, Renderer* _rdr, sint32 _id, const char* _unif,
                                                                const char* _file, uint _line);
#define ShaderBuffer_add_uniform_from_renderer(s, r, i, u) _ShaderBuffer_add_uniform_from_renderer(s, r, i, u, __FILE__, __LINE__)
API_EXPORT ShaderObject ShaderBuffer_new_object(ShaderBuffer _sb, shader_object_type _type, const char* _name, uint32 _array_size);
API_EXPORT ShaderObject ShaderBuffer_new_immediate_float_object(ShaderBuffer _sb, float _ft);
API_EXPORT ShaderObject ShaderBuffer_new_immediate_int_object(ShaderBuffer _sb, int _i);
API_EXPORT void ShaderBuffer_add_prototype_node(ShaderBuffer _sb, ShaderNode _sn);
///API_EXPORT void ShaderBuffer_add_reference_node(ShaderBuffer _sb, ShaderNode _sn);
API_EXPORT ShaderNode _ShaderBuffer_add_reference_node(ShaderBuffer _sb, const char* _name, const char* _file, uint _line);
#define ShaderBuffer_add_reference_node(s, n) _ShaderBuffer_add_reference_node(s, n, __FILE__, __LINE__)
API_EXPORT void _ShaderBuffer_add_branch_node(ShaderBuffer _sb, BranchNode _bn, const char* _file, uint _line);
#define ShaderBuffer_add_branch_node(s, b) _ShaderBuffer_add_branch_node(s, b, __FILE__, __LINE__)
API_EXPORT ShaderObject ShaderBuffer_find_object(ShaderBuffer _sb, const char* _name);

API_EXPORT void ShaderBuffer_print_shader_object(ShaderBuffer _sb);
API_EXPORT void ShaderBuffer_push_shader_object(ShaderBuffer _sb, ShaderObject _so);
API_EXPORT void ShaderBuffer_push_immediate_shader_object(ShaderBuffer _sb, ShaderObject _so);

API_EXPORT const char* ShaderBuffer_get_output(ShaderBuffer _sb);

/// 所有ShaderObject的销毁都由ShaderBuffer管理
API_EXPORT Tree ShaderBuffer_sell_param_source_object_tree(ShaderBuffer _sb);

API_EXPORT const char* get_element_type_string(element_type _type);
API_EXPORT const char* get_element_semantic_string(element_semantic _sem);
API_EXPORT element_type get_element_semantic_type(element_semantic _sem);
API_EXPORT param_type get_element_param_type(element_semantic _sem);
API_EXPORT const char* get_param_type_string(param_type _type);
API_EXPORT const char* get_shader_object_string(shader_object_type _type);

///======================///
API_EXPORT void ref_init();
API_EXPORT void ref_insert(ShaderNode _node, ShaderBuffer _buf);
API_EXPORT void ref_earse(ShaderNode _node);
API_EXPORT void ref_log();

#define _New()                                 __New(__FILE__, __LINE__)
#define _Delete(sb)                            __Delete(sb, __FILE__, __LINE__)
#define add_uniform(sb, t, u, a, s)            _add_uniform(sb, t, u, a, s, __FILE__, __LINE__)
#define add_uniform_from_renderer(sb, r, i, u) _add_uniform_from_renderer(sb, r, i, u, __FILE__, __LINE__)
#define add_reference_node(sb, n)              _add_reference_node(sb, n, __FILE__, __LINE__)
#define add_branch_node(sb, b)                 _add_branch_node(sb, b, __FILE__, __LINE__)

#ifdef _MSC_VER
inline ShaderBuffer ToShaderBuffer(void* t)
{
	ShaderBuffer ret = {(struct _shader_buffer*)t};
	return ret;
}
#endif

#endif
