#include "input_system.h"
#include "rwbuffer.h"
#include "tree.h"
#include "emem.h"
#include <pthread.h>
#include "input_win32.h"
#include "xhn_vector.hpp"
/**
static RWBuffer g_input_buffer = {NULL};
static input_event* g_read_buffer = NULL;
static InterRWBuffer g_inter_input_buffer(RWBuffer_Read, RWBuffer_Write);
static Tree g_input_listen_tree = NULL;
**/
ImplementRootRTTI(InputListener);
InputListener::InputListener()
: m_input_buffer(NULL)
, m_read_buffer(NULL)
, m_inter_input_buffer(RWBuffer_Read, RWBuffer_Write)
{
}
InputListener::~InputListener()
{
}
void InputListener::Init()
{
	m_input_buffer = RWBuffer_new(1024 * sizeof(input_event));
	m_read_buffer = (input_event*)SMalloc(1024 * sizeof(input_event) * 2);
}
void InputListener::Listen()
{
	uint size = 0;
    while (RWBuffer_Read(m_input_buffer, (uint*)m_read_buffer, &size))
	{
        uint count = size / sizeof(input_event);
		for (uint i = 0; i < count; i++)
		{
			ListenImpl(m_read_buffer[i]);
		}
	}
}

InputSystem::InputSystem()
: m_input_listen_tree(NULL)
{}

void InputSystem::Init(vptr platform_param)
{
    input_Init((HWND)platform_param);
    m_input_listen_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
}

void InputSystem::register_input_listener(InputListener* list)
{
    var key, data;
    key.vptr_var = (vptr)list;
    data.vptr_var = (vptr)list;
    Tree_insert(m_input_listen_tree, key, data);
}

void InputSystem::update()
{
	xhn::vector<input_buffer> buffer;
    Iterator iter = Tree_begin(m_input_listen_tree);
	while (iter)
	{
		var data = Tree_get_value(iter);
		InputListener* list = (InputListener*)data.vptr_var;

		input_buffer buf = {list->m_input_buffer, list->m_inter_input_buffer};
		buffer.push_back(buf);

		iter = Tree_next(iter);
	}
	input_Proc(buffer);
}
