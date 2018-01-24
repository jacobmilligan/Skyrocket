//
//  GDI.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Geometry/Rectangle.hpp"
#include "Skyrocket/Graphics/GDI/Definitions.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"

namespace sky {

class Path;

namespace experimental {

class CommandBuffer;

class GDI_EX {
public:
    GDI_EX() = default;

    virtual ~GDI_EX() = default;

    /// @brief Creates a new API-specific GDI. A GDI cannot be created without calling
    /// this method
    /// @return Unique pointer to the GDI
    static std::unique_ptr<GDI_EX> create() noexcept;

    /// @brief Initializes the graphics device, allocating resources and creating a
    /// device context
    /// @param viewport The viewport to attach the graphics device to
    /// @return Successful initialization if true, false otherwise
    virtual bool init(Viewport* viewport);

    void process_command_buffer(CommandBuffer* cmdbuf);
protected:
    /// @brief Sets the viewport as the active viewport for this graphics device
    /// @param viewport
    virtual void set_viewport(Viewport* viewport);

    /// @brief Creates a new vertex buffer
    /// @param vbuf_id The id handle of the buffer to create
    /// @param initial_data The data to copy into the buffer
    /// @param usage
    /// @return Success if true, false otherwise
    virtual bool create_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& initial_data,
                                      BufferUsage usage);

    /// @brief Sets the vertex buffer associated with the given id as the current one
    /// @param vbuf_id
    /// @return Success if true, false otherwise
    virtual bool set_vertex_buffer(uint32_t vbuf_id);

    /// @brief Creates a new index buffer
    /// @param ibuf_id The id handle of the buffer to create
    /// @param initial_data
    /// @return
    virtual bool create_index_buffer(uint32_t ibuf_id, const MemoryBlock& initial_data);

    /// @brief Sets the index buffer associated with the given id as the current one
    /// @param ibuf_id
    /// @return Success if true, false otherwise
    virtual bool set_index_buffer(uint32_t ibuf_id);

    /// @brief Draws primitive triangle data
    /// @return
    virtual bool draw();

    /// @brief Draws primitive triangle data
    /// @return
    virtual bool draw_instanced(uint32_t instance);

    /// @brief Creates a new shader
    /// @param program_id
    /// @param name
    /// @return
    virtual bool create_program(uint32_t program_id, const Path& vs_path, const Path& frag_path);

    virtual bool set_program(uint32_t program_id);

    virtual bool create_uniform(uint32_t u_id, uint32_t size);

    virtual void set_uniform(uint32_t u_id, uint32_t index);

    virtual void update_uniform(uint32_t u_id, const MemoryBlock& data, uint32_t offset);

    virtual void create_texture(uint32_t t_id, uint32_t width,
                                uint32_t height, PixelFormat::Enum pixel_format,
                                bool mipmapped);

    virtual void create_texture_region(uint32_t tex_id, const UIntRect& region,
                                       PixelFormat::Enum pixel_format, uint8_t* data);

    virtual void set_texture(uint32_t t_id, uint32_t index);

    virtual void set_state(uint32_t flags);
};


} // namespace experimental
} // namespace sky