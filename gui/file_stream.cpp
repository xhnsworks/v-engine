#include "file_stream.h"
#include "mult_thread_mem_lib.h"
file_open_fn _file_open_proc = NULL;
file_close_fn _file_close_proc = NULL;
void set_file_open_fn(file_open_fn _fn)
{
    _file_open_proc = _fn;
}
void set_file_close_fn(file_close_fn _fn)
{
    _file_close_proc = _fn;
}

FileStream::FileStream(const char* _file_name)
{
    file_data* _fd = _file_open_proc(ONLY_READ, _file_name);
    m_size = g_file_get_size_proc(_fd);
    m_data = (char*)mult_thread_mem_alloc(m_size, __FILE__, __LINE__);
    g_file_read_proc(_fd, m_data, m_size);
    _file_close_proc(_fd);
    m_ptr = 0;
    m_file_name = _file_name;
}

FileStream::~FileStream()
{
    mult_thread_mem_free(m_data, __FILE__, __LINE__);
}
