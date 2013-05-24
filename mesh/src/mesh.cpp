#include "mesh.h"
#include "elog.h"
///#include "float3.h"
#include "emem.h"
#include "mesh_base.h"
#include "mesh_triangle.h"
#include "eassert.h"
#include "exception.h"
#include "xhn_exception.hpp"

static int s_mesh_count = 0;
void Mesh_print_mesh_count()
{
    elog("mesh count %d\n", s_mesh_count);
}
/**
void Mesh_Dest(struct _mesh* _mesh)
{
    version_0007_Dest(_Mesh_get_data_0007(_mesh));
}
Mesh _Mesh_new(const char* _file, euint _line)
{
    Mesh ret;
    ret = (Mesh)SMalloc(sizeof(mesh));
	pthread_spin_init(&ret->lock, PTHREAD_PROCESS_SHARED);
    ret->ref_count = 1;
    ret->file = _file;
    ret->line = _line;

    s_mesh_count++;
    return ret;
}
void Mesh_delete(Mesh _mesh)
{
	try 
	{
	    pthread_spin_lock(&_mesh->lock);
        _mesh->ref_count--;
		pthread_spin_unlock(&_mesh->lock);
        EAssert(_mesh->ref_count >= 0, "%s", "mesh's reference count less than zero!");
        if (!_mesh->ref_count)
        {
            Mesh_Dest(_mesh);
            Mfree(_mesh);
            s_mesh_count--;
        }
	}
	catch (...)
	{
		pthread_spin_unlock(&_mesh->lock);
	}
}
**/
mesh::mesh(const char* _file, euint _line)
{
    file = _file;
    line = _line;

	version = 0;
	memset(&data, 0, sizeof(data));

    s_mesh_count++;
}
mesh::~mesh()
{
    s_mesh_count--;

	version_0007_Dest(_Mesh_get_data_0007(this));
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
float* Mesh_get_color(Mesh _mesh)
{
    return _Mesh_get_data_0007(_mesh)->vtx_col;
}
euint32* Mesh_get_index(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->indexs;
}
euint32 Mesh_get_vertex_count(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->vtx_count;
}
euint32 Mesh_get_face_count(Mesh _mesh)
{
    return _Mesh_get_data_0004(_mesh)->face_count;
}
e_mesh_mode Mesh_get_mode(Mesh _mesh)
{
    return _Mesh_get_data_0005(_mesh)->mesh_mode;
}
euint32 face_size(e_mesh_mode mesh_mode)
{
    if (mesh_mode == Triangular)
    {
        return sizeof(euint32) * 3;
    }
    else
    {
        return sizeof(euint32) * 2;
    }
}

void Mesh_load_form_file(Mesh _mesh, const char* _file_name)
{
    FILE* fp = SafeFOpen(_file_name, "rb");;
    char id[4];
    id[0] = getc(fp);
    id[1] = getc(fp);
    id[2] = getc(fp);
    id[3] = getc(fp);
    if (id[0] != 'O' || id[1] != 'G' || id[2] != 'L')
    {
        return;
    }

    euint32 word;
    /// Version

    fread(&word, sizeof(euint32), 1, fp);
    _mesh->version = word;
    printf("ogl version %d\n", _mesh->version);
    if ((euint16)word == 0x0004)
    {
        version_0004_load(fp, _Mesh_get_data_0004(_mesh));
        if (_Mesh_get_data_0004(_mesh)->has_uv)
        {
            version_0004_tangent_calculate(_Mesh_get_data_0004(_mesh));
        }
        _Mesh_get_data_0005(_mesh)->mesh_mode = Triangular;
    }
    else if ((euint16)word == 0x0005)
    {
        version_0005_load(fp, _Mesh_get_data_0005(_mesh));
    }
    else if ((euint16)word == 0x0006)
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

void _alloc_and_copy(void** p, void* s, euint size)
{
    *p = SMalloc(size);
    memcpy(*p, s, size);
}

///#define MESH_BUILD_DEBUG

void Mesh_build(Mesh _mesh,
                float* pos_stream, float* uv_stream, float* nor_stream, euint32 num_vtxs,
                euint32* idx_stream, euint32 num_faces,
                e_mesh_mode mesh_mode)
{
    TRY(0)
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_pos);
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_uv);
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_nor);
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_tgt);
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_binor);

    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->indexs);

    _Mesh_get_data_0004(_mesh)->vtx_count = num_vtxs;

    _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_pos, pos_stream, sizeof(float) * 3 * num_vtxs);
    _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_nor, nor_stream, sizeof(float) * 3 * num_vtxs);

    #ifdef MESH_BUILD_DEBUG
    if (!MCheck())
        throw_exception(MemPoolIsCorrupted, "ÄÚ´æ³ØÒÑËð»µ");
    #endif // MESH_BUILD_DEBUG
    if (uv_stream)
    {
        _Mesh_get_data_0004(_mesh)->has_uv = 1;
        _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_uv, uv_stream, sizeof(float) * 2 * num_vtxs);
    }
    else
    {
        _Mesh_get_data_0004(_mesh)->has_uv = 0;
    }
    #ifdef MESH_BUILD_DEBUG
    if (!MCheck())
        throw_exception(MemPoolIsCorrupted, "ÄÚ´æ³ØÒÑËð»µ");
    #endif // MESH_BUILD_DEBUG
    switch (mesh_mode)
    {
    case Triangular:
        _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->indexs, idx_stream, sizeof(euint32) * 3 * num_faces);
        _Mesh_get_data_0004(_mesh)->indexed_vtx_count = num_faces * 3;
        break;
    case Segment:
        _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->indexs, idx_stream, sizeof(euint32) * 2 * num_faces);
        _Mesh_get_data_0004(_mesh)->indexed_vtx_count = num_faces * 2;
        break;
    default:
        break;
    }
    #ifdef MESH_BUILD_DEBUG
    if (!MCheck())
        throw_exception(MemPoolIsCorrupted, "ÄÚ´æ³ØÒÑËð»µ");
    #endif // MESH_BUILD_DEBUG
    _Mesh_get_data_0004(_mesh)->face_count = num_faces;
    _Mesh_get_data_0005(_mesh)->mesh_mode = mesh_mode;

    if (uv_stream)
    {
        if (mesh_mode == Triangular)
            version_0004_tangent_calculate(_Mesh_get_data_0004(_mesh));
        else
        {
            _Mesh_get_data_0004(_mesh)->vtx_tgt = (float*)SMalloc(sizeof(float) * 2 * num_vtxs);
            _Mesh_get_data_0004(_mesh)->vtx_binor = (float*)SMalloc(sizeof(float) * 2 * num_vtxs);
        }
    }
    #ifdef MESH_BUILD_DEBUG
    if (!MCheck()) {
        throw_exception(MemPoolIsCorrupted, "ÄÚ´æ³ØÒÑËð»µ");
    }
    #endif // MESH_BUILD_DEBUG
    CATCH(0)
    {
        elog("%s", GET_EXCE);
    }
}

