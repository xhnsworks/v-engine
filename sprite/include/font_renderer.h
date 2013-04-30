/*
* Copyright (c) 2010, �캣��
* All rights reserved.
*
* �ļ����ƣ�font_renderer.h
* �ļ���ʶ�������ù���ƻ���
* ժ    Ҫ��
*
* ��ǰ�汾��0.3
* ����汾��0.2
* ��    �ߣ��캣��
* ������ڣ�2010��9��9��
* �޸����ڣ�2013��3��27��
*/

#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H
///*************************************************************************************************************************///
///                                                     include begin                                                       ///
///*************************************************************************************************************************///
#include "common.h"
#include "etypes.h"
#include "emem.hpp"
#include "ft2build.h"
#include "texture2d.h"
#include "xhn_map.hpp"
#include "xhn_list.hpp"
#include "xhn_string.hpp"
#include "xhn_static_string.hpp"
#include "xhn_smart_ptr.hpp"
#include FT_FREETYPE_H
#include FT_OUTLINE_H
///*************************************************************************************************************************///
///                                                     include end                                                         ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class FontRenderer;
class ComposingStick : public MemObject
{
public:
	struct GlyphInfo : public MemObject
	{
		euint x;
		euint y;
		euint width;
		euint height;
		wchar_t letter;
		euint usageCount;
		GlyphInfo()
			: x(0)
			, y(0)
			, width(0)
			, height(0)
			, letter(0)
			, usageCount(0)
		{}
	};
	class GlyphHandle : public MemObject
	{
		friend class ComposingStick;
	private:
        GlyphInfo* glyph;
		ComposingStick* owner;
		euint letterWidth;
	public:
		GlyphHandle()
			: glyph(NULL)
			, owner(NULL)
			, letterWidth(0)
		{}
		GlyphHandle(const GlyphHandle& h)
			: glyph(h.glyph)
			, owner(h.owner)
			, letterWidth(h.letterWidth)
		{
		    GlyphHandle& n_h = (GlyphHandle&)h;
			if (n_h.glyph)
				n_h.glyph->usageCount++;
		}
		void operator= (GlyphHandle& h)
		{
			glyph = h.glyph;
			owner = h.owner;
			letterWidth = h.letterWidth;
			if (h.owner && h.glyph) {
				h.owner->IncreaseGlyph(h.glyph->letter);
			}
		}
		~GlyphHandle()
		{
			if (owner && glyph)
			    owner->DecreaseGlyph(glyph->letter);
		}
		inline void SetGlyphInfo(GlyphInfo* g)
		{
			if (owner && glyph)
				owner->DecreaseGlyph(glyph->letter);
		    if (owner && g)
				owner->IncreaseGlyph(g->letter);
			glyph = g;
		}
		inline const GlyphInfo* GetGlyph() const
		{
			return glyph;
		}
		inline euint GetWidth() const
		{
			return letterWidth;
		}
	};
public:
    Texture2DPtr m_texture;
	xhn::list<GlyphInfo*> m_freeGlyphPool;
	xhn::map<wchar_t, GlyphInfo*> m_glyphIndex;
	FontRenderer* m_renderer;
public:
	ComposingStick(FontRenderer* renderer, euint numChars);
	GlyphHandle AllocGlyph(wchar_t ch);
	bool HasGlyph(wchar_t ch);
	void IncreaseGlyph(wchar_t ch);
	void DecreaseGlyph(wchar_t ch);
	static xhn::wstring Convert(const xhn::string& str);
	xhn::static_string GetFilename();
};
class FontRenderer : public MemObject
{
public:
    void Init(const char* _font_name);
    void Dest();
    FontRenderer(const char* _font_name);
    ~FontRenderer();
    void set_font_size(PixelSize _size);
	inline euint get_font_size() {
		return m_pixel_size;
	}
    void print(const wchar_t* _str, euint _num_chars, vptr _target, euint _x, euint _y, euint _w);
    void print(const char* _str, vptr _target, euint _x, euint _y, euint _w);
	euint draw_letter(wchar_t _letter, vptr _target, euint _width);
private:
    euint draw_text(FT_Bitmap* bitmap, vptr _target, euint _x, euint _y, euint _w);
    const char* m_font_name;
    FT_Library  m_ft_lib;
    FT_Face     m_ft_face;
    euint m_char_ptr;
    euint m_pixel_size;
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
#endif
