#include "pch.h"
#include "texture2d_array.h"
///#include <GL/gl.h>
#include "emem.h"
void create_tex_array(uint* _id)
{
    glGenTextures(1, _id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, *_id);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 512, 512, 4, 0, GL_RGB,  GL_UNSIGNED_BYTE, NULL);
    /**
    for(int j = 0; j < 4; j++)
    {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,  0,0,j, 512, 512,1, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    **/
}

/**
#extension GL_EXT_gpu_shader4 : enable
uniform sampler2DArray texarray;

    vec4 texCoord = vec4(gl_TexCoord[0].xy, floor(gl_TexCoord[0].z),gl_TexCoord[0].w);
    vec4 color1 = texture2DArray(texarray, texCoord.xyz);
**/

typedef struct _texture2d_array
{
    uint32 id;
    pixel_format format;
    uint32 width;
    uint32 height;
    uint32 count;
    uint32 have_image;
    vptr pxl_buffer;
} texture2d_array;

Texture2DArray Texture2DArray_new()
{
    Texture2DArray ret;
    ret.self = (texture2d_array*)SMalloc(sizeof(texture2d_array));
    glGenTextures(1, &ret.self->id);
    return ret;
}

void Texture2DArray_updata(Texture2DArray _self)
{
    if (_self.self->have_image)
    {
        glDeleteTextures(1, &_self.self->id);
    }
    _self.self->have_image = 1;
    glBindTexture(GL_TEXTURE_2D_ARRAY, _self.self->id);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);

    switch (_self.self->format)
    {
    case RGBA8:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, _self.self->width,
                     _self.self->height, _self.self->count, GL_RGBA, GL_UNSIGNED_BYTE,
                     _self.self->pxl_buffer);
        break;
    case RGB8:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB8, _self.self->width,
                     _self.self->height, _self.self->count, GL_RGB, GL_UNSIGNED_BYTE,
                     _self.self->pxl_buffer);
        break;
    case RGBA32F:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32F, _self.self->width,
                     _self.self->height, _self.self->count, GL_RGBA, GL_FLOAT,
                     _self.self->pxl_buffer);
        break;
    case RG32F:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RG32F, _self.self->width,
                     _self.self->height, _self.self->count, GL_RG, GL_FLOAT,
                     _self.self->pxl_buffer);
        break;
    case RG16:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RG16, _self.self->width,
                     _self.self->height, _self.self->count, GL_RG, GL_UNSIGNED_BYTE,
                     _self.self->pxl_buffer);
        break;
    case RGB32F:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB32F, _self.self->width,
                     _self.self->height, _self.self->count, GL_RGB, GL_FLOAT,
                     _self.self->pxl_buffer);
        break;
    case RGBA16F:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA16F, _self.self->width,
                     _self.self->height, _self.self->count, GL_RGBA, GL_HALF_FLOAT,
                     _self.self->pxl_buffer);
        break;
    case RGB16F:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB16F, _self.self->width,
                     _self.self->height, _self.self->count, GL_RGB, GL_HALF_FLOAT,
                     _self.self->pxl_buffer);
        break;
    case DEPTH32F:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, _self.self->width,
                     _self.self->height, _self.self->count, GL_DEPTH_COMPONENT, GL_FLOAT,
                     _self.self->pxl_buffer);
        break;
    case ALPHA8:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_ALPHA, _self.self->width,
                     _self.self->height, _self.self->count, GL_ALPHA, GL_UNSIGNED_BYTE,
                     _self.self->pxl_buffer);
        break;
    case RGBA16:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA16, _self.self->width,
                     _self.self->height, _self.self->count, GL_RGBA, GL_UNSIGNED_BYTE,
                     _self.self->pxl_buffer);
        break;
    case RGB16:
        glTexImage2D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB16, _self.self->width,
                     _self.self->height, _self.self->count, GL_RGB, GL_UNSIGNED_BYTE,
                     _self.self->pxl_buffer);
        break;
    }
}

void Texture2DArray_create(Texture2DArray _self, pixel_format _fmt, uint32 _w, uint32 _h, uint32 _c)
{
    _self.self->format = _fmt;
    _self.self->width = _w;
    _self.self->height = _h;
    _self.self->count = _c;
    _self.self->pxl_buffer = NULL;
    Texture2DArray_updata(_self);
}

void Texture2DArray_bind(Texture2DArray _self)
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, _self.self->id);
}

void Texture2DArray_unbind()
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

uint32 Texture2DArray_get_id(Texture2DArray _self)
{
    return _self.self->id;
}

uint32 Texture2DArray_get_count(Texture2DArray _self)
{
    return _self.self->count;
}
uint32 Texture2DArray_get_width(Texture2DArray _self)
{
    return _self.self->width;
}
uint32 Texture2DArray_get_height(Texture2DArray _self)
{
    return _self.self->height;
}
