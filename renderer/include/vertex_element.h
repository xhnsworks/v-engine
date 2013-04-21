#ifndef VERTEX_ELEMENT_H
#define VERTEX_ELEMENT_H
#include "list.h"
#include "stack.h"
#include "common.h"
#include "etypes.h"

API_EXPORT void VertexElement_Dest(VertexElement _ele);
API_EXPORT VertexElement _VertexElement_new(element_type _typ, element_semantic _sem, const char* _file, uint32 _line);
API_EXPORT void _VertexElement_delete(VertexElement _ele, const char* _file, uint32 _line);

API_EXPORT uint32 get_element_size(element_type _type);
API_EXPORT uint32 VertexElement_get_size(VertexElement _ele);
API_EXPORT element_type VertexElement_get_type(VertexElement _ele);
API_EXPORT element_semantic VertexElement_get_semantic(VertexElement _ele);
///API_EXPORT uint32 VertexElement_get_offset(VertexElement _ele);
///API_EXPORT void VertexElement_set_offset(VertexElement _ele, uint32 _offset);
API_EXPORT bool VertexElement_is_null(VertexElement _ele);
API_EXPORT VertexElement VertexElement_null();
/// privite
API_EXPORT VertexElement VertexElement_clone(VertexElement _ele);
API_EXPORT int VertexElement_compare(VertexElement e0, VertexElement e1);

#define VertexElement_new(t, s) _VertexElement_new(t, s, __FILE__, __LINE__)
#define VertexElement_delete(e) _VertexElement_delete(e, __FILE__, __LINE__)

#define var_to_vtx_ele(e, v) e = *((VertexElement*)&v)
#define vtx_ele_to_var(v, e) v = *((var*)&e)
#endif
