//
//  Window.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/Color.hpp"

#include <cstdint>

namespace sky {

class Platform;

class Viewport {
public:
    friend class GraphicsDriver;

    Viewport() {}

    static Viewport create(const char* caption, const uint16_t width,
                           const uint16_t height);

    void set_backing_color(const Color& color);
private:
    Viewport(const char* caption, const uint16_t width, const uint16_t height);

    const char* caption_;
    uint16_t width_;
    uint16_t height_;

    void* handle_;
};


}