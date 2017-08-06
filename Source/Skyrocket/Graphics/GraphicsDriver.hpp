//
//  GraphicsDriver.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 25/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/GDI/Definitions.hpp"
#include "Skyrocket/Graphics/GDI/GDI.hpp"
#include "Skyrocket/Core/Diagnostics/Timespan.hpp"

#include <memory>
#include <thread>

namespace sky {

class Viewport;
struct MemoryBlock;

class GraphicsDriver {
public:
    enum class ThreadSupport {
        single_thread,
        multithreaded
    };

    explicit GraphicsDriver(const ThreadSupport threading);
    ~GraphicsDriver();

    bool initialize(Viewport& view);

    void set_viewport(Viewport& viewport);

    uint32_t create_vertex_buffer(const MemoryBlock& initial_data,
                                  const BufferUsage usage);

    void set_vertex_buffer(const uint32_t vbuf_id, const uint32_t offset,
                           const uint32_t num_vertices);

    uint32_t create_index_buffer(const MemoryBlock& initial_data);

    void set_index_buffer(const uint32_t ibuf_id, const uint32_t offset,
                          const uint32_t num_indices);

    uint32_t create_shader(const char* name);

    bool set_shaders(const uint32_t vertex_id, const uint32_t fragment_id);

    void draw_primitives();

    void present(const float target_dt = 2.0f);
private:
    ThreadSupport threading_;
    uint32_t next_vbuf_id_;
    uint32_t next_ibuf_id_;
    uint32_t next_shader_id_;

    std::unique_ptr<GDI> gdi_;

    bool notified_;
    bool active_;
    bool rendering_;
    std::condition_variable cv_;
    std::thread render_thread_;

    Timespan dt_;

    void wait_for_render_finish();
    void kick_render_thread();
    void frame();
};


} // namespace sky