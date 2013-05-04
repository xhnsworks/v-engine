#include "pch.h"
#include "shader_object.h"
#include "emem.h"
#include "estring.h"
#include "shader_buffer.h"
#include "elog.h"
#include "xhn_exception.hpp"
typedef struct _shader_object
{
    shader_object_type type;
    const char* name_string;
    euint32 array_size;
    euint32 array_index;
} shader_object;

///==============================ShaderObject========================================///
shader_object* ShaderObject_Init(struct _shader_object* _so, shader_object_type _type, const char* _name, euint32 _array_size)
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
ShaderObject ShaderObject_new(shader_object_type _type, const char* _name, euint32 _array_size)
{
    ShaderObject ret;
    ret.self = (struct _shader_object*)Malloc(sizeof(shader_object));
    if (!_array_size)
        _array_size = 1;
    ret.self = ShaderObject_Init(ret.self, _type, _name, _array_size);
    return ret;
}
void ShaderObject_delete(ShaderObject _so)
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

bool ShaderObject_equal(ShaderObject _so0, ShaderObject _so1)
{
    if (_so0.self->type != _so1.self->type)
        return false;
    if (!_so0.self->array_size || !_so1.self->array_size)
    {
        elog("%s", "error£¬shader object array size is zero");
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

shader_object_type ShaderObject_get_type(ShaderObject _so, euint32* _array_size, euint32* _array_index)
{
    *_array_size = _so.self->array_size;
    *_array_index = _so.self->array_index;
    return _so.self->type;
}

const char* ShaderObject_get_name(ShaderObject _so)
{
    return _so.self->name_string;
}
void ShaderObject_set_type(ShaderObject _so, shader_object_type _type, euint32 _array_size, euint32 _array_index)
{
    _so.self->array_size = _array_size;
    _so.self->array_index = _array_index;
    _so.self->type = _type;
}
void ShaderObject_set_index(ShaderObject _so, euint32 _array_index)
{
    _so.self->array_index = _array_index;
}
void ShaderObject_print(ShaderObject _so)
{
    printf("####shader object type %s, name %s\n", get_shader_object_string(_so.self->type), _so.self->name_string);
}

_INLINE_ char _get_comp_char(component _comp)
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
_INLINE_ shader_object_type _get_min_type(component_index _comp_idx)
{
	shader_object_type t = Float2_Obj;
	for (euint i = 0; i < _comp_idx.num_comps; i++)
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
        default:
            VEngineExce(InvalidEnumerationException, "get min type fail");
            break;
		}
	}
	return t;
}
ShaderObject ShaderObject_get_component(ShaderObject _so, component_index _comp_idx, euint32 _array_idx)
{
    ShaderObject ret = {NULL};
    
    if (_array_idx != INVALID_ARRAY_INDEX && _array_idx >= _so.self->array_size)
        return ret;

    if (_so.self->type < _get_min_type(_comp_idx))
        return ret;

    int remainder = 256;
    char mbuf[256];
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

    for (euint i = 0; i < _comp_idx.num_comps; i++)
    {
        offs = snprintf(str, remainder, "%c", _get_comp_char(_comp_idx.comps[i]));
        str += offs;
        remainder -= offs;
    }

    shader_object_type type = Void_Obj;
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
    ret.self = (struct _shader_object*)SMalloc(sizeof(shader_object));
    ret.self->name_string = EString_new(mbuf);
    ret.self->type = (shader_object_type)((euint)Float_Obj + _comp_idx.num_comps - 1);
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
    ret.self = (struct _shader_object*)SMalloc(sizeof(shader_object));
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

ShaderObject ShaderObject_new_from_sint(esint _v)
{
	char mbuf[256];
#if BIT_WIDTH == 32
	snprintf(mbuf, 255, "%d", _v);
#elif BIT_WIDTH == 64
    snprintf(mbuf, 255, "%lld", _v);
#endif
	ShaderObject ret = ShaderObject_new(Int_Obj, mbuf, 1);
	return ret;
}

ShaderObject ShaderObject_sample_texture2d_rgb(ShaderObject _sampler, ShaderObject _uv)
{
    ShaderObject ret = {NULL};
	if (_sampler.self->type != Texture2D_Obj)
		return ret;
	if (_uv.self->type != Float2_Obj)
		return ret;
	char mbuf[256];
	snprintf(mbuf, 255, "texture(%s, %s).rgb", _sampler.self->name_string, _uv.self->name_string);
	ret = ShaderObject_new(Float3_Obj, mbuf, 1);
	return ret;
}
ShaderObject ShaderObject_sample_texture2d_rgba(ShaderObject _sampler, ShaderObject _uv)
{
	ShaderObject ret = {NULL};
	if (_sampler.self->type != Texture2D_Obj)
		return ret;
	if (_uv.self->type != Float2_Obj)
		return ret;
	char mbuf[256];
	snprintf(mbuf, 255, "texture(%s, %s).rgba", _sampler.self->name_string, _uv.self->name_string);
	ret = ShaderObject_new(Float4_Obj, mbuf, 1);
	return ret;
}