void Mesh_build2(Mesh _mesh,
                float* pos_stream, float* uv_stream, float* nor_stream, float* col_stream,
                euint32 num_vtxs,
                euint32* idx_stream, euint32 num_faces,
                e_mesh_mode mesh_mode)
{
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_pos);
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_uv);
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_nor);
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_tgt);
    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_binor);
    _free_and_set_null((vptr*)&_Mesh_get_data_0007(_mesh)->vtx_col);

    _free_and_set_null((vptr*)&_Mesh_get_data_0004(_mesh)->indexs);

    _Mesh_get_data_0004(_mesh)->vtx_count = num_vtxs;

    _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_pos, pos_stream, sizeof(float) * 3 * num_vtxs);
    _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_nor, nor_stream, sizeof(float) * 3 * num_vtxs);
    _alloc_and_copy((vptr*)&_Mesh_get_data_0007(_mesh)->vtx_col, col_stream, sizeof(float) * 4 * num_vtxs);
    if (uv_stream)
    {
        _Mesh_get_data_0004(_mesh)->has_uv = 1;
        _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->vtx_uv, uv_stream, sizeof(float) * 2 * num_vtxs);
    }
    else
    {
        _Mesh_get_data_0004(_mesh)->has_uv = 0;
    }

    switch (mesh_mode)
    {
    case Triangular:
        _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->indexs, idx_stream, sizeof(euint32) * 3 * num_faces);
        _Mesh_get_data_0004(_mesh)->indexed_vtx_count = num_faces * 3;
        break;
    case Segment:
        _alloc_and_copy((vptr*)&_Mesh_get_data_0004(_mesh)->indexs, idx_stream, sizeof(euint32) * 2 * num_faces);
        _Mesh_get_data_0004(_mesh)->indexed_vtx_count = num_faces * 2;
        break;
    default:
        VEngineExce(InvalidEnumerationException, "mesh mode is invalid enumeration");
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
            _Mesh_get_data_0004(_mesh)->vtx_tgt = (float*)SMalloc(sizeof(float) * 2 * num_vtxs);
            _Mesh_get_data_0004(_mesh)->vtx_binor = (float*)SMalloc(sizeof(float) * 2 * num_vtxs);
        }
    }
}

