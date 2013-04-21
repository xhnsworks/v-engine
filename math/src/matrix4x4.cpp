#include "matrix4x4.h"
#include <math.h>
#include "array.h"
#include "float3.h"
#include "sfloat3.h"
#include "sfloat4.h"
#include "sse_mathfun.h"
#include "elog.h"

void Matrix4x4_Dest(matrix4x4* _mat)
{
    printf("matrix4x4 dest\n");
}
void _Matrix4x4_delete(matrix4x4* _mat, const char* _file, uint _line)
{
    Mfree(_mat);
}
matrix4x4* _Matrix4x4_new(const char* _file, uint _line)
{
    matrix4x4* ret = (matrix4x4*)_Malloc(sizeof(matrix4x4), _file, _line);
    ///memset(ret, 0, sizeof(matrix4x4));
    return ret;
}

void _log__m128(__m128* _m)
{
    elog("##__m128 %f %f %f %f", ((float*)_m)[0], ((float*)_m)[1], ((float*)_m)[2], ((float*)_m)[3]);
}

void Matrix4x4_mul_matrix4_test()
{
    __m128 temp1, temp2, temp3, temp4,
           temp5, temp6, temp7, temp8,
           temp9, temp10, temp11, temp12,
           temp13, temp14, temp15, temp16;

    matrix4x4* m1 = Matrix4x4_new();
    matrix4x4* m2 = Matrix4x4_new();

    m1->m0 = _mm_setr_ps(1,  2,  3,  4);
    m1->m1 = _mm_setr_ps(5,  6,  7,  8);
    m1->m2 = _mm_setr_ps(9,  10, 11, 12);
    m1->m3 = _mm_setr_ps(13, 14, 15, 16);

    _log__m128(&m1->m0);
    _log__m128(&m1->m1);
    _log__m128(&m1->m2);
    _log__m128(&m1->m3);

    m2->m0 = _mm_setr_ps(1,  2,  3,  4);
    m2->m1 = _mm_setr_ps(5,  6,  7,  8);
    m2->m2 = _mm_setr_ps(9,  10, 11, 12);
    m2->m3 = _mm_setr_ps(13, 14, 15, 16);

    _log__m128(&m2->m0);
    _log__m128(&m2->m1);
    _log__m128(&m2->m2);
    _log__m128(&m2->m3);

    temp1 = _mm_shuffle_ps( m2->m0, m2->m0, 0x00 );
    temp2 = _mm_shuffle_ps( m2->m0, m2->m0, 0x55 );
    temp3 = _mm_shuffle_ps( m2->m0, m2->m0, 0xaa );
    temp4 = _mm_shuffle_ps( m2->m0, m2->m0, 0xff );

    _log__m128(&temp1);
    _log__m128(&temp2);
    _log__m128(&temp3);
    _log__m128(&temp4);

    temp5 = _mm_shuffle_ps( m2->m1, m2->m1, 0x00 );
    temp6 = _mm_shuffle_ps( m2->m1, m2->m1, 0x55 );
    temp7 = _mm_shuffle_ps( m2->m1, m2->m1, 0xaa );
    temp8 = _mm_shuffle_ps( m2->m1, m2->m1, 0xff );

    _log__m128(&temp5);
    _log__m128(&temp6);
    _log__m128(&temp7);
    _log__m128(&temp8);

    temp9 = _mm_shuffle_ps( m2->m2, m2->m2, 0x00 );
    temp10 = _mm_shuffle_ps( m2->m2, m2->m2, 0x55 );
    temp11 = _mm_shuffle_ps( m2->m2, m2->m2, 0xaa );
    temp12 = _mm_shuffle_ps( m2->m2, m2->m2, 0xff );

    _log__m128(&temp9);
    _log__m128(&temp10);
    _log__m128(&temp11);
    _log__m128(&temp12);

    temp13 = _mm_shuffle_ps( m2->m3, m2->m3, 0x00 );
    temp14 = _mm_shuffle_ps( m2->m3, m2->m3, 0x55 );
    temp15 = _mm_shuffle_ps( m2->m3, m2->m3, 0xaa );
    temp16 = _mm_shuffle_ps( m2->m3, m2->m3, 0xff );

    _log__m128(&temp13);
    _log__m128(&temp14);
    _log__m128(&temp15);
    _log__m128(&temp16);

    temp1 = _mm_mul_ps( m1->m0, temp1 );
    temp2 = _mm_mul_ps( m1->m1, temp2 );
    temp3 = _mm_mul_ps( m1->m2, temp3 );
    temp4 = _mm_mul_ps( m1->m3, temp4 );

    _log__m128(&temp1);
    _log__m128(&temp2);
    _log__m128(&temp3);
    _log__m128(&temp4);

    temp5 = _mm_mul_ps( m1->m0, temp5 );
    temp6 = _mm_mul_ps( m1->m1, temp6 );
    temp7 = _mm_mul_ps( m1->m2, temp7 );
    temp8 = _mm_mul_ps( m1->m3, temp8 );

    _log__m128(&temp5);
    _log__m128(&temp6);
    _log__m128(&temp7);
    _log__m128(&temp8);

    temp9 = _mm_mul_ps( m1->m0, temp9 );
    temp10 = _mm_mul_ps( m1->m1, temp10 );
    temp11 = _mm_mul_ps( m1->m2, temp11 );
    temp12 = _mm_mul_ps( m1->m3, temp12 );

    _log__m128(&temp9);
    _log__m128(&temp10);
    _log__m128(&temp11);
    _log__m128(&temp12);

    temp13 = _mm_mul_ps( m1->m0, temp13 );
    temp14 = _mm_mul_ps( m1->m1, temp14 );
    temp15 = _mm_mul_ps( m1->m2, temp15 );
    temp16 = _mm_mul_ps( m1->m3, temp16 );

    _log__m128(&temp13);
    _log__m128(&temp14);
    _log__m128(&temp15);
    _log__m128(&temp16);
///
    matrix4x4* result = Matrix4x4_new();

    temp1 = _mm_add_ps(temp1,temp2);
    temp3 = _mm_add_ps(temp3,temp4);
    result->m0 = _mm_add_ps(temp1,temp3);

    temp5 = _mm_add_ps(temp5,temp6);
    temp7 = _mm_add_ps(temp7,temp8);
    result->m1 = _mm_add_ps(temp5,temp7);

    temp9 = _mm_add_ps(temp9,temp10);
    temp11 = _mm_add_ps(temp11,temp12);
    result->m2 = _mm_add_ps(temp9,temp11);

    temp13 = _mm_add_ps(temp13,temp14);
    temp15 = _mm_add_ps(temp15,temp16);
    result->m3 = _mm_add_ps(temp13,temp15);

    _log__m128(&result->m0);
    _log__m128(&result->m1);
    _log__m128(&result->m2);
    _log__m128(&result->m3);
}

