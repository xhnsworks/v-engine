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
extern ShaderObject _ShaderObject_new(shader_object_type _type, const char* _name, uint32 _array_size, const char* _file, uint _line);
extern void _ShaderObject_delete(ShaderObject _so, const char* _file, uint _line);
#define ShaderObject_new(t, n, s) _ShaderObject_new(t, n, s, __FILE__, __LINE__)
#define ShaderObject_delete(s) _ShaderObject_delete(s, __FILE__, __LINE__)
extern ShaderObject ShaderObject_clone(ShaderObject _so);
extern shader_object_type ShaderObject_get_type(ShaderObject _so, uint32* _array_size, uint32* _array_index);
extern const char* ShaderObject_get_name(ShaderObject _so);
extern void ShaderObject_set_type(ShaderObject _so, shader_object_type _type, uint32 _array_size, uint32 _array_index);
extern void ShaderObject_set_index(ShaderObject _so, uint32 _array_index);
extern void ShaderObject_print(ShaderObject _so);
#endif
