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
void SpriteGeomBuffer::Attach(xhn::static_string& filename, Mesh mesh, float depth)
{
    SpriteSubGeomBuffer& subGeomBuf = m_subGeomBufferMap[filename];
	if (!subGeomBuf.m_buffer) {
		VertexDecl vdec = VertexDecl_new_from_string(
			"pf3"
			"tf2"
			"cf4"
			"nf3"
			"Tf3"
			"Bf3");
		MaterialInstance mat = MaterialInstance_new("default_material", filename.c_str(), NULL, "Texture");
		subGeomBuf.m_buffer = m_renderer->new_renderable(vdec, mat, Triangular);
		subGeomBuf.m_depthMin = depth;
		subGeomBuf.m_depthMax = depth;
	}
	Renderable_add_mesh(subGeomBuf.m_buffer, mesh);
	if (depth < subGeomBuf.m_depthMin)
		subGeomBuf.m_depthMin = depth;
	if (depth > subGeomBuf.m_depthMax)
		subGeomBuf.m_depthMax = depth;
}
void SpriteGeomBuffer::DepthSort()
{
	m_sortedSubGeomBuffers.clear();
	xhn::map<xhn::static_string, SpriteSubGeomBuffer>::iterator iter = m_subGeomBufferMap.begin();
	for (; iter != m_subGeomBufferMap.end(); iter++)
	{
        SpriteSubGeomBuffer& subGeomBuf = iter->second;
		if (m_sortedSubGeomBuffers.empty())
		{
			m_sortedSubGeomBuffers.push_back(&iter->second);
		}
		else
		{
			bool hasInserted = false;
			xhn::list<SpriteSubGeomBuffer*>::iterator listIter = m_sortedSubGeomBuffers.begin();
			for (; listIter != m_sortedSubGeomBuffers.end(); listIter++)
			{
                SpriteSubGeomBuffer* buf = *listIter;
				if (subGeomBuf.m_depthMin > buf->m_depthMin)
				{
					m_sortedSubGeomBuffers.insert_after(listIter, &subGeomBuf);
					hasInserted = true;
					break;
				}
			}
			if (!hasInserted)
			{
				m_sortedSubGeomBuffers.push_front(&subGeomBuf);
			}
		}
	}
}

void SpriteGeomBuffer::Clear()
{
	xhn::list<SpriteSubGeomBuffer*>::iterator iter = m_sortedSubGeomBuffers.begin();
	xhn::list<SpriteSubGeomBuffer*>::iterator end = m_sortedSubGeomBuffers.end();
	for (; iter != end; iter++)
	{
		SpriteSubGeomBuffer* subGeomBuf = *iter;
		subGeomBuf->Clear();
	}
}