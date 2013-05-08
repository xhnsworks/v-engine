#import <Cocoa/Cocoa.h>

#import "MyOpenGLViewBase.h"

#include "render_robot.h"

@interface MyOpenGLView : MyOpenGLViewBase
{
@private
	BOOL       mbFullscreen;
	NSPoint    m_MousePt;
    GLuint     mnSelector;
    RenderRobot* renderRobot;
} 

- (IBAction) toggleFullscreen:(id)sender;

@end
