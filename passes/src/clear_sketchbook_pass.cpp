#include "clear_sketchbook_pass.h"
#include "shader_node_generator.h"
#include "std_pass.h"
#include "shader_log.h"
#include "pass_console.h"
Pass create_clear_sketchbook_pass(VertexDecl _decl)
{
    VtxSdrBuf vsb = create_std_vertex_shader_buffer(_decl, false, false);
    {
        ///ShaderBuffer sb = to_ShaderBuffer(vsb);
        ShaderNode vsn = create_std_vertex_shader_node(_decl, vsb, false, false);
        IVtxSdrBuf.add_prototype_node((ShaderBuffer)vsb, vsn);
        IVtxSdrBuf.add_reference_node((ShaderBuffer)vsb, ShaderNode_get_name(vsn));
        IVtxSdrBuf.complete(vsb);
        ShaderNode_delete(vsn);
    }
    PxlSdrBuf psb = IPxlSdrBuf._New();
    {
        ShaderBuffer sb = to_ShaderBuffer(psb);
        SdrNdGen sng = ISdrNdGen.New();
        ISdrNdGen.register_default_nodes(sng);

        ISdrNdGen.attach_all_prototype_nodes(sng, sb);
        ISdrNdGen.add_reference_node_1(sng, ClearSketchNode);
        IPxlSdrBuf.complete(psb);
        ISdrNdGen.Delete(sng);
    }

    Shader auto_vs = Shader_new();
    Shader auto_ps = Shader_new();
    ShaderBuffer sb = to_ShaderBuffer(vsb);

    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
            GLSL_VERSION
            "%s", sb->output);

    Shader_load_from_string(auto_vs, mbuf, VertexShader);
    slog(StdPassLog, "%s", mbuf);

    sb = to_ShaderBuffer(psb);

    snprintf(mbuf, STRING_BUFFER_SIZE - 1,
            GLSL_VERSION
            "%s", sb->output);

    Shader_load_from_string(auto_ps, mbuf, PixelShader);
    slog(StdPassLog, "%s", mbuf);

    Pass ret = create_pass_from_shader(auto_vs, auto_ps);

    IVtxSdrBuf._Delete(vsb);
    IPxlSdrBuf._Delete(psb);

    return ret;
}
