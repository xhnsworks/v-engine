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
	bool m_leftButtomDown;
	bool m_middleButtonDown;
	bool m_rightButtonDown;
public:
	SpriteMouseMoveEvent()
		: m_leftButtomDown(false)
		, m_middleButtonDown(false)
		, m_rightButtonDown(false)
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
	void _BroadcastEvent(const SpriteEvent& evt, const ReceiverSet& reces);
private:
	SpriteEventHub();
	~SpriteEventHub();
public:
	void RegisterSprite(Sprite* spt);
	void BroadcastMouseEvent(const SpriteMouseMoveEvent& evt, const ReceiverSet& reces);
	void BroadcastFrameStartEvent(const SpriteFrameStartEvent& evt, const ReceiverSet& reces);
	inline const ReceiverSet& GetAllReceivers() const {
		return m_receiverSet;
	}
};
#endif