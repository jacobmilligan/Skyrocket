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

/// @brief Contains the current state of the graphics device - buffers,
/// shaders, indices etc. being used currently
struct RenderState {
    RenderState()
    {
        reset();
    }

    /// @brief Resets all values back to zero
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

/// @brief The Graphics Device Interface (GDI) is the main interface to the native
/// graphics API being currently used - each one with its own GDI deriving from this
/// class with all functions directly calling API-specific operations synchronously.
/// The GDI should only be created via the create() static member function.
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

    /// @brief Creates a new API-specific GDI. A GDI cannot be created without calling
    /// this method
    /// @return Unique pointer to the GDI
    static std::unique_ptr<GDI> create();

    /// @brief Writes a new render command to the internal write buffer
    /// @tparam T The type of render command being written
    /// @param cmd Pointer to the command structure
    template <typename T>
    void write_command(T* cmd);

    /// @brief Initializes the graphics device, allocating resources and creating a
    /// device context
    /// @param viewport The viewport to attach the graphics device to
    /// @return Successful initialization if true, false otherwise
    virtual bool initialize(Viewport* viewport);

    /// @brief Sets the viewport as the active viewport for this graphics device
    /// @param viewport
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

    /// @brief Flips the internal command buffers
    void flip()
    {
        cmdbufs_.flip();
    }

protected:
    Multibuffer<UINT16_MAX, max_frames_in_flight> cmdbufs_;

    RenderState target_;

    /// @brief Empties the current read command buffer of all commands, calling the
    /// relevant member functions
    void process_commands();
};


} // namespace sky


#include "Skyrocket/Graphics/GDI/Internal/GDI.inl"
