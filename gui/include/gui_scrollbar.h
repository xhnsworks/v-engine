#ifndef GUI_SCROLLBAR_H
#define GUI_SCROLLBAR_H
///**********************************************************************///
///                           include begin                              ///
///**********************************************************************///
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_bar.h"
#include "gui_simple_panel.h"
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUIVertSlider : public GUIVertBar
{
public:
	GUIVertSlider(SpriteRenderer* renderer, const xhn::static_string name)
		: GUIVertBar(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual ProcGroup NewProcGroup();
};

class GUIVertSliderFactory : public GUIVertBarFactory
{
public:
	AttributeHandle m_sizeHandle;
public:
	GUIVertSliderFactory(SpriteRenderer* renderer,
		                 const char* cfgName,
		                 AttributeHandle sizeHandle)
		: m_sizeHandle(sizeHandle)
		, GUIVertBarFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUIVertButton : public GUISimplePanel
{
public:
	GUIVertButton(SpriteRenderer* renderer, const xhn::static_string name)
		: GUISimplePanel(renderer, name)
	{}
	virtual ProcGroup NewProcGroup();
};

class GUIVertButtonFactory : public GUISimplePanelFactory
{
public:
	GUIVertButtonFactory(SpriteRenderer* renderer,
		const char* cfgName)
		: GUISimplePanelFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUIVertScrollbar : public GUIVertBar
{
public:
	AttributeHandle m_sliderSizeHandle;
    xhn::static_string m_sliderCfgName;
    xhn::static_string m_buttonCfgName;
	GUIVertSliderFactory* m_vertSliderFactory;
	GUIVertButtonFactory* m_vertButtonFactory;
public:
	GUIVertScrollbar(SpriteRenderer* renderer,
                     const xhn::static_string name,
                     const xhn::static_string sliderCfgName,
                     const xhn::static_string buttonCfgName);
	virtual void Init(const xhn::static_string configName);
	virtual ProcGroup NewProcGroup();
};

class GUIVertScrollbarFactory : public GUIVertBarFactory
{
public:
    xhn::static_string m_sliderCfgName;
    xhn::static_string m_buttonCfgName;
public:
	GUIVertScrollbarFactory(SpriteRenderer* renderer,
                            const char* sliderCfgName,
                            const char* buttonCfgName,
                            const char* scroolbarCfgName);
	virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif