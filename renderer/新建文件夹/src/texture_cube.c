#include "texture_cube.h"
#include <GL/glew.h>
#include "emem.h"
#include "texture2d_base.h"
/**
typedef void (*Texture2D_CreateProc)(Texture2D, pixel_format, uint32, uint32);
typedef void (*Texture2D_LoadFromMemProc)(Texture2D, vptr, pixel_format, uint32, uint32, uint32, bool);
typedef void (*Texture2D_BindProc)(Texture2D);
typedef void (*Texture2D_UnbindProc)(Texture2D _tex);
typedef void (*Texture2D_DestProc)(Texture2D);
**/

typedef struct _texture_cube
{
    texture2d base;
} texture_cube;

void _TextureCube_updata(TextureCube _self)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _self->base.id);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    for (int face = 0; face < 6; face++)
    {
        switch (_self->base.format)
        {
        case RGBA8:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA8, _self->base.width,
                         _self->base.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         _self->base.pxl_buffer);
            break;
        case RGB8:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB8, _self->base.width,
                         _self->base.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         _self->base.pxl_buffer);
            break;
        case RGBA32F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA32F, _self->base.width,
                         _self->base.height, 0, GL_RGBA, GL_FLOAT,
                         _self->base.pxl_buffer);
            break;
        case RG32F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RG32F, _self->base.width,
                         _self->base.height, 0, GL_RG, GL_FLOAT,
                         _self->base.pxl_buffer);
        case RGB32F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB32F, _self->base.width,
                         _self->base.height, 0, GL_RGB, GL_FLOAT,
                         _self->base.pxl_buffer);
            break;
        case RGBA16F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA16F, _self->base.width,
                         _self->base.height, 0, GL_RGBA, GL_HALF_FLOAT,
                         _self->base.pxl_buffer);
            break;
        case RGB16F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB16F, _self->base.width,
                         _self->base.height, 0, GL_RGB, GL_HALF_FLOAT,
                         _self->base.pxl_buffer);
            break;
        case DEPTH32F:
            glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT32F, _self->base.width,
                         _self->base.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                         _self->base.pxl_buffer);
            break;
        case ALPHA8:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_ALPHA, _self->base.width,
                         _self->base.height, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
                         _self->base.pxl_buffer);
            break;
        case RGBA16:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA16, _self->base.width,
                         _self->base.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         _self->base.pxl_buffer);
            break;
        case RGB16:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB16, _self->base.width,
                         _self->base.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         _self->base.pxl_buffer);
            break;
        }
    }
}

void _TextureCube_create(TextureCube _self, pixel_format _fmt, uint32 _size, uint _h)
{
    _self->base.format = _fmt;
    _self->base.width = _size;
    _self->base.height = _size;
    _self->base.pxl_buffer = NULL;
    _TextureCube_updata(_self);
}

void _TextureCube_load_from_mem(TextureCube _self, vptr _mem, pixel_format _fmt, uint32 _w, uint32 _h, uint32 _size_in_byte, bool is_compressed)
{
}

void _TextureCube_bind(TextureCube _self)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, _self->base.id);
}

void _TextureCube_unbind(TextureCube _self)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void _TextureCube_Dest(TextureCube _self)
{
    printf("texture dest\n");
    glDeleteTextures(1, &_self->base.id);
    Mfree(_self);
}
///==============================================================///
void _TextureCube_delete(TextureCube _self, const char* _file, uint _line)
{
    _self->base.ref_count--;
    if (!_self->base.ref_count)
    {
        _self->base.dest_proc(_self);
    }
}

TextureCube _TextureCube_new(const char* _file, uint _line)
{
    TextureCube ret = Malloc(sizeof(texture_cube));
    ret->base.ref_count = 1;
    ret->base.have_image = false;
    ret->base.pxl_buffer = NULL;

    glGenTextures(1, &ret->base.id);

    ret->base.create_proc = _TextureCube_create;
    ret->base.load_from_mem_proc = _TextureCube_load_from_mem;
    ret->base.bind_proc = _TextureCube_bind;
    ret->base.unbind_proc = _TextureCube_unbind;
    ret->base.dest_proc = _TextureCube_Dest;

    return ret;
}
