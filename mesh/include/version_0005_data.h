#ifndef VERSION_0005_DATA_H
#define VERSION_0005_DATA_H
#include "version_0004_data.h"
struct version_0005_data
{
    struct version_0004_data base_data;
    e_mesh_mode mesh_mode;
};

void version_0005_Dest(struct version_0005_data* data);
void version_0005_load(FILE* fp, struct version_0005_data* data);
void version_0005_apply_transform(struct version_0005_data* data, const matrix4x4* mat);
#endif
