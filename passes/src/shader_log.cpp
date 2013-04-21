#include "shader_log.h"
#include <string.h>
static FILE* g_std_pass_log_file = NULL;
static FILE* g_lighting_pass_log_file = NULL;
static FILE* g_post_pass_log_file = NULL;
char g_shader_log_buffer[SHADER_LOG_BUFFER_SIZE];
void ShaderLog_Init()
{
    g_std_pass_log_file = SafeFOpen("std_pass_log.txt", "w+");
    g_lighting_pass_log_file = SafeFOpen("lighting_pass_log.txt", "w+");
    g_post_pass_log_file = SafeFOpen("post_pass_log.txt", "w+");
}

void ShaderLog_write(ShaderLogType log_type)
{
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
}

void ShaderLog_Release()
{
    fclose(g_std_pass_log_file);
    fclose(g_lighting_pass_log_file);
    fclose(g_post_pass_log_file);
    g_std_pass_log_file = NULL;
    g_lighting_pass_log_file = NULL;
    g_post_pass_log_file = NULL;
}
