#ifndef CLAMP_H
#define CLAMP_H
#include "common.h"
#include "etypes.h"
#include "sfloat3.h"
#include "sfloat4.h"
template <typename T>
inline T clamp(const T _v, const T _min, const T _max) {
	if (_v < _min)
		return _min;
	else if (_v > _max)
		return _max;
	else
		return _v;
}

inline sfloat3 clamp_float3(const sfloat3 _v, const sfloat3 _min, const sfloat3 _max) {
#ifdef USE_SSE
#ifdef DEBUG
	SFloat3Assert(_v);
	SFloat3Assert(_min);
	SFloat3Assert(_max);
#endif
	return _mm_min_ps( _mm_max_ps(_v, _min), _max );
#endif
}
inline sfloat4 clamp_float4(const sfloat4 _v, const sfloat4 _min, const sfloat4 _max) {
#ifdef USE_SSE
	return _mm_min_ps( _mm_max_ps(_v, _min), _max );
#endif
}
#endif