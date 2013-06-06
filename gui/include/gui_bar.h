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
#include "gui_widget.h"
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUIHoriBarLayer : public SpriteNormalLayer
{
private:
	AttributeHandle m_pivotHandle;
	AttributeHandle m_sizeHandle;
public:
	GUIHoriBarLayer(const xhn::static_string name,
                    AttributeHandle pivotHandle,
                    AttributeHandle sizeHandle)
    : m_pivotHandle(pivotHandle)
    , m_sizeHandle(sizeHandle)
    , SpriteNormalLayer(name)
	{}
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void GetScopeImpl(SpriteRect& result);
};
class GUIHoriBar : public GUIWidget
{
public:
	AttributeHandle m_sizeHandle;
public:
	GUIHoriBar(SpriteRenderer* renderer, const xhn::static_string name);
    GUIHoriBar(SpriteRenderer* renderer,
               const xhn::static_string name,
               AttributeHandle sizeHandle);
	virtual void Init(const xhn::static_string configName);
	void SetSize(float x);
	///virtual void GetScopeImpl(SpriteRect& result);
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
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
	static void CreateSheetConfig(const char* cfgName,
                                  const char* sheetName,
                                  const char* textureName,
                                  const SpriteRect& panelRect,
                                  float cornerSize,
                                  const SpriteRect& areaRect,
                                  float areaCornerSize);
	static void CreateSheetConfig(const char* cfgName,
		                          const char* textureName,
		                          const SpriteRect& panelRect,
		                          float cornerSize,
		                          const EFloat2& areaSize,
		                          float areaCornerSize,
		                          const EFloat2& areaCoordNormal,
		                          const EFloat2& areaCoordTouched,
		                          const EFloat2& areaCoorfSelected);
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUIVertBarLayer : public SpriteNormalLayer
{
private:
	AttributeHandle m_pivotHandle;
	AttributeHandle m_sizeHandle;
public:
	GUIVertBarLayer(const xhn::static_string name,
                    AttributeHandle pivotHandle,
                    AttributeHandle sizeHandle)
		: m_pivotHandle(pivotHandle)
		, m_sizeHandle(sizeHandle)
		, SpriteNormalLayer(name)
	{}
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void GetScopeImpl(SpriteRect& result);
};
class GUIVertBar : public GUIWidget
{
public:
	AttributeHandle m_sizeHandle;
public:
	GUIVertBar(SpriteRenderer* renderer, const xhn::static_string name);
	virtual void Init(const xhn::static_string configName);
	void SetSize(float x);
	///virtual void GetScopeImpl(SpriteRect& result);
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
};

class GUIVertBarFactory : public SpriteFactory
{
public:
	int m_vertBarCount;
public:
	GUIVertBarFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_vertBarCount(0)
		, SpriteFactory(renderer, cfgName)
	{}
	///virtual Sprite* MakeSpriteImpl();
	static void CreateSheetConfig(const char* cfgName,
                                  const char* sheetName,
                                  const char* textureName,
		                          const SpriteRect& panelRect,
                                  float cornerSize,
                                  const SpriteRect& areaRect,
                                  float areaCornerSize);
	static void CreateSheetConfig(const char* cfgName,
		                          const char* textureName,
		                          const SpriteRect& panelRect,
		                          float cornerSize,
		                          const EFloat2& areaSize,
		                          float areaCornerSize,
		                          const EFloat2& areaCoordNormal,
		                          const EFloat2& areaCoordTouched,
		                          const EFloat2& areaCoorfSelected);
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
void CreateHoriLayer(pugi::xml_node sheet,
					 const char* textureName,
					 const SpriteRect& panelRect,
					 float cornerSize,
					 const EFloat2& areaSize,
					 float areaCornerSize,
					 const EFloat2& areaCoord);
void CreateVertLayer(pugi::xml_node sheet,
					 const char* textureName,
					 const SpriteRect& panelRect,
					 float cornerSize,
					 const EFloat2& areaSize,
					 float areaCornerSize,
					 const EFloat2& areaCoord);
#endif /* defined(__ecg__gui_bar__) */
