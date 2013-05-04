/*
* Copyright (c) 2013, �캣��
* All rights reserved.
*
* �ļ����ƣ�circuit_board.h
* �ļ���������·�壬����������ɫ����������֧
*
* ��ǰ�汾��0.1
* ��    �ߣ��캣��
* ������ڣ�2013��1��27��
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
API_EXPORT ShaderNode CircuitBoard_add_reference_node(CircuitBoard _self, ShaderNode _prototype_node);
API_EXPORT char* CircuitBoard_compile(CircuitBoard _self);
API_EXPORT CircuitBoard CircuitBoard_clone(CircuitBoard _self);
#endif
