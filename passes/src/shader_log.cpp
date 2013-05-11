#include "shader_log.h"
#include <string.h>
#ifdef USE_LOG_SYSTEM
static FILE* g_std_pass_log_file = NULL;
static FILE* g_lighting_pass_log_file = NULL;
static FILE* g_post_pass_log_file = NULL;
#endif
char g_shader_log_buffer[SHADER_LOG_BUFFER_SIZE];
void ShaderLog_Init()
{
#ifdef USE_LOG_SYSTEM
#if defined(_WIN32) || defined(_WIN64)
    g_std_pass_log_file = SafeFOpen("std_pass_log.log", "w+");
    g_lighting_pass_log_file = SafeFOpen("lighting_pass_log.log", "w+");
    g_post_pass_log_file = SafeFOpen("post_pass_log.log", "w+");
#elif defined(__APPLE__)
    g_std_pass_log_file = SafeFOpen("/Users/joumining/v-engine/std_pass_log.log", "w+");
    g_lighting_pass_log_file = SafeFOpen("/Users/joumining/v-engine/lighting_pass_log.log", "w+");
    g_post_pass_log_file = SafeFOpen("/Users/joumining/v-engine/post_pass_log.log", "w+");
#endif
#endif
}

void ShaderLog_write(ShaderLogType log_type)
{
#ifdef USE_LOG_SYSTEM
    snprintf(g_shader_log_buffer, SHADER_LOG_BUFFER_SIZE - 1, "%s\n", g_shader_log_buffer);
    switch (log_type)
    {
    case StdPassLog:
        fwrite(g_shader_log_buffer, strlen(g_shader_log_buffer), 1, g_std_pass_log_file);
        break;
    case LightingPassLog:
        fwrite(g_shader_log_buffer, strlen(g_shader_log_buffer), 1, g_lighting_pass_log_file);
        break;
    case PostPassLog:
        fwrite(g_shader_log_buffer, strlen(g_shader_log_buffer), 1, g_post_pass_log_file);
        break;
    }
#endif
}

void ShaderLog_Release()
{
#ifdef USE_LOG_SYSTEM
    fclose(g_std_pass_log_file);
    fclose(g_lighting_pass_log_file);
    fclose(g_post_pass_log_file);
    g_std_pass_log_file = NULL;
    g_lighting_pass_log_file = NULL;
    g_post_pass_log_file = NULL;
#endif
}
