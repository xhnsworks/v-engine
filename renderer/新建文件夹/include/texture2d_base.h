#ifndef TEXTURE2D_BASE_H
#define TEXTURE2D_BASE_H
#include "common.h"
#include "etypes.h"
#include "texture2d.h"
typedef struct _texture2d
{
    uint ref_count;
    uint32 id;
    pixel_format format;
    uint32 width;
    uint32 height;
    uint32 have_image;
    vptr pxl_buffer;
    Texture2D_CreateProc create_proc;
    Texture2D_LoadFromMemProc load_from_mem_proc;
    Texture2D_BindProc bind_proc;
    Texture2D_UnbindProc unbind_proc;
    Texture2D_DestProc dest_proc;
} texture2d;

#endif // TEXTURE2D_BASE_H
