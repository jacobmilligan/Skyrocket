//
//  Image.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 27/09/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Platform/Filesystem.hpp"

#include <cstdint>

namespace sky {


struct Image {
    ~Image();

    void load_from_file(const Path& path);

    uint8_t* data {nullptr};
    uint32_t width {0};
    uint32_t height {0};
    PixelFormat::Enum pixel_format;
};


} // namespace sky