/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：pass_declaration.h
* 文件描述：pass声明，用来生成标准pass的标识
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef PASS_DECLARATION_H
#define PASS_DECLARATION_H
#include "common.h"
#include "etypes.h"

#define COLOR_MAP_MASK    0x00000001
#define NORMAL_MAP_MASK   0x00000002
#define COLOR_SKETCH_MASK 0x00000004
struct pass_decl
{
    VertexDecl dec;
    euint usage_tex_flags;
    vertex_color_function vtx_color_func;
	pass_decl()
		: dec(NULL)
		, usage_tex_flags(0)
		, vtx_color_func(Multiply)
	{}
	pass_decl(VertexDecl _dec, euint _usage_tex_flags, vertex_color_function _vtx_color_func)
		: dec(_dec)
		, usage_tex_flags(_usage_tex_flags)
		, vtx_color_func(_vtx_color_func)
	{}
};

typedef struct pass_decl* PassDecl;
API_EXPORT PassDecl PassDecl_clone(PassDecl dec);
API_EXPORT int PassDecl_compare(PassDecl p0, PassDecl p1);
#endif
