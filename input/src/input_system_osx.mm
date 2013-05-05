//
//  input_system_osx.cpp
//  ecg
//
//  Created by 徐 海宁 on 13-5-1.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#include "input_system_osx.h"
#include "rwbuffer.h"
static RWBuffer g_event_buffer = NULL;
static float g_mouse_x = 0.0f;
static float g_mouse_y = 0.0f;

#include <AppKit/NSWorkspace.h>
#include <ApplicationServices/ApplicationServices.h>
#import <Foundation/NSAutoreleasePool.h>
#import <Foundation/NSDistributedNotificationCenter.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSString.h>
#include <stdio.h>

#define QUASIMODE_KEY kCGEventFlagMaskAlternate

#ifdef DEBUG
#define DEBUG_MSG( msg ) printf( msg );
#else
#define DEBUG_MSG( msg )
#endif

NSDistributedNotificationCenter *center;

CGKeyCode lastQuasimodalKeyCode;
CGEventFlags lastQuasimodalKeyFlags;
int numQuasimodalKeyDowns = 0;

static BOOL inQuasimode = NO;

static void sendSomeKeyEvent( void )
{
    NSArray *keys = [NSArray arrayWithObjects: @"event", nil];
    NSArray *values = [NSArray arrayWithObjects: @"someKey", nil];
    NSDictionary *dict = [NSDictionary dictionaryWithObjects: values forKeys: keys];
    
    [center postNotificationName: @"EnsoKeyNotifier_msg"
                          object: @"EnsoKeyNotifier"
                        userInfo: dict];
}

CGEventRef processEvent( CGEventTapProxy proxy,
                        CGEventType type,
                        CGEventRef event,
                        void *refcon )
{
    //int64_t keycode = CGEventGetIntegerValueField(
    //    event,
    //    kCGKeyboardEventKeycode
    //    );
    
    BOOL passOnEvent = !inQuasimode;
    
    if ( type == kCGEventFlagsChanged )
    {
        CGEventFlags flags = CGEventGetFlags( event );
        
        if ( inQuasimode )
        {
            if ( !(flags & QUASIMODE_KEY) )
            {
                NSArray *keys = [NSArray arrayWithObjects: @"event", nil];
                NSArray *values = [NSArray arrayWithObjects: @"quasimodeEnd", nil];
                NSDictionary *dict = [NSDictionary dictionaryWithObjects: values forKeys: keys];
                
                [center postNotificationName: @"EnsoKeyNotifier_msg"
                                      object: @"EnsoKeyNotifier"
                                    userInfo: dict];
                inQuasimode = NO;
                if ( numQuasimodalKeyDowns == 1 )
                {
                    CGEventRef event[2];
                    
                    DEBUG_MSG( "Re-posting single keypress\n" );
                    
                    event[0] = CGEventCreateKeyboardEvent(
                                                          NULL,
                                                          (CGKeyCode) lastQuasimodalKeyCode,
                                                          true
                                                          );
                    
                    event[1] = CGEventCreateKeyboardEvent(
                                                          NULL,
                                                          (CGKeyCode) lastQuasimodalKeyCode,
                                                          false
                                                          );
                    
                    CGEventSetFlags( event[0], lastQuasimodalKeyFlags );
                    CGEventSetFlags( event[1], lastQuasimodalKeyFlags );
                    
                    CGEventTapPostEvent( proxy, event[0] );
                    CGEventTapPostEvent( proxy, event[1] );
                    
                    CFRelease( event[0] );
                    CFRelease( event[1] );
                }
                DEBUG_MSG( "Exit quasimode\n" );
            }
        } else {
            if ( flags & QUASIMODE_KEY )
            {
                NSArray *keys = [NSArray arrayWithObjects: @"event", nil];
                NSArray *values = [NSArray arrayWithObjects: @"quasimodeStart", nil];
                NSDictionary *dict = [NSDictionary dictionaryWithObjects: values forKeys: keys];
                
                [center postNotificationName: @"EnsoKeyNotifier_msg"
                                      object: @"EnsoKeyNotifier"
                                    userInfo: dict];
                inQuasimode = YES;
                passOnEvent = NO;
                numQuasimodalKeyDowns = 0;
                DEBUG_MSG( "Enter quasimode\n" );
            } else {
                sendSomeKeyEvent();
            }
        }
    } else {
        /* Key up/down event */
        
        if ( inQuasimode )
        {
#define MAX_STR_LEN 10
            
            UniChar strbuf[MAX_STR_LEN];
            UniCharCount charsCopied;
            
            CGEventKeyboardGetUnicodeString(
                                            event,
                                            MAX_STR_LEN,
                                            &charsCopied,
                                            strbuf
                                            );
            
            NSString *chars = [NSString stringWithCharacters: strbuf
                                                      length: charsCopied];
            NSString *eventType;
            
            int64_t keycode = CGEventGetIntegerValueField(
                                                          event,
                                                          kCGKeyboardEventKeycode
                                                          );
            
            if ( type == kCGEventKeyDown ) {
                numQuasimodalKeyDowns += 1;
                lastQuasimodalKeyCode = keycode;
                lastQuasimodalKeyFlags = CGEventGetFlags( event );
                eventType = @"keyDown";
            } else
                eventType = @"keyUp";
            
            NSNumber *keycodeNum = [NSNumber numberWithUnsignedInt: keycode];
            
            NSArray *keys = [NSArray arrayWithObjects: @"event", @"chars", @"keycode", nil];
            NSArray *values = [NSArray arrayWithObjects: eventType, chars, keycodeNum, nil];
            NSDictionary *dict = [NSDictionary dictionaryWithObjects: values forKeys: keys];
            
            [center postNotificationName: @"EnsoKeyNotifier_msg"
                                  object: @"EnsoKeyNotifier"
                                userInfo: dict];
        } else {
            sendSomeKeyEvent();
        }
    }
    
    if ( passOnEvent )
        return event;
    else
        return NULL;
}

