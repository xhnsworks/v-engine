#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H
#include "common.h"
#include "etypes.h"
#include "renderer_base.h"
#include "map.hpp"
#include "sprite_geom_buffer.h"
#include "matrix4x4.h"
struct Border : public MemObject
{
	sfloat3 origin;
	sfloat3 normal;
};

struct FourBorders : public MemObject
{
	Border leftBorder;
	Border topBorder;
	Border rightBorder;
	Border bottomBorder;
	void ApplyTranform(Matrix4x4 tran);
	bool IsInBorders(const sfloat3& point) const;
};

class SpriteRenderableSorter : public RenderableSorter
{
private:
	SpriteGeomBufferPtr m_geomBuffer;
public:
    SpriteRenderableSorter(SpriteGeomBufferPtr geomBuffer)
	{
        m_geomBuffer = geomBuffer;
	}
	virtual void Sort(STD_NAMESPACE::set<Renderable>& used_renderable_set, STD_NAMESPACE::list<Renderable>& sorted_renderable_list);
};
class API_EXPORT SpriteRenderer : public RendererBase
{
private:
	TTMap<_vertex_decl, _pass> m_sprite_pass_map;
	float m_totel_width;
	float m_totel_height;
	float m_z;
	SpriteGeomBufferPtr m_geom_buffer;
public:
	SpriteRenderer( ViewportPtr view );
	SpriteRenderer(SpriteRenderer* prevRdr);
	virtual void render();
	virtual void prepare_renderable(Renderable _rbl);
	virtual bool is_uniform_param_source ( sint32 _src );
	virtual Texture2DPtr get_shadow_map ( uint i );
	virtual TextureCube get_shadow_cube_map();
	virtual renderer_param_value get_shader_object_value ( sint32 _src );
	Mesh new_widget_mesh(float _x, float _y, float _width, float _height, float _u0, float _u1, float _v0, float _v1,
		                 const EColor& color_u0v0, const EColor& color_u1v0, const EColor& color_u1v1, const EColor& color_u0v1);
	void get_four_borders(float _x, float _y, float _width, float _height, FourBorders& fourBorders);
	inline void Reset() {
		m_z = 0.0f;
	}
	inline float get_depth() const {
		return m_z;
	}
	inline void clear_depth() {
		m_z = 0.0f;
	}
	inline SpriteGeomBufferPtr GetGeomBuffer() {
		return m_geom_buffer;
	}
	EFloat2 get_real_position(float _x, float _y);
};
#endif