#include "pch.h"
#include "shader_buffer.h"
#include "shader_node.h"
#include "shader_node_base.h"
#include "emem.h"
#include "array.h"
#include "estring.h"
#include "elog.h"
#include "eassert.h"
#include "renderer.h"

typedef struct _ref_entry
{
    ShaderBuffer buffer;
    const char* buf_ctor_file;
    uint buf_ctor_line;
} ref_entry;

static Tree g_ref_table = NULL;
void ref_init()
{
    g_ref_table = Tree_new(Vptr, Vptr, malloc, free);
}
void ref_insert(ShaderNode _node, ShaderBuffer _buf)
{
    ref_entry* ent = (ref_entry*)malloc(sizeof(ref_entry));
    vptr vkey = to_ptr(_node);
    ent->buffer = _buf;
    ent->buf_ctor_file = Minfo(to_ptr(_buf), &ent->buf_ctor_line);
    var key;
    var data;
    key.vptr_var = vkey;
    data.vptr_var = ent;
    Tree_insert(g_ref_table, key, data);
}
void ref_earse(ShaderNode _node)
{
    var key;
    key.vptr_var = to_ptr(_node);
    Tree_remove(g_ref_table, key);
}
void ref_log()
{
    Iterator iter = Tree_begin(g_ref_table);
    while (iter)
    {
        var data = Tree_get_value(iter);
        ref_entry* ent = (ref_entry*)data.vptr_var;
        elog("####buffer %x file %s line %d", (ref_ptr)to_ptr(ent->buffer), ent->buf_ctor_file, ent->buf_ctor_line);
        iter = Tree_next(iter);
    }
}
///=============================///
typedef struct
{
    element_type type;
    const char* type_string;
} element_type_entry;
typedef struct
{
    element_semantic sem;
    const char* sem_string;
    element_type sem_type;
    param_type pam_type;
} element_semantic_entry;
typedef struct
{
    param_type type;
    const char* param_string;
} param_type_entry;
typedef struct
{
    shader_object_type type;
    const char* shader_objectr_string;
} shader_object_entry;

element_type_entry element_type_table[] =
{
    { Float32_1, "float" },
    { Float32_2, "vec2"  },
    { Float32_3, "vec3"  },
    { Float32_4, "vec4"  },
};
element_semantic_entry element_semantic_table[] =
{
    { Position,       "Position",       Float32_4, Float32x4_Param },
    { Normal,         "Normal",         Float32_3, Float32x3_Param },
    { Color,          "Color",          Float32_4, Float32x4_Param },
    { Tangent,        "Tangent",        Float32_3, Float32x3_Param },
    { Binormal,       "Binormal",       Float32_3, Float32x3_Param },
    { TexCoord,       "TexCoord",       Float32_2, Float32x2_Param },
};
param_type_entry param_type_table[] =
{
    { Texture2D_Param,   "sampler2D" },
    { TextureCube_Param, "samplerCube" },
    { Int_Param,         "int" },
    { Float32_Param,     "float" },
    { Float32x2_Param,   "vec2" },
    { Float32x3_Param,   "vec3" },
    { Float32x4_Param,   "vec4" },
    { Matrix4x4_Param,   "mat4" },
    { Bool_Param,        "bool" },
};

shader_object_entry shader_object_table[] =
{
    { Int_Obj,         "int" },
    { Float_Obj,       "float" },
    { Float2_Obj,      "vec2" },
    { Float3_Obj,      "vec3" },
    { Float4_Obj,      "vec4" },
    { Texture2D_Obj,   "sampler2D" },
    { TextureCube_Obj, "samplerCube"},
    { Matrix4x4_Obj,   "mat4" },
    { Bool_Obj,        "bool" },
    { Void_Obj,        "void" },
};
///==============================ShaderBuffer========================================///
extern inline uint32 _get_element_type_entry_count()
{
    return sizeof(element_type_table) / sizeof(element_type_entry);
}
extern inline uint32 _get_element_semantic_entry_count()
{
    return sizeof(element_semantic_table) / sizeof(element_semantic_entry);
}
extern inline uint32 _get_param_type_entry_count()
{
    return sizeof(param_type_table) / sizeof(param_type_entry);
}
extern inline uint32 _get_shader_object_entry_count()
{
    return sizeof(shader_object_table) / sizeof(shader_object_entry);
}
///==============================ShaderBuffer========================================///
const char* get_element_type_string(element_type _type)
{
    for (uint32 i = 0; i < _get_element_type_entry_count(); i++)
    {
        if (element_type_table[i].type == _type)
            return element_type_table[i].type_string;
    }
    return NULL;
}

