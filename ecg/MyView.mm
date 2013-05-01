//
//  MyView.m
//  ecg
//
//  Created by 徐 海宁 on 13-4-29.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

//codefragmentbegin,myView_source
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#import "MyView.h"
#include "emem.h"
#include "elog.h"
#include "shader_log.h"


@interface MyView (InternalMethods)

- (CVReturn)getFrameForTime:(const CVTimeStamp *)outputTime;
- (void)drawFrame;

@end

@implementation MyView

#pragma mark -
#pragma mark Display Link

static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *now,
                                      const CVTimeStamp *outputTime, CVOptionFlags flagsIn,
                                      CVOptionFlags *flagsOut, void *displayLinkContext)
{
    // go back to Obj-C for easy access to instance variables
    CVReturn result = [(MyView *)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (CVReturn)getFrameForTime:(const CVTimeStamp *)outputTime
{
    // deltaTime is unused in this bare bones demo, but here's how to calculate it using display link info
    deltaTime = 1.0 / (outputTime->rateScalar * (double)outputTime->videoTimeScale / (double)outputTime->videoRefreshPeriod);
    
    [self drawFrame];
    
    return kCVReturnSuccess;
}

- (void)dealloc
{
    CVDisplayLinkRelease(displayLink);
    
    [super dealloc];
}

- (id)initWithFrame:(NSRect)frameRect
{
    // context setup
    NSOpenGLPixelFormat        *windowedPixelFormat;
    NSOpenGLPixelFormatAttribute    attribs[] = {
        NSOpenGLPFAWindow,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFASingleRenderer,
        0 };
    
    windowedPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    if (windowedPixelFormat == nil)
    {
        NSLog(@"Unable to create windowed pixel format.");
        exit(0);
    }
    self = [super initWithFrame:frameRect pixelFormat:windowedPixelFormat];
    if (self == nil)
    {
        NSLog(@"Unable to create a windowed OpenGL context.");
        exit(0);
    }
    [windowedPixelFormat release];
    
    // set synch to VBL to eliminate tearing
    GLint    vblSynch = 1;
    [[self openGLContext] setValues:&vblSynch forParameter:NSOpenGLCPSwapInterval];
    
    // set up the display link
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink, MyDisplayLinkCallback, self);
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    /// create render robot
    MInit();
    ELog_Init();
    ShaderLog_Init();
    renderRobot = ENEW RenderRobot;
    
    return self;
}

- (void)awakeFromNib
{
    NSSize    viewBounds = [self bounds].size;
    viewWidth = viewBounds.width;
    viewHeight = viewBounds.height;
    
    // activate the display link
    CVDisplayLinkStart(displayLink);
}

- (void)reshape
{
    NSSize    viewBounds = [self bounds].size;
    viewWidth = viewBounds.width;
    viewHeight = viewBounds.height;
    
    NSOpenGLContext    *currentContext = [self openGLContext];
    [currentContext makeCurrentContext];
    
    // remember to lock the context before we touch it since display link is threaded
    CGLLockContext((CGLContextObj)[currentContext CGLContextObj]);
    
    // let the context know we've changed size
    [[self openGLContext] update];
    
    CGLUnlockContext((CGLContextObj)[currentContext CGLContextObj]);
}

- (void)drawRect:(NSRect)rect
{
    [self drawFrame];
}

- (void)drawFrame
{
    NSOpenGLContext    *currentContext = [self openGLContext];
    [currentContext makeCurrentContext];
    
    // must lock GL context because display link is threaded
    CGLLockContext((CGLContextObj)[currentContext CGLContextObj]);
    
    glViewport(0, 0, viewWidth, viewHeight);
    /**
    // Draw something that changes over time to prove to yourself that it's really updating in a tight loop
    glClearColor(
                 sin(CFAbsoluteTimeGetCurrent()),
                 sin(7.0*CFAbsoluteTimeGetCurrent()),
                 sin(CFAbsoluteTimeGetCurrent()/3.0),0);
    glClear(GL_COLOR_BUFFER_BIT);
    **/
    // draw here
    renderRobot->RunOnce();
    
    [currentContext flushBuffer];
    
    CGLUnlockContext((CGLContextObj)[currentContext CGLContextObj]);
}

@end
