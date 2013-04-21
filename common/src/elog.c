#include "elog.h"
static FILE* g_elog_file = NULL;
char g_elog_buffer[ELOG_BUFFER_SIZE];
void ELog_Init()
{
    g_elog_file = fopen("log.txt", "w+");
}

void ELog_write()
{
    sprintf(g_elog_buffer, "%s\n", g_elog_buffer);
    fwrite(g_elog_buffer, strlen(g_elog_buffer), 1, g_elog_file);
}

void ELog_Release()
{
    fclose(g_elog_file);
    g_elog_file = NULL;
}
