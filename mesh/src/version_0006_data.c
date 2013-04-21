#include "version_0006_data.h"
#include "version_0005_data.h"
#include "mem.h"
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
