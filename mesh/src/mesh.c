#include "mesh.h"
#include "elog.h"
#include "float3.h"
#include "mem.h"
#include "mesh_base.h"
#include "mesh_triangle.h"
#include "eassert.h"

void Mesh_Dest(struct _mesh* _mesh)
{
    version_0006_Dest(_Mesh_get_data_0006(_mesh));
}
Mesh Mesh_new()
{
    Mesh ret;
    ret = Malloc(sizeof(mesh));
    ret->ref_count = 1;
    return ret;
}
void Mesh_delete(Mesh _mesh)
{
    _mesh->ref_count--;
    EAssert(_mesh->ref_count >= 0, "mesh's reference count less than zero!");
    if (!_mesh->ref_count)
    {
       Mesh_Dest(_mesh);
       Mfree(_mesh);
    }
}

float* Mesh_get_position(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->vtx_pos;
}
float* Mesh_get_tex_coord(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->vtx_uv;
}
float* Mesh_get_normal(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->vtx_nor;
}
float* Mesh_get_tangent(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->vtx_tgt;
}
float* Mesh_get_binormal(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->vtx_binor;
}
float* Mesh_get_edge_proj_weight(Mesh _mesh)
{
    return _Mesh_get_data_0006(_mesh)->vtx_epw;
}
uint32* Mesh_get_index(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->indexs;
}
uint32 Mesh_get_vertex_count(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->vtx_count;
}
uint32 Mesh_get_face_count(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->face_count;
}
mesh_mode Mesh_get_mode(Mesh _mesh)
{
    return _Mesh_get_data_0005(_mesh)->mesh_mode;
}
uint32 face_size(mesh_mode mesh_mode)
{
    if (mesh_mode == Triangular)
    {
        return sizeof(uint32) * 3;
    }
    else
    {
        return sizeof(uint32) * 2;
    }
}

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
    _mesh->version = word;
    printf("ogl version %d\n", _mesh->version);
    if ((uint16)word == 0x0004)
    {
        version_0004_load(fp, _Mesh_get_data_0004(_mesh));
        if (_Mesh_get_data_0004(_mesh)->has_uv)
        {
            version_0004_tangent_calculate(_Mesh_get_data_0004(_mesh));
        }
        _Mesh_get_data_0005(_mesh)->mesh_mode = Triangular;
    }
    else if ((uint16)word == 0x0005)
    {
        version_0005_load(fp, _Mesh_get_data_0005(_mesh));
    }
    else if ((uint16)word == 0x0006)
    {
        version_0006_load(fp, _Mesh_get_data_0006(_mesh));
    }
    fclose(fp);
}

void _free_and_set_null(void** p)
{
    if (*p)
    {
        Mfree(*p);
        *p = NULL;
    }
}

void _alloc_and_copy(void** p, void* s, uint size)
{
    *p = Malloc(size);
    memcpy(*p, s, size);
}

void Mesh_build(Mesh _mesh,
                float* pos_stream, float* uv_stream, float* nor_stream, uint num_vtxs,
                uint32* idx_stream, uint num_faces,
                mesh_mode mesh_mode)
{
    _free_and_set_null(&_Mesh_get_data_0004(_mesh)->vtx_pos);
    _free_and_set_null(&_Mesh_get_data_0004(_mesh)->vtx_uv);
    _free_and_set_null(&_Mesh_get_data_0004(_mesh)->vtx_nor);
    _free_and_set_null(&_Mesh_get_data_0004(_mesh)->vtx_tgt);
    _free_and_set_null(&_Mesh_get_data_0004(_mesh)->vtx_binor);

    _free_and_set_null(&_Mesh_get_data_0004(_mesh)->indexs);

    _Mesh_get_data_0004(_mesh)->vtx_count = num_vtxs;

    _alloc_and_copy(&_Mesh_get_data_0004(_mesh)->vtx_pos, pos_stream, sizeof(float) * 3 * num_vtxs);
    _alloc_and_copy(&_Mesh_get_data_0004(_mesh)->vtx_nor, nor_stream, sizeof(float) * 3 * num_vtxs);
    if (uv_stream)
    {
        _Mesh_get_data_0004(_mesh)->has_uv = 1;
        _alloc_and_copy(&_Mesh_get_data_0004(_mesh)->vtx_uv, uv_stream, sizeof(float) * 2 * num_vtxs);
    }
    else
    {
        _Mesh_get_data_0004(_mesh)->has_uv = 0;
    }

    switch (mesh_mode)
    {
    case Triangular:
        _alloc_and_copy(&_Mesh_get_data_0004(_mesh)->indexs, idx_stream, sizeof(uint32) * 3 * num_faces);
        _Mesh_get_data_0004(_mesh)->indexed_vtx_count = num_faces * 3;
        break;
    case Segment:
        _alloc_and_copy(&_Mesh_get_data_0004(_mesh)->indexs, idx_stream, sizeof(uint32) * 2 * num_faces);
        _Mesh_get_data_0004(_mesh)->indexed_vtx_count = num_faces * 2;
        break;
    }

    _Mesh_get_data_0004(_mesh)->face_count = num_faces;
    _Mesh_get_data_0005(_mesh)->mesh_mode = mesh_mode;

    if (uv_stream)
    {
        if (mesh_mode == Triangular)
            version_0004_tangent_calculate(_Mesh_get_data_0004(_mesh));
        else
        {
            _Mesh_get_data_0004(_mesh)->vtx_tgt = (float*)Malloc(sizeof(float) * 2 * num_vtxs);
            _Mesh_get_data_0004(_mesh)->vtx_binor = (float*)Malloc(sizeof(float) * 2 * num_vtxs);
        }
    }
}

