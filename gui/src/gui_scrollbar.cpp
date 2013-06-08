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

GUIVertScrollbar::GUIVertScrollbar(SpriteRenderer* renderer,
				                   const xhn::static_string name,
				                   const xhn::static_string sliderCfgName,
				                   const xhn::static_string topBtnCfgName,
				                   const xhn::static_string bottomBtnCfgName)
				 : m_sliderCfgName(sliderCfgName)
				 , m_topBtnCfgName(topBtnCfgName)
				 , m_bottomBtnCfgName(bottomBtnCfgName)
				 , m_vertSliderFactory(NULL)
				 , m_vertTopButtonFactory(NULL)
				 , m_vertBottomButtonFactory(NULL)
				 , m_vertSlider(NULL)
                 , m_vertTopButton(NULL)
                 , m_vertBottomButton(NULL)
				 , GUIVertBar(renderer, name)
{
	m_sliderSizeHandle.m_lock = ENEW xhn::RWLock;
	m_sliderSizeHandle.AttachAttribute<FloatAttr>();
}

void GUIVertScrollbar::Init(const xhn::static_string configName)
{
    m_vertSliderFactory = ENEW GUIVertSliderFactory(m_renderer,
                                                    m_sliderCfgName.c_str(),
                                                    m_sliderSizeHandle);
    m_vertTopButtonFactory = ENEW GUIVertButtonFactory(m_renderer,
                                                       m_topBtnCfgName.c_str());
	m_vertBottomButtonFactory = ENEW GUIVertButtonFactory(m_renderer,
		                                                  m_bottomBtnCfgName.c_str());
    
    m_vertSlider = m_vertSliderFactory->MakeSprite();
    m_vertTopButton = m_vertTopButtonFactory->MakeSprite();
    m_vertBottomButton = m_vertBottomButtonFactory->MakeSprite();

	AddChild(m_vertSlider);
	AddChild(m_vertTopButton);
	AddChild(m_vertBottomButton);
}

void GUIVertScrollbar::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
	BuildTextLayer(to);
	BuildBackgroundLayer(to);
	BuildSlider(to);
	BuildButtons(to);

	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);

	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);

	m_fourBorders.ApplyTranform(&mat);
}

void GUIVertScrollbar::BuildSlider(xhn::list<SpriteElement>& to)
{
}
void GUIVertScrollbar::BuildButtons(xhn::list<SpriteElement>& to)
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

GUIVertScrollbarFactory::GUIVertScrollbarFactory(SpriteRenderer* renderer,
												 const char* sliderCfgName,
												 const char* topBtnCfgName,
												 const char* bottomBtnCfgName,
                                                 const char* scroolbarCfgName)
: m_sliderCfgName(sliderCfgName)
, m_topBtnCfgName(topBtnCfgName)
, m_bottomBtnCfgName(bottomBtnCfgName)
, GUIVertBarFactory(renderer, scroolbarCfgName)
{
}

Sprite* GUIVertScrollbarFactory::MakeSpriteImpl()
{
    return ENEW GUIVertScrollbar(m_renderer,
                                 m_configName,
                                 m_sliderCfgName,
                                 m_topBtnCfgName,
								 m_bottomBtnCfgName);
}