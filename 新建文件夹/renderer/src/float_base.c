#include "float_base.h"
#include "mem.h"
__m128* g_float3_mask;
__m128* g_float2_mask;

__m128* g_m128_queue = NULL;
unsigned int g_m128_queue_ptr = 0;

void Float_Init()
{
    g_float3_mask = Malloc(sizeof(__m128));
    g_float2_mask = Malloc(sizeof(__m128));

    ((unsigned int*)g_float3_mask)[0] = 0xffffffff;
    ((unsigned int*)g_float3_mask)[1] = 0xffffffff;
    ((unsigned int*)g_float3_mask)[2] = 0xffffffff;
    ((unsigned int*)g_float3_mask)[3] = 0x00000000;

    ((unsigned int*)g_float2_mask)[0] = 0xffffffff;
    ((unsigned int*)g_float2_mask)[1] = 0xffffffff;
    ((unsigned int*)g_float2_mask)[2] = 0x00000000;
    ((unsigned int*)g_float2_mask)[3] = 0x00000000;

    if (g_m128_queue)
        Mfree(g_m128_queue);
    g_m128_queue = Malloc(sizeof(__m128) * 256);
}

void Float_Dest()
{
    Mfree(g_float3_mask);
    Mfree(g_float2_mask);
    Mfree(g_m128_queue);
}

__m128* __m128_alloc()
{
    __m128* ret = &g_m128_queue[g_m128_queue_ptr];
    g_m128_queue_ptr++;
    g_m128_queue_ptr &= 0x000000ff;
    return ret;
}

uint16 float32_to_float16(float f)
{
    uint32 *i = (uint32 *)&f;
    uint32 sign = (*i >> 31) & 0x1;
    uint32 exponent = ((*i >> 23) & 0xff) - 0x7f;
    uint32 mantissa = (*i) & 0x7fffff;

    exponent += 0x7;
    uint16 ret = ((sign & 0x1) << 15);
    ret |= (exponent & 0xf) << 11;
    ret |= (mantissa  >> 13) & 0x7ff;
    return ret;
}

float float16_to_float32(uint16 f)
{
    uint16 *i = (uint16 *)&f;
    uint32 sign = (*i >> 15) & 0x1;
    uint32 exponent = ((*i >> 11) & 0xf) - 0x7;
    uint32 mantissa = (*i) & 0x7ff;

    exponent += 0x7f;
    uint32 ret = ((sign & 0x1) << 31);
    ret |= (exponent & 0xff) << 23;
    ret |= (mantissa << 13) & 0x7fffff;
    return *((float*)&ret);
}
