//
//  gui_bar.h
//  ecg
//
//  Created by 徐 海宁 on 13-5-9.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#ifndef __ecg__gui_bar__
#define __ecg__gui_bar__

#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"

///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class SpriteRenderer;
class GUIHoriBarLayer : public SpriteNormalLayer
{
	DeclareRTTI;
private:
	AttributeHandle m_pivotHandle;
	AttributeHandle m_sizeHandle;
public:
	GUIHoriBarLayer(const xhn::static_string name, AttributeHandle pivotHandle, AttributeHandle sizeHandle)
    : m_pivotHandle(pivotHandle)
    , m_sizeHandle(sizeHandle)
    , SpriteNormalLayer(name)
	{}
	virtual void BuildElements(xhn::list<SpriteElement>& to);
	virtual void GetScopeImpl(SpriteRect& result);
};
class GUIHoriBar : public Sprite
{
	DeclareRTTI;
	friend class GUIBarFactory;
protected:
	~GUIHoriBar() {}
public:
	AttributeHandle m_sizeHandle;
public:
	GUIHoriBar(SpriteRenderer* renderer, const xhn::static_string name);
	virtual void Init(const xhn::static_string configName);
	void SetSize(float x);
	virtual void GetScopeImpl(SpriteRect& result);
};

class GUIHoriBarFactory : public SpriteFactory
{
public:
	int m_horiBarCount;
public:
	GUIHoriBarFactory(SpriteRenderer* renderer, const char* cfgName)
    : m_horiBarCount(0)
    , SpriteFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
	static void CreateSheetConfig(const char* cfgName, const char* sheetName, const char* textureName,
                                  const SpriteRect& panelRect, float cornerSize, const SpriteRect& areaRect, float areaCornerSize);
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
#endif /* defined(__ecg__gui_bar__) */
