#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H
#include "list.h"
#include "stack.h"
#include "vertex_declaration.h"
#include "mesh.h"
#define DEFAULT_VERTEX_BUFFER_SIZE 128
typedef struct _vertex_buffer* VertexBuffer;

API_EXPORT void VertexBuffer_Dest(struct _vertex_buffer* _self);
API_EXPORT VertexBuffer _VertexBuffer_new(VertexDecl _dec, const char* _file, euint _line);
API_EXPORT void _VertexBuffer_delete(VertexBuffer _self, const char* _file, euint _line);
#define VertexBuffer_new(d) _VertexBuffer_new(d, __FILE__, __LINE__)
#define VertexBuffer_delete(b) _VertexBuffer_delete(b, __FILE__, __LINE__)
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

API_EXPORT euint* VertexBuffer_get_ele_offs(VertexBuffer _self);
#endif
