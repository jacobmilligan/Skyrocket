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

namespace sky {


GraphicsDriver::GraphicsDriver()
    : gdi_(GDI::create()),
      next_vbuf_id_(1),
      next_ibuf_id_(1),
      next_uniform_id_(1),
      next_program_id_(1),
      next_texture_id_(1),
      notified_(false),
      active_(false),
      dt_(high_resolution_time()),
      sem_(gdi_->max_frames_in_flight)
{}

GraphicsDriver::~GraphicsDriver()
{
    if ( threading_ == ThreadSupport::multithreaded ) {
        active_ = false;
        commit();
        render_thread_.join();
        if ( render_thread_.joinable() ) {
        }
    }
}

bool GraphicsDriver::initialize(const ThreadSupport threading, Viewport* view)
{
    threading_ = threading;

//    gdi_->write_command(RenderCommand(RenderCommand::Type::init));
    auto success = gdi_->initialize(view);

    if ( threading_ == ThreadSupport::multithreaded ) {
        render_thread_ = std::thread(&GraphicsDriver::frame, this);
    }

    return success;
}

void GraphicsDriver::set_viewport(Viewport* viewport)
{
    rc::SetViewport cmd(viewport);
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

uint32_t GraphicsDriver::create_program(const Path& vs_path, const Path& frag_path)
{
    auto id = next_program_id_;
    ++next_program_id_;

    rc::CreateProgram cmd(id, vs_path, frag_path);

    gdi_->write_command<rc::CreateProgram>(&cmd);

    return id;
}

bool GraphicsDriver::set_program(const uint32_t program_id)
{
    rc::SetProgram cmd(program_id);
    gdi_->write_command<rc::SetProgram>(&cmd);

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

void GraphicsDriver::update_uniform(const uint32_t u_id, const MemoryBlock& data,
                                    const uint32_t offset)
{
    rc::UpdateUniform cmd(u_id, offset, data);
    gdi_->write_command(&cmd);
}

uint32_t GraphicsDriver::create_texture(const Image& img, const bool mipmapped)
{
    auto id = next_texture_id_;
    ++next_texture_id_;

    rc::CreateTexture cmd(id, img.data, img.width, img.height, img.bytes_per_pixel, mipmapped);
    gdi_->write_command<rc::CreateTexture>(&cmd);

    return id;
}

bool GraphicsDriver::set_texture(const uint32_t texture, const uint32_t index)
{
    rc::SetTexture cmd(texture, index);
    gdi_->write_command<rc::SetTexture>(&cmd);
    return true;
}

void GraphicsDriver::draw()
{
    rc::Draw cmd;
    gdi_->write_command<rc::Draw>(&cmd);
}

void GraphicsDriver::draw_instanced(const uint32_t instances)
{
    rc::DrawInstanced cmd(instances);
    gdi_->write_command<rc::DrawInstanced>(&cmd);
}

void GraphicsDriver::set_state(const uint32_t state_flags)
{
    rc::SetState cmd(state_flags);
    gdi_->write_command<rc::SetState>(&cmd);
}

void GraphicsDriver::commit()
{
    if ( threading_ == ThreadSupport::multithreaded ) {
        std::unique_lock<std::mutex> lock(mut_);

        notified_ = true;
        cv_.notify_one();
        sem_.wait();
        gdi_->flip();
    } else {
        gdi_->flip();
        gdi_->commit();
    }
}

void GraphicsDriver::frame()
{
    active_ = true;

    while ( active_ ) {
        std::unique_lock<std::mutex> lock(mut_);

        cv_.wait(lock, [this]() { return notified_; });
        notified_ = false;

        if ( gdi_ != nullptr ) {
            gdi_->commit();
            sem_.signal();
        }
    }
}


} // namespace sky
