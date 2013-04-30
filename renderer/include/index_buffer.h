/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：index_buffer.h
* 文件描述：几何体渲染的索引缓冲
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H
#include "list.h"
#include "stack.h"
#include "mesh.h"
#define DEFAULT_INDEX_BUFFER_SIZE 6
typedef struct _index_buffer* IndexBuffer;
typedef struct _vertex_buffer* VertexBuffer;

API_EXPORT void IndexBuffer_Dest(struct _index_buffer* _self);
API_EXPORT IndexBuffer _IndexBuffer_new(e_mesh_mode mode, const char* _file, euint _line);
API_EXPORT void _IndexBuffer_delete(IndexBuffer _self, const char* _file, euint _line);
#define IndexBuffer_new(m) _IndexBuffer_new(m, __FILE__, __LINE__)
#define IndexBuffer_delete(b) _IndexBuffer_delete(b, __FILE__, __LINE__)
API_EXPORT bool IndexBuffer_read(IndexBuffer _self, unsigned int _i, vptr _result);
API_EXPORT vptr IndexBuffer_insert(IndexBuffer _self, unsigned int _i);
API_EXPORT uint32 IndexBuffer_get_id(IndexBuffer _self);
///API_EXPORT uint32 IndexBuffer_get_buffer_size(IndexBuffer _self);
API_EXPORT uint32 IndexBuffer_attach_mesh(IndexBuffer _self, Mesh _mesh, VertexBuffer _prev_vtx_buf);
API_EXPORT void IndexBuffer_buffer_data(IndexBuffer _self);
API_EXPORT euint IndexBuffer_get_num_faces(IndexBuffer _self);
API_EXPORT euint IndexBuffer_get_face_size(IndexBuffer _self);
API_EXPORT e_mesh_mode IndexBuffer_get_mesh_mode(IndexBuffer _self);
API_EXPORT void IndexBuffer_reset(IndexBuffer _self);
#endif
