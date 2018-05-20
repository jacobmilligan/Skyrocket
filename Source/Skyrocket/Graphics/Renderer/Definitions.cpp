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

#if SKY_DEBUG == 1

#include <string>

#endif

namespace sky {

SamplerStateDescriptor SamplerStateDescriptor::decode_flags(const uint32_t flags)
{
    SKY_ASSERT_GUARD(ag, "Decoding SamplerState flag", std::to_string(flags).c_str());

    const auto mip = flags & sampler_state_mip_mask;
    const auto minmag = flags & sampler_state_min_mag_mask;
    const auto texwrap = flags & sampler_state_texture_wrap_mask;

    SKY_ASSERT(mip != sampler_state_mip_mask,
               "Exactly one mipmap filter flag was specified");
    SKY_ASSERT(minmag != sampler_state_min_mag_mask,
               "Exactly one min/mag filter flag was specified");
    SKY_ASSERT(texwrap != sampler_state_texture_wrap_mask,
               "Exactly one texture wrap mode flag was specified");

    SKY_ASSERT(mip != 0, "A valid mipmap filter flag was specified");
    SKY_ASSERT(minmag != 0, "A valid min/mag filter flag was specified");
    SKY_ASSERT(texwrap != 0, "A valid texture wrap mode flag was specified");

    return SamplerStateDescriptor {
        static_cast<SamplerStateFlags>(mip),
        static_cast<SamplerStateFlags>(minmag),
        static_cast<SamplerStateFlags>(texwrap)
    };
}


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