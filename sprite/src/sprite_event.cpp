#include "sprite_pch.h"
#include "sprite_event.h"
#include "sprite.h"
#include "sprite_renderer.h"
ImplementRootRTTI(SpriteEvent);
ImplementRTTI(SpriteFrameStartEvent, SpriteEvent);

void SpriteFrameStartEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteFrameStartEvent* frameStartEvent = evt->DynamicCast<SpriteFrameStartEvent>();
	m_sprite->Tick(frameStartEvent->m_elapsedTime);
    m_sprite->Build();
	m_sprite->AttachToGeomBuffer(m_renderer->GetGeomBuffer());
}