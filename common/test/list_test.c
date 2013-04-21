#include <stdio.h>
#include <stdlib.h>
#include <etypes.h>
#include <common.h>
#include "list.h"
#include "stack.h"
#include "mem.h"
typedef struct _test_data
{
    uint count;
    var* data;
}test_data;
typedef test_data* TestData;

TestData TestData_Init(test_data* _test, uint _count)
{
    _test->count = _count;
    _test->data = malloc(sizeof(var) * _count);
    return _test;
}
void TestData_Dest(TestData _test)
{
    free(_test->data);
}
void TestData_list_uint_test(TestData _test)
{
	uint i = 0;
	///list _lst;
	List _lst = List_new(Vptr, malloc, free);
	List lst;
	Iterator iter;


    for (; i < _test->count; i++)
    {
        _test->data[i].uint32_var = i;
    }

	printf("push back begin test\n");

    lst = List_Init(_lst, Uint32, malloc, free);
    for (i = 0; i < _test->count; i++)
    {
        Iterator iter = List_push_back(lst, _test->data[i]);
        if (List_get_value(iter).uint32_var != _test->data[i].uint32_var)
        {
#if BIT_WIDTH == 64
            printf("data error! tmp %d data %d i %I64d count %I64d", List_get_value(iter).uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#else
            printf("data error! tmp %d data %d i %d count %d", List_get_value(iter).uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#endif
        }
    }
    i = 0;
    iter = List_begin(lst);
    while (iter)
    {
        var tmp = List_get_value(iter);
        if (tmp.uint32_var != _test->data[i].uint32_var)
        {
#if BIT_WIDTH == 64
            printf("data error! tmp %d data %d i %I64d count %I64d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#else
            printf("data error! tmp %d data %d i %d count %d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#endif
        }
        iter = List_next(iter);
        i++;
    }
    i = _test->count - 1;
    iter = List_end(lst);
    while (iter)
    {
        var tmp = List_get_value(iter);
        if (tmp.uint32_var != _test->data[i].uint32_var)
        {
#if BIT_WIDTH == 64
            printf("data error! tmp %d data %d i %I64d count %I64d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#else
            printf("data error! tmp %d data %d i %d count %d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#endif
        }
        iter = List_prev(iter);
        i--;
    }
    List_Dest(lst);
    printf("push back test end\n");
    ///
    printf("insert after test0 begin\n");
    lst = List_Init(lst, Uint32, malloc, free);
    iter = List_push_back(lst, _test->data[0]);
    for (i = 1; i < _test->count; i++)
    {
        iter = List_insert_after(lst, iter, _test->data[i]);
    }
    i = 0;
    iter = List_begin(lst);
    while (iter)
    {
        var tmp = List_get_value(iter);
        if (tmp.uint32_var != _test->data[i].uint32_var)
        {
#if BIT_WIDTH == 64
            printf("data error! tmp %d data %d i %I64d count %I64d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#else
            printf("data error! tmp %d data %d i %d count %d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#endif
        }
        iter = List_next(iter);
        i++;
    }
    List_Dest(lst);
    printf("insert after test0 end\n");
    ///
    printf("insert after test1 begin\n");
    lst = List_Init(lst, Uint32, malloc, free);
    iter = List_push_back(lst, _test->data[0]);
    List_push_back(lst, _test->data[_test->count - 1]);
    for (i = 1; i < _test->count - 1; i++)
    {
        iter = List_insert_after(lst, iter, _test->data[i]);
    }
    i = 0;
    iter = List_begin(lst);
    while (iter)
    {
        var tmp = List_get_value(iter);
        if (tmp.uint32_var != _test->data[i].uint32_var)
        {
#if BIT_WIDTH == 64
            printf("data error! tmp %d data %d i %I64d count %I64d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#else
            printf("data error! tmp %d data %d i %d count %d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#endif
        }
        iter = List_next(iter);
        i++;
    }
    List_Dest(lst);
    printf("insert after test1 end\n");
    ///
    printf("insert before test0 begin\n");
    lst = List_Init(lst, Uint32, malloc, free);
    iter = List_push_back(lst, _test->data[_test->count - 1]);
    for (i = _test->count - 2; i; i--)
    {
        iter = List_insert_before(lst, iter, _test->data[i]);
    }
    List_insert_before(lst, iter, _test->data[0]);
    i = 0;
    iter = List_begin(lst);
    while (iter)
    {
        var tmp = List_get_value(iter);
        if (tmp.uint32_var != _test->data[i].uint32_var)
        {
#if BIT_WIDTH == 64
            printf("data error! tmp %d data %d i %I64d count %I64d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#else
            printf("data error! tmp %d data %d i %d count %d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#endif
        }
        iter = List_next(iter);
        i++;
    }
    List_Dest(lst);
    printf("insert before test0 end\n");
    ///
    ///
    printf("insert before test1 begin\n");
    lst = List_Init(lst, Uint32, malloc, free);
    iter = List_push_back(lst, _test->data[0]);
    List_push_back(lst, _test->data[_test->count - 1]);
    iter = List_end(lst);
    for (i = _test->count - 2; i; i--)
    {
        iter = List_insert_before(lst, iter, _test->data[i]);
    }
    i = 0;
    iter = List_begin(lst);
    while (iter)
    {
        var tmp = List_get_value(iter);
        if (tmp.uint32_var != _test->data[i].uint32_var)
        {
#if BIT_WIDTH == 64
            printf("data error! tmp %d data %d i %I64d count %I64d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#else
            printf("data error! tmp %d data %d i %d count %d", tmp.uint32_var, _test->data[i].uint32_var, i, _test->count);
            getchar();
            exit(0);
#endif
        }
        iter = List_next(iter);
        i++;
    }
    List_Dest(lst);
    printf("insert before test1 end\n");
    ///

    printf("test succ\n");
}

void TestData_stack_uint_test(TestData _test)
{
	Stack _stk = Stack_new(Vptr);
	Stack stk = Stack_Init(_stk, Uint32);
	uint i = 0;

	for (; i < _test->count; i++)
    {
        _test->data[i].uint32_var = i;
    }

	for (i = 0; i < _test->count; i++)
	{
		Stack_push(stk, _test->data[i]);
	}
	for (i = 0; i < _test->count; i++)
	{
		if (Stack_find(stk, i)->uint32_var != _test->data[i].uint32_var)
		{
			printf("data error");
			getchar();
			exit(0);
		}
	}
    for (i = _test->count - 1; i; i--)
	{
		var tmp;
		Stack_pop(stk, &tmp);
		if (tmp.uint32_var != _test->data[i].uint32_var)
		{
			printf("data error");
			getchar();
			exit(0);
		}
	}

	Stack_Dest(stk);

	printf("test succ\n");
}
//////////////////////////////////////////


int list_test()
{
	test_data _t;
	TestData t;

	MInit();

    t = TestData_Init(&_t, 1024);
    TestData_list_uint_test(t);
    TestData_Dest(t);
/**
    t = TestData_Init(&_t, 1024);
    TestData_stack_uint_test(t);
    TestData_Dest(t);
**/
    getchar();
    return 0;
}
