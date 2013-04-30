#ifndef VERSION_0004_DATA_H
#define VERSION_0004_DATA_H
#include "common.h"
#include "etypes.h"
#include "matrix4x4.h"
struct version_0004_data
{
    euint32 face_count;
    euint32 indexed_vtx_count;
    euint32* indexs;
    euint32 vtx_count;
    float* vtx_pos;
    float* vtx_nor;
    float* vtx_uv;
    float* vtx_tgt;
    float* vtx_binor;
    euint32 has_uv;
};
void version_0004_Dest(struct version_0004_data* data);
void version_0004_load(FILE* fp, struct version_0004_data* data);
void version_0004_tangent_calculate(struct version_0004_data* data);
void version_0004_apply_transform(struct version_0004_data* data, const matrix4x4* trans);
#endif
