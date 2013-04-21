#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H
#include <recycle_bin.h>
#include <list.h>
#include <stack.h>
#include <vertex_declaration.h>
#include <mesh.h>
#define MAX_VERTEX_BUFFER_SIZE 128 * 1024
struct _vertex_buffer;
typedef struct vertex_buffer_
{
    struct _vertex_buffer* self;
} VertexBuffer;

extern void VertexBuffer_Dest(struct _vertex_buffer* _buf);
extern VertexBuffer _VertexBuffer_new(VertexDeclaration _dec, const char* _file, uint _line);
extern void _VertexBuffer_delete(VertexBuffer _buf, const char* _file, uint _line);
#define VertexBuffer_new(d) _VertexBuffer_new(d, __FILE__, __LINE__)
#define VertexBuffer_delete(b) _VertexBuffer_delete(b, __FILE__, __LINE__)
extern vptr VertexBuffer_access(VertexBuffer _buf, element_semantic _sem, unsigned int _i);
extern VertexDeclaration VertexBuffer_get_vertex_declaration(VertexBuffer _buf);
extern uint32 VertexBuffer_get_id(VertexBuffer _buf);
extern uint32 VertexBuffer_get_vertex_size(VertexBuffer _buf);
extern uint32 VertexBuffer_get_buffer_size(VertexBuffer _buf);
extern vptr VertexBuffer_get_buffer(VertexBuffer _buf);
extern uint32 VertexBuffer_attach_mesh(VertexBuffer _buf, Mesh _mesh);
extern void VertexBuffer_reset(VertexBuffer _buf);
extern void VertexBuffer_buffer_data(VertexBuffer _buf);
#endif
