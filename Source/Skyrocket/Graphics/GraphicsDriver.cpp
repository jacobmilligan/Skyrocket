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

#include "Skyrocket/Graphics/GraphicsDriver.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/GDI/GDI.hpp"

namespace sky {


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

bool GraphicsDriver::init(ThreadSupport threading, Viewport* viewport)
{
    gdi_ = GDI::create();
    gdi_->init(viewport);

    render_thread_active_ = true;
    render_thread_notified_ = false;
    render_thread_ = std::thread(&GraphicsDriver::render_thread_proc, this);

    cmdpool_[current_cmdqueue_].start_recording();

    return true;
}

CommandQueue* GraphicsDriver::command_queue()
{
    if (cmdpool_[current_cmdqueue_].state() == CommandQueue::State::processing
        || cmdpool_[current_cmdqueue_].state() == CommandQueue::State::unknown) {
        SKY_ERROR("GraphicsDriver", "All available command queues are busy processing");
        return nullptr;
    }
    return &cmdpool_[current_cmdqueue_];
}

void GraphicsDriver::submit_command_queue()
{
    cmdpool_[current_cmdqueue_].start_processing();
    cmdpool_[current_cmdqueue_].reset_cursor();
    current_cmdqueue_ = (current_cmdqueue_ + 1) & (cmdpool_size_ - 1);
    cmdpool_[current_cmdqueue_].start_recording();

    render_thread_notified_ = true;
    render_thread_cv_.notify_one();
}

void GraphicsDriver::render_thread_proc()
{
    CommandQueue* cmdqueue = nullptr;

    while (render_thread_active_) {
        {
            std::unique_lock<std::mutex> lock(render_thread_mutex_);
            render_thread_cv_.wait(lock, [&]() {
                return render_thread_notified_;
            });
            render_thread_notified_ = false;
        }

        cmdqueue = &cmdpool_[(current_cmdqueue_ - 1) & (cmdpool_size_ - 1)];

        gdi_->commit(cmdqueue);
        cmdqueue->end_processing();
        cmdqueue->clear();
    }
}


} // namespace sky