#ifndef GUI_TOUCHABLE_H
#define GUI_TOUCHABLE_H
///**********************************************************************///
///                           include begin                              ///
///**********************************************************************///
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_event_hub.h"
#include "render_system.h"
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUITouchable : public Sprite
{
	DeclareRTTI;
public:
	enum State
	{
		Normal,
		Touched,
		Selected,
		Pressed,
        Dragging,
	};
	class MouseMoveEventProc : public SpriteEventProc
	{
	public:
		GUITouchable* m_touchable;
	public:
		MouseMoveEventProc(GUITouchable* touchable) : m_touchable(touchable) {}
		~MouseMoveEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};

	class MouseButtonDownEventProc : public SpriteEventProc
	{
	public:
		GUITouchable* m_touchable;
	public:
		MouseButtonDownEventProc(GUITouchable* touchable) : m_touchable(touchable) {}
		~MouseButtonDownEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};

	class MouseButtonUpEventProc : public SpriteEventProc
	{
	public:
		GUITouchable* m_touchable;
	public:
		MouseButtonUpEventProc(GUITouchable* touchable) : m_touchable(touchable) {}
		~MouseButtonUpEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};
private:
    State m_curtState;
public:
    SInt2 m_prevMouseCoord;
public:
    GUITouchable(SpriteRenderer* renderer, const xhn::static_string name);
	virtual ~GUITouchable();
	inline State GetState() {
		return m_curtState;
	}
	inline void SetState(State s) {
        m_curtState = s;
	}
	virtual void OnMouseMove(const SpriteMouseMoveEvent* mouseEvt) = 0;
	virtual void OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt) = 0;
	virtual void OnMouseButtonUp(const SpriteMouseButtonUpEvent* mouseEvt) = 0;
	void BuildBackgroundLayer(xhn::list<SpriteElement>& to);
	void BuildTextLayer(xhn::list<SpriteElement>& to);
	SpriteTextLayer* GetTextLayer();
	template <typename T>
	void InitImpl(const xhn::static_string configName, AttributeHandle sizeHandle)
	{
		XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
		if (TestResourcePtr(cfg)) {
			pugi::xml_document& doc = cfg->GetDocument();
			pugi::xml_node root = doc.child("root");
			pugi::xml_node layers = root.child("layers");
			pugi::xml_node normallayer = layers.child("normal");
			pugi::xml_node touchedlayer = layers.child("touched");
			pugi::xml_node selectedlayer = layers.child("selected");
			pugi::xml_node pressedlayer = layers.child("pressed");
			if (normallayer)
			{
				SpriteLayerPtr layer = ENEW T("normal",
					m_pivotHandle,
					sizeHandle);
				layer->LoadConfigImpl(normallayer);
				AddChild(layer);
			}
			if (touchedlayer)
			{
				SpriteLayerPtr layer = ENEW T("touched",
					m_pivotHandle,
					sizeHandle);
				layer->LoadConfigImpl(touchedlayer);
				AddChild(layer);
			}
			if (selectedlayer)
			{
				SpriteLayerPtr layer = ENEW T("selected",
					m_pivotHandle,
					sizeHandle);
				layer->LoadConfigImpl(selectedlayer);
				AddChild(layer);
			}
            if (pressedlayer)
			{
				SpriteLayerPtr layer = ENEW T("pressed",
					m_pivotHandle,
					sizeHandle);
				layer->LoadConfigImpl(pressedlayer);
				AddChild(layer);
			}
			{
				SpriteLayerPtr layer = ENEW SpriteTextLayer("text");
				layer->m_horizontalAlignmentMode = SpriteLayer::CenterHorizontalAligned;
				layer->m_verticalAlignmentMode = SpriteLayer::CenterVerticalAligned;
				AddChild(layer);
			}
			SetState(GUITouchable::Normal);
		}
	}
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif