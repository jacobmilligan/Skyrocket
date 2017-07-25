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


struct Vertex {
    Vector4f position;
    Vector4f color;
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


