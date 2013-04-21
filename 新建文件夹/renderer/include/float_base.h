#ifndef FLOAT_BASE_H
#define FLOAT_BASE_H
#include <common.h>
#include <etypes.h>
#define USE_SSE
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
typedef struct __float4
{
    float x, y, z, w;
} _float4;
typedef struct __float3
{
    float x, y, z;
} _float3;
typedef struct __float2
{
    float x, y;
} _float2;

typedef _float4* float4;
typedef _float3* float3;
typedef _float2* float2;

extern __m128* g_float3_mask;
extern __m128* g_float2_mask;

extern __m128* g_m128_queue;
extern unsigned int g_m128_queue_ptr;

extern void Float_Init();
extern void Float_Dest();
extern __m128* __m128_alloc();

extern uint16 float32_to_float16(float f);
extern float float16_to_float32(uint16 f);

typedef float EFloat;
typedef _float2 EFloat2;
typedef _float3 EFloat3;
typedef _float4 EFloat4;
#endif
