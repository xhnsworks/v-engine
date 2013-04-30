#include "line_utility.h"
#include "sfloat3.h"
#include "locator.h"
#include "mesh.h"
#include "math_base.h"
ShaderNode flat_material_proc(PxlSdrBuf _psb, int _id)
{
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    vec3 cmap = texture(ColorMap, vTexCoord).rgb;\n"
                            "    gl_FragData[0] = vec4( cmap, 1.0 );"
                            "}\n");

    return psn;
}

ShaderNode red_material_proc4(PxlSdrBuf _psb, int _id)
{
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "RedPixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    gl_FragData[0] = vec4( 1.0, 0.0, 0.0, 1.0 );"
                            "}\n");
    return psn;
}

LineDrawer LineDrawer_new(Renderer* _rdr)
{
    LineDrawer ret = (LineDrawer)SMalloc(sizeof(line_drawer));
    ///std_pass_status ps_status;
    /**
    ps_status.dec.dec = VertexDecl_new_from_string(
                                                     "pf3"
                                                     "cf4"
                                                     "nf3"
                                                     );
    ps_status.dec.usage_tex_flags = 0;
    ps_status.num_render_pipes = 2;
    ps_status.render_pipes[0].out = ColorOutput;
    ps_status.render_pipes[0].tgt = RenderTarget0;
    ps_status.render_pipes[1].out = NormalTangentOutput;
    ps_status.render_pipes[1].tgt = RenderTarget1;
    **/

    VertexDecl decl = VertexDecl_new_from_string("pf3"
                                       "tf2"
                                       "nf3"
                                       "cf4"
                                       "Tf3"
                                       "Bf3");
    SDisplayProc s;
    s.proc = flat_material_proc;
    _rdr->register_material("flat_material", s, Shaded, true);

    MaterialInstance inst = MaterialInstance_new("flat_material", NULL, NULL, "Texture");
    Renderable rabl = _rdr->new_renderable(decl, inst, Segment);
    _rdr->use_renderable(rabl);

    ret->line_strip_rabl = rabl;
    ret->rdr = _rdr;

    ///MaterialInstance_delete(inst);

    line_def null_def;
    memset(&null_def, 0, sizeof(null_def));
    ret->lines = array_new(line_def, 100, null_def);

    ///Pass std_pass = create_flat_color_pass(&ps_status, true, false);
    return ret;
}

void LineDrawer_delete(LineDrawer _self)
{
    array_delete(_self->lines);
    Mfree(_self);
}

