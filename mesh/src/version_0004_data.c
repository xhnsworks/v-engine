#include "version_0004_data.h"
#include "float3.h"
#include "mem.h"
void version_0004_tangent_calculate(struct version_0004_data* data)
{
    data->vtx_tgt = (float*)Malloc(sizeof(float) * 3 * data->vtx_count);

    uint32 i0, i1, i2;
    float x0, x1, y0, y1, z0, z1, s0, s1, t0, t1;
    for (uint a = 0; a < data->face_count; a++)
    {
        i0 = data->indexs[a * 3];
        i1 = data->indexs[a * 3 + 1];
        i2 = data->indexs[a * 3 + 2];

        float* v0 = &data->vtx_pos[i0 * 3];
        float* v1 = &data->vtx_pos[i1 * 3];
        float* v2 = &data->vtx_pos[i2 * 3];

        float* w0 = &data->vtx_uv[i0 * 2];
        float* w1 = &data->vtx_uv[i1 * 2];
        float* w2 = &data->vtx_uv[i2 * 2];

        x0 = v1[0] - v0[0];
        x1 = v2[0] - v0[0];
        y0 = v1[1] - v0[1];
        y1 = v2[1] - v0[1];
        z0 = v1[2] - v0[2];
        z1 = v2[2] - v0[2];

        s0 = w1[0] - w0[0];
        s1 = w2[0] - w0[0];
        t0 = w1[1] - w0[1];
        t1 = w2[1] - w0[1];

        float r = 1.0 / (s0 * t1 - s1 * t0);

        data->vtx_tgt[i0 * 3    ] += (s0 * x1 - s1 * x0) * r;
        data->vtx_tgt[i0 * 3 + 1] += (s0 * y1 - s1 * y0) * r;
        data->vtx_tgt[i0 * 3 + 2] += (s0 * z1 - s1 * z0) * r;

        data->vtx_tgt[i1 * 3    ] += (s0 * x1 - s1 * x0) * r;
        data->vtx_tgt[i1 * 3 + 1] += (s0 * y1 - s1 * y0) * r;
        data->vtx_tgt[i1 * 3 + 2] += (s0 * z1 - s1 * z0) * r;

        data->vtx_tgt[i2 * 3    ] += (s0 * x1 - s1 * x0) * r;
        data->vtx_tgt[i2 * 3 + 1] += (s0 * y1 - s1 * y0) * r;
        data->vtx_tgt[i2 * 3 + 2] += (s0 * z1 - s1 * z0) * r;

    }
    ///Float_Init();
    data->vtx_binor = (float*)Malloc(sizeof(float) * 3 * data->vtx_count);
    for (uint a = 0; a < data->face_count; a++)
    {
        float x = data->vtx_tgt[a * 3];
        float y = data->vtx_tgt[a * 3 + 1];
        float z = data->vtx_tgt[a * 3 + 2];
        float3 tgt = Float3(x, y, z);
        tgt = Float3_normalize(tgt);

        x = data->vtx_nor[a * 3];
        y = data->vtx_nor[a * 3 + 1];
        z = data->vtx_nor[a * 3 + 2];

        float3 nor = Float3(x, y, z);
        float3 binor = Float3_cross(tgt, nor);
        tgt = Float3_cross(nor, binor);

        Float3_export(binor, &data->vtx_binor[a * 3]);
        Float3_export(tgt, &data->vtx_tgt[a * 3]);
    }
}

void version_0004_Dest(struct version_0004_data* data)
{
    if (data->indexs)
        Mfree( (vptr)data->indexs );
    if (data->vtx_pos)
        Mfree( (vptr)data->vtx_pos );
    if (data->vtx_nor)
        Mfree( (vptr)data->vtx_nor );
    if (data->vtx_uv)
        Mfree( (vptr)data->vtx_uv );
    if (data->vtx_tgt)
        Mfree( (vptr)data->vtx_tgt );
    if (data->vtx_binor)
        Mfree( (vptr)data->vtx_binor );
}

