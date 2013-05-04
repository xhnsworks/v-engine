#ifndef VERTEX_SHADER_BUFFER_H
#define VERTEX_SHADER_BUFFER_H
#include "common.h"
#include "etypes.h"
#include "shader_object.h"
#include "shader_buffer.h"
struct _vertex_shader_buffer;
/**
typedef struct vertex_shader_buffer_
{
    struct _vertex_shader_buffer* self;
} VtxSdrBuf;
**/
class Renderer;
typedef struct _vertex_shader_buffer* VtxSdrBuf;
API_EXPORT VtxSdrBuf VtxSdrBuf_new();
API_EXPORT void VtxSdrBuf_delete(VtxSdrBuf _vsb);
API_EXPORT ShaderObject VtxSdrBuf_add_attribute(VtxSdrBuf _vsb, element_semantic _sem, element_type _type);
API_EXPORT void VtxSdrBuf_complete(VtxSdrBuf _vsb);
API_EXPORT void VtxSdrBuf_print(VtxSdrBuf _vsb);

struct _i_vtx_sdr_buf
{
    VtxSdrBuf (*__New)();
    void (*__Delete)(VtxSdrBuf _vsb);
    ShaderObject (*add_attribute)(VtxSdrBuf _vsb, element_semantic _sem, element_type _type);
    void (*complete)(VtxSdrBuf _vsb);
    void (*print)(VtxSdrBuf _vsb);

    ShaderObject (*add_varying)(ShaderBuffer _sb, param_type _type, const char* _vary, esint32 _src);
    ShaderObject (*add_uniform)(ShaderBuffer _sb, param_type _type, const char* _unif, euint32 _array_size, esint32 _src);
    ShaderObject (*add_uniform_from_renderer)(ShaderBuffer _self, Renderer* _rdr, esint32 _id, const char* _unif);
    ShaderObject (*new_object)(ShaderBuffer _sb, shader_object_type _type, const char* _name, euint32 _array_size);
    ShaderObject (*new_immediate_float_object)(ShaderBuffer _sb, float _ft);
    ShaderObject (*new_immediate_int_object)(ShaderBuffer _sb, int _i);
    void (*add_prototype_node)(ShaderBuffer _sb, ShaderNode _sn);
    ShaderNode (*_add_reference_node)(ShaderBuffer _sb, const char* _name, const char* _file, euint _line);
    void (*_add_branch_node)(ShaderBuffer _sb, BranchNode _bn, const char* _file, euint _line);
    ShaderObject (*find_object)(ShaderBuffer _sb, const char* _name);
    void (*print_shader_object)(ShaderBuffer _sb);
    void (*push_shader_object)(ShaderBuffer _sb, ShaderObject _so);
    void (*push_immediate_shader_object)(ShaderBuffer _sb, ShaderObject _so);
    Tree (*sell_param_source_object_tree)(ShaderBuffer _sb);
    const char* (*get_output)(ShaderBuffer _sb);
};

static struct _i_vtx_sdr_buf IVtxSdrBuf = {
    VtxSdrBuf_new,
    VtxSdrBuf_delete,
    VtxSdrBuf_add_attribute,
    VtxSdrBuf_complete,
    VtxSdrBuf_print,

    ShaderBuffer_add_varying,
    ShaderBuffer_add_uniform,
    ShaderBuffer_add_uniform_from_renderer,
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
