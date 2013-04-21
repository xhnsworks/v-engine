#ifndef TEXTURE2D_ARRAY_H
#define TEXTURE2D_ARRAY_H
#include "common.h"
#include "etypes.h"
struct _texture2d_array;
typedef struct texture2d_array_
{
    struct _texture2d_array* self;
} Texture2DArray;

API_EXPORT Texture2DArray Texture2DArray_new();
API_EXPORT void Texture2DArray_create(Texture2DArray _self, pixel_format _fmt, uint32 _w, uint32 _h, uint32 _c);
API_EXPORT void Texture2DArray_bind(Texture2DArray _self);
API_EXPORT void Texture2DArray_unbind();
API_EXPORT uint32 Texture2DArray_get_id(Texture2DArray _self);
API_EXPORT uint32 Texture2DArray_get_count(Texture2DArray _self);
API_EXPORT uint32 Texture2DArray_get_width(Texture2DArray _self);
API_EXPORT uint32 Texture2DArray_get_height(Texture2DArray _self);
#endif
