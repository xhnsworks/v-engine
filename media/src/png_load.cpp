#include <stdlib.h>
#include <stdio.h>
#include "png_load.h"
#include "array.h"
static FILE* g_file_ptr = NULL;
static void** g_buffer_array = NULL;

///#ifndef __APPLE__
#include "png.h"
void png_chunk_dest(vptr _p)
{
    printf("png_chunk_dest\n");
}
static void _error_proc(png_structp _png, png_const_charp _str)
{
}

static void _warning_proc(png_structp _png, png_const_charp _str)
{
}

static png_voidp _malloc_proc(png_structp _png, png_alloc_size_t _size)
{
    /**
    Begin
    vptr ret = RBAlloc(_size, png_chunk_dest);
    EndRetPtr(ret)
    **/
    vptr ret = SMalloc(_size);
    g_buffer_array = array_push(g_buffer_array, ret);
    return ret;
}

static void _free_proc(png_structp _png, png_voidp _ptr)
{
    ///Mfree(_ptr);
}

static void _read_stream(png_structp _png, png_bytep _bytep, png_size_t _size)
{
    if (!g_file_ptr)
    {
        return;
    }
    fread(_bytep, _size, 1, g_file_ptr);
}

Image png_load(FILE* file_stream)
{
	g_file_ptr = file_stream;
    g_buffer_array = array_new(void*, 100, NULL);
    euint32 width;
    euint32 height;
    esint32 bit_depth;
    esint32 color_type;
    esint32 interlace_type;
    esint32 compression_type;
    esint32 filter_type;

    png_structp png_ptr = png_create_read_struct_2
                  (PNG_LIBPNG_VER_STRING, (png_voidp)NULL,
                   _error_proc, _warning_proc, (png_voidp)NULL,
                   _malloc_proc, _free_proc);

    png_set_read_fn(png_ptr, (png_voidp)NULL, _read_stream);

    png_infop info_ptr = png_create_info_struct(png_ptr);

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, &compression_type, &filter_type);

    euint8** rows = (euint8**)png_get_rows(png_ptr, info_ptr);

    fclose(g_file_ptr);
    g_file_ptr = NULL;

    Image ret = {NULL};
    switch (bit_depth)
    {
    case 8:
        switch (color_type)
        {
        case PNG_COLOR_TYPE_RGB:
            {
                ret = Image_new(RGB8, width, height);
                for (euint32 y = 0; y < height; y++)
                {
                    euint8* row = rows[y];
                    for (euint32 x = 0; x < width; x++)
                    {
                        euint8* pxl = (euint8*)Image_get_pixel(ret, x, y);
                        pxl[0] = row[x * 3];
                        pxl[1] = row[x * 3 + 1];
                        pxl[2] = row[x * 3 + 2];
                    }
                }
            }
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            {
                ret = Image_new(RGBA8, width, height);
                for (euint32 y = 0; y < height; y++)
                {
                    euint8* row = rows[y];
                    for (euint32 x = 0; x < width; x++)
                    {
                        euint8* pxl = (euint8*)Image_get_pixel(ret, x, y);
                        pxl[0] = row[x * 4];
                        pxl[1] = row[x * 4 + 1];
                        pxl[2] = row[x * 4 + 2];
                        pxl[3] = row[x * 4 + 3];
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    ///RecycleBin_audit();
    for (euint i = 0; i < array_n(g_buffer_array); i++)
    {
        void* ptr = array_safe_get(g_buffer_array, i);
        Mfree(ptr);
    }
    array_delete(g_buffer_array);
    ///g_buffer_array = array_new(void*, 100, NULL);

    return ret;
}

Image png_load(const char* _file_name)
{
	FILE* fp = SafeFOpen(_file_name, "rb");
	return png_load(fp);
}
/**
#else
Image png_load(FILE* file_stream)
{
    Image ret;
    return ret;
}
Image png_load(const char* _file_name)
{
    Image ret;
    return ret;
}
#endif
**/