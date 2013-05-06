//
//  input_robot.h
//  ecg
//
//  Created by 徐 海宁 on 13-5-4.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#ifndef __ecg__input_robot__
#define __ecg__input_robot__

////*************************************************************************************************************************///
///                                                     include begin                                                       ///
///*************************************************************************************************************************///
#include "common.h"
#include "etypes.h"
#include "xhn_map.hpp"
#include "pugixml.hpp"
#include "eassert.h"
#include "robot.h"
#include "float_base.h"
#include "timer.h"
#include "rwbuffer.h"
#include "render_robot.h"
#include "ecg2pugi.h"
#include "xml_resource.h"
#include "attribute.h"
#include "camera_utility.h"
///*************************************************************************************************************************///
///                                                     include end                                                         ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class DefaultKeyboardListener2 : public InputListener
{
	DeclareRTTI;
public:
	bool m_left_alt_key_down;
	DefaultKeyboardListener2()
    : m_left_alt_key_down(false)
	{}
	virtual void ListenImpl(const input_event& event);
};

class DefaultMouseListener2 : public InputListener
{
	DeclareRTTI;
public:
    CameraUtility m_camUtil;
    SpriteRenderer* m_guiRenderer;
	DefaultKeyboardListener2* m_keyboardListener;
	int m_mouseX;
	int m_mouseY;
	bool m_leftButtonDown;
	bool m_rightButtonDown;
	bool m_middleButtonDown;
	DefaultMouseListener2(DefaultKeyboardListener2* list);
	~DefaultMouseListener2();
	virtual void ListenImpl(const input_event& event);
};

class InputAction : public Action
{
    DeclareRTTI;
public:
	InputSystem* m_inputSys;
	InputListener* m_mouseListener;
	InputListener* m_keyboardListener;
public:
	InputAction(InputSystem* inputSys);
	~InputAction();
	void Init();
	virtual void DoImpl();
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
class InputRobot : public Robot
{
    DeclareRTTI;
public:
    InputSystem* m_inputSys;
public:
#if defined(_WIN32) || defined(_WIN64)
	InputRobot(HWND hwnd);
#elif defined(__APPLE__)
    InputRobot(vptr view);
#endif
    ~InputRobot();
    virtual xhn::static_string GetName();
};
///*************************************************************************************************************************///
///                                                  class define end                                                       ///
///*************************************************************************************************************************///
#endif /* defined(__ecg__input_robot__) *virtual void DoImpl() */
