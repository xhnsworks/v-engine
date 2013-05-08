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
		Renderable m_buffer;
		SpriteSubGeomBuffer()
			: m_buffer(NULL)
		{}
		~SpriteSubGeomBuffer();
	};
private:
	xhn::vector<SpriteSubGeomBuffer> m_subGeomBuffers;
	xhn::map<xhn::static_string, SpriteSubGeomBuffer> m_subGeomBufferMap;
	xhn::list<SpriteSubGeomBuffer*> m_sortedSubGeomBuffers;
	SpriteRenderer* m_renderer;
public:
	SpriteGeomBuffer(SpriteRenderer* renderer)
		: m_renderer(renderer)
	{}
	~SpriteGeomBuffer();
	void Attach(xhn::static_string& filename, Mesh mesh);
	void DepthSort();
	const xhn::list<SpriteSubGeomBuffer*>& GetSortedSubGeomBuffers() const {
		return m_sortedSubGeomBuffers;
	}
	void Clear();
};
typedef xhn::SmartPtr<SpriteGeomBuffer> SpriteGeomBufferPtr;
#endif