const char* get_element_semantic_string(element_semantic _sem)
{
    for (uint32 i = 0; i < _get_element_semantic_entry_count(); i++)
    {
        if (element_semantic_table[i].sem == _sem)
            return element_semantic_table[i].sem_string;
    }
    return NULL;
}

element_type get_element_semantic_type(element_semantic _sem)
{
    for (uint32 i = 0; i < _get_element_semantic_entry_count(); i++)
    {
        if (element_semantic_table[i].sem == _sem)
            return element_semantic_table[i].sem_type;
    }
    return EmptyType;
}

param_type get_element_param_type(element_semantic _sem)
{
    for (uint32 i = 0; i < _get_element_semantic_entry_count(); i++)
    {
        if (element_semantic_table[i].sem == _sem)
            return element_semantic_table[i].pam_type;
    }
    return EmptyParam;
}

const char* get_param_type_string(param_type _type)
{
    for (uint32 i = 0; i < _get_param_type_entry_count(); i++)
    {
        if (param_type_table[i].type == _type)
            return param_type_table[i].param_string;
    }
    return NULL;
}

const char* get_shader_object_string(shader_object_type _type)
{
    for (uint32 i = 0; i < _get_shader_object_entry_count(); i++)
    {
        if (shader_object_table[i].type == _type)
            return shader_object_table[i].shader_objectr_string;
    }
    return NULL;
}
///==============================ShaderBuffer========================================///
Tree ShaderBuffer_sell_param_source_object_tree(ShaderBuffer _sb)
{
    Tree ret = _sb->param_src_obj_tree;
    _sb->param_src_obj_tree = NULL;
    return ret;
}
void ShaderBuffer_push_shader_object(ShaderBuffer _sb, ShaderObject _so)
{
    vptr so_ptr = to_ptr(_so);
    if (so_ptr)
    {
        const char* name = ShaderObject_get_name(_so);
        var key, value;
        key.str_var = name;
        value.vptr_var = so_ptr;
        Tree_insert(_sb->shader_object_tree, key, value);
    }
}
void ShaderBuffer_push_immediate_shader_object(ShaderBuffer _sb, ShaderObject _so)
{
    vptr so_ptr = to_ptr(_so);
    if (so_ptr)
    {
        apush(_sb->imme_shader_objects, _so);
    }
}
const char* ShaderBuffer_get_output(ShaderBuffer _sb)
{
    return _sb->output;
}
void ShaderBuffer_print_shader_object(ShaderBuffer _sb)
{
    Iterator iter = Tree_begin(_sb->shader_object_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        ShaderObject so = {(struct _shader_object*)data.vptr_var};
        uint32 array_size, array_index;
        shader_object_type type = ShaderObject_get_type(so, &array_size, &array_index);
        const char* name = ShaderObject_get_name(so);
        if (array_size <= 1)
            printf("##object type %s name %s\n", get_shader_object_string(type), name);
        else
            printf("##object type %s name %s[%d]\n", get_shader_object_string(type), name, array_size);
        iter = Tree_next(iter);
    }
}
shader_buffer* ShaderBuffer_Init(struct _shader_buffer* _buf, const char* _file, uint _line)
{
    ///ShaderNode exec_node = {NULL};
    _buf->shader_node_prototype_tree = Tree_new(String, Vptr, Ealloc, Efree);
    _buf->param_src_obj_tree = Tree_new(Sint32, Vptr, Ealloc, Efree);
    ///_buf->shader_node_reference_table = array_new(ShaderNode, 5, exec_node);
    _buf->shader_object_tree = Tree_new(String, Vptr, Ealloc, Efree);
    _buf->function = (char*)EString_new("///function///\n");
    _buf->uniform = (char*)EString_new("///uniform///\n");
    _buf->varying = (char*)EString_new("///varying///\n");
    _buf->main = (char*)EString_new("///main///\nvoid main(void)\n{\n    ");
    _buf->output = (char*)EString_new("");

    CircuitBoard_Init(&_buf->circuit_board);
    ShaderObject null_obj = {NULL};
    _buf->imme_shader_objects = array_new(ShaderObject, 20, null_obj);

    return _buf;
}

