#ifndef SKETCH_H
#define SKETCH_H
#include "common.h"
#include "etypes.h"
#include "texture2d.h"
#include "texture_cube.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

typedef struct sketch_book* SketchBook;
typedef struct _sketch_cube* SketchCube;
typedef struct _renderable_plane* RenderablePlane;
typedef enum _cube_face_index
{
    PosX = 0,
    NegX,
    PosY,
    NegY,
    PosZ,
    NegZ,
} cube_face_index;

API_EXPORT RenderablePlane _RenderablePlane_new(const char* _file, euint _line);
#define RenderablePlane_new() _RenderablePlane_new(__FILE__, __LINE__)
API_EXPORT void RenderablePlane_delete(RenderablePlane _self);
API_EXPORT VertexBuffer RenderablePlane_get_vertex_buffer(RenderablePlane _self);
API_EXPORT IndexBuffer RenderablePlane_get_index_buffer(RenderablePlane _self);
API_EXPORT VertexDecl RenderablePlane_get_vertex_declaration(RenderablePlane _self);

typedef struct _sketch_book_config* SketchBookConfig;

API_EXPORT SketchBookConfig SketchBookConfig_new();
API_EXPORT void SketchBookConfig_delete(SketchBookConfig _self);
API_EXPORT void SketchBookConfig_set_sketch_format(SketchBookConfig _self, euint _idx, pixel_format _fmt);
API_EXPORT void SketchBookConfig_set_plaster_format(SketchBookConfig _self, pixel_format _fmt);
API_EXPORT bool SketchBookConfig_has_plaster(SketchBookConfig _self);
API_EXPORT pixel_format SketchBookConfig_get_plaster_format(SketchBookConfig _self);
API_EXPORT void SketchBookConfig_set_size(SketchBookConfig _self, euint _width, euint _height);
API_EXPORT euint SketchBookConfig_get_num_sketchs(SketchBookConfig _self);
API_EXPORT euint SketchBookConfig_get_width(SketchBookConfig _self);
API_EXPORT euint SketchBookConfig_get_height(SketchBookConfig _self);
API_EXPORT pixel_format SketchBookConfig_get_sketch_format(SketchBookConfig _self, euint _idx);

///API_EXPORT SketchBook SketchBook_new(uint32 _width, uint32 _height);
API_EXPORT SketchBook SketchBook_new(SketchBookConfig _cfg);
API_EXPORT void SketchBook_delete(SketchBook _self);
API_EXPORT void SketchBook_draw_begin(SketchBook _self, euint _use_skhs);
API_EXPORT void SketchBook_draw_end(SketchBook _self);

API_EXPORT void SketchBook_bind_framebuffer(SketchBook _self);
API_EXPORT void SketchBook_unbind_framebuffer();
API_EXPORT void SketchBook_bind_renderbuffer();
API_EXPORT void SketchBook_unbind_renderbuffer();

API_EXPORT Texture2DPtr SketchBook_get_sketch(SketchBook _self, euint _i);
API_EXPORT Texture2DPtr SketchBook_get_plaster(SketchBook _self);

API_EXPORT SketchCube SketchCube_new(euint size, pixel_format format);
API_EXPORT void SketchCube_delete(SketchCube _self);
API_EXPORT void SketchCube_draw_begin(SketchCube _self, cube_face_index index);
API_EXPORT void SketchCube_draw_end(SketchCube _self, cube_face_index index);
API_EXPORT void SketchCube_bind_framebuffer(SketchCube _self, cube_face_index index);
API_EXPORT void SketchCube_bind_renderbuffer();
API_EXPORT void SketchCube_unbind_framebuffer();
API_EXPORT void SketchCube_unbind_renderbuffer();
API_EXPORT TextureCube SketchCube_get_sketch(SketchCube _self);

API_EXPORT void clear_sketch_book ( SketchBook curt_skb );
API_EXPORT void clear_sketch_cube_face ( SketchCube curt_skc, cube_face_index face );
#endif
