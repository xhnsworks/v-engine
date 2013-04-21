#include "shader_object.h"
#include "mem.h"
#include "estring.h"
#include "shader_buffer.h"
#include "elog.h"
typedef struct _shader_object
{
    shader_object_type type;
    const char* name_string;
    uint32 array_size;
    uint32 array_index;
} shader_object;

///==============================ShaderObject========================================///
#define ShaderObject_Init(s, t, n, as) _ShaderObject_Init(s, t, n, as, __FILE__, __LINE__)
shader_object* _ShaderObject_Init(struct _shader_object* _so, shader_object_type _type, const char* _name, uint32 _array_size,
                                  const char* _file, uint _line)
{
    _so->type = _type;
    _so->name_string = _EString_new(_name, _file, _line);
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
    if (!_array_size)
        _array_size = 1;
    ret.self = _ShaderObject_Init(ret.self, _type, _name, _array_size, _file, _line);
    return ret;
}
void _ShaderObject_delete(ShaderObject _so, const char* _file, uint _line)
{
    ShaderObject_Dest(_so.self);
    Mfree(_so.self);
}
ShaderObject _ShaderObject_clone(ShaderObject _so, const char* _file, uint _line)
{
    ShaderObject ret = _ShaderObject_new(_so.self->type, _so.self->name_string, _so.self->array_size, _file, _line);
    ret.self->array_index = _so.self->array_index;
    return ret;
}

bool ShaderObject_equal(ShaderObject _so0, ShaderObject _so1)
{
    if (_so0.self->type != _so1.self->type)
        return false;
    if (!_so0.self->array_size || !_so1.self->array_size)
    {
        elog("error£¬shader object array size is zero");
        return false;
    }
    /// the logic is not stringent enough
    if (_so0.self->array_size != _so1.self->array_size)
    {
        if (_so0.self->array_index == INVALID_ARRAY_INDEX || _so1.self->array_index == INVALID_ARRAY_INDEX)
            return false;
        else
            return true;
    }
    else
    {
        if (_so0.self->array_index == INVALID_ARRAY_INDEX && _so1.self->array_index == INVALID_ARRAY_INDEX)
            return true;
        else if (_so0.self->array_index != INVALID_ARRAY_INDEX && _so1.self->array_index != INVALID_ARRAY_INDEX)
            return true;
        else
            return false;
    }
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

ShaderObject ShaderObject_get_component(ShaderObject _so, component_index _comp_idx, uint32 _array_idx)
{
    ShaderObject ret = {NULL};
    char _get_comp_char(component _comp)
    {
        switch (_comp)
        {
        case CompX:
            return 'x';
        case CompY:
            return 'y';
        case CompZ:
            return 'z';
        case CompW:
            return 'w';
        case CompR:
            return 'r';
        case CompG:
            return 'g';
        case CompB:
            return 'b';
        case CompA:
            return 'a';
        default:
            return 0x00;
        }
    }
    element_type _get_min_type(component_index _comp_idx)
    {
        element_type t = Float2_Obj;
        for (int i = 0; i < _comp_idx.num_comps; i++)
        {
            switch (_comp_idx.comps[i])
            {
            case CompZ:
                if (t < Float3_Obj) { t = Float3_Obj; }
                break;
            case CompB:
                if (t < Float3_Obj) { t = Float3_Obj; }
                break;
            case CompW:
                if (t < Float4_Obj) { t = Float4_Obj; }
                break;
            case CompA:
                if (t < Float4_Obj) { t = Float4_Obj; }
                break;
            }
            return t;
        }
    }
    if (_array_idx != INVALID_ARRAY_INDEX && _array_idx >= _so.self->array_size)
        return ret;

    if (_so.self->type < _get_min_type(_comp_idx))
        return ret;

    int remainder = 256;
    char mbuf[remainder];
    remainder--;
    char* str = &mbuf[0];

    int offs  = 0;
    if      (_so.self->array_size == 1)
        offs = snprintf(str, remainder, "%s.", _so.self->name_string);
    else if (_so.self->array_size > 1)
        offs = snprintf(str, remainder, "%s[%d].", _so.self->name_string, _so.self->array_index);
    else
        return ret;

    str += offs;
    remainder -= offs;

    for (int i = 0; i < _comp_idx.num_comps; i++)
    {
        offs = snprintf(str, remainder, "%c", _get_comp_char(_comp_idx.comps[i]));
        str += offs;
        remainder -= offs;
    }

    element_type type = EmptyType;
    switch (_comp_idx.num_comps)
    {
    case 1:
        type = Float_Obj;
        break;
    case 2:
        type = Float2_Obj;
        break;
    case 3:
        type = Float3_Obj;
        break;
    case 4:
        type = Float4_Obj;
        break;
    default:
        return ret;
    }
    ret.self = Malloc(sizeof(shader_object));
    ret.self->name_string = EString_new(mbuf);
    ret.self->type = Float_Obj + _comp_idx.num_comps - 1;
    ret.self->array_size = 1;
    ret.self->array_index = INVALID_ARRAY_INDEX;

    return ret;
}

ShaderObject ShaderObject_float_to_int(ShaderObject _so, float _scale)
{
    ShaderObject null_obj = {NULL};
    if (_so.self->type != Float_Obj || _so.self->array_size != 1)
        return null_obj;
    ShaderObject ret;
    ret.self = Malloc(sizeof(shader_object));
    if (_scale != 1.0f)
    {
        char mbuf[STRING_BUFFER_SIZE];
        snprintf(mbuf, STRING_BUFFER_SIZE - 1, "int(%s * %f)", _so.self->name_string, _scale);
        ret.self->name_string = EString_new(mbuf);
    }
    else
    {
        char mbuf[STRING_BUFFER_SIZE];
        snprintf(mbuf, STRING_BUFFER_SIZE - 1, "int(%s)", _so.self->name_string);
        ret.self->name_string = EString_new(mbuf);
    }
    ret.self->type = Int_Obj;
    ret.self->array_size = 1;
    ret.self->array_index = INVALID_ARRAY_INDEX;
    return ret;
}
