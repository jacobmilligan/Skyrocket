//
//  GDI.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 24/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "GDI.hpp"
#include "CommandList.hpp"


namespace sky {


void GDI::execute_commands(CommandList* cmdbuf)
{
    CommandType* typeptr = nullptr;

    while (cmdbuf->cursor() <= cmdbuf->size()) {

        typeptr = cmdbuf->read<CommandType>();
        if (typeptr == nullptr) {
            break;
        }

        switch (*typeptr) {
            case CommandType::unknown: {

            } break;

            case CommandType::init: {
//                auto
//                auto& view = static_cast<rc::SetViewport&>(next_cmd).viewport;
//                initialize(view);
            } break;

            case CommandType::set_viewport: {
                auto view = cmdbuf->read<Viewport*>();
                set_viewport(*view);
            } break;

            case CommandType::create_vertex_buffer: {
                auto data = cmdbuf->read<CreateVertexBufferData>();
                create_vertex_buffer(data->buf_id, data->data, data->buf_usage);
            } break;

            case CommandType::set_vertex_buffer: {
                auto data = cmdbuf->read<SetVertexBufferData>();
                set_vertex_buffer(data->buf_id);
                state_.vertex_buffer = data->buf_id;
                state_.vertex_count = data->count;
                state_.vertex_offset = data->first_vertex;
            } break;

            case CommandType::update_vertex_buffer: {
                auto data = cmdbuf->read<UpdateBufferData>();
                update_vertex_buffer(data->buf_id, data->data);
            } break;

            case CommandType::create_index_buffer: {
                auto data = cmdbuf->read<CreateIndexBufferData>();
                create_index_buffer(data->buf_id, data->data);
            } break;

            case CommandType::set_index_buffer: {
                auto data = cmdbuf->read<SetIndexBufferData>();
                set_index_buffer(data->buf_id);
                state_.index_buffer = data->buf_id;
                state_.index_count = data->count;
                state_.index_offset = data->first_index;
            } break;

            case CommandType::create_program: {
                auto* data = cmdbuf->read<CreateProgramData>();

                auto prog_id = data->prog_id;

                Path vs(data->vs);
                Path frag(data->frag);

                create_program(prog_id, vs, frag);
            } break;

            case CommandType::set_program: {
                auto idptr = cmdbuf->read<uint32_t>();

                if ( *idptr == invalid_handle ) {
//                    printf("invali\n");
                } else {
                    set_program(*idptr);
                }

            } break;

            case CommandType::create_uniform: {
                auto data = cmdbuf->read<CreateUniformData>();
                create_uniform(data->uniform_id, data->size);
            } break;

            case CommandType::set_uniform: {
                auto data = cmdbuf->read<SetUniformData>();
                set_uniform(data->uniform_id, data->uniform_index);
            } break;

            case CommandType::update_uniform: {
                auto data = cmdbuf->read<UpdateUniformData>();
//                if ( data->type != CommandType::unknown ) {
                update_uniform(data->uniform_id, data->new_data, data->offset);
//                }
            } break;

            case CommandType::create_texture: {
                auto cmd = cmdbuf->read<CreateTextureData>();
                create_texture(cmd->tid, cmd->width, cmd->height, cmd->format, cmd->mipmapped);
            } break;

            case CommandType::create_texture_region: {
                auto cmd = cmdbuf->read<CreateTextureRegionData>();
                create_texture_region(cmd->tex_id, cmd->rect, cmd->format, cmd->data);
            } break;

            case CommandType::set_texture: {
                auto cmd = cmdbuf->read<SetTextureData>();
                set_texture(cmd->tid, cmd->index);
            } break;

            case CommandType::set_state: {
                auto flagsptr = cmdbuf->read<uint32_t>();
                set_state(*flagsptr);
            } break;

            case CommandType::draw: {
                draw();
            } break;

            case CommandType::draw_instanced: {
                auto instanceptr = cmdbuf->read<uint32_t>();
                draw_instanced(*instanceptr);
            } break;
        }
    }
}


bool GDI::init(Viewport*  /*viewport*/)
{
    return false;
}

void GDI::commit(CommandList* cmdbuf)
{
    execute_commands(cmdbuf);
}

void GDI::set_viewport(Viewport*  /*viewport*/)
{
    // no op
}

bool GDI::create_vertex_buffer(const uint32_t  /*vbuf_id*/, const MemoryBlock&  /*initial_data*/,
                               const BufferUsage  /*usage*/)
{
    // no op
    return false;
}

bool GDI::set_vertex_buffer(const uint32_t  /*vbuf_id*/)
{
    // no op
    return false;
}

bool GDI::update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data)
{
    // no op
    return false;
}

bool GDI::create_index_buffer(const uint32_t /*ibuf_id*/, const MemoryBlock& /*initial_data*/)
{
    // no op
    return false;
}

bool GDI::set_index_buffer(const uint32_t /*ibuf_id*/)
{
    // no op
    return false;
}

bool GDI::create_program(const uint32_t /*program_id*/, const Path& /*vs_path*/, const Path& /*frag_path*/)
{
    // no op
    return false;
};

bool GDI::set_program(const uint32_t)
{
    // no op
    return false;
}

bool GDI::create_uniform(const uint32_t  /*u_id*/, const uint32_t /*initial_data*/)
{
    // no op
    return false;
}

void GDI::set_uniform(const uint32_t  /*u_id*/, const uint32_t  /*index*/)
{
    // no op
}

void GDI::update_uniform(const uint32_t  /*u_id*/, const MemoryBlock&  /*data*/, const uint32_t /*offset*/)
{
    // no op
}

void GDI::create_texture(const uint32_t  /*t_id*/, const uint32_t /*width*/,
                         const uint32_t  /*height*/, const PixelFormat::Enum /*pixel_format*/,
                         const bool  /*mipmapped*/)
{
    // no op
}

void GDI::create_texture_region(const uint32_t /*tex_id*/, const UIntRect& /*region*/,
                                const PixelFormat::Enum /*pixel_format*/, uint8_t* /*data*/)
{
    // no op
}

void GDI::set_texture(const uint32_t /*t_id*/, const uint32_t /*index*/)
{
    // no op
}

bool GDI::draw()
{
    // no op
    return false;
}

bool GDI::draw_instanced(const uint32_t /*instance*/)
{
    // no op
    return false;
}

void GDI::set_state(const uint32_t /*flags*/)
{
    //no op
}


} // namespace sky