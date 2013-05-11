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

#include "input_system_osx.h"
#include "input_robot.h"
#include "animation.hpp"
#include "robot_thread.h"
#include "sprite_event_hub.h"
#include "input_system_osx.h"

#include "elog.h"

#import <Cocoa/Cocoa.h>

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
    timer += deltaTime;
    fps += 1.0f;
    if (timer > 1.0) {
        timer = 0.0;
        printf("fps %f\n", fps);
        fps = 0.0f;
    }
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
    
    timer = .0;
    fps = 0;
    
    /// create render robot
    MInit();
    ELog_Init();
    ShaderLog_Init();
    input_Init();
    
    rwbuffer = get_rwbuffer();
    RobotManager::Init();
    RobotThreadManager::Init();
    SpriteEventHub::Init();
    renderRobot = RobotManager::Get()->AddRobot<RenderRobot>();
    RobotManager::Get()->AddRobot<InputRobot, vptr>((vptr)self);
    RobotManager::Get()->AddRobot<AnimationRobot>();
    RobotThreadManager::Get()->AddRobotThread();
    
    RobotManager::Get()->Remove(renderRobot->GetName());
    return self;
}

- (void)awakeFromNib
{
    NSSize    viewBounds = [self bounds].size;
    viewWidth = viewBounds.width;
    viewHeight = viewBounds.height;
    
    [[self window] makeFirstResponder: self];
    [[self window] setAcceptsMouseMovedEvents: YES];
    if ([[self window] acceptsMouseMovedEvents]) {NSLog(@"window now acceptsMouseMovedEvents");}
    
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
    
    ///glViewport(0, 0, viewWidth, viewHeight);
    // draw here
    renderRobot->RunOnce();
    
    [currentContext flushBuffer];
    CGLUnlockContext((CGLContextObj)[currentContext CGLContextObj]);
}

- (BOOL) acceptsFirstResponder
{
    return YES;
}

- (void)mouseMoved:(NSEvent *)pEvent
{
    NSPoint event_location = [pEvent locationInWindow];
    NSPoint local_point = [self convertPoint:event_location fromView:nil];
    printf("pt %f %f\n", local_point.x, local_point.y);
    input_event evt;
    evt.type = MouseAbsolutePositionEvent;
    evt.info.mouse_info.mouse_abs_pos.x = (int)(local_point.x);
    evt.info.mouse_info.mouse_abs_pos.y = (int)(viewHeight - local_point.y);
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
    
    float dx = [pEvent deltaX];
    float dy = [pEvent deltaY];
    printf("delta %f %f\n", dx, dy);
    evt.type = MouseMoveEvent;
    evt.info.mouse_info.mouse_move_info.x = (int)(dx);
    evt.info.mouse_info.mouse_move_info.y = (int)(dy);
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void)mouseDown:(NSEvent *)pEvent
{
    input_event evt;
    evt.type = MouseButtonDownEvent;
    evt.info.mouse_info.mouse_button_info = LeftButton;
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void)rightMouseDown:(NSEvent *)pEvent
{
    input_event evt;
    evt.type = MouseButtonDownEvent;
    evt.info.mouse_info.mouse_button_info = RightButton;
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void)otherMouseDown:(NSEvent *)pEvent
{
    input_event evt;
    evt.type = MouseButtonDownEvent;
    evt.info.mouse_info.mouse_button_info = MiddleButton;
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void)mouseUp:(NSEvent *)pEvent
{
    input_event evt;
    evt.type = MouseButtonUpEvent;
    evt.info.mouse_info.mouse_button_info = LeftButton;
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void)rightMouseUp:(NSEvent *)pEvent
{
    input_event evt;
    evt.type = MouseButtonUpEvent;
    evt.info.mouse_info.mouse_button_info = RightButton;
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void)otherMouseUp:(NSEvent *)pEvent
{
    input_event evt;
    evt.type = MouseButtonUpEvent;
    evt.info.mouse_info.mouse_button_info = MiddleButton;
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void)mouseDragged:(NSEvent *)pEvent
{
    input_event evt;
    float dx = [pEvent deltaX];
    float dy = [pEvent deltaY];
    printf("delta %f %f\n", dx, dy);
    evt.type = MouseMoveEvent;
    evt.info.mouse_info.mouse_move_info.x = (int)(dx);
    evt.info.mouse_info.mouse_move_info.y = (int)(dy);
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void)rightMouseDragged:(NSEvent *)pEvent
{
    input_event evt;
    float dx = [pEvent deltaX];
    float dy = [pEvent deltaY];
    printf("delta %f %f\n", dx, dy);
    evt.type = MouseMoveEvent;
    evt.info.mouse_info.mouse_move_info.x = (int)(dx);
    evt.info.mouse_info.mouse_move_info.y = (int)(dy);
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void)otherMouseDragged:(NSEvent *)pEvent
{
    input_event evt;
    float dx = [pEvent deltaX];
    float dy = [pEvent deltaY];
    printf("delta %f %f\n", dx, dy);
    evt.type = MouseMoveEvent;
    evt.info.mouse_info.mouse_move_info.x = (int)(dx);
    evt.info.mouse_info.mouse_move_info.y = (int)(dy);
    evt.time_stamp = 0;
    RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
}
- (void) keyDown:(NSEvent *)pEvent
{
    NSEventType type = [pEvent type];
    if (NSKeyDown == type) {
        unsigned short key = [pEvent keyCode];
        printf("key %d\n", key);
    }
}
- (void)flagsChanged:(NSEvent *)pEvent
{
    NSUInteger flags = [pEvent modifierFlags];
    printf("flagsChanged %lx\n", flags);
    if (flags & NSCommandKeyMask) {
        input_event evt;
        evt.type = KeyDownEvent;
        evt.info.key_info = 56;
        evt.time_stamp = 0;
        RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
    }
    else {
        input_event evt;
        evt.type = KeyUpEvent;
        evt.info.key_info = 56;
        evt.time_stamp = 0;
        RWBuffer_Write(rwbuffer, (const euint*)&evt, sizeof(evt));
    }
}
@end
