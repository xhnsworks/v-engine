#ifndef IMAGE_H
#define IMAGE_H
#include "common.h"
#include "etypes.h"

struct _image;
typedef struct image_
{
    struct _image* img;
} Image;

API_EXPORT Image Image_new(pixel_format _fmt, uint32 _row_width, uint32 _num_rows);
API_EXPORT void Image_delete(Image _img);
API_EXPORT vptr Image_get_row(Image _img, uint32 _i);
API_EXPORT vptr Image_get_pixel(Image _img, uint32 _x, uint32 _y);
API_EXPORT uint32 Image_get_pixel_size(Image _img);
API_EXPORT uint32 Image_get_num_rows(Image _img);
API_EXPORT uint32 Image_get_row_width(Image _img);
API_EXPORT pixel_format Image_get_pixel_format(Image _img);
API_EXPORT uint32 Image_get_size_in_byte(Image _img);
#endif
