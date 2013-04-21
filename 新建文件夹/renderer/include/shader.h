#ifndef SHADER_H
#define SHADER_H
#include <common.h>
#include <etypes.h>

struct _shader;
typedef struct shader_
{
    struct _shader* self;
} Shader;

extern Shader Shader_new();
///extern void Shader_Dest(struct _shader* _sdr);
extern void Shader_delete(Shader _sdr);
extern shader_type Shader_get_type(Shader _sdr);
extern uint32 Shader_get_id(Shader _sdr);
extern void Shader_load_from_string(Shader _sdr, const char* _str, shader_type _type);

#endif
