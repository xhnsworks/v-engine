#include "texture2d.h"
#include "recycle_bin.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_utility.h"
typedef struct _texture2d
{
    uint32 id;
    pixel_format format;
    uint32 width;
    uint32 height;
    uint32 have_image;
    vptr pxl_buffer;
} texture2d;

void _Texture2D_delete(Texture2D _tex, const char* _file, uint _line)
{
    printf("teture dest\n");
    glDeleteTextures(1, &_tex->id);
    if (_tex->pxl_buffer)
        Mfree(_tex->pxl_buffer);
    Mfree(_tex);
}
Texture2D _Texture2D_new(const char* _file, uint _line)
{
    Texture2D ret = _Malloc( sizeof(texture2d), _file, _line );
    memset(ret, 0, sizeof(texture2d));

    glGenTextures(1, &ret->id);

    return ret;
}

void Texture2D_updata(Texture2D _tex)
{
    if (_tex->have_image)
    {
        glDeleteTextures(1, &_tex->id);
    }
    _tex->have_image = 1;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, _tex->id);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    switch (_tex->format)
    {
    case RGBA8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _tex->width,
                     _tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    case RGB8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _tex->width,
                     _tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    case RGBA32F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _tex->width,
                     _tex->height, 0, GL_RGBA, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
    case RGB32F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _tex->width,
                     _tex->height, 0, GL_RGB, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
    case RGBA16F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _tex->width,
                     _tex->height, 0, GL_RGBA, GL_HALF_FLOAT,
                     _tex->pxl_buffer);
        break;
    case RGB16F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _tex->width,
                     _tex->height, 0, GL_RGB, GL_HALF_FLOAT,
                     _tex->pxl_buffer);
        break;
    case DEPTH32F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _tex->width,
                     _tex->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
    }
}

void Texture2D_create(Texture2D _tex, pixel_format _fmt, uint32 _w, uint32 _h)
{
    _tex->format = _fmt;
    _tex->width = _w;
    _tex->height = _h;
    _tex->pxl_buffer = NULL;
    Texture2D_updata(_tex);
}

void Texture2D_load_from_mem(Texture2D _tex, vptr _mem, pixel_format _fmt, uint32 _w, uint32 _h, uint32 _size_in_byte)
{
    _tex->format = _fmt;
    _tex->width = _w;
    _tex->height = _h;
    ///_tex->pxl_buffer = _mem;
    _tex->pxl_buffer = Malloc(_size_in_byte);
    memcpy(_tex->pxl_buffer, _mem, _size_in_byte);
    Texture2D_updata(_tex);
}

void Texture2D_bind(Texture2D _tex)
{
    glBindTexture(GL_TEXTURE_2D, _tex->id);
}

void Texture2D_unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32 Texture2D_get_id(Texture2D _tex)
{
    return _tex->id;
}
