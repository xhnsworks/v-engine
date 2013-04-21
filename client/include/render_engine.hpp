extern "C" {
#include "common.h"
#include "etypes.h"
#include "renderable.h"
#include "locator.h"
}
typedef struct _renderer* Renderer;
class RenderEngine
{
public:
    static Renderable s_pointLightsShape;
    static void DrawPointLightShapeProc(PointLight pl);
public:
    Renderer m_mainRenderer;
    Renderer m_coverRenderer;
    Renderable m_locator;
public:
    RenderEngine()
    : m_mainRenderer(NULL)
    , m_coverRenderer(NULL)
    , m_locator(NULL)
    {}
    ~RenderEngine();
    void Init(uint x, uint y, uint width, uint height, HWND hwnd);
    void Clear();
    void Render(HDC hDC);
    Ray GetMouseRay();
    MaterialInstance GetMaterialInst(VertexDecl decl, const char* mat_name, const char* color_tex_name, const char* normal_tex_name);
};
