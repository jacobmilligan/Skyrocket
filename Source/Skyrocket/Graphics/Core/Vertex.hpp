//
//  PackedVertex.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Math/Vector4.hpp"

#include <cstdint>

namespace sky {

struct Float4 {
    float x, y, z, w;
};

struct Vertex {
    Vertex(const float x, const float y, const float z, const float w,
           const float r, const float g, const float b, const float a)
        : position(Float4 { x, y, z, w }), color(Float4 { r, g, b, a })
    {}

    Float4 position;
    Float4 color;
};

/// A normal vector packed to fit into a 32bit value
struct Normal {
    union {
        struct {
            uint8_t x;
            uint8_t y;
            uint8_t z;
            uint8_t w;
        } packed_vector;
        uint32_t packed_value;
    };
};


}  // namespace sky


