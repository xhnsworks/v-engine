#ifndef SKETCH_H
#define SKETCH_H
#include "common.h"
#include "etypes.h"
#include "texture2d.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

typedef struct _sketch_book* SketchBook;
typedef struct _sketch_cube* SketchCube;
typedef struct _renderable_plane* RenderablePlane;

API_EXPORT RenderablePlane _RenderablePlane_new(const char* _file, uint _line);
#define RenderablePlane_new() _RenderablePlane_new(__FILE__, __LINE__)
API_EXPORT void RenderablePlane_delete(RenderablePlane _self);
API_EXPORT VertexBuffer RenderablePlane_get_vertex_buffer(RenderablePlane _self);
API_EXPORT IndexBuffer RenderablePlane_get_index_buffer(RenderablePlane _self);
API_EXPORT VertexDecl RenderablePlane_get_vertex_declaration(RenderablePlane _self);

typedef struct _sketch_book_config* SketchBookConfig;

API_EXPORT SketchBookConfig SketchBookConfig_new();
API_EXPORT void SketchBookConfig_delete(SketchBookConfig _self);
API_EXPORT void SketchBookConfig_set_sketch_format(SketchBookConfig _self, uint _idx, pixel_format _fmt);
API_EXPORT void SketchBookConfig_set_plaster_format(SketchBookConfig _self, pixel_format _fmt);
API_EXPORT bool SketchBookConfig_has_plaster(SketchBookConfig _self);
API_EXPORT pixel_format SketchBookConfig_get_plaster_format(SketchBookConfig _self);
API_EXPORT void SketchBookConfig_set_size(SketchBookConfig _self, uint _width, uint _height);
API_EXPORT uint SketchBookConfig_get_num_sketchs(SketchBookConfig _self);
API_EXPORT uint SketchBookConfig_get_width(SketchBookConfig _self);
API_EXPORT uint SketchBookConfig_get_height(SketchBookConfig _self);
API_EXPORT pixel_format SketchBookConfig_get_sketch_format(SketchBookConfig _self, uint _idx);

///API_EXPORT SketchBook SketchBook_new(uint32 _width, uint32 _height);
API_EXPORT SketchBook SketchBook_new(SketchBookConfig _cfg);
API_EXPORT void SketchBook_delete(SketchBook _self);
API_EXPORT void SketchBook_draw_begin(SketchBook _self, uint _use_skhs);
API_EXPORT void SketchBook_draw_end(SketchBook _self);

API_EXPORT void SketchBook_bind_framebuffer(SketchBook _self);
API_EXPORT void SketchBook_unbind_framebuffer();
API_EXPORT void SketchBook_bind_renderbuffer();
API_EXPORT void SketchBook_unbind_renderbuffer();

API_EXPORT Texture2D SketchBook_get_sketch(SketchBook _self, uint _i);
API_EXPORT Texture2D SketchBook_get_plaster(SketchBook _self);
#endif
