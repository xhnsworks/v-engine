#ifndef ECG_2_PUGI_H
#define ECG_2_PUGI_H
#include "xhn_string.hpp"
#include "float_base.h"
#include "attribute.h"
#include "color.h"
#include "pugixml.hpp"
inline void _SaveColor(const EColor& from, pugi::xml_node& to, const xhn::string& name)
{
	pugi::xml_attribute r = to.append_attribute( (name + "_r").c_str() );
	pugi::xml_attribute g = to.append_attribute( (name + "_g").c_str() );
	pugi::xml_attribute b = to.append_attribute( (name + "_b").c_str() );
	pugi::xml_attribute a = to.append_attribute( (name + "_a").c_str() );
	r.set_value(from.red);
	g.set_value(from.green);
	b.set_value(from.blue);
	a.set_value(from.alpha);
}

inline void _LoadColor(EColor& to, const pugi::xml_node& from, const xhn::string& name)
{
	pugi::xml_attribute r = from.attribute( (name + "_r").c_str() );
	pugi::xml_attribute g = from.attribute( (name + "_g").c_str() );
	pugi::xml_attribute b = from.attribute( (name + "_b").c_str() );
	pugi::xml_attribute a = from.attribute( (name + "_a").c_str() );
	to.red = r.as_float();
	to.green = g.as_float();
	to.blue = b.as_float();
	to.alpha = a.as_float();
}

inline void _SaveEFloat(const EFloat& from, pugi::xml_node& to, const xhn::string& name)
{
	pugi::xml_attribute x = to.append_attribute( (name + "_x").c_str() );
	x.set_value(from.x);
}

inline void _SaveEFloat(const FloatAttr& from, pugi::xml_node& to, const xhn::string& name)
{
	pugi::xml_attribute x = to.append_attribute( (name + "_x").c_str() );
	x.set_value(from.x);
}

inline void _LoadEFloat(EFloat& to, const pugi::xml_node& from, const xhn::string& name)
{
	pugi::xml_attribute x = from.attribute( (name + "_x").c_str() );
	to.x = x.as_float();
}

inline void _LoadEFloat(FloatAttr& to, const pugi::xml_node& from, const xhn::string& name)
{
	pugi::xml_attribute x = from.attribute( (name + "_x").c_str() );
	to.x = x.as_float();
}

inline void _SaveEFloat2(const EFloat2& from, pugi::xml_node& to, const xhn::string& name)
{
	pugi::xml_attribute x = to.append_attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = to.append_attribute( (name + "_y").c_str() );
	x.set_value(from.x);
	y.set_value(from.y);
}

inline void _LoadEFloat2(EFloat2& to, const pugi::xml_node& from, const xhn::string& name)
{
	pugi::xml_attribute x = from.attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = from.attribute( (name + "_y").c_str() );
	to.x = x.as_float();
	to.y = y.as_float();
}

inline void _LoadEFloat2(Float2Attr& to, const pugi::xml_node& from, const xhn::string& name)
{
	pugi::xml_attribute x = from.attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = from.attribute( (name + "_y").c_str() );
	to.x = x.as_float();
	to.y = y.as_float();
}

inline void _SaveEFloat3(const EFloat3& from, pugi::xml_node& to, const xhn::string& name)
{
	pugi::xml_attribute x = to.append_attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = to.append_attribute( (name + "_y").c_str() );
	pugi::xml_attribute z = to.append_attribute( (name + "_z").c_str() );
	x.set_value(from.x);
	y.set_value(from.y);
	z.set_value(from.z);
}

inline void _SaveEFloat3(const Float3Attr& from, pugi::xml_node& to, const xhn::string& name)
{
	pugi::xml_attribute x = to.append_attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = to.append_attribute( (name + "_y").c_str() );
	pugi::xml_attribute z = to.append_attribute( (name + "_z").c_str() );
	x.set_value(from.x);
	y.set_value(from.y);
	z.set_value(from.z);
}

inline void _LoadEFloat3(EFloat3& to, const pugi::xml_node& from, const xhn::string& name)
{
	pugi::xml_attribute x = from.attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = from.attribute( (name + "_y").c_str() );
	pugi::xml_attribute z = from.attribute( (name + "_z").c_str() );
	to.x = x.as_float();
	to.y = y.as_float();
	to.z = z.as_float();
}

inline void _LoadEFloat3(Float3Attr& to, const pugi::xml_node& from, const xhn::string& name)
{
	pugi::xml_attribute x = from.attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = from.attribute( (name + "_y").c_str() );
	pugi::xml_attribute z = from.attribute( (name + "_z").c_str() );
	to.x = x.as_float();
	to.y = y.as_float();
	to.z = z.as_float();
}