void Matrix4x4_mul_matrix4(const matrix4x4* m1, const matrix4x4* m2, matrix4x4* result)
{
#ifdef USE_SSE
    __m128 temp1, temp2, temp3, temp4,
           temp5, temp6, temp7, temp8,
           temp9, temp10, temp11, temp12,
           temp13, temp14, temp15, temp16;

    temp1 = _mm_shuffle_ps( m2->m0, m2->m0, 0x00 );
    temp2 = _mm_shuffle_ps( m2->m0, m2->m0, 0x55 );
    temp3 = _mm_shuffle_ps( m2->m0, m2->m0, 0xaa );
    temp4 = _mm_shuffle_ps( m2->m0, m2->m0, 0xff );

    temp5 = _mm_shuffle_ps( m2->m1, m2->m1, 0x00 );
    temp6 = _mm_shuffle_ps( m2->m1, m2->m1, 0x55 );
    temp7 = _mm_shuffle_ps( m2->m1, m2->m1, 0xaa );
    temp8 = _mm_shuffle_ps( m2->m1, m2->m1, 0xff );

    temp9 = _mm_shuffle_ps( m2->m2, m2->m2, 0x00 );
    temp10 = _mm_shuffle_ps( m2->m2, m2->m2, 0x55 );
    temp11 = _mm_shuffle_ps( m2->m2, m2->m2, 0xaa );
    temp12 = _mm_shuffle_ps( m2->m2, m2->m2, 0xff );

    temp13 = _mm_shuffle_ps( m2->m3, m2->m3, 0x00 );
    temp14 = _mm_shuffle_ps( m2->m3, m2->m3, 0x55 );
    temp15 = _mm_shuffle_ps( m2->m3, m2->m3, 0xaa );
    temp16 = _mm_shuffle_ps( m2->m3, m2->m3, 0xff );

    temp1 = _mm_mul_ps( m1->m0, temp1 );
    temp2 = _mm_mul_ps( m1->m1, temp2 );
    temp3 = _mm_mul_ps( m1->m2, temp3 );
    temp4 = _mm_mul_ps( m1->m3, temp4 );

    temp5 = _mm_mul_ps( m1->m0, temp5 );
    temp6 = _mm_mul_ps( m1->m1, temp6 );
    temp7 = _mm_mul_ps( m1->m2, temp7 );
    temp8 = _mm_mul_ps( m1->m3, temp8 );

    temp9 = _mm_mul_ps( m1->m0, temp9 );
    temp10 = _mm_mul_ps( m1->m1, temp10 );
    temp11 = _mm_mul_ps( m1->m2, temp11 );
    temp12 = _mm_mul_ps( m1->m3, temp12 );

    temp13 = _mm_mul_ps( m1->m0, temp13 );
    temp14 = _mm_mul_ps( m1->m1, temp14 );
    temp15 = _mm_mul_ps( m1->m2, temp15 );
    temp16 = _mm_mul_ps( m1->m3, temp16 );

///

    temp1 = _mm_add_ps(temp1,temp2);
    temp3 = _mm_add_ps(temp3,temp4);
    result->m0 = _mm_add_ps(temp1,temp3);

    temp5 = _mm_add_ps(temp5,temp6);
    temp7 = _mm_add_ps(temp7,temp8);
    result->m1 = _mm_add_ps(temp5,temp7);

    temp9 = _mm_add_ps(temp9,temp10);
    temp11 = _mm_add_ps(temp11,temp12);
    result->m2 = _mm_add_ps(temp9,temp11);

    temp13 = _mm_add_ps(temp13,temp14);
    temp15 = _mm_add_ps(temp15,temp16);
    result->m3 = _mm_add_ps(temp13,temp15);
#else
    float _ret[4][4];
    _ret[0][0] = m1->m[0][0] * m2->m[0][0] + m1->m[0][1] * m2->m[1][0] + m1->m[0][2] * m2->m[2][0] + m1->m[0][3] * m2->m[3][0];
    _ret[0][1] = m1->m[0][0] * m2->m[0][1] + m1->m[0][1] * m2->m[1][1] + m1->m[0][2] * m2->m[2][1] + m1->m[0][3] * m2->m[3][1];
    _ret[0][2] = m1->m[0][0] * m2->m[0][2] + m1->m[0][1] * m2->m[1][2] + m1->m[0][2] * m2->m[2][2] + m1->m[0][3] * m2->m[3][2];
    _ret[0][3] = m1->m[0][0] * m2->m[0][3] + m1->m[0][1] * m2->m[1][3] + m1->m[0][2] * m2->m[2][3] + m1->m[0][3] * m2->m[3][3];

    _ret[1][0] = m1->m[1][0] * m2->m[0][0] + m1->m[1][1] * m2->m[1][0] + m1->m[1][2] * m2->m[2][0] + m1->m[1][3] * m2->m[3][0];
    _ret[1][1] = m1->m[1][0] * m2->m[0][1] + m1->m[1][1] * m2->m[1][1] + m1->m[1][2] * m2->m[2][1] + m1->m[1][3] * m2->m[3][1];
    _ret[1][2] = m1->m[1][0] * m2->m[0][2] + m1->m[1][1] * m2->m[1][2] + m1->m[1][2] * m2->m[2][2] + m1->m[1][3] * m2->m[3][2];
    _ret[1][3] = m1->m[1][0] * m2->m[0][3] + m1->m[1][1] * m2->m[1][3] + m1->m[1][2] * m2->m[2][3] + m1->m[1][3] * m2->m[3][3];

    _ret[2][0] = m1->m[2][0] * m2->m[0][0] + m1->m[2][1] * m2->m[1][0] + m1->m[2][2] * m2->m[2][0] + m1->m[2][3] * m2->m[3][0];
    _ret[2][1] = m1->m[2][0] * m2->m[0][1] + m1->m[2][1] * m2->m[1][1] + m1->m[2][2] * m2->m[2][1] + m1->m[2][3] * m2->m[3][1];
    _ret[2][2] = m1->m[2][0] * m2->m[0][2] + m1->m[2][1] * m2->m[1][2] + m1->m[2][2] * m2->m[2][2] + m1->m[2][3] * m2->m[3][2];
    _ret[2][3] = m1->m[2][0] * m2->m[0][3] + m1->m[2][1] * m2->m[1][3] + m1->m[2][2] * m2->m[2][3] + m1->m[2][3] * m2->m[3][3];

    _ret[3][0] = m1->m[3][0] * m2->m[0][0] + m1->m[3][1] * m2->m[1][0] + m1->m[3][2] * m2->m[2][0] + m1->m[3][3] * m2->m[3][0];
    _ret[3][1] = m1->m[3][0] * m2->m[0][1] + m1->m[3][1] * m2->m[1][1] + m1->m[3][2] * m2->m[2][1] + m1->m[3][3] * m2->m[3][1];
    _ret[3][2] = m1->m[3][0] * m2->m[0][2] + m1->m[3][1] * m2->m[1][2] + m1->m[3][2] * m2->m[2][2] + m1->m[3][3] * m2->m[3][2];
    _ret[3][3] = m1->m[3][0] * m2->m[0][3] + m1->m[3][1] * m2->m[1][3] + m1->m[3][2] * m2->m[2][3] + m1->m[3][3] * m2->m[3][3];

    result->m[0][0] = _ret[0][0];
    result->m[0][1] = _ret[0][1];
    result->m[0][2] = _ret[0][2];
    result->m[0][3] = _ret[0][3];

    result->m[1][0] = _ret[1][0];
    result->m[1][1] = _ret[1][1];
    result->m[1][2] = _ret[1][2];
    result->m[1][3] = _ret[1][3];

    result->m[2][0] = _ret[2][0];
    result->m[2][1] = _ret[2][1];
    result->m[2][2] = _ret[2][2];
    result->m[2][3] = _ret[2][3];

    result->m[3][0] = _ret[3][0];
    result->m[3][1] = _ret[3][1];
    result->m[3][2] = _ret[3][2];
    result->m[3][3] = _ret[3][3];
#endif
}

void Matrix4x4_assign(matrix4x4* _dst, const matrix4x4* _src)
{
#ifdef USE_SSE
    _dst->m0 = _src->m0;
    _dst->m1 = _src->m1;
    _dst->m2 = _src->m2;
    _dst->m3 = _src->m3;
#else
#endif
}


void Matrix4x4_set_translate(matrix4x4* _mat, float x, float y, float z)
{
#ifdef USE_SSE
	((float*)&_mat->m0)[3] = x;
	((float*)&_mat->m1)[3] = y;
	((float*)&_mat->m2)[3] = z;
#else
#endif
}

void Matrix4x4_set_as_translate(matrix4x4* _mat, float x, float y, float z)
{
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps(1.0,0.0,0.0,x);
    _mat->m1 = _mm_setr_ps(0.0,1.0,0.0,y);
    _mat->m2 = _mm_setr_ps(0.0,0.0,1.0,z);
    _mat->m3 = _mm_setr_ps(0.0,0.0,0.0,1.0);
#else
    _mat->m[0][0] = 1.0;
    _mat->m[0][1] = 0.0;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = x;

    _mat->m[1][0] = 0.0;
    _mat->m[1][1] = 1.0;
    _mat->m[1][2] = 0.0;
    _mat->m[1][3] = y;

    _mat->m[2][0] = 0.0;
    _mat->m[2][1] = 0.0;
    _mat->m[2][2] = 1.0;
    _mat->m[2][3] = z;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = 0.0;
    _mat->m[3][3] = 1.0;
#endif
}

EFloat3 Matrix4x4_get_translate(matrix4x4* _mat)
{
#ifdef USE_SSE
    EFloat3 ret( ((float*)&_mat->m0)[3], ((float*)&_mat->m1)[3], ((float*)&_mat->m2)[3] );
    return ret;
#else
#endif
}

void Matrix4x4_set_zero(matrix4x4* _mat)
{
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps(0.0,0.0,0.0,0.0);
    _mat->m1 = _mm_setr_ps(0.0,0.0,0.0,0.0);
    _mat->m2 = _mm_setr_ps(0.0,0.0,0.0,0.0);
    _mat->m3 = _mm_setr_ps(0.0,0.0,0.0,0.0);
#else
    _mat->m[0][0] = 0.0;
    _mat->m[0][1] = 0.0;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = 0.0;
    _mat->m[1][1] = 0.0;
    _mat->m[1][2] = 0.0;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = 0.0;
    _mat->m[2][1] = 0.0;
    _mat->m[2][2] = 0.0;
    _mat->m[2][3] = 0.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = 0.0;
    _mat->m[3][3] = 0.0;
#endif
}

