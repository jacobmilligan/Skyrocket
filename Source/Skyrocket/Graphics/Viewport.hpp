//
//  Viewport.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 9/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>

namespace sky {

struct Color;

class Viewport {
public:
    friend class GraphicsDriver;

    Viewport() {}

    ~Viewport();

    static Viewport create(const char* caption, const uint16_t width,
                           const uint16_t height);

    void set_backing_color(const Color& color);
private:
    void create_native_viewport();

    const char* caption_;
    uint16_t width_;
    uint16_t height_;

    struct NativeViewport;

    NativeViewport* handle_;
};


}