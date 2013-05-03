#ifndef SPRITE_EVENT_HUB_H
#define SPRITE_EVENT_HUB_H
#include "xhn_vector.hpp"
#include "xhn_set.hpp"
#include "xhn_map.hpp"
#include "sprite_event.h"
#include "sprite.h"
#include "rtti.hpp"
#include "emem.hpp"
#include "ray.h"

class SpriteMouseMoveEvent : public SpriteEvent
{
	DeclareRTTI;
public:
	SInt2 m_curtMousePos;
public:
	SpriteMouseMoveEvent()
	{}
};

class SpriteMouseButtonDownEvent : public SpriteEvent
{
	DeclareRTTI;
public:
	bool m_leftButtomDown;
	bool m_middleButtonDown;
	bool m_rightButtonDown;
public:
	SpriteMouseButtonDownEvent()
		: m_leftButtomDown(false)
		, m_middleButtonDown(false)
		, m_rightButtonDown(false)
	{}
};

class SpriteMouseButtonUpEvent : public SpriteEvent
{
	DeclareRTTI;
public:
	bool m_leftButtomUp;
	bool m_middleButtonUp;
	bool m_rightButtonUp;
public:
	SpriteMouseButtonUpEvent()
		: m_leftButtomUp(false)
		, m_middleButtonUp(false)
		, m_rightButtonUp(false)
	{}
};

class SpriteEventHub : public MemObject
{
private:
	static SpriteEventHub* s_singleton_inst;
public:
	static void Init();
	static void Dest();
	static SpriteEventHub* Get();
private:
	xhn::map< const RTTI*, xhn::set<Sprite*> > m_spriteMap;
	ReceiverSet m_receiverSet;
private:
	SpriteEventHub();
	~SpriteEventHub();
public:
	void BroadcastPublicEvent(const SpriteEvent& evt, const ReceiverSet& reces);
	void RegisterSprite(Sprite* spt);
	void BroadcastFrameStartEvent(const SpriteFrameStartEvent& evt, const ReceiverSet& reces);
	inline const ReceiverSet& GetAllReceivers() const {
		return m_receiverSet;
	}
};
#endif