#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#include "common.h"
#include "etypes.h"
#include "emem.hpp"
#include "rtti.hpp"
///typedef void (*input_listener)(const input_event event);
class API_EXPORT InputListener : public MemObject
{
	DeclareRootRTTI;
public:
	RWBuffer m_input_buffer;
	input_event* m_read_buffer;
	InterRWBuffer m_inter_input_buffer;
public:
	InputListener();
	~InputListener();
	void Init();
	virtual void ListenImpl(const input_event& event) = 0;
	void Listen();
};

class API_EXPORT InputSystem : public MemObject
{
public:
	Tree m_input_listen_tree;

    InputSystem();
	void Init(vptr platform_param);
	void register_input_listener(InputListener* list);
	void update();
};
/**
API_EXPORT void InputSystem_Init(vptr platform_param);
API_EXPORT void InputSystem_register_input_listener(InputListener* list);
API_EXPORT void InputSystem_update();
**/
#endif
