#include "sprite_pch.h"
#include "sprite_event.h"
#include "sprite.h"
#include "sprite_renderer.h"
ImplementRootRTTI(SpriteEvent);
ImplementRTTI(SpriteFrameStartEvent, SpriteEvent);

void SpriteFrameStartEventProc::Proc(const SpriteEvent* evt)
{
    m_sprite->Build();
	m_sprite->AttachToGeomBuffer(m_renderer->GetGeomBuffer());
}