void ShaderBuffer_Dest(struct _shader_buffer* _buf, const char* _file, uint _line)
{
    Iterator iter = NULL;
    if (_buf->param_src_obj_tree)
    {
        iter = Tree_begin(_buf->param_src_obj_tree);
        while (iter)
        {
            var data = Tree_get_value(iter);
            ShaderObject obj = {(struct _shader_object*)data.vptr_var};
            ShaderObject_delete(obj);
            iter = Tree_next(iter);
        }
        Tree_Dest(_buf->param_src_obj_tree);
    }

    iter = Tree_begin(_buf->shader_node_prototype_tree);
    while (iter)
    {
        var v = Tree_get_value(iter);
        ShaderNode node = {(ShaderNode)v.vptr_var};
        ShaderNode_delete(node);
        iter = Tree_next(iter);
    }
    iter = Tree_begin(_buf->shader_object_tree);
    while (iter)
    {
        var v = Tree_get_value(iter);
        ShaderObject obj = {(struct _shader_object*)v.vptr_var};
        ShaderObject_delete(obj);
        iter = Tree_next(iter);
    }
    Tree_Dest(_buf->shader_node_prototype_tree);
    Tree_Dest(_buf->shader_object_tree);

    EString_delete(_buf->function);
    EString_delete(_buf->uniform);
    EString_delete(_buf->varying);
    EString_delete(_buf->main);
    EString_delete(_buf->output);

    CircuitBoard_Dest(&_buf->circuit_board);
    uint n = array_n(_buf->imme_shader_objects);
    for (uint i = 0; i < n; i++)
    {
        ShaderObject_delete(_buf->imme_shader_objects[i]);
    }
    array_delete(_buf->imme_shader_objects);
}

void ShaderBuffer_complete(struct _shader_buffer* _buf)
{
    /**
    char mbuf[STRING_BUFFER_SIZE];
    mbuf[0] = 0x00;
    char* tmp = mbuf;
    **/
    string_buffer_new(STRING_BUFFER_SIZE);

    ///tmp += sprintf(tmp, "%s", _buf->function);
    sbuf_printf("%s", _buf->function);
    EString_delete(_buf->function);
#ifndef USE_RECOMPILE
    Iterator iter = Tree_begin(_buf->shader_node_prototype_tree);
    while(iter)
    {
        var data = Tree_get_value(iter);
        ShaderNode sn = {(struct _shader_node*)data.vptr_var};
        if ( to_ptr(sn) != NULL )
        {
            const char* compiled = ShaderNode_compile_function_declaration(sn);
            const char* func = ShaderNode_get_function(sn);
            ///tmp += sprintf(tmp, "%s%s", compiled, func);
            sbuf_printf("%s%s", compiled, func);
            EString_delete(compiled);
        }
        iter = Tree_next(iter);
    }
#endif
    ///_buf->function = EString_new(mbuf);
    _buf->function = (char*)EString_new(get_string_buffer);
    ///tmp = mbuf;
    string_buffer_clear;
    ///tmp += sprintf(tmp, "%s%s%s%s", _buf->uniform, _buf->varying, _buf->function, _buf->main);
    sbuf_printf("%s%s%s%s", _buf->uniform, _buf->varying, _buf->function, _buf->main);
    {
        char* buf = CircuitBoard_compile(&_buf->circuit_board);
        ///tmp += sprintf(tmp, "%s", buf);
        sbuf_printf("%s", buf);
        EString_delete(buf);
    }

    ///tmp += sprintf(tmp, "\n}\n");
    sbuf_printf("%s", "\n}\n");
    EString_delete(_buf->output);

    ///_buf->output = EString_new(mbuf);
    _buf->output = (char*)EString_new(get_string_buffer);
}

ShaderObject ShaderBuffer_add_varying(ShaderBuffer _sb, param_type _type, const char* _vary, sint32 _src)
{
    const char* type = get_param_type_string(_type);
    ShaderObject ret = {NULL};
    if (type == NULL)
    {
        return ret;
    }

    switch (_type)
    {
    case Texture2D_Param:
        ret = ShaderObject_new(Texture2D_Obj, _vary, 1);
        break;
    case Int_Param:
        ret = ShaderObject_new(Int_Obj, _vary, 1);
        break;
    case Float32_Param:
        ret = ShaderObject_new(Float_Obj, _vary, 1);
        break;
    case Float32x2_Param:
        ret = ShaderObject_new(Float2_Obj, _vary, 1);
        break;
    case Float32x3_Param:
        ret = ShaderObject_new(Float3_Obj, _vary, 1);
        break;
    case Float32x4_Param:
        ret = ShaderObject_new(Float4_Obj, _vary, 1);
        break;
    case Matrix4x4_Param:
        ret = ShaderObject_new(Matrix4x4_Obj, _vary, 1);
        break;
    case Bool_Param:
        ret = ShaderObject_new(Bool_Obj, _vary, 1);
        break;
    default:
        return ret;
    }
/**
    char mbuf[STRING_BUFFER_SIZE];
    mbuf[0] = 0x00;
    char* tmp = mbuf;
**/
    string_buffer_new(STRING_BUFFER_SIZE);

    ///tmp += sprintf(tmp, "%s", _sb.self->varying);
    sbuf_printf("%s", _sb->varying);
    EString_delete(_sb->varying);
    ///tmp += sprintf(tmp, "varying %s %s;\n", type, _vary);
    sbuf_printf("varying %s %s;\n", type, _vary);
    ///_sb.self->varying = EString_new(mbuf);
    _sb->varying = (char*)EString_new(get_string_buffer);

    ShaderBuffer_push_shader_object(_sb, ret);

    var key, data;
    key.uint32_var = _src;
    ShaderObject cloned_obj = ShaderObject_clone(ret);
    data.vptr_var = to_ptr(cloned_obj);
    Tree_insert(_sb->param_src_obj_tree, key, data);
    return ret;
}

