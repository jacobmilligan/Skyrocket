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

namespace sky {


struct ShaderProgramData {
    ShaderProgramData(uint32_t vprog, uint32_t fprog)
        : vertex_program(vprog), fragment_program(fprog)
    {}

    uint32_t vertex_program;
    uint32_t fragment_program;
};

struct CreateBufferData {
    CreateBufferData(const uint32_t buf_id, const MemoryBlock& buf_data, BufferUsage buf_usage)
        : id(buf_id), data(buf_data), usage(buf_usage)
    {}

    uint32_t id;
    MemoryBlock data;
    BufferUsage usage;
};

struct RenderCommand {
    enum class Type {
        unknown,
        init,
        set_viewport,
        create_vertex_buffer,
        create_index_buffer,
        create_shader,
        set_vertex_buffer,
        set_shaders
    };

    explicit RenderCommand(const Type& cmd_type)
        : type(cmd_type)
    {}

    Type type;
    void* data;
};


} // namespace sky