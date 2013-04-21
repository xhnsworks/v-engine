#include "shader_node.h"
#include "mem.h"
#include "array.h"
#include "estring.h"
#include "elog.h"
#include "shader_buffer.h"
typedef struct _shader_node
{
    char* node_name;
    ShaderObject result;
    ShaderObject* input_param_table;
    ShaderObject* output_param_table;
    ShaderObject result_link;
    ShaderObject* input_links;
    ShaderObject* output_links;
    char* function;
} shader_node;

const char* _compile_params(ShaderNode _sn)
{
    char mbuf[4 * 1024];
    ///sprintf(mbuf, "");
    mbuf[0] = 0x00;
    char* tmp = mbuf;

    if (array_n(_sn.self->input_param_table))
    {
        for (uint32 i = 0; i < array_n(_sn.self->input_param_table); i++)
        {
            ShaderObject so = array_safe_get(_sn.self->input_param_table, i);

            uint32 array_size, array_index;

            const char* obj_type = get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            const char* obj_name = ShaderObject_get_name(so);

            if (array_size <= 1)
            {
                if (i < array_n(_sn.self->input_param_table) - 1)
                    tmp += sprintf(tmp, "%s %s, ", obj_type, obj_name);
                else
                    tmp += sprintf(tmp, "%s %s"  , obj_type, obj_name);
            }
            else
            {
                if (i < array_n(_sn.self->input_param_table) - 1)
                    tmp += sprintf(tmp, "%s[%d] %s, ", obj_type, array_size, obj_name);
                else
                    tmp += sprintf(tmp, "%s[%d] %s"  , obj_type, array_size, obj_name);
            }
        }
    }

    if (array_n(_sn.self->output_param_table))
    {
        if (array_n(_sn.self->input_param_table))
        {
            tmp += sprintf(tmp, ", ");
        }

        for (uint32 i = 0; i < array_n(_sn.self->output_param_table); i++)
        {
            ShaderObject so = array_safe_get(_sn.self->output_param_table, i);

            uint32 array_size, array_index;

            const char* obj_type = get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            const char* obj_name = ShaderObject_get_name(so);

            if (array_size <= 1)
            {
                if (i < array_n(_sn.self->output_param_table) - 1)
                    tmp += sprintf(tmp, "out %s %s, ", obj_type, obj_name);
                else
                    tmp += sprintf(tmp, "out %s %s"  , obj_type, obj_name);
            }
            else
            {
                if (i < array_n(_sn.self->input_param_table) - 1)
                    tmp += sprintf(tmp, "out %s[%d] %s, ", obj_type, array_size, obj_name);
                else
                    tmp += sprintf(tmp, "out %s[%d] %s"  , obj_type, array_size, obj_name);
            }
        }
    }
    return EString_new(mbuf);
}

const char* _compile_links(ShaderNode _sn)
{
    char mbuf[4 * 1024];
    ///sprintf(mbuf, "");
    mbuf[0] = 0x00;
    char* tmp = mbuf;

    if (array_n(_sn.self->input_links))
    {
        for (uint32 i = 0; i < array_n(_sn.self->input_links); i++)
        {
            ShaderObject so = array_safe_get(_sn.self->input_links, i);

            uint32 array_size, array_index;

            ///const char* obj_type = get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            const char* obj_name = ShaderObject_get_name(so);

            if (array_size <= 1 || array_index == INVALID_ARRAY_INDEX)
            {
                if (i < array_n(_sn.self->input_links) - 1)
                    tmp += sprintf(tmp, "%s, ", obj_name);
                else
                    tmp += sprintf(tmp, "%s"  , obj_name);
            }
            else
            {
                if (i < array_n(_sn.self->input_links) - 1)
                    tmp += sprintf(tmp, "%s[%d], ", obj_name, array_index);
                else
                    tmp += sprintf(tmp, "%s[%d]"  , obj_name, array_index);
            }
        }
    }

    if (array_n(_sn.self->output_links))
    {
        if (array_n(_sn.self->input_links))
        {
            tmp += sprintf(tmp, ", ");
        }
        for (uint32 i = 0; i < array_n(_sn.self->output_links); i++)
        {
            ShaderObject so = array_safe_get(_sn.self->output_links, i);

            uint32 array_size, array_index;

            ///const char* obj_type = get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            const char* obj_name = ShaderObject_get_name(so);

            if (array_size <= 1 || array_index == INVALID_ARRAY_INDEX)
            {
                if (i < array_n(_sn.self->output_links) - 1)
                    tmp += sprintf(tmp, "%s, ", obj_name);
                else
                    tmp += sprintf(tmp, "%s"  , obj_name);
            }
            else
            {
                if (i < array_n(_sn.self->output_links) - 1)
                    tmp += sprintf(tmp, "%s[%d], ", obj_name, array_index);
                else
                    tmp += sprintf(tmp, "%s[%d]"  , obj_name, array_index);
            }

        }

    }
    return EString_new(mbuf);
}
///==============================ShaderNode========================================///
shader_node* ShaderNode_Init(struct _shader_node* _sn, const char* _file, uint _line)
{
    _sn->node_name = NULL;
    ShaderObject exec_so = {NULL};
    _sn->result = ShaderObject_new(Void_Obj, "", 1);
    _sn->input_param_table = array_new(ShaderObject, 5, exec_so);
    _sn->output_param_table = array_new(ShaderObject, 5, exec_so);
    _sn->result_link = exec_so;
    _sn->input_links = array_new(ShaderObject, 5, exec_so);
    _sn->output_links = array_new(ShaderObject, 5, exec_so);
    _sn->function = NULL;
    return _sn;
}

