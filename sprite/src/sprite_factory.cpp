#include "sprite_pch.h"
#include "sprite_factory.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"

bool FSpriteDestProc::Test(SpriteLayer* ptr, xhn::set<SpriteLayer*>& testBuffer) {
	SpriteLayerList::iterator iter = ptr->m_children.begin();
	SpriteLayerList::iterator end = ptr->m_children.end();
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
	SpriteLayerList::iterator iter = ptr->m_children.begin();
	SpriteLayerList::iterator end = ptr->m_children.end();
	xhn::set<SpriteLayer*> buffer;
	buffer.insert(ptr);
	for (; iter != end; iter++) {	
		if (!Test(ptr, buffer))
			return false;
	}
	return true;
}

xhn::RWLock SpriteFactory::s_renderListLock;
SpriteFactory::RenderList SpriteFactory::s_renderList;
/// hash map do not need to lock
SpriteFactory::RenderHandleMap SpriteFactory::s_renderHandleMap;
SpriteFactory::SpriteLayerAnimAttrMap SpriteFactory::s_spriteLayerAnimAttrMap;
SpriteFactory::AnimAttrSpriteLayerMap SpriteFactory::s_animAttrSpriteLayerMap;
Sprite* SpriteFactory::MakeSprite()
{
    Sprite* ret = MakeSpriteImpl();
	ret->RegisterAnimAttrs(s_spriteLayerAnimAttrMap, s_animAttrSpriteLayerMap);
	SpriteEventHub::Get()->RegisterSprite(ret);
	RenderHandle handle;
	{
		xhn::RWLock::Instance inst = s_renderListLock.GetWriteLock();
		handle = s_renderList.push_back(ret);
	}
	if (handle.is_valid())
	    s_renderHandleMap.insert(ret, handle);
	return ret;
}

void SpriteFactory::SpriteLayerDestCallback(SpriteLayer* sl)
{
	SpriteLayerAnimAttrMap::bucket* b = s_spriteLayerAnimAttrMap.find_bucket(sl);
	{
		xhn::RWLock::Instance inst = b->get_write_lock();
		AnimAttrArray* a = b->find_unlock(sl);
		if (a) {
			for (euint i = 0; i < a->size(); i++)
				s_animAttrSpriteLayerMap.erase( (*a)[i] );
		}
	}
	s_spriteLayerAnimAttrMap.erase(sl);
	Sprite* spt = sl->DynamicCast<Sprite>();
	if (spt) {
		xhn::RWLock::Instance inst = s_renderListLock.GetWriteLock();
		RenderHandle* handlePtr = s_renderHandleMap.find(spt);
		if (handlePtr) {
		    s_renderList.remove(*handlePtr);
			s_renderHandleMap.erase(spt);
		}
	}
}

bool SpriteFactory::TestAnimAttr(Attribute* aa)
{
    return s_animAttrSpriteLayerMap.test(aa);
}

void SpriteFactory::FrameEnd(double elapsedTime)
{
	xhn::RWLock::Instance inst = s_renderListLock.GetReadLock();
	RenderList::iterator iter = s_renderList.begin();
	RenderList::iterator end = s_renderList.end();
	for (; iter != end; iter++) {
		Sprite* spt = *iter;
        if (!spt->m_parent) {
            ///xhn::static_string name = spt->GetName();
            ///printf("sprite name %s\n", name.c_str());
            spt->Tick(elapsedTime);
            spt->Build();
            spt->AttachToGeomBuffer(spt->m_renderer->GetGeomBuffer());
        }
	}
}

void SpriteFactory::AlwaysOnTop(Sprite* spt)
{
	xhn::RWLock::Instance inst = s_renderListLock.GetReadLock();
	RenderHandle* handlePtr = s_renderHandleMap.find(spt);
	if (handlePtr) {
		s_renderList.throw_front(*handlePtr);
	} 
}