#include <matrix4x3.h>
typedef struct _matrix4x3
{
#ifdef USE_SSE
    __m128 m0;
    __m128 m1;
    __m128 m2;
#else
    float m[3][4];
#endif
} matrix4x3;

void Matrix4x3_export(Matrix4x3 _mat, float* _ret)
{
    matrix4x3* mat = (matrix4x3*)_mat;
#ifdef USE_SSE
    _ret[0] = ((float*)&mat->m0)[0];
    _ret[1] = ((float*)&mat->m0)[1];
    _ret[2] = ((float*)&mat->m0)[2];
    _ret[3] = ((float*)&mat->m0)[3];

    _ret[4] = ((float*)&mat->m1)[0];
    _ret[5] = ((float*)&mat->m1)[1];
    _ret[6] = ((float*)&mat->m1)[2];
    _ret[7] = ((float*)&mat->m1)[3];

    _ret[8] = ((float*)&mat->m2)[0];
    _ret[9] = ((float*)&mat->m2)[1];
    _ret[10] = ((float*)&mat->m2)[2];
    _ret[11] = ((float*)&mat->m2)[3];
#else
    _ret[0] = mat->m[0][0];
    _ret[1] = mat->m[0][1];
    _ret[2] = mat->m[0][2];
    _ret[3] = mat->m[0][3];

    _ret[4] = mat->m[1][0];
    _ret[5] = mat->m[1][1];
    _ret[6] = mat->m[1][2];
    _ret[7] = mat->m[1][3];

    _ret[8] = mat->m[2][0];
    _ret[9] = mat->m[2][1];
    _ret[10] = mat->m[2][2];
    _ret[11] = mat->m[2][3];
#endif
}
