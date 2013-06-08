//
//  gui_simple_panel.h
//  ecg
//
//  Created by 徐 海宁 on 13-5-19.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#ifndef __ecg__gui_simple_panel__
#define __ecg__gui_simple_panel__

#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_widget.h"
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUISimplePanelLayer : public SpriteNormalLayer
{
private:
	AttributeHandle m_pivotHandle;
public:
	GUISimplePanelLayer(const xhn::static_string name,
                        AttributeHandle pivotHandle)
    : m_pivotHandle(pivotHandle)
    , SpriteNormalLayer(name)
	{}
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void GetScopeImpl(SpriteRect& result);
};
class GUISimplePanel : public GUIWidget
{
public:
	GUISimplePanel(SpriteRenderer* renderer, const xhn::static_string name);
	virtual void Init(const xhn::static_string configName);
	///virtual void GetScopeImpl(SpriteRect& result);
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
	virtual ProcGroup NewProcGroup();
};

template <typename SIMPLE_PANEL_TYPE>
class GUISimplePanelFactory : public SpriteFactory<SIMPLE_PANEL_TYPE>
{
public:
	int m_simplePanelCount;
public:
	GUISimplePanelFactory(SpriteRenderer* renderer, const char* cfgName)
    : m_simplePanelCount(0)
    , SpriteFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl()
	{
		char mbuf[256];
		snprintf(mbuf, 255, "GUISimplePanel_%d", m_simplePanelCount);
		m_simplePanelCount++;
		GUISimplePanel* ret = ENEW GUISimplePanel(m_renderer, mbuf);
		ret->Init(m_configName);
		return ret;
	}
	static void CreateSheetConfig(const char* cfgName,
                                  const char* sheetName,
                                  const char* textureName,
		                          const SpriteRect& panelRect,
                                  const SpriteRect& areaRect)
	{
		GUISimplePanelFactory_CreateSheetConfig(cfgName,
			                                    sheetName,
												textureName,
												panelRect,
												areaRect);
	}
};
API_EXPORT void GUISimplePanelFactory_CreateSheetConfig(const char* cfgName,
														const char* sheetName,
														const char* textureName,
														const SpriteRect& panelRect,
														const SpriteRect& areaRect);
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///

#endif /* defined(__ecg__gui_simple_panel__) */
