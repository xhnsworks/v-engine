#ifndef VERTEX_SHADER_BUFFER_H
#define VERTEX_SHADER_BUFFER_H
#include "common.h"
#include "etypes.h"
#include "shader_object.h"
#include "shader_buffer.h"
struct _vertex_shader_buffer;
typedef struct vertex_shader_buffer_
{
    struct _vertex_shader_buffer* self;
} VtxSdrBuf;

API_EXPORT VtxSdrBuf _VtxSdrBuf_new(const char* _file, uint _line);
API_EXPORT void _VtxSdrBuf_delete(VtxSdrBuf _vsb, const char* _file, uint _line);
#define VtxSdrBuf_new() _VtxSdrBuf_new(__FILE__, __LINE__)
#define VtxSdrBuf_delete(p) _VtxSdrBuf_delete(p, __FILE__, __LINE__)
API_EXPORT ShaderObject VtxSdrBuf_add_attribute(VtxSdrBuf _vsb, element_semantic _sem, element_type _type);
API_EXPORT void VtxSdrBuf_complete(VtxSdrBuf _vsb);
API_EXPORT void VtxSdrBuf_print(VtxSdrBuf _vsb);

struct _i_vtx_sdr_buf
{
    VtxSdrBuf (*__New)(const char* _file, uint _line);
    void (*__Delete)(VtxSdrBuf _vsb, const char* _file, uint _line);
    ShaderObject (*add_attribute)(VtxSdrBuf _vsb, element_semantic _sem, element_type _type);
    void (*complete)(VtxSdrBuf _vsb);
    void (*print)(VtxSdrBuf _vsb);

    ShaderObject (*add_varying)(VtxSdrBuf _sb, param_type _type, const char* _vary, sint32 _src);
    ShaderObject (*_add_uniform)(VtxSdrBuf _sb, param_type _type, const char* _unif, uint32 _array_size, sint32 _src,
                                                  const char* _file, uint _line);
    ShaderObject (*_add_uniform_from_renderer)(VtxSdrBuf _self, Renderer _rdr, sint32 _id, const char* _unif,
                                                                const char* _file, uint _line);
    ShaderObject (*new_object)(VtxSdrBuf _sb, shader_object_type _type, const char* _name, uint32 _array_size);
    ShaderObject (*new_immediate_float_object)(VtxSdrBuf _sb, float _ft);
    ShaderObject (*new_immediate_int_object)(VtxSdrBuf _sb, int _i);
    ShaderNode (*add_prototype_node)(VtxSdrBuf _sb, ShaderNode _sn);
    ShaderNode (*_add_reference_node)(VtxSdrBuf _sb, const char* _name, const char* _file, uint _line);
    ShaderNode (*_add_branch_node)(VtxSdrBuf _sb, BranchNode _bn, const char* _file, uint _line);
    ShaderObject (*find_object)(VtxSdrBuf _sb, const char* _name);
    void (*print_shader_object)(VtxSdrBuf _sb);
    void (*push_shader_object)(VtxSdrBuf _sb, ShaderObject _so);
    void (*push_immediate_shader_object)(VtxSdrBuf _sb, ShaderObject _so);
    Tree (*sell_param_source_object_tree)(VtxSdrBuf _sb);
    const char* (*get_output)(VtxSdrBuf _sb);
};

static struct _i_vtx_sdr_buf IVtxSdrBuf = {
    _VtxSdrBuf_new,
    _VtxSdrBuf_delete,
    VtxSdrBuf_add_attribute,
    VtxSdrBuf_complete,
    VtxSdrBuf_print,

    ShaderBuffer_add_varying,
    _ShaderBuffer_add_uniform,
    _ShaderBuffer_add_uniform_from_renderer,
    ShaderBuffer_new_object,
    ShaderBuffer_new_immediate_float_object,
    ShaderBuffer_new_immediate_int_object,
    ShaderBuffer_add_prototype_node,
    _ShaderBuffer_add_reference_node,
    _ShaderBuffer_add_branch_node,
    ShaderBuffer_find_object,
    ShaderBuffer_print_shader_object,
    ShaderBuffer_push_shader_object,
    ShaderBuffer_push_immediate_shader_object,
    ShaderBuffer_sell_param_source_object_tree,
    ShaderBuffer_get_output,
};
#endif
