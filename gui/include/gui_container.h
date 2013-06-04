#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_simple_panel.h"
#include "gui_touchable.h"
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUIContainer : public GUITouchable
{
	DeclareRTTI;
public:
	AttributeHandle m_rectHandle;
	SpriteLayerPtr m_simplePanel;
    GUISimplePanelFactory* m_simplePanelFactory;
public:
	GUIContainer(SpriteRenderer* renderer, const xhn::static_string name);
	virtual ~GUIContainer();
	virtual void Init(const xhn::static_string configName);
	void SetRect(float left, float top, float width, float height);
	virtual void GetScopeImpl(SpriteRect& result);
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
	virtual void Build();
	virtual void OnMouseMove(const SpriteMouseMoveEvent* mouseEvt) {}
	virtual void OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt) {}
	virtual void OnMouseButtonUp(const SpriteMouseButtonUpEvent* mouseEvt) {}
};

class GUIContainerFactory : public SpriteFactory
{
public:
	int m_containerCount;
public:
	GUIContainerFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_containerCount(0)
		, SpriteFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
	static void CreateSheetConfig(const char* cfgName,
                                  const char* sheetName,
                                  const SpriteRect& panelRect,
								  bool isDebugMode);
};
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
#endif