#ifndef SPRITE_FACTORY_H
#define SPRITE_FACTORY_H
#include "xhn_hash_set.hpp"
#include "xhn_hash_map.hpp"
#include "xhn_lock.hpp"
#include "xhn_vector.hpp"
#include "attribute.h"
#include "sprite_event_hub.h"
class Sprite;
class SpriteLayer;

class SpriteRenderer;
template <typename SPRITE_TYPE>
class SpriteFactory : public MemObject
{
protected:
    SpriteRenderer* m_renderer;
	xhn::static_string m_configName;
protected:
    virtual Sprite* MakeSpriteImpl() = 0;
public:
	SpriteFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_renderer(renderer)
		, m_configName(cfgName)
	{}
	SPRITE_TYPE* MakeSprite()
	{
		Sprite* ret = MakeSpriteImpl();
		ret->Init();
		ret->RegisterAnimAttrs(InterfaceRenderList::Get()->GetSpriteLayerAnimAttrMap(), 
			                   InterfaceRenderList::Get()->GetAnimAttrSpriteLayerMap());
		SpriteEventHub::Get()->RegisterSprite(ret);
		RenderHandle handle;
		{
			xhn::RWLock::Instance inst = InterfaceRenderList::Get()->GetRenderListLock().GetWriteLock();
			handle = InterfaceRenderList::Get()->GetRenderList().push_back(ret);
		}
		if (handle.is_valid())
			InterfaceRenderList::Get()->GetRenderHandleMap().insert(ret, handle);
		return (SPRITE_TYPE*)ret;
	}
};

#endif