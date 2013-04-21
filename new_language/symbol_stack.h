#ifndef SYMBOL_STACK_H
#define SYMBOL_STACK_H
#include "common.h"
#include "etypes.h"
#include "stack.h"
#include "tree.h"

typedef struct _value_allocator
{
    int tag;
} value_allocator;
typedef struct _value_allocator* ValueAllocator;
typedef struct _symbol_stack
{
    FixedStack main_stack;
} symbol_stack;
typedef struct _symbol_stack* SymbolStack;
typedef struct _symbol_value SymbolValue;
static inline void ValueAllocator_Init(ValueAllocator _self)
{
    _self->tag = 0;
}
static int _get_value_type_size(ValueType _type)
{
    switch(_type)
    {
        case IntValue:
        case FloatValue:
        case Float2Value:
        case Float3Value:
        case Float4Value:
        case BoolValue:
        case Function:
            return 16;
        /// string as a pointer type
        case StringValue:
            return sizeof(char*);
        case Matrix4x4Value:
            return 64;
        default:
            return -1;
    }
}
static inline int ValueAllocator_alloc(ValueAllocator _self, ValueType _type)
{
    int ret = _self->tag;
    /**
    switch (_type)
    {
        case IntValue:
        case FloatValue:
        case Float2Value:
        case Float3Value:
        case Float4Value:
        case BoolValue:
        case Function:
            _self->tag += 16;
            return ret;
        /// string as a pointer type
        case StringValue:
            _self->tag += sizeof(char*);
            return ret;
        case Matrix4x4Value:
            _self->tag += 64;
            return ret;
        default:
            return -1;
    }
    **/
    int size = _get_value_type_size(_type);
    if (size > 0) {
        _self->tag += size;
        return ret;
    }
    else
        return -1;
}

static inline int ValueAllocator_totel_size(ValueAllocator _self)
{
    return _self->tag;
}

static inline void ValueAllocator_clear(ValueAllocator _self)
{
    _self->tag = 0;
}
void SymbolStack_Init(SymbolStack _self);
void SymbolStack_Dest(SymbolStack _self);
Iterator SymbolStack_find_symbol(SymbolStack _self, const char* _sym);
SymbolValue* SymbolStack_insert_unknown_symbol(SymbolStack _self, const char* _sym);
void SymbolStack_push(SymbolStack _self);
void SymbolStack_pop(SymbolStack _self);

#endif
