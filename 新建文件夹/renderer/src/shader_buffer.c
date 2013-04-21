#include "shader_buffer.h"
#include "shader_node.h"
#include "mem.h"
#include "array.h"
#include "estring.h"
#include "elog.h"
typedef struct _ref_entry
{
    ShaderBuffer buffer;
    const char* buf_ctor_file;
    uint buf_ctor_line;
} ref_entry;

static Tree g_ref_table = NULL;
void ref_init()
{
    ///ref_entry null_ent = {{NULL}, NULL, 0, {NULL}};
    ///g_ref_table = array_new(ref_entry, 20, null_ent);
    g_ref_table = Tree_new(Vptr, Vptr, malloc, free);
}
void ref_insert(ShaderNode _node, ShaderBuffer _buf)
{
    ref_entry* ent = malloc(sizeof(ref_entry));
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
        ref_entry* ent = data.vptr_var;
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
    { Float32_2, "vec2" },
    { Float32_3, "vec3" },
    { Float32_4, "vec4" },
};
element_semantic_entry element_semantic_table[] =
{
    { Position, "Position", Float32_3, Float32x3_Param },
    { Normal,   "Normal",   Float32_3, Float32x3_Param },
    { Color,    "Color",    Float32_4, Float32x4_Param },
    { Tangent,  "Tangent",  Float32_3, Float32x3_Param },
    { Binormal, "Binormal", Float32_3, Float32x3_Param },
    { TexCoord, "Texcoord", Float32_2, Float32x2_Param },
};
param_type_entry param_type_table[] =
{
    { Texture2D_Param, "sampler2D" },
    { Int_Param,       "int" },
    { Float32_Param,   "float" },
    { Float32x2_Param, "vec2" },
    { Float32x3_Param, "vec3" },
    { Float32x4_Param, "vec4" },
    { Matrix4x4_Param, "mat4" },
};

shader_object_entry shader_object_table[] =
{
    { Int_Obj,       "int" },
    { Float_Obj,     "float" },
    { Float2_Obj,    "vec2" },
    { Float3_Obj,    "vec3" },
    { Float4_Obj,    "vec4" },
    { Texture2D_Obj, "sampler2D" },
    { Matrix4x4_Obj, "mat4" },
    { Void_Obj,      "void" },
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
void ShaderBuffer_push_shader_object(ShaderBuffer _sb, ShaderObject _so)
{
    vptr so_ptr = to_ptr(_so);
    if (so_ptr)
    {
        const char* name = ShaderObject_get_name(_so);
        var key, value;
        key.str_var = (vptr)name;
        value.vptr_var = so_ptr;
        Tree_insert(_sb.self->shader_object_tree, key, value);
    }
}
void ShaderBuffer_print_shader_object(ShaderBuffer _sb)
{
    Iterator iter = Tree_begin(_sb.self->shader_object_tree);
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
    ShaderNode exec_node = {NULL};
    ///ShaderObject exec_obj = {NULL};
    _buf->shader_node_prototype_tree = Tree_new(String, Vptr, Ealloc, Efree);
    _buf->shader_node_reference_table = array_new(ShaderNode, 5, exec_node);
    ///_buf->main_var_table = array_new(ShaderObject, 5, exec_obj);
    _buf->shader_object_tree = Tree_new(String, Vptr, Ealloc, Efree);
/**
    _buf->function = _EString_new("///function///\n", _file, _line);
    _buf->uniform = _EString_new("///uniform///\n", _file, _line);
    _buf->varying = _EString_new("///varying///\n", _file, _line);
    _buf->main = _EString_new("///main///\nvoid main(void)\n{\n    ", _file, _line);
    _buf->output = _EString_new("", _file, _line);
**/
    _buf->function = EString_new("///function///\n");
    _buf->uniform = EString_new("///uniform///\n");
    _buf->varying = EString_new("///varying///\n");
    _buf->main = EString_new("///main///\nvoid main(void)\n{\n    ");
    _buf->output = EString_new("");

    return _buf;
}

void ShaderBuffer_Dest(struct _shader_buffer* _buf, const char* _file, uint _line)
{
    Iterator iter = Tree_begin(_buf->shader_node_prototype_tree);
    while (iter)
    {
        var v = Tree_get_value(iter);
        ShaderNode node = {v.vptr_var};
        ShaderNode_delete(node);
        iter = Tree_next(iter);
    }
    iter = Tree_begin(_buf->shader_object_tree);
    while (iter)
    {
        var v = Tree_get_value(iter);
        ShaderObject obj = {v.vptr_var};
        ShaderObject_delete(obj);
        iter = Tree_next(iter);
    }
    Tree_Dest(_buf->shader_node_prototype_tree);
    Tree_Dest(_buf->shader_object_tree);

    for (uint i = 0; i < array_n(_buf->shader_node_reference_table); i++)
    {
        ///ShaderNode node = array_safe_get(_buf->shader_node_reference_table, i);
        ShaderNode node = _buf->shader_node_reference_table[i];
        ShaderNode_delete(node);
    }
    array_delete(_buf->shader_node_reference_table);
/**
    _EString_delete(_buf->function, _file, _line);
    _EString_delete(_buf->uniform, _file, _line);
    _EString_delete(_buf->varying, _file, _line);
    _EString_delete(_buf->main, _file, _line);
    _EString_delete(_buf->output, _file, _line);
**/
    EString_delete(_buf->function);
    EString_delete(_buf->uniform);
    EString_delete(_buf->varying);
    EString_delete(_buf->main);
    EString_delete(_buf->output);
}

void ShaderBuffer_release(struct _shader_buffer* _buf)
{
    char mbuf[4 * 1024];
    ///sprintf(mbuf, "");
    mbuf[0] = 0x00;
    char* tmp = mbuf;

    tmp += sprintf(tmp, "%s", _buf->function);
    EString_delete(_buf->function);

    Iterator iter = Tree_begin(_buf->shader_node_prototype_tree);
    while(iter)
    {
        var data = Tree_get_value(iter);
        ShaderNode sn = {(struct _shader_node*)data.vptr_var};
        if ( to_ptr(sn) != NULL )
        {
            const char* compiled = ShaderNode_compile_function_declaration(sn);
            const char* func = ShaderNode_get_function(sn);
            tmp += sprintf(tmp, "%s%s", compiled, func);

            EString_delete(compiled);
        }
        iter = Tree_next(iter);
    }
    _buf->function = EString_new(mbuf);
    tmp = mbuf;

    tmp += sprintf(tmp, "%s%s%s%s", _buf->uniform, _buf->varying, _buf->function, _buf->main);
    uint n = array_n(_buf->shader_node_reference_table);
    for (uint i = 0; i < n; i++)
    {
        ///ShaderNode node = array_safe_get(_buf->shader_node_reference_table, i);
        ShaderNode node = _buf->shader_node_reference_table[i];
        const char* compiled = ShaderNode_compile(node);
        tmp += sprintf(tmp, "%s\n    ", compiled);
        EString_delete(compiled);
    }
    tmp += sprintf(tmp, "\n}\n");
    EString_delete(_buf->output);
    _buf->output = EString_new(mbuf);
}

ShaderObject ShaderBuffer_add_varying(ShaderBuffer _sb, param_type _type, const char* _vary)
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
    default:
        return ret;
    }

    char mbuf[4 * 1024];
    ///sprintf(mbuf, "");
    mbuf[0] = 0x00;
    char* tmp = mbuf;

    tmp += sprintf(tmp, "%s", _sb.self->varying);
    EString_delete(_sb.self->varying);
    tmp += sprintf(tmp, "varying %s %s;\n", type, _vary);
    _sb.self->varying = EString_new(mbuf);

    ShaderBuffer_push_shader_object(_sb, ret);
    return ret;
}

