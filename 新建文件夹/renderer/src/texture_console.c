#include "texture_console.h"
#include "texture2d.h"
#include "image.h"
#include "png_load.h"
#include "recycle_bin.h"
Texture2D create_texture_from_file(const char* _file_name)
{
    Image img = png_load(_file_name);

    if (to_ptr(img))
    {
        uint32 h = Image_get_num_rows(img);
        uint32 w = Image_get_row_width(img);
        pixel_format fmt = Image_get_pixel_format(img);
        Texture2D tex = Texture2D_new();
        vptr pxls = Image_get_row(img, 0);
        Texture2D_load_from_mem(tex, pxls, fmt, w, h, Image_get_size_in_byte(img));

        Image_delete(img);

        return tex;
    }

    return NULL;
}
