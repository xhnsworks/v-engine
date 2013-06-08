#ifndef SPRITE_FACTORY_H
#define SPRITE_FACTORY_H
#include "xhn_hash_set.hpp"
#include "xhn_hash_map.hpp"
#include "xhn_lock.hpp"
#include "xhn_vector.hpp"
#include "attribute.h"
class Sprite;
class SpriteLayer;

typedef xhn::vector<Attribute*> AnimAttrArray;
typedef xhn::list< Sprite* > RenderList;
typedef xhn::list<Sprite*>::iterator RenderHandle;
typedef xhn::hash_map<Sprite*, RenderHandle> RenderHandleMap;
typedef xhn::hash_map< SpriteLayer*, AnimAttrArray > SpriteLayerAnimAttrMap;
typedef xhn::hash_map< Attribute*, SpriteLayer* > AnimAttrSpriteLayerMap;

class InterfaceRenderList : public MemObject
{
private:
	xhn::RWLock m_renderListLock;
	RenderList m_renderList;
	/// hash map do not need to lock
	RenderHandleMap m_renderHandleMap;
	SpriteLayerAnimAttrMap m_spriteLayerAnimAttrMap;
	AnimAttrSpriteLayerMap m_animAttrSpriteLayerMap;
	static InterfaceRenderList* s_InterfaceRenderList;
public:
	SpriteLayerAnimAttrMap& GetSpriteLayerAnimAttrMap();
	AnimAttrSpriteLayerMap& GetAnimAttrSpriteLayerMap();
	xhn::RWLock& GetRenderListLock();
	RenderList& GetRenderList();
	RenderHandleMap& GetRenderHandleMap();
	void SpriteLayerDestCallback(SpriteLayer* sl);
	bool TestAnimAttr(Attribute* aa);
	void FrameEnd(double elapsedTime);
	void AlwaysOnTop(Sprite* spt);
	static void Init();
    static InterfaceRenderList* Get();
};

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

struct FSpriteDestProc
{
	bool Test(SpriteLayer* ptr, xhn::set<SpriteLayer*>& testBuffer);
	bool Test(SpriteLayer* ptr);
	void operator () (SpriteLayer* ptr) {
		InterfaceRenderList::Get()->SpriteLayerDestCallback(ptr);
	}
};
#endif