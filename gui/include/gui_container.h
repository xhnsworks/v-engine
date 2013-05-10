#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class SpriteRenderer;
class GUIContantner : public Sprite
{
	DeclareRTTI;
	friend class GUIContantnerFactory;
protected:
	~GUIContantner() {}
public:
	AttributeHandle m_rectHandle;
public:
	GUIContantner(SpriteRenderer* renderer, const xhn::static_string name);
	virtual void Init(const xhn::static_string configName);
	void SetRect(float left, float top, float width, float height);
	virtual void GetScopeImpl(SpriteRect& result);
};

class GUIContantnerFactory : public SpriteFactory
{
public:
	int m_contantnerCount;
public:
	GUIContantnerFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_contantnerCount(0)
		, SpriteFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
	static void CreateSheetConfig(const char* cfgName, const char* sheetName, const SpriteRect& panelRect);
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
#endif