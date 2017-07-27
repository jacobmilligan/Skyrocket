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

#include <condition_variable>

namespace sky {


Renderer::Renderer(const ThreadSupport threading)
    : gdi_(GDI::create()),
      next_vbuf_id_(1),
      next_ibuf_id_(1),
      next_shader_id_(1),
      notified_(false),
      threading_(threading),
      active_(false),
      rendering_(false)
{}

Renderer::~Renderer()
{
    if ( threading_ == ThreadSupport::multithreaded ) {
        active_ = false;
        present();
        if ( render_thread_.joinable() ) {
            render_thread_.join();
        }
    }
}

bool Renderer::initialize(Viewport& view)
{
//    gdi_->write_command(RenderCommand(RenderCommand::Type::init));
    auto success = gdi_->initialize(&view);

    if ( threading_ == ThreadSupport::multithreaded ) {
        render_thread_ = std::thread(&Renderer::frame, this);
    }

    return success;
}

void Renderer::set_viewport(Viewport& viewport)
{
    rc::SetViewport cmd(&viewport);
    gdi_->write_command<rc::SetViewport>(cmd);
}

uint32_t Renderer::create_vertex_buffer(const MemoryBlock& initial_data, const BufferUsage usage)
{
    auto id = next_vbuf_id_;
    next_vbuf_id_++;

    rc::CreateVertexBuffer cmd(id, initial_data, usage);
    gdi_->write_command<rc::CreateVertexBuffer>(cmd);

    return id;
}

void Renderer::set_vertex_buffer(const uint32_t vbuf_id)
{
    rc::SetVertexBuffer cmd(vbuf_id);
    gdi_->write_command<rc::SetVertexBuffer>(cmd);
}

uint32_t Renderer::create_shader(const char* name)
{
    auto id = next_shader_id_;
    next_shader_id_++;

    rc::CreateShader cmd(id, name);

    gdi_->write_command<rc::CreateShader>(cmd);

    return id;
}

bool Renderer::set_shaders(const uint32_t vertex_id, const uint32_t fragment_id)
{
    rc::SetShaders cmd(vertex_id, fragment_id);
    gdi_->write_command<rc::SetShaders>(cmd);

    return true;
}

void Renderer::present()
{
    if ( threading_ == ThreadSupport::multithreaded ) {
//        wait_for_render_finish();
        gdi_->swap_buffers();
        kick_render_thread();
    } else {
        gdi_->swap_buffers();
        gdi_->present();
    }
}

void Renderer::kick_render_thread()
{
    notified_ = true;
    cv_.notify_one();
}

void Renderer::frame()
{
    active_ = true;
    std::mutex mut;

    while ( active_ ) {
        std::unique_lock<std::mutex> lock(mut);

        cv_.wait(lock, [this]() { return notified_; });
        notified_ = false;

        rendering_ = true;
        if ( gdi_ != nullptr ) {
            gdi_->present();
        }
        rendering_ = false;
    }
}

void Renderer::wait_for_render_finish()
{
    while ( rendering_ ) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    }
}


} // namespace sky
