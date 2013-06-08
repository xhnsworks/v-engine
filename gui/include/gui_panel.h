#ifndef GUI_PANEL_H
#define GUI_PANEL_H
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_widget.h"
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUIPanelLayer : public SpriteNormalLayer
{
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
class GUIPanel : public GUIWidget
{
public:
	AttributeHandle m_sizeHandle;
public:
	GUIPanel(SpriteRenderer* renderer,
             const xhn::static_string name);
    GUIPanel(SpriteRenderer* renderer,
             const xhn::static_string name,
             AttributeHandle sizeHandle);
	virtual void Init(const xhn::static_string configName);
	void SetSize(float x, float y);
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
	inline AttributeHandle GetSizeHandle() {
		return m_sizeHandle;
	}
};

template <typename PANEL_TYPE>
class GUIPanelFactory : public SpriteFactory<PANEL_TYPE>
{
public:
	int m_panelCount;
public:
	GUIPanelFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_panelCount(0)
		, SpriteFactory(renderer, cfgName)
	{}
};
API_EXPORT void GUIPanelFactory_CreateSheetConfig(const char* cfgName,
												  const char* sheetName,
												  const char* textureName,
												  const SpriteRect& panelRect,
												  const SpriteSize& cornerSize,
												  const SpriteRect& areaRect,
												  const SpriteSize& areaCornerSize);
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif