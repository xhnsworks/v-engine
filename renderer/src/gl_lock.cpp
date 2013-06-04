//
//  gl_lock.cpp
//  ecg
//
//  Created by 徐 海宁 on 13-6-4.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#include "gl_lock.h"

GLLock* GLLock::s_GLLock = 0;
void GLLock::Init(GLLock* lock)
{
    s_GLLock = lock;
}
GLLock* GLLock::Get()
{
    return s_GLLock;
}