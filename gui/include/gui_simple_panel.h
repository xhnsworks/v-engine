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
	AttributeHandle m_sizeHandle;
public:
	GUISimplePanelLayer(const xhn::static_string name,
                        AttributeHandle pivotHandle,
                        AttributeHandle sizeHandle)
    : m_pivotHandle(pivotHandle)
    , m_sizeHandle(sizeHandle)
    , SpriteNormalLayer(name)
	{}
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void GetScopeImpl(SpriteRect& result);
};
class GUISimplePanel : public Sprite
{
	DeclareRTTI;
	friend class GUIPanelFactory;
protected:
	~GUISimplePanel() {}
public:
	AttributeHandle m_sizeHandle;
public:
	GUISimplePanel(SpriteRenderer* renderer, const xhn::static_string name);
	virtual void Init(const xhn::static_string configName);
	void SetSize(float x, float y);
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
                                  const SpriteSize& cornerSize,
                                  const SpriteRect& areaRect,
                                  const SpriteSize& areaCornerSize);
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///

#endif /* defined(__ecg__gui_simple_panel__) */