ShaderObject _ShaderBuffer_add_uniform(ShaderBuffer _sb, param_type _type, const char* _unif, uint32 _array_size, sint32 _src,
                                       const char* _file, uint _line)
{
    const char* type = get_param_type_string(_type);
    ShaderObject ret = {NULL};
    if (type == NULL)
    {
        return ret;
    }
    switch (_type)
    {
    case Texture2D_Param:
        ret = ShaderObject_new(Texture2D_Obj, _unif, _array_size);
        break;
    case TextureCube_Param:
        ret = ShaderObject_new(TextureCube_Obj, _unif, _array_size);
        break;
    case Int_Param:
        ret = ShaderObject_new(Int_Obj, _unif, _array_size);
        break;
    case Float32_Param:
        ret = ShaderObject_new(Float_Obj, _unif, _array_size);
        break;
    case Float32x2_Param:
        ret = ShaderObject_new(Float2_Obj, _unif, _array_size);
        break;
    case Float32x3_Param:
        ret = ShaderObject_new(Float3_Obj, _unif, _array_size);
        break;
    case Float32x4_Param:
        ret = ShaderObject_new(Float4_Obj, _unif, _array_size);
        break;
    case Bool_Param:
        ret = ShaderObject_new(Bool_Obj, _unif, _array_size);
        break;
    case Matrix4x4_Param:
        ret = ShaderObject_new(Matrix4x4_Obj, _unif, _array_size);
        break;
    default:
        return ret;
    }
/**
    char mbuf[STRING_BUFFER_SIZE];
    mbuf[0] = 0x00;
    char* tmp = mbuf;
**/
    string_buffer_new(STRING_BUFFER_SIZE);

    if (_array_size <= 1)
    {
        sbuf_printf("%s", _sb->uniform);
        EString_delete(_sb->uniform);
		sbuf_printf("uniform %s %s;\n", type, _unif);
        _sb->uniform = (char*)EString_new(get_string_buffer);
    }
    else
    {
        sbuf_printf("%s", _sb->uniform);
        EString_delete(_sb->uniform);
		sbuf_printf("uniform %s %s[%d];\n", type, _unif, _array_size);
        _sb->uniform = (char*)EString_new(get_string_buffer);
    }
    var key, data;
    key.uint32_var = _src;
    EAssert(!Tree_find(_sb->param_src_obj_tree, key, &data), "%s", "insert uniform fail!");

    ShaderBuffer_push_shader_object(_sb, ret);

    ShaderObject cloned_obj = _ShaderObject_clone(ret, _file, _line);
    data.vptr_var = to_ptr(cloned_obj);
    Tree_insert(_sb->param_src_obj_tree, key, data);
    return ret;
}

ShaderObject _ShaderBuffer_add_uniform_from_renderer(ShaderBuffer _self, Renderer* _rdr, sint32 _id, const char* _unif,
                                                     const char* _file, uint _line)
{
    RendererParamEntry ent = _rdr->get_param_entry(_id);
    if (!ent)
    {
        ShaderObject null_obj = {NULL};
        return null_obj;
    }
    return _ShaderBuffer_add_uniform(_self, ent->type, _unif, ent->array_size, _id, _file, _line);
}

