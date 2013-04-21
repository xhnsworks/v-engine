#include "texture2d.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "gl_utility.h"
#include "emem.h"
#include "texture2d_base.h"

void _Texture2D_Dest(Texture2D _tex)
{
    printf("teture dest\n");
    glDeleteTextures(1, &_tex->id);
    if (_tex->pxl_buffer)
        Mfree(_tex->pxl_buffer);
    Mfree(_tex);
}

void _Texture2D_updata(Texture2D _tex, bool is_compressed)
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
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    switch (_tex->format)
    {
    case RGBA8:
    if (is_compressed)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, _tex->width,
                     _tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _tex->width,
                     _tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    case RGB8:
    if (is_compressed)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, _tex->width,
                     _tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _tex->width,
                     _tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    case RGBA32F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _tex->width,
                     _tex->height, 0, GL_RGBA, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
    case RG32F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, _tex->width,
                     _tex->height, 0, GL_RG, GL_FLOAT,
                     _tex->pxl_buffer);
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
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _tex->width,
                     _tex->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     _tex->pxl_buffer);
        break;
    case ALPHA8:
        if (is_compressed)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_ALPHA, _tex->width,
                         _tex->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
                         _tex->pxl_buffer);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, _tex->width,
                     _tex->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    case RGBA16:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, _tex->width,
                     _tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    case RGB16:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, _tex->width,
                     _tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     _tex->pxl_buffer);
        break;
    }
}

void _Texture2D_create(Texture2D _tex, pixel_format _fmt, uint32 _w, uint32 _h)
{
    _tex->format = _fmt;
    _tex->width = _w;
    _tex->height = _h;
    _tex->pxl_buffer = NULL;
    _Texture2D_updata(_tex, false);
}

void _Texture2D_load_from_mem(Texture2D _tex, vptr _mem, pixel_format _fmt, uint32 _w, uint32 _h, uint32 _size_in_byte, bool is_compressed)
{
    _tex->format = _fmt;
    _tex->width = _w;
    _tex->height = _h;
    _tex->pxl_buffer = Malloc(_size_in_byte);
    memcpy(_tex->pxl_buffer, _mem, _size_in_byte);
    _Texture2D_updata(_tex, is_compressed);
}

void _Texture2D_bind(Texture2D _tex)
{
    glBindTexture(GL_TEXTURE_2D, _tex->id);
}

void _Texture2D_unbind(Texture2D _tex)
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void _Texture2D_delete(Texture2D _tex, const char* _file, uint _line)
{
    _tex->ref_count--;
    if (!_tex->ref_count)
    {
        _tex->dest_proc(_tex);
    }
}
Texture2D _Texture2D_new(const char* _file, uint _line)
{
    Texture2D ret = _Malloc( sizeof(texture2d), _file, _line );
    ret->ref_count = 1;

    glGenTextures(1, &ret->id);
    ret->dest_proc = _Texture2D_Dest;
    ret->create_proc = _Texture2D_create;
    ret->load_from_mem_proc = _Texture2D_load_from_mem;
    ret->bind_proc = _Texture2D_bind;
    ret->unbind_proc = _Texture2D_unbind;

    return ret;
}
///==============================================================///
uint32 Texture2D_get_id(Texture2D _tex)
{
    return _tex->id;
}

Texture2D Texture2D_clone(Texture2D _tex)
{
    if (!_tex)
        return NULL;
    _tex->ref_count++;
    return _tex;
}

uint Texture2D_get_width(Texture2D _tex)
{
    return _tex->width;
}
uint Texture2D_get_height(Texture2D _tex)
{
    return _tex->height;
}
void Texture2D_bind(Texture2D _tex)
{
    _tex->bind_proc(_tex);
}
void Texture2D_unbind(Texture2D _tex)
{
    _tex->unbind_proc(_tex);
}
void Texture2D_load_from_mem(Texture2D _tex, vptr _mem, pixel_format _fmt, uint32 _w, uint32 _h, uint32 _size_in_byte, bool is_compressed)
{
    _tex->load_from_mem_proc(_tex, _mem, _fmt, _w, _h, _size_in_byte, is_compressed);
}
void Texture2D_create(Texture2D _tex, pixel_format _fmt, uint32 _width, uint32 _h)
{
    _tex->create_proc(_tex, _fmt, _width, _h);
}