void Matrix4x4_set_one(matrix4x4* _mat)
{
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps(1.0,0.0,0.0,0.0);
    _mat->m1 = _mm_setr_ps(0.0,1.0,0.0,0.0);
    _mat->m2 = _mm_setr_ps(0.0,0.0,1.0,0.0);
    _mat->m3 = _mm_setr_ps(0.0,0.0,0.0,1.0);
#else
    _mat->m[0][0] = 1.0;
    _mat->m[0][1] = 0.0;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = 0.0;
    _mat->m[1][1] = 1.0;
    _mat->m[1][2] = 0.0;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = 0.0;
    _mat->m[2][1] = 0.0;
    _mat->m[2][2] = 1.0;
    _mat->m[2][3] = 0.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = 0.0;
    _mat->m[3][3] = 1.0;
#endif
}
void _Matrix4x4_export_empty_matrix(float* _ret)
{
    _ret[0] = 1.0f;
    _ret[1] = 0.0f;
    _ret[2] = 0.0f;
    _ret[3] = 0.0f;

    _ret[4] = 0.0f;
    _ret[5] = 1.0f;
    _ret[6] = 0.0f;
    _ret[7] = 0.0f;

    _ret[8] = 0.0f;
    _ret[9] = 0.0f;
    _ret[10] = 1.0f;
    _ret[11] = 0.0f;

    _ret[12] = 0.0f;
    _ret[13] = 0.0f;
    _ret[14] = 0.0f;
    _ret[15] = 1.0f;
}
void Matrix4x4_export(matrix4x4* _mat, float* _ret)
{
    if (!_mat) {
        _Matrix4x4_export_empty_matrix(_ret);
        return;
    }
#ifdef USE_SSE
    _ret[0] = ((float*)&_mat->m0)[0];
    _ret[1] = ((float*)&_mat->m0)[1];
    _ret[2] = ((float*)&_mat->m0)[2];
    _ret[3] = ((float*)&_mat->m0)[3];

    _ret[4] = ((float*)&_mat->m1)[0];
    _ret[5] = ((float*)&_mat->m1)[1];
    _ret[6] = ((float*)&_mat->m1)[2];
    _ret[7] = ((float*)&_mat->m1)[3];

    _ret[8] = ((float*)&_mat->m2)[0];
    _ret[9] = ((float*)&_mat->m2)[1];
    _ret[10] = ((float*)&_mat->m2)[2];
    _ret[11] = ((float*)&_mat->m2)[3];

    _ret[12] = ((float*)&_mat->m3)[0];
    _ret[13] = ((float*)&_mat->m3)[1];
    _ret[14] = ((float*)&_mat->m3)[2];
    _ret[15] = ((float*)&_mat->m3)[3];
#else
    _ret[0] = _mat->m[0][0];
    _ret[1] = _mat->m[0][1];
    _ret[2] = _mat->m[0][2];
    _ret[3] = _mat->m[0][3];

    _ret[4] = _mat->m[1][0];
    _ret[5] = _mat->m[1][1];
    _ret[6] = _mat->m[1][2];
    _ret[7] = _mat->m[1][3];

    _ret[8] = _mat->m[2][0];
    _ret[9] = _mat->m[2][1];
    _ret[10] = _mat->m[2][2];
    _ret[11] = _mat->m[2][3];

    _ret[12] = _mat->m[3][0];
    _ret[13] = _mat->m[3][1];
    _ret[14] = _mat->m[3][2];
    _ret[15] = _mat->m[3][3];
#endif
}

void Matrix4x4_import(matrix4x4* _mat, float* _src)
{
#ifdef USE_SSE
    ((float*)&_mat->m0)[0] = _src[0];
    ((float*)&_mat->m0)[1] = _src[1];
    ((float*)&_mat->m0)[2] = _src[2];
    ((float*)&_mat->m0)[3] = _src[3];

    ((float*)&_mat->m1)[0] = _src[4];
    ((float*)&_mat->m1)[1] = _src[5];
    ((float*)&_mat->m1)[2] = _src[6];
    ((float*)&_mat->m1)[3] = _src[7];

    ((float*)&_mat->m2)[0] = _src[8];
    ((float*)&_mat->m2)[1] = _src[9];
    ((float*)&_mat->m2)[2] = _src[10];
    ((float*)&_mat->m2)[3] = _src[11];

    ((float*)&_mat->m3)[0] = _src[12];
    ((float*)&_mat->m3)[1] = _src[13];
    ((float*)&_mat->m3)[2] = _src[14];
    ((float*)&_mat->m3)[3] = _src[15];
#else
    _mat->m[0][0] = _src[0]
    _mat->m[0][1] = _src[1]
    _mat->m[0][2] = _src[2]
    _mat->m[0][3] = _src[3]

    _mat->m[1][0] = _src[4]
    _mat->m[1][1] = _src[5]
    _mat->m[1][2] = _src[6]
    _mat->m[1][3] = _src[7]

    _mat->m[2][0] = _src[8]
    _mat->m[2][1] = _src[9]
    _mat->m[2][2] = _src[10];
    _mat->m[2][3] = _src[11];

    _mat->m[3][0] = _src[12];
    _mat->m[3][1] = _src[13];
    _mat->m[3][2] = _src[14];
    _mat->m[3][3] = _src[15];
#endif
}

void Matrix4x4_blend(matrix4x4* _mat)
{
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps(0.5, 0.0, 0.0, 0.5);
    _mat->m1 = _mm_setr_ps(0.0, 0.5, 0.0, 0.5);
    _mat->m2 = _mm_setr_ps(0.0, 0.0, 0.5, 0.5);
    _mat->m3 = _mm_setr_ps(0.5, 0.5, 0.5, 1.0);
#else
#endif
}

///void Matrix4x4_projection(matrix4x4* _mat, float _left, float _right, float _top, float _bottom, float _near, float _far)
void Matrix4x4_projection(matrix4x4* _mat, float _width, float _height, float _near, float _far)
{
    float epsilon = 2.4e-7f;
    float l = -0.5f * _width;
    float r = 0.5f * _width;
    float t = 0.5f * _height;
    float b = -0.5f * _height;
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps( 2.0f * _near / (r - l), 0.0f,                   0.0f,                                  0.0f  );
    _mat->m1 = _mm_setr_ps( 0.0f,                   2.0f * _near / (t - b), 0.0f,                                  0.0f  );
    ///_mat->m2 = _mm_setr_ps( 0.0,                   0.0f,                   epsilon-1.0f,                          -1.0f );
    _mat->m2 = _mm_setr_ps( 0.0f,                   0.0f,                   -(_far + _near) / (_far - _near),     -1.0f );
    _mat->m3 = _mm_setr_ps( 0.0f,                   0.0f,                   -2.0f * _far * _near / (_far - _near), 0.0f  );
#else
    _mat->m[0][0] = 2.0 * _near / (r - l);
    _mat->m[0][1] = 0.0;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = 0.0;
    _mat->m[1][1] = 2.0 * _near / (t - b);
    _mat->m[1][2] = 0.0;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = 0.0;
    _mat->m[2][1] = 0.0;
    ///_mat->m[2][2] = epsilon-1.0;
    _mat->m[2][2] = -(_far + _near) / (_far - _near);
    _mat->m[2][3] = -1.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = -2.0 * _far * _near / (_far - _near);
    _mat->m[3][3] = 0.0;
#endif
}
void Matrix4x4_orthogonal(matrix4x4* _mat, float _width, float _height, float _near, float _far)
{
    float epsilon = 2.4e-7f;
    float l = -0.5f * _width;
    float r = 0.5f * _width;
    float t = 0.5f * _height;
    float b = -0.5f * _height;
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps( 2.0f / (r - l),         0.0f,                   0.0f,                      0.0f );
    _mat->m1 = _mm_setr_ps( 0.0f,                   2.0f / (t - b),         0.0f,                      0.0f );
    _mat->m2 = _mm_setr_ps( 0.0f,                   0.0f,                   2.0f / (_near - _far),     (_near + _far) / (_near - _far) );
    _mat->m3 = _mm_setr_ps( 0.0f,                   0.0f,                   0.0f,                      1.0f  );
#endif
	matrix4x4 tran;
	Matrix4x4_set_as_translate(&tran, 0.0f, 0.0f, 1.0f);
	Matrix4x4_mul_matrix4(_mat, &tran, _mat);
}
void Matrix4x4_proj_scale(matrix4x4* _mat, float x_scale, float y_scale)
{
#ifdef USE_SSE
    __m128 x_scale_m128 = _mm_setr_ps(x_scale, 0.0f, 0.0f, 0.0f);
    __m128 y_scale_m128 = _mm_setr_ps(0.0f, y_scale, 0.0f, 0.0f);
    _mat->m0 = _mm_mul_ps(_mat->m0, x_scale_m128);
    _mat->m1 = _mm_mul_ps(_mat->m1, y_scale_m128);
#endif
}

