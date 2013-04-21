/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：display_pass.h
* 文件描述：显示pass
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef DISPLAY_PASS_H
#define DISPLAY_PASS_H
#include "common.h"
#include "etypes.h"
#include "shader.h"
#include "pass.h"
#include "shader_node.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "lighting.h"
///#include "material_instance.h"
#include "material_prototype.h"

API_EXPORT Pass create_display_pass_ex2(VertexDecl _dec, material_decl* _mat_decls);
API_EXPORT Pass create_display_texture_pass(VertexDecl _dec);

///void _create_dispaly_pass_ex2_test(VertexDecl _dec);
#endif
