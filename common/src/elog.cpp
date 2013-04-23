#include "pch.h"
#include "elog.h"
#include "string.h"

pthread_rwlock_t g_lock;
static FILE* g_elog_file = NULL;
char g_elog_buffer[ELOG_BUFFER_SIZE];
void ELog_Init()
{
	g_elog_file = SafeFOpen("log.log", "w+");
	pthread_rwlock_init(&g_lock, NULL);
}

void ELog_write()
{
	///pthread_spin_lock(&g_lock);
    snprintf(g_elog_buffer, ELOG_BUFFER_SIZE - 1, "%s\n", g_elog_buffer);
    fwrite(g_elog_buffer, strlen(g_elog_buffer), 1, g_elog_file);
	///pthread_spin_unlock(&g_lock);
}

void ELog_Release()
{
	///pthread_spin_lock(&g_lock);
    fclose(g_elog_file);
    g_elog_file = NULL;
	///pthread_spin_unlock(&g_lock);
}
