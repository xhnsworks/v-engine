#include "font_renderer.h"
#include "render_system.h"
#include "texture2d.h"
#include <math.h>
#include "emem.h"
#include "eassert.h"
#include "clamp.h"

/****************/
/// fixed me! this function is unsafed
#ifdef _WIN32
esint _MByteToWChar(const char* _src_str, wchar_t* _dst_str)
{
	// Get the required size of the buffer that receives the Unicode
	// string.
	euint32 _size;
	_size = MultiByteToWideChar (0, 0, _src_str, -1, NULL, 0);

	// Convert headers from ASCII to Unicode.
	return MultiByteToWideChar (0, 0, _src_str, -1, (WCHAR*)_dst_str, _size);
}
#else
euint32 _MByteToWChar(const char* _src_str, wchar_t* _dst_str)
{
	euint32 _count = 0;
	while (_src_str[_count])
	{
		_count++;
	}
	euint32 i = 0;
	for (;i < _count; i++)
	{
		_dst_str[i] = (wchar_t)_src_str[i];
	}
	_dst_str[_count + 1] = 0x0000;

	return _count;
}
#endif
/****************/

ComposingStick::ComposingStick(FontRenderer* renderer, euint32 numChars)
: m_renderer(renderer)
{
	int numCharsPerRow = (int)sqrtf((float)numChars + 0.5f);
	euint32 fontPixelWidth = renderer->get_font_size();
	euint32 fontPixelHeight = fontPixelWidth;
    euint32 texPixelWidth = (euint32)(numCharsPerRow + 1) * fontPixelWidth;
	euint32 texPixelHeight = texPixelWidth;

	euint32 numHoris = numCharsPerRow;
	euint32 numVerts = numCharsPerRow;
	for (euint32 y = 0; y < numVerts; y++)
	{
		for (euint32 x = 0; x < numHoris; x++)
		{
			GlyphInfo* info = ENEW GlyphInfo();
			info->x = x * fontPixelWidth;
			info->y = y * fontPixelHeight;
			info->width = fontPixelWidth;
			info->height = fontPixelHeight;
			info->usageCount = 0;
			m_freeGlyphPool.push_back(info);
		}
	}
	/// here create texture
	m_texture = RenderSystem_new_texture2d("ComposingStick.png", "Texture");
	m_texture->Create(RGBA8, texPixelWidth, texPixelHeight);
	EColor color;
	color.red = 0.0f;
	color.green = 0.0f;
	color.blue = 0.0f;
	color.alpha = 0.0f;
	m_texture->LoadFromColor(color);
}
xhn::static_string ComposingStick::GetFilename()
{
    return "ComposingStick.png";
}
ComposingStick::GlyphHandle ComposingStick::AllocGlyph(wchar_t ch)
{
	ComposingStick::GlyphHandle ret;
	ret.owner = this;
	xhn::map<wchar_t, GlyphInfo*>::iterator iter = m_glyphIndex.find(ch);
	if (iter != m_glyphIndex.end()) {
		ret.glyph = iter->second;
	}
	else {
		if (m_freeGlyphPool.empty())
			return ret;
		wchar_t wbuf[2];
		wbuf[0] = ch;
		wbuf[1] = 0;
        GlyphInfo* info = m_freeGlyphPool.front();
		m_freeGlyphPool.pop_front();
		Tex2DRect rect;
		rect.x = info->x;
		rect.y = info->y;
		rect.width = m_renderer->get_font_size();
		rect.height = m_renderer->get_font_size();
		Tex2DLockedRect* lock = m_texture->Lock(rect);
		euint32 letterWidth = m_renderer->draw_letter(ch, lock->GetAt(0, 0), lock->GetWidth());
		m_texture->Unlock(lock);
		m_glyphIndex.insert(xhn::make_pair(ch, info));
		info->letter = ch;
		ret.glyph = info;
		ret.letterWidth = letterWidth;
	}
	return ret;
}

void ComposingStick::IncreaseGlyph(wchar_t ch)
{
	xhn::map<wchar_t, GlyphInfo*>::iterator iter = m_glyphIndex.find(ch);
	if (iter != m_glyphIndex.end()) {
		ComposingStick::GlyphInfo* glyph = iter->second;
		glyph->usageCount++;
	}
}
void ComposingStick::DecreaseGlyph(wchar_t ch)
{
	xhn::map<wchar_t, GlyphInfo*>::iterator iter = m_glyphIndex.find(ch);
	if (iter != m_glyphIndex.end()) {
		ComposingStick::GlyphInfo* glyph = iter->second;
		glyph->usageCount--;
		if (!glyph->usageCount) {
			m_glyphIndex.erase(iter);
			m_freeGlyphPool.push_back(glyph);
		}
	}
}
xhn::wstring ComposingStick::Convert(const xhn::string& str)
{
	wchar_t wbuf[256];
	_MByteToWChar(str.c_str(), wbuf);
	return wbuf;
}

void FontRenderer::Init(const char* _font_name)
{
    m_char_ptr = 0;
    m_pixel_size = Pixel30;
    FT_Error error = FT_Init_FreeType( &m_ft_lib );
    error = FT_New_Face( m_ft_lib,
                         _font_name,
                         0,
                         &m_ft_face );

    error = FT_Set_Char_Size(
                m_ft_face,
                0,
                m_pixel_size * 64,
                300,
                300 );
    error = FT_Set_Pixel_Sizes(
                m_ft_face,
                0,
                (FT_UInt)m_pixel_size );
    error = FT_Select_Charmap(
                m_ft_face, /* 目标face对象 */
                FT_ENCODING_UNICODE ); /* 编码 */

}
void FontRenderer::Dest()
{
}

