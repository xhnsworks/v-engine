#ifndef VERTEX_DECLARATION_H
#define VERTEX_DECLARATION_H
#include "list.h"
#include "stack.h"
#include "vertex_element.h"
#include "elog.h"
#include "array.h"

API_EXPORT void VertexDecl_Dest(VertexElement* _eles);
///API_EXPORT VertexDecl VertexDecl_new();
API_EXPORT VertexDecl VertexDecl_new_from_string(const char* str);
API_EXPORT void _VertexDecl_delete(VertexDecl _dec);
#define VertexDecl_delete(d) _VertexDecl_delete(d);

API_EXPORT void VertexDecl_add_element(VertexDecl _dec, VertexElement _ele);
API_EXPORT uint32 VertexDecl_count(VertexDecl _dec);
API_EXPORT VertexElement VertexDecl_find(VertexDecl _dec, uint32 _i);

API_EXPORT const char* VertexDecl_get_decl_string(VertexDecl _dec);

API_EXPORT uint VertexDecl_get_vertex_size(VertexDecl _dec);

API_EXPORT bool VertexDecl_test(VertexDecl _dec);
API_EXPORT bool VertexDecl_test_element_semantic(VertexDecl _dec, element_semantic _sem);

API_EXPORT VertexDecl VertexDecl_clone(VertexDecl _dec);

API_EXPORT int VertexDecl_compare(VertexDecl d0, VertexDecl d1);

#endif
