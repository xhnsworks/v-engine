//
//  gui_list.h
//  ecg
//
//  Created by 徐 海宁 on 13-6-3.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#ifndef __ecg__gui_list__
#define __ecg__gui_list__
///**********************************************************************///
///                           include begin                              ///
///**********************************************************************///
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_bar.h"
#include "gui_container.h"
#include "gui_panel.h"
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUIList;

class GUIListEntry : public GUIHoriBar
{
public:
    GUIListEntry(SpriteRenderer* renderer,
                 const xhn::static_string name,
                 AttributeHandle sizeHandle)
    : GUIHoriBar(renderer, name, sizeHandle)
    {}
    virtual ~GUIListEntry();
    virtual void Init(const xhn::static_string configName);
    virtual void Build();
    virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void BuildDropDownMenu(xhn::list<SpriteElement>& to) {}
	void SetText(const xhn::string& text);
	xhn::string GetText();
	void GetBackgroundRect(SpriteRect& rect);
	virtual void OnPress();
	virtual ProcGroup NewProcGroup();
};

class GUIListEntryFactory : public GUIHoriBarFactory<GUIListEntry>
{
public:
    AttributeHandle m_sizeHandle;
public:
    GUIListEntryFactory(SpriteRenderer* renderer,
                        const char* cfgName,
                        AttributeHandle sizeHandle)
    : m_sizeHandle(sizeHandle)
    , GUIHoriBarFactory(renderer, cfgName)
    {}
	GUIListEntryFactory(SpriteRenderer* renderer,
                        const char* cfgName);
    virtual Sprite* MakeSpriteImpl();
	static void CreateSheetConfig(const char* cfgName,
		                               const char* textureName,
		                               const SpriteRect& panelRect,
		                               float cornerSize,
		                               const EFloat2& areaSize,
		                               float areaCornerSize,
		                               const EFloat2& areaCoordNormal,
		                               const EFloat2& areaCoordTouched,
		                               const EFloat2& areaCoorfSelected)
	{
		GUIHoriBarFactory_CreateSheetConfig(cfgName,
			                                textureName,
											panelRect,
											cornerSize,
											areaSize,
											areaCornerSize, 
											areaCoordNormal,
											areaCoordTouched,
											areaCoorfSelected);
	}
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUIList : public GUIPanel
{
public:
	int m_entryCount;
    GUIListEntryFactory* m_entryFactory;
public:
	GUIList(SpriteRenderer* renderer,
            GUIListEntryFactory* entryFactory,
            AttributeHandle sizeHandle);
	void AddEntry(const xhn::string& str);
	void RemoveAllEntries();
    void RemoveBackground();
	void GetBackgroundRect(SpriteRect& rect);
	virtual void BuildFourBorders();
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void Build();
	virtual ProcGroup NewProcGroup();
};

class GUIListFactory : public GUIPanelFactory<GUIList>
{
public:
    AttributeHandle m_listSizeHandle;
	GUIListEntryFactory* m_entryFactory;
public:
	GUIListFactory(SpriteRenderer* renderer,
                   const char* cfgName,
                   GUIListEntryFactory* entryFactory,
                   AttributeHandle listSizeHandle);
	virtual Sprite* MakeSpriteImpl();
	static void CreateAnimationConfig(const char* cfgName,
		                              const char* showAnimName,
									  const char* hideAnimName,
                                      float width,
									  float maxHeight);
	inline AttributeHandle GetListSizeHandle() {
		return m_listSizeHandle;
	}
	static void CreateSheetConfig(const char* cfgName,
		                          const char* sheetName,
		                          const char* textureName,
		                          const SpriteRect& panelRect,
		                          const SpriteSize& cornerSize,
		                          const SpriteRect& areaRect,
		                          const SpriteSize& areaCornerSize)
	{
		GUIPanelFactory_CreateSheetConfig(cfgName,
			                              sheetName,
										  textureName,
										  panelRect,
										  cornerSize,
										  areaRect, 
										  areaCornerSize);
	}
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif /* defined(__ecg__gui_list__) */
