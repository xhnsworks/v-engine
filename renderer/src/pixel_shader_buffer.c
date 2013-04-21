#include "pixel_shader_buffer.h"
#include "shader_buffer.h"
#include "mem.h"
typedef struct _pixel_shader_buffer
{
    shader_buffer base;
} pixel_shader_buffer;

///==============================PixelShaderBuffer========================================///
pixel_shader_buffer* PixelShaderBuffer_Init(struct _pixel_shader_buffer* _buf, const char* _file, uint _line)
{
    ShaderBuffer sb = {(struct _shader_buffer*)_buf};
    ShaderObject color_out_0 = ShaderObject_new(Float4_Obj, "gl_FragData[0]", 1);
    ShaderObject color_out_1 = ShaderObject_new(Float4_Obj, "gl_FragData[1]", 1);
    ShaderObject color_out_2 = ShaderObject_new(Float4_Obj, "gl_FragData[2]", 1);
    ShaderObject color_out_3 = ShaderObject_new(Float4_Obj, "gl_FragData[3]", 1);

    ShaderBuffer_push_shader_object(sb, color_out_0);
    ShaderBuffer_push_shader_object(sb, color_out_1);
    ShaderBuffer_push_shader_object(sb, color_out_2);
    ShaderBuffer_push_shader_object(sb, color_out_3);
    return _buf;
}

void PixelShaderBuffer_Dest(struct _pixel_shader_buffer* _buf, const char* _file, uint _line)
{
}

PixelShaderBuffer _PixelShaderBuffer_new(const char* _file, uint _line)
{
    PixelShaderBuffer ret;
    ret.self = _Malloc(sizeof(pixel_shader_buffer), _file, _line);
    ret.self = (pixel_shader_buffer*)ShaderBuffer_Init((struct _shader_buffer*)ret.self, __FILE__, __LINE__);
    ret.self = PixelShaderBuffer_Init(ret.self, _file, _line);
    return ret;
}

void _PixelShaderBuffer_delete(PixelShaderBuffer _psb, const char* _file, uint _line)
{
    PixelShaderBuffer_Dest(_psb.self, _file, _line);
    ShaderBuffer sb = to_ShaderBuffer(_psb);
    ShaderBuffer_Dest(sb.self, __FILE__, __LINE__);
    Mfree(_psb.self);
}

void PixelShaderBuffer_release(PixelShaderBuffer _psb)
{
    ShaderBuffer_release(&_psb.self->base);
}
