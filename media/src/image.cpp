#include "image.h"
#include "emem.h"
#include "texture_utility.h"
typedef struct _image
{
    pixel_format pxl_fmt;
    uint32 row_width;
    uint32 num_rows;
    uint32 size_in_byte;
    vptr pixels;
} image;

void Image_Dest(struct _image* _img)
{
    Mfree(_img->pixels);
}
Image Image_new(pixel_format _fmt, uint32 _row_width, uint32 _num_rows)
{
    uint32 pxl_size = _get_pixel_size(_fmt);
    if (pxl_size == UINT32_EXCE || !_row_width || !_num_rows)
    {
        Image ret = {NULL};
        return ret;
    }
    uint32 size_in_byte = pxl_size * _row_width * _num_rows;
    vptr pxls = SMalloc(size_in_byte);
    image* img = (image*)SMalloc(sizeof(image));
    img->pixels = pxls;
    img->pxl_fmt = _fmt;
    img->num_rows = _num_rows;
    img->row_width = _row_width;
    img->size_in_byte = size_in_byte;
    Image ret;
    ret.img = img;
    return ret;
}
void Image_delete(Image _img)
{
    Image_Dest(_img.img);
    Mfree(_img.img);
}

vptr Image_get_row(Image _img, uint32 _i)
{
    if (_i < _img.img->num_rows)
    {
        uint32 pxl_size = _get_pixel_size(_img.img->pxl_fmt);
        if (pxl_size != UINT32_EXCE)
            return (vptr)((ref_ptr)_img.img->pixels + (ref_ptr)(_img.img->row_width * _i * pxl_size));
        else
            return NULL;
    }
    else
        return NULL;
}

vptr Image_get_pixel(Image _img, uint32 _x, uint32 _y)
{
    vptr row = Image_get_row(_img, _y);
    if (row)
    {
        if (_x < _img.img->row_width)
            return (vptr)((ref_ptr)row + (ref_ptr)(_x * _get_pixel_size(_img.img->pxl_fmt)));
        else
            return NULL;
    }
    else
        return NULL;
}

uint32 Image_get_pixel_size(Image _img)
{
    return _get_pixel_size(_img.img->pxl_fmt);
}

uint32 Image_get_num_rows(Image _img)
{
    return _img.img->num_rows;
}

uint32 Image_get_row_width(Image _img)
{
    return _img.img->row_width;
}

pixel_format Image_get_pixel_format(Image _img)
{
    return _img.img->pxl_fmt;
}

uint32 Image_get_size_in_byte(Image _img)
{
    return _img.img->size_in_byte;
}
