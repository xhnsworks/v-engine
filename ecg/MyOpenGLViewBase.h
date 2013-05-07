#import <Cocoa/Cocoa.h>


@interface MyOpenGLViewBase : NSOpenGLView
{
@private
    NSDictionary  *mpOptions;
	NSTimer       *mpTimer;
} 

- (void) cleanup;

- (void) scene;

- (void) setFullscreen:(const BOOL)theMode;


@end
