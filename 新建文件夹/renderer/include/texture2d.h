#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include "common.h"
#include "etypes.h"
#include "image.h"
struct _texture2d;
typedef struct _texture2d* Texture2D;
extern Texture2D _Texture2D_new(const char* _file, uint _line);
extern void _Texture2D_delete(Texture2D _tex, const char* _file, uint _line);
#define Texture2D_new() _Texture2D_new(__FILE__, __LINE__)
#define Texture2D_delete(t) _Texture2D_delete(t, __FILE__, __LINE__)
extern void Texture2D_create(Texture2D _tex, pixel_format _fmt, uint32 _width, uint32 _h);
extern void Texture2D_load_from_mem(Texture2D _tex, vptr _mem, pixel_format _fmt, uint32 _w, uint32 _h, uint32 _size_in_byte);
extern void Texture2D_bind(Texture2D _tex);
extern void Texture2D_unbind(void);
extern uint32 Texture2D_get_id(Texture2D _tex);
#endif
