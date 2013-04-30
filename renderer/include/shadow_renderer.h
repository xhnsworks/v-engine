#ifndef SHADOW_RENDERER_H
#define SHADOW_RENDERER_H
#include "common.h"
#include "etypes.h"
#include "sketch.h"
#include "texture_cube.h"
#include "texture2d.h"
#include "pass.h"
class RendererBase;
typedef struct _blurred_sketch_book {
    SketchBook sketch_book;
    SketchBookConfig sketch_cfg;
} blurred_sketch_book;
class ShadowRenderer
{
public:
    SketchBook depth_sketch_book;
    SketchBookConfig depth_sketch_cfg;
    SketchCube blurred_depth_sketch_cube;
    uint32 blurred_depth_sketch_cube_size;
    Pass blur_pass;

    blurred_sketch_book *blurred_sketch_books;
    ShadowRenderer();
    ~ShadowRenderer();
    inline TextureCube get_shadow_cube_map() {
        return SketchCube_get_sketch ( blurred_depth_sketch_cube );
    }
    Texture2DPtr get_shadow_map ( euint i );
    void prepare ( RenderablePlane render_plane );
    void delete_blurred_sketck_books();
    void depth_map_blur ( RendererBase *rdr );
    void depth_map_blur_to_cube_map ( RendererBase *rdr, euint face );
};
#endif