FontRenderer::FontRenderer(const char* _font_name) : m_font_name(_font_name), m_char_ptr(0), m_pixel_size(Pixel30)
{
    FT_Error error = FT_Init_FreeType( &m_ft_lib );
    error = FT_New_Face( m_ft_lib,
                         _font_name,
                         0,
                         &m_ft_face );

    error = FT_Set_Char_Size(
                m_ft_face,
                0,
                m_pixel_size * 64,
                300,
                300 );
    error = FT_Set_Pixel_Sizes(
                m_ft_face,
                0,
                (FT_UInt)m_pixel_size );
    error = FT_Select_Charmap(
                m_ft_face, /* 目标face对象 */
                FT_ENCODING_UNICODE ); /* 编码 */

    ///m_text_buf = mult_thread_mem_alloc( _32BIT_PIXEL_SIZE_ * m_pixel_size * m_pixel_size * NUM_OF_CHARS_PRE_ROW );
}

void FontRenderer::set_font_size(PixelSize _size)
{
    m_pixel_size = (euint32)_size;

    FT_Error error = FT_Set_Char_Size(
                m_ft_face,
                0,
                m_pixel_size * 64,
                300,
                300 );
    error = FT_Set_Pixel_Sizes(
                m_ft_face,
                (FT_UInt)0,
                (FT_UInt)m_pixel_size );
    error = (FT_UInt)FT_Select_Charmap(
                m_ft_face, /* 目标face对象 */
                FT_ENCODING_UNICODE ); /* 编码 */
}

FontRenderer::~FontRenderer()
{
}

euint32 FontRenderer::draw_text(FT_Bitmap* bitmap, vptr _target, euint32 _x, euint32 _y, euint32 _w)
{
    FT_Int i, j, p, q;
    FT_Int x_max = (FT_Int)m_char_ptr + bitmap->width;
    FT_Int y_max = bitmap->rows;

    esint _offset_y = m_pixel_size - bitmap->rows + 1;
	EAssert(_offset_y >= 0 && _offset_y < 65535, "offset out range");

    euint8 _gray;

    euint8* _c;
    ///uint8 _r, _g, _b, _a;
    for ( i = 0, p = 0; i < y_max; i++, p++ )
    {
        for ( j = (FT_Int)m_char_ptr, q = 0; j < x_max; j++, q++ )
        {
            _gray = bitmap->buffer[p * bitmap->width + q];
            if (_gray)
            {
                _c = (euint8*)( ( (i + _y + _offset_y) * _w + j + _x ) * _32BIT_PIXEL_SIZE_ + (ref_ptr)_target );
	
				_c[3] = _gray;
				_c[2] = 255;
				_c[1] = 255;
				_c[0] = 255;
            }
        }
    }
	return bitmap->width;
}

void FontRenderer::print(const wchar_t* _str, euint32 _num_chars, vptr _target, euint32 _x, euint32 _y, euint32 _w)
{
    FT_GlyphSlot slot = m_ft_face->glyph;
    FT_UInt glyph_index;
    FT_UInt error;

    euint32 n;
    m_char_ptr = 0;
    for ( n = 0; n < _num_chars; n++ )
    {
        glyph_index = FT_Get_Char_Index( m_ft_face, _str[n] );

        error = FT_Load_Glyph( m_ft_face, glyph_index, FT_LOAD_DEFAULT );

        if ( error )
        {
            continue;
        }

        /// 加粗函数 FT_Outline_Embolden( &m_ft_face->glyph->outline, 100 );

        error = FT_Render_Glyph( m_ft_face->glyph, FT_RENDER_MODE_NORMAL );

        if ( error )
        {
            continue;
        }

        draw_text( &slot->bitmap, _target, _x, _y, _w);
        if (0xff00 & _str[n])
        {
            m_char_ptr += m_pixel_size;
        }
        else
        {
            m_char_ptr += m_pixel_size/2;
        }
    }
}

euint32 FontRenderer::draw_letter(wchar_t _letter, vptr _target, euint32 _width)
{
	FT_GlyphSlot slot = m_ft_face->glyph;
	FT_UInt glyph_index;
	FT_UInt error;

	glyph_index = FT_Get_Char_Index( m_ft_face, _letter );

	error = FT_Load_Glyph( m_ft_face, glyph_index, FT_LOAD_DEFAULT );

	if ( error )
	{
		return 0;
	}

	/// 加粗函数 FT_Outline_Embolden( &m_ft_face->glyph->outline, 100 );

	error = FT_Render_Glyph( m_ft_face->glyph, FT_RENDER_MODE_NORMAL );

	if ( error )
	{
		return 0;
	}

	return draw_text( &slot->bitmap, _target, 0, 0, _width);
}

void FontRenderer::print(const char* _str, vptr _target, euint32 _x, euint32 _y, euint32 _w)
{
    wchar_t _buf[128];
    esint32 _count = _MByteToWChar(_str, _buf);
    print(_buf, _count, _target, _x, _y, _w);
}
