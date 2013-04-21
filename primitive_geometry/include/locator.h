/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：locator.h
* 文件描述：生成定位器的函数
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef LOCATOR_H
#define LOCATOR_H
#include "mesh.h"
#include "float_base.h"
API_EXPORT Mesh create_locator(EFloat3* pos, float radius);
API_EXPORT Mesh create_locator2(EFloat3* pos, float radius);
#endif
