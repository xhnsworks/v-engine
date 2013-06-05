#include "gui_pch.h"
#include "gui_scrollbar.h"
#include "render_system.h"
ImplementRTTI(GUIVertSlider, GUIVertBar);
ImplementRTTI(GUIVertButton, GUISimplePanel);
ImplementRTTI(GUIVertScrollbar, GUIVertBar);

void GUIVertSlider::Init(const xhn::static_string configName)
{
    InitImpl<GUIHoriBarLayer>(configName, m_sizeHandle);
}
void GUIVertSlider::OnMouseMove(const SpriteMouseMoveEvent* mouseEvt)
{
    if (GetState() == GUITouchable::Normal) {
        const FourBorders& borders = GetFourBorders();
        EFloat2 realCrd =
        m_renderer->get_real_position((float)mouseEvt->m_curtMouseCoord.x,
                                      (float)mouseEvt->m_curtMouseCoord.y);
        EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
        sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);
        
        if (borders.IsInBorders(pt)) {
            SetState(GUITouchable::Touched);
        }
    }
    else if (GetState() == GUITouchable::Dragging) {
        esint move = mouseEvt->m_curtMouseCoord.y - m_prevMouseCoord.y;
        Float2Attr coord;
        m_coordinateHandle.GetAttribute(&coord);
        coord.y += (float)move;
        m_coordinateHandle.SetAttribute(&coord);
        m_prevMouseCoord = mouseEvt->m_curtMouseCoord;
    }
}
void GUIVertSlider::OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt)
{
    if (mouseEvt->m_leftButtomDown) {
        if (GetState() == GUITouchable::Touched) {
            SetState(GUITouchable::Dragging);
            m_prevMouseCoord = mouseEvt->m_curtMouseCoord;
        }
    }
}
void GUIVertSlider::OnMouseButtonUp(const SpriteMouseButtonUpEvent* mouseEvt)
{
    if (mouseEvt->m_leftButtomUp) {
        if (GetState() == GUITouchable::Dragging) {
            SetState(GUITouchable::Normal);
        }
    }
}
void GUIVertSlider::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
	BuildTextLayer(to);
	BuildBackgroundLayer(to);

	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);

	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);

	m_fourBorders.ApplyTranform(&mat);
}