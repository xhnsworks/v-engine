//
//  attribute.cpp
//  ecg
//
//  Created by 徐 海宁 on 13-5-24.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#include "attribute.h"

ImplementRootRTTI(Attribute);
ImplementRTTI(FloatAttr, Attribute);
ImplementRTTI(Float2Attr, FloatAttr);
ImplementRTTI(Float3Attr, Float2Attr);
ImplementRTTI(Float4Attr, Float3Attr);