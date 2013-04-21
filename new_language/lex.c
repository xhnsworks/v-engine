
#include "etypes.h"
#include "common.h"
#include "tree.h"
#include "test.tab.h"
#include "parser_env.h"
#include "symbol_stack.h"
#include "estring.h"
#include "mem.h"
/**
#define SYMBOL_VALUE_STACK_SIZE 1024
typedef struct _symbol_value_stack
{
    SymbolValue symbols[SYMBOL_VALUE_STACK_SIZE];
} symbol_value_stack;
**/

static inline bool _sym_tree_search(Tree t, char* str, SymbolValue* result)
{
    var key, data;
    key.str_var = str;

    if (Tree_find(t, key, &data))
    {
        SymbolValue* v = data.vptr_var;
        memcpy(result, v, sizeof(SymbolValue));
        return true;
    }
    else
    {
        return false;
    }
}

static inline bool _sym_stack_search(ParserEnv* e, char* str, SymbolValue* result)
{
    Iterator iter = SymbolStack_find_symbol(&e->symbol_stack, str);
    if (iter)
    {
        var data = Tree_get_value(iter);
        SymbolValue* v= data.vptr_var;
        memcpy(result, v, sizeof(SymbolValue));
        return true;
    }
    else
        return false;
}

static inline bool _func_tree_search(ParserEnv* e, char* str, SymbolValue* result)
{
    var key, data;
    key.str_var = str;
    if (Tree_find(e->func_tree, key, &data))
    {
        FunctionClosures fc = data.vptr_var;
        result->type = Function;
        result->data.func = fc;
        return true;
    }
    else
        return false;
}

bool _find_key_word(ParserEnv* e, char* str, SymbolValue* result)
{
    return _sym_tree_search(e->key_word_tree, str, result);
}

bool _find_func(ParserEnv* e, char* str, SymbolValue* result)
{
    return _func_tree_search(e, str, result);
}

bool _find_symbol(ParserEnv* e, char* str, SymbolValue* result)
{
    return _sym_stack_search(e, str, result);
}
/**
void _push_const_float_value(ParserEnv* e, SymbolValue* lvalp, float f)
{
    int offs = ValueAllocator_alloc(&e->cons_alloc, FloatValue);
    lvalp->type = FloatValue;
    lvalp->data.mem_addr.mem_type = ConsMem;
    lvalp->data.mem_addr.offs = offs;
    char* mem = e->mems[ConsMem];
    memcpy(mem + offs, &f, sizeof(&f));
}
**/
/// 直到最后一刻才确定具体的数值类型，是int还是float
int _push_const_numerical_value(ParserEnv* e, SymbolValue* lvalp, const char* str)
{
    int count = 0;
    int decimal_point_pos = -1;
    while (str[count])
    {
        if (str[count] == '.') {
            decimal_point_pos = count;
            break;
        }
        count++;
    }
    if (decimal_point_pos >= 0)
    {
        float f = atof(str);
        int offs = ValueAllocator_alloc(&e->cons_alloc, FloatValue);
        lvalp->type = FloatValue;
        lvalp->data.mem_addr.mem_type = ConsMem;
        lvalp->data.mem_addr.offs = offs;
        char* mem = e->mems[ConsMem];
        memcpy(mem + offs, &f, sizeof(&f));
        return _float;
    }
    else
    {
        int i = atof(str);
        int offs = ValueAllocator_alloc(&e->cons_alloc, IntValue);
        lvalp->type = IntValue;
        lvalp->data.mem_addr.mem_type = ConsMem;
        lvalp->data.mem_addr.offs = offs;
        char* mem = e->mems[ConsMem];
        memcpy(mem + offs, &i, sizeof(&i));
        return _int;
    }
}

void _push_const_string_value(ParserEnv* e, SymbolValue* lvalp, char* s)
{
    const char* str = EString_new(s);
    int offs = ValueAllocator_alloc(&e->cons_str_alloc, StringValue);
    lvalp->type = StringValue;
    lvalp->data.mem_addr.mem_type = ConsStrMem;
    lvalp->data.mem_addr.offs = offs;
    char* mem = e->mems[ConsStrMem];
    memcpy(mem + offs, &str, sizeof(str));
}

