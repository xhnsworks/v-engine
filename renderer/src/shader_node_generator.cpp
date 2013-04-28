#include "pch.h"
#include "shader_node_generator.h"
#include "tree.h"
#include "emem.h"
#include "array.h"
#include "pass_console.h"
#include "lighting_pass.h"
typedef struct _shader_node_generator
{
    Tree shader_node_factory_tree;
    Tree prototype_shader_node_tree;
    ShaderNode* recycle_bin;
    ShaderBuffer owner;
} shader_node_generator;

SdrNdGen SdrNdGen_new()
{
    shader_node_generator* ret = (shader_node_generator*)SMalloc(sizeof(shader_node_generator));
    ret->shader_node_factory_tree = Tree_new(Uint32, Vptr, (MALLOC)Ealloc, (MFREE)Efree);
    ret->prototype_shader_node_tree = Tree_new(Uint32, Vptr, (MALLOC)Ealloc, (MFREE)Efree);
    ShaderNode null_node = {NULL};
    ret->recycle_bin = array_new(ShaderNode, 20, null_node);
    ShaderBuffer null_buf = {NULL};
    ret->owner = null_buf;
    return ret;
}
void SdrNdGen_delete(SdrNdGen _self)
{
    Tree_Dest(_self->shader_node_factory_tree);
    Tree_Dest(_self->prototype_shader_node_tree);
    for (uint i = 0; i < array_n(_self->recycle_bin); i++)
    {
        ShaderNode_delete(_self->recycle_bin[i]);
    }
    array_delete(_self->recycle_bin);
    Mfree(_self);
}
void SdrNdGen_register_shader_node(SdrNdGen _self, uint _node_id, CreateShaderNodeProc _create_node_proc)
{
    var key, data;
    key.uint32_var = _node_id;
    data.vptr_var = (vptr)_create_node_proc;
    Tree_insert(_self->shader_node_factory_tree, key, data);
}
ShaderNode SdrNdGen_generate_shader_node(SdrNdGen _self, uint _node_id)
{
    ShaderNode ret = {NULL};
    var key, data;
    key.uint32_var = _node_id;
    if (Tree_find(_self->shader_node_factory_tree, key, &data))
    {
        CreateShaderNodeProc proc = (CreateShaderNodeProc)data.vptr_var;
        ret = proc();
        apush(_self->recycle_bin, ret);
        return ret;
    }
    else
    {
        return ret;
    }
}
void SdrNdGen_destroy_all_generated_shader_node(SdrNdGen _self)
{
    for (uint i = 0; i < array_n(_self->recycle_bin); i++)
    {
        ShaderNode_delete(_self->recycle_bin[i]);
    }
    array_n(_self->recycle_bin) = 0;
}

ShaderNode create_clear_sketch_node(VertexDecl _dec)
{
    if (!VertexDecl_test(_dec))
    {
        ShaderNode ret = {NULL};
        return ret;
    }
    ShaderNode ret = ShaderNode_new();
    ShaderNode_set_name(ret, "ClearSketch");

    ShaderNode_set_function(ret,
                            "{\n"
                            "    gl_FragData[0] = vec4( 0.0, 0.0, 0.0, 0.0 );\n"
                            "    gl_FragData[1] = vec4( 0.0, 0.0, 0.0, 0.0 );\n"
                            "    gl_FragData[2] = vec4( 0.0, 0.0, 0.0, 0.0 );\n"
                            "}\n");

    return ret;
}

void SdrNdGen_register_default_nodes(SdrNdGen _self)
{
    SdrNdGen_register_shader_node(_self, MapSampleNode, create_map_sample_node);
    SdrNdGen_register_shader_node(_self, MapSampleNodeRGBA, create_map_sample_node_rgba);
    SdrNdGen_register_shader_node(_self, ClearLightingValueNode, create_clear_lighting_value_node);
    SdrNdGen_register_shader_node(_self, PositionDecodeNode, create_position_decode_node);
    SdrNdGen_register_shader_node(_self, NormalDecodeNode, create_normal_decode_node);
    SdrNdGen_register_shader_node(_self, PointLightingNode, create_point_array_lighting_node);
    SdrNdGen_register_shader_node(_self, LightingAccumulatorNode, create_lighting_accumulator_node);
    SdrNdGen_register_shader_node(_self, LightingOutputNode, create_lighting_output_node);
    SdrNdGen_register_shader_node(_self, ToCameraSpaceNode, create_to_camera_space_node);

    ///SdrNdGen_register_shader_node(_self, ClearSketchNode, create_clear_sketch_node);
}

void SdrNdGen_attach_all_prototype_nodes(SdrNdGen _self, ShaderBuffer _sb)
{
    if (to_ptr(_self->owner))
        return;
    Iterator iter = Tree_begin(_self->shader_node_factory_tree);
    while (iter)
    {
        var key = Tree_get_key(iter);
        ShaderNode node = SdrNdGen_generate_shader_node(_self, key.uint32_var);
        ShaderBuffer_add_prototype_node(_sb, node);
        var data;
        data.vptr_var = node;
        Tree_insert(_self->prototype_shader_node_tree, key, data);
        iter = Tree_next(iter);
    }
    _self->owner = _sb;
}

ShaderNode SdrNdGen_add_reference_node(SdrNdGen _self, uint _node_id)
{
    if (!to_ptr(_self->owner))
        return NULL;
    var key, data;
    key.uint32_var = _node_id;
    if (Tree_find(_self->prototype_shader_node_tree, key, &data))
    {
        ShaderNode node = (ShaderNode)data.vptr_var;
        return ShaderBuffer_add_reference_node(_self->owner, ShaderNode_get_name(node));
    }
    else
        return NULL;
}

ShaderNode SdrNdGen_add_reference_node2(SdrNdGen _self, CircuitBoard _cb, uint _node_id)
{
    if (!to_ptr(_self->owner))
        return NULL;
    var key, data;
    key.uint32_var = _node_id;
    if (Tree_find(_self->prototype_shader_node_tree, key, &data))
    {
        ShaderNode node = (ShaderNode)data.vptr_var;
        return CircuitBoard_add_reference_node(_cb, node);
    }
    else
        return NULL;
}
