#include "pch.h"
#include "shader_node.h"
#include "emem.h"
#include "array.h"
#include "estring.h"
#include "elog.h"
#include "shader_buffer.h"
#include "shader_node_base.h"
#include "eassert.h"
#include "glsl_lex.h"
typedef struct _shader_node
{
    shader_node_base base;
    char* node_name;
    ///ShaderObject result;
    ShaderObject* input_param_table;
    ShaderObject* output_param_table;
    ///ShaderObject result_link;
    ShaderObject* input_links;
    ShaderObject* output_links;
    char* function;
} shader_node;

EString _recompile(ShaderNode _sn)
{
	GLSL::ShaderTranslator sdrTran(_sn->function);
	euint num_input_params = array_n(_sn->input_param_table);
	euint num_input_links = array_n(_sn->input_links);
	euint num_output_params = array_n(_sn->output_param_table);
	euint num_output_links = array_n(_sn->output_links);
	if (num_input_params != num_input_links)
		return NULL;
	if (num_output_params != num_output_links)
		return NULL;
	for (euint i = 0; i < num_input_params; i++) {
		const char* param = ShaderObject_get_name(_sn->input_param_table[i]);
		const char* link = ShaderObject_get_name(_sn->input_links[i]);
		sdrTran.m_translationTable.insert(xhn::make_pair(xhn::string(param), xhn::string(link)));
	}
	for (euint i = 0; i < num_output_params; i++) {
		const char* param = ShaderObject_get_name(_sn->output_param_table[i]);
		const char* link = ShaderObject_get_name(_sn->output_links[i]);
		sdrTran.m_translationTable.insert(xhn::make_pair(xhn::string(param), xhn::string(link)));
	}
	while (GLSL::yylex(NULL, &sdrTran) > 0) {
	}
	xhn::string output = "//##";
	output += ShaderNode_get_name(_sn);
	output += "\n";
	output += sdrTran.m_output;
	return EString_new(output.c_str());
}

const char* _compile_params(ShaderNode _sn)
{
    char mbuf[STRING_BUFFER_SIZE];
    ///sprintf(mbuf, "");
	int remainder = STRING_BUFFER_SIZE - 1;
	int offs = 0;
    mbuf[0] = 0x00;
    char* tmp = mbuf;

    if (array_n(_sn->input_param_table))
    {
        for (euint32 i = 0; i < array_n(_sn->input_param_table); i++)
        {
            ShaderObject so = array_safe_get(_sn->input_param_table, i);

            euint array_size, array_index;

            const char* obj_type = get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            const char* obj_name = ShaderObject_get_name(so);

            if (array_size <= 1)
            {
                if (i < array_n(_sn->input_param_table) - 1) {
                    
					offs = snprintf(tmp, remainder, "%s %s, ", obj_type, obj_name);
					tmp += offs;
					remainder -= offs;
				}
                else {
                    offs = snprintf(tmp, remainder, "%s %s"  , obj_type, obj_name);
					tmp += offs;
					remainder -= offs;
				}
            }
            else
            {
                if (i < array_n(_sn->input_param_table) - 1) {
                    offs = snprintf(tmp, remainder, "%s[%d] %s, ", obj_type, array_size, obj_name);
					tmp += offs;
					remainder -= offs;
				}
                else {
                    offs = snprintf(tmp, remainder, "%s[%d] %s"  , obj_type, array_size, obj_name);
					tmp += offs;
					remainder -= offs;
				}
            }
        }
    }

    if (array_n(_sn->output_param_table))
    {
        if (array_n(_sn->input_param_table))
        {
            offs = snprintf(tmp, remainder, ", ");
			tmp += offs;
			remainder -= offs;
        }

        for (euint32 i = 0; i < array_n(_sn->output_param_table); i++)
        {
            ShaderObject so = array_safe_get(_sn->output_param_table, i);

            euint array_size, array_index;

            const char* obj_type = get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            const char* obj_name = ShaderObject_get_name(so);

            if (array_size <= 1)
            {
                if (i < array_n(_sn->output_param_table) - 1) {
					offs = snprintf(tmp, remainder, "out %s %s, ", obj_type, obj_name);
					tmp += offs;
					remainder -= offs;
				}
                else {
                    offs = snprintf(tmp, remainder, "out %s %s"  , obj_type, obj_name);
				    tmp += offs;
					remainder -= offs;
				}
            }
            else
            {
                if (i < array_n(_sn->input_param_table) - 1) {
                    offs = snprintf(tmp, remainder, "out %s[%d] %s, ", obj_type, array_size, obj_name);
					tmp += offs;
					remainder -= offs;
				}
                else {
                    offs = snprintf(tmp, remainder, "out %s[%d] %s"  , obj_type, array_size, obj_name);
					tmp += offs;
					remainder -= offs;
				}
            }
        }
    }
    return EString_new(mbuf);
}

