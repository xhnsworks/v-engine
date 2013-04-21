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
		float m_depthMin;
		float m_depthMax;
		Renderable m_buffer;
		SpriteSubGeomBuffer()
			: m_depthMin(0.0f)
            , m_depthMax(0.0f)
			, m_buffer(NULL)
		{}
		~SpriteSubGeomBuffer();
		inline void Clear() {
			if (m_buffer)
				Renderable_clear(m_buffer);
		}
	};
private:
	xhn::map<xhn::static_string, SpriteSubGeomBuffer> m_subGeomBufferMap;
	xhn::list<SpriteSubGeomBuffer*> m_sortedSubGeomBuffers;
	SpriteRenderer* m_renderer;
public:
	SpriteGeomBuffer(SpriteRenderer* renderer)
		: m_renderer(renderer)
	{}
	~SpriteGeomBuffer();
	void Attach(xhn::static_string& filename, Mesh mesh, float depth);
	void DepthSort();
	const xhn::list<SpriteSubGeomBuffer*>& GetSortedSubGeomBuffers() const {
		return m_sortedSubGeomBuffers;
	}
	void Clear();
};
typedef xhn::SmartPtr<SpriteGeomBuffer> SpriteGeomBufferPtr;
#endif