void Matrix4x4_from_axis_angle(matrix4x4* _mat, sfloat3 _axis, float _radian)
{

#ifdef USE_SSE
    ///ALIGN_Value20(__m128, buf, x, s, c, one, fOneMinusCos, fxyz2, fxyz_sin, tmp0, tmp1, tmp11, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10);
    ///__m128 buf, x, s, c, one, fOneMinusCos, fxyz2, fxyz_sin, tmp0, tmp1, tmp11, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
	__m128 x, s, c, one, fOneMinusCos, fxyz2, fxyz_sin, tmp0, tmp1, tmp11, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;

    x = _mm_set1_ps(_radian);
    one = _mm_set1_ps(1.0f);
    sincos_ps(x, &s, &c);
    fOneMinusCos = _mm_sub_ps(one, c);

    ///__m128 axis_m = _mm_set_ps(1.0f, _axis->z, _axis->y, _axis->x);
    ///__m128 axis_m = _mm_set_ps(1.0f, ((float*)&_axis)[0], ((float*)&_axis)[1], ((float*)&_axis)[2]);
    __m128 axis_m = _axis;
    ((float*)&axis_m)[3] = 1.0f;

    fxyz2 = _mm_mul_ps(axis_m, axis_m);
    fxyz_sin = _mm_mul_ps(axis_m, s);

    tmp0 = _mm_shuffle_ps(axis_m, axis_m, _MM_SHUFFLE(3, 1, 0, 0));
    // x, x, y
    tmp1 = _mm_shuffle_ps(axis_m, axis_m, _MM_SHUFFLE(3, 2, 2, 1));
    // y, z, z

    tmp0 = _mm_mul_ps(tmp0, tmp1);
    tmp0 = _mm_mul_ps(tmp0, fOneMinusCos);
    // tmp0 fxym fxzm fyzm
    // 01 00 00 00
    // 00000000 ffffffff ffffffff ffffffff
    // tmp1 00000000 ffffffff ffffffff ffffffff
    // tmp1 ffffffff 00000000 00000000 00000000
    // 00 01 01 01

    ///tmp0 = _mm_slli_si128( (__m128i)tmp0, 4 );
    ///tmp0 = _mm_srli_si128( (__m128i)tmp0, 4 );
    tmp0 = _mm_and_ps(tmp0, *((__m128*)g_float3_mask));
	/**
    asm
    (
        "movaps %%xmm0, %[buf]        \n\t"
        "movd %%eax, %%xmm0           \n\t"
        "movaps %%xmm0, %[tmp1]       \n\t"
        "movaps %[buf], %%xmm0        \n\t"
    :[tmp0]"+m"(tmp0), [tmp1]"+m"(tmp1), [buf]"+m"(buf)
    :"a"(0xffffffff)
            );
			**/
	float fe = fexce;
	tmp1 = _mm_setr_ps(fe, 0.0f, 0.0f, 0.0f);
    // tmp0 fxym fxzm fyzm 0

    tmp11 = _mm_mul_ps(fxyz2, fOneMinusCos);
    tmp11 = _mm_add_ps(tmp11, c);
    tmp2 = _mm_and_ps(tmp11, tmp1);
    // fX2*fOneMinusCos+fCos
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(1, 1, 0, 1));
    tmp3 = _mm_and_ps(tmp11, tmp1);
    // fY2*fOneMinusCos+fCos
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(0, 1, 0, 0));
    tmp4 = _mm_and_ps(tmp11, tmp1);

    tmp5 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(0, 2, 2, 2));
    fxyz_sin = _mm_and_ps(fxyz_sin, tmp5);
    tmp5 = _mm_shuffle_ps(fxyz_sin, fxyz_sin, _MM_SHUFFLE(3, 1, 2, 3));
    // 0, fzsin, fysin, 0
    tmp6 = _mm_shuffle_ps(fxyz_sin, fxyz_sin, _MM_SHUFFLE(3, 0, 3, 2));
    // fzsin, 0, fxsin, 0
    tmp7 = _mm_shuffle_ps(fxyz_sin, fxyz_sin, _MM_SHUFFLE(3, 3, 0, 1));
    // fysin, fxsin, 0, 0

    tmp8 = _mm_setr_ps(0.0, -1.0, 1.0, 0.0);
    tmp9 = _mm_shuffle_ps(tmp8, tmp8, _MM_SHUFFLE(0, 1, 0, 2));
    // 1, 0, -1, 0
    tmp10 = _mm_shuffle_ps(tmp8, tmp8, _MM_SHUFFLE(0, 0, 2, 1));
    // -1, 1, 0, 0

    tmp5 = _mm_mul_ps(tmp5, tmp8);
    tmp6 = _mm_mul_ps(tmp6, tmp9);
    tmp7 = _mm_mul_ps(tmp7, tmp10);

    tmp5 = _mm_or_ps(tmp5, tmp2);
    tmp6 = _mm_or_ps(tmp6, tmp3);
    tmp7 = _mm_or_ps(tmp7, tmp4);

    tmp2 = _mm_shuffle_ps(tmp0, tmp0, _MM_SHUFFLE(3, 1, 0 ,3));
    tmp3 = _mm_shuffle_ps(tmp0, tmp0, _MM_SHUFFLE(3, 2, 3 ,0));
    tmp4 = _mm_shuffle_ps(tmp0, tmp0, _MM_SHUFFLE(3, 3, 2 ,1));

    _mat->m0 = _mm_add_ps(tmp5, tmp2);
    _mat->m1 = _mm_add_ps(tmp6, tmp3);
    _mat->m2 = _mm_add_ps(tmp7, tmp4);
    _mat->m3 = _mm_setr_ps(0.0, 0.0, 0.0, 1.0);
#else
    float fCos = cos(radian);
    float fSin = sin(radian);
    float fOneMinusCos = 1.0f-fCos;
    float fX2 = axis.x*axis.x;
    float fY2 = axis.y*axis.y;
    float fZ2 = axis.z*axis.z;
    float fXYM = axis.x*axis.y*fOneMinusCos;
    float fXZM = axis.x*axis.z*fOneMinusCos;
    float fYZM = axis.y*axis.z*fOneMinusCos;
    float fXSin = axis.x*fSin;
    float fYSin = axis.y*fSin;
    float fZSin = axis.z*fSin;

    _mat->m[0][0] = fX2*fOneMinusCos+fCos;
    _mat->m[0][1] = fXYM-fZSin;
    _mat->m[0][2] = fXZM+fYSin;
    _mat->m[1][0] = fXYM+fZSin;
    _mat->m[1][1] = fY2*fOneMinusCos+fCos;
    _mat->m[1][2] = fYZM-fXSin;
    _mat->m[2][0] = fXZM-fYSin;
    _mat->m[2][1] = fYZM+fXSin;
    _mat->m[2][2] = fZ2*fOneMinusCos+fCos;
#endif
}

void Matrix4x4_from_vec_to_vec(matrix4x4* _mat, sfloat3 src_vec, sfloat3 dst_vec)
{
    sfloat3 axis = SFloat3_cross(src_vec, dst_vec);
    axis = SFloat3_normalize(axis);

    float dot = SFloat3_dot(src_vec, dst_vec);
    float rad = acosf(dot);

    if (fabs(rad) > 0.0001f)
    {
        Matrix4x4_from_axis_angle(_mat, axis, rad);
    }
    else
    {
        Matrix4x4_set_one(_mat);
    }
}

