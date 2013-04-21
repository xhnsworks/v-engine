#include "texture1d.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_utility.h"
#include "mem.h"
typedef struct _texture1d
{
    uint32 id;
    pixel_format format;
    uint32 width;
    uint32 have_image;
    vptr pxl_buffer;
} texture1d;

Texture1D Texture1D_new(void)
{
    Texture1D ret = Malloc( sizeof(texture1d) );

    glGenTextures(1, &ret->id);
    return ret;
}
void Texture1D_delete(Texture1D _tex)
{
    glDeleteTextures(1, &_tex->id);
    if (_tex->pxl_buffer)
        Mfree(_tex->pxl_buffer);
}

void Texture1D_updata(Texture1D _tex)
{
    if (_tex->have_image)
    {
        glDeleteTextures(1, &_tex->id);
    }
    _tex->have_image = 1;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_1D, _tex->id);
    glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    switch (_tex->format)
    {
    case RGBA8:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, _tex->width,
                     0, GL_RGBA, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    case RGB8:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, _tex->width,
                     0, GL_RGB, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    case RGBA32F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, _tex->width,
                     0, GL_RGBA, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
    case RGB32F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, _tex->width,
                     0, GL_RGB, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
    case RGBA16F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16F, _tex->width,
                     0, GL_RGBA, GL_HALF_FLOAT,
                     _tex->pxl_buffer);
        break;
    case RGB16F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB16F, _tex->width,
                     0, GL_RGB, GL_HALF_FLOAT,
                     _tex->pxl_buffer);
        break;
    case DEPTH32F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_DEPTH_COMPONENT32F, _tex->width,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
    case ALPHA8:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_ALPHA, _tex->width,
                     0, GL_ALPHA, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    case RGBA16:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16, _tex->width,
                     0, GL_RGBA, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    }
}

void Texture1D_create(Texture1D _tex, pixel_format _fmt, uint32 _w)
{
    _tex->format = _fmt;
    _tex->width = _w;
    _tex->pxl_buffer = NULL;
    Texture1D_updata(_tex);
}

void Texture1D_load_from_mem(Texture1D _tex, vptr _mem, pixel_format _fmt, uint32 _w)
{
    _tex->format = _fmt;
    _tex->width = _w;
    _tex->pxl_buffer = _mem;
    Texture1D_updata(_tex);
}

void Texture1D_bind(Texture1D _tex)
{
    glBindTexture(GL_TEXTURE_1D, _tex->id);
}

void Texture1D_unbind()
{
    glBindTexture(GL_TEXTURE_1D, 0);
}

uint32 Texture1D_get_id(Texture1D _tex)
{
    return _tex->id;
}
