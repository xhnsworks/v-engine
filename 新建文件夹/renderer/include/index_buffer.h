#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H
#include <recycle_bin.h>
#include <list.h>
#include <stack.h>
#include <mesh.h>
struct _index_buffer;
typedef struct index_buffer_
{
    struct _index_buffer* self;
} IndexBuffer;

extern void IndexBuffer_Dest(struct _index_buffer* _buf);
extern IndexBuffer _IndexBuffer_new(const char* _file, uint _line);
extern void _IndexBuffer_delete(IndexBuffer _buf, const char* _file, uint _line);
#define IndexBuffer_new() _IndexBuffer_new(__FILE__, __LINE__)
#define IndexBuffer_delete(b) _IndexBuffer_delete(b, __FILE__, __LINE__)
extern uint32* IndexBuffer_get_buffer(IndexBuffer _buf);
extern uint32 IndexBuffer_get_id(IndexBuffer _buf);
extern uint32 IndexBuffer_get_buffer_size(IndexBuffer _buf);
extern uint32 IndexBuffer_attach_mesh(IndexBuffer _buf, Mesh _mesh);
extern void IndexBuffer_buffer_data(IndexBuffer _buf);
#endif
