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
#include "Skyrocket/Core/Containers/Buffer.hpp"

#include <memory>
#include <queue>

namespace sky {

class Path;

class GDI {
public:
    static constexpr uint32_t invalid_handle = 0;
    static constexpr uint16_t vertex_buffer_max = 1024;
    static constexpr uint16_t index_buffer_max = 1024;
    static constexpr uint16_t shader_max = 512;
    static constexpr uint16_t max_frames_in_flight = 2;

    GDI() = default;

    virtual ~GDI() = default;

    template <typename T>
    void enqueue_command(T& cmd)
    {
        auto is_base = std::is_base_of<rc::Command, T>::value;
        SKY_ASSERT(is_base, "Command is derived from RenderCommand");

        auto header = static_cast<rc::Command&>(cmd).type;
        cmdbufs[cur_buf].write(&header);
        cmdbufs[cur_buf].write(&cmd);
    }

    virtual bool initialize(Viewport* viewport);

    virtual void set_viewport(Viewport* viewport);

    virtual bool create_vertex_buffer(const uint32_t vbuf_id, const MemoryBlock& initial_data,
                                      const BufferUsage usage);

    virtual bool set_vertex_buffer(const uint32_t vbuf_id);

    virtual bool create_shader(const uint32_t shader_id, const char* name);

    virtual bool set_shaders(const uint32_t vertex_id, const uint32_t fragment_id);

    void swap_buffers();

    virtual void present() {}

protected:
    uint16_t cur_buf {0};
    uint16_t prev_buf {0};
    Buffer<UINT16_MAX> cmdbufs[max_frames_in_flight];

    void process_commands();
};

std::unique_ptr<GDI> create_graphics_device_interface();


} // namespace sky