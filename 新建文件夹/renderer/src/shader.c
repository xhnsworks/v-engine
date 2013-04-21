#include "shader.h"
#include "recycle_bin.h"
#include <GL/glew.h>
#include <GL/gl.h>

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
    ret.self = Malloc(sizeof(shader));
    memset(ret.self, 0, sizeof(shader));
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

void Shader_load_from_string(Shader _sdr, const char* _str, shader_type _type)
{
    uint32 p;
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
    glCompileShader(p);
    _sdr.self->id = p;
    _sdr.self->type = _type;

    ///
    uint32 count = 0;
    while(_str[count])
    {
        count++;
    }
    FILE* fp = fopen("D:\\shader_output.txt", "w+");
    fwrite(_str, count, 1, fp);
    fclose(fp);
    ///
}




