//
//  GraphicsDriver.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/GDI/Experimental/GraphicsDriver.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/GDI/Experimental/GDI_EX.hpp"

namespace sky {
namespace experimental {


GraphicsDriver::GraphicsDriver() = default;

GraphicsDriver::~GraphicsDriver()
{
    if (render_thread_.joinable()) {
        render_thread_notified_ = true;
        render_thread_active_ = false;
        render_thread_cv_.notify_one();
        render_thread_.join();
    }
}

void GraphicsDriver::init(Viewport* viewport)
{
    cmdbuf_pool_ = FixedPoolAllocator(sizeof(CommandBuffer), cmdbuf_max_);

    gdi_ = GDI_EX::create();
    gdi_->init(viewport);

    render_thread_active_ = true;
    render_thread_notified_ = false;
    render_thread_ = std::thread(&GraphicsDriver::render_thread_proc, this);
}

CommandBuffer* GraphicsDriver::make_command_buffer()
{
    auto buf = static_cast<CommandBuffer*>(cmdbuf_pool_.allocate());

    if (buf == nullptr) {
        return nullptr;
    }

    buf->clear();
    return buf;
}

void GraphicsDriver::free_command_buffer(CommandBuffer*& buf)
{
    if (buf->state() == CommandBuffer::State::processing) {
        SKY_ERROR("GraphicsDriver", "Cannot free command buffer that's currently being processed "
            "by the render thread.");
        return;
    }

    if (buf->state() == CommandBuffer::State::recording) {
        SKY_ERROR("GraphicsDriver", "Cannot free command buffer that's in the middle of recording "
            "commands. Please call `end()` on the buffer before freeing it.");
        return;
    }

    cmdbuf_pool_.free(buf);
    buf = nullptr;
}

void GraphicsDriver::submit_command_buffer(CommandBuffer* buf)
{
    buf->start_processing();

    {
        std::unique_lock<std::mutex> lock(cmdbuf_queue_mutex_);
        cmdbuf_queue_.push(buf);
    }

    render_thread_notified_ = true;
    render_thread_cv_.notify_one();
}

void GraphicsDriver::render_thread_proc()
{
    CommandBuffer* cmdbuf = nullptr;
    while (render_thread_active_) {
        if (cmdbuf_queue_.empty()) {
            std::unique_lock<std::mutex> lock(render_thread_mutex_);
            render_thread_cv_.wait(lock, [&]() {
                return render_thread_notified_;
            });
            render_thread_notified_ = false;
        }

        cmdbuf = nullptr;

        {
            std::unique_lock<std::mutex> queue_lock(cmdbuf_queue_mutex_);
            if (!cmdbuf_queue_.empty()) {
                cmdbuf = cmdbuf_queue_.front();
                cmdbuf_queue_.pop();
            }
        }

        if (cmdbuf != nullptr) {
            cmdbuf->reset_cursor();
            gdi_->commit(cmdbuf);
            cmdbuf->end_processing();
            free_command_buffer(cmdbuf);
        }
    }
}


} // namespace experimental
} // namespace sky