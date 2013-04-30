#include "sprite_pch.h"
#include "sprite_factory.h"
#include "sprite_event_hub.h"

bool FSpriteDestProc::Test(SpriteLayer* ptr, xhn::set<SpriteLayer*>& testBuffer) {
	xhn::vector< xhn::SmartPtr<SpriteLayer, FSpriteDestProc> >::iterator iter = ptr->m_children.begin();
	xhn::vector< xhn::SmartPtr<SpriteLayer, FSpriteDestProc> >::iterator end = ptr->m_children.end();
	for (; iter != end; iter++) {
		SpriteLayer* p = (*iter).get();
		if (testBuffer.find(p) != testBuffer.end())
			return false;
		xhn::set<SpriteLayer*> nextTestBuffer = testBuffer;
		if (!Test(p, nextTestBuffer))
			return false;
	}
	return true;
}
bool FSpriteDestProc::Test(SpriteLayer* ptr) {
	xhn::vector< xhn::SmartPtr<SpriteLayer, FSpriteDestProc> >::iterator iter = ptr->m_children.begin();
	xhn::vector< xhn::SmartPtr<SpriteLayer, FSpriteDestProc> >::iterator end = ptr->m_children.end();
	xhn::set<SpriteLayer*> buffer;
	buffer.insert(ptr);
	for (; iter != end; iter++) {	
		if (!Test(ptr, buffer))
			return false;
	}
	return true;
}

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
			for (euint i = 0; i < a->size(); i++)
				s_animAttrSpriteLayerMap.erase( (*a)[i] );
		}
	}
	s_spriteLayerAnimAttrMap.erase(sl);
}

bool SpriteFactory::TestAnimAttr(Attribute* aa)
{
    return s_animAttrSpriteLayerMap.test(aa);
}