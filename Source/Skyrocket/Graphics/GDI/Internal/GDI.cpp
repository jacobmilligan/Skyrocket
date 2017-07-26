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


void GDI::swap_buffers()
{
    prev_buf = cur_buf;
    cur_buf = static_cast<uint16_t>(cur_buf + 1) % max_frames_in_flight;
    cmdbufs[prev_buf].reset();
    cmdbufs[cur_buf].reset();
}

void GDI::process_commands()
{
    rc::CmdType next_cmd_type = rc::CmdType::unknown;

    auto cmd_buf = &cmdbufs[prev_buf];

    while ( cmd_buf->cursor() != cmd_buf->end() ) {
        cmd_buf->read<rc::CmdType>(&next_cmd_type);

        switch (next_cmd_type) {
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
                rc::SetViewport cmd(nullptr);
                cmd_buf->read(&cmd);
                auto view = cmd.viewport;
                set_viewport(view);
            } break;

            case rc::CmdType::create_vertex_buffer:
            {
                rc::CreateVertexBuffer cmd(0, MemoryBlock(), BufferUsage::none);
                cmd_buf->read(&cmd);
                create_vertex_buffer(cmd.buf_id, cmd.data, cmd.buf_usage);
            } break;

            case rc::CmdType::set_vertex_buffer:
            {
                rc::SetVertexBuffer cmd(0);
                cmd_buf->read(&cmd);
                set_vertex_buffer(cmd.buf_id);
            } break;

            case rc::CmdType::create_index_buffer:
            {

            } break;

            case rc::CmdType::create_shader:
            {

            } break;

            case rc::CmdType::set_shaders:
            {
                rc::SetShaders cmd(0, 0);
                cmd_buf->read(&cmd);

                auto v_prog = cmd.vertex_program;
                auto f_prog = cmd.fragment_program;

                if ( v_prog == invalid_handle || f_prog == invalid_handle ) {
                    printf("hey\n");
                } else {
                    set_shaders(v_prog, f_prog);
                }

            } break;
        }
    }
}

bool GDI::initialize(Viewport* viewport)
{
    return false;
}

void GDI::set_viewport(Viewport* viewport)
{
    // no op
}

bool GDI::create_vertex_buffer(const uint32_t vbuf_id, const MemoryBlock& initial_data,
                                       const BufferUsage usage)
{
    // no op
    return false;
}

bool GDI::set_vertex_buffer(const uint32_t vbuf_id)
{
    // no op
    return false;
}

bool GDI::create_shader(const uint32_t shader_id, const char* name)
{
    // no op
    return false;
};

bool GDI::set_shaders(const uint32_t vertex_id, const uint32_t fragment_id)
{
    // no op
    return false;
}


} // namespace sky