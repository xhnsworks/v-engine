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

@implementation MyView

-( id )initWithFrame : ( NSRect )frameRect
{
    NSLog( @"myView::initWithFrame" );
    
    if( ( self = [ super initWithFrame:frameRect ] ) != nil )
    {
        GLuint attributes[] =
        {
            NSOpenGLPFAWindow,         // choose among pixelformats capable of rendering to windows
            NSOpenGLPFAAccelerated,    // require hardware-accelerated pixelformat
            NSOpenGLPFADoubleBuffer,   // require double-buffered pixelformat
            NSOpenGLPFAColorSize, 24,  // require 24 bits for color-channels
            NSOpenGLPFAAlphaSize, 8,   // require an 8-bit alpha channel
            NSOpenGLPFADepthSize, 24,  // require a 24-bit depth buffer
            NSOpenGLPFAMinimumPolicy,  // select a pixelformat which meets or exceeds these requirements
            0
        };
        
        _pixelformat = [[ NSOpenGLPixelFormat alloc ] initWithAttributes: ( NSOpenGLPixelFormatAttribute * ) attributes ];
        
        if( _pixelformat == nil )
        {
            NSLog( @"No valid OpenGL pixel format matching attributes specified" );
            // at this point, we'd want to try different sets of pixelformat attributes until we
            // got a match, or decided we couldn't create a proper working environment for our
            // application
        }
    }
    
    // init the context for later construction
    _context = nil;
    
    return( self );
}

-( NSOpenGLContext * ) openGLContext
{
    if( _context == nil ) // check to see that we've not already been intialized
    {
        // if this is our first time to intialize
        _context = [[ NSOpenGLContext alloc ] initWithFormat: _pixelformat shareContext: nil ];
        
        if( _context == nil )
        {
            NSLog( @"No valid OpenGL context can be created with that pixelformat" );
            //
            // we can fail a few ways:
            // 1 - bogus parameters: nil pixelformat, invalid sharecontext, etc.
            // 2 - share context uses a different Renderer than the specified pixelformat
             
            // recovery techniques:
            // 1 - choose a different pixelformat
            // 2 -proceed without a shared context
            // 
        }
    }
    
    return( _context );
}

-( void ) lockFocus
{
    NSLog( @"myView::lockFocus" );
    
    // ensure we are ready to draw
    [ super lockFocus ];
    
    // get our context
    NSOpenGLContext * cxt = [ self openGLContext ];
    
    // ensure we are pointing to ourself as the Drawable
    if([ cxt view ] != self )
    {
        [ cxt setView : self ];
    }
    
    // make us the current OpenGL context
    [ cxt makeCurrentContext ];
}

-( void ) prepareOpenGL
{
    NSLog( @"myView::prepareOpenGL" );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -1, 1, -1, 1, -1, 100 );
}

-( void ) drawRect : ( NSRect ) rect
{
    NSRect frame = [ self frame ];
    NSRect bounds = [ self bounds ]; // NSView
    NSLog( @"myView::lockFocus ff %f %f %f %f\n", frame.origin.x, frame.origin.y, frame.size.width, frame.size.height );
    NSLog( @"myView::lockFocus bb %f %f %f %f\n", bounds.origin.x, bounds.origin.y, bounds.size.width, frame.size.height );
    NSLog( @"myView::lockFocus rr %f %f %f %f\n", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height );
    
    // adjust viewing parameters
    glViewport( 0, 0, ( GLsizei ) rect.size.width, ( GLsizei ) rect.size.height );
    
    glClearColor( 0, .5, .8, 0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glTranslatef( 0, 0, -1 );
    
    GLfloat green[ 4 ] = { 0, 1, 0, 0 };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green );
    glutSolidTeapot( .5 );
    
    [[ self openGLContext ] flushBuffer ];
}

@end

//codefragmentend,myView_source
