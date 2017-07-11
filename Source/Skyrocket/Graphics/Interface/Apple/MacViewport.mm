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
#include "Skyrocket/Platform/Platform.hpp"

#if SKY_GRAPHICS_API_METAL

#include "Skyrocket/Graphics/Interface/Apple/Metal/MetalView.h"

#endif

namespace sky {

struct Viewport::NativeViewport {
    CocoaWindow* window;
    CocoaView* view;
};


Viewport::destroy_native_viewport()
{
    if ( handle_ ) {
        delete handle_;
    }
}

void Viewport::create_native_viewport()
{
    handle_ = new NativeViewport;
    
    CocoaWindow* window = (CocoaWindow*)Platform::new_native_window(caption_, width_, height_);
    NSRect frame = [window frame];
    CocoaView* view = [[[CocoaView alloc] initWithFrame:frame] autorelease];
    
#if SKY_GRAPHICS_API_METAL
    view = [[[MetalView alloc] initWithFrame:frame] autorelease];
#endif
    
    [window setContentView:view];
    
    handle_ = new NativeViewport;
    handle_->view = view;
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