int _yylex (SymbolValue * lvalp, ParserEnv* e)
{
    int remainder = 1024;
    int string_size = 0;
    char mbuf[remainder];
    char* str = mbuf;
    remainder--;
    enum yytokentype curtSymType = empty;

    int _push_value()
    {
        if (string_size) {
            if (curtSymType == _numerical) {
                ///_push_const_numerical_value(e, lvalp, atof(mbuf));
                ///return _float;
                return _push_const_numerical_value(e, lvalp, mbuf);
            }
            else if (curtSymType == _sym) {
                if (_find_key_word(e, mbuf, lvalp))
                    return lvalp->type;
                if (_find_func(e, mbuf, lvalp))
                    return _function;
                if (!_find_symbol(e, mbuf, lvalp))
                    ParserEnv_insert_unknown_symbol(e, mbuf, lvalp);
                return _sym;
            }
            else if (curtSymType == _text) {
                int offs = snprintf(str, remainder, " ");
                str += offs;
                remainder -= offs;
                string_size += offs;
                return 0;
            }
            else if (curtSymType == empty) {
                // 什麼也不做
                return 0;
            }
        }
        return -1;
    }

    int _cale_sym_proc(int ret_value)
    {
        if (curtSymType != empty) {
            int ret = _push_value();
            if (ret == -1)
                return 0;
            else if (ret > 0)
                return ret;
            else {
                _push_const_string_value(e, lvalp, mbuf);
                return _text;
            }
        }
        else {
            e->char_count++;
            return ret_value;
        }
    }

    while (e->text[e->char_count])
    {
        if ( (e->text[e->char_count] >= 'A' &&
                e->text[e->char_count] <= 'Z') ||
                (e->text[e->char_count] >= 'a' &&
                 e->text[e->char_count] <= 'z') )
        {
            if (curtSymType == empty)
                curtSymType = _sym;
            if (curtSymType != _sym &&
                    curtSymType != _text)
                return 0;
        }
        else if (e->text[e->char_count] >= '0' &&
                 e->text[e->char_count] <= '9')
        {
            if (curtSymType == empty)
                curtSymType = _numerical;
            if (curtSymType != _numerical &&
                curtSymType != _sym &&
                    curtSymType != _text)
                return 0;
        }
        else
        {
            switch(e->text[e->char_count]) {
            case '\n':
                e->line_count++;
                e->char_count++;
                continue;
            case ' ':
            case '\r': {
                if (curtSymType != empty) {
                    int ret = _push_value();
                    if (ret == -1)
                        return 0;
                    else if (ret > 0) {
                        e->char_count++;
                        return ret;
                    }
                }
                else {
                    e->char_count++;
                    continue;
                }
            }
            break;
            case '"': {
                if (curtSymType == empty)
                    curtSymType = _text;
                else if (curtSymType == _text) {
                    _push_const_string_value(e, lvalp, mbuf);
                    e->char_count++;
                    return _text;
                }
                else
                    return 0;
            }
            break;
            case '+':
                return _cale_sym_proc(_add);
            case '-':
                return _cale_sym_proc(_sub);
            case '*':
                return _cale_sym_proc(_mul);
            case '/':
                return _cale_sym_proc(_div);
            case '(':
                return _cale_sym_proc(left_parenthese);
            case ')':
                return _cale_sym_proc(right_parenthese);
            case '[':
                return _cale_sym_proc(left_bracket);
            case ']':
                return _cale_sym_proc(right_bracket);
            case '{':
                SymbolStack_push(&e->symbol_stack);
                return _cale_sym_proc(left_brace);
            case '}':
                SymbolStack_pop(&e->symbol_stack);
                return _cale_sym_proc(right_brace);
            case '=': {
                char next_char = e->text[e->char_count + 1];
                if (next_char) {
                    if (next_char == '=') {
                        if (curtSymType == empty)
                            e->char_count++;
                        return _cale_sym_proc(_equal);
                    }
                    else
                        return _cale_sym_proc(_assign);
                }
                else
                    return 0;
            }
            case '<': {
                char next_char = e->text[e->char_count + 1];
                if (next_char) {
                    if (next_char == '=') {
                        if (curtSymType == empty)
                            e->char_count++;
                        return _cale_sym_proc(_less_equal);
                    }
                    else
                        return _cale_sym_proc(_less);
                }
                else
                    return 0;
            }
            case '>': {
                char next_char = e->text[e->char_count + 1];
                if (next_char) {
                    if (next_char == '=') {
                        if (curtSymType == empty)
                            e->char_count++;
                        return _cale_sym_proc(_greater_equal);
                    }
                    else
                        return _cale_sym_proc(_greater);
                }
                else
                    return 0;
            }
            case ':': {
                char next_char = e->text[e->char_count + 1];
                if (next_char) {
                    if (next_char == '=') {
                        if (curtSymType == empty)
                            e->char_count++;
                        return _cale_sym_proc(_init_assign);
                    }
                    else
                        return _cale_sym_proc(_risk);
                }
                else
                    return 0;
            }
            case ',':
                return _cale_sym_proc(comma);
            case ';':
                return _cale_sym_proc(line_end);
            }
        }

        int offs = snprintf(str, remainder, "%c", e->text[e->char_count]);
        str += offs;
        remainder -= offs;
        string_size += offs;
        e->char_count++;
    }
    return 0;
}

int yylex (SymbolValue * lvalp, ParserEnv* e)
{
    int ret = _yylex (lvalp, e);
    return ret;
}

