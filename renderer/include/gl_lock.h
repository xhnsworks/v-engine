//
//  gl_lock.h
//  ecg
//
//  Created by 徐 海宁 on 13-6-4.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#ifndef ecg_gl_lock_h
#define ecg_gl_lock_h

class GLLock
{
private:
    static GLLock* s_GLLock;
public:
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
    static void Init(GLLock* lock);
    static GLLock* Get();
};

#endif
