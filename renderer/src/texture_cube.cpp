#include "pch.h"
#include "texture_cube.h"
#include "emem.h"
typedef struct _texture_cube
{
    volatile euint ref_count;
    uint32 id;
    pixel_format format;
    uint32 width;
    uint32 height;
    uint32 have_image;
    vptr pxl_buffer;
} texture_cube;

void _TextureCube_delete(TextureCube _self, const char* _file, euint _line)
{
    _self->ref_count--;
    if (!_self->ref_count)
    {
        printf("teture dest\n");
        glDeleteTextures(1, &_self->id);
        if (_self->pxl_buffer)
            Mfree(_self->pxl_buffer);
        Mfree(_self);
    }
}

TextureCube _TextureCube_new(const char* _file, euint _line)
{
    TextureCube ret = (TextureCube)SMalloc(sizeof(texture_cube));
    ret->ref_count = 1;
    ret->have_image = false;
    ret->pxl_buffer = NULL;

    glGenTextures(1, &ret->id);
    return ret;
}

void TextureCube_updata(TextureCube _self);
void TextureCube_create(TextureCube _self, pixel_format _fmt, uint32 _size)
{
    _self->format = _fmt;
    _self->width = _size;
    _self->height = _size;
    _self->pxl_buffer = NULL;
    TextureCube_updata(_self);
}

uint32 TextureCube_get_id(TextureCube _self)
{
    return _self->id;
}

void TextureCube_updata(TextureCube _self)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _self->id);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    ///glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    for (int face = 0; face < 6; face++)
    {
        switch (_self->format)
        {
        case RGBA8:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA8, _self->width,
                         _self->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         _self->pxl_buffer);
            break;
        case RGB8:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB8, _self->width,
                         _self->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         _self->pxl_buffer);
            break;
        case RGBA32F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA32F, _self->width,
                         _self->height, 0, GL_RGBA, GL_FLOAT,
                         _self->pxl_buffer);
            break;
        case RG32F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RG32F, _self->width,
                         _self->height, 0, GL_RG, GL_FLOAT,
                         _self->pxl_buffer);
            break;
        case RG16:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RG16, _self->width,
                         _self->height, 0, GL_RG, GL_UNSIGNED_BYTE,
                         _self->pxl_buffer);
            break;
        case RGB32F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB32F, _self->width,
                         _self->height, 0, GL_RGB, GL_FLOAT,
                         _self->pxl_buffer);
            break;
        case RGBA16F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA16F, _self->width,
                         _self->height, 0, GL_RGBA, GL_HALF_FLOAT,
                         _self->pxl_buffer);
            break;
        case RGB16F:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB16F, _self->width,
                         _self->height, 0, GL_RGB, GL_HALF_FLOAT,
                         _self->pxl_buffer);
            break;
        case DEPTH32F:
            glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT32F, _self->width,
                         _self->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                         _self->pxl_buffer);
            break;
        case ALPHA8:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_ALPHA, _self->width,
                         _self->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
                         _self->pxl_buffer);
            break;
        case RGBA16:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA16, _self->width,
                         _self->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         _self->pxl_buffer);
            break;
        case RGB16:
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB16, _self->width,
                         _self->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         _self->pxl_buffer);
            break;
        }
    }
}

void TextureCube_bind(TextureCube _self)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, _self->id);
}

void TextureCube_unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
