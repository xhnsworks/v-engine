#include "collada_loader.h"
#include "array.h"
#include "estring.h"
#include "elog.h"
#include "mesh.h"
///#include "mxml.h"
#include "pugixml.hpp"
#include "tree.h"
#include "xhn_string.hpp"
#include "xhn_vector.hpp"
#include "xhn_utility.hpp"
#include <string.h>
/**
mxml_type_t collada_callback(mxml_node_t * node)
{
    ///printf(node->value.opaque);
    if (strcmp(node->value.opaque, "float_array") == 0)
    {
        printf(node->value.text.string);
    }
    return MXML_TEXT;
}
**/
typedef struct _stream
{
    float* float_array;
    uint set;
    uint count;
    uint stride;
    bool is_vertex_stream;
} stream;
typedef struct _stream* FloatStream;
FloatStream FloatStream_new()
{
    FloatStream ret = (FloatStream)SMalloc(sizeof(stream));
    ret->float_array = array_new(float, 100, fexce);
    ret->set = 0;
    ret->count = 0;
    ret->stride = 0;
    ret->is_vertex_stream = false;
    return ret;
}
void FloatStream_delete(FloatStream _self)
{
    array_delete(_self->float_array);
    Mfree(_self);
}
void FloatStream_log(FloatStream _self)
{
    elog("array size %d, set %d, count %d, stride %d\n", array_n(_self->float_array), _self->set, _self->count, _self->stride);
}

typedef enum _collada_semantic
{
    ColladaEmptySemantic = 0,
    ColladaVertex,
    ColladaPosition,
    ColladaNormal,
    ColladaTexCoord,
} collada_semantic;
typedef struct _index_element
{
    collada_semantic sem;
    uint offs;
} index_element;

typedef struct _collada_state
{
    Tree stream_tree;
    Tree sem_stream_tree;
    index_element* idx_eles;
    uint num_tri;
    uint32* idx_stream;
} collada_state;

