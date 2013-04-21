#include "mesh.h"
#include "recycle_bin.h"
#include "elog.h"
#include "float3.h"
typedef struct _mesh
{
    uint32 version;
    uint32 face_count;
    uint32 face_vtx_count;
    uint32* face_vtx_idx;
    uint32 vtx_count;
    float* vtx_pos;
    float* vtx_nor;
    float* vtx_uv;
    float* vtx_tgt;
    float* vtx_binor;
    uint32 has_uv;
}mesh;
void Mesh_Dest(struct _mesh* _mesh)
{
    if (_mesh->face_vtx_idx)
        Mfree( (vptr)_mesh->face_vtx_idx );
    if (_mesh->vtx_pos)
        Mfree( (vptr)_mesh->vtx_pos );
    if (_mesh->vtx_nor)
        Mfree( (vptr)_mesh->vtx_nor );
    if (_mesh->vtx_uv)
        Mfree( (vptr)_mesh->vtx_uv );
    if (_mesh->vtx_tgt)
        Mfree( (vptr)_mesh->vtx_tgt );
    if (_mesh->vtx_binor)
        Mfree( (vptr)_mesh->vtx_binor );

    memset(_mesh, 0, sizeof(mesh));
}
Mesh Mesh_new()
{
    Mesh ret;
    ret.self = Malloc(sizeof(mesh));
    memset(ret.self, 0, sizeof(mesh));
    return ret;
}
void Mesh_delete(Mesh _mesh)
{
    Mesh_Dest(_mesh.self);
    Mfree(_mesh.self);
}
float* Mesh_get_position(Mesh _mesh)
{
    return _mesh.self->vtx_pos;
}
float* Mesh_get_tex_coord(Mesh _mesh)
{
    return _mesh.self->vtx_uv;
}
float* Mesh_get_normal(Mesh _mesh)
{
    return _mesh.self->vtx_nor;
}
float* Mesh_get_tangent(Mesh _mesh)
{
    return _mesh.self->vtx_tgt;
}
float* Mesh_get_binormal(Mesh _mesh)
{
    return _mesh.self->vtx_binor;
}
uint32* Mesh_get_index(Mesh _mesh)
{
    return _mesh.self->face_vtx_idx;
}
uint32 Mesh_get_vertex_count(Mesh _mesh)
{
    return _mesh.self->vtx_count;
}
uint32 Mesh_get_face_count(Mesh _mesh)
{
    return _mesh.self->face_count;
}
///============================================================================================///
void Mesh_tangent_calculate(Mesh _mesh);
void Mesh_load_form_file(Mesh _mesh, const char* _file_name)
{
    FILE* fp = fopen(_file_name, "rb");;
    char id[4];
    id[0] = getc(fp);
    id[1] = getc(fp);
    id[2] = getc(fp);
    id[3] = getc(fp);
    if (id[0] != 'O' || id[1] != 'G' || id[2] != 'L')
    {
        return;
    }

    uint32 word;
    /// Version

    fread(&word, sizeof(uint32), 1, fp);
    _mesh.self->version = word;
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
    _mesh.self->face_count = word / 3;
    _mesh.self->face_vtx_count = word;

    /// GL_TRIANGLES and Material Id;

    fread(&word, sizeof(uint32), 1, fp);

    _mesh.self->face_vtx_idx = (uint32*)Malloc(sizeof(uint32) * _mesh.self->face_vtx_count);

    uint i = 0;
    for (; i < _mesh.self->face_count; i++)
    {
        fread(&word, sizeof(uint32), 1, fp);
        _mesh.self->face_vtx_idx[i * 3] = word;
        fread(&word, sizeof(uint32), 1, fp);
        _mesh.self->face_vtx_idx[i * 3 + 1] = word;
        fread(&word, sizeof(uint32), 1, fp);
        _mesh.self->face_vtx_idx[i * 3 + 2] = word;
    }

    /// OGL_OBJECT_INDEXED
    fread(&word, sizeof(uint32), 1, fp);
    /// OGL_OBJECT_COORDINATES
    fread(&word, sizeof(uint32), 1, fp);

    /// vertex count
    fread(&word, sizeof(uint32), 1, fp);
    _mesh.self->vtx_count = word;
    /// GL_TRIANGLES and Material Id;
    fread(&word, sizeof(uint32), 1, fp);

    double dbword;
    _mesh.self->vtx_pos = (float*)Malloc(sizeof(float) * 3 * _mesh.self->vtx_count);
    for (i = 0; i < _mesh.self->vtx_count; i++)
    {
        fread(&dbword, sizeof(double), 1, fp);
        _mesh.self->vtx_pos[i * 3] = (float)dbword;
        fread(&dbword, sizeof(double), 1, fp);
        _mesh.self->vtx_pos[i * 3 + 1] = (float)dbword;
        fread(&dbword, sizeof(double), 1, fp);
        _mesh.self->vtx_pos[i * 3 + 2] = (float)dbword;
    }

    /// OGL_OBJECT_INDEXED
    fread(&word, sizeof(uint32), 1, fp);
    /// OGL_OBJECT_NORMALS
    fread(&word, sizeof(uint32), 1, fp);
    /// vertex count
    fread(&word, sizeof(uint32), 1, fp);
    /// GL_TRIANGLES and Material Id;
    fread(&word, sizeof(uint32), 1, fp);

    _mesh.self->vtx_nor = (float*)Malloc(sizeof(float) * 3 * _mesh.self->vtx_count);
    for (i = 0; i < _mesh.self->vtx_count; i++)
    {
        fread(&dbword, sizeof(double), 1, fp);
        _mesh.self->vtx_nor[i * 3] = (float)dbword;
        fread(&dbword, sizeof(double), 1, fp);
        _mesh.self->vtx_nor[i * 3 + 1] = (float)dbword;
        fread(&dbword, sizeof(double), 1, fp);
        _mesh.self->vtx_nor[i * 3 + 2] = (float)dbword;
    }

    _mesh.self->has_uv = 1;
    /// OGL_OBJECT_INDEXED
    fread(&word, sizeof(uint32), 1, fp);
    if (word != 0x21)
    {
        _mesh.self->has_uv = 0;
    }
    /// OGL_OBJECT_TEXTURE
    fread(&word, sizeof(uint32), 1, fp);
    if (word != 0x3a)
    {
        _mesh.self->has_uv = 0;
    }

    if (_mesh.self->has_uv)
    {
        /// vertex count
        fread(&word, sizeof(uint32), 1, fp);
        /// GL_TRIANGLES and Material Id;
        fread(&word, sizeof(uint32), 1, fp);
        _mesh.self->vtx_uv = (float*)Malloc(sizeof(float) * 2 * _mesh.self->vtx_count);
        for (i = 0; i < _mesh.self->vtx_count; i++)
        {
            fread(&dbword, sizeof(double), 1, fp);
            _mesh.self->vtx_uv[i * 2] = dbword;
            fread(&dbword, sizeof(double), 1, fp);
            _mesh.self->vtx_uv[i * 2 + 1] = 1.0 + dbword;
        }

        Mesh_tangent_calculate(_mesh);
    }
    ///============================================================================================///
    printf("##vtx count %d\n", _mesh.self->vtx_count);
}

