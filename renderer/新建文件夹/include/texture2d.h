#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include "common.h"
#include "etypes.h"
#include "image.h"
typedef void (*Texture2D_CreateProc)(Texture2D, pixel_format, uint32, uint32);
typedef void (*Texture2D_LoadFromMemProc)(Texture2D, vptr, pixel_format, uint32, uint32, uint32, bool);
typedef void (*Texture2D_BindProc)(Texture2D);
typedef void (*Texture2D_UnbindProc)(Texture2D _tex);
typedef void (*Texture2D_DestProc)(Texture2D);

API_EXPORT Texture2D _Texture2D_new(const char* _file, uint _line);
API_EXPORT void _Texture2D_delete(Texture2D _tex, const char* _file, uint _line);
#define Texture2D_new() _Texture2D_new(__FILE__, __LINE__)
#define Texture2D_delete(t) _Texture2D_delete(t, __FILE__, __LINE__)
API_EXPORT void Texture2D_create(Texture2D _tex, pixel_format _fmt, uint32 _width, uint32 _h);
API_EXPORT void Texture2D_load_from_mem(Texture2D _tex, vptr _mem, pixel_format _fmt, uint32 _w, uint32 _h, uint32 _size_in_byte, bool is_compressed);
API_EXPORT void Texture2D_bind(Texture2D _tex);
API_EXPORT void Texture2D_unbind(Texture2D _tex);
API_EXPORT uint32 Texture2D_get_id(Texture2D _tex);
API_EXPORT Texture2D Texture2D_clone(Texture2D _tex);
API_EXPORT uint Texture2D_get_width(Texture2D _tex);
API_EXPORT uint Texture2D_get_height(Texture2D _tex);
#endif
