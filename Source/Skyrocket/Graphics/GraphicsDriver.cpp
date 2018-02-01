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

#include "Skyrocket/Core/Diagnostics/Timespan.hpp"
#include "Skyrocket/Platform/Thread.hpp"
#include "Skyrocket/Graphics/GraphicsDriver.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/GDI/GDI.hpp"

namespace sky {


GraphicsDriver::GraphicsDriver()
    : cmdlist_allocator_(sizeof(CommandList), cmdpool_size_),
      cmdqueue_allocator_(sizeof(MPSCQueue<CommandQueueNode>::Node), cmdpool_size_ * cmdpool_size_),
      cmdqueue_(cmdqueue_allocator_),
      cmdlist_sem_(GDI::max_frames_in_flight),
      vsync_on_(false)
{}

GraphicsDriver::~GraphicsDriver()
{
    render_thread_notified_ = true;
    render_thread_active_ = false;
    render_thread_cv_.notify_one();

    if (render_thread_.joinable()) {
        render_thread_.join();
    }
}

bool GraphicsDriver::init(ThreadSupport threading, Viewport* viewport)
{
    threadsupport_ = threading;
    viewport_ = viewport;
    gdi_ = GDI::create();
    gdi_->init(viewport_);

    if (threadsupport_ == ThreadSupport::multi_threaded) {
        render_thread_active_ = true;
        render_thread_notified_ = false;
        render_thread_ = std::thread(&GraphicsDriver::render_thread_proc, this);
    }

    cmdlist_ = static_cast<CommandList*>(cmdlist_allocator_.allocate(0));
    cmdlist_->start_recording();

    frame_pool_[0].reset(num_frames_);
    frame_pool_[0].begin_frame();
    frame_pool_[0].sim_begin();
    frame_pool_[0].cpu_begin();

    return true;
}

CommandList* GraphicsDriver::command_list()
{
    while (cmdlist_ == nullptr) {
        cmdlist_ = static_cast<CommandList*>(cmdlist_allocator_.allocate(0));
        if (cmdlist_ != nullptr) {
            cmdlist_->start_recording();
            break;
        }
    }

    if (cmdlist_ != nullptr && cmdlist_->state() != CommandList::State::recording) {
        SKY_ERROR("GraphicsDriver", "All available command lists are busy processing");
        return nullptr;
    }

    return cmdlist_;
}

void GraphicsDriver::commit_frame()
{
    frame_pool_[current_frame_].sim_end();

    if (threadsupport_ == ThreadSupport::single_threaded && !vsync_on_) {
        frame_pool_[current_frame_].gdi_begin();
        process_command_list(cmdlist_);
        frame_pool_[current_frame_].gdi_end();
    } else {
        cmdlist_->queue();
        cmdqueue_.push({ cmdlist_, &frame_pool_[current_frame_] });

        cmdlist_ = static_cast<CommandList*>(cmdlist_allocator_.allocate(0));
        if (cmdlist_ != nullptr) {
            cmdlist_->start_recording();
        }

        if (threadsupport_ == ThreadSupport::multi_threaded && !vsync_on_) {
            notify_render_thread();
        }

        cmdlist_sem_.wait();
    }

    frame_pool_[current_frame_].end_frame();

    current_frame_ = (current_frame_ + 1) & (framepool_size_ - 1);

    frame_pool_[current_frame_].reset(++num_frames_);
    frame_pool_[current_frame_].begin_frame();
    frame_pool_[current_frame_].sim_begin();
    frame_pool_[current_frame_].cpu_begin();
}

void GraphicsDriver::process_command_list(CommandList* list)
{
    list->reset_cursor();
    list->start_processing();

    gdi_->commit(list, &frame_pool_[current_frame_]);

    list->end_processing();
    list->clear();
    list->start_recording();
}

void GraphicsDriver::set_viewport(sky::Viewport* viewport)
{
    viewport_ = viewport;
    gdi_->set_viewport(viewport);
}

void GraphicsDriver::set_vsync_enabled(bool enabled)
{
    vsync_on_ = enabled;
    Viewport::render_proc_t render_proc = &GraphicsDriver::render_thread_frame;
    if (threadsupport_ == ThreadSupport::multi_threaded) {
        render_proc = &GraphicsDriver::notify_render_thread;
    }
    viewport_->set_vsync_enabled(this, render_proc, enabled);
}

void GraphicsDriver::notify_render_thread()
{
    render_thread_notified_ = true;
    render_thread_cv_.notify_one();
}

void GraphicsDriver::render_thread_frame()
{
    CommandQueueNode node {nullptr, nullptr};

    if (cmdqueue_.pop(node)) {
        node.frame->gdi_begin();
        {
            process_command_list(node.cmdlist);

            cmdlist_allocator_.free(node.cmdlist);
            cmdlist_sem_.signal();
        }
        node.frame->gdi_end();
    }
}

void GraphicsDriver::render_thread_proc()
{
    while (render_thread_active_) {
        if (cmdqueue_allocator_.blocks_initialized() <= 0) {
            std::unique_lock<std::mutex> lock(render_thread_mutex_);
            render_thread_cv_.wait(lock, [&]() {
                return render_thread_notified_;
            });
            render_thread_notified_ = false;
        }

        render_thread_frame();
    }
}


} // namespace sky