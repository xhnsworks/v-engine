#ifndef SHADER_H
#define SHADER_H
#include <common.h>
#include <etypes.h>

struct _shader;
typedef struct shader_
{
    struct _shader* self;
} Shader;

API_EXPORT Shader Shader_new();
///API_EXPORT void Shader_Dest(struct _shader* _sdr);
API_EXPORT void Shader_delete(Shader _sdr);
API_EXPORT shader_type Shader_get_type(Shader _sdr);
API_EXPORT uint32 Shader_get_id(Shader _sdr);
API_EXPORT void _Shader_load_from_string(const char* _file, euint _line, Shader _sdr, const char* _str, shader_type _type);
#define Shader_load_from_string(sd, s, t) _Shader_load_from_string(__FILE__, __LINE__, sd, s, t)
#endif
