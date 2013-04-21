#ifndef VERTEX_DECLARATION_H
#define VERTEX_DECLARATION_H
#include <recycle_bin.h>
#include <list.h>
#include <stack.h>
#include <vertex_element.h>
#include <elog.h>
typedef struct _vertex_declaration
{
    Stack self;
} VertexDeclaration;

extern void VertexDeclaration_Dest(Stack _dec_stk);
extern VertexDeclaration VertexDeclaration_new();
extern void _VertexDeclaration_delete(VertexDeclaration _dec);
#define VertexDeclaration_delete(d) delete(VertexDeclaration, d);

extern void VertexDeclaration_add_element(VertexDeclaration _dec, VertexElement _ele);
extern uint32 VertexDeclaration_count(VertexDeclaration _dec);
extern VertexElement VertexDeclaration_find(VertexDeclaration _dec, uint32 _i);

extern bool VertexDeclaration_test(VertexDeclaration _dec);
extern bool VertexDeclaration_test_element_semantic(VertexDeclaration _dec, element_semantic _sem);
#endif