ShaderObject ShaderBuffer_new_object(ShaderBuffer _sb, shader_object_type _type, const char* _name, uint32 _array_size)
{
    /**
    char mbuf[STRING_BUFFER_SIZE];
    mbuf[0] = 0x00;
    char* tmp = mbuf;
    tmp += sprintf(tmp, "%s", _sb.self->main);
    **/
    string_buffer_new(STRING_BUFFER_SIZE);
    sbuf_printf("%s", _sb->main);

    const char* type = get_shader_object_string(_type);
    ShaderObject ret = {NULL};
    if (type == NULL)
    {
        return ret;
    }

    ret = ShaderObject_new(_type, _name, _array_size);
    ShaderObject so = ShaderBuffer_find_object(_sb, _name);
    if (to_ptr(so))
    {
        ShaderObject tmp = ret;
        if (ShaderObject_equal(ret, so))
            ret = so;
        else
            ret.self = NULL;
        ShaderObject_delete(tmp);
        return ret;
    }

    if (_array_size <= 1)
    {
        ///tmp += sprintf(tmp, "%s %s;\n    ", type, _name);
        sbuf_printf("%s %s;\n    ", type, _name);
        EString_delete(_sb->main);
        ///_sb.self->main = EString_new(mbuf);
        _sb->main = (char*)EString_new(get_string_buffer);
    }
    else
    {
        ///tmp += sprintf(tmp, "%s %s[%d];\n    ", type, _name, _array_size);
        sbuf_printf("%s %s[%d];\n    ", type, _name, _array_size);
        EString_delete(_sb->main);
        ///_sb.self->main = EString_new(mbuf);
        _sb->main = (char*)EString_new(get_string_buffer);
    }

    ShaderBuffer_push_shader_object(_sb, ret);
    return ret;
}

ShaderObject ShaderBuffer_new_immediate_float_object(ShaderBuffer _sb, float _ft)
{
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "%f", _ft);
    ShaderObject ret = ShaderObject_new(Float_Obj, mbuf, 1);
    ShaderBuffer_push_immediate_shader_object(_sb, ret);
    return ret;
}

ShaderObject ShaderBuffer_new_immediate_int_object(ShaderBuffer _sb, int _i)
{
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "%d", _i);
    ShaderObject ret = ShaderObject_new(Int_Obj, mbuf, 1);
    ShaderBuffer_push_immediate_shader_object(_sb, ret);
    return ret;
}

ShaderObject ShaderBuffer_sample_texture2d_rgba(ShaderBuffer _sb, ShaderObject _tex, ShaderObject _uv)
{
	ShaderObject ret = ShaderObject_sample_texture2d_rgba(_tex, _uv);
	ShaderBuffer_push_immediate_shader_object(_sb, ret);
	return ret;
}

ShaderObject ShaderBuffer_sample_texture2d_rgb(ShaderBuffer _sb, ShaderObject _tex, ShaderObject _uv)
{
	ShaderObject ret = ShaderObject_sample_texture2d_rgb(_tex, _uv);
	ShaderBuffer_push_immediate_shader_object(_sb, ret);
	return ret;
}

void ShaderBuffer_add_prototype_node(ShaderBuffer _sb, ShaderNode _sn)
{
    if (ShaderNode_get_name(_sn))
    {
        ShaderNodeBase snb = (ShaderNodeBase)_sn;
        ShaderNode sn = (ShaderNode)snb->clone_proc((ShaderNodeBase)_sn, __FILE__, __LINE__);
        var key;
        var data;
        key.str_var = ShaderNode_get_name(sn);
        data.vptr_var = to_ptr(sn);
        Tree_insert(_sb->shader_node_prototype_tree, key, data);
    }
}

ShaderNode _ShaderBuffer_add_reference_node(ShaderBuffer _sb, const char* _name, const char* _file, uint _line)
{
    var key;
    var data;
    key.str_var = _name;
    if (Tree_find(_sb->shader_node_prototype_tree, key, &data))
    {
        /**
        ShaderNode sn = {(struct _shader_node*)data.vptr_var};
        ShaderNode csn = _ShaderNode_clone(sn, ShaderNode_compile, _file, _line);
        _sb.self->shader_node_reference_table = array_push(_sb.self->shader_node_reference_table, csn);
        **/
        ///return csn;
        ShaderNode sn = {(struct _shader_node*)data.vptr_var};
        return CircuitBoard_add_reference_node(&_sb->circuit_board, sn);
    }
    else
    {
        ShaderNode ret = {NULL};
        return ret;
    }
}

void _ShaderBuffer_add_branch_node(ShaderBuffer _sb, BranchNode _bn, const char* _file, uint _line)
{
    _CircuitBoard_add_reference_node(&_sb->circuit_board, (ShaderNode)_bn, _file, _line);
}

ShaderObject ShaderBuffer_find_object(ShaderBuffer _sb, const char* _name)
{
    var key;
    var data;
    key.str_var = _name;
    if (Tree_find(_sb->shader_object_tree, key, &data))
    {
        ShaderObject ret = {(struct _shader_object*)data.vptr_var};
        return ret;
    }
    else
    {
        ShaderObject ret = {NULL};
        return ret;
    }
}
