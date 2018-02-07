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
#include "Skyrocket/Graphics/Renderer/GDI.hpp"
#include "Skyrocket/Graphics/Apple/MacViewport.h"
#include "Skyrocket/Graphics/Renderer/Renderer.hpp"

#if SKY_GRAPHICS_API_METAL == 1
#include "Skyrocket/Graphics/Renderer/Metal/MetalView.h"
#endif

#if SKY_GRAPHICS_API_OPENGL == 1
#include "Skyrocket/Graphics/Renderer/OpenGL/NSGLView.h"
#endif

namespace sky {

void Renderer::set_vsync_enabled(bool enabled)
{
    vsync_on_ = enabled;
    auto render_proc = &Renderer::vsync_notify;
    if (threadsupport_ == ThreadSupport::multi_threaded) {
        render_proc = &Renderer::render_thread_notify;
    }

    [viewport_->get_native_handle()->view setVsyncEnabled:enabled
                                             graphicsDriver:this
                                              frameCallback:render_proc];
}


Viewport::Viewport() = default;

Viewport::~Viewport()
{
    [handle_->view setVsyncEnabled:NO graphicsDriver:nullptr frameCallback:nullptr];
    close();
}

void Viewport::create_native_viewport(const Renderer& renderer)
{
    handle_ = std::make_unique<NativeHandle>();

    auto* window = (CocoaWindow*)Platform::create_native_window(caption_, width_, height_);
    NSRect frame = [window frame];


    CocoaView* view;

    if (renderer.active_backend() == RendererBackend::Metal) {
        view = [[[MetalView alloc] initWithFrame:frame] autorelease];
    } else {
        view = [[[NSGLView alloc] initWithFrame:frame] autorelease];
    }

    [window setContentView:view];

    handle_->view = view;
    handle_->window = window;
}

void Viewport::destroy_native_viewport()
{
    [handle_->window close];
    [handle_->window release];
    [handle_->view release];
}

void Viewport::set_backing_color(const sky::Color& color)
{
    [handle_->view setBackingColor:((CGFloat) color.r) / 255.0
                                 g:((CGFloat) color.g) / 255.0
                                 b:((CGFloat) color.b) / 255.0
                                 a:((CGFloat) color.a) / 255.0];
}

NativeHandle* Viewport::get_native_handle()
{
    return handle_.get();
}


}
