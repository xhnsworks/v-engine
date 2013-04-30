#ifndef FLOAT_BASE_H
#define FLOAT_BASE_H
#include "common.h"
#include "etypes.h"
#include "attribute.h"
class EFloat4 : public Attribute
{
public:
    float x, y, z, w;
public:
	EFloat4()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
		, w(0.0f)
	{}
	EFloat4(float _x, float _y, float _z, float _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{}
	inline EFloat4 operator *(const EFloat4& f) const {
        return EFloat4(x * f.x, y * f.y, z * f.z, w * f.w);
	}
	inline EFloat4 operator *(float f) const {
		return EFloat4(x * f, y * f, z * f, w * f);
	}
	inline EFloat4 operator /(const EFloat4& f) const {
		return EFloat4(x / f.x, y / f.y, z / f.z, w / f.w);
	}
	inline EFloat4 operator +(const EFloat4& f) const {
		return EFloat4(x + f.x, y + f.y, z + f.z, w + f.w);
	}
	inline EFloat4 operator -(const EFloat4& f) const {
		return EFloat4(x - f.x, y - f.y, z - f.z, w - f.w);
	}
	static inline EFloat4 Lerp(const EFloat4& a, const EFloat4& b, float factor) {
		EFloat4 delt = b - a;
		EFloat4 ret = a + delt * factor;
		return ret;
	}
};
class EFloat3 : public Attribute
{
public:
    float x, y, z;
public:
	EFloat3()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
	{}
	EFloat3(float _x, float _y, float _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{}
	inline EFloat3 operator *(const EFloat3& f) const {
		return EFloat3(x * f.x, y * f.y, z * f.z);
	}
	inline EFloat3 operator *(float f) const {
		return EFloat3(x * f, y * f, z * f);
	}
	inline EFloat3 operator /(const EFloat3& f) const {
		return EFloat3(x / f.x, y / f.y, z / f.z);
	}
	inline EFloat3 operator +(const EFloat3& f) const {
		return EFloat3(x + f.x, y + f.y, z + f.z);
	}
	inline EFloat3 operator -(const EFloat3& f) const {
		return EFloat3(x - f.x, y - f.y, z - f.z);
	}
	static inline EFloat3 Lerp(const EFloat3& a, const EFloat3& b, float factor) {
		EFloat3 delt = b - a;
		EFloat3 ret = a + delt * factor;
		return ret;
	}
};
class EFloat2 : public Attribute
{
public:
    float x, y;
public:
	EFloat2()
		: x(0.0f)
		, y(0.0f)
	{}
	EFloat2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}
	inline EFloat2 operator *(const EFloat2& f) const {
		return EFloat2(x * f.x, y * f.y);
	}
	inline EFloat2 operator *(float f) const {
		return EFloat2(x * f, y * f);
	}
	inline EFloat2 operator /(const EFloat2& f) const {
		return EFloat2(x / f.x, y / f.y);
	}
	inline EFloat2 operator +(const EFloat2& f) const {
		return EFloat2(x + f.x, y + f.y);
	}
	inline EFloat2 operator -(const EFloat2& f) const {
		return EFloat2(x - f.x, y - f.y);
	}
	static inline EFloat2 Lerp(const EFloat2& a, const EFloat2& b, float factor) {
		EFloat2 delt = b - a;
		EFloat2 ret = a + delt * factor;
		return ret;
	}
};
class EFloat : public Attribute
{
public:
	float x;
public:
	EFloat()
		: x(0.0f)
	{}
	EFloat(float _x)
		: x(_x)
	{}
	inline EFloat operator *(const EFloat& f) const {
		return EFloat(x * f.x);
	}
	inline EFloat operator *(float f) const {
		return EFloat(x * f);
	}
	inline EFloat operator /(const EFloat& f) const {
		return EFloat(x / f.x);
	}
	inline EFloat operator +(const EFloat& f) const {
		return EFloat(x + f.x);
	}
	inline EFloat operator -(const EFloat& f) const {
		return EFloat(x - f.x);
	}
	inline float Get() const {
		return x;
	}
	static inline EFloat Lerp(const EFloat& a, const EFloat& b, float factor) {
		EFloat delt = b - a;
		EFloat ret = a + delt * factor;
		return ret;
	}
};

typedef EFloat4* float4;
typedef EFloat3* float3;
typedef EFloat2* float2;

#ifdef USE_SSE
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#else
typedef struct __m128_t_
{
	float x, y, z, w;
} __m128;
typedef struct __m64_t_
{
	int x, y;
} __m64;
#endif

typedef __m128* v128;

extern v128 g_float3_mask;
extern v128 g_float2_mask;

API_EXPORT void Float_Init();
API_EXPORT void Float_Dest();

API_EXPORT v128 __m128_alloc();

API_EXPORT euint16 float32_to_float16(float f);
API_EXPORT float float16_to_float32(euint16 f);

///typedef float EFloat;

typedef __m128 sfloat3;
typedef __m128 sfloat4;

#define SFloat3Assert(f3) EAssert(((float*)&f3)[3] == 0.0f, "%s", "float3 error")
#endif