const char* _compile_links(ShaderNode _sn)
{
    char mbuf[STRING_BUFFER_SIZE];
    ///sprintf(mbuf, "");
	int remainder = STRING_BUFFER_SIZE - 1;
	int offs = 0;
    mbuf[0] = 0x00;
    char* tmp = mbuf;

    if (array_n(_sn->input_links))
    {
        for (euint32 i = 0; i < array_n(_sn->input_links); i++)
        {
            ShaderObject so = array_safe_get(_sn->input_links, i);

            euint array_size, array_index;

            ///const char* obj_type = get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            const char* obj_name = ShaderObject_get_name(so);

            if (array_size <= 1 || array_index == INVALID_ARRAY_INDEX)
            {
                if (i < array_n(_sn->input_links) - 1) {
                    offs = snprintf(tmp, remainder, "%s, ", obj_name);
					tmp += offs;
					remainder -= offs;
				}
                else {
                    offs = snprintf(tmp, remainder, "%s"  , obj_name);
					tmp += offs;
					remainder -= offs;
				}
            }
            else
            {
                if (i < array_n(_sn->input_links) - 1) {
                    offs = snprintf(tmp, remainder, "%s[%d], ", obj_name, array_index);
					tmp += offs;
					remainder -= offs;
				}
                else {
                    offs = snprintf(tmp, remainder, "%s[%d]"  , obj_name, array_index);
					tmp += offs;
					remainder -= offs;
				}
            }
        }
    }

    if (array_n(_sn->output_links))
    {
        if (array_n(_sn->input_links))
        {
            offs = snprintf(tmp, remainder, ", ");
			tmp += offs;
			remainder -= offs;
        }
        for (euint32 i = 0; i < array_n(_sn->output_links); i++)
        {
            ShaderObject so = array_safe_get(_sn->output_links, i);

            euint array_size, array_index;

            ///const char* obj_type = get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            get_shader_object_string( ShaderObject_get_type(so, &array_size, &array_index) );
            const char* obj_name = ShaderObject_get_name(so);

            if (array_size <= 1 || array_index == INVALID_ARRAY_INDEX)
            {
                if (i < array_n(_sn->output_links) - 1) {
                    offs = snprintf(tmp, remainder, "%s, ", obj_name);
					tmp += offs;
					remainder -= offs;
				}
                else {
                    offs = snprintf(tmp, remainder, "%s"  , obj_name);
				    tmp += offs;
					remainder -= offs;
				}
            }
            else
            {
                if (i < array_n(_sn->output_links) - 1) {
                    offs = snprintf(tmp, remainder, "%s[%d], ", obj_name, array_index);
					tmp += offs;
					remainder -= offs;
				}
                else {
                    offs = snprintf(tmp, remainder, "%s[%d]"  , obj_name, array_index);
					tmp += offs;
					remainder -= offs;
				}
            }
        }

    }
    return EString_new(mbuf);
}
///==============================ShaderNode========================================///
void ShaderNode_Dest(shader_node* _sn, const char* _file, euint _line)
{
    if (_sn->node_name)
        EString_delete(_sn->node_name);
    if (_sn->function)
        EString_delete(_sn->function);
/**
    ShaderObject_delete(_sn->result);
    if ( to_ptr(_sn->result_link) != NULL )
        ShaderObject_delete(_sn->result_link);
**/
    for (euint32 i = 0; i < array_n(_sn->input_param_table); i++)
    {
        ShaderObject so = array_safe_get(_sn->input_param_table, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->input_param_table);

    for (euint32 i = 0; i < array_n(_sn->output_param_table); i++)
    {
        ShaderObject so = array_safe_get(_sn->output_param_table, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->output_param_table);

    for (euint32 i = 0; i < array_n(_sn->input_links); i++)
    {
        ShaderObject so = array_safe_get(_sn->input_links, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->input_links);

    for (euint32 i = 0; i < array_n(_sn->output_links); i++)
    {
        ShaderObject so = array_safe_get(_sn->output_links, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->output_links);
}

shader_node* ShaderNode_Init(struct _shader_node* _sn, const char* _file, euint _line)
{
    _sn->base.init_proc = (shader_node_init_proc)ShaderNode_Init;
    _sn->base.dest_proc = (shader_node_dest_proc)ShaderNode_Dest;
    _sn->base.clone_proc = (shader_node_clone_proc)_ShaderNode_clone;
    _sn->base.compile_proc = NULL;
    _sn->node_name = NULL;
    ShaderObject exec_so = {NULL};
    ///_sn->result = _ShaderObject_new(Void_Obj, "", 1, _file, _line);
    _sn->input_param_table = array_new(ShaderObject, 5, exec_so);
    _sn->output_param_table = array_new(ShaderObject, 5, exec_so);
    ///_sn->result_link = exec_so;
    _sn->input_links = array_new(ShaderObject, 5, exec_so);
    _sn->output_links = array_new(ShaderObject, 5, exec_so);
    _sn->function = NULL;
    return _sn;
}

ShaderNode _ShaderNode_new(const char* _file, euint _line)
{
    ShaderNode ret;
    ret = (ShaderNode)_Malloc(sizeof(shader_node), _file, _line);
    ret = ShaderNode_Init(ret, _file, _line);
    return ret;
}

void _ShaderNode_delete(ShaderNode _sn, const char* _file, euint _line)
{
    ShaderNodeBase_delete((ShaderNodeBase)_sn, _file, _line);
}
/**
void ShaderNode_set_return_type(ShaderNode _sn, shader_object_type _type, euint32 _array_size)
{
    ShaderObject_set_type(_sn->result, _type, _array_size, INVALID_ARRAY_INDEX);
}
**/
void _ShaderNode_add_input_param(ShaderNode _sn, shader_object_type _type, const char* _pam_name, euint32 _array_size,
                                 const char* _file, euint _line)
{
    ShaderObject so = _ShaderObject_new(_type, _pam_name, _array_size, _file, _line);
    _sn->input_param_table = array_push(_sn->input_param_table, so);
}

void ShaderNode_add_output_param(ShaderNode _sn, shader_object_type _type, const char* _pam_name, euint32 _array_size)
{
    ShaderObject so = ShaderObject_new(_type, _pam_name, _array_size);
    _sn->output_param_table = array_push(_sn->output_param_table, so);
}

bool _ShaderNode_test_input_link(const char* _file, euint _line, ShaderNode _sn, ShaderObject _so)
{
    euint n   = array_n(_sn->input_links);
    euint max = array_n(_sn->input_param_table);
    if (n >= max)
        return false;

    ShaderObject obj = _sn->input_param_table[n];
    if (!ShaderObject_equal(obj, _so))
    {
        elog("%s %d invalid input link", _file, _line);
        return false;
    }
    else
    {
        return true;
    }
}

void _ShaderNode_add_input_link(const char* _file, euint _line, ShaderNode _sn, ShaderObject _so, euint32 _array_index)
{
    ShaderObject so = ShaderObject_clone(_so);
    ShaderObject_set_index(so, _array_index);
    EAssert (_ShaderNode_test_input_link(_file, _line, _sn, so), "%s", "test input link fail");
    _sn->input_links = array_push(_sn->input_links, so);
}

void ShaderNode_add_output_link(ShaderNode _sn, ShaderObject _so, euint32 _array_index)
{
    ShaderObject so = ShaderObject_clone(_so);
    ShaderObject_set_index(so, _array_index);
    _sn->output_links = array_push(_sn->output_links, so);
}
/**
void ShaderNode_set_result_link(ShaderNode _sn, ShaderObject _so, euint32 _array_index)
{
    if ( to_ptr(_sn->result_link) != NULL )
    {
        ShaderObject_delete(_sn->result_link);
    }
    ShaderObject so = ShaderObject_clone(_so);
    ShaderObject_set_index(so, _array_index);
    _sn->result_link = so;
}
**/
void ShaderNode_set_function(ShaderNode _sn, const char* _func)
{
    _sn->function = (char*)EString_new(_func);
}

void ShaderNode_clear_links(ShaderNode _sn)
{
    ShaderObject exec_so = {NULL};
	/**
    if ( to_ptr(_sn->result_link) != NULL )
    {
        ShaderObject_delete(_sn->result_link);
        _sn->result_link = exec_so;
    }
**/
    for (euint32 i = 0; i < array_n(_sn->input_links); i++)
    {
        ShaderObject so = array_safe_get(_sn->input_links, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->input_links);
    _sn->input_links = array_new(ShaderObject, 5, exec_so);

    for (euint32 i = 0; i < array_n(_sn->output_links); i++)
    {
        ShaderObject so = array_safe_get(_sn->output_links, i);
        if ( to_ptr(so) != NULL )
            ShaderObject_delete(so);
    }
    array_delete(_sn->output_links);
    _sn->output_links = array_new(ShaderObject, 5, exec_so);
}

const char* ShaderNode_compile(ShaderNode _sn)
{
#ifndef USE_RECOMPILE
	string_buffer_new(STRING_BUFFER_SIZE);
	EString tmp = _compile_links(_sn);
	sbuf_printf("%s( %s );", _sn->node_name, tmp);
	EString_delete(tmp);
	return EString_new(get_string_buffer);
#else
	return _recompile(_sn);
#endif
}

const char* ShaderNode_compile_function_declaration(ShaderNode _sn)
{
    char mbuf[STRING_BUFFER_SIZE];
    ///sprintf(mbuf, "");
    mbuf[0] = 0x00;
    char* tmp = mbuf;
    ///euint32 array_size, array_index;
    ///const char* obj_type = get_shader_object_string( ShaderObject_get_type(_sn->result, &array_size, &array_index) );
    const char* node_name = _sn->node_name;
    const char* params = _compile_params(_sn);
	tmp += snprintf(tmp, STRING_BUFFER_SIZE - 1, "void %s(%s)", node_name,  params);
    EString_delete(params);
    return EString_new(mbuf);
}

const char* ShaderNode_get_function(ShaderNode _sn)
{
    return _sn->function;
}

void _ShaderNode_set_name(ShaderNode _sn, const char* _name, const char* _file, euint _line)
{
    if (_sn->node_name)
    {
        EString_delete(_sn->node_name);
    }
    _sn->node_name = (char*)_EString_new(_name, _file, _line);
}

const char* ShaderNode_get_name(ShaderNode _sn)
{
    return _sn->node_name;
}

ShaderNode _ShaderNode_clone(ShaderNode _sn, const char* _file, euint _line)
{
    ShaderNode ret = _ShaderNode_new(_file, _line);

    if (_sn->node_name)
    {
        _ShaderNode_set_name(ret, _sn->node_name, _file, _line);
    }
    if (_sn->function)
    {
        ret->function = (char*)EString_new(_sn->function);
    }
    for (euint32 i = 0; i < array_n(_sn->input_param_table); i++)
    {
        ///ShaderObject so = array_safe_get(_sn->input_param_table, i);
        ShaderObject so  = _sn->input_param_table[i];
        ShaderObject cso = ShaderObject_clone(so);
        EAssert (to_ptr(so) && to_ptr(cso), "%s", "clone null shader object");
        ret->input_param_table = array_push(ret->input_param_table, cso);
    }
    for (euint32 i = 0; i < array_n(_sn->output_param_table); i++)
    {
        ///ShaderObject so = array_safe_get(_sn->output_param_table, i);
        ShaderObject so  = _sn->output_param_table[i];
        ShaderObject cso = ShaderObject_clone(so);
        EAssert (to_ptr(so) && to_ptr(cso), "%s", "clone null shader object");
        ret->output_param_table = array_push(ret->output_param_table, cso);
    }
    for (euint32 i = 0; i < array_n(_sn->input_links); i++)
    {
        ShaderObject so = array_safe_get(_sn->input_links, i);
        ret->input_links = array_push(ret->input_links, ShaderObject_clone(so));
    }
    for (euint32 i = 0; i < array_n(_sn->output_links); i++)
    {
        ShaderObject so = array_safe_get(_sn->output_links, i);
        ret->output_links = array_push(ret->output_links, ShaderObject_clone(so));
    }
	/**
    if ( to_ptr(_sn->result_link) )
    {
        ret->result_link = ShaderObject_clone(_sn->result_link);
    }
	**/
    ///ShaderObject_delete(ret->result);
    ///ret->result = ShaderObject_clone(_sn->result);
    ret->base.compile_proc = (shader_node_compile_proc)ShaderNode_compile;
    return ret;
}
