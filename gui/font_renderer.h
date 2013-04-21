/*
* Copyright (c) 2010, �캣��
* All rights reserved.
*
* �ļ����ƣ�font_renderer.h
* �ļ���ʶ�������ù���ƻ���
* ժ    Ҫ��
*
* ��ǰ�汾��0.2
* ����汾��0.1
* ��    �ߣ��캣��
* ������ڣ�2010��9��9��
*/

#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H
///*************************************************************************************************************************///
///                                                     include begin                                                       ///
///*************************************************************************************************************************///
#include "types.h"
#include "ft2build.h"
#include "class_typedef.h"
#include FT_FREETYPE_H
#include FT_OUTLINE_H
///*************************************************************************************************************************///
///                                                     include end                                                         ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class FontRenderer
{
public:
    void init(const char* _font_name);
    void dest();
    FontRenderer(const char* _font_name);
    ~FontRenderer();
    void set_font_size(FontSize _size);
    void print(const wchar_t* _str, uint _num_chars, vptr _target, uint _x, uint _y, uint _w);
    void print(const char* _str, vptr _target, uint _x, uint _y, uint _w);
private:
    void draw_text(FT_Bitmap* bitmap, vptr _target, uint _x, uint _y, uint _w);
    const char* m_font_name;
    FT_Library  m_ft_lib;
    FT_Face     m_ft_face;
    uint m_char_ptr;
    uint m_pixel_size;
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
#endif
