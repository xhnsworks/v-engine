#ifndef CURSOR_H
#define CURSOR_H
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUICursorLayer : public SpriteNormalLayer
{
public:
	GUICursorLayer();
};

class GUICursor : public Sprite
{
	friend class GUICursorFactory;
protected:
	~GUICursor() {}
public:
	GUICursor(SpriteRenderer* renderer, const xhn::static_string name)
		: Sprite(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
	///virtual void GetScopeImpl(SpriteRect& result);
    virtual void Build();
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
};

class GUICursorMouseEventProc : public SpriteEventProc
{
public:
	GUICursor* m_cursor;
public:
	GUICursorMouseEventProc(GUICursor* cursor)
		: m_cursor(cursor)
	{}
	~GUICursorMouseEventProc() {}
public:
	virtual void Proc(const SpriteEvent* evt);
};

class GUICursorFactory : public SpriteFactory
{
public:
	int m_cursorCount;
public:
	GUICursorFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_cursorCount(0)
		, SpriteFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif