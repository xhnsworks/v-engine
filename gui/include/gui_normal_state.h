#ifndef GUI_NORMAL_STATE_H
#define GUI_NORMAL_STATE_H
#include "gui_state.h"
class GUINormalState
{
public:
	virtual xhn::static_string GetName() {
		return "normal";
	}
	virtual void OnMouseMove(const SpriteMouseMoveEvent* mouseEvt) {}
	virtual void OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt) {}
	virtual void OnMouseButtonUp(const SpriteMouseButtonUpEvent* mouseEvt) {}
};
#endif