void LineDrawer_update(LineDrawer _self)
{
    Renderable_clear(_self->line_strip_rabl);
    euint n = array_n(_self->lines);
    ///EFloat3* pos_stream = (EFloat3*)SMalloc(sizeof(EFloat3) * n * 2);
    ///EFloat4* col_stream = (EFloat4*)SMalloc(sizeof(EFloat4) * n * 2);
    ///EFloat3* nor_stream = (EFloat3*)SMalloc(sizeof(EFloat3) * n * 2);
	EFloat3* pos_stream = ENEW_ARRAY EFloat3[n * 2];
	EFloat4* col_stream = ENEW_ARRAY EFloat4[n * 2];
	EFloat3* nor_stream = ENEW_ARRAY EFloat3[n * 2];
    euint32* idx_stream = (euint32*)SMalloc(sizeof(euint32) * n * 2);
    euint count = 0;
    MeshPtr m = Mesh_new();
    for (euint i = 0; i < n; i++)
    {
        pos_stream[count] = _self->lines[i].begin;
        col_stream[count] = _self->lines[i].begin_color;
        nor_stream[count] = _self->lines[i].begin_normal;
        idx_stream[count] = count;
        count++;
        pos_stream[count] = _self->lines[i].end;
        col_stream[count] = _self->lines[i].end_color;
        nor_stream[count] = _self->lines[i].end_normal;
        idx_stream[count] = count;
        count++;
    }

    Mesh_build2(m.get(), (float*)pos_stream, NULL, (float*)nor_stream, (float*)col_stream,
                n * 2,
                idx_stream, n,
                Segment);

    Renderable_add_mesh(_self->line_strip_rabl, m);
    /**
    matrix4x4* wmat = Renderable_get_world_matrix(_self->line_strip_rabl);
    Matrix4x4_set_one(wmat);
    Matrix4x4_set_translate(wmat, 0.0f, 0.5f, 0.0f);
**/
    ///Mfree(pos_stream);
    ///Mfree(col_stream);
    ///Mfree(nor_stream);
	EDELETE_ARRAY[] pos_stream;
	EDELETE_ARRAY[] col_stream;
	EDELETE_ARRAY[] nor_stream;
    Mfree(idx_stream);
    ///Mesh_delete(mesh);
	///delete m;

    array_n(_self->lines) = 0;
}
void LineDrawer_add_line(LineDrawer _self, line_def* def)
{
    apush(_self->lines, *def);
}
void LineDrawer_draw_circle(LineDrawer _self, EFloat3* ori, EFloat3* dir, float radius)
{
    matrix4x4* mat = Matrix4x4_new();
    int num_sect = 12;
    ///sfloat3 axis = SFloat3_assign_from_efloat3(dir);
    sfloat3 axis = SFloat3(0.0f, 0.0f, 1.0f);
    float r = 2.0f * E_PI / (float)num_sect;
    Matrix4x4_from_axis_angle(mat, axis, r);
    sfloat3 pt = SFloat3(0.0f, radius, 0.0f);
    sfloat3 nor = SFloat3(0.0f, 0.0f, 1.0f);
    sfloat3* pts = (sfloat3*)SMalloc(sizeof(sfloat3) * num_sect * 2);
    sfloat3* nors = (sfloat3*)SMalloc(sizeof(sfloat3) * num_sect * 2);
    for (int i = 0; i < num_sect; i++)
    {
        pts[i * 2] = pt;
        nors[i * 2] = nor;
        ///SFloat3_print(pt);

        pt = Matrix4x4_mul_float3(mat, pt);
        pts[i * 2 + 1] = pt;
        nors[i * 2 + 1] = nor;

        ///SFloat3_print(pt);
    }
    sfloat3 dst_vec = SFloat3_assign_from_efloat3(dir);
    Matrix4x4_from_vec_to_vec(mat, axis, dst_vec);

    sfloat3 pos = SFloat3_assign_from_efloat3(ori);
    for (int i = 0; i < num_sect * 2; i++)
    {
        pts[i] = Matrix4x4_mul_float3(mat, pts[i]);
        nors[i] = Matrix4x4_mul_float3(mat, nors[i]);

        pts[i] = SFloat3_add(pts[i], pos);
    }
    EFloat4 color(1.0f, 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < num_sect; i++)
    {
        line_def line;
        line.begin = SFloat3_convert(pts[i * 2]);
        line.begin_normal = SFloat3_convert(nors[i * 2]);
        line.begin_color = color;

        line.end = SFloat3_convert(pts[i * 2 + 1]);
        line.end_normal = SFloat3_convert(nors[i * 2 + 1]);
        line.end_color = color;
        LineDrawer_add_line(_self, &line);
    }
    Mfree(pts);
    Mfree(nors);
    Matrix4x4_delete(mat);
}
void LineDrawer_draw_cone(LineDrawer _self, EFloat3* ori, EFloat3* dir, EFloat4* color, float radius)
{
    matrix4x4* mat = Matrix4x4_new();
    int num_sect = 12;

    sfloat3 axis = SFloat3(0.0f, 0.0f, 1.0f);
    float r = 2.0f * E_PI / (float)num_sect;
    Matrix4x4_from_axis_angle(mat, axis, r);
    sfloat3 pt = SFloat3(0.0f, radius, 0.0f);
    sfloat3 nor = SFloat3(0.0f, 0.0f, 1.0f);
    sfloat3* pts = (sfloat3*)SMalloc(sizeof(sfloat3) * num_sect * 2);
    sfloat3* nors = (sfloat3*)SMalloc(sizeof(sfloat3) * num_sect * 2);
    for (int i = 0; i < num_sect; i++)
    {
        pts[i * 2] = pt;
        nors[i * 2] = nor;

        pt = Matrix4x4_mul_float3(mat, pt);
        pts[i * 2 + 1] = pt;
        nors[i * 2 + 1] = nor;
    }
    sfloat3 dst_vec = SFloat3_assign_from_efloat3(dir);
    Matrix4x4_from_vec_to_vec(mat, axis, dst_vec);

    sfloat3 pos = SFloat3_assign_from_efloat3(ori);
    for (int i = 0; i < num_sect * 2; i++)
    {
        pts[i] = Matrix4x4_mul_float3(mat, pts[i]);
        nors[i] = Matrix4x4_mul_float3(mat, nors[i]);

        pts[i] = SFloat3_add(pts[i], pos);
    }
    /// ¼ÆËã¼â¶Ë×ø±ê
    pt = SFloat3_assign_from_efloat3(ori);
    sfloat3 d = SFloat3_assign_from_efloat3(dir);
    d = SFloat3_mul_float(radius * 2.0f, d);
    pt = SFloat3_add(pt, d);

    for (int i = 0; i < num_sect; i++)
    {
        line_def line;
        line.begin = SFloat3_convert(pts[i * 2]);
        line.begin_normal = SFloat3_convert(nors[i * 2]);
        line.begin_color = *color;

        line.end = SFloat3_convert(pts[i * 2 + 1]);
        line.end_normal = SFloat3_convert(nors[i * 2 + 1]);
        line.end_color = *color;
        LineDrawer_add_line(_self, &line);

        line.begin = SFloat3_convert(pts[i * 2]);
        line.begin_normal = SFloat3_convert(nors[i * 2]);
        line.begin_color = *color;

        line.end = SFloat3_convert(pt);
        line.end_normal = *dir;
        line.end_color = *color;
        LineDrawer_add_line(_self, &line);
    }

    Mfree(pts);
    Mfree(nors);
    Matrix4x4_delete(mat);
}
void LineDrawer_draw_translate_manipulator(LineDrawer _self, EFloat3* ori, EFloat3* z_axis, float radius)
{
    /**
    EFloat3 x_axis = {1.0f, 0.0f, 0.0f};
    EFloat3 y_axis = {0.0f, 1.0f, 0.0f};
    EFloat3 z_axis = {0.0f, 0.0f, 1.0f};
    **/
    sfloat3 xaxis = SFloat3(1.0f, 0.0f, 0.0f);
    sfloat3 yaxis = SFloat3(0.0f, 1.0f, 0.0f);
    sfloat3 zaxis = SFloat3(0.0f, 0.0f, 1.0f);
    sfloat3 o = SFloat3_assign_from_efloat3(ori);
    sfloat3 xo = SFloat3_mul_float(radius, xaxis);
    sfloat3 yo = SFloat3_mul_float(radius, yaxis);
    sfloat3 zo = SFloat3_mul_float(radius, zaxis);
    xo = SFloat3_add(xo, o);
    yo = SFloat3_add(yo, o);
    zo = SFloat3_add(zo, o);

    EFloat4 xcol(1.0f, 0.0f, 0.0f, 1.0f);
    EFloat4 ycol(0.0f, 1.0f, 0.0f, 1.0f);
    EFloat4 zcol(0.0f, 0.0f, 1.0f, 1.0f);
    {
        EFloat3 pt = SFloat3_convert(xo);
        EFloat3 dir = SFloat3_convert(xaxis);
        LineDrawer_draw_cone(_self, &pt, &dir, &xcol, radius * 0.05f);
    }
    {
        EFloat3 pt = SFloat3_convert(yo);
        EFloat3 dir = SFloat3_convert(yaxis);
        LineDrawer_draw_cone(_self, &pt, &dir, &ycol, radius * 0.05f);
    }
    {
        EFloat3 pt = SFloat3_convert(zo);
        EFloat3 dir = SFloat3_convert(zaxis);
        LineDrawer_draw_cone(_self, &pt, &dir, &zcol, radius * 0.05f);
    }
    {
        line_def line;
        line.begin = *ori;
        line.begin_normal = SFloat3_convert(xaxis);
        line.begin_color = xcol;

        line.end = SFloat3_convert(xo);
        line.end_normal = SFloat3_convert(xaxis);
        line.end_color = xcol;
        LineDrawer_add_line(_self, &line);
    }
    {
        line_def line;
        line.begin = *ori;
        line.begin_normal = SFloat3_convert(yaxis);
        line.begin_color = ycol;

        line.end = SFloat3_convert(yo);
        line.end_normal = SFloat3_convert(yaxis);
        line.end_color = ycol;
        LineDrawer_add_line(_self, &line);
    }
    {
        line_def line;
        line.begin = *ori;
        line.begin_normal = SFloat3_convert(zaxis);
        line.begin_color = zcol;

        line.end = SFloat3_convert(zo);
        line.end_normal = SFloat3_convert(zaxis);
        line.end_color = zcol;
        LineDrawer_add_line(_self, &line);
    }
}