inline void _SaveEFloat3_2(const EFloat3& from, pugi::xml_node& to)
{
	pugi::xml_attribute x = to.append_attribute( "x" );
	pugi::xml_attribute y = to.append_attribute( "y" );
	pugi::xml_attribute z = to.append_attribute( "z" );
	x.set_value(from.x);
	y.set_value(from.y);
	z.set_value(from.z);
}

inline void _LoadEFloat3_2(EFloat3& to, const pugi::xml_node& from)
{
	pugi::xml_attribute x = from.attribute( "x" );
	pugi::xml_attribute y = from.attribute( "y" );
	pugi::xml_attribute z = from.attribute( "z" );
	to.x = x.as_float();
	to.y = y.as_float();
	to.z = z.as_float();
}

inline void _SaveEFloat4(const EFloat4& from, pugi::xml_node& to, const xhn::string& name)
{
	pugi::xml_attribute x = to.append_attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = to.append_attribute( (name + "_y").c_str() );
	pugi::xml_attribute z = to.append_attribute( (name + "_z").c_str() );
	pugi::xml_attribute w = to.append_attribute( (name + "_w").c_str() );
	x.set_value(from.x);
	y.set_value(from.y);
	z.set_value(from.z);
	w.set_value(from.w);
}

inline void _SaveEFloat4(const Float4Attr& from, pugi::xml_node& to, const xhn::string& name)
{
	pugi::xml_attribute x = to.append_attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = to.append_attribute( (name + "_y").c_str() );
	pugi::xml_attribute z = to.append_attribute( (name + "_z").c_str() );
	pugi::xml_attribute w = to.append_attribute( (name + "_w").c_str() );
	x.set_value(from.x);
	y.set_value(from.y);
	z.set_value(from.z);
	w.set_value(from.w);
}

inline void _LoadEFloat4(Float4Attr& to, const pugi::xml_node& from, const xhn::string& name)
{
	pugi::xml_attribute x = from.attribute( (name + "_x").c_str() );
	pugi::xml_attribute y = from.attribute( (name + "_y").c_str() );
	pugi::xml_attribute z = from.attribute( (name + "_z").c_str() );
	pugi::xml_attribute w = from.attribute( (name + "_w").c_str() );
	to.x = x.as_float();
	to.y = y.as_float();
	to.z = z.as_float();
	to.w = w.as_float();
}

inline void _SaveEFloat4_2(const EFloat4& from, pugi::xml_node& to)
{
	pugi::xml_attribute x = to.append_attribute( "x" );
	pugi::xml_attribute y = to.append_attribute( "y" );
	pugi::xml_attribute z = to.append_attribute( "z" );
	pugi::xml_attribute w = to.append_attribute( "w" );
	x.set_value(from.x);
	y.set_value(from.y);
	z.set_value(from.z);
	w.set_value(from.w);
}

inline void _LoadEFloat4_2(EFloat4& to, const pugi::xml_node& from)
{
	pugi::xml_attribute x = from.attribute( "x" );
	pugi::xml_attribute y = from.attribute( "y" );
	pugi::xml_attribute z = from.attribute( "z" );
	pugi::xml_attribute w = from.attribute( "w" );
	to.x = x.as_float();
	to.y = y.as_float();
	to.z = z.as_float();
	to.w = w.as_float();
}

inline void _LoadAttributeType(Attribute::Type& to, const pugi::xml_node& from, const xhn::string& name)
{
    pugi::xml_attribute type = from.attribute( name.c_str() );
	xhn::static_string typeStr(type.value());
	if (typeStr == xhn::static_string("Float"))
		to = Attribute::Float;
	else if (typeStr == xhn::static_string("Float2"))
		to = Attribute::Float2;
	else if (typeStr == xhn::static_string("Float3"))
		to = Attribute::Float3;
	else
		to = Attribute::Float4;
}

inline void _SaveAttributeType(const Attribute::Type& from, pugi::xml_node& to, const xhn::string& name)
{
	switch (from)
	{
	case Attribute::Float:
		to.append_attribute(name.c_str()).set_value("Float");
		break;
	case Attribute::Float2:
		to.append_attribute(name.c_str()).set_value("Float2");
		break;
	case Attribute::Float3:
		to.append_attribute(name.c_str()).set_value("Float3");
		break;
	case Attribute::Float4:
		to.append_attribute(name.c_str()).set_value("Float4");
		break;
	}
}
#endif