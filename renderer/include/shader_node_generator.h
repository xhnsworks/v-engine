#ifndef SHADER_NODE_GENERATOR_H
#define SHADER_NODE_GENERATOR_H
#include "common.h"
#include "etypes.h"
#include "shader_node.h"
#include "shader_buffer.h"

typedef enum _shader_node_type
{
    MapSampleNode           = 0,
    MapSampleNodeRGBA       = 1,
    ClearLightingValueNode  = 2,
    PositionDecodeNode      = 3,
    NormalDecodeNode        = 4,
    PointLightingNode       = 6,
    LightingAccumulatorNode = 7,
    LightingOutputNode      = 8,
    ClearSketchNode         = 9,
    ToCameraSpaceNode       = 10,
    MaxShaderNode           = 11,
} shader_node_type;

typedef ShaderNode (*CreateShaderNodeProc)();
typedef struct _shader_node_generator* SdrNdGen;

SdrNdGen SdrNdGen_new();
void SdrNdGen_delete(SdrNdGen _self);
void SdrNdGen_register_shader_node(SdrNdGen _self, uint _node_id, CreateShaderNodeProc _create_node_proc);
ShaderNode SdrNdGen_generate_shader_node(SdrNdGen _self, uint _node_id);
void SdrNdGen_destroy_all_generated_shader_node(SdrNdGen _self);

void SdrNdGen_register_default_nodes(SdrNdGen _self);
void SdrNdGen_attach_all_prototype_nodes(SdrNdGen _self, ShaderBuffer _sb);

ShaderNode SdrNdGen_add_reference_node(SdrNdGen _self, uint _node_id);
ShaderNode SdrNdGen_add_reference_node2(SdrNdGen _self, CircuitBoard _cb, uint _node_id);

struct _i_sdr_nd_gen
{
    SdrNdGen (*New)();
    void (*Delete)(SdrNdGen _self);
    void (*register_shader_node)(SdrNdGen _self, uint _node_id, CreateShaderNodeProc _create_node_proc);
    ShaderNode (*generate_shader_node)(SdrNdGen _self, uint _node_id);
    void (*destroy_all_generated_shader_node)(SdrNdGen _self);
    void (*register_default_nodes)(SdrNdGen _self);
    void (*attach_all_prototype_nodes)(SdrNdGen _self, ShaderBuffer _sb);
    ShaderNode (*add_reference_node_1)(SdrNdGen _self, uint _node_id);
    ShaderNode (*add_reference_node_2)(SdrNdGen _self, CircuitBoard _cb, uint _node_id);
};

static struct _i_sdr_nd_gen ISdrNdGen = {
    SdrNdGen_new,
    SdrNdGen_delete,
    SdrNdGen_register_shader_node,
    SdrNdGen_generate_shader_node,
    SdrNdGen_destroy_all_generated_shader_node,
    SdrNdGen_register_default_nodes,
    SdrNdGen_attach_all_prototype_nodes,
    SdrNdGen_add_reference_node,
    SdrNdGen_add_reference_node2,
};
#endif
