#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H
#include <common.h>
#include <etypes.h>
struct _shader_object;
typedef struct shader_object_
{
    struct _shader_object* self;
} ShaderObject;
#define INVALID_ARRAY_INDEX 0xffffffff
API_EXPORT ShaderObject _ShaderObject_new(shader_object_type _type, const char* _name, euint32 _array_size, const char* _file, euint _line);
API_EXPORT void _ShaderObject_delete(ShaderObject _so, const char* _file, euint _line);
#define ShaderObject_new(t, n, s) _ShaderObject_new(t, n, s, __FILE__, __LINE__)
#define ShaderObject_delete(s) _ShaderObject_delete(s, __FILE__, __LINE__)
API_EXPORT ShaderObject _ShaderObject_clone(ShaderObject _so, const char* _file, euint _line);
#define ShaderObject_clone(s) _ShaderObject_clone(s, __FILE__, __LINE__)
API_EXPORT bool ShaderObject_equal(ShaderObject _so0, ShaderObject _so1);
API_EXPORT shader_object_type ShaderObject_get_type(ShaderObject _so, euint* _array_size, euint* _array_index);
API_EXPORT const char* ShaderObject_get_name(ShaderObject _so);
API_EXPORT void ShaderObject_set_type(ShaderObject _so, shader_object_type _type, euint32 _array_size, euint32 _array_index);
API_EXPORT void ShaderObject_set_index(ShaderObject _so, euint32 _array_index);
API_EXPORT void ShaderObject_print(ShaderObject _so);

typedef enum _component
{
    CompX,
    CompY,
    CompZ,
    CompW,
    CompR,
    CompG,
    CompB,
    CompA,
} component;

typedef struct _component_index
{
    euint num_comps;
    component comps[4];
} component_index;
API_EXPORT ShaderObject ShaderObject_get_component(ShaderObject _so, component_index _comp_idx, euint32 _array_idx);
API_EXPORT ShaderObject ShaderObject_float_to_int(ShaderObject _so, float _scale);

API_EXPORT ShaderObject ShaderObject_new_from_sint(esint _v);
API_EXPORT ShaderObject ShaderObject_sample_texture2d_rgb(ShaderObject _sampler, ShaderObject _uv);
API_EXPORT ShaderObject ShaderObject_sample_texture2d_rgba(ShaderObject _sampler, ShaderObject _uv);
#endif
