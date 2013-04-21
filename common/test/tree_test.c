#include "tree_test.h"
#include "mem.h"
#include "estring.h"
void tree_test()
{
    MInit();
    Tree t = Tree_new(Uint32, Uint32, malloc, free);
    Tree str_t = Tree_new(String, Uint32, malloc, free);
    var key, data;
    uint32 test[1024];
    char* c_test[1024];
    for (uint32 i = 0; i < 1024; i++)
    {
        char mbuf[256];
        snprintf(mbuf, 256, "key%d", i);
        test[i] = i;
        c_test[i] = EString_new(mbuf);
    }
    for (uint32 i = 0; i < 1024; i += 2)
    {
        key.uint32_var = i;
        data.uint32_var = test[i];
        Tree_insert(t, key, data);
    }
    for (uint32 i = 0; i < 1024; i++)
    {
        data.uint32_var = 0;
        key.uint32_var = i;
        bool ret = Tree_find(t, key, &data);
        if (i % 2 && ret)
        {
            printf("test find fail\n");
            getchar();
            exit(0);
        }
        else if ( !(i % 2) )
        {
            if (!ret)
            {
                printf("test find result fail\n");
                getchar();
                exit(0);
            }
            if (data.uint32_var != i)
            {
                printf("test find data fail\n");
                getchar();
                exit(0);
            }
        }
    }
    for (uint32 i = 0; i < 1024; i++)
    {
        key.str_var = c_test[i];
        data.uint32_var = i;
        Tree_insert(str_t, key, data);
    }
    for (uint32 i = 0; i < 1024; i++)
    {
        char mbuf[256];
        snprintf(mbuf, 256, "key%d", i);
        key.str_var = EString_new(mbuf);
        data.uint32_var = 0;
        bool ret = Tree_find(str_t, key, &data);
        if (data.uint32_var != i && ret)
        {
            printf("string test find fail\n");
            getchar();
            exit(0);
        }
    }
    printf("tree test succ\n");
    getchar();
}
