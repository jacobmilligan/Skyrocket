//
//  Viewport.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 9/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"

namespace sky {

uint16_t Viewport::open_windows_ = 0;

void Viewport::open(const Renderer& renderer, const char* caption, const uint16_t width,
                    const uint16_t height)
{
    caption_ = caption;
    width_ = width;
    height_ = height;

    AssertGuard assert_guard("Creating window with caption", caption);

    //SKY_ASSERT(Platform::is_initialized(), "Platform is uninitialized");

    create_native_viewport(renderer);

    is_open_ = true;
    ++open_windows_;
}

void Viewport::close()
{
    is_open_ = false;
    destroy_native_viewport();
    --open_windows_;
}

uint16_t Viewport::open_viewports()
{
    return open_windows_;
}

bool Viewport::close_requested()
{
    return window_data_.close_requested;
}

Vector2f Viewport::size()
{
    return Vector2f(width_, height_);
}


}  // namespace sky