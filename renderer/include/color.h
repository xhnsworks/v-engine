#ifndef COLOR_H
#define COLOR_H
#include "common.h"
#include "etypes.h"
class EColor
{
public:
    float red;
	float green;
	float blue;
	float alpha;
	EColor()
		: red(1.0f)
		, green(1.0f)
		, blue(1.0f)
		, alpha(1.0f)
	{}
	EColor(const EColor& c)
		: red(c.red)
		, green(c.green)
		, blue(c.blue)
		, alpha(c.alpha)
	{}
	inline EColor& operator +=(const EColor& c) {
		red += c.red;
		green += c.green;
		blue += c.blue;
		alpha += c.alpha;
		return *this;
	}
	inline EColor operator *(const EColor& c) const {
		return EColor(red * c.red, green * c.green, blue * c.blue, alpha * c.alpha);
	}
	inline EColor operator *(float f) const {
		return EColor(red * f, green * f, blue * f, alpha * f);
	}
	inline EColor operator /(const EColor& c) const {
		return EColor(red / c.red, green / c.green, blue / c.blue, alpha / c.alpha);
	}
	inline EColor operator +(const EColor& c) const {
		return EColor(red + c.red, green + c.green, blue + c.blue, alpha + c.alpha);
	}
	inline EColor operator -(const EColor& c) const {
		return EColor(red - c.red, green - c.green, blue - c.blue, alpha - c.alpha);
	}
	void Convert(vptr _dst, pixel_format _fmt) const;
};
#endif