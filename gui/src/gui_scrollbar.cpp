#include "gui_pch.h"
#include "gui_scrollbar.h"
#include "render_system.h"
#include "gui_proc_group.h"
void GUIVertSlider::Init(const xhn::static_string configName)
{
    InitImpl<GUIHoriBarLayer>(configName, m_sizeHandle);
}

void GUIVertSlider::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
	BuildBackgroundLayer(to);

	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);

	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);

	m_fourBorders.ApplyTranform(&mat);
}

ProcGroup GUIVertSlider::NewProcGroup()
{
	ProcGroup pg;
	DraggableMouseMoveProc* mouseMoveProc = ENEW DraggableMouseMoveProc(this);
	pg.mouseMoveProc = mouseMoveProc;
	pg.mouseButtonDownProc = ENEW DraggableMouseButtonDownProc(this, mouseMoveProc);
	pg.mouseButtonUpProc = ENEW EmptyMouseButtonUpProc(this);
	return pg;
}

Sprite* GUIVertSliderFactory::MakeSpriteImpl()
{
    return ENEW GUIVertSlider(m_renderer, m_configName);
}

ProcGroup GUIVertButton::NewProcGroup()
{
	ProcGroup pg;
	pg.mouseMoveProc = ENEW PressableMouseMoveProc(this);
	pg.mouseButtonDownProc = ENEW PressableMouseButtonDownProc(this);
	pg.mouseButtonUpProc = ENEW EmptyMouseButtonUpProc(this);
	return pg;
}

Sprite* GUIVertButtonFactory::MakeSpriteImpl()
{
	return ENEW GUIVertButton(m_renderer, m_configName);
}

void GUIVertScrollbar::Init(const xhn::static_string configName)
{
}

ProcGroup GUIVertScrollbar::NewProcGroup()
{
	ProcGroup pg;
	pg.mouseMoveProc = ENEW EmptyMouseMoveProc(this);
	pg.mouseButtonDownProc = ENEW EmptyMouseButtonDownProc(this);
	pg.mouseButtonUpProc = ENEW EmptyMouseButtonUpProc(this);
	return pg;
}

Sprite* GUIVertScrollbarFactory::MakeSpriteImpl()
{
    return ENEW GUIVertScrollbar(m_renderer, m_configName);
}