void Mesh_tangent_calculate(Mesh _mesh)
{
    _mesh.self->vtx_tgt = (float*)Malloc(sizeof(float) * 3 * _mesh.self->vtx_count);
    memset(_mesh.self->vtx_tgt, 0, sizeof(float) * 3 * _mesh.self->vtx_count);

    uint32 i0, i1, i2;
    float x0, x1, y0, y1, z0, z1, s0, s1, t0, t1;
    for (uint a = 0; a < _mesh.self->face_count; a++)
    {
        i0 = _mesh.self->face_vtx_idx[a * 3];
        i1 = _mesh.self->face_vtx_idx[a * 3 + 1];
        i2 = _mesh.self->face_vtx_idx[a * 3 + 2];

        float* v0 = &_mesh.self->vtx_pos[i0 * 3];
        float* v1 = &_mesh.self->vtx_pos[i1 * 3];
        float* v2 = &_mesh.self->vtx_pos[i2 * 3];

        float* w0 = &_mesh.self->vtx_uv[i0 * 2];
        float* w1 = &_mesh.self->vtx_uv[i1 * 2];
        float* w2 = &_mesh.self->vtx_uv[i2 * 2];

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

        _mesh.self->vtx_tgt[i0 * 3    ] += (s0 * x1 - s1 * x0) * r;
        _mesh.self->vtx_tgt[i0 * 3 + 1] += (s0 * y1 - s1 * y0) * r;
        _mesh.self->vtx_tgt[i0 * 3 + 2] += (s0 * z1 - s1 * z0) * r;

        _mesh.self->vtx_tgt[i1 * 3    ] += (s0 * x1 - s1 * x0) * r;
        _mesh.self->vtx_tgt[i1 * 3 + 1] += (s0 * y1 - s1 * y0) * r;
        _mesh.self->vtx_tgt[i1 * 3 + 2] += (s0 * z1 - s1 * z0) * r;

        _mesh.self->vtx_tgt[i2 * 3    ] += (s0 * x1 - s1 * x0) * r;
        _mesh.self->vtx_tgt[i2 * 3 + 1] += (s0 * y1 - s1 * y0) * r;
        _mesh.self->vtx_tgt[i2 * 3 + 2] += (s0 * z1 - s1 * z0) * r;

    }
    ///Float_Init();
    _mesh.self->vtx_binor = (float*)Malloc(sizeof(float) * 3 * _mesh.self->vtx_count);
    for (uint a = 0; a < _mesh.self->face_count; a++)
    {
        float x = _mesh.self->vtx_tgt[a * 3];
        float y = _mesh.self->vtx_tgt[a * 3 + 1];
        float z = _mesh.self->vtx_tgt[a * 3 + 2];
        float3 tgt = Float3(x, y, z);
        tgt = Float3_normalize(tgt);

        x = _mesh.self->vtx_nor[a * 3];
        y = _mesh.self->vtx_nor[a * 3 + 1];
        z = _mesh.self->vtx_nor[a * 3 + 2];

        float3 nor = Float3(x, y, z);
        float3 binor = Float3_cross(tgt, nor);
        tgt = Float3_cross(nor, binor);

        Float3_export(binor, &_mesh.self->vtx_binor[a * 3]);
        Float3_export(tgt, &_mesh.self->vtx_tgt[a * 3]);
    }
}
