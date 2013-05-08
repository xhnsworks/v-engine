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
        /**
        NSWindow* win = [ g_view window ];
        NSRect rc;
        rc.origin.x = 0.0f;
        rc.origin.y = 0.0f;
        if (win) {
            rc = [ win frame ];
        }

        input_event evt;
        evt.type = MouseAbsolutePositionEvent;
        evt.info.mouse_info.mouse_abs_pos.x = (int)(location.x - rc.origin.x);
        evt.info.mouse_info.mouse_abs_pos.y = (int)(location.y - rc.origin.y);
        evt.time_stamp = 0;
        RWBuffer_Write(g_event_buffer, (const euint*)&evt, sizeof(evt));
        
        evt.type = MouseMoveEvent;
        evt.info.mouse_info.mouse_move_info.x = (int)(location.x - g_mouse_x);
        evt.info.mouse_info.mouse_move_info.y = (int)(location.y - g_mouse_y);
        evt.time_stamp = 0;
        RWBuffer_Write(g_event_buffer, (const euint*)&evt, sizeof(evt));
        
        NSLog(@"pos  %f %f", location.x, location.y);
         **/
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


void input_Init()
{
    if (g_event_buffer == NULL)
        g_event_buffer = RWBuffer_new(sizeof(input_event) * 1024);
}

RWBuffer get_rwbuffer()
{
    return g_event_buffer;
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
