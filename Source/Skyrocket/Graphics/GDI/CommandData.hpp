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


class Viewport;
class CommandList;

enum class CommandType : uint8_t {
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

struct CreateVertexBufferData {
    uint32_t buf_id{0};
    MemoryBlock data;
    BufferUsage buf_usage{BufferUsage::none};
};

struct SetVertexBufferData {
    uint32_t buf_id{0};
    uint32_t first_vertex{0};
    uint32_t count{0};
};

struct CreateIndexBufferData {
    uint32_t buf_id{0};
    MemoryBlock data;
};

struct SetIndexBufferData {
    uint32_t buf_id{0};
    uint32_t first_index{0};
    uint32_t count{0};
};

struct CreateProgramData {
    uint32_t prog_id{0};
    char vs[SKY_MAX_PATH]{};
    char frag[SKY_MAX_PATH]{};
};

struct CreateUniformData {
    uint32_t uniform_id;
    UniformType uniform_type;
    uint32_t size;
};

struct SetUniformData {
    uint32_t uniform_id;
    uint32_t uniform_index;
};

struct UpdateUniformData {
    uint32_t uniform_id{0};
    uint32_t offset{0};
    MemoryBlock new_data;
};

struct CreateTextureData {
    uint32_t tid;
    uint32_t width;
    uint32_t height;
    PixelFormat::Enum format;
    bool mipmapped;
};

struct CreateTextureRegionData {
    uint32_t tex_id{0};
    UIntRect rect;
    PixelFormat::Enum format{PixelFormat::unknown};
    uint8_t* data{nullptr};
};

struct SetTextureData {
    uint32_t tid;
    uint32_t index;
};


} // namespace sky