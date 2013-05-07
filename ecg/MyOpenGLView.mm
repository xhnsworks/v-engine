

#pragma mark -
#pragma mark Headers

#import "MyOpenGLView.h"

#pragma mark -
#pragma mark Enumerated Types

#pragma mark -

@interface MyOpenGLView(Private)

- (NSPoint) mousePoint:(NSEvent *)pEvent;

- (void) terminate:(NSNotification *)notification;

- (void) initOpenGL;

@end

#pragma mark -

@implementation MyOpenGLView

- (void) scene
{
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth ( 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
} 

- (void) terminate:(NSNotification *)notification
{
	[self  cleanup];
} 

- (id) initWithFrame:(NSRect)frameRect
{
	self = [super initWithFrame:frameRect];
    
    if( self )
    {
        mbFullscreen = NO;
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(terminate:)
                                                     name:@"NSApplicationWillTerminateNotification"
                                                   object:NSApp];
    }

    return self;
}

- (void) cleanup
{
    [super cleanup];
}

- (void) dealloc
{
    [self cleanup];
    
	[super dealloc];
}

- (BOOL) isOpaque
{
    return YES;
}

- (BOOL) acceptsFirstResponder
{
    return YES;
}

- (void) initOpenGL
{
    GLint nSyncVR = GL_TRUE;
    
	[[self openGLContext] setValues:&nSyncVR
					   forParameter:NSOpenGLCPSwapInterval];
} 

- (void) prepareOpenGL
{
	[super prepareOpenGL];
	[self initOpenGL];
}

- (void) reshape
{
    [super reshape];
    
    NSRect bounds = [self bounds];
    
    GLsizei nWidth  = GLsizei(bounds.size.width);
    GLsizei nHeight = GLsizei(bounds.size.height);
	
    glViewport(0, 0, nWidth, nHeight);

} 

- (IBAction) toggleFullscreen:(id)sender
{
	mbFullscreen = !mbFullscreen;
	
    [self setFullscreen:mbFullscreen];
} 

- (void) keyDown:(NSEvent *)pEvent
{
} 

- (NSPoint) mousePoint:(NSEvent *)pEvent
{
    NSPoint point = [pEvent locationInWindow];
    
    return [self convertPoint:point
					 fromView:nil];
} 

- (void) mouseDown:(NSEvent *)pEvent
{
} 
- (void) rightMouseDown:(NSEvent *)pEvent
{
} 

- (void) mouseDragged:(NSEvent *)pEvent
{
} 

- (void) rightMouseDragged:(NSEvent *)pEvent
{
} 

@end
