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

#include <cstdint>


namespace sky {

class Platform;

class Viewport {
public:
    Viewport() {}

    static Viewport create(const char* caption, const uint16_t width, const uint16_t height);

private:
    Viewport(const char* caption, const uint16_t width, const uint16_t height);

    const char* caption_;
    uint16_t width_;
    uint16_t height_;

    void* handle_;
};


}