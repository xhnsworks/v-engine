#ifndef GUI_PANEL_H
#define GUI_PANEL_H
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class SpriteRenderer;
class GUIPanelLayer : public SpriteNormalLayer
{
	DeclareRTTI;
private:
	AttributeHandle m_pivotHandle;
	AttributeHandle m_sizeHandle;
public:
	GUIPanelLayer(AttributeHandle pivotHandle, AttributeHandle sizeHandle)
		: m_pivotHandle(pivotHandle)
	    , m_sizeHandle(sizeHandle)
		, SpriteNormalLayer("base")
	{}
	///virtual void ApplyTransform(const matrix4x4* trans);
	virtual void BuildElements(xhn::list<SpriteElement>& to);
};
class GUIPanel : public Sprite
{
	DeclareRTTI;
public:
    EFloat2 m_size;
	AttributeHandle m_sizeHandle;
public:
	GUIPanel(SpriteRenderer* renderer, const xhn::static_string name);
	virtual void Init(const xhn::static_string configName);
	void SetSize(float x, float y);
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
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
#endif