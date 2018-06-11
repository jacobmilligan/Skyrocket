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

enum class MipMapFilter : uint_least32_t {
    none,
    linear,
    nearest,
    unknown
};

enum class MinMagFilter : uint_least32_t {
    linear,
    nearest,
    unknown
};

enum class TextureWrapMode : uint_least32_t {
    clamp_to_zero,
    clamp_to_edge,
    mirrored_clamp_to_edge,
    repeat,
    mirrored_repeat,
    unknown
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

struct SamplerStateDescriptor {
    MipMapFilter mip_map_filter;
    MinMagFilter min_filter;
    MinMagFilter mag_filter;
    TextureWrapMode texture_wrap_mode;

    uint32_t get_hash() const;
};

uint32_t pf_bytes_per_pixel(const PixelFormat& format);


}  // namespace sky