CGEventRef myCallback( CGEventTapProxy proxy,
                      CGEventType type,
                      CGEventRef event,
                      void *refcon )
{
    /**
    ///NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    CGEventRef retval;
    NSString *bundleId = [[[NSWorkspace sharedWorkspace] activeApplication] objectForKey: @"NSApplicationBundleIdentifier"];
    
    if (bundleId &&
        [bundleId isEqualToString: @"com.blizzard.worldofwarcraft"])
    {
        retval = event;
    } else {
        retval = processEvent(proxy, type, event, refcon);
    }
    
    ///[pool release];
    **/
    ///return retval;
    return event;
}

int initx()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    center = [NSDistributedNotificationCenter defaultCenter];
    
    CGEventMask mask = ( CGEventMaskBit( kCGEventKeyDown ) |
                        CGEventMaskBit( kCGEventKeyUp ) |
                        CGEventMaskBit( kCGEventFlagsChanged ) );
    
    CFMachPortRef portRef = CGEventTapCreate(
                                             //kCGSessionEventTap,
                                             kCGHIDEventTap,
                                             kCGHeadInsertEventTap,
                                             0,
                                             mask,
                                             myCallback,
                                             NULL
                                             );
    
    CFRunLoopSourceRef rlSrcRef;
    
    if ( portRef == NULL )
    {
        printf( "CGEventTapCreate() failed.\n" );
        return -1;
    }
    
    rlSrcRef = CFMachPortCreateRunLoopSource(
                                             kCFAllocatorDefault,
                                             portRef,
                                             0
                                             );
    
    CFRunLoopAddSource(
                       CFRunLoopGetCurrent(),
                       rlSrcRef,
                       kCFRunLoopDefaultMode
                       );
    
    ///printf( "Greetings from %s.\n", argv[0] );
    printf( "Please make sure either of the following is true:\n\n"
           "  (1) This program is running with super-user privileges.\n"
           "  (2) Access for assistive devices is enabled in the \n"
           "      Universal Access System Preferences.\n\n"
           "If one or more of these conditions is not satisfied, then "
           "quasimodal keypresses will not be recognized.\n" );
    
    printf( "Running event loop...\n" );
    
    //CFRunLoopRunInMode( kCFRunLoopDefaultMode, 10.0, false );
    CFRunLoopRun();
    
    printf( "Done running event loop.\n" );
    
    CFRunLoopRemoveSource(
                          CFRunLoopGetCurrent(),
                          rlSrcRef,
                          kCFRunLoopDefaultMode
                          );
    
    CFRelease( rlSrcRef );
    CFRelease( portRef );
    
    [pool release];
    
    return 0;
}

static CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (kCGEventLeftMouseDown == type) {
        input_event evt;
        evt.type = MouseButtonDownEvent;
        evt.info.mouse_info.mouse_button_info = LeftButton;
        evt.time_stamp = 0;
        RWBuffer_Write(g_event_buffer, (const euint*)&evt, sizeof(evt));
    }
    if (kCGEventRightMouseDown == type) {
        input_event evt;
        evt.type = MouseButtonDownEvent;
        evt.info.mouse_info.mouse_button_info = RightButton;
        evt.time_stamp = 0;
        RWBuffer_Write(g_event_buffer, (const euint*)&evt, sizeof(evt));
    }
    if (kCGEventLeftMouseUp == type) {
        input_event evt;
        evt.type = MouseButtonUpEvent;
        evt.info.mouse_info.mouse_button_info = LeftButton;
        evt.time_stamp = 0;
        RWBuffer_Write(g_event_buffer, (const euint*)&evt, sizeof(evt));
    }
    if (kCGEventRightMouseUp == type) {
        input_event evt;
        evt.type = MouseButtonUpEvent;
        evt.info.mouse_info.mouse_button_info = RightButton;
        evt.time_stamp = 0;
        RWBuffer_Write(g_event_buffer, (const euint*)&evt, sizeof(evt));
    }
    if (kCGEventMouseMoved == type) {
        CGPoint location = CGEventGetLocation(event);
        input_event evt;
        evt.type = MouseAbsolutePositionEvent;
        evt.info.mouse_info.mouse_abs_pos.x = (int)location.x;
        evt.info.mouse_info.mouse_abs_pos.y = (int)location.y;
        evt.time_stamp = 0;
        RWBuffer_Write(g_event_buffer, (const euint*)&evt, sizeof(evt));
        
        evt.type = MouseMoveEvent;
        evt.info.mouse_info.mouse_move_info.x = (int)(location.x - g_mouse_x);
        evt.info.mouse_info.mouse_move_info.y = (int)(location.y - g_mouse_y);
        evt.time_stamp = 0;
        RWBuffer_Write(g_event_buffer, (const euint*)&evt, sizeof(evt));
        
        NSLog(@"pos  %f %f", location.x, location.y);
    }
    if (kCGEventKeyDown == type) {
        printf("here\n");
    }
    if (kCGEventFlagsChanged == type) {
        
        CGEventFlags flag = CGEventGetFlags(event);
        if (flag & kCGEventFlagMaskCommand) {
            input_event evt;
            evt.type = KeyDownEvent;
            evt.info.key_info = 56;
            evt.time_stamp = 0;
            RWBuffer_Write(g_event_buffer, (const euint*)&evt, sizeof(evt));
        }
        
        ///NSLog(@"inputed string:%@, flags:%lld", inputedString, flag);
    }
    ///CGPoint location = CGEventGetLocation(event);
    ///NSInteger windowNumber = [NSWindow windowNumberAtPoint:location belowWindowWithWindowNumber:0];
    ///CGWindowID windowID = (CGWindowID)windowNumber;
    /**
    CFArrayRef array = CFArrayCreate(NULL, (const void **)&windowID, 1, NULL);
    NSArray *windowInfos = (NSArray *)CGWindowListCreateDescriptionFromArray(array);
    CFRelease(array);
    
    if (windowInfos.count > 0) {
        NSDictionary *windowInfo = [windowInfos objectAtIndex:0];
        NSLog(@"Window name:  %@", [windowInfo objectForKey:(NSString *)kCGWindowName]);
        NSLog(@"Window owner: %@", [windowInfo objectForKey:(NSString *)kCGWindowOwnerName]);
    }
    [windowInfos release];
    **/
    return event;
}

