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

#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/GDI/Definitions.hpp"

namespace sky {
namespace rc {


enum class CmdType : uint8_t {
    unknown,
    init,
    set_viewport,
    create_vertex_buffer,
    create_index_buffer,
    create_shader,
    set_vertex_buffer,
    set_index_buffer,
    set_shaders,
    draw_primitives
};

struct Command {
    explicit Command(const CmdType& cmd_type)
        :
        type(cmd_type)
    {}

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

struct SetVertexBuffer: public Command {
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

struct SetIndexBuffer: public Command {
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

struct CreateShader : public Command {
    CreateShader(const uint32_t shader_id, const char* shader_name)
        : Command(CmdType::create_shader),
          sid(shader_id),
          name(shader_name)
    {}

    uint32_t sid;
    const char* name;
};

struct SetShaders : public Command {
    SetShaders(const uint32_t vert_id, const uint32_t frag_id)
        : Command(CmdType::set_shaders),
          vertex_program(vert_id),
          fragment_program(frag_id)
    {}

    uint32_t vertex_program;
    uint32_t fragment_program;
};

struct DrawPrimitives : public Command {
    DrawPrimitives()
        : Command(CmdType::draw_primitives)
    {}
};


} // namespace rc
} // namespace sky