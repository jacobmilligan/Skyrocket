//
//  GenericDriver.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 26/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/GDI/GDI.hpp"
#include "Skyrocket/Platform/Filesystem.hpp"

namespace sky {

void GDI::process_commands()
{
    auto& cmd_buf = cmdbufs_.get_read();

    rc::CmdType* next_cmd_type = nullptr;

    while ( cmd_buf.cursor_pos() < cmd_buf.end() ) {

        next_cmd_type = cmd_buf.read<rc::CmdType>();

        switch ( *next_cmd_type ) {
            case rc::CmdType::unknown: {

            } break;

            case rc::CmdType::init: {
//                auto
//                auto& view = static_cast<rc::SetViewport&>(next_cmd).viewport;
//                initialize(view);
            } break;

            case rc::CmdType::set_viewport: {
                auto cmd = cmd_buf.read<rc::SetViewport>();
                auto view = cmd->viewport;
                set_viewport(view);
            } break;

            case rc::CmdType::create_vertex_buffer: {
                auto cmd = cmd_buf.read<rc::CreateVertexBuffer>();
                create_vertex_buffer(cmd->buf_id, cmd->data, cmd->buf_usage);
            } break;

            case rc::CmdType::set_vertex_buffer: {
                auto cmd = cmd_buf.read<rc::SetVertexBuffer>();
                set_vertex_buffer(cmd->buf_id);
                target_.vertex_buffer = cmd->buf_id;
                target_.vertex_count = cmd->count;
                target_.vertex_offset = cmd->first_vertex;
            } break;

            case rc::CmdType::create_index_buffer: {
                auto cmd = cmd_buf.read<rc::CreateIndexBuffer>();
                create_index_buffer(cmd->buf_id, cmd->data);
            } break;

            case rc::CmdType::set_index_buffer: {
                auto cmd = cmd_buf.read<rc::SetIndexBuffer>();
                set_index_buffer(cmd->buf_id);
                target_.index_buffer = cmd->buf_id;
                target_.index_count = cmd->count;
                target_.index_offset = cmd->first_index;
            } break;

            case rc::CmdType::create_program: {
                auto* cmd = cmd_buf.read<rc::CreateProgram>();

                auto prog_id = cmd->prog_id;

                Path vs(cmd->vs);
                Path frag(cmd->frag);

                create_program(prog_id, vs, frag);
            } break;

            case rc::CmdType::set_program: {
                auto cmd = cmd_buf.read<rc::SetProgram>();

                auto prog_id = cmd->prog_id;

                if ( prog_id == invalid_handle ) {
//                    printf("invali\n");
                } else {
                    set_program(prog_id);
                }

            } break;

            case rc::CmdType::create_uniform: {
                auto cmd = cmd_buf.read<rc::CreateUniform>();
                create_uniform(cmd->uniform_id, cmd->size);
            } break;

            case rc::CmdType::set_uniform: {
                auto cmd = cmd_buf.read<rc::SetUniform>();
                set_uniform(cmd->uniform_id, cmd->uniform_index);
            } break;

            case rc::CmdType::update_uniform: {
                auto cmd = cmd_buf.read<rc::UpdateUniform>();
                if ( cmd->type != rc::CmdType::unknown ) {
                    update_uniform(cmd->uniform_id, cmd->new_data, cmd->offset);
                }
            } break;

            case rc::CmdType::create_texture: {
                auto cmd = cmd_buf.read<rc::CreateTexture>();
                create_texture(cmd->tid, cmd->data, cmd->width, cmd->height, cmd->bpp, cmd->mipmapped);
            } break;

            case rc::CmdType::set_texture: {
                auto cmd = cmd_buf.read<rc::SetTexture>();
                set_texture(cmd->tid, cmd->index);
            } break;

            case rc::CmdType::set_state: {
                auto cmd = cmd_buf.read<rc::SetState>();
                set_state(cmd->flags);
            } break;

            case rc::CmdType::draw: {
                cmd_buf.read<rc::Draw>();
                draw();
            } break;

            case rc::CmdType::draw_instanced: {
                auto cmd = cmd_buf.read<rc::DrawInstanced>();
                draw_instanced(cmd->instance);
            } break;
        }
    }
}

bool GDI::initialize(Viewport*  /*viewport*/)
{
    return false;
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

void GDI::create_texture(const uint32_t  /*t_id*/, const uint8_t* /*data*/, const int32_t /*width*/,
                         const int32_t  /*height*/, const int32_t /*bytes_per_pixel*/,
                         const bool  /*mipmapped*/)
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

bool GDI::draw_instanced(const uint32_t instance)
{
    // no op
    return false;
}

void GDI::set_state(const uint32_t flags)
{
    //no op
}

void GDI::present()
{
    // no op
}



} // namespace sky