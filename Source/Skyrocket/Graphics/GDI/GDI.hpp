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
#include "Skyrocket/Core/Memory/Memory.hpp"
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
    static constexpr uint16_t texture_max = 512;
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
    template<typename T>
    void write_command(T* cmd);

    /// @brief Initializes the graphics device, allocating resources and creating a
    /// device context
    /// @param viewport The viewport to attach the graphics device to
    /// @return Successful initialization if true, false otherwise
    virtual bool initialize(Viewport* viewport);

    /// @brief Sets the viewport as the active viewport for this graphics device
    /// @param viewport
    virtual void set_viewport(Viewport* viewport);

    /// @brief Creates a new vertex buffer
    /// @param vbuf_id The id handle of the buffer to create
    /// @param initial_data The data to copy into the buffer
    /// @param usage
    /// @return Success if true, false otherwise
    virtual bool create_vertex_buffer(const uint32_t vbuf_id, const MemoryBlock& initial_data,
                                      const BufferUsage usage);

    /// @brief Sets the vertex buffer associated with the given id as the current one
    /// @param vbuf_id
    /// @return Success if true, false otherwise
    virtual bool set_vertex_buffer(const uint32_t vbuf_id);

    /// @brief Creates a new index buffer
    /// @param ibuf_id The id handle of the buffer to create
    /// @param initial_data
    /// @return
    virtual bool create_index_buffer(const uint32_t ibuf_id, const MemoryBlock& initial_data);

    /// @brief Sets the index buffer associated with the given id as the current one
    /// @param ibuf_id
    /// @return Success if true, false otherwise
    virtual bool set_index_buffer(const uint32_t ibuf_id);

    /// @brief Draws primitive triangle data
    /// @return
    virtual bool draw();

    /// @brief Draws primitive triangle data
    /// @return
    virtual bool draw_instanced(const uint32_t instance);

    /// @brief Creates a new shader
    /// @param program_id
    /// @param name
    /// @return
    virtual bool create_program(const uint32_t program_id, const Path& vs_path, const Path& frag_path);

    virtual bool set_program(const uint32_t program_id);

    virtual bool create_uniform(const uint32_t u_id, const uint32_t size);

    virtual void set_uniform(const uint32_t u_id, const uint32_t index);

    virtual void update_uniform(const uint32_t u_id, const MemoryBlock& data, const uint32_t offset);

    virtual void create_texture(const uint32_t t_id, const uint32_t width,
                                const uint32_t height, const PixelFormat::Enum pixel_format,
                                const bool mipmapped);

    virtual void create_texture_region(const uint32_t tex_id, const UIntRect& region,
                                       const PixelFormat::Enum pixel_format, uint8_t* data);

    virtual void set_texture(const uint32_t t_id, const uint32_t index);

    virtual void set_state(const uint32_t flags);

    /// @brief Commits the current command buffer and processes all commands
    virtual void commit();

    /// @brief Flips the internal command buffers
    void flip()
    {
        cmdbufs_.flip();
    }

protected:
    Multibuffer<UINT16_MAX * 10, max_frames_in_flight> cmdbufs_;

    RenderState target_;

    /// @brief Empties the current read command buffer of all commands, calling the
    /// relevant member functions
    void process_commands();
};


} // namespace sky


#include "GDI.inl"
