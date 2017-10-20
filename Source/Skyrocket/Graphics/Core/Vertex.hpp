//
//  Vertex.hpp
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


/// @brief Packed graphics vertex. 16 byte aligned for Metal support.
struct alignas(16) Vertex {
    Vertex(const float x, const float y, const float z, const float w,
           const float r, const float g, const float b, const float a,
           const float s, const float t)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        position[3] = w;

        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;

        tex_coords[0] = s;
        tex_coords[1] = t;
    }

    float position[4]{};
    float color[4]{};
    float tex_coords[2]{};
};


}  // namespace sky


