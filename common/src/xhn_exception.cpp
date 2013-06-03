//
//  xhn_exception.cpp
//  ecg
//
//  Created by 徐 海宁 on 13-5-3.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#include "xhn_exception.hpp"

ImplementRootRTTI(Exception);
ImplementRTTI(FunctionException, Exception);
ImplementRTTI(FunctionArgumentException, FunctionException);
ImplementRTTI(FunctionExecutionException, FunctionException);
ImplementRTTI(FunctionResultException, FunctionException);
ImplementRTTI(ObjectException, Exception);
ImplementRTTI(ObjectNameAlreadyExistedException, ObjectException);
ImplementRTTI(ObjectUninitializedException, ObjectException);
ImplementRTTI(InvalidEnumerationException, FunctionExecutionException);
