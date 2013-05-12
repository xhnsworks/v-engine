#ifndef GUI_PANEL_H
#define GUI_PANEL_H
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUIPanelLayer : public SpriteNormalLayer
{
	DeclareRTTI;
private:
	AttributeHandle m_pivotHandle;
	AttributeHandle m_sizeHandle;
public:
	GUIPanelLayer(const xhn::static_string name,
                  AttributeHandle pivotHandle,
                  AttributeHandle sizeHandle)
		: m_pivotHandle(pivotHandle)
	    , m_sizeHandle(sizeHandle)
		, SpriteNormalLayer(name)
	{}
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void GetScopeImpl(SpriteRect& result);
};
class GUIPanel : public Sprite
{
	DeclareRTTI;
	friend class GUIPanelFactory;
protected:
	~GUIPanel() {}
public:
	AttributeHandle m_sizeHandle;
public:
	GUIPanel(SpriteRenderer* renderer, const xhn::static_string name);
	virtual void Init(const xhn::static_string configName);
	void SetSize(float x, float y);
	virtual void GetScopeImpl(SpriteRect& result);
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
};

class GUIPanelFactory : public SpriteFactory
{
public:
	int m_panelCount;
public:
	GUIPanelFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_panelCount(0)
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
#endif