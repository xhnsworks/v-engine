#ifndef SPRITE_GEOM_BUFFER_H
#define SPRITE_GEOM_BUFFER_H
#include "common.h"
#include "etypes.h"
#include "xhn_list.hpp"
#include "xhn_map.hpp"
#include "xhn_static_string.hpp"
#include "emem.hpp"
#include "xhn_smart_ptr.hpp"
#include "renderable.h"

class SpriteRenderer;
class SpriteGeomBuffer : public RefObject
{
public:
	class SpriteSubGeomBuffer : public MemObject
	{
	public:
		MeshPtr m_mesh;
		MaterialInstancePtr m_matInst;
		const FourBorders* m_fourBorders;
		SpriteSubGeomBuffer()
		{}
		~SpriteSubGeomBuffer()
		{}
	};
private:
	xhn::list<SpriteSubGeomBuffer> m_subGeomBuffers;
	xhn::list<Renderable> m_sortedRenderables;
	SpriteRenderer* m_renderer;
public:
	SpriteGeomBuffer(SpriteRenderer* renderer)
		: m_renderer(renderer)
	{}
	~SpriteGeomBuffer();
	void Attach(xhn::static_string& filename, 
		        Mesh mesh, 
				const FourBorders* fourBorders);
	void Sort();
	const xhn::list<Renderable>& GetSortedRenderables() const {
		return m_sortedRenderables;
	}
	void Clear();
};
typedef xhn::SmartPtr<SpriteGeomBuffer> SpriteGeomBufferPtr;
#endif