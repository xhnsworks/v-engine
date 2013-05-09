#ifndef SPRITE_FACTORY_H
#define SPRITE_FACTORY_H
#include "xhn_hash_set.hpp"
#include "xhn_hash_map.hpp"
#include "xhn_lock.hpp"
#include "xhn_vector.hpp"
#include "attribute.h"
class Sprite;
class SpriteLayer;
class SpriteRenderer;
class SpriteFactory : public MemObject
{
public:
	typedef xhn::vector<Attribute*> AnimAttrArray;
	typedef xhn::list< Sprite* > RenderList;
	typedef xhn::list<Sprite*>::iterator RenderHandle;
	typedef xhn::hash_map<Sprite*, RenderHandle> RenderHandleMap;
	typedef xhn::hash_map< SpriteLayer*, AnimAttrArray > SpriteLayerAnimAttrMap;
	typedef xhn::hash_map< Attribute*, SpriteLayer* > AnimAttrSpriteLayerMap;
protected:
    SpriteRenderer* m_renderer;
	xhn::static_string m_configName;
	static xhn::RWLock s_renderListLock;
	static RenderList s_renderList;
	static RenderHandleMap s_renderHandleMap;
	static SpriteLayerAnimAttrMap s_spriteLayerAnimAttrMap;
	static AnimAttrSpriteLayerMap s_animAttrSpriteLayerMap;
protected:
    virtual Sprite* MakeSpriteImpl() = 0;
public:
	SpriteFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_renderer(renderer)
		, m_configName(cfgName)
	{}
	Sprite* MakeSprite();
	static void SpriteLayerDestCallback(SpriteLayer* sl);
	static bool TestAnimAttr(Attribute* aa);
	static void FrameEnd(double elapsedTime);
	static void AlwaysOnTop(Sprite* spt);
};

struct FSpriteDestProc
{
	bool Test(SpriteLayer* ptr, xhn::set<SpriteLayer*>& testBuffer);
	bool Test(SpriteLayer* ptr);
	void operator () (SpriteLayer* ptr) {
		SpriteFactory::SpriteLayerDestCallback(ptr);
	}
};
#endif