void version_0004_load(FILE* fp, struct version_0004_data* data)
{
    uint32 word;
    fread(&word, sizeof(uint32), 1, fp);

    /// Object count
    if (!word)
    {
        return;
    }

    /// Object radius (actually just fill with zero

    fread(&word, sizeof(uint32), 1, fp);

    /// OGL_OBJECT_INDEXED

    fread(&word, sizeof(uint32), 1, fp);
    /// OGL_OBJECT_COORDINATEINDEX

    fread(&word, sizeof(uint32), 1, fp);
    /// vertex count

    fread(&word, sizeof(uint32), 1, fp);
    data->face_count = word / 3;
    data->indexed_vtx_count = word;

    /// GL_TRIANGLES and Material Id;

    fread(&word, sizeof(uint32), 1, fp);

    data->indexs = (uint32*)Malloc(sizeof(uint32) * data->indexed_vtx_count);

    uint i = 0;
    for (; i < data->face_count; i++)
    {
        fread(&word, sizeof(uint32), 1, fp);
        data->indexs[i * 3] = word;
        fread(&word, sizeof(uint32), 1, fp);
        data->indexs[i * 3 + 1] = word;
        fread(&word, sizeof(uint32), 1, fp);
        data->indexs[i * 3 + 2] = word;
    }

    /// OGL_OBJECT_INDEXED
    fread(&word, sizeof(uint32), 1, fp);
    /// OGL_OBJECT_COORDINATES
    fread(&word, sizeof(uint32), 1, fp);

    /// vertex count
    fread(&word, sizeof(uint32), 1, fp);
    data->vtx_count = word;
    /// GL_TRIANGLES and Material Id;
    fread(&word, sizeof(uint32), 1, fp);

    double dbword;
    data->vtx_pos = (float*)Malloc(sizeof(float) * 3 * data->vtx_count);
    for (i = 0; i < data->vtx_count; i++)
    {
        fread(&dbword, sizeof(double), 1, fp);
        data->vtx_pos[i * 3] = (float)dbword;
        fread(&dbword, sizeof(double), 1, fp);
        data->vtx_pos[i * 3 + 1] = (float)dbword;
        fread(&dbword, sizeof(double), 1, fp);
        data->vtx_pos[i * 3 + 2] = (float)dbword;
    }

    /// OGL_OBJECT_INDEXED
    fread(&word, sizeof(uint32), 1, fp);
    /// OGL_OBJECT_NORMALS
    fread(&word, sizeof(uint32), 1, fp);
    /// vertex count
    fread(&word, sizeof(uint32), 1, fp);
    /// GL_TRIANGLES and Material Id;
    fread(&word, sizeof(uint32), 1, fp);

    data->vtx_nor = (float*)Malloc(sizeof(float) * 3 * data->vtx_count);
    for (i = 0; i < data->vtx_count; i++)
    {
        fread(&dbword, sizeof(double), 1, fp);
        data->vtx_nor[i * 3] = (float)dbword;
        fread(&dbword, sizeof(double), 1, fp);
        data->vtx_nor[i * 3 + 1] = (float)dbword;
        fread(&dbword, sizeof(double), 1, fp);
        data->vtx_nor[i * 3 + 2] = (float)dbword;
    }

    data->has_uv = 1;
    /// OGL_OBJECT_INDEXED
    fread(&word, sizeof(uint32), 1, fp);
    if (word != 0x21)
    {
        data->has_uv = 0;
    }
    /// OGL_OBJECT_TEXTURE
    fread(&word, sizeof(uint32), 1, fp);
    if (word != 0x3a)
    {
        data->has_uv = 0;
    }

    if (data->has_uv)
    {
        /// vertex count
        fread(&word, sizeof(uint32), 1, fp);
        /// GL_TRIANGLES and Material Id;
        fread(&word, sizeof(uint32), 1, fp);
        data->vtx_uv = (float*)Malloc(sizeof(float) * 2 * data->vtx_count);
        for (i = 0; i < data->vtx_count; i++)
        {
            fread(&dbword, sizeof(double), 1, fp);
            data->vtx_uv[i * 2] = dbword;
            fread(&dbword, sizeof(double), 1, fp);
            data->vtx_uv[i * 2 + 1] = 1.0 + dbword;
        }
    }
}
