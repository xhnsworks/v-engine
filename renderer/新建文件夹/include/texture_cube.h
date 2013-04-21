#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H
#include "common.h"
#include "etypes.h"
#include "texture2d.h"
typedef struct _texture_cube* TextureCube;

API_EXPORT TextureCube _TextureCube_new(const char* _file, uint _line);
API_EXPORT void _TextureCube_delete(TextureCube _self, const char* _file, uint _line);
#define TextureCube_new() _TextureCube_new(__FILE__, __LINE__)
#define TextureCube_delete(s) _TextureCube_delete(s, __FILE__, __LINE__)

#endif // TEXTURE_CUBE_H
