/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：plane_mesh.h
* 文件描述：生成片状mesh的函数
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef PLANE_MESH_H
#define PLANE_MESH_H
#include "mesh.h"
#include "color.h"
template <typename POINT, typename MAKE_POINT>
void make_plane_points(float x, float y, float z, float width, float height, POINT* result)
{
    MAKE_POINT makePoint;
	result[0] = makePoint(x        , y         , z);
	result[1] = makePoint(x + width, y         , z);
	result[2] = makePoint(x + width, y + height, z);
	result[3] = makePoint(x        , y + height, z);
}
API_EXPORT Mesh create_plane_mesh();
API_EXPORT Mesh create_plane_mesh(float x, float y, float z, float width, float height);
API_EXPORT Mesh create_plane_mesh(float x, float y, float z, float width, float height, float u0, float u1, float v0, float v1, 
								  const EColor& color_u0v0, const EColor& color_u1v0, const EColor& color_u1v1, const EColor& color_u0v1);
#endif // PLANE_H
