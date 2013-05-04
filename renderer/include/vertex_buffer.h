#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H
#include "list.h"
#include "stack.h"
#include "vertex_declaration.h"
#include "mesh.h"
#define DEFAULT_VERTEX_BUFFER_SIZE 128
typedef struct _vertex_buffer* VertexBuffer;

API_EXPORT void VertexBuffer_Dest(struct _vertex_buffer* _self);
API_EXPORT VertexBuffer VertexBuffer_new(VertexDecl _dec);
API_EXPORT void VertexBuffer_delete(VertexBuffer _self);
///API_EXPORT vptr VertexBuffer_access(VertexBuffer _self, element_semantic _sem, unsigned int _i);
API_EXPORT bool VertexBuffer_read(VertexBuffer _self, element_semantic _sem, unsigned int _i, vptr _result);
API_EXPORT vptr VertexBuffer_insert(VertexBuffer _self, element_semantic _sem, unsigned int _i);
API_EXPORT VertexDecl VertexBuffer_get_vertex_declaration(VertexBuffer _self);
API_EXPORT euint32 VertexBuffer_get_id(VertexBuffer _self);
API_EXPORT euint32 VertexBuffer_get_vertex_size(VertexBuffer _self);
API_EXPORT euint32 VertexBuffer_get_buffer_size(VertexBuffer _self);
API_EXPORT euint32 VertexBuffer_get_buffer_tail(VertexBuffer _self);
API_EXPORT euint32 VertexBuffer_attach_mesh(VertexBuffer _self, Mesh _mesh);
API_EXPORT void VertexBuffer_reset(VertexBuffer _self);
API_EXPORT void VertexBuffer_buffer_data(VertexBuffer _self);

API_EXPORT euint32* VertexBuffer_get_ele_offs(VertexBuffer _self);
#endif
