/*
* Copyright (c) 2010, 徐海宁
* All rights reserved.
*
* 文件名称：font_renderer.h
* 文件标识：见配置管理计划书
* 摘    要：
*
* 当前版本：0.2
* 替代版本：0.1
* 作    者：徐海宁
* 完成日期：2010年9月9日
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
