#ifndef SKETCH_H
#define SKETCH_H
#include <common.h>
#include <etypes.h>
#include <texture2d.h>
#include <vertex_buffer.h>
#include <index_buffer.h>
struct _sketch_book;
typedef struct sketch_book_
{
    struct _sketch_book* self;
} SketchBook;
struct _renderable_plane;
typedef struct renderable_plane_
{
    struct _renderable_plane* self;
} RenderablePlane;

extern RenderablePlane RenderablePlane_new();
extern void RenderablePlane_delete(RenderablePlane _self);
extern VertexBuffer RenderablePlane_get_vertex_buffer(RenderablePlane _self);
extern IndexBuffer RenderablePlane_get_index_buffer(RenderablePlane _self);
extern VertexDeclaration RenderablePlane_get_vertex_declaration(RenderablePlane _self);

extern SketchBook SketchBook_new(uint32 _width, uint32 _height);
extern void SketchBook_delete(SketchBook _self);
extern void SketchBook_draw_begin(SketchBook _self, uint32 _use_skhs);
extern void SketchBook_draw_end(SketchBook _self);
extern Texture2D SketchBook_get_sketch(SketchBook _self, uint32 _i);
#endif
