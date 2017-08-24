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
#include <Skyrocket/Platform/Thread.hpp>

namespace sky {

class Viewport;

struct MemoryBlock;

/// @brief A GraphicsDriver controls the creation of render commands and synchronization between
/// the main thread and the render thread.
class GraphicsDriver {
public:
    enum class ThreadSupport {
        single_thread,
        multithreaded
    };

    /// @brief Creates a new graphics driver
    /// @param threading Either single or multithreaded
    explicit GraphicsDriver(const ThreadSupport threading);
    ~GraphicsDriver();

    /// @brief Initializes the graphics driver and render thread, creating the native graphics API
    /// context
    /// @param view The view to associate the graphics device with
    /// @return True if successful initialization, false otherwise
    bool initialize(Viewport& view);

    /// @brief Attaches this graphics driver to the specified viewport
    /// @param viewport
    void set_viewport(Viewport& viewport);

    /// @brief Sends a command to create a new vertex buffer resource
    /// @param initial_data The initial data to copy into the buffer
    /// @param usage
    /// @return Handle id to the vertex buffer resource
    uint32_t create_vertex_buffer(const MemoryBlock& initial_data,
                                  const BufferUsage usage);

    /// @brief Sets the vertex buffer associated with the specified id as the current one
    /// @param vbuf_id
    /// @param offset
    /// @param num_vertices
    void set_vertex_buffer(const uint32_t vbuf_id, const uint32_t offset,
                           const uint32_t num_vertices);

    /// @brief Sends a command to create a new index buffer resource
    /// @param initial_data The initial data to copy into the buffer
    /// @return Handle id to the index buffer resource
    uint32_t create_index_buffer(const MemoryBlock& initial_data);

    /// @brief Sets the index buffer associated with the specified id as the current one
    /// @param ibuf_id
    /// @param offset
    /// @param num_indices
    void set_index_buffer(const uint32_t ibuf_id, const uint32_t offset,
                          const uint32_t num_indices);

    /// @brief Sends a command to create a new shader uniform resource
    /// @param type The type of structure or data associated with the uniform
    /// @param count The number of instances of `type` associated with the uniform
    /// @return Handle id to the shader uniform resource
    uint32_t create_uniform(const UniformType type, const uint16_t count);

    /// @brief Sets the uniform associated with the given ID as the current one
    /// @param u_id
    /// @param index Index to assign to the uniform as referenced in the shader
    void set_uniform(const uint32_t u_id, const uint32_t index);

    /// @brief Sends a command to update the data associated with a shader uniform
    /// @param u_id
    /// @param data
    void update_uniform(const uint32_t u_id, const MemoryBlock& data);

    /// @brief Sends a command to create a new shader
    /// @param name
    /// @return Handle ID to the shader
    uint32_t create_shader(const char* name);

    /// @brief Sets the vertex and fragment shaders as the current ones
    /// @param vertex_id
    /// @param fragment_id
    /// @return
    bool set_shaders(const uint32_t vertex_id, const uint32_t fragment_id);

    /// @brief Sends a command to draw triangle primitives
    void draw_primitives();

    /// @brief Commits the current command buffer and syncs up the render and main threads
    /// before swapping internal buffers, kicking the render thread and processing all commands
    void present();

private:
    ThreadSupport threading_;
    uint32_t next_vbuf_id_;
    uint32_t next_ibuf_id_;
    uint32_t next_shader_id_;
    uint32_t next_uniform_id_;

    std::unique_ptr<GDI> gdi_;

    bool notified_;
    bool active_;
    std::condition_variable cv_;
    std::thread render_thread_;

    Timespan dt_;
    Semaphore sem;

    void kick_render_thread();
    void frame();
};


} // namespace sky