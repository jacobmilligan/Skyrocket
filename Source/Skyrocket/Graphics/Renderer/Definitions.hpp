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

#include <Skyrocket/Core/Config.hpp>

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

SKY_FLAGS(RenderPipelineState, uint32_t) {
    culling_none        = (1 << 0),
    culling_backface    = (1 << 1),
    culling_frontface   = (1 << 2)
};

enum class SamplerState {
    mip_none                    = (1 << 0),
    mip_nearest                 = (1 << 1),
    mip_linear                  = (1 << 2),
    mip_nearest_nearest         = (1 << 3),
    mip_nearest_linear          = (1 << 4),
    mip_linear_nearest          = (1 << 5),
    mip_linear_linear           = (1 << 6),
    tex_wrap_clamp_to_edge      = (1 << 7),
    tex_wrap_repeat             = (1 << 8),
    tex_wrap_mirrored_repeat    = (1 << 9)
};

enum class MinMagFilter {
    nearest,
    linear
};

enum class WrapMode {
    clamp_to_edge,
    repeat,
    mirrored_repeat
};

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

uint32_t pf_bytes_per_pixel(const PixelFormat& format);


}  // namespace sky