/**
void Matrix4x4_projection2(matrix4x4* _mat, float _width, float _height, float _near, float _far)
{
#ifdef USE_SSE
    float l = -0.5 * _width;
    float r = 0.5 * _width;
    float t = 0.5 * _height;
    float b = -0.5 * _height;
    _mat->m0 = _mm_setr_ps( 2.0 * _near / (r - l), 0.0,                   0.0,                                  0.0  );
    _mat->m1 = _mm_setr_ps( 0.0,                   2.0 * _near / (t - b), 0.0,                                  0.0  );
    _mat->m2 = _mm_setr_ps( (r + l) / (r - l),     (t + b) / (t - b),     -(_far + _near) / (_far - _near),     -1.0 );
    _mat->m3 = _mm_setr_ps( 0.0,                   0.0,                   -2.0 * _far * _near / (_far - _near), 0.0  );
#else
#endif
}
**/
/**
void Matrix4x4_projection(matrix4x4* _mat, float _left, float _right, float _top, float _bottom, float _far, float _near)
{
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps(2.0 * _far / (_right-_left), 0.0, 0.0, 0.0);
    _mat->m1 = _mm_setr_ps(0.0, 2.0 * _far / (_top - _bottom), 0.0, 0.0);
    _mat->m2 = _mm_setr_ps((_right + _left) / (_right - _left), (_top + _bottom) / (_top - _bottom), -(_near + _far) / (_near - _far), -1.0);
    _mat->m3 = _mm_setr_ps(0.0, 0.0, -2.0 * _near * _far / (_near - _far), 0.0);
#else
    _mat->m[0][0] = 2.0 * _near / (_right-_left);
    _mat->m[0][1] = 0.0;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = 0.0;
    _mat->m[1][1] = 2.0 * _near / (_top - _bottom);
    _mat->m[1][2] = 0.0;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = (_right + _left) / (_right - _left);
    _mat->m[2][1] = (_top + _bottom) / (_top - _bottom);
    _mat->m[2][2] = -(_far + _near) / (_far - _near);
    _mat->m[2][3] = -1.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = -2.0 * _far * _near / (_far - _near);
    _mat->m[3][3] = 0.0;
#endif
}
**/
void Matrix4x4_turn(matrix4x4* _mat)
{
#ifdef USE_SSE
    float x0, y0, z0, w0;
    x0 = ((float*)&_mat->m0)[0];
    y0 = ((float*)&_mat->m1)[0];
    z0 = ((float*)&_mat->m2)[0];
    w0 = ((float*)&_mat->m3)[0];

    float x1, y1, z1, w1;
    x1 = ((float*)&_mat->m0)[1];
    y1 = ((float*)&_mat->m1)[1];
    z1 = ((float*)&_mat->m2)[1];
    w1 = ((float*)&_mat->m3)[1];

    float x2, y2, z2, w2;
    x2 = ((float*)&_mat->m0)[2];
    y2 = ((float*)&_mat->m1)[2];
    z2 = ((float*)&_mat->m2)[2];
    w2 = ((float*)&_mat->m3)[2];

    float x3, y3, z3, w3;
    x3 = ((float*)&_mat->m0)[3];
    y3 = ((float*)&_mat->m1)[3];
    z3 = ((float*)&_mat->m2)[3];
    w3 = ((float*)&_mat->m3)[3];

    _mat->m0 = _mm_setr_ps(x0, y0, z0, w0);
    _mat->m1 = _mm_setr_ps(x1, y1, z1, w1);
    _mat->m2 = _mm_setr_ps(x2, y2, z2, w2);
    _mat->m3 = _mm_setr_ps(x3, y3, z3, w3);
#else
#endif
}

