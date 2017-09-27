//
//  GraphicsData.hpp
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


/// @brief A graphics-related vertex
struct GraphicsData {
    GraphicsData(const float x, const float y, const float z, const float w,
           const float r, const float g, const float b, const float a)
        :
        position(Vector4f {x, y, z, w}),
        color(Vector4f {r, g, b, a})
    {}

    Vector4f position;
    Vector4f color;
};

/// @brief A normal vector packed to fit into a 32bit value
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


