#include "shader_object.h"
#include "mem.h"
#include "estring.h"
#include "shader_buffer.h"
typedef struct _shader_object
{
    shader_object_type type;
    const char* name_string;
    uint32 array_size;
    uint32 array_index;
} shader_object;

///==============================ShaderObject========================================///
shader_object* ShaderObject_Init(struct _shader_object* _so, shader_object_type _type, const char* _name, uint32 _array_size)
{
    _so->type = _type;
    _so->name_string = EString_new(_name);
    _so->array_size = _array_size;
    _so->array_index = INVALID_ARRAY_INDEX;
    return _so;
}
void ShaderObject_Dest(shader_object* _so)
{
    EString_delete(_so->name_string);
}
ShaderObject _ShaderObject_new(shader_object_type _type, const char* _name, uint32 _array_size, const char* _file, uint _line)
{
    ShaderObject ret;
    ret.self = _Malloc(sizeof(shader_object), _file, _line);
    ret.self = ShaderObject_Init(ret.self, _type, _name, _array_size);
    return ret;
}
void _ShaderObject_delete(ShaderObject _so, const char* _file, uint _line)
{
    ShaderObject_Dest(_so.self);
    Mfree(_so.self);
}
ShaderObject ShaderObject_clone(ShaderObject _so)
{
    ShaderObject ret = ShaderObject_new(_so.self->type, _so.self->name_string, _so.self->array_size);
    ret.self->array_index = _so.self->array_index;
    return ret;
}

shader_object_type ShaderObject_get_type(ShaderObject _so, uint32* _array_size, uint32* _array_index)
{
    *_array_size = _so.self->array_size;
    *_array_index = _so.self->array_index;
    return _so.self->type;
}

const char* ShaderObject_get_name(ShaderObject _so)
{
    return _so.self->name_string;
}
void ShaderObject_set_type(ShaderObject _so, shader_object_type _type, uint32 _array_size, uint32 _array_index)
{
    _so.self->array_size = _array_size;
    _so.self->array_index = _array_index;
    _so.self->type = _type;
}
void ShaderObject_set_index(ShaderObject _so, uint32 _array_index)
{
    _so.self->array_index = _array_index;
}
void ShaderObject_print(ShaderObject _so)
{
    printf("####shader object type %s, name %s\n", get_shader_object_string(_so.self->type), _so.self->name_string);
}
