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

#include "Skyrocket/Core/Containers/Buffer.hpp"
#include "Skyrocket/Core/Memory.hpp"
#include "Skyrocket/Graphics/GDI/Commands.hpp"
#include "Skyrocket/Graphics/GDI/Definitions.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"

#include <memory>
#include <queue>

namespace sky {

class Path;

struct DrawTarget {
    DrawTarget()
    {
        reset();
    }

    void reset()
    {
        vertex_buffer = 0;
        vertex_count = 0;
        vertex_offset = 0;
        index_count = 0;
        index_buffer = 0;
        index_offset = 0;
    }

    uint32_t vertex_buffer{0};
    uint32_t vertex_count{0};
    uint32_t vertex_offset{0};
    uint32_t index_buffer{0};
    uint32_t index_count{0};
    uint32_t index_offset{0};
};

class GDI {
public:
    static constexpr uint32_t invalid_handle = 0;
    static constexpr uint16_t vertex_buffer_max = 1024;
    static constexpr uint16_t index_buffer_max = 1024;
    static constexpr uint16_t uniform_buffer_max = 512;
    static constexpr uint16_t shader_max = 512;
    static constexpr uint16_t max_frames_in_flight = 3;

    GDI() = default;

    virtual ~GDI() = default;

    static std::unique_ptr<GDI> create();

    template <typename T>
    void write_command(T* cmd);

    virtual bool initialize(Viewport* viewport);

    virtual void set_viewport(Viewport* viewport);

    virtual bool create_vertex_buffer(const uint32_t vbuf_id, const MemoryBlock& initial_data,
                                      const BufferUsage usage);

    virtual bool set_vertex_buffer(const uint32_t vbuf_id);

    virtual bool create_index_buffer(const uint32_t ibuf_id, const MemoryBlock& initial_data);

    virtual bool set_index_buffer(const uint32_t ibuf_id);

    virtual bool draw_primitives();

    virtual bool create_shader(const uint32_t shader_id, const char* name);

    virtual bool set_shaders(const uint32_t vertex_id, const uint32_t fragment_id);

    virtual bool create_uniform(const uint32_t u_id, const uint32_t size);

    virtual void set_uniform(const uint32_t u_id, const uint32_t index);

    virtual void update_uniform(const uint32_t u_id, const MemoryBlock& data);

    virtual void present();

    void flip()
    {
        cmdbufs_.flip();
    }

    uint16_t frames_in_flight()
    {
        return static_cast<uint16_t>(cmdbufs_.read_index() + 1);
    }

protected:
    CommandBuffer<UINT16_MAX, max_frames_in_flight> cmdbufs_;

    DrawTarget target_;

    void process_commands();
};


} // namespace sky


#include "Skyrocket/Graphics/GDI/Internal/GDI.inl"
