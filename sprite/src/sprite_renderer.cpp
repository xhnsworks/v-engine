#include "sprite_pch.h"
#include "sprite_renderer.h"
#include "renderable_base.h"
#include "gl_utility.h"
#include "plane_mesh.h"
#include "sfloat3.h"
#include "ray.h"

void SpriteRenderableSorter::Sort(STD_NAMESPACE::set<Renderable>& used_renderable_set, STD_NAMESPACE::list<Renderable>& sorted_renderable_list)
{
	sorted_renderable_list.clear();
    m_geomBuffer->Sort();
	const xhn::list<Renderable>& sortedRenderables = m_geomBuffer->GetSortedRenderables();
	xhn::list<Renderable>::const_iterator iter = sortedRenderables.begin();
    xhn::list<Renderable>::const_iterator end = sortedRenderables.end();
	for (; iter != end; iter++)
	{
		const Renderable rdl = *iter;
		sorted_renderable_list.push_back(rdl);
	}
}
void _dest_sprite_pass_tree(Tree _sprite_pass_tree)
{
}
SpriteRenderer::SpriteRenderer(ViewportPtr view)
: m_sprite_pass_map(_dest_sprite_pass_tree, VertexDecl_compare)
, m_totel_width((float)view->parent->width)
, m_totel_height((float)view->parent->height)
, RendererBase(view)
{
	///RendererBase::Init();
	RendererBase::CommonInit();

	float scale = 1.0f;
	float w = (float)view->width;
	float h = (float)view->height;

    ///Camera_set_proj_matrix(camera_base, orthogonal_matrix);
	Camera_set(camera_base, true, w * scale, -h * scale, 0.1f, 100.0f);
	Camera_translate(camera_base, 0.0f, 0.0f, 5.0f);

	curt_render_cam = camera_base;

	SpriteGeomBufferPtr geomBuffer = ENEW SpriteGeomBuffer(this);
	renderable_sorter = ENEW SpriteRenderableSorter(geomBuffer);
	m_geom_buffer = geomBuffer;
}

SpriteRenderer::SpriteRenderer(SpriteRenderer* prevRdr)
: m_sprite_pass_map(_dest_sprite_pass_tree, VertexDecl_compare)
, RendererBase(prevRdr)
{
}

void SpriteRenderer::render()
{
    Pass std_pass = {NULL};
    glViewport ( x, y, width, height );
    glClear ( GL_DEPTH_BUFFER_BIT );
    glDepthMask ( GL_FALSE );

	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	ERROR_PROC;

	renderable_sorter->Sort(used_renderable_set, sorted_renderable_list);
	STD_NAMESPACE::list<Renderable>::iterator r_iter = sorted_renderable_list.begin();
	STD_NAMESPACE::list<Renderable>::iterator r_iter_end = sorted_renderable_list.end();
    euint count = 0;

	for (; r_iter != r_iter_end; r_iter++) {
        var key, data;

		Renderable rbl = *r_iter;
		
        if ( !rbl->world_matrix ) {
            continue;
        }

		curt_render_cam = camera_base;
        curt_rend_world_matrix = rbl->world_matrix;

        ///const char *mat_name = MaterialInstance_get_material_name ( rbl->material );
		const char *mat_name = rbl->material->GetMaterialName().c_str();

        key.str_var = mat_name;
        Iterator iter = Tree_find ( material_table, key, &data );

        if ( !iter ) {
            continue;
        }

        MaterialPrototype mp = ( MaterialPrototype ) data.vptr_var;

        if ( mp->draw_mode == Shaded ) {
            glPolygonMode ( GL_FRONT, GL_FILL );
        }
        else {
            glPolygonMode ( GL_FRONT, GL_LINE );
        }

        ERROR_PROC;

		glDisable ( GL_CULL_FACE );

        ERROR_PROC;

        prepare_renderable ( rbl );
        std_pass = rbl->std_pass;

        curt_mat_proto = mp;
        curt_mat_inst = rbl->material.get();
        Pass_auto_set_uniform_params ( std_pass, this, false );

        euint32 face_count = IndexBuffer_get_num_faces ( rbl->idx_buf );
        e_mesh_mode mode = IndexBuffer_get_mesh_mode ( rbl->idx_buf );

		euint32 clip_plane_count = 0;
		xhn::vector<plane>::iterator cpIter = rbl->clip_planes.begin();
		xhn::vector<plane>::iterator cpEnd = rbl->clip_planes.end();
		for (; cpIter != cpEnd; cpIter++, clip_plane_count++) {
			plane& p = *cpIter;
			GLPlane glplane = Plane_to_glplane(&p); 
			glClipPlane(GL_CLIP_PLANE0 + clip_plane_count, glplane.GetPointer());
			glEnable(GL_CLIP_PLANE0 + clip_plane_count);
		}

        if ( mode == Triangular ) {
            Pass_render ( std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 3, mode );
        }
        else {
            Pass_render ( std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 2, mode );
        }

		for (euint32 i = 0; i < clip_plane_count; i++) {
			glDisable(GL_CLIP_PLANE0 + i);
		}

        ERROR_PROC;

        count++;
        ///r_iter = Tree_next ( r_iter );
    }
}

