#include "sprite_pch.h"
#include "sprite_event_hub.h"
#include "xhn_exception.hpp"
#include "sprite.h"
ImplementRTTI(SpriteMouseMoveEvent, SpriteEvent);
ImplementRTTI(SpriteMouseButtonDownEvent, SpriteEvent);
ImplementRTTI(SpriteMouseButtonUpEvent, SpriteEvent);

SpriteEventHub* SpriteEventHub::s_singleton_inst = NULL;
SpriteEventHub::SpriteEventHub()
{
}
SpriteEventHub::~SpriteEventHub()
{
}
void SpriteEventHub::Init()
{
	if (!s_singleton_inst)
        s_singleton_inst = ENEW SpriteEventHub;
}
void SpriteEventHub::Dest()
{
	if (s_singleton_inst) {
		delete s_singleton_inst;
		s_singleton_inst = NULL;
	}
}
SpriteEventHub* SpriteEventHub::Get()
{
	if (!s_singleton_inst)
		VEngineExce(ObjectUninitializedException, "SpriteEventHub is used, but it not initialized");
    return s_singleton_inst;
}

void SpriteEventHub::BroadcastPublicEvent(const SpriteEvent& evt, const ReceiverSet& reces)
{
	ReceiverSet::const_iterator iter = reces.begin();
	ReceiverSet::const_iterator end = reces.end();
	for (; iter != end; iter++) {
		const RTTI* rtti = *iter;
		xhn::map< const RTTI*, xhn::set<Sprite*> >::iterator evtProcIter = m_spriteMap.find(rtti);
		if (evtProcIter != m_spriteMap.end()) {
			xhn::set<Sprite*>& sptSet = evtProcIter->second;
			xhn::set<Sprite*>::iterator i = sptSet.begin();
			xhn::set<Sprite*>::iterator e = sptSet.end();
			for (; i != e; i++) {
				Sprite* spt = *i;
				spt->PublicEventCallback(&evt);
			}
		}
	}
}
void SpriteEventHub::RegisterSprite(Sprite* spt)
{
	const EventProcMap& evnMap = spt->GetPublicEventProcMap();
	EventProcMap::const_iterator iter = evnMap.begin();
	EventProcMap::const_iterator end = evnMap.end();
	for (; iter != end; iter++) {
        const RTTI* rtti = iter->first;
		m_spriteMap[rtti].insert(spt);
		m_receiverSet.insert(rtti);
	}
}

void SpriteEventHub::BroadcastFrameStartEvent(const SpriteFrameStartEvent& evt, const ReceiverSet& reces)
{
    BroadcastPublicEvent(evt, reces);
}