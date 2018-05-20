//
//  DefinitionsTest.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 20/05/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Graphics/Renderer/Definitions.hpp>

#include "catch/catch.hpp"

TEST_CASE("test_sampler_state_flags", "[sampler_state]")
{
    const auto test_flags = [](sky::SamplerStateFlags mip, sky::SamplerStateFlags minmag,
                               sky::SamplerStateFlags texwrap) -> bool
    {
        auto flags = mip | minmag | texwrap;
        auto sampler_state = sky::SamplerStateDescriptor::decode_flags(static_cast<uint32_t>(flags));

        return sampler_state.mip_map_filter == mip && sampler_state.min_mag_filer == minmag
            && sampler_state.texture_wrap_mode == texwrap;
    };

    SECTION("test_flags_pass")
    {
        auto flags_decoded = test_flags(sky::SamplerStateFlags::mip_linear,
            sky::SamplerStateFlags::min_mag_nearest,
            sky::SamplerStateFlags::texture_wrap_mirrored_repeat);
        REQUIRE(flags_decoded);
    }
}