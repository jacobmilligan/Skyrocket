//
//  CommandList.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Viewport.hpp"
#include "CommandList.hpp"

namespace sky {


uint32_t CommandList::next_handle_ = 1;

void CommandList::set_viewport(Viewport* viewport)
{
    buffer->write_command(CommandType::set_viewport, viewport);
}

uint32_t CommandList::create_vertex_buffer(const MemoryBlock& initial_data,
                                             const BufferUsage usage)
{
    auto handle = make_handle();
    buffer->write_command(CommandType::create_vertex_buffer, CreateVertexBufferData {
        handle, initial_data, usage
    });
    return handle;
}

void CommandList::set_vertex_buffer(const uint32_t vbuf_id, const uint32_t offset,
                                      const uint32_t num_vertices)
{
    buffer->write_command(CommandType::set_vertex_buffer, SetVertexBufferData {
        vbuf_id, offset, num_vertices
    });
}

void CommandList::update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data)
{
    buffer->write_command(CommandType::update_vertex_buffer, UpdateBufferData{
        vbuf_id, data
    });
}

uint32_t CommandList::create_index_buffer(const MemoryBlock& initial_data)
{
    auto handle = make_handle();
    buffer->write_command(CommandType::create_index_buffer, CreateIndexBufferData {
       handle, initial_data
    });
    return handle;
}

void CommandList::set_index_buffer(const uint32_t ibuf_id, const uint32_t offset,
                                     const uint32_t num_indices)
{
    buffer->write_command(CommandType::set_index_buffer, SetVertexBufferData {
        ibuf_id, offset, num_indices
    });
}

uint32_t CommandList::create_uniform(const UniformType type, const uint32_t size)
{
    auto handle = make_handle();
    buffer->write_command(CommandType::create_uniform, CreateUniformData {
        handle, type, size
    });
    return handle;
}

void CommandList::set_uniform(const uint32_t u_id, const uint32_t index)
{
    buffer->write_command(CommandType::set_uniform, SetUniformData {
        u_id, index
    });
}

void CommandList::update_uniform(const uint32_t u_id, const MemoryBlock& data,
                                   const uint32_t offset)
{
    buffer->write_command(CommandType::update_uniform, UpdateUniformData {
        u_id, offset, data
    });
}

uint32_t CommandList::create_program(const Path& vs_path, const Path& frag_path)
{
    CreateProgramData data{};
    data.prog_id = make_handle();
    strcpy(data.vs, vs_path.str());
    strcpy(data.frag, frag_path.str());

    buffer->write_command(CommandType::create_program, data);

    return data.prog_id;
}

bool CommandList::set_program(const uint32_t program_id)
{
    buffer->write_command(CommandType::set_program, program_id);
    return true;
}

uint32_t CommandList::create_texture(const uint32_t width, const uint32_t height,
                                       const PixelFormat::Enum pixel_format, const bool mipmapped)
{
    auto handle = make_handle();
    buffer->write_command(CommandType::create_texture, CreateTextureData {
        handle, width, height, pixel_format, mipmapped
    });
    return handle;
}

void CommandList::create_texture_region(const uint32_t texture, const UIntRect& region,
                                          const PixelFormat::Enum pixel_format, uint8_t* data)
{
    buffer->write_command(CommandType::create_texture_region, CreateTextureRegionData {
        texture, region, pixel_format, data
    });
}

bool CommandList::set_texture(const uint32_t texture, const uint32_t index)
{
    buffer->write_command(CommandType::set_texture, SetTextureData {
        texture, index
    });
    return true;
}

void CommandList::set_state(const uint32_t state_flags)
{
    buffer->write_command(CommandType::set_state, state_flags);
}

void CommandList::draw()
{
    buffer->write_command(CommandType::draw);
}

void CommandList::draw_instanced(const uint32_t instances)
{
    buffer->write_command(CommandType::draw_instanced, instances);
}


} // namespace sky