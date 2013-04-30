#include "symbol_stack.h"
#include "emem.h"
#include "eassert.h"
#include "parser_env.h"
void SymbolStack_Init(SymbolStack _self)
{
    _self->main_stack = FixedStack_new(sizeof(Tree));
}
void SymbolStack_Dest(SymbolStack _self)
{
    Tree sym_tree = NULL;
    while(FixedStack_pop(_self->main_stack, &sym_tree))
    {
        Iterator iter = Tree_begin(sym_tree);
        while (iter)
        {
            var value = Tree_get_value(iter);
            SymbolValue* sv = (SymbolValue*)value.vptr_var;
            Mfree(sv);
            /// only free SymbolValue
            ///Mfree(sv->value);
            iter = Tree_next(iter);
        }
        Tree_Dest(sym_tree);
        ///Mfree(sym_tree);
    }
    FixedStack_Dest(_self->main_stack);
}

Iterator SymbolStack_find_symbol(SymbolStack _self, const char* _sym)
{
    var key, data;
    key.str_var = (EString)_sym;
    euint top_ptr = 0;
    Tree sym_tree = NULL;
    Iterator ret = NULL;
    if (FixedStack_walk_begin(_self->main_stack, &sym_tree, &top_ptr))
    {
        ret = Tree_find(sym_tree, key, &data);
        if (ret)
            return ret;
        while (FixedStack_walk(_self->main_stack, &sym_tree, &top_ptr))
        {
            ret = Tree_find(sym_tree, key, &data);
            if (ret)
                return ret;
        }
    }
    return NULL;
}

SymbolValue* SymbolStack_insert_unknown_symbol(SymbolStack _self, const char* _sym)
{
    Tree sym_tree = NULL;
    if (FixedStack_get_top(_self->main_stack, &sym_tree))
    {
        var key, data;
        key.str_var = (EString)_sym;
        SymbolValue* v = (SymbolValue*)SMalloc(sizeof(SymbolValue));
        v->type = UnknownValue;
        /// 创建未知符号时将符号节点填入value
        ///v->value = NULL;
        memset(&v->data, 0, sizeof(v->data));
        ///v->offs = -1;
        data.vptr_var = v;

        Iterator iter = Tree_insert(sym_tree, key, data);
        v->data.tree_iter = iter;
        return v;
    }
    return NULL;
}

void SymbolStack_push(SymbolStack _self)
{
    Tree sym_tree = Tree_new(String, Vptr, Ealloc, Efree);
    FixedStack_push(_self->main_stack, &sym_tree);
}

void SymbolStack_pop(SymbolStack _self)
{
    Tree sym_tree = NULL;
    if (FixedStack_pop(_self->main_stack, &sym_tree))
    {
        Iterator iter = Tree_begin(sym_tree);
        while (iter)
        {
            var value = Tree_get_value(iter);
            SymbolValue* sv = (SymbolValue*)value.vptr_var;
            Mfree(sv);
            /// only free SymbolValue
            ///Mfree(sv->value);
            iter = Tree_next(iter);
        }
        Tree_Dest(sym_tree);
    }
}
