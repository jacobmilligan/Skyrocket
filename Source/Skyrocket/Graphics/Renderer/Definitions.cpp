//
//  Definitions.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 28/10/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Renderer/Definitions.hpp"

namespace sky {


uint32_t pf_bytes_per_pixel(const PixelFormat& format)
{
    switch (format) {
        case PixelFormat::r8:
        case PixelFormat::stencil:
            return 1;
        case PixelFormat::r16:
        case PixelFormat::rg8:
        case PixelFormat::depth:
            return 2;
        case PixelFormat::r32:
        case PixelFormat::rg16:
        case PixelFormat::bgra8:
        case PixelFormat::rgba8:
            return 4;
        case PixelFormat::rg32:
        case PixelFormat::rgba16:
            return 8;
        case PixelFormat::rgb8:
            return 3;
        case PixelFormat::rgba32:
            return 16;
        default:
            return 0;
    }
}


} // namespace sky