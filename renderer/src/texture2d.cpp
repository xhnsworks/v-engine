#include "pch.h"
#include "texture2d.h"
#include "gl_utility.h"
#include "emem.h"
#include "clamp.h"

Tex2DLockedRect::Tex2DLockedRect(euint _x, euint _y, euint _width, euint _height, pixel_format _fmt)
: x(_x)
, y(_y)
, width(_width)
, height(_height)
, format(_fmt)
{
    pxl_buffer.convert<PixelConvertProc>(_get_pixel_size(_fmt));
	pxl_buffer.resize(_width * _height);
	euint32 pxl = 0xffffffff;
	pxl_buffer.flush(&pxl);
}
Tex2DLockedRect::~Tex2DLockedRect()
{
}
vptr Tex2DLockedRect::GetAt(euint _x, euint _y)
{
    _x = clamp<euint>(_x, 0, width - 1);
	_y = clamp<euint>(_y, 0, height - 1);
	char* ret = (char*)pxl_buffer.get();
	return (vptr)(ret + (height * _y + _x) * _get_pixel_size(format));
}

bool Texture2D::s_sub_update_enable = true;

Texture2D::Texture2D()
: format(RGBA8)
, width(0)
, height(0)
, is_locked(0)
{
    glGenTextures(1, &id);
}
Texture2D::~Texture2D()
{
	glDeleteTextures(1, &id);
}

void Texture2D::Create(pixel_format _fmt, euint _w, euint _h)
{
	format = _fmt;
	width = _w;
	height = _h;
	pxl_buffer.clear();
	Update(false);
}

void Texture2D::LoadFromMem(vptr _mem, pixel_format _fmt, euint _w, euint _h, euint _size_in_byte, bool is_compressed)
{
	format = _fmt;
	width = _w;
	height = _h;
	pxl_buffer.clear();
	pxl_buffer.convert<PixelConvertProc>(_get_pixel_size(_fmt));
	pxl_buffer.resize(_w * _h);
	memcpy(pxl_buffer.get(), _mem, _size_in_byte);
	Update(is_compressed);
}

void Texture2D::LoadFromColor(const EColor& _color)
{
    vptr pxl = SMalloc(_get_pixel_size(format));
	_color.Convert(pxl, format);
	pxl_buffer.clear();
	pxl_buffer.convert<PixelConvertProc>(_get_pixel_size(format));
	pxl_buffer.resize(width * height);
	pxl_buffer.flush(pxl);
	Mfree(pxl);
	Update(false);
}

void Texture2D::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SubUpdate(Tex2DLockedRect* rect)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, id);
	switch (format)
	{
	case RGBA8:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RGBA, GL_UNSIGNED_BYTE,
			rect->pxl_buffer.get());
		break;
	case RGB8:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RGB, GL_UNSIGNED_BYTE,
			rect->pxl_buffer.get());
		break;
	case RGBA32F:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RGBA, GL_FLOAT,
			rect->pxl_buffer.get());
		break;
	case RG32F:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RG, GL_FLOAT,
			rect->pxl_buffer.get());
		break;
	case RG16:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RG, GL_UNSIGNED_BYTE,
			rect->pxl_buffer.get());
		break;
	case RGB32F:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RGB, GL_FLOAT,
			rect->pxl_buffer.get());
		break;
	case RGBA16F:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RGBA, GL_HALF_FLOAT,
			rect->pxl_buffer.get());
		break;
	case RGB16F:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RGB, GL_HALF_FLOAT,
			rect->pxl_buffer.get());
		break;
	case DEPTH32F:
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_DEPTH_COMPONENT, GL_FLOAT,
			rect->pxl_buffer.get());
		break;
	case ALPHA8:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_ALPHA, GL_UNSIGNED_BYTE,
			rect->pxl_buffer.get());
		break;
	case RGBA16:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RGBA, GL_UNSIGNED_BYTE,
			rect->pxl_buffer.get());
		break;
	case RGB16:
		glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y,
			rect->width, rect->height, GL_RGB, GL_UNSIGNED_BYTE,
			rect->pxl_buffer.get());
		break;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Update(bool is_compressed)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	///glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	switch (format)
	{
	case RGBA8:
		if (is_compressed)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, width,
			height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			pxl_buffer.get());
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width,
			height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			pxl_buffer.get());
		break;
	case RGB8:
		if (is_compressed)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, width,
			height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			pxl_buffer.get());
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width,
		    height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			pxl_buffer.get());
		break;
