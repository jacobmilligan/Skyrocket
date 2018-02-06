//
//  ContextNSGL.mm
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Renderer/OpenGL/GLContext.hpp"
#include "Skyrocket/Core/Config.hpp"
#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/Apple/CocoaView.h"

#import <Cocoa/Cocoa.h>
#import <Skyrocket/Graphics/Apple/MacViewport.h>

namespace sky {


void GLContext::create(Viewport* viewport)
{
//    auto version = glGetString(GL_VERSION);
//    printf("OpenGL version: %s\n", version);
    NSOpenGLPixelFormatAttribute attributes[] = {
        NSOpenGLPFAOpenGLProfile,   NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAColorSize,       24,
        NSOpenGLPFAAlphaSize,       8,
        NSOpenGLPFADepthSize,       24,
        NSOpenGLPFAStencilSize,     8,
        NSOpenGLPFADoubleBuffer,    YES,
        NSOpenGLPFAAccelerated,     YES,
        NSOpenGLPFANoRecovery,      YES,
        0,                          0,
    };

    auto pixel_format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    SKY_ASSERT(pixel_format != nil, "NSOpenGLPixelFormat successfully created with the specified "
        "attributes");

    // TODO (Jacob): add sharing of contexts
    auto context = [[NSOpenGLContext alloc] initWithFormat: pixel_format
                                              shareContext:nil];

    SKY_ASSERT(context != nil, "NSOpenGLContext created successfully with the specified pixel "
        "format");

    glcontext_ = static_cast<void*>(context);

    [context makeCurrentContext];
    [context setView:viewport->get_native_handle()->view];

    SKY_OBJC_RELEASE(pixel_format);
}

void GLContext::destroy()
{
    auto context = (NSOpenGLContext*)glcontext_;
    [context release];
}

void GLContext::make_current()
{
    auto context = (NSOpenGLContext*)glcontext_;
    [context makeCurrentContext];
}

void GLContext::set_size(uint32_t width, uint32_t height)
{

}

void GLContext::swap_buffers()
{
    auto context = (NSOpenGLContext*)glcontext_;
    [context flushBuffer];
}

void GLContext::set_swap_interval(const int interval)
{
    GLint sync = interval;
    auto context = (NSOpenGLContext*)glcontext_;
    [context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
}



}