//
//  Commands.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 25/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Geometry/Rectangle.hpp"
#include "Skyrocket/Graphics/GDI/Definitions.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Platform/Filesystem.hpp"

namespace sky {
namespace rc {


/// @brief List of all available command types - used as a flag for the command buffer
/// to work out what the next expected command will be
enum class CmdType : uint8_t {
    unknown,
    init,
    set_viewport,
    create_vertex_buffer,
    create_index_buffer,
    create_program,
    create_uniform,
    create_texture,
    create_texture_region,
    set_vertex_buffer,
    set_index_buffer,
    set_program,
    set_uniform,
    set_texture,
    update_uniform,
    draw,
    draw_instanced,
    set_state
};

/// @brief Base command struct - all other render commands derive from this
struct Command {
    explicit Command(const CmdType& cmd_type)
        : type(cmd_type)
    {}

    /// @brief The type flag for the command buffer
    CmdType type;
};

struct SetViewport : public Command {
    explicit SetViewport(Viewport* viewport_ptr)
        : Command(CmdType::set_viewport),
          viewport(viewport_ptr)
    {}

    Viewport* viewport;
};

struct CreateVertexBuffer : public Command {
    CreateVertexBuffer(const uint32_t id, const MemoryBlock& initial_data, const BufferUsage usage)
        : Command(CmdType::create_vertex_buffer),
          buf_id(id),
          data(initial_data),
          buf_usage(usage)
    {}

    uint32_t buf_id;
    MemoryBlock data;
    BufferUsage buf_usage;
};

struct SetVertexBuffer : public Command {
    explicit SetVertexBuffer(const uint32_t id, const uint32_t offset,
                             const uint32_t num_vertices)
        : Command(CmdType::set_vertex_buffer),
          buf_id(id),
          first_vertex(offset),
          count(num_vertices)
    {}

    uint32_t buf_id;
    uint32_t first_vertex;
    uint32_t count;
};

struct CreateIndexBuffer : public Command {
    CreateIndexBuffer(const uint32_t id, const MemoryBlock& initial_data)
        : Command(CmdType::create_index_buffer),
          buf_id(id),
          data(initial_data)
    {}

    uint32_t buf_id;
    MemoryBlock data;
};

struct SetIndexBuffer : public Command {
    explicit SetIndexBuffer(const uint32_t id, const uint32_t offset,
                            const uint32_t num_indices)
        : Command(CmdType::set_index_buffer),
          buf_id(id),
          first_index(offset),
          count(num_indices)
    {}

    uint32_t buf_id;
    uint32_t first_index;
    uint32_t count;
};

struct CreateProgram : public Command {
    CreateProgram(const uint32_t program_id, const Path& vs_path, const Path& frag_path)
        : Command(CmdType::create_program),
          prog_id(program_id)
    {
        vs = static_cast<char*>( malloc(strlen(vs_path.str()) + 1) );
        frag = static_cast<char*>( malloc(strlen(frag_path.str()) + 1) );
        strcpy(vs, vs_path.str());
        strcpy(frag, frag_path.str());
    }

    void destroy()
    {
        if ( vs != nullptr ) {
            free(vs);
        }
        if ( frag != nullptr ) {
            free(frag);
        }
    }

    uint32_t prog_id;
    char* vs;
    char* frag;
};

struct SetProgram : public Command {
    explicit SetProgram(const uint32_t program_id)
        : Command(CmdType::set_program),
          prog_id(program_id)
    {}

    uint32_t prog_id;
};

struct CreateUniform : public Command {
    CreateUniform(const uint32_t u_id, const UniformType type, const uint32_t data_size)
        : Command(CmdType::create_uniform),
          uniform_id(u_id),
          uniform_type(type),
          size(data_size)
    {}

    uint32_t uniform_id;
    UniformType uniform_type;
    uint32_t size;
};

struct SetUniform : public Command {
    SetUniform(const uint32_t u_id, const uint32_t index)
        : Command(CmdType::set_uniform),
          uniform_id(u_id),
          uniform_index(index)
    {}

    uint32_t uniform_id;
    uint32_t uniform_index;
};

struct UpdateUniform : public Command {
    UpdateUniform(const uint32_t u_id, const uint32_t buffer_offset, const MemoryBlock& data)
        : Command(CmdType::update_uniform),
          uniform_id(u_id),
          offset(buffer_offset),
          new_data(data)
    {}

    uint32_t uniform_id;
    uint32_t offset;
    MemoryBlock new_data;
};

struct CreateTexture : public Command {
    CreateTexture(const uint32_t tex_id, const uint32_t img_width, const uint32_t img_height,
                  const PixelFormat::Enum pixel_format, const bool img_mipmapped)
        : Command(CmdType::create_texture),
          tid(tex_id),
          width(img_width),
          height(img_height),
          format(pixel_format),
          mipmapped(img_mipmapped)
    {}

    const uint32_t tid;
    const uint32_t width;
    const uint32_t height;
    const PixelFormat::Enum format;
    const bool mipmapped;
};

struct CreateTextureRegion : public Command {
    CreateTextureRegion(const uint32_t texture, const UIntRect& region,
                        const PixelFormat::Enum pixel_format, uint8_t* region_data)
        : Command(CmdType::create_texture_region),
          tex_id(texture),
          rect(region),
          format(pixel_format),
          data(region_data)
    {}

    const uint32_t tex_id;
    const UIntRect rect;
    const PixelFormat::Enum format;
    uint8_t* data;
};

struct SetTexture : public Command {
    SetTexture(const uint32_t id, const uint32_t tex_index)
        : Command(CmdType::set_texture), tid(id), index(tex_index)
    {}

    const uint32_t tid;
    const uint32_t index;
};

struct Draw : public Command {
    Draw()
        : Command(CmdType::draw)
    {}
};

struct DrawInstanced : public Command {
    DrawInstanced(const uint32_t draw_instance)
        : Command(CmdType::draw_instanced),
          instance(draw_instance)
    {}

    const uint32_t instance;
};

struct SetState : public Command {
    SetState(const uint32_t state_flags)
        : Command(CmdType::set_state), flags(state_flags)
    {}

    const uint32_t flags{};
};


} // namespace rc
} // namespace sky