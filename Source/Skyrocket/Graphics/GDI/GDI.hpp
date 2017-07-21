//
//  GDI.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 15/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Memory.hpp"
#include "Skyrocket/Graphics/GDI/Definitions.hpp"

#include <memory>

namespace sky {

struct VertexBufferHandle {
    uint16_t id;
};

class GDI {
public:
    static constexpr uint32_t vertex_buffer_max = 1024;
    static constexpr uint32_t index_buffer_max = 1024;

    GDI() {}

    virtual ~GDI() {}

    virtual bool initialize()
    {
        return false;
    }

    virtual VertexBufferHandle create_vertex_buffer(const uint32_t size, const BufferUsage usage,
                                                    const MemoryBlock& initial_data)
    {
        return VertexBufferHandle { 0 };
    }
};

std::unique_ptr<GDI> create_graphics_device_interface();


}