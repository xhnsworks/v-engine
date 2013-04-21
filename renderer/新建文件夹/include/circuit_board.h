#ifndef CIRCUIT_BOARD_H
#define CIRCUIT_BOARD_H
#include "common.h"
#include "etypes.h"
#include "array.h"
#include "shader_node.h"
typedef struct _circuit_board
{
    ShaderNode* shader_node_reference_table;
} circuit_board;
typedef struct _circuit_board* CircuitBoard;

API_EXPORT void CircuitBoard_Init(CircuitBoard _self);
API_EXPORT void CircuitBoard_Dest(CircuitBoard _self);
API_EXPORT CircuitBoard CircuitBoard_new();
API_EXPORT void CircuitBoard_delete(CircuitBoard _self);
API_EXPORT ShaderNode _CircuitBoard_add_reference_node(CircuitBoard _self, ShaderNode _prototype_node, const char* _file, uint _line);
#define CircuitBoard_add_reference_node(s, n) _CircuitBoard_add_reference_node(s, n, __FILE__, __LINE__)
API_EXPORT char* CircuitBoard_compile(CircuitBoard _self);
API_EXPORT CircuitBoard CircuitBoard_clone(CircuitBoard _self, const char* _file, uint _line);
#endif
