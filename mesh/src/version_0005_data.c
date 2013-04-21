#include "version_0005_data.h"

void version_0005_Dest(struct version_0005_data* data)
{
    version_0004_Dest(&data->base_data);
}

void version_0005_load(FILE* fp, struct version_0005_data* data)
{
    version_0004_load(fp, &data->base_data);
    uint32 word;
    /// vertex count
    fread(&word, sizeof(uint32), 1, fp);
    for (mesh_mode m = Triangular; m < MeshModeMax; m++)
    {
        if (m == (mesh_mode)word)
        {
            data->mesh_mode = m;
            break;
        }
    }
    if (data->mesh_mode == Triangular)
    {
        version_0004_tangent_calculate(&data->base_data);
    }
}
