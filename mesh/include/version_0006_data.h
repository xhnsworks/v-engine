#ifndef VERSION_0006_DATA_H
#define VERSION_0006_DATA_H
#include "version_0005_data.h"
struct version_0006_data
{
    struct version_0005_data base_data;
    float* vtx_epw;
};

void version_0006_Dest(struct version_0006_data* data);
void version_0006_load(FILE* fp, struct version_0006_data* data);
void version_0006_apply_transform(struct version_0006_data* data, const matrix4x4* trans);
#endif
