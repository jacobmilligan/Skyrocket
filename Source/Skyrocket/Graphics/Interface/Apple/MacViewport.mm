//
//  MacViewport.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Platform/Platform.hpp"

#include "Skyrocket/Platform/macOS/CocoaView.h"

#import <AppKit/AppKit.h>

namespace sky {
    
struct Viewport::NativeHandle {
    NSWindow* window;
    CocoaView* view;
};

Viewport::~Viewport()
{
    if ( handle_ )
        delete handle_;
}

void Viewport::create(const char *caption, const uint16_t width, const uint16_t height)
{
    caption_ = caption;
    width_ = width;
    height_ = height;

    AssertGuard assert_guard("Creating window with caption", caption);
    
    SKY_ASSERT(Platform::is_initialized(), "Platform is uninitialized");
    
    NSString* nsTitle;
    if ( caption ) {
        nsTitle = [[[NSString alloc] initWithUTF8String:caption] autorelease];
    } else {
        nsTitle = [[[NSString alloc] initWithUTF8String:"Skyrocket Application"] autorelease];
    }
    
    // Create windowrect and associated window with specified style
    NSRect windowRect = NSMakeRect(0, 0, width, height);
    NSUInteger windowStyle = NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask;
    NSWindow* window = [[NSWindow alloc] initWithContentRect:windowRect
                                                   styleMask:windowStyle
                                                     backing:NSBackingStoreBuffered // double buffered rendering
                                                       defer:NO]; // create immediately
    
    [window autorelease];
    [window setTitle:nsTitle];

    handle_ = new NativeHandle;
    
    handle_->view = (CocoaView*)create_handle(caption, width, height);
    
    [window setContentView:handle_->view];
    [window setLevel:NSMainMenuWindowLevel];
    [window makeKeyAndOrderFront:window];
    
    handle_->window = window;
    
    set_backing_color(Color::gray);
}

void Viewport::set_backing_color(const Color &color)
{
    [handle_->view setBackingColor:((CGFloat)color.r) / 255.0
                                 g:((CGFloat)color.g) / 255.0
                                 b:((CGFloat)color.b) / 255.0
                                 a:((CGFloat)color.a) / 255.0];
}
    
    
}
