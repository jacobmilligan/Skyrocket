//
//  MacViewport.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 9/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/Config.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/Color.hpp"
#include "Skyrocket/Platform/macOS/CocoaWindow.h"

#if SKY_GRAPHICS_API_METAL

#include "Skyrocket/Graphics/Interface/Apple/Metal/MetalView.h"

#endif

namespace sky {

struct Viewport::NativeViewport {
    CocoaWindow* window;

#if SKY_GRAPHICS_API_METAL
    MetalView* view;
#else
    CocoaView* view;
#endif

};


Viewport::~Viewport()
{
    if ( handle_ ) {
        delete handle_;
    }
}

void Viewport::create_native_viewport()
{
    handle_ = new NativeViewport;
    NSRect frame = NSMakeRect(0, 0, width_, height_);
    CocoaWindow* window = [[CocoaWindow alloc] initWithSizeAndCaption:frame captionString:caption_];
    
    handle_ = new NativeViewport;
    
#if SKY_GRAPHICS_API_METAL
    handle_->view = [[[MetalView alloc] initWithFrame:frame] autorelease];
#else
    handle_->view = [[[CocoaView alloc] initWithFrame:frame] autorelease];
#endif
    
    [window setContentView:handle_->view];
    
    handle_->window = window;
}
    
void Viewport::set_backing_color(const sky::Color &color)
{
    [handle_->view setBackingColor:((CGFloat)color.r) / 255.0
                                 g:((CGFloat)color.g) / 255.0
                                 b:((CGFloat)color.b) / 255.0
                                 a:((CGFloat)color.a) / 255.0];
}
    
    
}
