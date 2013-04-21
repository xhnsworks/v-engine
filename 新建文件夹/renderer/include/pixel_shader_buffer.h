#ifndef PIXEL_SHADER_BUFFER_H
#define PIXEL_SHADER_BUFFER_H
#include <common.h>
#include <etypes.h>
#include <shader_object.h>
struct _pixel_shader_buffer;
typedef struct pixel_shader_buffer_
{
    struct _pixel_shader_buffer* self;
} PixelShaderBuffer;
extern PixelShaderBuffer _PixelShaderBuffer_new(const char* _file, uint _line);
extern void _PixelShaderBuffer_delete(PixelShaderBuffer _psb, const char* _file, uint _line);
#define PixelShaderBuffer_new() _PixelShaderBuffer_new(__FILE__, __LINE__)
#define PixelShaderBuffer_delete(p) _PixelShaderBuffer_delete(p, __FILE__, __LINE__)
extern void PixelShaderBuffer_release(PixelShaderBuffer _psb);
#endif
