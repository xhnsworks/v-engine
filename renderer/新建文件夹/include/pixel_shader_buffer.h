#ifndef PIXEL_SHADER_BUFFER_H
#define PIXEL_SHADER_BUFFER_H
#include "common.h"
#include "etypes.h"
#include "shader_object.h"
#include "shader_buffer.h"
struct _pixel_shader_buffer;
typedef struct pixel_shader_buffer_
{
    struct _pixel_shader_buffer* self;
} PxlSdrBuf;
API_EXPORT PxlSdrBuf _PxlSdrBuf_new(const char* _file, uint _line);
API_EXPORT void _PxlSdrBuf_delete(PxlSdrBuf _psb, const char* _file, uint _line);
#define PxlSdrBuf_new() _PxlSdrBuf_new(__FILE__, __LINE__)
#define PxlSdrBuf_delete(p) _PxlSdrBuf_delete(p, __FILE__, __LINE__)
API_EXPORT void PxlSdrBuf_complete(PxlSdrBuf _psb);

struct _i_pxl_sdr_buf
{
    PxlSdrBuf (*__New)(const char* _file, uint _line);
    void (*__Delete)(PxlSdrBuf _vsb, const char* _file, uint _line);
    void (*complete)(PxlSdrBuf _vsb);

    ShaderObject (*add_varying)(PxlSdrBuf _sb, param_type _type, const char* _vary, sint32 _src);
    ShaderObject (*_add_uniform)(PxlSdrBuf _sb, param_type _type, const char* _unif, uint32 _array_size, sint32 _src,
                                                  const char* _file, uint _line);
    ShaderObject (*_add_uniform_from_renderer)(PxlSdrBuf _self, Renderer _rdr, sint32 _id, const char* _unif,
                                                                const char* _file, uint _line);
    ShaderObject (*new_object)(PxlSdrBuf _sb, shader_object_type _type, const char* _name, uint32 _array_size);
    ShaderObject (*new_immediate_float_object)(PxlSdrBuf _sb, float _ft);
    ShaderObject (*new_immediate_int_object)(PxlSdrBuf _sb, int _i);
    ShaderNode (*add_prototype_node)(PxlSdrBuf _sb, ShaderNode _sn);
    ShaderNode (*_add_reference_node)(PxlSdrBuf _sb, const char* _name, const char* _file, uint _line);
    ShaderNode (*_add_branch_node)(PxlSdrBuf _sb, BranchNode _bn, const char* _file, uint _line);
    ShaderObject (*find_object)(PxlSdrBuf _sb, const char* _name);
    void (*print_shader_object)(PxlSdrBuf _sb);
    void (*push_shader_object)(PxlSdrBuf _sb, ShaderObject _so);
    void (*push_immediate_shader_object)(PxlSdrBuf _sb, ShaderObject _so);
    Tree (*sell_param_source_object_tree)(PxlSdrBuf _sb);
    const char* (*get_output)(PxlSdrBuf _sb);
};

static struct _i_pxl_sdr_buf IPxlSdrBuf = {
    _PxlSdrBuf_new,
    _PxlSdrBuf_delete,
    PxlSdrBuf_complete,

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