ColladaState ColladaState_new()
{
    ColladaState ret = (ColladaState)SMalloc(sizeof(collada_state));
    ret->stream_tree = Tree_new(String, Vptr, Ealloc, Efree);
    ret->sem_stream_tree = Tree_new(Uint32, Vptr, Ealloc, Efree);
    index_element null_idx_ele = {ColladaEmptySemantic, 0};
    ret->idx_eles = array_new(index_element, 5, null_idx_ele);
    ret->num_tri = 0;
    ret->idx_stream = array_new(uint32, 100, 0xffffffff);
    return ret;
}
/**
void ColladaState_enum_semantics(ColladaState _self, mxml_node_t* node, bool is_vertex_stream)
{
    mxml_index_t* input_idx = mxmlIndexNew(node, NULL, NULL);
    mxml_node_t* input = mxmlIndexEnum(input_idx);
    while (input)
    {
        const char* sem_str = mxmlElementGetAttr(input, "semantic");
        const char* source = mxmlElementGetAttr(input, "source");
        const char* set = mxmlElementGetAttr(input, "set");
        const char* offs = mxmlElementGetAttr(input, "offset");

        if (sem_str && source)
        {
            uint count = 0;
            while (source[count] && source[count] == '#')
                count++;

            FloatStream s = NULL;
            {
                var key, data;
                key.str_var = &source[count];
                if (Tree_find(_self->stream_tree, key, &data))
                {
					s = (FloatStream)data.vptr_var;
                    if (set)
                        s->set = atoi(set);
                    else
                        s->set = 0;
                    s->is_vertex_stream = is_vertex_stream;
                }
            }
            if (s)
            {
                var key, data;
                if      (strcmp(sem_str, "VERTEX") == 0)
                    key.uint32_var = ColladaVertex;
                else if (strcmp(sem_str, "POSITION") == 0)
                    key.uint32_var = ColladaPosition;
                else if (strcmp(sem_str, "NORMAL") == 0)
                    key.uint32_var = ColladaNormal;
                else if (strcmp(sem_str, "TEXCOORD") == 0)
                    key.uint32_var = ColladaTexCoord;
                else
                    key.uint32_var = ColladaEmptySemantic;
                data.vptr_var = s;
                Tree_insert(_self->sem_stream_tree, key, data);
                if (offs)
                {
					index_element ie = {(collada_semantic)key.uint32_var, atoi(offs)};
                    apush(_self->idx_eles, ie);
                }
            }
            else
            {
                if      (strcmp(sem_str, "VERTEX") == 0 && offs)
                {
                    var key;
                    key.uint32_var = ColladaVertex;
                    index_element ie = {(collada_semantic)key.uint32_var, atoi(offs)};
                    apush(_self->idx_eles, ie);
                }
            }
        }

        input = mxmlIndexEnum(input_idx);
    }
    mxmlIndexDelete(input_idx);
}
**/
void ColladaState_enum_semantics(ColladaState _self, pugi::xml_node& node, bool is_vertex_stream)
{
	pugi::xml_node::iterator inputIter = node.begin();
	pugi::xml_node::iterator inputEnd = node.end();
	for (; inputIter != inputEnd; inputIter++)
	{
		pugi::xml_node input = *inputIter;
		xhn::string name = input.name();
		if (name != "input")
			continue;
		pugi::xml_attribute semAttr = input.attribute("semantic");
		pugi::xml_attribute sourceAttr = input.attribute("source");
		pugi::xml_attribute setAttr = input.attribute("set");
		pugi::xml_attribute offsAttr = input.attribute("offset");

		if (semAttr && sourceAttr)
        {
            uint count = 0;
			const char* sem = semAttr.value();
			const char* source = sourceAttr.value();
            while (source[count] && source[count] == '#')
                count++;

            FloatStream s = NULL;
            {
                var key, data;
                key.str_var = &source[count];
                if (Tree_find(_self->stream_tree, key, &data))
                {
					s = (FloatStream)data.vptr_var;
                    if (setAttr)
                        s->set = atoi(setAttr.value());
                    else
                        s->set = 0;
                    s->is_vertex_stream = is_vertex_stream;
                }
            }
            if (s)
            {
                var key, data;
                if      (strcmp(sem, "VERTEX") == 0)
                    key.uint32_var = ColladaVertex;
                else if (strcmp(sem, "POSITION") == 0)
                    key.uint32_var = ColladaPosition;
                else if (strcmp(sem, "NORMAL") == 0)
                    key.uint32_var = ColladaNormal;
                else if (strcmp(sem, "TEXCOORD") == 0)
                    key.uint32_var = ColladaTexCoord;
                else
                    key.uint32_var = ColladaEmptySemantic;
                data.vptr_var = s;
                Tree_insert(_self->sem_stream_tree, key, data);
                if (offsAttr)
                {
					index_element ie = {(collada_semantic)key.uint32_var, atoi(offsAttr.value())};
                    apush(_self->idx_eles, ie);
                }
            }
            else
            {
                if      (strcmp(sem, "VERTEX") == 0 && offsAttr)
                {
                    var key;
                    key.uint32_var = ColladaVertex;
                    index_element ie = {(collada_semantic)key.uint32_var, atoi(offsAttr.value())};
                    apush(_self->idx_eles, ie);
                }
            }
        }
	}
}

void _parse_float_array(const char* str, FloatStream result)
{
	uint count = 0;
	xhn::vector< char, xhn::FGetCharRealSizeProc<char> > tmp;
	while (str[count])
	{
		char c = str[count];
		if ((c >= '0' && c <= '9') || c == '-' || c == '.')
		{
			tmp.push_back(c);
		}
		else
		{
			if (tmp.size())
			{
				xhn::string s = tmp;
				float f = (float)atof(s.c_str());
				apush(result->float_array, f);
			}
			tmp.clear();
		}
		count++;
	}
}

