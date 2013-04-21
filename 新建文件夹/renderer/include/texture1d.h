#ifndef TEXTURE1D_H
#define TEXTURE1D_H
#include "common.h"
#include "etypes.h"
#include "image.h"
struct _texture1d;
typedef struct _texture1d* Texture1D;
extern Texture1D Texture1D_new(void);
extern void Texture1D_delete(Texture1D _tex);
extern void Texture1D_create(Texture1D _tex, pixel_format _fmt, uint32 _width);
extern void Texture1D_load_from_mem(Texture1D _tex, vptr _mem, pixel_format _fmt, uint32 _w);
extern void Texture1D_bind(Texture1D _tex);
extern void Texture1D_unbind(void);
extern uint32 Texture1D_get_id(Texture1D _tex);
#endif
