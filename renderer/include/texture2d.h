#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include "common.h"
#include "etypes.h"
#include "image.h"
#include "emem.hpp"
#include "xhn_smart_ptr.hpp"
#include "xhn_string.hpp"
#include "xhn_vector.hpp"
#include "texture_utility.h"
#include "xhn_void_vector.hpp"
#include "color.h"
class GetTex2dPixelSize
{
public:
    pixel_format m_pixelFormat;
public:
    GetTex2dPixelSize(pixel_format fmt)
		: m_pixelFormat(fmt)
	{}
	uint operator () () {
		return _get_pixel_size(m_pixelFormat);
	}
};
class PixelConvertProc
{
public:
	void operator() (vptr dst, vptr src) {}
};
struct Tex2DRect
{
	uint x;
	uint y;
	uint width;
	uint height;
    Tex2DRect()
		: x(0)
		, y(0)
		, width(32)
		, height(32)
	{}
};
class Tex2DLockedRect : public MemObject
{
	friend class Texture2D;
private:
	uint x;
	uint y;
	uint width;
	uint height;
	pixel_format format;
	xhn::void_vector pxl_buffer;
    Tex2DLockedRect(uint _x, uint _y, uint _width, uint _height, pixel_format _fmt);
	~Tex2DLockedRect();
public:
	vptr GetAt(uint _x, uint _y);
	inline uint GetWidth() const {
		return width;
	}
	inline uint GetHeight() const {
		return height;
	}
};
class Texture2D : public RefObject
{
public:
	uint32 id;
	pixel_format format;
	uint32 width;
	uint32 height;
	xhn::void_vector pxl_buffer;
	uint32 is_locked;
	static bool s_sub_update_enable;
public:
	Texture2D();
	~Texture2D();
	void Create(pixel_format _fmt, uint32 _w, uint32 _h);
	void LoadFromMem(vptr _mem, pixel_format _fmt, uint32 _w, uint32 _h, uint32 _size_in_byte, bool is_compressed);
	void LoadFromColor(const EColor& _color);
	void Bind();
	static void Unbind();
	void Update(bool is_compressed);
	void SubUpdate(Tex2DLockedRect* rect);
	inline uint32 GetID() const {
		return id;
	}
	inline uint GetWidth() const {
		return width;
	}
	inline uint GetHeight() const {
		return height;
	}
	Tex2DLockedRect* Lock(const Tex2DRect& rect);
	void Unlock(Tex2DLockedRect* rect);
};
typedef xhn::SmartPtr<Texture2D> Texture2DPtr;

#endif