#if defined(GL_RGBA32F)
	case RGBA32F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width,
			height, 0, GL_RGBA, GL_FLOAT,
			pxl_buffer.get());
		break;
#elif defined(GL_RGBA32F_ARB)
    case RGBA32F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width,
                     height, 0, GL_RGBA, GL_FLOAT,
                     pxl_buffer.get());
        break;
#endif
	case RG32F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width,
			height, 0, GL_RG, GL_FLOAT,
			pxl_buffer.get());
		break;
	case RG16:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, width,
			height, 0, GL_RG, GL_UNSIGNED_BYTE,
			pxl_buffer.get());
		break;
#if defined(GL_RGB32F)
	case RGB32F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width,
			height, 0, GL_RGB, GL_FLOAT,
			pxl_buffer.get());
		break;
#elif defined(GL_RGB32F_ARB)
    case RGB32F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F_ARB, width,
            height, 0, GL_RGB, GL_FLOAT,
            pxl_buffer.get());
        break;
#endif
#if defined(GL_RGBA16F)
	case RGBA16F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width,
			height, 0, GL_RGBA, GL_HALF_FLOAT,
			pxl_buffer.get());
		break;
#elif defined(GL_RGBA16F_ARB)
    case RGBA16F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width,
            height, 0, GL_RGBA, GL_HALF_FLOAT,
            pxl_buffer.get());
        break;
#endif
#if defined(GL_RGB16F)
	case RGB16F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width,
			height, 0, GL_RGB, GL_HALF_FLOAT,
			pxl_buffer.get());
		break;
#elif defined(GL_RGB16F_ARB)
    case RGB16F:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, width,
            height, 0, GL_RGB, GL_HALF_FLOAT,
            pxl_buffer.get());
        break;
#endif
	case DEPTH32F:
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width,
			height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
			pxl_buffer.get());
		break;
	case ALPHA8:
		if (is_compressed)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_ALPHA, width,
			height, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
			pxl_buffer.get());
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width,
			height, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
			pxl_buffer.get());
		break;
	case RGBA16:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width,
			height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			pxl_buffer.get());
		break;
	case RGB16:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, width,
			height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			pxl_buffer.get());
		break;
	}
}

Tex2DLockedRect* Texture2D::Lock(const Tex2DRect& rect)
{
    if (!is_locked)
	{
		is_locked = 1;
		if (!pxl_buffer.get()) {
			pxl_buffer.resize(width * height);
		}
        Tex2DLockedRect* ret = ENEW Tex2DLockedRect(rect.x, rect.y, rect.width, rect.height, format);
		char* dst = (char*)ret->pxl_buffer.get();
        char* src = (char*)pxl_buffer.get();
		for (euint y = 0; y < rect.height; y++)
		{
			char* dst_line = &dst[y * rect.width * _get_pixel_size(format)];
			char* src_line = &src[( (y + rect.y) * rect.width + rect.x ) * _get_pixel_size(format)];
			memcpy(dst_line, src_line, rect.width * _get_pixel_size(format));
		}
		return ret;
	}
	else
		return NULL;
}
void Texture2D::Unlock(Tex2DLockedRect* rect)
{
	if (is_locked)
	{
        is_locked = 0;

		unsigned char* src = (unsigned char*)rect->pxl_buffer.get();
		unsigned char* dst = (unsigned char*)pxl_buffer.get();
		for (euint y = 0; y < rect->height; y++)
		{
			unsigned char* dst_line = &dst[( (y + rect->y) * width + rect->x ) * _get_pixel_size(format)];
			unsigned char* src_line = &src[y * rect->width * _get_pixel_size(format)];
			memcpy(dst_line, src_line, rect->width * _get_pixel_size(format));
		}
        if (s_sub_update_enable)
		{
			SubUpdate(rect);
		}
		else
		{
			Update(false);
		}

		delete rect;
	}
}