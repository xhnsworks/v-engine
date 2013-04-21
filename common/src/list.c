#include "list.h"
#include "container.h"

_INLINE_ void list_node_Init(struct list_node* _node, var _data)
{
    _node->iter_prev = NULL;
    _node->iter_next = NULL;
    _node->value = _data;
}
List List_Init(List _lst, etype _value_type, MALLOC _alc, MFREE _fre)
{
    list* lst = (list*)_lst;
    lst->head = NULL;
    lst->tail = NULL;
    lst->count = 0;
    lst->value_type = _value_type;
    lst->alloc_proc = _alc;
    lst->free_proc = _fre;
    pthread_spin_init(&lst->lock, PTHREAD_PROCESS_PRIVATE);
    return _lst;
}
List List_new(etype _value_type, MALLOC _alc, MFREE _fre)
{
    list* ret = _alc(sizeof(list));
    return List_Init(ret, _value_type, _alc, _fre);
}
void List_Dest(List _lst)
{
    Iterator iter = List_begin(_lst);
    while (iter)
    {
        iter = List_remove(_lst, iter);
    }
    pthread_spin_destroy(&_lst->lock);
}

Iterator List_push_back(List _lst, var _data)
{
    struct list_node* node = _lst->alloc_proc(sizeof(struct list_node));
    list_node_Init(node, _data);
    pthread_spin_lock(&_lst->lock);
    if (_lst->tail)
    {
        _lst->tail->iter_next = node;
        node->iter_prev = _lst->tail;
        node->iter_next = NULL;
        _lst->tail = node;
    }
    else
    {
        _lst->head = _lst->tail = node;
        _lst->tail->iter_prev = NULL;
        _lst->tail->iter_next = NULL;
    }
    _lst->count++;
    Iterator ret = _lst->tail;
    pthread_spin_unlock(&_lst->lock);
    return ret;
}

Iterator List_push_front(List _lst, var _data)
{
    struct list_node* node = _lst->alloc_proc(sizeof(struct list_node));
    list_node_Init(node, _data);
    pthread_spin_lock(&_lst->lock);
    if (_lst->head)
    {
        _lst->head->iter_prev = node;
        node->iter_next = _lst->head;
        node->iter_prev = NULL;
        _lst->head = node;
    }
    else
    {
        _lst->tail = _lst->head = node;
        _lst->head->iter_prev = NULL;
        _lst->head->iter_next = NULL;
    }
    _lst->count++;
    Iterator ret = _lst->head;
    pthread_spin_unlock(&_lst->lock);
    return ret;
}

Iterator List_insert_after(List _lst, Iterator _i, var _data)
{
    struct list_node* node = (struct list_node*)_i;
    struct list_node* n = _lst->alloc_proc(sizeof(struct list_node));
    list_node_Init(n, _data);
    pthread_spin_lock(&_lst->lock);
    if (node->iter_next)
    {
        n->iter_next = node->iter_next;
        node->iter_next->iter_prev = n;
        n->iter_prev = node;
        node->iter_next = n;
        _lst->count++;
        pthread_spin_unlock(&_lst->lock);
        return n;
    }
    else
    {
        node->iter_next = n;
        _lst->tail = node->iter_next;
        _lst->tail->iter_prev = node;
        _lst->tail->iter_next = NULL;
        _lst->count++;
        Iterator ret = _lst->tail;
        pthread_spin_unlock(&_lst->lock);
        return ret;
    }
}

Iterator List_insert_before(List _lst, Iterator _i, var _data)
{
    struct list_node* node = (struct list_node*)_i;
    struct list_node* n = _lst->alloc_proc(sizeof(struct list_node));
    list_node_Init(n, _data);
    pthread_spin_lock(&_lst->lock);
    if (node->iter_prev)
    {
        n->iter_prev = node->iter_prev;
        node->iter_prev->iter_next = n;
        n->iter_next = node;
        node->iter_prev = n;
        _lst->count++;
        pthread_spin_unlock(&_lst->lock);
        return n;
    }
    else
    {
        node->iter_prev = n;
        _lst->head = node->iter_prev;
        _lst->head->iter_prev = NULL;
        _lst->head->iter_next = node;
        _lst->count++;
        Iterator ret = _lst->head;
        pthread_spin_unlock(&_lst->lock);
        return ret;
    }
}

Iterator List_remove(List _lst, Iterator _i)
{
    if (!_lst->count)
    {
        return NULL;
    }
    else
    {
        struct list_node* node = (struct list_node*)_i;
        pthread_spin_lock(&_lst->lock);
        _lst->count--;
        if (node != _lst->head && node != _lst->tail)
        {
            struct list_node* ret = node->iter_next;
            node->iter_prev->iter_next = node->iter_next;
            node->iter_next->iter_prev = node->iter_prev;
            pthread_spin_unlock(&_lst->lock);
            _lst->free_proc((vptr)node);
            return ret;
        }
        else if (node != _lst->head && node == _lst->tail)
        {
            node->iter_prev->iter_next = NULL;
            _lst->tail = node->iter_prev;
            pthread_spin_unlock(&_lst->lock);
            _lst->free_proc(node);
            return NULL;
        }
        else if (node == _lst->head && node != _lst->tail)
        {
            struct list_node* ret = node->iter_next;
            node->iter_next->iter_prev = NULL;
            _lst->head = node->iter_next;
            pthread_spin_unlock(&_lst->lock);
            _lst->free_proc(node);
            return ret;
        }
        else
        {
            _lst->head = _lst->tail = NULL;
            pthread_spin_unlock(&_lst->lock);
            _lst->free_proc(_lst->head);
            return NULL;
        }
    }
}

