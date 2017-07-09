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
#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/Color.hpp"

namespace sky {

Viewport Viewport::create(const char* caption, const uint16_t width,
                      const uint16_t height)
{
    Viewport view;
    view.caption_ = caption;
    view.width_ = width;
    view.height_ = height;

    AssertGuard assert_guard("Creating window with caption", caption);

    SKY_ASSERT(Platform::is_initialized(), "Platform is uninitialized");

    view.create_native_viewport();
    view.set_backing_color(Color::gray);

    return view;
}


}