void ShaderNode_Dest(shader_node* _sn, const char* _file, uint _line)
{
    if (_sn->node_name)
        EString_delete(_sn->node_name);
    if (_sn->function)
        EString_delete(_sn->function);

    ShaderObject_delete(_sn->result);
    if ( to_ptr(_sn->result_link) != NULL )
        ShaderObject_delete(_sn->result_link);

    for (uint32 i = 0; i < array_n(_sn->input_param_table); i++)
    {
        ShaderObject so = array_safe_get(_sn->input_param_table, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->input_param_table);

    for (uint32 i = 0; i < array_n(_sn->output_param_table); i++)
    {
        ShaderObject so = array_safe_get(_sn->output_param_table, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->output_param_table);

    for (uint32 i = 0; i < array_n(_sn->input_links); i++)
    {
        ShaderObject so = array_safe_get(_sn->input_links, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->input_links);

    for (uint32 i = 0; i < array_n(_sn->output_links); i++)
    {
        ShaderObject so = array_safe_get(_sn->output_links, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->output_links);
}

ShaderNode _ShaderNode_new(const char* _file, uint _line)
{
    ShaderNode ret;
    ret.self = _Malloc(sizeof(shader_node), _file, _line);
    ret.self = ShaderNode_Init(ret.self, __FILE__, __LINE__);
    return ret;
}

void _ShaderNode_delete(ShaderNode _sn, const char* _file, uint _line)
{
    ShaderNode_Dest(_sn.self, _file, _line);
    Mfree(_sn.self);
}

void ShaderNode_set_return_type(ShaderNode _sn, shader_object_type _type, uint32 _array_size)
{
    ShaderObject_set_type(_sn.self->result, _type, _array_size, INVALID_ARRAY_INDEX);
}

void ShaderNode_add_input_param(ShaderNode _sn, shader_object_type _type, const char* _pam_name, uint32 _array_size)
{
    ShaderObject so = ShaderObject_new(_type, _pam_name, _array_size);
    _sn.self->input_param_table = array_push(_sn.self->input_param_table, so);
}

void ShaderNode_add_output_param(ShaderNode _sn, shader_object_type _type, const char* _pam_name, uint32 _array_size)
{
    ShaderObject so = ShaderObject_new(_type, _pam_name, _array_size);
    _sn.self->output_param_table = array_push(_sn.self->output_param_table, so);
}

void ShaderNode_add_input_link(ShaderNode _sn, ShaderObject _so, uint32 _array_index)
{
    ShaderObject so = ShaderObject_clone(_so);
    ShaderObject_set_index(so, _array_index);
    _sn.self->input_links = array_push(_sn.self->input_links, so);
}

void ShaderNode_add_output_link(ShaderNode _sn, ShaderObject _so, uint32 _array_index)
{
    ShaderObject so = ShaderObject_clone(_so);
    ShaderObject_set_index(so, _array_index);
    _sn.self->output_links = array_push(_sn.self->output_links, so);
}

void ShaderNode_set_result_link(ShaderNode _sn, ShaderObject _so, uint32 _array_index)
{
    if ( to_ptr(_sn.self->result_link) != NULL )
    {
        ShaderObject_delete(_sn.self->result_link);
    }
    ShaderObject so = ShaderObject_clone(_so);
    ShaderObject_set_index(so, _array_index);
    _sn.self->result_link = so;
}

void ShaderNode_set_function(ShaderNode _sn, const char* _func)
{
    _sn.self->function = EString_new(_func);
}

void ShaderNode_clear_links(ShaderNode _sn)
{
    ShaderObject exec_so = {NULL};
    if ( to_ptr(_sn.self->result_link) != NULL )
    {
        ShaderObject_delete(_sn.self->result_link);
        _sn.self->result_link = exec_so;
    }

    for (uint32 i = 0; i < array_n(_sn.self->input_links); i++)
    {
        ShaderObject so = array_safe_get(_sn.self->input_links, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn.self->input_links);
    _sn.self->input_links = array_new(ShaderObject, 5, exec_so);

    for (uint32 i = 0; i < array_n(_sn.self->output_links); i++)
    {
        ShaderObject so = array_safe_get(_sn.self->output_links, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn.self->output_links);
    _sn.self->output_links = array_new(ShaderObject, 5, exec_so);
}

const char* ShaderNode_compile(ShaderNode _sn)
{
    char* tmp0 = EString_new("");
    if ( to_ptr(_sn.self->result_link) != NULL )
    {
        char* tmp = tmp0;
        tmp0 = EString_add( tmp0, ShaderObject_get_name(_sn.self->result_link) );
        EString_delete(tmp);
        tmp = tmp0;
        tmp0 = EString_add( tmp0, " = ");
        EString_delete(tmp);
    }

    char* tmp1 = EString_add(tmp0, _sn.self->node_name);
    EString_delete(tmp0);
    char* tmp = tmp1;
    tmp1 = EString_add(tmp1, "( ");
    EString_delete(tmp);

    tmp0 = (char*)_compile_links(_sn);
    tmp = tmp1;
    tmp1 = EString_add(tmp1, tmp0);
    EString_delete(tmp);
    EString_delete(tmp0);

    tmp0 = EString_add(tmp1, " );");
    EString_delete(tmp1);

    return tmp0;
}

const char* ShaderNode_compile_function_declaration(ShaderNode _sn)
{
    char mbuf[4 * 1024];
    ///sprintf(mbuf, "");
    mbuf[0] = 0x00;
    char* tmp = mbuf;
    uint32 array_size, array_index;
    const char* obj_type = get_shader_object_string( ShaderObject_get_type(_sn.self->result, &array_size, &array_index) );
    const char* node_name = _sn.self->node_name;
    const char* params = _compile_params(_sn);
    if (array_size <= 1)
        tmp += sprintf(tmp, "%s %s(%s)"    , obj_type, node_name,  params);
    else
        tmp += sprintf(tmp, "%s[%d] %s(%s)", obj_type, array_size, node_name, params);
    EString_delete(params);
    return EString_new(mbuf);
}

const char* ShaderNode_get_function(ShaderNode _sn)
{
    return _sn.self->function;
}

void _ShaderNode_set_name(ShaderNode _sn, const char* _name, const char* _file, uint _line)
{
    if (_sn.self->node_name)
    {
        EString_delete(_sn.self->node_name);
    }
    _sn.self->node_name = _EString_new(_name, _file, _line);
}

const char* ShaderNode_get_name(ShaderNode _sn)
{
    return _sn.self->node_name;
}

ShaderNode _ShaderNode_clone(ShaderNode _sn, const char* _file, uint _line)
{
    ShaderNode ret = ShaderNode_new();

    if (_sn.self->node_name)
    {
        ShaderNode_set_name(ret, _sn.self->node_name);
    }
    if (_sn.self->function)
    {
        ret.self->function = EString_new(_sn.self->function);
    }
    for (uint32 i = 0; i < array_n(_sn.self->input_param_table); i++)
    {
        ShaderObject so = array_safe_get(_sn.self->input_param_table, i);
        ret.self->input_param_table = array_push(ret.self->input_param_table, ShaderObject_clone(so));
    }
    for (uint32 i = 0; i < array_n(_sn.self->output_param_table); i++)
    {
        ShaderObject so = array_safe_get(_sn.self->output_param_table, i);
        ret.self->output_param_table = array_push(ret.self->output_param_table, ShaderObject_clone(so));
    }
    for (uint32 i = 0; i < array_n(_sn.self->input_links); i++)
    {
        ShaderObject so = array_safe_get(_sn.self->input_links, i);
        ret.self->input_links = array_push(ret.self->input_links, ShaderObject_clone(so));
    }
    for (uint32 i = 0; i < array_n(_sn.self->output_links); i++)
    {
        ShaderObject so = array_safe_get(_sn.self->output_links, i);
        ret.self->output_links = array_push(ret.self->output_links, ShaderObject_clone(so));
    }
    if ( to_ptr(_sn.self->result_link) )
    {
        ret.self->result_link = ShaderObject_clone(_sn.self->result_link);
    }
    ShaderObject_delete(ret.self->result);
    ret.self->result = ShaderObject_clone(_sn.self->result);
    return ret;
}
