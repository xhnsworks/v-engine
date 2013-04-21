#ifndef VERSION_0007_DATA_H
#define VERSION_0007_DATA_H
#include "version_0006_data.h"
struct version_0007_data
{
    struct version_0006_data base_data;
    float* vtx_col;
};
void version_0007_Dest(struct version_0007_data* data);
void version_0007_load(FILE* fp, struct version_0007_data* data);
void version_0007_apply_transform(struct version_0007_data* data, const matrix4x4* trans);
#endif
