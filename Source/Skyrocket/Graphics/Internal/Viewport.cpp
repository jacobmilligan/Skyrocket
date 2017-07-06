//
//  Viewport.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Platform/Platform.hpp"

namespace sky {


Viewport::Viewport(const char* caption, const uint16_t width,
                   const uint16_t height)
    : caption_(caption), width_(width), height_(height)
{
    handle_ = Platform::create_window(caption, width, height);
    set_backing_color(Color::gray);
}

Viewport
Viewport::create(const char* caption, const uint16_t width, const uint16_t height)
{
    return Viewport(caption, width, height);
}

void Viewport::set_backing_color(const Color& color)
{
    Platform::set_view_backing_color(handle_, color);
}


}
