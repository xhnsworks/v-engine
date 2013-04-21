/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：cylinder.h
* 文件描述：生成圆柱里mesh的函数
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef CYLINDER_H
#define CYLINDER_H
#include "mesh.h"
API_EXPORT Mesh create_cylinder_mesh(float height, float radius, int num_segments);
#endif
