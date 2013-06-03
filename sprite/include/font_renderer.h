/*
* Copyright (c) 2010, –Ï∫£ƒ˛
* All rights reserved.
*
* Œƒº˛√˚≥∆£∫font_renderer.h
* Œƒº˛±Í ∂£∫º˚≈‰÷√π‹¿Ìº∆ªÆ È
* ’™    “™£∫
*
* µ±«∞∞Ê±æ£∫0.3
* ÃÊ¥˙∞Ê±æ£∫0.2
* ◊˜    ’ﬂ£∫–Ï∫£ƒ˛
* ÕÍ≥…»’∆⁄£∫2010ƒÍ9‘¬9»’
* –ﬁ∏ƒ»’∆⁄£∫2013ƒÍ3‘¬27»’
*/

#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H
///**********************************************************************///
///                           include begin                              ///
///**********************************************************************///
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
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class FontRenderer;
class ComposingStick : public MemObject
{
public:
	struct GlyphInfo : public MemObject
	{
		euint32 x;
		euint32 y;
		euint32 width;
		euint32 height;
		euint32 letterWidth;
		wchar_t letter;
		euint32 usageCount;
		GlyphInfo()
			: x(0)
			, y(0)
			, width(0)
			, height(0)
			, letterWidth(0)
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
	public:
		GlyphHandle()
			: glyph(NULL)
			, owner(NULL)
		{}
		GlyphHandle(const GlyphHandle& h)
			: glyph(h.glyph)
			, owner(h.owner)
		{
		    GlyphHandle& n_h = (GlyphHandle&)h;
			if (n_h.owner && n_h.glyph) {
				n_h.owner->IncreaseGlyph(n_h.glyph->letter);
			}
		}
		void operator= (GlyphHandle& h)
		{
			glyph = h.glyph;
			owner = h.owner;
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
		inline euint32 GetWidth() const
		{
			return glyph->letterWidth;
		}
		inline xhn::static_string GetFilename() const 
		{
			return owner->GetFilename();
		}
	};
public:
    Texture2DPtr m_texture;
	xhn::list<GlyphInfo*> m_freeGlyphPool;
	xhn::map<wchar_t, GlyphInfo*> m_glyphMap;
	FontRenderer* m_renderer;
	PixelSize m_pixelSize;
	xhn::static_string m_filename;
public:
	ComposingStick(FontRenderer* renderer, 
		           PixelSize size, 
				   euint32 numChars,
				   xhn::static_string filename);
	~ComposingStick();
	GlyphHandle AllocGlyph(wchar_t ch);
	bool HasGlyph(wchar_t ch);
	bool HasFreeGlyphs();
	void IncreaseGlyph(wchar_t ch);
	void DecreaseGlyph(wchar_t ch);
	static xhn::wstring Convert(const xhn::string& str);
	xhn::static_string GetFilename() const;
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class ComposingStickManager : MemObject
{
private:
    static ComposingStickManager* s_ComposingStickManager;
public:
	typedef xhn::map< PixelSize, xhn::list<ComposingStick*> > ComposingStickMap;
	FontRenderer* m_renderer;
	ComposingStickMap m_composingStickMap;
	int m_composingStickCount;
public:
    static void Init();
    static void Dest();
    static ComposingStickManager* Get();
	ComposingStickManager();
	~ComposingStickManager();
	ComposingStick::GlyphHandle AllocGlyph(wchar_t ch, PixelSize size);
	void IncreaseGlyph(wchar_t ch, PixelSize size);
	void DecreaseGlyph(wchar_t ch, PixelSize size);
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class FontRenderer : public MemObject
{
	friend class ComposingStickManager;
private:
	FontRenderer(const char* _font_name);
	~FontRenderer();
public:
    void Init(const char* _font_name);
    void Dest();
    void set_font_size(PixelSize _size);
	inline euint32 get_font_size() {
		return m_pixel_size;
	}
    void print(const wchar_t* _str, euint32 _num_chars, vptr _target, euint32 _x, euint32 _y, euint32 _w);
    void print(const char* _str, vptr _target, euint32 _x, euint32 _y, euint32 _w);
	euint32 draw_letter(wchar_t _letter, vptr _target, euint32 _width);
private:
    euint32 draw_text(FT_Bitmap* bitmap, vptr _target, euint32 _x, euint32 _y, euint32 _w);
    const char* m_font_name;
    FT_Library  m_ft_lib;
    FT_Face     m_ft_face;
    euint32 m_char_ptr;
    euint32 m_pixel_size;
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif
