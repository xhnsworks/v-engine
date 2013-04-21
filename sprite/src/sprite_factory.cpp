#include "sprite_pch.h"
#include "sprite_factory.h"
#include "sprite_event_hub.h"
SpriteFactory::SpriteLayerAnimAttrMap SpriteFactory::s_spriteLayerAnimAttrMap;
SpriteFactory::AnimAttrSpriteLayerMap SpriteFactory::s_animAttrSpriteLayerMap;
Sprite* SpriteFactory::MakeSprite()
{
    Sprite* ret = MakeSpriteImpl();
	ret->RegisterAnimAttrs(s_spriteLayerAnimAttrMap, s_animAttrSpriteLayerMap);
	SpriteEventHub::Get()->RegisterSprite(ret);
	return ret;
}

void SpriteFactory::SpriteLayerDestCallback(SpriteLayer* sl)
{
	SpriteLayerAnimAttrMap::bucket* b = s_spriteLayerAnimAttrMap.find_bucket(sl);
	{
		xhn::RWLock2::Instance inst = b->get_write_lock();
		AnimAttrArray* a = b->find_unlock(sl);
		if (a) {
			for (uint i = 0; i < a->size(); i++)
				s_animAttrSpriteLayerMap.erase( (*a)[i] );
		}
	}
	s_spriteLayerAnimAttrMap.erase(sl);
}

bool SpriteFactory::TestAnimAttr(Attribute* aa)
{
    return s_animAttrSpriteLayerMap.test(aa);
}