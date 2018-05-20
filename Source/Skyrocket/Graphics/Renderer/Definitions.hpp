//
//  Definitions.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 5/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>

#include "Skyrocket/Core/Config.hpp"
#include "Skyrocket/Core/Diagnostics/Error.hpp"

class SKY_ENUM_FLAG_OPERATOR;
namespace sky {


enum class BufferUsage {
    none = 0x0,
    staticbuf,
    dynamic
};

enum class UniformType {
    vec1,
    vec2,
    vec3,
    vec4,
    mat2,
    mat3,
    mat4,
    tex1d,
    tex2d,
    tex3d,
    cubemap,
    unknown
};

SKY_FLAGS(PipelineStateFlags, uint32_t) {
    culling_none        = (1 << 0),
    culling_backface    = (1 << 1),
    culling_frontface   = (1 << 2)
};

/// SamplerStateFlags are used to determine how a sampler is used. The bits in the sampler states
/// flags are arranged like so:
/// - bits 0-2: mip map filter
/// - bits 3-4: min/mag filter
/// - bits 5-9: texture wrap/address mode
SKY_FLAGS(SamplerStateFlags, uint32_t) {
    mip_none                                = (1 << 0),
    mip_nearest                             = (1 << 1),
    mip_linear                              = (1 << 2),
    min_mag_nearest                         = (1 << 3),
    min_mag_linear                          = (1 << 4),
    texture_wrap_clamp_to_zero              = (1 << 5),
    texture_wrap_clamp_to_edge              = (1 << 6),
    texture_wrap_mirrored_clamp_to_edge     = (1 << 7),
    texture_wrap_repeat                     = (1 << 8),
    texture_wrap_mirrored_repeat            = (1 << 9),
    unknown
};

static constexpr auto sampler_state_mip_mask =
    static_cast<uint32_t>(SamplerStateFlags::mip_linear)
        | static_cast<uint32_t>(SamplerStateFlags::mip_nearest)
        | static_cast<uint32_t>(SamplerStateFlags::mip_none);

static constexpr auto sampler_state_min_mag_mask =
    static_cast<uint32_t>(SamplerStateFlags::min_mag_nearest)
        | static_cast<uint32_t>(SamplerStateFlags::min_mag_linear);

static constexpr auto sampler_state_texture_wrap_mask =
    static_cast<uint32_t>(SamplerStateFlags::texture_wrap_clamp_to_edge)
        | static_cast<uint32_t>(SamplerStateFlags::texture_wrap_clamp_to_zero)
        | static_cast<uint32_t>(SamplerStateFlags::texture_wrap_mirrored_clamp_to_edge)
        | static_cast<uint32_t>(SamplerStateFlags::texture_wrap_mirrored_repeat)
        | static_cast<uint32_t>(SamplerStateFlags::texture_wrap_repeat);

SKY_FLAGS(PixelFormat, uint32_t) {
    r8,
    r16,
    r32,
    rg8,
    rg16,
    rg32,
    rgb8,
    bgra8,
    rgba8,
    rgba16,
    rgba32,
    depth,
    stencil,
    unknown
};

struct SamplerStateDescriptor {
    SamplerStateFlags mip_map_filter;
    SamplerStateFlags min_mag_filer;
    SamplerStateFlags texture_wrap_mode;

    /// Takes an integer containing flags and decodes it into a SamplerStateDescriptor
    /// @param flags
    /// @return
    static SamplerStateDescriptor decode_flags(const uint32_t flags);
};

uint32_t pf_bytes_per_pixel(const PixelFormat& format);


}  // namespace sky