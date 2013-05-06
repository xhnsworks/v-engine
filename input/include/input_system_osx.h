//
//  input_system_osx.h
//  ecg
//
//  Created by 徐 海宁 on 13-5-1.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#ifndef __ecg__input_system_osx__
#define __ecg__input_system_osx__
#include "common.h"
#include "etypes.h"
#include "xhn_vector.hpp"
API_EXPORT void input_Init(vptr view);
API_EXPORT void input_Proc(xhn::vector<input_buffer>& input_buffers);

#endif /* defined(__ecg__input_system_osx__) */
