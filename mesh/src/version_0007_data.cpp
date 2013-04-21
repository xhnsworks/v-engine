#include "version_0007_data.h"
#include "emem.h"
void version_0007_Dest(struct version_0007_data* data)
{
    version_0006_Dest(&data->base_data);
    if (data->vtx_col)
        Mfree(data->vtx_col);
}
void version_0007_load(FILE* fp, struct version_0007_data* data)
{
    version_0006_load(fp, &data->base_data);
}
void version_0007_apply_transform(struct version_0007_data* data, const matrix4x4* trans)
{
	version_0006_apply_transform(&data->base_data, trans);
}