void SpriteRenderer::prepare_renderable(Renderable _rbl)
{
	if (_rbl->std_pass_dirty_flag) {
		TTMap<_vertex_decl, _pass>::TTIterator iter = m_sprite_pass_map.Find(_rbl->vtx_dec);
		if (!iter.IsValid()) {
			std_pass_status status;
			
			status.dec.dec = _rbl->vtx_dec;
			status.dec.usage_tex_flags = COLOR_MAP_MASK;
			status.dec.vtx_color_func = Multiply;
			status.use_proj_mat = true;
			status.use_world_mat = false;
			status.use_gausscian_blur = false;
			status.num_render_pipes = 1;

			status.render_pipes[0].out = ColorOutput;
			status.render_pipes[0].tgt = RenderTarget0;

			Pass ret = create_std_pass_from_dec(&status, true);
			_rbl->std_pass = ret;
			m_sprite_pass_map.Insert(_rbl->vtx_dec, ret);
		}
		else
			_rbl->std_pass = iter.GetData();
		_rbl->std_pass_dirty_flag = false;
	}
}

bool SpriteRenderer::is_uniform_param_source ( esint32 _src )
{
	///var key, data;
	///key.sint32_var = _src;

	///if ( Tree_find ( param_proc_tree, key, &data ) ) {
	if (param_proc_map.find((param_source)_src) != param_proc_map.end()) {
		return true;
	}
	else {
		return false;
	}
}

Texture2DPtr SpriteRenderer::get_shadow_map ( euint i )
{
	return NULL;
}
TextureCube SpriteRenderer::get_shadow_cube_map()
{
	return NULL;
}
renderer_param_value SpriteRenderer::get_shader_object_value ( esint32 _src )
{
	///var key, data;
	///key.sint32_var = _src;

	STD_NAMESPACE::map<param_source, RendererParamEntry>::iterator iter = param_proc_map.find((param_source)_src);

	///if ( Tree_find ( param_proc_tree, key, &data ) ) {
	if (iter != param_proc_map.end()) {
		///RendererParamEntry entry = ( RendererParamEntry ) data.vptr_var;
		RendererParamEntry entry = iter->second;
		return entry->get_value_proc ( this );
	}
	else {
		renderer_param_value null_ret = {NULL};
		return null_ret;
	}
}
EFloat2 SpriteRenderer::get_real_position(float _x, float _y)
{
	float half_width = (float)width * 0.5f;
	float half_height = (float)height * 0.5f;
	return EFloat2(_x - half_width, _y - half_height);
}

