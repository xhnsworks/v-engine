#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H
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
struct ProcGroup
{
	SpriteEventProcPtr mouseMoveProc;
	SpriteEventProcPtr mouseButtonDownProc;
	SpriteEventProcPtr mouseButtonUpProc;
};
class GUIWidget : public Sprite
{
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
		GUIWidget* m_widget;
	public:
		MouseMoveEventProc(GUIWidget* widget) : m_widget(widget) {}
		~MouseMoveEventProc() {}
	};

	class MouseButtonDownEventProc : public SpriteEventProc
	{
	public:
		GUIWidget* m_widget;
	public:
		MouseButtonDownEventProc(GUIWidget* widget) : m_widget(widget) {}
		~MouseButtonDownEventProc() {}
	};

	class MouseButtonUpEventProc : public SpriteEventProc
	{
	public:
		GUIWidget* m_widget;
	public:
		MouseButtonUpEventProc(GUIWidget* widget) : m_widget(widget) {}
		~MouseButtonUpEventProc() {}
	};
private:
	State m_curtState;
public:
	SInt2 m_prevMouseCoord;
public:
	GUIWidget(SpriteRenderer* renderer, const xhn::static_string name);
	virtual ~GUIWidget();
	virtual void Init();
	inline State GetState() {
		return m_curtState;
	}
	inline void SetState(State s) {
		m_curtState = s;
	}
	virtual void OnMove(float x, float y) {}
	virtual void OnPress() {}
	virtual void OnLeave() {}
	virtual ProcGroup NewProcGroup() = 0;
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
			SetState(GUIWidget::Normal);
		}
	}
private:
	virtual void Build();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif