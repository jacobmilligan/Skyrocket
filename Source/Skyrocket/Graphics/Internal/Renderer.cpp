//
//  Renderer.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 25/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/GDI/GDI.hpp"
#include "Skyrocket/Graphics/Renderer.hpp"

namespace sky {


Renderer::Renderer()
    : gdi_(create_graphics_device_interface()),
      next_vbuf_id_(1),
      next_ibuf_id_(1),
      next_shader_id_(1),
      finished_(false)
{}

Renderer::~Renderer() = default;

bool Renderer::initialize(Viewport& view)
{
//    gdi_->enqueue_command(RenderCommand(RenderCommand::Type::init));
    return gdi_->initialize(&view);
}

void Renderer::set_viewport(Viewport& viewport)
{
    RenderCommand cmd(RenderCommand::Type::set_viewport);
    cmd.data = &viewport;
    gdi_->enqueue_command(cmd);
}

uint32_t Renderer::create_vertex_buffer(const MemoryBlock& initial_data, const BufferUsage usage)
{
    auto id = next_vbuf_id_;
    next_vbuf_id_++;

    RenderCommand cmd(RenderCommand::Type::create_vertex_buffer);
    cmd.data = new CreateBufferData(id, initial_data, usage);

    gdi_->enqueue_command(cmd);

    return id;
}

void Renderer::set_vertex_buffer(const uint32_t vbuf_id)
{
    RenderCommand cmd(RenderCommand::Type::set_vertex_buffer);
    cmd.data = new uint32_t(vbuf_id);

    gdi_->enqueue_command(cmd);
}

uint32_t Renderer::create_shader(const char* name)
{
    auto id = next_shader_id_;
    next_shader_id_++;

    RenderCommand cmd(RenderCommand::Type::create_shader);
    cmd.data = new uint32_t(id);

    gdi_->enqueue_command(cmd);

    return id;
}

bool Renderer::set_shaders(const uint32_t vertex_id, const uint32_t fragment_id)
{
    RenderCommand cmd(RenderCommand::Type::set_shaders);
    cmd.data = new ShaderProgramData(vertex_id, fragment_id);

    gdi_->enqueue_command(cmd);

    return true;
}

void Renderer::present()
{
    finished_ = false;
    gdi_->swap_buffers();
    gdi_->present();
    finished_ = true;
}


} // namespace sky