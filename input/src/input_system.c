#include "input_system.h"
#include "rwbuffer.h"
#include "tree.h"
#include "mem.h"
#include <pthread.h>
#include "input_win32.h"
static RWBuffer g_input_buffer = {NULL};
static input_event* g_read_buffer = NULL;
static InterRWBuffer g_inter_input_buffer = {RWBuffer_Read, RWBuffer_Write};
static Tree g_input_listen_tree = NULL;

void InputSystem_Init(vptr platform_param)
{
    g_input_buffer = RWBuffer_new(1024 * sizeof(input_event));
    g_read_buffer = Malloc(1024 * sizeof(input_event) * 2);
    input_Init((HWND)platform_param);
    g_input_listen_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
}

void InputSystem_register_input_listen(input_listen listen)
{
    var key, data;
    key.vptr_var = listen;
    data.vptr_var = listen;
    Tree_insert(g_input_listen_tree, key, data);
}

void InputSystem_update()
{
    input_proc_param param = {g_input_buffer, g_inter_input_buffer};
    input_proc(&param);
    Iterator iter = Tree_begin(g_input_listen_tree);

    uint size = 0;
    if (RWBuffer_Read(g_input_buffer, g_read_buffer, &size))
    {
        uint event_count = size / sizeof(input_event);
        while (iter)
        {
            var data = Tree_get_value(iter);
            input_listen listen = (input_listen)data.vptr_var;

            input_event event;
            for (uint i = 0; i < event_count; i++)
            {
                event = g_read_buffer[i];
                listen(event);
            }

            iter = Tree_next(iter);
        }
    }
}
