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
    SECTION("test_flags_pass")
    {
        const auto mip = sky::SamplerState::mip_linear;
        const auto min = sky::SamplerState::min_nearest;
        const auto mag = sky::SamplerState::mag_nearest;
        const auto texwrap = sky::SamplerState::texture_wrap_mirrored_repeat;

        auto ss = sky::SamplerStateDescriptor::decode_flags(
            static_cast<uint32_t>(mip | min | mag | texwrap)
        );

        auto decoded = ss.min_filter == min && ss.mag_filter == mag && ss.mip_map_filter == mip
            && ss.texture_wrap_mode == texwrap;

        REQUIRE(decoded);
    }
}