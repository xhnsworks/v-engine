//
//  input_robot.cpp
//  ecg
//
//  Created by 徐 海宁 on 13-5-4.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#include "input_robot.h"
#include "sfloat3.h"
ImplementRTTI(DefaultKeyboardListener2, InputListener);
ImplementRTTI(DefaultMouseListener2, InputListener);

void DefaultKeyboardListener2::ListenImpl(const input_event& event)
{
	if (event.type == KeyDownEvent)
	{
		printf("key down %d\n", event.info.key_info);
		if (event.info.key_info == 56)
		{
			m_left_alt_key_down = true;
		}
	}
	else if (event.type == KeyUpEvent)
	{
		printf("key up %d\n", event.info.key_info);
		if (event.info.key_info == 56)
		{
			m_left_alt_key_down = false;
		}
	}
}

DefaultMouseListener2::DefaultMouseListener2(DefaultKeyboardListener2* list)
: m_camUtil(NULL)
, m_keyboardListener(list)
, m_mouseX(0)
, m_mouseY(0)
, m_leftButtonDown(false)
, m_rightButtonDown(false)
, m_middleButtonDown(false)
{
}

DefaultMouseListener2::~DefaultMouseListener2()
{
}

void DefaultMouseListener2::ListenImpl(const input_event& event)
{
    if (!m_camUtil) {
        Robot* rob = RobotManager::Get()->GetRobot("RenderRobot");
        RenderRobot* renderRob = rob->DynamicCast<RenderRobot>();
        m_camUtil = CameraUtility_new(renderRob->GetMainCamera());
    }
    
	if (event.type == MouseMoveEvent)
	{
		if (m_keyboardListener->m_left_alt_key_down && m_rightButtonDown)
		{
			sfloat3 vec = SFloat3( (float)event.info.mouse_info.mouse_move_info.x,
                                  (float)event.info.mouse_info.mouse_move_info.y,
                                  0.0f );
			float dist = SFloat3_length(vec);
            
			if (event.info.mouse_info.mouse_move_info.x < 0)
				dist = -dist;
			///CameraUtility_dolly_shot(m_cameraUtil, dist);
            if (m_camUtil) {
                CameraUtility_dolly_shot(m_camUtil, dist);
            }
		}
		else if (m_keyboardListener->m_left_alt_key_down && m_middleButtonDown)
		{
			///CameraUtility_pan(m_cameraUtil, (float)event.info.mouse_info.mouse_move_info.x, (float)event.info.mouse_info.mouse_move_info.y);
            if (m_camUtil) {
                CameraUtility_pan(m_camUtil, (float)event.info.mouse_info.mouse_move_info.x, (float)event.info.mouse_info.mouse_move_info.y);
            }
		}
		else if (m_keyboardListener->m_left_alt_key_down && m_leftButtonDown)
		{
			///CameraUtility_rotate(m_cameraUtil, (float)-event.info.mouse_info.mouse_move_info.x, 0.0f);
            if (m_camUtil) {
                CameraUtility_rotate(m_camUtil, (float)-event.info.mouse_info.mouse_move_info.x, 0.0f);
            }
		}
	}
	else if (event.type == MouseAbsolutePositionEvent)
	{
		m_mouseX = event.info.mouse_info.mouse_abs_pos.x;
		m_mouseY = event.info.mouse_info.mouse_abs_pos.y + 35;
		printf("mouse x %d, y %d\n", m_mouseX, m_mouseY);
        /**
		SpriteMouseMoveEvent sptEvt;
		sptEvt.m_curtMousePos.x = m_mouseX;
		sptEvt.m_curtMousePos.y = m_mouseY;
		///m_guiRenderer->get_mouse_ray(m_mouseX, m_mouseY, &guiEvt.m_mouseRay.origin, &guiEvt.m_mouseRay.direction);
		SpriteEventHub::Get()->BroadcastPublicEvent(sptEvt, SpriteEventHub::Get()->GetAllReceivers());
         **/
	}
	else if (event.type == MouseButtonDownEvent)
	{
		if (event.info.mouse_info.mouse_button_info == LeftButton) {
			m_leftButtonDown = true;
            /**
			SpriteMouseButtonDownEvent sptEvt;
			sptEvt.m_leftButtomDown = true;
			SpriteEventHub::Get()->BroadcastPublicEvent(sptEvt, SpriteEventHub::Get()->GetAllReceivers());
             **/
		}
		else if (event.info.mouse_info.mouse_button_info == RightButton)
			m_rightButtonDown = true;
		else if (event.info.mouse_info.mouse_button_info == MiddleButton)
			m_middleButtonDown = true;
	}
	else if (event.type == MouseButtonUpEvent)
	{
		if (event.info.mouse_info.mouse_button_info == LeftButton) {
			m_leftButtonDown = false;
			m_leftButtonDown = true;
            /**
			SpriteMouseButtonUpEvent sptEvt;
			sptEvt.m_leftButtomUp = true;
			SpriteEventHub::Get()->BroadcastPublicEvent(sptEvt, SpriteEventHub::Get()->GetAllReceivers());
             **/
		}
		else if (event.info.mouse_info.mouse_button_info == RightButton)
			m_rightButtonDown = false;
		else if (event.info.mouse_info.mouse_button_info == MiddleButton)
			m_middleButtonDown = false;
	}
}


ImplementRTTI(InputAction, Action);

InputAction::InputAction(InputSystem* inputSys)
: m_inputSys(inputSys)
, m_mouseListener(NULL)
, m_keyboardListener(NULL)
{
}

InputAction::~InputAction()
{
	delete m_keyboardListener;
    delete m_mouseListener;
}
void InputAction::Init()
{
	DefaultKeyboardListener2* list = ENEW DefaultKeyboardListener2;
	m_keyboardListener = list;
	list->Init();
	m_mouseListener = ENEW DefaultMouseListener2(list);
	m_mouseListener->Init();
	m_inputSys->register_input_listener(m_keyboardListener);
	m_inputSys->register_input_listener(m_mouseListener);
}
void InputAction::DoImpl()
{
	m_inputSys->update();
    m_keyboardListener->Listen();
	m_mouseListener->Listen();
}

ImplementRTTI(InputRobot, Robot);

InputRobot::InputRobot()
{
    m_inputSys = ENEW InputSystem;
    m_inputSys->Init(NULL);
    InputAction* act = ENEW InputAction(m_inputSys);
    act->Init();
    AddAction(act);
}

InputRobot::~InputRobot()
{
    delete m_inputSys;
}

xhn::static_string InputRobot::GetName()
{
    return "InputRobot";
}