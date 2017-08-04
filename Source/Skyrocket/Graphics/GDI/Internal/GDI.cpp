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

namespace sky {


void GDI::next_frame()
{
    prev_buf = cur_buf;
    cur_buf = static_cast<uint16_t>(cur_buf + 1) % max_frames_in_flight;
    cmdbufs[prev_buf].clear();
    cmdbufs[cur_buf].clear();
}

void GDI::process_commands()
{
    auto cmd_buf = &cmdbufs[prev_buf];

    rc::CmdType* next_cmd_type = nullptr;

    while ( cmd_buf->cursor_pos() <= cmd_buf->end() ) {

        next_cmd_type = cmd_buf->read<rc::CmdType>();

        switch (*next_cmd_type) {
            case rc::CmdType::unknown:
            {

            } break;

            case rc::CmdType::init:
            {
//                auto
//                auto& view = static_cast<rc::SetViewport&>(next_cmd).viewport;
//                initialize(view);
            } break;

            case rc::CmdType::set_viewport:
            {
                auto cmd = cmd_buf->read<rc::SetViewport>();
                auto view = cmd->viewport;
                set_viewport(view);
            } break;

            case rc::CmdType::create_vertex_buffer:
            {
                auto cmd = cmd_buf->read<rc::CreateVertexBuffer>();
                create_vertex_buffer(cmd->buf_id, cmd->data, cmd->buf_usage);
            } break;

            case rc::CmdType::set_vertex_buffer:
            {
                auto cmd = cmd_buf->read<rc::SetVertexBuffer>();
                set_vertex_buffer(cmd->buf_id);
            } break;

            case rc::CmdType::create_index_buffer:
            {

            } break;

            case rc::CmdType::create_shader:
            {

            } break;

            case rc::CmdType::set_shaders:
            {
                auto cmd = cmd_buf->read<rc::SetShaders>();

                auto v_prog = cmd->vertex_program;
                auto f_prog = cmd->fragment_program;

                if ( v_prog == invalid_handle || f_prog == invalid_handle ) {
//                    printf("invali\n");
                } else {
                    set_shaders(v_prog, f_prog);
                }

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

bool GDI::create_shader(const uint32_t  /*shader_id*/, const char*  /*name*/)
{
    // no op
    return false;
};

bool GDI::set_shaders(const uint32_t  /*vertex_id*/, const uint32_t  /*fragment_id*/)
{
    // no op
    return false;
}

void GDI::present()
{
    // no op
}


} // namespace sky