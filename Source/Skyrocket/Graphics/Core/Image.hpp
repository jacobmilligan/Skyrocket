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
    int32_t width {0};
    int32_t height {0};
    int32_t bytes_per_pixel {0};
};


} // namespace sky