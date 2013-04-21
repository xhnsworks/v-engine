#ifndef VERTEX_ELEMENT_H
#define VERTEX_ELEMENT_H
#include <recycle_bin.h>
#include <list.h>
#include <stack.h>

struct _vertex_element;
typedef struct vertex_element_
{
    struct _vertex_element* self;
}VertexElement;

extern void VertexElement_Dest(VertexElement _ele);
extern VertexElement _VertexElement_new(element_type _typ, element_semantic _sem, const char* _file, uint32 _line);
extern void _VertexElement_delete(VertexElement _ele, const char* _file, uint32 _line);

extern uint32 get_element_size(element_type _type);
extern element_type VertexElement_get_type(VertexElement _ele);
extern element_semantic VertexElement_get_semantic(VertexElement _ele);
extern uint32 VertexElement_get_offset(VertexElement _ele);
extern void VertexElement_set_offset(VertexElement _ele, uint32 _offset);
extern bool VertexElement_is_null(VertexElement _ele);
extern VertexElement VertexElement_null();

#define VertexElement_new(t, s) _VertexElement_new(t, s, __FILE__, __LINE__)
#define VertexElement_delete(e) _VertexElement_delete(e, __FILE__, __LINE__)

#define var_to_vtx_ele(e, v) e = *((VertexElement*)&v)
#define vtx_ele_to_var(v, e) v = *((var*)&e)
#endif