void Matrix4x4_inverse(matrix4x4* _mat)
{
#ifdef USE_SSE
    __m128 dm, idm, one, ffffffff, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, v0_3, v4_5, v5543, v4221,
    m1000, m2211, m3332, v3100, t, invDet, d00_10_20_30, d01_11_21_31, d02_12_22_32, d03_13_23_33;

    dm = _mm_setr_ps(1.0, -1.0, 1.0, -1.0);
    idm = _mm_setr_ps(-1.0, 1.0, -1.0, 1.0);
    one = _mm_set1_ps(1.0);

    ((uint32*)&ffffffff)[0] = 0xffffffff;
    ((uint32*)&ffffffff)[1] = 0x00000000;
    ((uint32*)&ffffffff)[2] = 0x00000000;
    ((uint32*)&ffffffff)[3] = 0x00000000;

    tmp0 = _mm_shuffle_ps(_mat->m2, _mat->m2, _MM_SHUFFLE(1, 0, 0 ,0));
    tmp1 = _mm_shuffle_ps(_mat->m3, _mat->m3, _MM_SHUFFLE(2, 3, 2 ,1));
    tmp2 = _mm_shuffle_ps(_mat->m2, _mat->m2, _MM_SHUFFLE(2, 3, 2 ,1));
    tmp3 = _mm_shuffle_ps(_mat->m3, _mat->m3, _MM_SHUFFLE(1, 0, 0 ,0));

    tmp4 = _mm_shuffle_ps(_mat->m2, _mat->m2, _MM_SHUFFLE(0, 0, 2 ,1));
    tmp5 = _mm_shuffle_ps(_mat->m3, _mat->m3, _MM_SHUFFLE(0, 0, 3 ,3));
    tmp6 = _mm_shuffle_ps(_mat->m2, _mat->m2, _MM_SHUFFLE(0, 0, 3 ,3));
    tmp7 = _mm_shuffle_ps(_mat->m3, _mat->m3, _MM_SHUFFLE(0, 0, 2 ,1));

    tmp0 = _mm_mul_ps(tmp0, tmp1);
    tmp2 = _mm_mul_ps(tmp2, tmp3);
    v0_3 = _mm_sub_ps(tmp0, tmp2);

    tmp4 = _mm_mul_ps(tmp4, tmp5);
    tmp6 = _mm_mul_ps(tmp6, tmp7);
    v4_5 = _mm_sub_ps(tmp4, tmp6);


    tmp2 = _mm_shuffle_ps(ffffffff, ffffffff, _MM_SHUFFLE(0, 1, 1 ,1));
    tmp3 = _mm_shuffle_ps(ffffffff, ffffffff, _MM_SHUFFLE(1, 0, 0 ,0));

    tmp5 = _mm_shuffle_ps(v4_5, v4_5, _MM_SHUFFLE(0, 0, 1 ,1));
    tmp6 = _mm_shuffle_ps(v0_3, v0_3, _MM_SHUFFLE(3, 0, 0 ,0));
    tmp6 = _mm_and_ps(tmp2, tmp6);
    tmp5 = _mm_and_ps(tmp3, tmp5);
    v5543 = _mm_or_ps(tmp6, tmp5);

    tmp2 = _mm_shuffle_ps(ffffffff, ffffffff, _MM_SHUFFLE(0, 0, 0 ,1));
    tmp4 = _mm_shuffle_ps(v0_3, v0_3, _MM_SHUFFLE(1, 2, 2,0));
    tmp5 = _mm_and_ps(ffffffff, v4_5);
    tmp6 = _mm_and_ps(tmp2, tmp4);
    v4221 = _mm_or_ps(tmp5, tmp6);

    m1000 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(0, 0, 0,1));
    m2211 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(1, 1, 2,2));
    m3332 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(2, 3, 3,3));

    v3100 = _mm_shuffle_ps(v0_3, v0_3, _MM_SHUFFLE(0, 0, 1,3));

    tmp0 = _mm_mul_ps(v5543, m1000);
    tmp1 = _mm_mul_ps(v4221, m2211);
    tmp2 = _mm_mul_ps(v3100, m3332);
    tmp0 = _mm_sub_ps(tmp0, tmp1);
    tmp0 = _mm_add_ps(tmp0, tmp2);

    t = _mm_mul_ps(tmp0, dm);

    tmp0 = _mm_mul_ps(t, _mat->m0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);
    tmp0 = _mm_hadd_ps(tmp0, tmp0);

    invDet = _mm_div_ps(one, tmp0);

    d00_10_20_30 = _mm_mul_ps(t, invDet);

    m1000 = _mm_shuffle_ps(_mat->m0, _mat->m0, _MM_SHUFFLE(0, 0, 0,1));
    m2211 = _mm_shuffle_ps(_mat->m0, _mat->m0, _MM_SHUFFLE(1, 1, 2,2));
    m3332 = _mm_shuffle_ps(_mat->m0, _mat->m0, _MM_SHUFFLE(2, 3, 3,3));

    tmp0 = _mm_mul_ps(v5543, m1000);
    tmp1 = _mm_mul_ps(v4221, m2211);
    tmp2 = _mm_mul_ps(v3100, m3332);
    tmp0 = _mm_sub_ps(tmp0, tmp1);
    tmp0 = _mm_add_ps(tmp0, tmp2);

    tmp0 = _mm_mul_ps(tmp0, idm);
    d01_11_21_31 = _mm_mul_ps(tmp0, invDet);
    /************/

    tmp0 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(1, 0, 0 ,0));
    tmp1 = _mm_shuffle_ps(_mat->m3, _mat->m3, _MM_SHUFFLE(2, 3, 2 ,1));
    tmp2 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(2, 3, 2 ,1));
    tmp3 = _mm_shuffle_ps(_mat->m3, _mat->m3, _MM_SHUFFLE(1, 0, 0 ,0));

    tmp4 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(0, 0, 2 ,1));
    tmp5 = _mm_shuffle_ps(_mat->m3, _mat->m3, _MM_SHUFFLE(0, 0, 3 ,3));
    tmp6 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(0, 0, 3 ,3));
    tmp7 = _mm_shuffle_ps(_mat->m3, _mat->m3, _MM_SHUFFLE(0, 0, 2 ,1));

    tmp0 = _mm_mul_ps(tmp0, tmp1);
    tmp2 = _mm_mul_ps(tmp2, tmp3);
    v0_3 = _mm_sub_ps(tmp0, tmp2);

    tmp4 = _mm_mul_ps(tmp4, tmp5);
    tmp6 = _mm_mul_ps(tmp6, tmp7);
    v4_5 = _mm_sub_ps(tmp4, tmp6);

    tmp2 = _mm_shuffle_ps(ffffffff, ffffffff, _MM_SHUFFLE(0, 1, 1 ,1));
    tmp3 = _mm_shuffle_ps(ffffffff, ffffffff, _MM_SHUFFLE(1, 0, 0 ,0));
    tmp5 = _mm_shuffle_ps(v4_5, v4_5, _MM_SHUFFLE(0, 0, 1 ,1));
    tmp6 = _mm_shuffle_ps(v0_3, v0_3, _MM_SHUFFLE(3, 0, 0 ,0));
    tmp6 = _mm_and_ps(tmp2, tmp6);
    tmp5 = _mm_and_ps(tmp3, tmp5);
    v5543 = _mm_or_ps(tmp6, tmp5);
    tmp2 = _mm_shuffle_ps(ffffffff, ffffffff, _MM_SHUFFLE(0, 0, 0 ,1));
    tmp4 = _mm_shuffle_ps(v0_3, v0_3, _MM_SHUFFLE(1, 2, 2,0));
    tmp5 = _mm_and_ps(ffffffff, v4_5);
    tmp6 = _mm_and_ps(tmp2, tmp4);
    v4221 = _mm_or_ps(tmp5, tmp6);
    v3100 = _mm_shuffle_ps(v0_3, v0_3, _MM_SHUFFLE(0, 0, 1,3));

    tmp0 = _mm_mul_ps(v5543, m1000);
    tmp1 = _mm_mul_ps(v4221, m2211);
    tmp2 = _mm_mul_ps(v3100, m3332);
    tmp0 = _mm_sub_ps(tmp0, tmp1);
    tmp0 = _mm_add_ps(tmp0, tmp2);

    tmp0 = _mm_mul_ps(tmp0, dm);
    d02_12_22_32 = _mm_mul_ps(tmp0, invDet);

    /************/

    tmp0 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(1, 0, 0 ,0));
    tmp1 = _mm_shuffle_ps(_mat->m2, _mat->m2, _MM_SHUFFLE(2, 3, 2 ,1));
    tmp2 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(2, 3, 2 ,1));
    tmp3 = _mm_shuffle_ps(_mat->m2, _mat->m2, _MM_SHUFFLE(1, 0, 0 ,0));

    tmp4 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(0, 0, 2 ,1));
    tmp5 = _mm_shuffle_ps(_mat->m2, _mat->m2, _MM_SHUFFLE(0, 0, 3 ,3));
    tmp6 = _mm_shuffle_ps(_mat->m1, _mat->m1, _MM_SHUFFLE(0, 0, 3 ,3));
    tmp7 = _mm_shuffle_ps(_mat->m2, _mat->m2, _MM_SHUFFLE(0, 0, 2 ,1));

    tmp0 = _mm_mul_ps(tmp0, tmp1);
    tmp2 = _mm_mul_ps(tmp2, tmp3);
    v0_3 = _mm_sub_ps(tmp0, tmp2);

    tmp4 = _mm_mul_ps(tmp4, tmp5);
    tmp6 = _mm_mul_ps(tmp6, tmp7);
    v4_5 = _mm_sub_ps(tmp4, tmp6);

    tmp2 = _mm_shuffle_ps(ffffffff, ffffffff, _MM_SHUFFLE(0, 1, 1 ,1));
    tmp3 = _mm_shuffle_ps(ffffffff, ffffffff, _MM_SHUFFLE(1, 0, 0 ,0));
    tmp5 = _mm_shuffle_ps(v4_5, v4_5, _MM_SHUFFLE(0, 0, 1 ,1));
    tmp6 = _mm_shuffle_ps(v0_3, v0_3, _MM_SHUFFLE(3, 0, 0 ,0));
    tmp6 = _mm_and_ps(tmp2, tmp6);
    tmp5 = _mm_and_ps(tmp3, tmp5);
    v5543 = _mm_or_ps(tmp6, tmp5);
    tmp2 = _mm_shuffle_ps(ffffffff, ffffffff, _MM_SHUFFLE(0, 0, 0 ,1));
    tmp4 = _mm_shuffle_ps(v0_3, v0_3, _MM_SHUFFLE(1, 2, 2,0));
    tmp5 = _mm_and_ps(ffffffff, v4_5);
    tmp6 = _mm_and_ps(tmp2, tmp4);
    v4221 = _mm_or_ps(tmp5, tmp6);
    v3100 = _mm_shuffle_ps(v0_3, v0_3, _MM_SHUFFLE(0, 0, 1,3));

    tmp0 = _mm_mul_ps(v5543, m1000);
    tmp1 = _mm_mul_ps(v4221, m2211);
    tmp2 = _mm_mul_ps(v3100, m3332);
    tmp0 = _mm_sub_ps(tmp0, tmp1);
    tmp0 = _mm_add_ps(tmp0, tmp2);

    tmp0 = _mm_mul_ps(tmp0, idm);
    d03_13_23_33 = _mm_mul_ps(tmp0, invDet);

    float* d = (float*)&_mat->m0;
    float* s0 = (float*)&d00_10_20_30;
    float* s1 = (float*)&d01_11_21_31;
    float* s2 = (float*)&d02_12_22_32;
    float* s3 = (float*)&d03_13_23_33;
    d[0] = s0[0];
    d[1] = s1[0];
    d[2] = s2[0];
    d[3] = s3[0];

    d = (float*)&_mat->m1;

    d[0] = s0[1];
    d[1] = s1[1];
    d[2] = s2[1];
    d[3] = s3[1];

    d = (float*)&_mat->m2;

    d[0] = s0[2];
    d[1] = s1[2];
    d[2] = s2[2];
    d[3] = s3[2];

    d = (float*)&_mat->m3;

    d[0] = s0[3];
    d[1] = s1[3];
    d[2] = s2[3];
    d[3] = s3[3];
#else
    float m00 = _mat->m[0][0], m01 = _mat->m[0][1], m02 = _mat->m[0][2], m03 = _mat->m[0][3];
    float m10 = _mat->m[1][0], m11 = _mat->m[1][1], m12 = _mat->m[1][2], m13 = _mat->m[1][3];
    float m20 = _mat->m[2][0], m21 = _mat->m[2][1], m22 = _mat->m[2][2], m23 = _mat->m[2][3];
    float m30 = _mat->m[3][0], m31 = _mat->m[3][1], m32 = _mat->m[3][2], m33 = _mat->m[3][3];

    float v0 = m20 * m31 - m21 * m30;
    float v1 = m20 * m32 - m22 * m30;
    float v2 = m20 * m33 - m23 * m30;
    float v3 = m21 * m32 - m22 * m31;
    float v4 = m21 * m33 - m23 * m31;
    float v5 = m22 * m33 - m23 * m32;

    float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
    float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
    float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
    float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

    float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

    float d00 = t00 * invDet;
    float d10 = t10 * invDet;
    float d20 = t20 * invDet;
    float d30 = t30 * invDet;

    float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;

    float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;

    float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    _mat->m[0][0] = d00;
    _mat->m[0][1] = d01;
    _mat->m[0][2] = d02;
    _mat->m[0][3] = d03;
    _mat->m[1][0] = d10;
    _mat->m[1][1] = d11;
    _mat->m[1][2] = d12;
    _mat->m[1][3] = d13;
    _mat->m[2][0] = d20;
    _mat->m[2][1] = d21;
    _mat->m[2][2] = d22;
    _mat->m[2][3] = d23;
    _mat->m[3][0] = d30;
    _mat->m[3][1] = d31;
    _mat->m[3][2] = d32;
    _mat->m[3][3] = d33;
