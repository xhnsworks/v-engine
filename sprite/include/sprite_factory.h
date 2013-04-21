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
	typedef xhn::hash_map< SpriteLayer*, AnimAttrArray > SpriteLayerAnimAttrMap;
	typedef xhn::hash_map< Attribute*, SpriteLayer* > AnimAttrSpriteLayerMap;
protected:
    SpriteRenderer* m_renderer;
	xhn::static_string m_configName;
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
};

struct FSpriteDestProc
{
	void operator () (SpriteLayer* ptr) {
		SpriteFactory::SpriteLayerDestCallback(ptr);
	}
};
#endif