/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：shadow_emitter.h
* 文件描述：阴影发射器，用以产生阴影测试节点
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef SHADOW_EMITTER_H
#define SHADOW_EMITTER_H
#include "common.h"
#include "etypes.h"
#include "std_pass.h"
#include "shader_buffer.h"
API_EXPORT ShaderNode create_shadow2d_test_node();
API_EXPORT ShaderNode create_shadowcube_test_node();
API_EXPORT ShaderNode create_white_screen_node();
API_EXPORT Pass create_shadow_pass(VertexDecl _dec, int num_shadow_emitters, int idx_emitter);
#endif