#endif

}

void Matrix4x4_print(matrix4x4* _mat)
{
#ifdef USE_SSE
#ifndef _MSC_VER
    float __attribute__((aligned(16))) tempf[4];
    _mm_store_ps(tempf,_mat->m0);
    printf("\n%f %f %f %f", tempf[0],tempf[1],tempf[2],tempf[3]);
    _mm_store_ps(tempf,_mat->m1);
    printf("\n%f %f %f %f", tempf[0],tempf[1],tempf[2],tempf[3]);
    _mm_store_ps(tempf,_mat->m2);
    printf("\n%f %f %f %f", tempf[0],tempf[1],tempf[2],tempf[3]);
    _mm_store_ps(tempf,_mat->m3);
    printf("\n%f %f %f %f", tempf[0],tempf[1],tempf[2],tempf[3]);
    printf("\n");
#else
#endif
#else
    printf("\n%f %f %f %f", _mat->m[0][0],_mat->m[0][1],_mat->m[0][2],_mat->m[0][3]);
    printf("\n%f %f %f %f", _mat->m[1][0],_mat->m[1][1],_mat->m[1][2],_mat->m[1][3]);
    printf("\n%f %f %f %f", _mat->m[2][0],_mat->m[2][1],_mat->m[2][2],_mat->m[2][3]);
    printf("\n%f %f %f %f", _mat->m[3][0],_mat->m[3][1],_mat->m[3][2],_mat->m[3][3]);
    printf("\n");
#endif
}

void Matrix4x4_log(matrix4x4* _mat)
{
#ifdef USE_SSE
#ifndef _MSC_VER
    float __attribute__((aligned(16))) tempf[4];
    _mm_store_ps(tempf,_mat->m0);
    elog("\n%f %f %f %f", tempf[0],tempf[1],tempf[2],tempf[3]);
    _mm_store_ps(tempf,_mat->m1);
    elog("\n%f %f %f %f", tempf[0],tempf[1],tempf[2],tempf[3]);
    _mm_store_ps(tempf,_mat->m2);
    elog("\n%f %f %f %f", tempf[0],tempf[1],tempf[2],tempf[3]);
    _mm_store_ps(tempf,_mat->m3);
    elog("\n%f %f %f %f", tempf[0],tempf[1],tempf[2],tempf[3]);
    elog("%s", "\n");
#else
	elog("\n%f %f %f %f", _mat->m0.m128_f32[0],_mat->m0.m128_f32[1],_mat->m0.m128_f32[2],_mat->m0.m128_f32[3]);
	elog("\n%f %f %f %f", _mat->m1.m128_f32[0],_mat->m1.m128_f32[1],_mat->m1.m128_f32[2],_mat->m1.m128_f32[3]);
	elog("\n%f %f %f %f", _mat->m2.m128_f32[0],_mat->m2.m128_f32[1],_mat->m2.m128_f32[2],_mat->m2.m128_f32[3]);
	elog("\n%f %f %f %f", _mat->m3.m128_f32[0],_mat->m3.m128_f32[1],_mat->m3.m128_f32[2],_mat->m3.m128_f32[3]);
	elog("%s", "\n");
#endif
#else
    elog("\n%f %f %f %f", _mat->m[0][0],_mat->m[0][1],_mat->m[0][2],_mat->m[0][3]);
    elog("\n%f %f %f %f", _mat->m[1][0],_mat->m[1][1],_mat->m[1][2],_mat->m[1][3]);
    elog("\n%f %f %f %f", _mat->m[2][0],_mat->m[2][1],_mat->m[2][2],_mat->m[2][3]);
    elog("\n%f %f %f %f", _mat->m[3][0],_mat->m[3][1],_mat->m[3][2],_mat->m[3][3]);
    elog("\n");
#endif
}

void Matrix4x4_set_x_axis_rotate(matrix4x4* _mat, float _r)
{
#ifdef USE_SSE
    float msin = sin(_r);
    float mcos = cos(_r);

    _mat->m0 = _mm_setr_ps(1.0, 0.0, 0.0, 0.0);
    _mat->m1 = _mm_setr_ps(0.0, mcos, -msin, 0.0);
    _mat->m2 = _mm_setr_ps(0.0, msin, mcos, 0.0);
    _mat->m3 = _mm_setr_ps(0.0, 0.0, 0.0, 1.0);
#else
    float msin = sin(_r);
    float mcos = cos(_r);
    _mat->m[0][0] = 1.0;
    _mat->m[0][1] = 0.0;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = 0.0;
    _mat->m[1][1] = mcos;
    _mat->m[1][2] = -msin;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = 0.0;
    _mat->m[2][1] = msin;
    _mat->m[2][2] = mcos;
    _mat->m[2][3] = 0.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = 0.0;
    _mat->m[3][3] = 1.0;
#endif
}

void Matrix4x4_set_y_axis_rotate(matrix4x4* _mat, float _r)
{
#ifdef USE_SSE
    float msin = sin(_r);
    float mcos = cos(_r);

    _mat->m0 = _mm_setr_ps(mcos, 0.0, msin, 0.0);
    _mat->m1 = _mm_setr_ps(0.0, 1.0, 0.0, 0.0);
    _mat->m2 = _mm_setr_ps(-msin, 0.0, mcos, 0.0);
    _mat->m3 = _mm_setr_ps(0.0, 0.0, 0.0, 1.0);
#else
    float msin = sin(_r);
    float mcos = cos(_r);
    _mat->m[0][0] = mcos;
    _mat->m[0][1] = msin;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = 0.0;
    _mat->m[1][1] = 1.0;
    _mat->m[1][2] = 0.0;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = -msin;
    _mat->m[2][1] = 0.0;
    _mat->m[2][2] = mcos;
    _mat->m[2][3] = 0.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = 0.0;
    _mat->m[3][3] = 1.0;
#endif
}

void Matrix4x4_set_z_axis_rotate(matrix4x4* _mat, float _r)
{
#ifdef USE_SSE
    float msin = sin(_r);
    float mcos = cos(_r);

    _mat->m0 = _mm_setr_ps(mcos, -msin, 0.0, 0.0);
    _mat->m1 = _mm_setr_ps(msin, mcos, 0.0, 0.0);
    _mat->m2 = _mm_setr_ps(0.0, 0.0, 1.0, 0.0);
    _mat->m3 = _mm_setr_ps(0.0, 0.0, 0.0, 1.0);
#else
    float msin = sin(_r);
    float mcos = cos(_r);
    _mat->m[0][0] = mcos;
    _mat->m[0][1] = -msin;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = msin;
    _mat->m[1][1] = mcos;
    _mat->m[1][2] = 0.0;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = 0.0;
    _mat->m[2][1] = 0.0;
    _mat->m[2][2] = 1.0;
    _mat->m[2][3] = 0.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = 0.0;
    _mat->m[3][3] = 1.0;
#endif
}

void Matrix4x4_set_scale(matrix4x4* _mat, float x, float y, float z)
{
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps(x,0.0,0.0,0.0);
    _mat->m1 = _mm_setr_ps(0.0,y,0.0,0.0);
    _mat->m2 = _mm_setr_ps(0.0,0.0,z,0.0);
    _mat->m3 = _mm_setr_ps(0.0,0.0,0.0,1.0);
#else
#endif
}

sfloat4 Matrix4x4_mul_float4(const matrix4x4* _mat, sfloat4 _ft)///__m128& point, matrix4x4*& matrix, __m128& result)
{
#ifdef USE_SSE
    ///ALIGN_Value4(__m128, temp0, temp1, temp2, temp3);
    __m128 point = _ft;

    __m128 temp0 = _mm_mul_ps(point, _mat->m0);
    __m128 temp1 = _mm_mul_ps(point, _mat->m1);
    __m128 temp2 = _mm_mul_ps(point, _mat->m2);
    __m128 temp3 = _mm_mul_ps(point, _mat->m3);

    temp0 = _mm_hadd_ps(temp0, temp0);
    temp0 = _mm_hadd_ps(temp0, temp0);
    temp1 = _mm_hadd_ps(temp1, temp1);
    temp1 = _mm_hadd_ps(temp1, temp1);
    temp2 = _mm_hadd_ps(temp2, temp2);
    temp2 = _mm_hadd_ps(temp2, temp2);
    temp3 = _mm_hadd_ps(temp3, temp3);
    temp3 = _mm_hadd_ps(temp3, temp3);

    temp0 = _mm_and_ps(temp0, *((__m128*)g_float3_mask));
    temp0 = _mm_shuffle_ps(temp0, temp0, _MM_SHUFFLE(3, 3, 3, 0));

    temp1 = _mm_and_ps(temp1, *((__m128*)g_float3_mask));
    temp1 = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(3, 3, 0, 3));

    temp2 = _mm_and_ps(temp2, *((__m128*)g_float3_mask));
    temp2 = _mm_shuffle_ps(temp2, temp2, _MM_SHUFFLE(3, 0, 3, 3));

    temp3 = _mm_and_ps(temp3, *((__m128*)g_float3_mask));
    temp3 = _mm_shuffle_ps(temp3, temp3, _MM_SHUFFLE(0, 3, 3, 3));

    __m128 result;
