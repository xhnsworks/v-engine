#import <Cocoa/Cocoa.h>

#import "MyOpenGLViewBase.h"

@interface MyOpenGLView : MyOpenGLViewBase
{
@private
	BOOL       mbFullscreen;
	NSPoint    m_MousePt;
    GLuint     mnSelector;
} 

- (IBAction) toggleFullscreen:(id)sender;

@end
