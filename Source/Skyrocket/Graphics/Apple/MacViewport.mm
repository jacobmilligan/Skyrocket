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

#include "Skyrocket/Core/Config.hpp"
#include "Skyrocket/Graphics/Color.hpp"
#include "Skyrocket/Graphics/GDI/GDI.hpp"
#include "Skyrocket/Graphics/Apple/MacViewport.h"

#if SKY_GRAPHICS_API_METAL

#include "MetalView.h"

#endif

namespace sky {


Viewport::Viewport() = default;

Viewport::~Viewport()
{
    close();
}

void Viewport::create_native_viewport()
{
    handle_ = std::make_unique<NativeViewport>();

    auto* window = (CocoaWindow * )Platform::create_native_window(caption_, width_, height_);
    NSRect frame = [window frame];
    CocoaView * view = [[[CocoaView alloc] initWithFrame:frame] autorelease];

#if SKY_GRAPHICS_API_METAL
    view = [[[MetalView alloc] initWithFrame:frame] autorelease];
#endif

    [window setContentView:view];

    handle_->view = view;
    handle_->window = window;
}

void Viewport::destroy_native_viewport()
{

}

void Viewport::set_backing_color(const sky::Color& color)
{
    [handle_->view setBackingColor:((CGFloat) color.r) / 255.0
                                 g:((CGFloat) color.g) / 255.0
                                 b:((CGFloat) color.b) / 255.0
                                 a:((CGFloat) color.a) / 255.0];
}

NativeViewport* Viewport::get_native_viewport()
{
    return handle_.get();
}


}