#ifdef _MSC_VER
	result = _mm_setzero_ps();
#else
    result = _mm_xor_ps(result, result);
#endif
    result = _mm_or_ps(result, temp0);
    result = _mm_or_ps(result, temp1);
    result = _mm_or_ps(result, temp2);
    result = _mm_or_ps(result, temp3);
    return result;
#else
    __m128 result;
    float* _rf = (float*)&result;
    float* _pf = (float*)_ft;

    _rf[0] = _pf[0] * _mat->m[0][0] + _pf[1] * _mat->m[0][1] + _pf[2] * _mat->m[0][2] + _pf[3] * _mat->m[0][3];
    _rf[1] = _pf[0] * _mat->m[1][0] + _pf[1] * _mat->m[1][1] + _pf[2] * _mat->m[1][2] + _pf[3] * _mat->m[1][3];
    _rf[2] = _pf[0] * _mat->m[2][0] + _pf[1] * _mat->m[2][1] + _pf[2] * _mat->m[2][2] + _pf[3] * _mat->m[2][3];
    _rf[3] = _pf[0] * _mat->m[3][0] + _pf[1] * _mat->m[3][1] + _pf[2] * _mat->m[3][2] + _pf[3] * _mat->m[3][3];

    return result;
#endif
}

sfloat3 Matrix4x4_mul_float3(const matrix4x4* _mat, sfloat3 _ft)
{
    sfloat4 tmp = SFloat4_assign(_ft);
    tmp = Matrix4x4_mul_float4(_mat, tmp);
    return SFloat4_xyz(tmp);
}
/**
Matrix4x4Array Matrix4x4Array_new()
{
#ifdef USE_SSE
    matrix4x4 mat4exec;
    mat4exec.m0 = _mm_set1_ps(fexce);
    mat4exec.m1 = _mm_set1_ps(fexce);
    mat4exec.m2 = _mm_set1_ps(fexce);
    mat4exec.m3 = _mm_set1_ps(fexce);

    Matrix4x4Array ret;
    ret.self = array_new(matrix4x4, 4, mat4exec);
    return ret;
#else
#endif
}

void Matrix4x4Array_delete(Matrix4x4Array _m4_array)
{
    array_delete(_m4_array.self);
}

void Matrix4x4Array_push(Matrix4x4Array _m4_array, matrix4x4* _mt4)
{
    _m4_array.self = array_push(_m4_array.self, *_mt4);
}

matrix4x4* Matrix4x4Array_pop(Matrix4x4Array _m4_array)
{
    matrix4x4* ret = Matrix4x4_new();
    *ret = array_pop(_m4_array.self);
    return ret;
}

matrix4x4* Matrix4x4Array_get(Matrix4x4Array _m4_array, uint32 _i)
{
    return array_safe_get_ptr(_m4_array.self, _i);
}

uint32 Matrix4x4Array_get_size(Matrix4x4Array _m4_array)
{
    return array_n(_m4_array.self);
}

float* Matrix4x4Array_get_ptr(Matrix4x4Array _m4_array)
{
    return (float*)_m4_array.self;
}

void Matrix4x4Array_export(Matrix4x4Array _m4_array, float* _ret)
{
    memcpy(_ret, _m4_array.self, array_n(_m4_array.self) * sizeof(matrix4x4));
}
**/

void Matrix4x4_set_right_z_axis(matrix4x4* _mat)
{
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps(0,-1.0,0.0,0.0);
    _mat->m1 = _mm_setr_ps(1.0,0,0.0,0.0);
    _mat->m2 = _mm_setr_ps(0.0,0.0,1.0,0.0);
    _mat->m3 = _mm_setr_ps(0.0,0.0,0.0,1.0);
#else
    _mat->m[0][0] = 0.0;
    _mat->m[0][1] = -1.0;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = 1.0;
    _mat->m[1][1] = 0.0;
    _mat->m[1][2] = 0.0;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = 0.0;
    _mat->m[2][1] = 0.0;
    _mat->m[2][2] = 1,0;
    _mat->m[2][3] = 0.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = 0.0;
    _mat->m[3][3] = 1.0;
#endif
}

void Matrix4x4_set_right_y_axis(matrix4x4* _mat)
{
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps(0,0.0,-1.0,0.0);
    _mat->m1 = _mm_setr_ps(0.0,1.0,0,0.0);
    _mat->m2 = _mm_setr_ps(1.0,0.0,0.0,0.0);
    _mat->m3 = _mm_setr_ps(0.0,0.0,0.0,1.0);
#else
    _mat->m[0][0] = 0.0;
    _mat->m[0][1] = 0.0;
    _mat->m[0][2] = -1.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = 0.0;
    _mat->m[1][1] = 1.0;
    _mat->m[1][2] = 0.0;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = 1.0;
    _mat->m[2][1] = 0.0;
    _mat->m[2][2] = 0.0;
    _mat->m[2][3] = 0.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = 0.0;
    _mat->m[3][3] = 1.0;
#endif
}

void Matrix4x4_set_right_x_axis(matrix4x4* _mat)
{
#ifdef USE_SSE
    _mat->m0 = _mm_setr_ps(1.0,0.0,0.0,0.0);
    _mat->m1 = _mm_setr_ps(0.0,0.0,1.0,0.0);
    _mat->m2 = _mm_setr_ps(0.0,-1.0,0.0,0.0);
    _mat->m3 = _mm_setr_ps(0.0,0.0,0.0,1.0);
#else
    _mat->m[0][0] = 1.0;
    _mat->m[0][1] = 0.0;
    _mat->m[0][2] = 0.0;
    _mat->m[0][3] = 0.0;

    _mat->m[1][0] = 0.0;
    _mat->m[1][1] = 0.0;
    _mat->m[1][2] = 1.0;
    _mat->m[1][3] = 0.0;

    _mat->m[2][0] = 0.0;
    _mat->m[2][1] = -1.0;
    _mat->m[2][2] = 0.0;
    _mat->m[2][3] = 0.0;

    _mat->m[3][0] = 0.0;
    _mat->m[3][1] = 0.0;
    _mat->m[3][2] = 0.0;
    _mat->m[3][3] = 1.0;
#endif
}
/**
void EtGeometryUtil::MakeRotationFromPositiveAxis(NiMatrix3& mat, const NiPoint3& destDirection, EtAxis axis)
{
	float destLength = destDirection.Length();
	if (destLength == 0.0f)
	{
		mat.MakeIdentity();
		return;
	}
	NiPoint3 unitDestDirection = destDirection / destLength;

	NiPoint3 rotAxis;
	NiPoint3 right;
	float d = 0.0f;
	switch (axis)
	{
	case EtXAxis:
		rotAxis = unitDestDirection.UnitCross(NiPoint3::UNIT_X);
		d = unitDestDirection.Dot(NiPoint3::UNIT_X);
		right = NiPoint3::UNIT_X;
		break;
	case EtYAxis:
		rotAxis = unitDestDirection.UnitCross(NiPoint3::UNIT_Y);
		d = unitDestDirection.Dot(NiPoint3::UNIT_Y);
		right = NiPoint3::UNIT_Y;
		break;
	case EtZAxis:
		rotAxis = unitDestDirection.UnitCross(NiPoint3::UNIT_Z);
		d = unitDestDirection.Dot(NiPoint3::UNIT_Z);
		right = NiPoint3::UNIT_Z;
		break;
	default:
        mat.MakeIdentity();
		return;
	}

	float rad = NiACos( d );

	mat.MakeRotation( rad, rotAxis );
	NiMatrix3 rightMat;
	rightMat.MakeRotation( -NI_PI * 0.5f, rotAxis );
	right = rightMat * right;
	if (right.Dot(unitDestDirection) < 0.0f)
	{
		mat.MakeRotation( -rad, rotAxis );
	}
}
**/
