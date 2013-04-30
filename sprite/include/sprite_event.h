#ifndef SPRITE_EVENT_H
#define SPRITE_EVENT_H
#include "common.h"
#include "etypes.h"
#include "rtti.hpp"
#include "xhn_smart_ptr.hpp"
class SpriteEvent : public MemObject
{
	DeclareRootRTTI;
public:
	SpriteEvent()
	{}
};

class SpriteFrameStartEvent : public SpriteEvent
{
	DeclareRTTI;
public:
	double m_elapsedTime;
	SpriteFrameStartEvent(double elapsedTime)
		: m_elapsedTime(elapsedTime)
	{}
};

struct SInt2
{
	esint x;
	esint y;
	SInt2(const SInt2& int2)
		: x(int2.x)
		, y(int2.y)
	{}
	SInt2()
		: x(0)
		, y(0)
	{}
};

class SpriteEventProc : public RefObject
{
public:
	virtual ~SpriteEventProc() {}
	virtual void Proc(const SpriteEvent* evt) = 0;
};
typedef xhn::SmartPtr<SpriteEventProc> SpriteEventProcPtr;

class Sprite;
class SpriteRenderer;
class SpriteFrameStartEventProc : public SpriteEventProc
{
public:
    Sprite* m_sprite;
	SpriteRenderer* m_renderer;
public:
	SpriteFrameStartEventProc(Sprite* sprite, SpriteRenderer* renderer)
		: m_sprite(sprite)
		, m_renderer(renderer)
	{}
	virtual void Proc(const SpriteEvent* evt);
};
#endif