Iterator List_next(Iterator _i)
{
    return ((struct list_node*)_i)->iter_next;
}

Iterator List_prev(Iterator _i)
{
    return ((struct list_node*)_i)->iter_prev;
}

Iterator List_begin(List _lst)
{
    return _lst->head;
}

Iterator List_end(List _lst)
{
    return _lst->tail;
}

List List_append(List _head, List _tail)
{
    pthread_spin_lock(&_head->lock);
    pthread_spin_lock(&_tail->lock);
    if (!_head || !_tail)
    {
        pthread_spin_unlock(&_head->lock);
        pthread_spin_unlock(&_tail->lock);
        return NULL;
    }
    if (_head->value_type != _tail->value_type)
    {
        pthread_spin_unlock(&_head->lock);
        pthread_spin_unlock(&_tail->lock);
        return NULL;
    }
    if (_head->count && _tail->count)
    {
        _head->count += _tail->count;
        _head->tail = _tail->tail;
        pthread_spin_unlock(&_head->lock);
        pthread_spin_unlock(&_tail->lock);
        return _head;
    }
    else if (!_head->count && _tail->count)
    {
        _head->count = _tail->count;
        _head->head = _tail->head;
        _head->tail = _tail->tail;
        pthread_spin_unlock(&_head->lock);
        pthread_spin_unlock(&_tail->lock);
        return _head;
    }
    else if (_head->count && !_tail->count)
    {
        pthread_spin_unlock(&_head->lock);
        pthread_spin_unlock(&_tail->lock);
        return _head;
    }
    else if (!_head->count && !_tail->count)
    {
        pthread_spin_unlock(&_head->lock);
        pthread_spin_unlock(&_tail->lock);
        return _head;
    }
    pthread_spin_unlock(&_head->lock);
    pthread_spin_unlock(&_tail->lock);
    return NULL;
}

List List_break(List _lst, Iterator _i)
{
	struct list_node* prev = NULL;
	pthread_spin_lock(&_lst->lock);
    if (!_lst || !_i)
    {
        pthread_spin_unlock(&_lst->lock);
        return NULL;
    }

    prev = ((struct list_node*)_i)->iter_prev;
    if (prev)
    {
		List ret = NULL;
        uint32 count = 0;
        struct list_node* iter = (struct list_node*)_i;
        while (iter)
        {
            count++;
            iter = iter->iter_next;
        }
        if (!count)
        {
            pthread_spin_unlock(&_lst->lock);
            return NULL;
        }
        ret = _lst->alloc_proc(sizeof(list));
        ret->head = (struct list_node*)_i;
        ret->tail = _lst->tail;
        ret->count = count;
        ret->value_type = _lst->value_type;

        _lst->count -= count;
        _lst->tail = prev;
        pthread_spin_unlock(&_lst->lock);
        return ret;
    }
    else
    {
        pthread_spin_unlock(&_lst->lock);
        return NULL;
    }
}

uint List_count(List _lst)
{
    return _lst->count;
}

var List_get_value(Iterator _i)
{
    return ((struct list_node*)_i)->value;
}

void List_throw_front(List _lst, Iterator _i)
{
    if (!_lst->count)
        return;
    {
        struct list_node* node = (struct list_node*)_i;
        pthread_spin_lock(&_lst->lock);
        if (node != _lst->head && node != _lst->tail)
        {
            node->iter_prev->iter_next = node->iter_next;
            node->iter_next->iter_prev = node->iter_prev;
        }
        else if (node != _lst->head && node == _lst->tail)
        {
            node->iter_prev->iter_next = NULL;
            _lst->tail = node->iter_prev;
        }
        else
        {
            pthread_spin_unlock(&_lst->lock);
            return;
        }

        _lst->head->iter_prev = node;
        node->iter_next = _lst->head;
        node->iter_prev = NULL;
        _lst->head = node;
        pthread_spin_unlock(&_lst->lock);
    }
}

void List_throw_back(List _lst, Iterator _i)
{
    if (!_lst->count)
        return;
    {
        struct list_node* node = (struct list_node*)_i;
        pthread_spin_lock(&_lst->lock);
        if (node != _lst->head && node != _lst->tail)
        {
            node->iter_prev->iter_next = node->iter_next;
            node->iter_next->iter_prev = node->iter_prev;
        }
        else if (node == _lst->head && node != _lst->tail)
        {
            node->iter_next->iter_prev = NULL;
            _lst->head = node->iter_next;
        }
        else
        {
            pthread_spin_unlock(&_lst->lock);
            return;
        }

        _lst->tail->iter_next = node;
        node->iter_prev = _lst->tail;
        node->iter_next = NULL;
        _lst->tail = node;
        pthread_spin_unlock(&_lst->lock);
    }
}
