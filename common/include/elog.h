#ifndef ELOG_H
#define ELOG_H
#include "common.h"
#include "etypes.h"
#include "pthread.h"
extern pthread_spinlock_t g_lock;
#define ELOG_BUFFER_SIZE 4096 * 2
API_EXPORT void ELog_Init();
API_EXPORT void ELog_Release();
extern char g_elog_buffer[ELOG_BUFFER_SIZE];
API_EXPORT void ELog_write();

#define elog(fmt,...) { pthread_spin_lock(&g_lock); snprintf(g_elog_buffer,ELOG_BUFFER_SIZE - 2,fmt,##__VA_ARGS__); ELog_write(); pthread_spin_unlock(&g_lock); }
///#define delete(n, o) { snprintf(g_elog_buffer,ELOG_BUFFER_SIZE - 2,#n"_delete FILE %s LINE %d", __FILE__, __LINE__); ELog_write(); _##n##_delete(o); }
#define dest(n, p) { pthread_spin_lock(&g_lock); snprintf(g_elog_buffer,ELOG_BUFFER_SIZE - 2,#n"_dest FILE %s LINE %d", __FILE__, __LINE__); ELog_write(); _##n##_Dest(p); pthread_spin_unlock(&g_lock); }
#endif
