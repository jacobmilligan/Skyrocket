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
#include "Skyrocket/Graphics/GDI/Commands.hpp"
#include "Skyrocket/Graphics/GDI/Definitions.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"

#include <memory>
#include <queue>

namespace sky {

class Path;

class GDI {
public:
    static constexpr uint16_t vertex_buffer_max = 1024;
    static constexpr uint16_t index_buffer_max = 1024;
    static constexpr uint16_t shader_max = 512;
    static constexpr uint16_t invalid_handle = 0;

    GDI() = default;

    virtual ~GDI() = default;

    void enqueue_command(const RenderCommand& cmd)
    {
        cmds_.push(cmd);
    }

    virtual bool initialize(Viewport* viewport)
    {
        return false;
    }

    virtual void set_viewport(Viewport* viewport) {}

    virtual bool create_vertex_buffer(const uint32_t vbuf_id, const MemoryBlock& initial_data,
                                      const BufferUsage usage)
    {
        return invalid_handle;
    }

    virtual bool set_vertex_buffer(const uint32_t vbuf_id)
    {
        return false;
    }

    virtual bool create_shader(const uint32_t shader_id, const char* name)
    {
        return invalid_handle;
    };

    virtual bool set_shaders(const uint32_t vertex_id, const uint32_t fragment_id)
    {
        return false;
    }


    virtual void present() {}

protected:
    std::queue<RenderCommand> cmds_;
};

std::unique_ptr<GDI> create_graphics_device_interface();


} // namespace sky