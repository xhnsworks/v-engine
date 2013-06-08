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

static AnimAttrSpriteLayerMap s_animAttrSpriteLayerMap;

InterfaceRenderList* InterfaceRenderList::s_InterfaceRenderList = NULL;

SpriteLayerAnimAttrMap& InterfaceRenderList::GetSpriteLayerAnimAttrMap()
{
	return m_spriteLayerAnimAttrMap;
}
AnimAttrSpriteLayerMap& InterfaceRenderList::GetAnimAttrSpriteLayerMap()
{
	return m_animAttrSpriteLayerMap;
}
xhn::RWLock& InterfaceRenderList::GetRenderListLock()
{
    return m_renderListLock;
}
RenderList& InterfaceRenderList::GetRenderList()
{
    return m_renderList;
}
RenderHandleMap& InterfaceRenderList::GetRenderHandleMap()
{
	return m_renderHandleMap;
}

void InterfaceRenderList::SpriteLayerDestCallback(SpriteLayer* sl)
{
	SpriteLayerAnimAttrMap::bucket* b = m_spriteLayerAnimAttrMap.find_bucket(sl);
	{
		xhn::RWLock::Instance inst = b->get_write_lock();
		AnimAttrArray* a = b->find_unlock(sl);
		if (a) {
			for (euint i = 0; i < a->size(); i++)
				m_animAttrSpriteLayerMap.erase( (*a)[i] );
		}
	}
	m_spriteLayerAnimAttrMap.erase(sl);
	if (sl->IsSprite()) {
        Sprite* spt = static_cast<Sprite*>(sl);
		xhn::RWLock::Instance inst = m_renderListLock.GetWriteLock();
		RenderHandle* handlePtr = m_renderHandleMap.find(spt);
		if (handlePtr) {
			m_renderList.remove(*handlePtr);
			m_renderHandleMap.erase(spt);
		}
	}
}

bool InterfaceRenderList::TestAnimAttr(Attribute* aa)
{
    return s_animAttrSpriteLayerMap.test(aa);
}

void InterfaceRenderList::FrameEnd(double elapsedTime)
{
	xhn::RWLock::Instance inst = m_renderListLock.GetReadLock();
	RenderList::iterator iter = m_renderList.begin();
	RenderList::iterator end = m_renderList.end();
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

void InterfaceRenderList::AlwaysOnTop(Sprite* spt)
{
	xhn::RWLock::Instance inst = m_renderListLock.GetReadLock();
	RenderHandle* handlePtr = m_renderHandleMap.find(spt);
	if (handlePtr) {
		m_renderList.throw_front(*handlePtr);
	} 
}

void InterfaceRenderList::Init()
{
	if (!s_InterfaceRenderList)
		s_InterfaceRenderList = ENEW InterfaceRenderList;
}
InterfaceRenderList* InterfaceRenderList::Get()
{
	return s_InterfaceRenderList;
}