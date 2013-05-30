#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include "emem.hpp"
#include "xhn_lock.hpp"
#include "float_base.h"
#include "rtti.hpp"
class Attribute : public MemObject
{
    DeclareRootRTTI;
public:
	enum Type
	{
		Float,
		Float2,
		Float3,
		Float4,
	};
	static component ComponentFormat(component comp);
	static euint GetNumComponents(Attribute* attr);
	static void Copy(Attribute* dst, Attribute* src, component_index bridge);
	static void GetBridge(Attribute* attr, component_index& bridge);
	static component_index GetReverseBridge(component_index bridge);
};
class FloatAttr : public Attribute
{
    DeclareRTTI;
public:
	float x;
public:
	FloatAttr()
    : x(0.0f)
	{}
	FloatAttr(float _x)
    : x(_x)
	{}
	inline FloatAttr operator *(const FloatAttr& f) const {
		return FloatAttr(x * f.x);
	}
	inline FloatAttr operator *(float f) const {
		return FloatAttr(x * f);
	}
	inline FloatAttr operator /(const FloatAttr& f) const {
		return FloatAttr(x / f.x);
	}
	inline FloatAttr operator +(const FloatAttr& f) const {
		return FloatAttr(x + f.x);
	}
	inline FloatAttr operator -(const FloatAttr& f) const {
		return FloatAttr(x - f.x);
	}
	inline float Get() const {
		return x;
	}
	static inline FloatAttr Lerp(const FloatAttr& a, const FloatAttr& b, float factor) {
		FloatAttr delt = b - a;
		FloatAttr ret = a + delt * factor;
		return ret;
	}
    inline void Export(float* mem) {
        mem[0] = x;
    }
    inline EFloat ToEFloat() {
        return EFloat(x);
    }
};
class Float2Attr : public FloatAttr
{
    DeclareRTTI;
public:
	float y;
public:
	Float2Attr()
    : y(0.0f)
	{}
	Float2Attr(float _x, float _y)
    : FloatAttr(_x)
    , y(_y)
	{}
	inline Float2Attr operator *(const Float2Attr& f) const {
		return Float2Attr(x * f.x, y * f.y);
	}
	inline Float2Attr operator *(float f) const {
		return Float2Attr(x * f, y * f);
	}
	inline Float2Attr operator /(const Float2Attr& f) const {
		return Float2Attr(x / f.x, y / f.y);
	}
	inline Float2Attr operator +(const Float2Attr& f) const {
		return Float2Attr(x + f.x, y + f.y);
	}
	inline Float2Attr operator -(const Float2Attr& f) const {
		return Float2Attr(x - f.x, y - f.y);
	}
	static inline Float2Attr Lerp(const Float2Attr& a, const Float2Attr& b, float factor) {
		Float2Attr delt = b - a;
		Float2Attr ret = a + delt * factor;
		return ret;
	}
    inline void Export(float* mem) {
        mem[0] = x;
        mem[1] = y;
    }
    inline EFloat2 ToEFloat2() {
        return EFloat2(x, y);
    }
};
class Float3Attr : public Float2Attr
{
    DeclareRTTI;
public:
	float z;
public:
	Float3Attr()
    : z(0.0f)
	{}
	Float3Attr(float _x, float _y, float _z)
    : Float2Attr(_x, _y)
    , z(_z)
	{}
	inline Float3Attr operator *(const Float3Attr& f) const {
		return Float3Attr(x * f.x, y * f.y, z * f.z);
	}
	inline Float3Attr operator *(float f) const {
		return Float3Attr(x * f, y * f, z * f);
	}
	inline Float3Attr operator /(const Float3Attr& f) const {
		return Float3Attr(x / f.x, y / f.y, z / f.z);
	}
	inline Float3Attr operator +(const Float3Attr& f) const {
		return Float3Attr(x + f.x, y + f.y, z + f.z);
	}
	inline Float3Attr operator -(const Float3Attr& f) const {
		return Float3Attr(x - f.x, y - f.y, z - f.z);
	}
	static inline Float3Attr Lerp(const Float3Attr& a, const Float3Attr& b, float factor) {
		Float3Attr delt = b - a;
		Float3Attr ret = a + delt * factor;
		return ret;
	}
    inline void Export(float* mem) {
        mem[0] = x;
        mem[1] = y;
        mem[2] = z;
    }
    inline EFloat3 ToEFloat3() {
        return EFloat3(x, y, z);
    }
};
class Float4Attr : public Float3Attr
{
    DeclareRTTI;
public:
    float w;
public:
	Float4Attr()
    : w(0.0f)
	{}
	Float4Attr(const Float3Attr& nor, float w)
    : Float3Attr(nor.x, nor.y, nor.z)
    , w(w)
	{}
	Float4Attr(float _x, float _y, float _z, float _w)
    : Float3Attr(_x, _y, _z)
    , w(_w)
	{}
	inline Float4Attr operator *(const Float4Attr& f) const {
        return Float4Attr(x * f.x, y * f.y, z * f.z, w * f.w);
	}
	inline Float4Attr operator *(float f) const {
		return Float4Attr(x * f, y * f, z * f, w * f);
	}
	inline Float4Attr operator /(const Float4Attr& f) const {
		return Float4Attr(x / f.x, y / f.y, z / f.z, w / f.w);
	}
	inline Float4Attr operator +(const Float4Attr& f) const {
		return Float4Attr(x + f.x, y + f.y, z + f.z, w + f.w);
	}
	inline Float4Attr operator -(const Float4Attr& f) const {
		return Float4Attr(x - f.x, y - f.y, z - f.z, w - f.w);
	}
	static inline Float4Attr Lerp(const Float4Attr& a, const Float4Attr& b, float factor) {
		Float4Attr delt = b - a;
		Float4Attr ret = a + delt * factor;
		return ret;
	}
    inline void Export(float* mem) {
        mem[0] = x;
        mem[1] = y;
        mem[2] = z;
        mem[3] = w;
    }
    inline EFloat4 ToEFloat4() {
        return EFloat4(x, y, z, w);
    }
};
class AttributeHandle
{
private:
	struct FAttrDestProc
	{
		inline void operator() (xhn::RWLock* lock) {
            Attribute* attr = (Attribute*)lock->GetUserdata();
			if (attr) {
				delete attr;
			}
		}
	};
public:
	/// if the lock is existed, then the attribute is existed;
	xhn::SmartPtr<xhn::RWLock, FAttrDestProc> m_lock;
	component_index m_bridge;
public:
	AttributeHandle()
	{}
	AttributeHandle(const AttributeHandle& ah)
		: m_lock(ah.m_lock)
		, m_bridge(ah.m_bridge)
	{}
	inline void operator = (const AttributeHandle& ah)
	{
		m_lock = ah.m_lock;
		m_bridge = ah.m_bridge;
	}
	template <typename T>
	void AttachAttribute()
	{
		if (m_lock.get() && !m_lock->GetUserdata()) {
			T* attr = ENEW T;
			m_lock->SetUserdata(attr);
			Attribute::GetBridge(attr, m_bridge);
		}
	}

	template <typename T>
	void GetAttribute(T* to) {
		if (m_lock.get()) {
			xhn::RWLock::Instance inst = m_lock->GetReadLock();
			T* attr = ((Attribute*)m_lock->GetUserdata())->DynamicCast<T>();
			///*to = *attr;
			Attribute::Copy(to, attr, m_bridge);
		}
	}
	template <typename T>
	void SetAttribute(T* from) {
		if (m_lock.get()) {
			xhn::RWLock::Instance inst = m_lock->GetWriteLock();
			T* attr = ((Attribute*)m_lock->GetUserdata())->DynamicCast<T>();
			///*attr = *from;
			component_index bridge = Attribute::GetReverseBridge(m_bridge);
			Attribute::Copy(attr, from, bridge);
		}
	}
};
#endif