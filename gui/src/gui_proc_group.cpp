#include "gui_pch.h"
#include "gui_proc_group.h"

void TouchableMouseMoveProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseMoveEvent* mouseEvt = evt->DynamicCast<SpriteMouseMoveEvent>();
	if (mouseEvt) {
		const FourBorders& borders = m_widget->GetFourBorders();
		EFloat2 realCrd =
			m_widget->m_renderer->get_real_position((float)mouseEvt->m_curtMouseCoord.x,
			(float)mouseEvt->m_curtMouseCoord.y);
		EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
		sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);

		if (borders.IsInBorders(pt)) {
			m_widget->SetState(GUIWidget::Touched);
		}
		else {
			m_widget->SetState(GUIWidget::Normal);
		}
	}
}

void PressableMouseMoveProc::Proc(const SpriteEvent* evt)
{
	if (m_widget->GetState() == GUIWidget::Pressed) {
		return;
	}
	else {
		const SpriteMouseMoveEvent* mouseEvt = evt->DynamicCast<SpriteMouseMoveEvent>();
		if (mouseEvt) {
			const FourBorders& borders = m_widget->GetFourBorders();
			EFloat2 realCrd =
				m_widget->m_renderer->get_real_position((float)mouseEvt->m_curtMouseCoord.x,
				(float)mouseEvt->m_curtMouseCoord.y);
			EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
			sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);

			if (borders.IsInBorders(pt)) {
				m_widget->SetState(GUIWidget::Touched);
			}
			else {
				m_widget->SetState(GUIWidget::Normal);
			}
		}
	}
}

void LeaveableMouseMoveProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseMoveEvent* mouseEvt = evt->DynamicCast<SpriteMouseMoveEvent>();
	if (mouseEvt) {
		const FourBorders& borders = m_widget->GetFourBorders();
		EFloat2 realCrd =
			m_widget->m_renderer->get_real_position((float)mouseEvt->m_curtMouseCoord.x,
			(float)mouseEvt->m_curtMouseCoord.y);
		EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
		sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);

		if (borders.IsInBorders(pt)) {
			m_widget->SetState(GUIWidget::Touched);
		}
		else {
			m_widget->SetState(GUIWidget::Normal);
			m_widget->OnLeave();
		}
	}
}

void PressableMouseButtonDownProc::Proc(const SpriteEvent* evt)
{
	if (m_widget->GetState() != GUIWidget::Touched)
		return;
	const SpriteMouseButtonDownEvent* mouseEvt = evt->DynamicCast<SpriteMouseButtonDownEvent>();
	if (mouseEvt && mouseEvt->m_leftButtomDown) {
		m_widget->SetState(GUIWidget::Pressed);
		m_widget->OnPress();
	}
}

void DraggableMouseMoveProc::Proc(const SpriteEvent* evt)
{
	if (m_widget->GetState() == GUIWidget::Dragging) {
		const SpriteMouseMoveEvent* mouseEvt = evt->DynamicCast<SpriteMouseMoveEvent>();
		if (mouseEvt) {
			float dispX = (float)(mouseEvt->m_curtMouseCoord.x - m_prevMouseCoord.x);
			float dispY = (float)(mouseEvt->m_curtMouseCoord.y - m_prevMouseCoord.y);
			m_widget->OnMove(dispX, dispY);
		}
	}
	else {
        TouchableMouseMoveProc::Proc(evt);
	}
}
void DraggableMouseButtonDownProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseButtonDownEvent* mouseEvt = evt->DynamicCast<SpriteMouseButtonDownEvent>();
	if (mouseEvt && mouseEvt->m_leftButtomDown) {
		if (m_widget->GetState() == GUIWidget::Normal || m_widget->GetState() == GUIWidget::Touched) {
			const FourBorders& borders = m_widget->GetFourBorders();
			EFloat2 realCrd =
				m_widget->m_renderer->get_real_position((float)mouseEvt->m_curtMouseCoord.x,
				(float)mouseEvt->m_curtMouseCoord.y);
			EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
			sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);

			if (borders.IsInBorders(pt)) {
				m_mouseMoveProc->m_prevMouseCoord = mouseEvt->m_curtMouseCoord;
				m_widget->SetState(GUIWidget::Dragging);
			}
			else {
                m_widget->SetState(GUIWidget::Normal);
			}
		}
	}
}