//
//  MyView.h
//  ecg
//
//  Created by 徐 海宁 on 13-4-29.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#ifndef ecg_MyView_h
#define ecg_MyView_h

//codefragmentbegin,myView_include
#import <Cocoa/Cocoa.h>
/**
@interface MyView : NSView
{
@private
    NSOpenGLContext *_context;
    NSOpenGLPixelFormat* _pixelformat;
}

- (NSOpenGLContext*) openGLContext;
- (void) prepareOpenGL;

@end
**/
// for display link
#import <QuartzCore/QuartzCore.h>
#include "render_robot.h"

@interface MyView : NSOpenGLView
{
    CVDisplayLinkRef displayLink;
    
    double    deltaTime;
    double    outputTime;
    float    viewWidth;
    float    viewHeight;
    
    RenderRobot* renderRobot;
}

@end
#endif