ShaderObject ShaderBuffer_add_uniform(ShaderBuffer _sb, param_type _type, const char* _unif, uint32 _array_size)
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
    case Matrix4x4_Param:
        ret = ShaderObject_new(Matrix4x4_Obj, _unif, _array_size);
        break;
    default:
        return ret;
    }

    char mbuf[4 * 1024];
    ///sprintf(mbuf, "");
    mbuf[0] = 0x00;
    char* tmp = mbuf;

    if (_array_size <= 1)
    {
        tmp += sprintf(tmp, "%s", _sb.self->uniform);
        EString_delete(_sb.self->uniform);
        tmp += sprintf(tmp, "uniform %s %s;\n", type, _unif);
        _sb.self->uniform = EString_new(mbuf);
    }
    else
    {
        tmp += sprintf(tmp, "%s", _sb.self->uniform);
        EString_delete(_sb.self->uniform);
        tmp += sprintf(tmp, "uniform %s %s[%d];\n", type, _unif, _array_size);
        _sb.self->uniform = EString_new(mbuf);
    }

    ShaderBuffer_push_shader_object(_sb, ret);
    return ret;
}

ShaderObject ShaderBuffer_new_object(ShaderBuffer _sb, shader_object_type _type, const char* _name, uint32 _array_size)
{
    char mbuf[4 * 1024];
    ///sprintf(mbuf, "");
    mbuf[0] = 0x00;
    char* tmp = mbuf;
    tmp += sprintf(tmp, "%s", _sb.self->main);

    const char* type = get_shader_object_string(_type);
    ShaderObject ret = {NULL};
    if (type == NULL)
    {
        return ret;
    }
    ret = ShaderObject_new(_type, _name, _array_size);

    if (_array_size <= 1)
    {
        tmp += sprintf(tmp, "%s %s;\n    ", type, _name);
        EString_delete(_sb.self->main);
        _sb.self->main = EString_new(mbuf);
    }
    else
    {
        tmp += sprintf(tmp, "%s %s[%d];\n    ", type, _name, _array_size);
        EString_delete(_sb.self->main);
        _sb.self->main = EString_new(mbuf);
    }

    ShaderBuffer_push_shader_object(_sb, ret);
    return ret;
}

void ShaderBuffer_add_prototype_node(ShaderBuffer _sb, ShaderNode _sn)
{
    if (ShaderNode_get_name(_sn))
    {
        ShaderNode sn = ShaderNode_clone(_sn);
        var key;
        var data;
        key.str_var = (vptr)ShaderNode_get_name(sn);
        data.vptr_var = to_ptr(sn);
        Tree_insert(_sb.self->shader_node_prototype_tree, key, data);
    }
}

ShaderNode _ShaderBuffer_add_reference_node(ShaderBuffer _sb, const char* _name, const char* _file, uint _line)
{
    var key;
    var data;
    key.str_var = (vptr)_name;
    if (Tree_find(_sb.self->shader_node_prototype_tree, key, &data))
    {
        ShaderNode sn = {(struct _shader_node*)data.vptr_var};
        ShaderNode csn = _ShaderNode_clone(sn, _file, _line);
        _sb.self->shader_node_reference_table = array_push(_sb.self->shader_node_reference_table, csn);
        return csn;
    }
    else
    {
        ShaderNode ret = {NULL};
        return ret;
    }
}

ShaderObject ShaderBuffer_find_object(ShaderBuffer _sb, const char* _name)
{
    var key;
    var data;
    key.str_var = (vptr)_name;
    if (Tree_find(_sb.self->shader_object_tree, key, &data))
    {
        ShaderObject ret = {data.vptr_var};
        return ret;
    }
    else
    {
        ShaderObject ret = {NULL};
        return ret;
    }
}
