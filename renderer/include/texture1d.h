#ifndef TEXTURE1D_H
#define TEXTURE1D_H
#include "common.h"
#include "etypes.h"
#include "image.h"
struct _texture1d;
typedef struct _texture1d* Texture1D;
API_EXPORT Texture1D Texture1D_new(void);
API_EXPORT void Texture1D_delete(Texture1D _tex);
API_EXPORT void Texture1D_create(Texture1D _tex, pixel_format _fmt, euint32 _width);
API_EXPORT void Texture1D_load_from_mem(Texture1D _tex, vptr _mem, pixel_format _fmt, euint32 _w);
API_EXPORT void Texture1D_bind(Texture1D _tex);
API_EXPORT void Texture1D_unbind(void);
API_EXPORT euint32 Texture1D_get_id(Texture1D _tex);
#endif
