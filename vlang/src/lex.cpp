
#include "etypes.h"
#include "common.h"
#include "tree.h"
#include "test.tab.h"
#include "parser_env.h"
#include "symbol_stack.h"
#include "estring.h"
#include "emem.h"
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
        SymbolValue* v = (SymbolValue*)data.vptr_var;
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
    Iterator iter = SymbolStack_find_symbol(&e->sym_stack, str);
    if (iter)
    {
        var data = Tree_get_value(iter);
        SymbolValue* v = (SymbolValue*)data.vptr_var;
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
        FunctionClosures fc = (FunctionClosures)data.vptr_var;
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
        float f = (float)atof(str);
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
        int i = atoi(str);
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
/**
static int remainder = 1024;
static int string_size = 0;
static char mbuf[1024];
static enum yytokentype curtSymType = empty;
static char* str = mbuf;
**/
int _push_value(SymbolValue * lvalp, ParserEnv* e)
{
	if (e->lex_state.string_size) {
		if (e->lex_state.curtSymType == _numerical) {
			///_push_const_numerical_value(e, lvalp, atof(mbuf));
			///return _float;
			return _push_const_numerical_value(e, lvalp, e->lex_state.mbuf);
		}
		else if (e->lex_state.curtSymType == _sym) {
			if (_find_key_word(e, e->lex_state.mbuf, lvalp))
				return lvalp->type;
			if (_find_func(e, e->lex_state.mbuf, lvalp))
				return _function;
			if (!_find_symbol(e, e->lex_state.mbuf, lvalp))
				ParserEnv_insert_unknown_symbol(e, e->lex_state.mbuf, lvalp);
			return _sym;
		}
		else if (e->lex_state.curtSymType == _text) {
			int offs = snprintf(e->lex_state.str, e->lex_state.remainder, " ");
			e->lex_state.str += offs;
			e->lex_state.remainder -= offs;
			e->lex_state.string_size += offs;
			return 0;
		}
		else if (e->lex_state.curtSymType == empty) {
			// 什麼也不做
			return 0;
		}
	}
	return -1;
}

int _cale_sym_proc(SymbolValue * lvalp, ParserEnv* e, int ret_value)
{
	if (e->lex_state.curtSymType != empty) {
		int ret = _push_value(lvalp, e);
		if (ret == -1)
			return 0;
		else if (ret > 0)
			return ret;
		else {
			_push_const_string_value(e, lvalp, e->lex_state.mbuf);
			return _text;
		}
	}
	else {
		e->char_count++;
		return ret_value;
	}
}

int _yylex (SymbolValue * lvalp, ParserEnv* e)
{
	/**
remainder = 1024;
string_size = 0;
curtSymType = empty;
str = mbuf;
**/
	ParserEnv_lex_Init(e);
    e->lex_state.remainder--;

    while (e->text[e->char_count])
    {
        if ( (e->text[e->char_count] >= 'A' &&
                e->text[e->char_count] <= 'Z') ||
                (e->text[e->char_count] >= 'a' &&
                 e->text[e->char_count] <= 'z') )
        {
            if (e->lex_state.curtSymType == empty)
                e->lex_state.curtSymType = _sym;
            if (e->lex_state.curtSymType != _sym &&
                    e->lex_state.curtSymType != _text)
                return 0;
        }
        else if (e->text[e->char_count] >= '0' &&
                 e->text[e->char_count] <= '9')
        {
            if (e->lex_state.curtSymType == empty)
                e->lex_state.curtSymType = _numerical;
            if (e->lex_state.curtSymType != _numerical &&
                e->lex_state.curtSymType != _sym &&
                    e->lex_state.curtSymType != _text)
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
                if (e->lex_state.curtSymType != empty) {
                    int ret = _push_value(lvalp, e);
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
                if (e->lex_state.curtSymType == empty)
                    e->lex_state.curtSymType = _text;
                else if (e->lex_state.curtSymType == _text) {
                    _push_const_string_value(e, lvalp, e->lex_state.mbuf);
                    e->char_count++;
                    return _text;
                }
                else
                    return 0;
            }
            break;
            case '+':
                return _cale_sym_proc(lvalp, e, _add);
            case '-':
                return _cale_sym_proc(lvalp, e, _sub);
            case '*':
                return _cale_sym_proc(lvalp, e, _mul);
            case '/':
                return _cale_sym_proc(lvalp, e, _div);
            case '(':
                return _cale_sym_proc(lvalp, e, left_parenthese);
            case ')':
                return _cale_sym_proc(lvalp, e, right_parenthese);
            case '[':
                return _cale_sym_proc(lvalp, e, left_bracket);
            case ']':
                return _cale_sym_proc(lvalp, e, right_bracket);
            case '{':
                SymbolStack_push(&e->sym_stack);
                return _cale_sym_proc(lvalp, e, left_brace);
            case '}':
                SymbolStack_pop(&e->sym_stack);
                return _cale_sym_proc(lvalp, e, right_brace);
            case '=': {
                char next_char = e->text[e->char_count + 1];
                if (next_char) {
                    if (next_char == '=') {
                        if (e->lex_state.curtSymType == empty)
                            e->char_count++;
                        return _cale_sym_proc(lvalp, e, _equal);
                    }
                    else
                        return _cale_sym_proc(lvalp, e, _assign);
                }
                else
                    return 0;
            }
            case '<': {
                char next_char = e->text[e->char_count + 1];
                if (next_char) {
                    if (next_char == '=') {
                        if (e->lex_state.curtSymType == empty)
                            e->char_count++;
                        return _cale_sym_proc(lvalp, e, _less_equal);
                    }
                    else
                        return _cale_sym_proc(lvalp, e, _less);
                }
                else
                    return 0;
            }
            case '>': {
                char next_char = e->text[e->char_count + 1];
                if (next_char) {
                    if (next_char == '=') {
                        if (e->lex_state.curtSymType == empty)
                            e->char_count++;
                        return _cale_sym_proc(lvalp, e, _greater_equal);
                    }
                    else
                        return _cale_sym_proc(lvalp, e, _greater);
                }
                else
                    return 0;
            }
            case ':': {
                char next_char = e->text[e->char_count + 1];
                if (next_char) {
                    if (next_char == '=') {
                        if (e->lex_state.curtSymType == empty)
                            e->char_count++;
                        return _cale_sym_proc(lvalp, e, _init_assign);
                    }
                    else
                        return _cale_sym_proc(lvalp, e, _risk);
                }
                else
                    return 0;
            }
            case ',':
                return _cale_sym_proc(lvalp, e, comma);
            case ';':
                return _cale_sym_proc(lvalp, e, line_end);
            }
        }

        int offs = snprintf(e->lex_state.str, e->lex_state.remainder, "%c", e->text[e->char_count]);
        e->lex_state.str += offs;
        e->lex_state.remainder -= offs;
        e->lex_state.string_size += offs;
        e->char_count++;
    }
    return 0;
}

int yylex (SymbolValue * lvalp, ParserEnv* e)
{
    int ret = _yylex (lvalp, e);
    return ret;
}

