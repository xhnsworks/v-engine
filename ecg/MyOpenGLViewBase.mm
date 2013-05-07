

#pragma mark -
#pragma mark Headers


#import "MyOpenGLViewBase.h"

#pragma mark -

@interface MyOpenGLViewBase(Private)

- (void) render;

@end

#pragma mark -

@implementation MyOpenGLViewBase

- (void) scene
{
} 

- (void) render
{    
    [[self openGLContext] makeCurrentContext];
    {
        [self scene];
    }
	[[self openGLContext] flushBuffer];
} 

- (void) cleanup
{
	if( mpTimer )
	{
		[mpTimer invalidate];
		[mpTimer release];
	} 
    
	if( mpOptions )
	{
		[mpOptions release];
	} 
} 


- (void) dealloc
{
    [self cleanup];
    
	[super dealloc];
} 

- (id) initWithFrame:(NSRect)frameRect
{
	NSOpenGLPixelFormatAttribute attribs[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		0
	};
	
	NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    
	self = [super initWithFrame:frameRect
					pixelFormat:format];
	
	if( self )
	{
        mpTimer = [[NSTimer timerWithTimeInterval:1.0/60.0
                        target:self
                        selector:@selector(render)
                        userInfo:self
                        repeats:true] retain];
        
        if( mpTimer )
        {
            [[NSRunLoop currentRunLoop] addTimer:mpTimer
                forMode:NSRunLoopCommonModes];
        } 

        mpOptions  = [[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES]
                        forKey:NSFullScreenModeSetting] retain];
	} 
	
	if( format )
	{
		[format release];
	} 
	
	if( ![self openGLContext] )
	{
        [self cleanup];
		exit(-1);
	} 
	
    return self;
} 


- (void) prepareOpenGL
{
	[super prepareOpenGL];
	
} 

- (BOOL) isOpaque
{
    return YES;
} 

- (BOOL) acceptsFirstResponder
{
    return YES;
} 

- (void) setFullscreen:(const BOOL)theMode
{
	if( theMode )
	{
		[self enterFullScreenMode:[NSScreen mainScreen]
                      withOptions:mpOptions];
	} 
	else
	{
		[self exitFullScreenModeWithOptions:mpOptions];
	} 
} 



@end
