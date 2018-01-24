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

#include "Skyrocket/Graphics/GDI/Experimental/GDI_EX.hpp"
#include "Skyrocket/Graphics/GDI/Experimental/CommandBuffer.hpp"

namespace sky {
namespace experimental {

std::unique_ptr<GDI_EX> GDI_EX::create() noexcept
{
    return std::make_unique<GDI_EX>();
}

void GDI_EX::process_command_buffer(CommandBuffer* cmdbuf)
{
    CommandType* type = nullptr;

    while (true) {

        type = cmdbuf->read<CommandType>();
        if (type == nullptr) {
            break;
        }

        switch (*type) {
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
//                auto cmd = cmd_buf.read<rc::CreateVertexBuffer>();
//                create_vertex_buffer(cmd->buf_id, cmd->data, cmd->buf_usage);
            } break;

            case CommandType::set_vertex_buffer: {
//                auto cmd = cmd_buf.read<rc::SetVertexBuffer>();
//                set_vertex_buffer(cmd->buf_id);
//                target_.vertex_buffer = cmd->buf_id;
//                target_.vertex_count = cmd->count;
//                target_.vertex_offset = cmd->first_vertex;
            } break;

            case CommandType::create_index_buffer: {
//                auto cmd = cmd_buf.read<rc::CreateIndexBuffer>();
//                create_index_buffer(cmd->buf_id, cmd->data);
            } break;

            case CommandType::set_index_buffer: {
//                auto cmd = cmd_buf.read<rc::SetIndexBuffer>();
//                set_index_buffer(cmd->buf_id);
//                target_.index_buffer = cmd->buf_id;
//                target_.index_count = cmd->count;
//                target_.index_offset = cmd->first_index;
            } break;

            case CommandType::create_program: {
//                auto* cmd = cmd_buf.read<rc::CreateProgram>();
//
//                auto prog_id = cmd->prog_id;
//
//                Path vs(cmd->vs);
//                Path frag(cmd->frag);
//
//                create_program(prog_id, vs, frag);
//                cmd->destroy();
            } break;

            case CommandType::set_program: {
//                auto cmd = cmd_buf.read<rc::SetProgram>();
//
//                auto prog_id = cmd->prog_id;
//
//                if ( prog_id == invalid_handle ) {
////                    printf("invali\n");
//                } else {
//                    set_program(prog_id);
//                }

            } break;

            case CommandType::create_uniform: {
//                auto cmd = cmd_buf.read<rc::CreateUniform>();
//                create_uniform(cmd->uniform_id, cmd->size);
            } break;

            case CommandType::set_uniform: {
//                auto cmd = cmd_buf.read<rc::SetUniform>();
//                set_uniform(cmd->uniform_id, cmd->uniform_index);
            } break;

            case CommandType::update_uniform: {
//                auto cmd = cmd_buf.read<rc::UpdateUniform>();
//                if ( cmd->type != CommandType::unknown ) {
//                    update_uniform(cmd->uniform_id, cmd->new_data, cmd->offset);
//                }
            } break;

            case CommandType::create_texture: {
//                auto cmd = cmd_buf.read<rc::CreateTexture>();
//                create_texture(cmd->tid, cmd->width, cmd->height, cmd->format, cmd->mipmapped);
            } break;

            case CommandType::create_texture_region: {
//                auto cmd = cmd_buf.read<rc::CreateTextureRegion>();
//                create_texture_region(cmd->tex_id, cmd->rect, cmd->format, cmd->data);
            } break;

            case CommandType::set_texture: {
//                auto cmd = cmd_buf.read<rc::SetTexture>();
//                set_texture(cmd->tid, cmd->index);
            } break;

            case CommandType::set_state: {
//                auto cmd = cmd_buf.read<rc::SetState>();
//                set_state(cmd->flags);
            } break;

            case CommandType::draw: {
                draw();
            } break;

            case CommandType::draw_instanced: {
//                auto cmd = cmd_buf.read<rc::DrawInstanced>();
//                draw_instanced(cmd->instance);
            } break;
        }
    }
}


bool GDI_EX::init(Viewport*  /*viewport*/)
{
    return false;
}

void GDI_EX::set_viewport(Viewport*  /*viewport*/)
{
    // no op
}

bool GDI_EX::create_vertex_buffer(const uint32_t  /*vbuf_id*/, const MemoryBlock&  /*initial_data*/,
                               const BufferUsage  /*usage*/)
{
    // no op
    return false;
}

bool GDI_EX::set_vertex_buffer(const uint32_t  /*vbuf_id*/)
{
    // no op
    return false;
}

bool GDI_EX::create_index_buffer(const uint32_t /*ibuf_id*/, const MemoryBlock& /*initial_data*/)
{
    // no op
    return false;
}

bool GDI_EX::set_index_buffer(const uint32_t /*ibuf_id*/)
{
    // no op
    return false;
}

bool GDI_EX::create_program(const uint32_t /*program_id*/, const Path& /*vs_path*/, const Path& /*frag_path*/)
{
    // no op
    return false;
};

bool GDI_EX::set_program(const uint32_t)
{
    // no op
    return false;
}

bool GDI_EX::create_uniform(const uint32_t  /*u_id*/, const uint32_t /*initial_data*/)
{
    // no op
    return false;
}

void GDI_EX::set_uniform(const uint32_t  /*u_id*/, const uint32_t  /*index*/)
{
    // no op
}

void GDI_EX::update_uniform(const uint32_t  /*u_id*/, const MemoryBlock&  /*data*/, const uint32_t /*offset*/)
{
    // no op
}

void GDI_EX::create_texture(const uint32_t  /*t_id*/, const uint32_t /*width*/,
                         const uint32_t  /*height*/, const PixelFormat::Enum /*pixel_format*/,
                         const bool  /*mipmapped*/)
{
    // no op
}

void GDI_EX::create_texture_region(const uint32_t /*tex_id*/, const UIntRect& /*region*/,
                                const PixelFormat::Enum /*pixel_format*/, uint8_t* /*data*/)
{
    // no op
}

void GDI_EX::set_texture(const uint32_t /*t_id*/, const uint32_t /*index*/)
{
    // no op
}

bool GDI_EX::draw()
{
    // no op
    return false;
}

bool GDI_EX::draw_instanced(const uint32_t /*instance*/)
{
    // no op
    return false;
}

void GDI_EX::set_state(const uint32_t /*flags*/)
{
    //no op
}


} // namespace experimental
} // namespace sky