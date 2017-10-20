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

namespace sky {


enum class BufferUsage {
    none = 0x0,
    staticbuf,
    dynamic
};

enum class UniformType {
    vec2 = 0x0,
    vec3,
    vec4,
    mat2,
    mat3,
    mat4
};

enum RenderPipelineState {
    culling_none = 0x0,
    culling_backface,
    culling_frontface
};


}  // namespace sky