uint32* _parse_p(const char* str, uint32* result)
{
	uint count = 0;
	xhn::vector< char, xhn::FGetCharRealSizeProc<char> > tmp;
	while (str[count])
	{
		char c = str[count];
		if (c >= '0' && c <= '9')
		{
			tmp.push_back(c);
		}
		else
		{
			if (tmp.size())
			{
				xhn::string s = tmp;
				uint u = (uint)atoi(s.c_str());
				apush(result, u);
			}
			tmp.clear();
		}
		count++;
	}
	return result;
}
/**
void ColladaState_load_mesh2(ColladaState _self, const char* filepath)
{
    FILE* fp = SafeFOpen(filepath, "rb");
    mxml_node_t* doc = mxmlLoadFile(NULL, fp, NULL);
    mxml_node_t* elem = mxmlFindElement(doc, doc, "library_geometries", NULL, NULL, MXML_DESCEND);
    if (elem)
    {
        mxml_node_t* mesh = mxmlFindElement(elem, elem, "mesh", NULL, NULL, MXML_DESCEND);
        if (mesh)
        {
            mxml_index_t* idx = mxmlIndexNew(mesh, NULL, NULL);
            mxml_node_t* node = mxmlIndexEnum(idx);
            while (node)
            {
                if (strcmp(node->value.opaque, "source") == 0)
                {
                    const char* id = mxmlElementGetAttr(node, "id");
                    ///printf("id %s\n", id);
                    mxml_node_t* float_array = mxmlFindElement(node, node, "float_array", NULL, NULL, MXML_DESCEND);
                    mxml_node_t* technique_common = mxmlFindElement(node, node, "technique_common", NULL, NULL, MXML_DESCEND);
                    mxml_node_t* accessor = mxmlFindElement(technique_common, technique_common, "accessor", NULL, NULL, MXML_DESCEND);
                    /// count和stride必须从technique_common里面取，真他妈操蛋
                    const char* count_str = mxmlElementGetAttr(accessor, "count");
                    const char* stride_str = mxmlElementGetAttr(accessor, "stride");
                    uint count = atoi(count_str);
                    uint stride = atoi(stride_str);
                    FloatStream s = FloatStream_new();
                    s->count = count;
                    s->stride = stride;
                    if (float_array)
                    {
                        mxml_node_t* child = float_array->child;
                        while (child)
                        {
							///float* arr = s->float_array;
							float v = (float)atof(child->value.text.string);
                            apush(s->float_array, v);
                            child = mxmlWalkNext(child, float_array, MXML_DESCEND);
                        }
                    }
                    var key, data;
                    key.str_var = id;
                    data.vptr_var = s;
                    Tree_insert(_self->stream_tree, key, data);
                }
                node = mxmlIndexEnum(idx);
            }
            mxmlIndexDelete(idx);
        }
        mxml_node_t* vertices = mxmlFindElement(mesh, mesh, "vertices", NULL, NULL, MXML_DESCEND);
        if (vertices)
        {
            ColladaState_enum_semantics(_self, vertices, true);
        }
        mxml_node_t* triangles = mxmlFindElement(mesh, mesh, "triangles", NULL, NULL, MXML_DESCEND);
        if (triangles)
        {
            ColladaState_enum_semantics(_self, triangles, false);

            uint count = atoi(mxmlElementGetAttr(triangles, "count"));
            _self->num_tri = count;
            mxml_node_t* p = mxmlFindElement(triangles, triangles, "p", NULL, NULL, MXML_DESCEND);
            if (p)
            {
                mxml_node_t* child = p->child;
                while (child)
                {
					uint32 v = atoi(child->value.text.string);
                    apush(_self->idx_stream, v);
                    child = mxmlWalkNext(child, p, MXML_DESCEND);
                }
            }
        }
    }
    mxmlDelete(doc);
    fclose(fp);
}
**/
void ColladaState_load_mesh(ColladaState _self, const char* filepath)
{
	pugi::xml_document doc;
	doc.load_file(filepath);
	pugi::xml_node root = doc.child("COLLADA");
	pugi::xml_node geoms = root.child("library_geometries");
	pugi::xml_node::iterator geomIter = geoms.begin();
	pugi::xml_node::iterator geomEnd = geoms.end();
	for (; geomIter != geomEnd; geomIter++)
	{
		pugi::xml_node geom = *geomIter;
		pugi::xml_node mesh = geom.child("mesh");
		pugi::xml_node::iterator eleIter = mesh.begin();
		pugi::xml_node::iterator eleEnd = mesh.end();
		for (; eleIter != eleEnd; eleIter++)
		{
			pugi::xml_node ele = *eleIter;
			xhn::string eleName = ele.name();
			if (eleName == "source")
			{
				pugi::xml_attribute idAttr = ele.attribute("id");
				pugi::xml_node floatArray = ele.child("float_array");
				const char* value = floatArray.child_value();
				
				pugi::xml_node techniqueCommon = ele.child("technique_common");
				pugi::xml_node accessor = techniqueCommon.child("accessor");
				pugi::xml_attribute countAttr = accessor.attribute("count");
				pugi::xml_attribute strideAttr = accessor.attribute("stride");

				FloatStream floats = FloatStream_new();
				floats->count = countAttr.as_uint();
				floats->stride = strideAttr.as_uint();
				_parse_float_array(value, floats);

				var key, data;
				key.str_var = idAttr.value();
                data.vptr_var = floats;
                Tree_insert(_self->stream_tree, key, data);
			}
		}

		eleIter = mesh.begin();
		eleEnd = mesh.end();
		for (; eleIter != eleEnd; eleIter++)
		{
			pugi::xml_node ele = *eleIter;
			xhn::string eleName = ele.name();
			if (eleName == "vertices")
			{
				ColladaState_enum_semantics(_self, ele, true);
			}
			else if (eleName == "triangles")
			{
				ColladaState_enum_semantics(_self, ele, false);
				uint count = ele.attribute("count").as_uint();
				_self->num_tri = count;
				pugi::xml_node p = ele.child("p");
				if (p)
				{
					const char* value = p.child_value();
				    _self->idx_stream = _parse_p(value, _self->idx_stream);
				}
			}
		}
	}
}