int initxx()
{
    g_event_buffer = RWBuffer_new(sizeof(input_event) * 1024);
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    center = [NSDistributedNotificationCenter defaultCenter];
    
    CGEventMask mask = ( CGEventMaskBit( kCGEventKeyDown ) |
                        CGEventMaskBit( kCGEventKeyUp ) |
                        CGEventMaskBit( kCGEventFlagsChanged ) );
    
    CFMachPortRef portRef = CGEventTapCreate(
                                             //kCGSessionEventTap,
                                             kCGHIDEventTap,
                                             kCGHeadInsertEventTap,
                                             0,
                                             mask,
                                             eventCallback,
                                             NULL
                                             );
    
    CFRunLoopSourceRef rlSrcRef;
    
    if ( portRef == NULL )
    {
        printf( "CGEventTapCreate() failed.\n" );
        return -1;
    }
    
    rlSrcRef = CFMachPortCreateRunLoopSource(
                                             kCFAllocatorDefault,
                                             portRef,
                                             0
                                             );
    
    CFRunLoopAddSource(
                       CFRunLoopGetCurrent(),
                       rlSrcRef,
                       kCFRunLoopDefaultMode
                       );
    
    ///printf( "Greetings from %s.\n", argv[0] );
    printf( "Please make sure either of the following is true:\n\n"
           "  (1) This program is running with super-user privileges.\n"
           "  (2) Access for assistive devices is enabled in the \n"
           "      Universal Access System Preferences.\n\n"
           "If one or more of these conditions is not satisfied, then "
           "quasimodal keypresses will not be recognized.\n" );
    
    printf( "Running event loop...\n" );
    
    //CFRunLoopRunInMode( kCFRunLoopDefaultMode, 10.0, false );
    CFRunLoopRun();
    
    printf( "Done running event loop.\n" );
    
    CFRunLoopRemoveSource(
                          CFRunLoopGetCurrent(),
                          rlSrcRef,
                          kCFRunLoopDefaultMode
                          );
    
    CFRelease( rlSrcRef );
    CFRelease( portRef );
    
    [pool release];
    
    return 0;
}

void input_Init()
{
    CGEventMask mask =
    CGEventMaskBit(kCGEventLeftMouseDown) |
    CGEventMaskBit(kCGEventLeftMouseUp) |
    CGEventMaskBit(kCGEventRightMouseDown) |
    CGEventMaskBit(kCGEventRightMouseUp) |
    CGEventMaskBit(kCGEventMouseMoved);
    /**
    CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp) | CGEventMaskBit(kCGEventFlagsChanged);
    **/
    CFMachPortRef portRef = CGEventTapCreate(
                                             //kCGSessionEventTap,
                                             kCGHIDEventTap,
                                             kCGHeadInsertEventTap,
                                             0,
                                             mask,
                                             eventCallback,
                                             NULL
                                             );
    
    CFRunLoopSourceRef rlSrcRef;
    
    rlSrcRef = CFMachPortCreateRunLoopSource(
                                             kCFAllocatorDefault,
                                             portRef,
                                             0
                                             );
    
    CFRunLoopAddSource(
                       CFRunLoopGetCurrent(),
                       rlSrcRef,
                       kCFRunLoopDefaultMode
                       );
    
    CFRunLoopAddSource(CFRunLoopGetCurrent(), rlSrcRef, kCFRunLoopCommonModes);
    CGEventTapEnable(portRef, true);

    CFRelease( rlSrcRef );
    CFRelease( portRef );
    
    /**
    // Create an event tap.
    CGEventMask eventMask =
    CGEventMaskBit(kCGEventLeftMouseDown) |
    CGEventMaskBit(kCGEventLeftMouseUp) |
    CGEventMaskBit(kCGEventRightMouseDown) |
    CGEventMaskBit(kCGEventRightMouseUp) |
    CGEventMaskBit(kCGEventMouseMoved) |
    CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp) | CGEventMaskBit(kCGEventFlagsChanged);
    
    CFMachPortRef eventTap = CGEventTapCreate(kCGHIDEventTap, kCGHeadInsertEventTap, kCGEventTapOptionListenOnly, eventMask, eventCallback, NULL);
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    CFRelease(eventTap);
    CFRelease(runLoopSource);
    **/
    g_event_buffer = RWBuffer_new(sizeof(input_event) * 1024);
}

void input_Proc(xhn::vector<input_buffer>& input_buffers)
{
    input_event tmp[2];
    euint readSize = 0;
    while (RWBuffer_Read(g_event_buffer, (euint*)tmp, &readSize)) {
        xhn::vector<input_buffer>::iterator iter = input_buffers.begin();
        xhn::vector<input_buffer>::iterator end = input_buffers.end();
        for (; iter != end; iter++) {
            input_buffer& buf = *iter;
            buf.inter_input_buffer.write(buf.input_buffer, (euint*)&tmp[0], sizeof(tmp[0]));
        }
    }
}
