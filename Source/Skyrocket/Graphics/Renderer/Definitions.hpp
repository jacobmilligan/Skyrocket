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

enum RenderPipelineState {
    culling_none = 0x0,
    culling_backface,
    culling_frontface
};

struct PixelFormat {
    enum Enum {
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

    static uint32_t bytes_per_pixel(const Enum& format);
};


}  // namespace sky