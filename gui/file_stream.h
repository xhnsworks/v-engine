/*
* Copyright (c) 2010, 徐海宁
* All rights reserved.
*
* 文件名称：file_strem.h
* 文件标识：见配置管理计划书
* 摘    要：
*
* 当前版本：0.2
* 替代版本：0.1
* 作    者：徐海宁
* 完成日期：2010年9月9日
*/

#ifndef FILE_STREAM_H
#define FILE_STREAM_H
///*************************************************************************************************************************///
///                                                     include begin                                                       ///
///*************************************************************************************************************************///
#include "types.h"
#include "common.h"
#include "class_typedef.h"
#include "fstring.h"
///*************************************************************************************************************************///
///                                                     include end                                                         ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                    function begin                                                       ///
///*************************************************************************************************************************///
void set_file_open_fn(file_open_fn _fn);
void set_file_close_fn(file_close_fn _fn);
///*************************************************************************************************************************///
///                                                     function end                                                        ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                             global function pointer begin                                               ///
///*************************************************************************************************************************///
extern file_open_fn _file_open_proc;
extern file_close_fn _file_close_proc;
///*************************************************************************************************************************///
///                                             global function pointer end                                                 ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class FileStream
{
public:
/**
    FileStream(file_data* _fd)
    {
        m_size = g_file_get_size_proc(_fd);
        m_data = (char*)mult_thread_mem_alloc(m_size);
        g_file_read_proc(_fd, m_data, m_size);
        g_file_close_proc(_fd);
        m_ptr = 0;
    }
    **/
    FileStream(const char* _file_name);
    ~FileStream();
    void reset()
    {
        m_ptr = 0;
    }
    char getc()
    {
        char _ret = m_data[m_ptr];
        m_ptr++;
        return _ret;
    }
    void ungetc(char c)
    {
        m_ptr--;
    }
    bool feof()
    {
        if (m_ptr >= m_size)
            return true;
        else
            return false;
    }
    uint read(char* _buf, uint _size)
    {
        uint _cpy_size;
        if (m_ptr + _size <= m_size)
            _cpy_size = _size;
        else
            _cpy_size = m_size - m_ptr;
        g_mem_copy_proc(_buf, &m_data[m_ptr], _cpy_size);
        m_ptr += _cpy_size;
        return _cpy_size;
    }
    uint read(uint* _buf)
    {
        uint _cpy_size;
        if (m_ptr + sizeof(uint) <= m_size)
            _cpy_size = sizeof(uint);
        else
            _cpy_size = m_size - m_ptr;
        g_mem_copy_proc(_buf, &m_data[m_ptr], _cpy_size);
        m_ptr += _cpy_size;
        return _cpy_size;
    }
    uint read(real* _buf)
    {
        uint _cpy_size;
        if (m_ptr + sizeof(real) <= m_size)
            _cpy_size = sizeof(real);
        else
            _cpy_size = m_size - m_ptr;
        g_mem_copy_proc(_buf, &m_data[m_ptr], _cpy_size);
        m_ptr += _cpy_size;
        return _cpy_size;
    }
    uint read(dreal* _buf)
    {
        uint _cpy_size;
        if (m_ptr + sizeof(dreal) <= m_size)
            _cpy_size = sizeof(dreal);
        else
            _cpy_size = m_size - m_ptr;
        g_mem_copy_proc(_buf, &m_data[m_ptr], _cpy_size);
        m_ptr += _cpy_size;
        return _cpy_size;
    }
    FString m_file_name;
    char* m_data;
    uint m_ptr;
    uint m_size;
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
#endif