const char* _get_collada_sem_str(uint32 sem)
{
    switch (sem)
    {
    case ColladaVertex:
        return "ColladaVertex";
    case ColladaPosition:
        return "ColladaPosition";
    case ColladaNormal:
        return "ColladaNormal";
    case ColladaTexCoord:
        return "ColladaTexCoord";
    case ColladaEmptySemantic:
    default:
        return "ColladaEmptySemantic";
    }
}

void ColladaState_log(ColladaState _self)
{
    Iterator iter = Tree_begin(_self->sem_stream_tree);
    while (iter)
    {
        var key = Tree_get_key(iter);
        var data = Tree_get_value(iter);
        uint32 sem = key.uint32_var;
        FloatStream s = (FloatStream)data.vptr_var;
        elog("%s", _get_collada_sem_str(sem));
        FloatStream_log(s);
        iter = Tree_next(iter);
    }
}

uint _find_offset(ColladaState _self, collada_semantic sem)
{
    uint n = array_n(_self->idx_eles);
    for (uint i = 0; i < n; i++)
    {
        if (sem == _self->idx_eles[i].sem)
            return _self->idx_eles[i].offs;
    }
    return UINT32_EXCE;
}

Mesh ColladaState_create_mesh(ColladaState _self)
{
    uint num_vtx = 0;
    float* pos_src = NULL;
    float* tex_src = NULL;
    float* nor_src = NULL;
    uint num_pos = 0;
    uint num_tex = 0;
    uint num_nor = 0;
    uint stride_pos = 0;
    uint stride_tex = 0;
    uint stride_nor = 0;
    uint offs_pos = 0;
    uint offs_tex = 0;
    uint offs_nor = 0;
    collada_semantic base_sem = ColladaEmptySemantic;
    uint offs_base = 0;
    Iterator iter = Tree_begin(_self->sem_stream_tree);
    while (iter)
    {
        var key = Tree_get_key(iter);
        var data = Tree_get_value(iter);
        FloatStream s = (FloatStream)data.vptr_var;
        if (s->count > num_vtx)
        {
            num_vtx = s->count;
			base_sem =(collada_semantic)key.uint32_var;
        }
        switch (key.uint32_var)
        {
        case ColladaPosition:
            if (s->set == 0)
            {
                pos_src = s->float_array;
                num_pos = s->count;
                stride_pos = s->stride;
                if (s->is_vertex_stream)
                    offs_pos = _find_offset(_self, ColladaVertex);
                else
                    offs_pos = _find_offset(_self, ColladaPosition);
            }
            break;
        case ColladaTexCoord:
            if (s->set == 0)
            {
                tex_src = s->float_array;
                num_tex = s->count;
                stride_tex = s->stride;
                if (s->is_vertex_stream)
                    offs_tex = _find_offset(_self, ColladaVertex);
                else
                    offs_tex = _find_offset(_self, ColladaTexCoord);
            }
            break;
        case ColladaNormal:
            if (s->set == 0)
            {
                nor_src = s->float_array;
                num_nor = s->count;
                stride_nor = s->stride;
                if (s->is_vertex_stream)
                    offs_nor = _find_offset(_self, ColladaVertex);
                else
                    offs_nor = _find_offset(_self, ColladaNormal);
            }
            break;
        default:
            break;
        }
        iter = Tree_next(iter);
    }

    switch (base_sem)
    {
    case ColladaPosition:
        offs_base = offs_pos;
        break;
    case ColladaTexCoord:
        offs_base = offs_tex;
        break;
    case ColladaNormal:
        offs_base = offs_nor;
        break;
    default:
        break;
    }

    ///uint32* idx_src = _self->idx_stream;
    uint stride_idx = array_n(_self->idx_eles);
    ///uint num_tri = _self->num_tri;
    ///uint32* idx_stream = SMalloc(sizeof(uint32) * 3 * num_tri);

    float* pos_stream = (float*)SMalloc(sizeof(float) * 3 * num_vtx);
    float* tex_stream = (float*)SMalloc(sizeof(float) * 2 * num_vtx);
    float* nor_stream = (float*)SMalloc(sizeof(float) * 3 * num_vtx);
    uint32* idx_src = _self->idx_stream;

    uint num_tri = _self->num_tri;
    uint32* idx_stream = (uint32*)SMalloc(sizeof(uint32) * 3 * num_tri);

    memset(pos_stream, 0, sizeof(float) * 3 * num_vtx);
    memset(tex_stream, 0, sizeof(float) * 2 * num_vtx);
    memset(nor_stream, 0, sizeof(float) * 3 * num_vtx);
    memset(idx_stream, 0, sizeof(sizeof(uint32) * 3 * num_tri));

    struct idx_group
    {
        uint32 idx_base;
        uint32 idx_pos;
        uint32 idx_tex;
        uint32 idx_nor;
    };
	/**
    struct idx_group _get_idx_group(uint i, uint idx_tri_vtx)
    {
        struct idx_group ret;
        uint base = i * stride_idx * 3 + stride_idx * idx_tri_vtx;
        ret.idx_base = idx_src[base + offs_base];
        ret.idx_pos = idx_src[base + offs_pos];
        ret.idx_tex = idx_src[base + offs_tex];
        ret.idx_nor = idx_src[base + offs_nor];

        return ret;
    }
	**/
    uint max_idx_base = 0;
    uint max_idx_pos = 0;
    uint max_idx_tex = 0;
    uint max_idx_nor = 0;
    uint idx_count = 0;
	/**
    void _stream_copy(struct idx_group* idx_grp)
    {
        ///elog("idx_base %d\n", idx_grp->idx_base);
        if (idx_grp->idx_base > max_idx_base)
            max_idx_base = idx_grp->idx_base;
        if (idx_grp->idx_pos > max_idx_pos)
            max_idx_pos = idx_grp->idx_pos;
        if (idx_grp->idx_tex > max_idx_tex)
            max_idx_tex = idx_grp->idx_tex;
        if (idx_grp->idx_nor > max_idx_nor)
            max_idx_nor = idx_grp->idx_nor;

        pos_stream[idx_grp->idx_base * 3 + 0] = pos_src[idx_grp->idx_pos * stride_pos + 0];
        pos_stream[idx_grp->idx_base * 3 + 1] = pos_src[idx_grp->idx_pos * stride_pos + 1];
        pos_stream[idx_grp->idx_base * 3 + 2] = pos_src[idx_grp->idx_pos * stride_pos + 2];

        tex_stream[idx_grp->idx_base * 2 + 0] = tex_src[idx_grp->idx_tex * stride_tex + 0];
        tex_stream[idx_grp->idx_base * 2 + 1] = 1.0 - tex_src[idx_grp->idx_tex * stride_tex + 1];

        nor_stream[idx_grp->idx_base * 3 + 0] = nor_src[idx_grp->idx_nor * stride_nor + 0];
        nor_stream[idx_grp->idx_base * 3 + 1] = nor_src[idx_grp->idx_nor * stride_nor + 1];
        nor_stream[idx_grp->idx_base * 3 + 2] = nor_src[idx_grp->idx_nor * stride_nor + 2];
    }
	**/
    /**
    uint num_vtx = 0;
    float* pos_src = NULL;
    float* tex_src = NULL;
    float* nor_src = NULL;
    uint num_pos = 0;
    uint num_tex = 0;
    uint num_nor = 0;
    uint stride_pos = 0;
    uint stride_tex = 0;
    uint stride_nor = 0;
    uint offs_pos = 0;
    uint offs_tex = 0;
    uint offs_nor = 0;
    collada_semantic base_sem = ColladaEmptySemantic;
    uint offs_base = 0;
    **/
    elog("num_vtx %d\n", num_vtx);
    elog("pos_src %x\n", pos_src);
    elog("tex_src %x\n", tex_src);
    elog("nor_src %x\n", nor_src);
    elog("num_pos %d\n", num_pos);
    elog("num_tex %d\n", num_tex);
    elog("num_nor %d\n", num_nor);
    elog("stride_pos %d\n", stride_pos);
    elog("stride_tex %d\n", stride_tex);
    elog("stride_nor %d\n", stride_nor);
    elog("stride_idx %d\n", stride_idx);
    elog("offs_pos %d\n", offs_pos);
    elog("offs_tex %d\n", offs_tex);
    elog("offs_nor %d\n", offs_nor);
    elog("offs_base %d\n", offs_base);
    elog("base_sem %d\n", base_sem);
    elog("num_tri %d\n", num_tri);
	uint num_idx = array_n(_self->idx_stream);
	elog("real_num_idx %d\n", num_idx);

    for (uint i = 0; i < num_tri; i++)
    {
        ///struct idx_group idx0 = _get_idx_group(i, 0);
        ///struct idx_group idx1 = _get_idx_group(i, 1);
        ///struct idx_group idx2 = _get_idx_group(i, 2);
		struct idx_group idx0;
		struct idx_group idx1;
		struct idx_group idx2;
		{
			struct idx_group ret;
            uint base = i * stride_idx * 3 + stride_idx * 0;
            ret.idx_base = idx_src[base + offs_base];
            ret.idx_pos = idx_src[base + offs_pos];
            ret.idx_tex = idx_src[base + offs_tex];
            ret.idx_nor = idx_src[base + offs_nor];
			idx0 = ret;
		}
		{
			struct idx_group ret;
            uint base = i * stride_idx * 3 + stride_idx * 1;
            ret.idx_base = idx_src[base + offs_base];
            ret.idx_pos = idx_src[base + offs_pos];
            ret.idx_tex = idx_src[base + offs_tex];
            ret.idx_nor = idx_src[base + offs_nor];
			idx1 = ret;
		}
		{
			struct idx_group ret;
            uint base = i * stride_idx * 3 + stride_idx * 2;
            ret.idx_base = idx_src[base + offs_base];
            ret.idx_pos = idx_src[base + offs_pos];
            ret.idx_tex = idx_src[base + offs_tex];
            ret.idx_nor = idx_src[base + offs_nor];
			idx2 = ret;
		}

        ///_stream_copy(&idx0);
        ///_stream_copy(&idx1);
        ///_stream_copy(&idx2);

		struct idx_group* idx_grp = &idx0;

		{
        ///elog("idx_base %d\n", idx_grp->idx_base);
            if (idx_grp->idx_base > max_idx_base)
                max_idx_base = idx_grp->idx_base;
            if (idx_grp->idx_pos > max_idx_pos)
                max_idx_pos = idx_grp->idx_pos;
            if (idx_grp->idx_tex > max_idx_tex)
                max_idx_tex = idx_grp->idx_tex;
            if (idx_grp->idx_nor > max_idx_nor)
                max_idx_nor = idx_grp->idx_nor;

            pos_stream[idx_grp->idx_base * 3 + 0] = pos_src[idx_grp->idx_pos * stride_pos + 0];
            pos_stream[idx_grp->idx_base * 3 + 1] = pos_src[idx_grp->idx_pos * stride_pos + 1];
            pos_stream[idx_grp->idx_base * 3 + 2] = pos_src[idx_grp->idx_pos * stride_pos + 2];

            tex_stream[idx_grp->idx_base * 2 + 0] = tex_src[idx_grp->idx_tex * stride_tex + 0];
            tex_stream[idx_grp->idx_base * 2 + 1] = 1.0f - tex_src[idx_grp->idx_tex * stride_tex + 1];

            nor_stream[idx_grp->idx_base * 3 + 0] = nor_src[idx_grp->idx_nor * stride_nor + 0];
            nor_stream[idx_grp->idx_base * 3 + 1] = nor_src[idx_grp->idx_nor * stride_nor + 1];
            nor_stream[idx_grp->idx_base * 3 + 2] = nor_src[idx_grp->idx_nor * stride_nor + 2];
        }
		idx_grp = &idx1;
		{
        ///elog("idx_base %d\n", idx_grp->idx_base);
            if (idx_grp->idx_base > max_idx_base)
                max_idx_base = idx_grp->idx_base;
            if (idx_grp->idx_pos > max_idx_pos)
                max_idx_pos = idx_grp->idx_pos;
            if (idx_grp->idx_tex > max_idx_tex)
                max_idx_tex = idx_grp->idx_tex;
            if (idx_grp->idx_nor > max_idx_nor)
                max_idx_nor = idx_grp->idx_nor;

            pos_stream[idx_grp->idx_base * 3 + 0] = pos_src[idx_grp->idx_pos * stride_pos + 0];
            pos_stream[idx_grp->idx_base * 3 + 1] = pos_src[idx_grp->idx_pos * stride_pos + 1];
            pos_stream[idx_grp->idx_base * 3 + 2] = pos_src[idx_grp->idx_pos * stride_pos + 2];

            tex_stream[idx_grp->idx_base * 2 + 0] = tex_src[idx_grp->idx_tex * stride_tex + 0];
            tex_stream[idx_grp->idx_base * 2 + 1] = 1.0f - tex_src[idx_grp->idx_tex * stride_tex + 1];

            nor_stream[idx_grp->idx_base * 3 + 0] = nor_src[idx_grp->idx_nor * stride_nor + 0];
            nor_stream[idx_grp->idx_base * 3 + 1] = nor_src[idx_grp->idx_nor * stride_nor + 1];
            nor_stream[idx_grp->idx_base * 3 + 2] = nor_src[idx_grp->idx_nor * stride_nor + 2];
        }
		idx_grp = &idx2;
		{
        ///elog("idx_base %d\n", idx_grp->idx_base);
            if (idx_grp->idx_base > max_idx_base)
                max_idx_base = idx_grp->idx_base;
            if (idx_grp->idx_pos > max_idx_pos)
                max_idx_pos = idx_grp->idx_pos;
            if (idx_grp->idx_tex > max_idx_tex)
                max_idx_tex = idx_grp->idx_tex;
            if (idx_grp->idx_nor > max_idx_nor)
                max_idx_nor = idx_grp->idx_nor;

            pos_stream[idx_grp->idx_base * 3 + 0] = pos_src[idx_grp->idx_pos * stride_pos + 0];
            pos_stream[idx_grp->idx_base * 3 + 1] = pos_src[idx_grp->idx_pos * stride_pos + 1];
            pos_stream[idx_grp->idx_base * 3 + 2] = pos_src[idx_grp->idx_pos * stride_pos + 2];

            tex_stream[idx_grp->idx_base * 2 + 0] = tex_src[idx_grp->idx_tex * stride_tex + 0];
            tex_stream[idx_grp->idx_base * 2 + 1] = 1.0f - tex_src[idx_grp->idx_tex * stride_tex + 1];

            nor_stream[idx_grp->idx_base * 3 + 0] = nor_src[idx_grp->idx_nor * stride_nor + 0];
            nor_stream[idx_grp->idx_base * 3 + 1] = nor_src[idx_grp->idx_nor * stride_nor + 1];
            nor_stream[idx_grp->idx_base * 3 + 2] = nor_src[idx_grp->idx_nor * stride_nor + 2];
        }

        idx_stream[i * 3 + 0] = idx0.idx_base;
        idx_stream[i * 3 + 1] = idx1.idx_base;
        idx_stream[i * 3 + 2] = idx2.idx_base;
    }
    ///MCheck();

    elog("max_idx_base %d\n", max_idx_base);
    elog("max_idx_pos %d\n", max_idx_pos);
    elog("max_idx_tex %d\n", max_idx_tex);
    elog("max_idx_nor %d\n", max_idx_nor);

    Mesh ret = Mesh_new();
    /**
    for (int i = 0; i < 6; i++)
    {
        elog("##nor %f %f %f", nor_stream[i * 3 + 0] = 0.0, nor_stream[i * 3 + 1] = 0.0, nor_stream[i * 3 + 2] = 1.0);
    }
    **/
    Mesh_build(ret, pos_stream, tex_stream, nor_stream, num_vtx, idx_stream, num_tri, Triangular);
    ///MCheck();
    return ret;
}
