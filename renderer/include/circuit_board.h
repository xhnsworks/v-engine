/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：circuit_board.h
* 文件描述：线路板，用以生成着色器的条件分支
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef CIRCUIT_BOARD_H
#define CIRCUIT_BOARD_H
#include "common.h"
#include "etypes.h"
#include "array.h"
#include "shader_node.h"
typedef struct _circuit_board
{
    ShaderNode* shader_node_reference_table;
} s_circuit_board;
typedef struct _circuit_board* CircuitBoard;

API_EXPORT void CircuitBoard_Init(CircuitBoard _self);
API_EXPORT void CircuitBoard_Dest(CircuitBoard _self);
API_EXPORT CircuitBoard CircuitBoard_new();
API_EXPORT void CircuitBoard_delete(CircuitBoard _self);
API_EXPORT ShaderNode _CircuitBoard_add_reference_node(CircuitBoard _self, ShaderNode _prototype_node, const char* _file, euint _line);
#define CircuitBoard_add_reference_node(s, n) _CircuitBoard_add_reference_node(s, n, __FILE__, __LINE__)
API_EXPORT char* CircuitBoard_compile(CircuitBoard _self);
API_EXPORT CircuitBoard CircuitBoard_clone(CircuitBoard _self, const char* _file, euint _line);
#endif