void Mesh_fill_epw(Mesh _mesh, float* _epw_stream)
{
    euint vtx_count = _Mesh_get_data_0004(_mesh)->vtx_count;
    _free_and_set_null((vptr*)&_Mesh_get_data_0006(_mesh)->vtx_epw);
    _Mesh_get_data_0006(_mesh)->vtx_epw = (float*)SMalloc(sizeof(float) * vtx_count);
    memcpy(_Mesh_get_data_0006(_mesh)->vtx_epw, _epw_stream, sizeof(float) * vtx_count);
}

float* _stream_doubling(float* _stream, euint _cnt, euint _num_comp)
{
    float* ret = (float*)SMalloc(sizeof(float) * _num_comp * _cnt * 2);
    memcpy(ret, _stream, sizeof(float) * _num_comp * _cnt);
    memcpy(&ret[_num_comp * _cnt], _stream, sizeof(float) * _num_comp * _cnt);
    Mfree(_stream);
    return ret;
}

static inline void _fill_ext_idxs(euint i, euint32 e0, euint32 e1, euint32 vtx_cnt, euint32* idxs0, euint32* idxs1)
{
    idxs0[i * 3 + 0] = e1;
    idxs0[i * 3 + 1] = e0;
    idxs0[i * 3 + 2] = e0 + vtx_cnt;

    idxs1[i * 3 + 0] = e1;
    idxs1[i * 3 + 1] = e0 + vtx_cnt;
    idxs1[i * 3 + 2] = e1 + vtx_cnt;
}

Mesh Mesh_build_volume(Mesh _mesh)
{
    if (_Mesh_get_data_0004(_mesh)->indexs && _Mesh_get_data_0005(_mesh)->mesh_mode == Triangular && !_Mesh_get_data_0006(_mesh)->vtx_epw)
    {
        Mesh ret = Mesh_new();
        euint32 vtx_count = _Mesh_get_data_0004(_mesh)->vtx_count;
        euint32 idx_count = _Mesh_get_data_0004(_mesh)->indexed_vtx_count;

        float* pos_buf = (float*)SMalloc(sizeof(float) * 3 * vtx_count * 2);
        float* base_pos_ptr0 = pos_buf;
        float* base_pos_ptr1 = &pos_buf[3 * vtx_count];
        memcpy(base_pos_ptr0, _Mesh_get_data_0004(_mesh)->vtx_pos, sizeof(float) * 3 * vtx_count);
        memcpy(base_pos_ptr1, _Mesh_get_data_0004(_mesh)->vtx_pos, sizeof(float) * 3 * vtx_count);

        float* epw_buf = (float*)SMalloc(sizeof(float) * vtx_count * 2);
        ///float* base_epw_ptr0 = epw_buf;
        float* base_epw_ptr1 = &epw_buf[vtx_count];

        for (euint i = 0; i < vtx_count; i++)
        {
            base_epw_ptr1[i] = 1.0f;
        }

        euint32* idx_buf = (euint32*)SMalloc(sizeof(euint32) * idx_count * 7);
        euint32* base_idx_ptr0 = idx_buf;

        euint32* base_idx_ptr1 = &idx_buf[idx_count * 1];
        euint32* base_idx_ptr2 = &idx_buf[idx_count * 2];

        euint32* base_idx_ptr3 = &idx_buf[idx_count * 3];
        euint32* base_idx_ptr4 = &idx_buf[idx_count * 4];

        euint32* base_idx_ptr5 = &idx_buf[idx_count * 5];
        euint32* base_idx_ptr6 = &idx_buf[idx_count * 6];

        memcpy(base_idx_ptr0, _Mesh_get_data_0004(_mesh)->indexs, sizeof(euint32) * idx_count);

        EAssert( _Mesh_get_data_0004(_mesh)->face_count == _Mesh_get_data_0004(_mesh)->indexed_vtx_count / 3, "%s", "index count error!" );
        EAssert( 0 == _Mesh_get_data_0004(_mesh)->indexed_vtx_count % 3, "%s", "index count error!" );

        euint32 face_count = _Mesh_get_data_0004(_mesh)->face_count;
        for (euint i = 0; i < face_count; i++)
        {
            euint32 vtx0 = base_idx_ptr0[i * 3 + 0];
            euint32 vtx1 = base_idx_ptr0[i * 3 + 1];
            euint32 vtx2 = base_idx_ptr0[i * 3 + 2];

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

void Mesh_apply_transform(Mesh _mesh, const matrix4x4* _trans)
{
	version_0007_apply_transform(&_mesh->data, _trans);
}