void Mesh_fill_epw(Mesh _mesh, float* _epw_stream)
{
    uint vtx_count = _Mesh_get_data_0004(_mesh)->vtx_count;
    _free_and_set_null(&_Mesh_get_data_0006(_mesh)->vtx_epw);
    _Mesh_get_data_0006(_mesh)->vtx_epw = Malloc(sizeof(float) * vtx_count);
    memcpy(_Mesh_get_data_0006(_mesh)->vtx_epw, _epw_stream, sizeof(float) * vtx_count);
}

float* _stream_doubling(float* _stream, uint _cnt, uint _num_comp)
{
    float* ret = Malloc(sizeof(float) * _num_comp * _cnt * 2);
    memcpy(ret, _stream, sizeof(float) * _num_comp * _cnt);
    memcpy(&ret[_num_comp * _cnt], _stream, sizeof(float) * _num_comp * _cnt);
    Mfree(_stream);
    return ret;
}

Mesh Mesh_build_volume(Mesh _mesh)
{
    if (_Mesh_get_data_0004(_mesh)->indexs && _Mesh_get_data_0005(_mesh)->mesh_mode == Triangular && !_Mesh_get_data_0006(_mesh)->vtx_epw)
    {
        Mesh ret = Mesh_new();
        uint vtx_count = _Mesh_get_data_0004(_mesh)->vtx_count;
        uint idx_count = _Mesh_get_data_0004(_mesh)->indexed_vtx_count;

        float* pos_buf = Malloc(sizeof(float) * 3 * vtx_count * 2);
        float* base_pos_ptr0 = pos_buf;
        float* base_pos_ptr1 = &pos_buf[3 * vtx_count];
        memcpy(base_pos_ptr0, _Mesh_get_data_0004(_mesh)->vtx_pos, sizeof(float) * 3 * vtx_count);
        memcpy(base_pos_ptr1, _Mesh_get_data_0004(_mesh)->vtx_pos, sizeof(float) * 3 * vtx_count);

        float* epw_buf = Malloc(sizeof(float) * vtx_count * 2);
        float* base_epw_ptr0 = epw_buf;
        float* base_epw_ptr1 = &epw_buf[vtx_count];

        for (uint i = 0; i < vtx_count; i++)
        {
            base_epw_ptr1[i] = 1.0f;
        }

        uint32* idx_buf = Malloc(sizeof(uint32) * idx_count * 7);
        uint32* base_idx_ptr0 = idx_buf;

        uint32* base_idx_ptr1 = &idx_buf[idx_count * 1];
        uint32* base_idx_ptr2 = &idx_buf[idx_count * 2];

        uint32* base_idx_ptr3 = &idx_buf[idx_count * 3];
        uint32* base_idx_ptr4 = &idx_buf[idx_count * 4];

        uint32* base_idx_ptr5 = &idx_buf[idx_count * 5];
        uint32* base_idx_ptr6 = &idx_buf[idx_count * 6];

        memcpy(base_idx_ptr0, _Mesh_get_data_0004(_mesh)->indexs, sizeof(uint32) * idx_count);

        EAssert( _Mesh_get_data_0004(_mesh)->face_count == _Mesh_get_data_0004(_mesh)->indexed_vtx_count / 3, "index count error!" );
        EAssert( 0 == _Mesh_get_data_0004(_mesh)->indexed_vtx_count % 3, "index count error!" );

        void _fill_ext_idxs(uint i, uint32 e0, uint e1, uint vtx_cnt, uint32* idxs0, uint32* idxs1)
        {
            idxs0[i * 3 + 0] = e1;
            idxs0[i * 3 + 1] = e0;
            idxs0[i * 3 + 2] = e0 + vtx_cnt;

            idxs1[i * 3 + 0] = e1;
            idxs1[i * 3 + 1] = e0 + vtx_cnt;
            idxs1[i * 3 + 2] = e1 + vtx_cnt;
        }

        uint face_count = _Mesh_get_data_0004(_mesh)->face_count;
        for (uint i = 0; i < face_count; i++)
        {
            uint32 vtx0 = base_idx_ptr0[i * 3 + 0];
            uint32 vtx1 = base_idx_ptr0[i * 3 + 1];
            uint32 vtx2 = base_idx_ptr0[i * 3 + 2];

            _fill_ext_idxs(i, vtx0, vtx1, vtx_count, base_idx_ptr1, base_idx_ptr2);
            _fill_ext_idxs(i, vtx1, vtx2, vtx_count, base_idx_ptr3, base_idx_ptr4);
            _fill_ext_idxs(i, vtx2, vtx0, vtx_count, base_idx_ptr5, base_idx_ptr6);
        }
        ret->version = 0x0006;

        _Mesh_get_data_0004(ret)->vtx_pos = pos_buf;
        _Mesh_get_data_0004(ret)->indexs = idx_buf;

        _Mesh_get_data_0005(ret)->mesh_mode = Triangular;

        _Mesh_get_data_0006(ret)->vtx_epw = epw_buf;

        _Mesh_get_data_0004(ret)->vtx_count = vtx_count * 2;
        _Mesh_get_data_0004(ret)->face_count = face_count * 7;
        _Mesh_get_data_0004(ret)->indexed_vtx_count = face_count * 7 * 3;
        _Mesh_get_data_0004(ret)->has_uv = 0;
        return ret;
    }
    else
        return NULL;
}
