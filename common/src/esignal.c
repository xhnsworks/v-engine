#include "esignal.h"
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
static jmp_buf s_jmp_buf;
void handler(int s)
{
    longjmp(s_jmp_buf, 1);
}
void ESignalInit()
{
    signal(SIGINT, handler);
}

int ESetJmp()
{
    return setjmp(s_jmp_buf);
}

void ERaise(int s)
{
    raise(s);
}
