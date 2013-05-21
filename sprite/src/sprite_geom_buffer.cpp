#include "sprite_pch.h"
#include "sprite_geom_buffer.h"
#include "sprite_renderer.h"
SpriteGeomBuffer::~SpriteGeomBuffer()
{
}
void SpriteGeomBuffer::Attach(xhn::static_string& filename, 
							  Mesh mesh, 
							  const FourBorders* fourBorders)
{
	MaterialInstance* mat = ENEW MaterialInstance("default_material", filename.c_str(), NULL, "Texture");
	SpriteSubGeomBuffer subGeomBuf;
	subGeomBuf.m_mesh = mesh;
	subGeomBuf.m_matInst = mat;
	subGeomBuf.m_fourBorders = fourBorders;
	m_subGeomBuffers.push_back(subGeomBuf);
}
void SpriteGeomBuffer::Sort()
{
	VertexDecl vdec = VertexDecl_new_from_string(
		"pf3"
		"tf2"
		"cf4"
		"nf3"
		"Tf3"
		"Bf3");
	renderable* rbl = NULL;
    xhn::list<SpriteSubGeomBuffer>::iterator iter = m_subGeomBuffers.begin();
	while (iter != m_subGeomBuffers.end()) {
		SpriteSubGeomBuffer* curtSubGeomBuffer = &(*iter);
		if (rbl) {
			if (rbl->material->GetColorTexture() == curtSubGeomBuffer->m_matInst->GetColorTexture() &&
				rbl->four_borders == curtSubGeomBuffer->m_fourBorders) {
				/// do nothing
			}
			else {
				m_sortedRenderables.push_front(rbl);
				MaterialInstance mat(curtSubGeomBuffer->m_matInst);
				rbl = m_renderer->new_renderable(vdec, &mat, Triangular);
				rbl->four_borders = curtSubGeomBuffer->m_fourBorders;
			}
		}
		else {
			MaterialInstance mat(curtSubGeomBuffer->m_matInst);
			rbl = m_renderer->new_renderable(vdec, &mat, Triangular);
			rbl->four_borders = curtSubGeomBuffer->m_fourBorders;
		}
		Renderable_add_mesh(rbl, curtSubGeomBuffer->m_mesh);
		iter++;
	}
	if (rbl) {
		m_sortedRenderables.push_front(rbl);
	}
}

void SpriteGeomBuffer::Clear()
{
	m_sortedRenderables.clear();
	m_subGeomBuffers.clear();
	m_renderer->clear_renderables();
}