#include "version_0005_data.h"

void version_0005_Dest(struct version_0005_data* data)
{
    version_0004_Dest(&data->base_data);
}

void version_0005_load(FILE* fp, struct version_0005_data* data)
{
    version_0004_load(fp, &data->base_data);
    euint32 word;
    /// vertex count
    fread(&word, sizeof(euint32), 1, fp);
    ///for (mesh_mode m = Triangular; m < MeshModeMax; m++)
	for (euint32 m = (euint32)Triangular; m < (euint32)MeshModeMax; m++)
    {
        if (m == word)
        {
            data->mesh_mode = (e_mesh_mode)m;
            break;
        }
    }
    if (data->mesh_mode == Triangular)
    {
        version_0004_tangent_calculate(&data->base_data);
    }
}

void version_0005_apply_transform(struct version_0005_data* data, const matrix4x4* trans)
{
    version_0004_apply_transform(&data->base_data, trans);
}