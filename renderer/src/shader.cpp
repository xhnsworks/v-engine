#include "pch.h"
#include "shader.h"
///#include <GL/gl.h>
#include "gl_utility.h"
#include "emem.h"

typedef struct _shader
{
    uint32 id;
    shader_type type;
} shader;

void Shader_Dest(struct _shader* _sdr)
{
    if ( _sdr->id )
        glDeleteShader(_sdr->id);
}
Shader Shader_new()
{
    Shader ret;
    ret.self = (struct _shader*)SMalloc(sizeof(shader));
    ///memset(ret.self, 0, sizeof(shader));
    return ret;
}

void Shader_delete(Shader _sdr)
{
    if (_sdr.self)
    {
        Shader_Dest(_sdr.self);
        Mfree(_sdr.self);
    }
}

shader_type Shader_get_type(Shader _sdr)
{
    return _sdr.self->type;
}
uint32 Shader_get_id(Shader _sdr)
{
    return _sdr.self->id;
}

void _Shader_load_from_string(const char* _file, uint _line, Shader _sdr, const char* _str, shader_type _type)
{
    uint32 p = 0;
    switch (_type)
    {
    case VertexShader:
        p = glCreateShader(GL_VERTEX_SHADER);
        break;
    case PixelShader:
        p = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
    glShaderSource(p, 1, &_str, NULL);
    _error_proc(_file, _line);
	///ERROR_PROC
    glCompileShader(p);
    ///ERROR_PROC
	_error_proc(_file, _line);
    _shader_log(_file, _line, p);

    _sdr.self->id = p;
    _sdr.self->type = _type;
}




