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
    set_vertex_buffer,
    set_index_buffer,
    set_program,
    set_uniform,
    update_uniform,
    draw_primitives
};

/// @brief Base command struct - all other render commands derive from this
struct Command {
    explicit Command(const CmdType& cmd_type)
        :
        type(cmd_type)
    {}

    /// @brief The type flag for the command buffer
    CmdType type;
};

struct SetViewport : public Command {
    explicit SetViewport(Viewport* viewport_ptr)
        :
        Command(CmdType::set_viewport),
        viewport(viewport_ptr)
    {}

    Viewport* viewport;
};

struct CreateVertexBuffer : public Command {
    CreateVertexBuffer(const uint32_t id, const MemoryBlock& initial_data, const BufferUsage usage)
        :
        Command(CmdType::create_vertex_buffer),
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
        :
        Command(CmdType::set_vertex_buffer),
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
        :
        Command(CmdType::create_index_buffer),
        buf_id(id),
        data(initial_data)
    {}

    uint32_t buf_id;
    MemoryBlock data;
};

struct SetIndexBuffer : public Command {
    explicit SetIndexBuffer(const uint32_t id, const uint32_t offset,
                            const uint32_t num_indices)
        :
        Command(CmdType::set_index_buffer),
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
          prog_id(program_id),
          vs(vs_path.str()),
          frag(frag_path.str())
    {}

    uint32_t prog_id;
    const char* vs;
    const char* frag;
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
        :
        Command(CmdType::create_uniform),
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
        :
        Command(CmdType::set_uniform),
        uniform_id(u_id),
        uniform_index(index)
    {}

    uint32_t uniform_id;
    uint32_t uniform_index;
};

struct UpdateUniform : public Command {
    UpdateUniform(const uint32_t u_id, const MemoryBlock& data)
        :
        Command(CmdType::update_uniform),
        uniform_id(u_id),
        new_data(data)
    {}

    uint32_t uniform_id;
    MemoryBlock new_data;
};

struct DrawPrimitives : public Command {
    DrawPrimitives()
        :
        Command(CmdType::draw_primitives)
    {}
};


} // namespace rc
} // namespace sky