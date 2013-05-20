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
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUISimplePanelLayer : public SpriteNormalLayer
{
	DeclareRTTI;
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
class GUISimplePanel : public Sprite
{
	DeclareRTTI;
	friend class GUISimplePanelFactory;
protected:
	~GUISimplePanel() {}
	GUISimplePanel(SpriteRenderer* renderer, const xhn::static_string name);
public:
	virtual void Init(const xhn::static_string configName);
	virtual void GetScopeImpl(SpriteRect& result);
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
};

class GUISimplePanelFactory : public SpriteFactory
{
public:
	int m_simplePanelCount;
public:
	GUISimplePanelFactory(SpriteRenderer* renderer, const char* cfgName)
    : m_simplePanelCount(0)
    , SpriteFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
	static void CreateSheetConfig(const char* cfgName,
                                  const char* sheetName,
                                  const char* textureName,
		                          const SpriteRect& panelRect,
                                  const SpriteRect& areaRect);
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///

#endif /* defined(__ecg__gui_simple_panel__) */
