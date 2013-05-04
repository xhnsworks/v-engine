#include "pch.h"
#include "pixel_shader_buffer.h"
#include "shader_buffer.h"
#include "emem.h"
typedef struct _pixel_shader_buffer
{
    shader_buffer base;
} pixel_shader_buffer;

///==============================PxlSdrBuf========================================///
pixel_shader_buffer* PxlSdrBuf_Init(struct _pixel_shader_buffer* _buf)
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

void PxlSdrBuf_Dest(struct _pixel_shader_buffer* _buf)
{
}

PxlSdrBuf PxlSdrBuf_new()
{
    PxlSdrBuf ret;
    ret = (pixel_shader_buffer*)Malloc(sizeof(pixel_shader_buffer));
    ret = (pixel_shader_buffer*)ShaderBuffer_Init((struct _shader_buffer*)ret);
    ret = PxlSdrBuf_Init(ret);
    return ret;
}

void PxlSdrBuf_delete(PxlSdrBuf _psb)
{
    PxlSdrBuf_Dest(_psb);
    ShaderBuffer sb = to_ShaderBuffer(_psb);
    ShaderBuffer_Dest(sb);
    Mfree(_psb);
}

void PxlSdrBuf_complete(PxlSdrBuf _psb)
{
    ShaderBuffer_complete((ShaderBuffer)_psb);
}
