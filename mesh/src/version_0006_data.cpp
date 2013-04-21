#include "version_0006_data.h"
#include "version_0005_data.h"
#include "emem.h"
void version_0006_Dest(struct version_0006_data* data)
{
    version_0005_Dest(&data->base_data);
    if (data->vtx_epw)
        Mfree(data->vtx_epw);
}

void version_0006_load(FILE* fp, struct version_0006_data* data)
{
    version_0005_load(fp, &data->base_data);
}

void version_0006_apply_transform(struct version_0006_data* data, const matrix4x4* trans)
{
	version_0005_apply_transform(&data->base_data, trans);
}