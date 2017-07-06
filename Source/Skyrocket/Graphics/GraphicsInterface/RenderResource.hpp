//
//  RenderResource.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 5/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/GraphicsCore/Vertex.hpp"
#include "Skyrocket/Graphics/GraphicsInterface/Definitions.hpp"

#include <cstdint>

namespace sky {


class VertexBuffer {
public:
    VertexBuffer()
        : initialized_(false), size_(0), usage_(BufferUsage::none)
    {}

    static VertexBuffer create(const uint32_t size, const BufferUsage usage);

    bool set_data(Vertex* vertices);
private:
    bool initialized_;

    uint32_t size_;
    BufferUsage usage_;

    VertexBuffer(const uint32_t size, const BufferUsage usage);
};


}