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
#include "Skyrocket/Graphics/Renderer/Definitions.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Platform/Filesystem.hpp"

#include <Shadecc.hpp>

namespace sky {


class Viewport;
class CommandList;

enum class CommandType : uint8_t {
    unknown,
    init,
    set_viewport,
    set_clear_color,
    create_vertex_buffer,
    create_index_buffer,
    create_program,
    create_uniform,
    create_instance_buffer,
    create_texture,
    create_texture_region,
    set_vertex_buffer,
    set_index_buffer,
    set_program,
    set_uniform,
    set_instance_buffer,
    set_texture,
    update_vertex_buffer,
    update_uniform,
    update_instance_buffer,
    draw,
    draw_instanced,
    set_state
};

struct UpdateBufferData {
    uint32_t buf_id{0};
    MemoryBlock data;
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
    const shadecc::ShaderSource& vs;
    const shadecc::ShaderSource& fs;
};

struct CreateUniformData {
    uint32_t uniform_id;
    char name[64];
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

struct CreateInstanceBufferData {
    uint32_t buf_id{0};
    uint32_t stride{0};
    uint32_t size{0};
};

struct UpdateInstanceBufferData {
    uint32_t buf_id{0};
    uint8_t* data{nullptr};
    uint32_t index{0};
};

struct SetInstanceBufferData {
    uint32_t buf_id{0};
    uint32_t index{0};
};

struct CreateTextureData {
    uint32_t tid;
    uint32_t width;
    uint32_t height;
    PixelFormat format;
    bool mipmapped;
};

struct CreateTextureRegionData {
    uint32_t tex_id{0};
    UIntRect rect;
    PixelFormat format{PixelFormat::unknown};
    uint8_t* data{nullptr};
};

struct SetTextureData {
    uint32_t tid;
    uint32_t index;
};


} // namespace sky