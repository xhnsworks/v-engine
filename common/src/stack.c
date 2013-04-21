#include "stack.h"
#include "mem.h"
#include "container.h"

Stack Stack_Init(Stack _stk, etype _value_type)
{
    _stk->stack = Malloc(sizeof(var) * STACK_INITIAL_LENGTH);
    _stk->stack_length = STACK_INITIAL_LENGTH;
    _stk->stack_top_ptr = 0;
    _stk->value_type = _value_type;
    return _stk;
}
Stack Stack_new(etype _value_type)
{
    Stack ret = Malloc(sizeof(stack));
    Stack_Init(ret, _value_type);
    return ret;
}
void Stack_Dest(Stack _stk)
{
    Mfree((vptr)_stk->stack);
}
void _stack_realloc(Stack _stk)
{
    uint _pre_stack_length = _stk->stack_length;
    _stk->stack_length *= 2;
    {
        var* _buf = Malloc(sizeof(var) * _stk->stack_length);
        uint i = 0;
        for (; i < _pre_stack_length; i++)
        {
            _buf[i] = _stk->stack[i];
        }
        Mfree(_stk->stack);
        _stk->stack = _buf;
    }
}
void Stack_push(Stack _stk, var _data)
{
    _stk->stack[_stk->stack_top_ptr] = _data;
    _stk->stack_top_ptr++;
    if (unlikely(_stk->stack_top_ptr == _stk->stack_length))
    {
        _stack_realloc(_stk);
    }
}
bool Stack_pop(Stack _stk, var* _to)
{
    if (Stack_get_top(_stk, _to))
    {
        _stk->stack_top_ptr--;
        return true;
    }
    else
    {
        return false;
    }
}
bool Stack_get_top(Stack _stk, var* _to)
{
    if (unlikely(!_stk->stack_top_ptr))
        return false;
    {
        var _ret = _stk->stack[_stk->stack_top_ptr - 1];
        *_to = _ret;
        return true;
    }
}
void Stack_clear(Stack _stk)
{
    _stk->stack_top_ptr = 0;
}
void Stack_resize(Stack _stk, uint _size)
{
    var* new_stack = Malloc(sizeof(var) * _size);
    if (_stk->stack_length > _size)
    {
        _stk->stack_length = _size;
    }
    if (_stk->stack_top_ptr > _size - 1)
    {
        _stk->stack_top_ptr = _size - 1;
    }
    memcpy(new_stack, _stk->stack, sizeof(var) * _stk->stack_length);
    Mfree(_stk->stack);
    _stk->stack = new_stack;
}
uint Stack_count(Stack _stk)
{
    return _stk->stack_top_ptr;
}
var* Stack_find(Stack _stk, uint _i)
{
    if (_i < _stk->stack_top_ptr)
    {
        return &_stk->stack[_i];
    }
    else
    {
        return NULL;
    }
}

FixedStack FixedStack_Init(FixedStack _stk, uint _value_size)
{
    _stk->stack = Malloc(_value_size * STACK_INITIAL_LENGTH);
    _stk->stack_length = STACK_INITIAL_LENGTH;
    _stk->stack_top_ptr = 0;
    _stk->value_size = _value_size;
    return _stk;
}
FixedStack FixedStack_new(uint _value_size)
{
    FixedStack ret = Malloc(sizeof(fixed_stack));
    FixedStack_Init(ret, _value_size);
    return ret;
}
void FixedStack_Dest(FixedStack _stk)
{
    Mfree((vptr)_stk->stack);
}
void _fixed_stack_realloc(FixedStack _stk)
{
    uint _pre_stack_length = _stk->stack_length;
    _stk->stack_length *= 2;
    {
        void* _buf = Malloc(_stk->value_size * _stk->stack_length);
        memcpy(_buf, _stk->stack, _pre_stack_length * _stk->value_size);
        Mfree(_stk->stack);
        _stk->stack = _buf;
    }
}
void FixedStack_push(FixedStack _stk, void* _data)
{
    memcpy((char*)_stk->stack + _stk->stack_top_ptr * _stk->value_size, _data, _stk->value_size);
    _stk->stack_top_ptr++;
    if (unlikely(_stk->stack_top_ptr == _stk->stack_length))
    {
        _fixed_stack_realloc(_stk);
    }
}
bool FixedStack_pop(FixedStack _stk, void* _to)
{
    if (FixedStack_get_top(_stk, _to))
    {
        _stk->stack_top_ptr--;
        return true;
    }
    else
    {
        return false;
    }
}
bool FixedStack_get_top(FixedStack _stk, void* _to)
{
    if (unlikely(!_stk->stack_top_ptr))
        return false;
    {
        memcpy(_to, (char*)_stk->stack + (_stk->stack_top_ptr - 1) * _stk->value_size, _stk->value_size);
        return true;
    }
}
void FixedStack_clear(FixedStack _stk)
{
    _stk->stack_top_ptr = 0;
}
void FixedStack_resize(FixedStack _stk, uint _size)
{
    void* new_stack = Malloc(_stk->value_size * _size);
    if (_stk->stack_length > _size)
    {
        _stk->stack_length = _size;
    }
    if (_stk->stack_top_ptr > _size - 1)
    {
        _stk->stack_top_ptr = _size - 1;
    }
    memcpy(new_stack, _stk->stack, _stk->value_size * _stk->stack_length);
    Mfree(_stk->stack);
    _stk->stack = new_stack;
}
uint FixedStack_count(FixedStack _stk)
{
    return _stk->stack_top_ptr;
}
void* FixedStack_find(FixedStack _stk, uint _i)
{
    if (_i < _stk->stack_top_ptr)
    {
        return (char*)_stk->stack + _i * _stk->value_size;
    }
    else
    {
        return NULL;
    }
}

void FixedStack_test()
{
    FixedStack s = FixedStack_new(sizeof(int));
    int from = 0;
    for (int i = 0; i < 100; i++)
    {
        from = i * 100;
        FixedStack_push(s, &from);
    }
    int to = 0;

    for (int i = 0; i < 101; i++)
    {
        if (FixedStack_pop(s, &to))
            printf("pop %d\n", to);
        else
            printf("pop fail\n");
    }
}