Mesh SpriteRenderer::new_widget_mesh(float _x, float _y, float _width, float _height, float _u0, float _u1, float _v0, float _v1,
									 const EColor& color_u0v0, const EColor& color_u1v0, const EColor& color_u1v1, const EColor& color_u0v1)
{
	float half_width = (float)width * 0.5f;
	float half_height = (float)height * 0.5f;

	_x -= half_width;
	_y -= half_height;

	Mesh ret = create_plane_mesh(_x, _y, 0.0f, _width, _height, _u0, _u1, _v0, _v1, color_u0v0, color_u1v0, color_u1v1, color_u0v1);
	return ret;
}
/**
	result[0] = makePoint(x        , y         , z);
	result[1] = makePoint(x + width, y         , z);
	result[2] = makePoint(x + width, y + height, z);
	result[3] = makePoint(x        , y + height, z);

	make_plane_points<EFloat3, MakeEFloat3>(x, y, z, width, height, pos);
**/
void SpriteRenderer::get_four_borders(float _x, float _y, float _width, float _height, FourBorders& fourBorder)
{
	sfloat3 points[4];
	float half_width = (float)width * 0.5f;
	float half_height = (float)height * 0.5f;

	_x -= half_width;
	_y -= half_height;
	make_plane_points<sfloat3, MakeSFloat3>(_x, _y, 0.0f, _width, _height, points);

	fourBorder.leftBorder.origin = SFloat3_cale_mid_point(points[0], points[3]);
	fourBorder.leftBorder.normal = SFloat3(1.0f, 0.0f, 0.0f);

	fourBorder.topBorder.origin = SFloat3_cale_mid_point(points[0], points[1]);
	fourBorder.topBorder.normal = SFloat3(0.0f, 1.0f, 0.0f);

	fourBorder.rightBorder.origin = SFloat3_cale_mid_point(points[1], points[2]);
	fourBorder.rightBorder.normal = SFloat3(-1.0f, 0.0f, 0.0f);

	fourBorder.bottomBorder.origin = SFloat3_cale_mid_point(points[2], points[3]);
	fourBorder.bottomBorder.normal = SFloat3(0.0f, -1.0f, 0.0f);
}

void FourBorders::ApplyTranform(Matrix4x4 tran)
{
	matrix4x4 nonTranslate;
	Matrix4x4_assign(&nonTranslate, tran);
	Matrix4x4_set_translate(&nonTranslate, 0.0f, 0.0f, 0.0f);
	leftBorder.normal = Matrix4x4_mul_float3(&nonTranslate, leftBorder.normal);
	topBorder.normal = Matrix4x4_mul_float3(&nonTranslate, topBorder.normal);
	rightBorder.normal = Matrix4x4_mul_float3(&nonTranslate, rightBorder.normal);
	bottomBorder.normal = Matrix4x4_mul_float3(&nonTranslate, bottomBorder.normal);
	leftBorder.normal = SFloat3_normalize(leftBorder.normal);
	topBorder.normal = SFloat3_normalize(topBorder.normal);
	rightBorder.normal = SFloat3_normalize(rightBorder.normal);
	bottomBorder.normal = SFloat3_normalize(bottomBorder.normal);
	leftBorder.origin = Matrix4x4_mul_float3(tran, leftBorder.origin);
	topBorder.origin = Matrix4x4_mul_float3(tran, topBorder.origin);
	rightBorder.origin = Matrix4x4_mul_float3(tran, rightBorder.origin);
	bottomBorder.origin = Matrix4x4_mul_float3(tran, bottomBorder.origin);
}

inline bool BorderTest(const Border& b, const sfloat3& p)
{
	sfloat3 tmp = SFloat3_sub(p, b.origin);
	return SFloat3_dot(tmp, b.normal) > 0.0f;
}

bool FourBorders::IsInBorders(const sfloat3& point) const
{
	return BorderTest(leftBorder, point) &&
		   BorderTest(topBorder, point) &&
		   BorderTest(rightBorder, point) &&
		   BorderTest(bottomBorder, point);
}