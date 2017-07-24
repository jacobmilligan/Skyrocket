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
#include "Skyrocket/Graphics/Viewport.hpp"

#include <memory>

namespace sky {


class GDI {
public:
    static constexpr uint32_t vertex_buffer_max = 1024;
    static constexpr uint32_t index_buffer_max = 1024;
    static constexpr uint32_t shader_max = 512;
    static constexpr uint32_t invalid_handle = 0;

    GDI() = default;

    virtual ~GDI() = default;

    virtual bool initialize()
    {
        return false;
    }

    virtual void set_viewport(Viewport*  /*viewport*/) {};

    virtual uint32_t create_vertex_buffer(const MemoryBlock&  /*initial_data*/,
                                          const BufferUsage  /*usage*/)
    {
        return invalid_handle;
    }

    virtual uint32_t create_shader(const char*  /*path*/)
    {
        return invalid_handle;
    };

    virtual void present() {}
};

std::unique_ptr<GDI> create_graphics_device_interface();


} // namespace sky