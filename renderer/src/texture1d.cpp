#include "pch.h"
#include "texture1d.h"
///#include <GL/gl.h>
#include "gl_utility.h"
#include "emem.h"
typedef struct _texture1d
{
    euint32 id;
    pixel_format format;
    euint32 width;
    euint32 have_image;
    vptr pxl_buffer;
} texture1d;

Texture1D Texture1D_new(void)
{
    Texture1D ret = (Texture1D)SMalloc( sizeof(texture1d) );

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
#if defined(GL_RGBA32F)
    case RGBA32F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, _tex->width,
                     0, GL_RGBA, GL_FLOAT,
                     _tex->pxl_buffer);
#elif defined(GL_RGBA32F_ARB)
    case RGBA32F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB, _tex->width,
                     0, GL_RGBA, GL_FLOAT,
                     _tex->pxl_buffer);

#endif
        break;
    case RG32F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, _tex->width,
                     0, GL_RG, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
    case RG16:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RG16, _tex->width,
                     0, GL_RG, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
#if defined(GL_RGB32F)
    case RGB32F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, _tex->width,
                     0, GL_RGB, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
#elif defined(GL_RGB32F_ARB)
    case RGB32F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F_ARB, _tex->width,
                     0, GL_RGB, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
#endif
#if defined(GL_RGBA16F)
    case RGBA16F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16F, _tex->width,
                     0, GL_RGBA, GL_HALF_FLOAT,
                     _tex->pxl_buffer);
        break;
#elif defined(GL_RGBA16F_ARB)
    case RGBA16F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16F_ARB, _tex->width,
                     0, GL_RGBA, GL_HALF_FLOAT,
                     _tex->pxl_buffer);
        break;
#endif
#if defined(GL_RGB16F)
    case RGB16F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB16F, _tex->width,
                     0, GL_RGB, GL_HALF_FLOAT,
                     _tex->pxl_buffer);
        break;
#elif defined(GL_RGB16F_ARB)
    case RGB16F:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB16F_ARB, _tex->width,
                     0, GL_RGB, GL_HALF_FLOAT,
                     _tex->pxl_buffer);
        break;
#endif
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
    case RGB16:
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB16, _tex->width,
                     0, GL_RGB, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    }
}

void Texture1D_create(Texture1D _tex, pixel_format _fmt, euint32 _w)
{
    _tex->format = _fmt;
    _tex->width = _w;
    _tex->pxl_buffer = NULL;
    Texture1D_updata(_tex);
}

void Texture1D_load_from_mem(Texture1D _tex, vptr _mem, pixel_format _fmt, euint32 _w)
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

euint32 Texture1D_get_id(Texture1D _tex)
{
    return _tex->id;
}
