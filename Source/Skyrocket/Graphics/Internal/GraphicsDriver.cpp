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
#include "Skyrocket/Graphics/GraphicsDriver.hpp"
#include "Skyrocket/Core/Math.hpp"

#include <condition_variable>
#include <Skyrocket/Graphics/Core/Vertex.hpp>

namespace sky {


GraphicsDriver::GraphicsDriver(const ThreadSupport threading)
    :
    gdi_(GDI::create()),
    next_vbuf_id_(1),
    next_ibuf_id_(1),
    next_shader_id_(1),
    next_uniform_id_(1),
    notified_(false),
    threading_(threading),
    active_(false),
    dt_(high_resolution_time()),
    sem(gdi_->max_frames_in_flight)
{}

GraphicsDriver::~GraphicsDriver()
{
    if ( threading_ == ThreadSupport::multithreaded ) {
        active_ = false;
        present();
        render_thread_.join();
        if ( render_thread_.joinable() ) {
        }
    }
}

bool GraphicsDriver::initialize(Viewport& view)
{
//    gdi_->write_command(RenderCommand(RenderCommand::Type::init));
    auto success = gdi_->initialize(&view);

    if ( threading_ == ThreadSupport::multithreaded ) {
        render_thread_ = std::thread(&GraphicsDriver::frame, this);
    }

    return success;
}

void GraphicsDriver::set_viewport(Viewport& viewport)
{
    rc::SetViewport cmd(&viewport);
    gdi_->write_command<rc::SetViewport>(&cmd);
}

uint32_t
GraphicsDriver::create_vertex_buffer(const MemoryBlock& initial_data, const BufferUsage usage)
{
    auto id = next_vbuf_id_;
    ++next_vbuf_id_;

    rc::CreateVertexBuffer cmd(id, initial_data, usage);
    gdi_->write_command<rc::CreateVertexBuffer>(&cmd);

    return id;
}

void GraphicsDriver::set_vertex_buffer(const uint32_t vbuf_id, const uint32_t offset,
                                       const uint32_t num_vertices)
{
    rc::SetVertexBuffer cmd(vbuf_id, offset, num_vertices);
    gdi_->write_command<rc::SetVertexBuffer>(&cmd);
}

uint32_t GraphicsDriver::create_index_buffer(const MemoryBlock& initial_data)
{
    auto id = next_ibuf_id_;
    ++next_ibuf_id_;

    rc::CreateIndexBuffer cmd(id, initial_data);
    gdi_->write_command<rc::CreateIndexBuffer>(&cmd);

    return id;
}

void GraphicsDriver::set_index_buffer(const uint32_t ibuf_id, const uint32_t offset,
                                      const uint32_t num_indices)
{
    rc::SetIndexBuffer cmd(ibuf_id, offset, num_indices);
    gdi_->write_command<rc::SetIndexBuffer>(&cmd);
}

uint32_t GraphicsDriver::create_shader(const char* name)
{
    auto id = next_shader_id_;
    ++next_shader_id_;

    rc::CreateShader cmd(id, name);

    gdi_->write_command<rc::CreateShader>(&cmd);

    return id;
}

bool GraphicsDriver::set_shaders(const uint32_t vertex_id, const uint32_t fragment_id)
{
    rc::SetShaders cmd(vertex_id, fragment_id);
    gdi_->write_command<rc::SetShaders>(&cmd);

    return true;
}

uint32_t GraphicsDriver::create_uniform(const UniformType type, const uint16_t count)
{
    auto id = next_uniform_id_;
    ++next_uniform_id_;

    uint32_t size = 0;

    switch ( type ) {
        case UniformType::vec2:
            size = sizeof(Vector2f);
            break;
        case UniformType::vec3:
            size = sizeof(Vector3f);
            break;
        case UniformType::vec4:
            size = sizeof(Vector4f);
            break;
        case UniformType::mat4:
            size = sizeof(Matrix4f);
            break;
        default:
            size = 0;
    }

    size *= count;

    rc::CreateUniform cmd(id, type, size);
    gdi_->write_command<rc::CreateUniform>(&cmd);
    return id;
}

void GraphicsDriver::set_uniform(const uint32_t u_id, const uint32_t index)
{
    rc::SetUniform cmd(u_id, index);
    gdi_->write_command(&cmd);
}

void GraphicsDriver::update_uniform(const uint32_t u_id, const MemoryBlock& data)
{
    rc::UpdateUniform cmd(u_id, data);
    gdi_->write_command(&cmd);
}

void GraphicsDriver::draw_primitives()
{
    rc::DrawPrimitives cmd;
    gdi_->write_command<rc::DrawPrimitives>(&cmd);
}

void GraphicsDriver::present()
{
    if ( threading_ == ThreadSupport::multithreaded ) {
        kick_render_thread();
        sem.wait();
    } else {
        gdi_->flip();
        gdi_->present();
    }
}

void GraphicsDriver::kick_render_thread()
{
    notified_ = true;
    cv_.notify_one();
}

void GraphicsDriver::frame()
{
    active_ = true;
    std::mutex mut;

    while ( active_ ) {
        std::unique_lock<std::mutex> lock(mut);

        cv_.wait(lock, [this]() { return notified_; });
        notified_ = false;

        if ( gdi_ != nullptr ) {
            gdi_->flip();
            gdi_->present();
            sem.signal();
        }
    }
}


} // namespace sky
