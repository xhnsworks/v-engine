#include "sprite_pch.h"
#include "sprite_geom_buffer.h"
#include "sprite_renderer.h"
SpriteGeomBuffer::~SpriteGeomBuffer()
{
}
SpriteGeomBuffer::SpriteSubGeomBuffer::~SpriteSubGeomBuffer()
{
	m_buffer = NULL;
}
void SpriteGeomBuffer::Attach(xhn::static_string& filename, Mesh mesh)
{
	VertexDecl vdec = VertexDecl_new_from_string(
		"pf3"
		"tf2"
		"cf4"
		"nf3"
		"Tf3"
		"Bf3");
	MaterialInstance mat = MaterialInstance_new("default_material", filename.c_str(), NULL, "Texture");
	renderable* rbl = m_renderer->new_renderable(vdec, mat, Triangular);
	Renderable_add_mesh(rbl, mesh);
	SpriteSubGeomBuffer subGeomBuf;
	subGeomBuf.m_buffer = rbl;
	m_subGeomBuffers.push_back(subGeomBuf);
}
void SpriteGeomBuffer::DepthSort()
{
	m_sortedSubGeomBuffers.clear();
	xhn::vector<SpriteSubGeomBuffer>::iterator iter = m_subGeomBuffers.begin();
	xhn::vector<SpriteSubGeomBuffer>::iterator end = m_subGeomBuffers.end();
	for (; iter != end; iter++) {
		m_sortedSubGeomBuffers.push_front(&(*iter));
	}
}

void SpriteGeomBuffer::Clear()
{
	m_subGeomBuffers.clear();
	m_renderer->clear_renderables();
}