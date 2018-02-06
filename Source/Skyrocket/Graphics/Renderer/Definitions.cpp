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


uint32_t PixelFormat::bytes_per_pixel(const Enum& format)
{
    switch (format) {
        case Enum::r8:
        case Enum::stencil:
            return 1;
        case Enum::r16:
        case Enum::rg8:
        case Enum::depth:
            return 2;
        case Enum::r32:
        case Enum::rg16:
        case Enum::bgra8:
        case Enum::rgba8:
            return 4;
        case Enum::rg32:
        case Enum::rgba16:
            return 8;
        case Enum::rgb8:
            return 3;
        case Enum::rgba32:
            return 16;
        default:
            return 0;
    }
}


} // namespace sky