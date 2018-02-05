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

namespace sky {

GraphicsDriver::GraphicsDriver()
    : cmdpool_(sizeof(CommandBuffer), cmdpool_size_),
      cmdqueue_allocator_(sizeof(MPSCQueue<CommandQueueNode>::Node), GDI::max_frames_in_flight + 1),
      cmdqueue_(cmdqueue_allocator_),
      cmdlist_sem_(GDI::max_frames_in_flight),
      vsync_on_(false)
{}

GraphicsDriver::~GraphicsDriver()
{
    render_thread_shutdown();
}

bool GraphicsDriver::init(ThreadSupport threading, Viewport* viewport)
{
    threadsupport_ = threading;
    viewport_ = viewport;
    gdi_ = GDI::create();
    gdi_->init(viewport_);

    if (threadsupport_ == ThreadSupport::multi_threaded) {
        render_thread_start();
    }

    current_frame().reset(num_frames_);
    current_frame().begin_frame();
    current_frame().sim_begin();
    current_frame().cpu_begin();

    return true;
}

CommandList GraphicsDriver::make_command_list()
{
    auto cmdbuf = static_cast<CommandBuffer*>(cmdpool_.allocate(0));

    while (cmdbuf == nullptr) {
        cmdbuf = static_cast<CommandBuffer*>(cmdpool_.allocate(0));
        if (cmdbuf != nullptr) {
            break;
        }
    }

    return CommandList(cmdbuf);
}

void GraphicsDriver::submit(sky::CommandList& cmdlist)
{
    submission_[current_submit_].push(cmdlist);
}

void GraphicsDriver::commit_frame()
{
    auto submission = &submission_[current_submit_];
    auto single_threaded = threadsupport_ == ThreadSupport::single_threaded;

    if (submission->current_list <= 0) {
        return;
    }

    current_frame().sim_end();

    if (single_threaded) {

        if (vsync_on_) {
            std::unique_lock<std::mutex> lock(vsync_mutex_);
            vsync_cv_.wait(lock);
        }

        current_frame().gdi_begin();
        if (gdi_->begin(&current_frame())) {

            for (int c = 0; c < submission->current_list; ++c) {
                process_command_list(&submission->lists[c]);
                cmdpool_.free(submission->lists[c].buffer);
            }

            gdi_->end(&current_frame());
        }
        current_frame().gdi_end();

    } else {

        // Multi-threaded or single-threaded with vsync on:
        cmdlist_sem_.wait();

        // Commands must be queued even for single-threaded as vsync on certain platforms uses a
        // separate thread
        cmdqueue_.push({ submission, &current_frame() });

        swap_submit_buffers();

        // Notify render thread without vsyncs help if off
        if (!vsync_on_) {
            render_thread_notify();
        }
    }

    submission_[current_submit_].reset();

    current_frame().end_frame();

    next_frame_info();

    current_frame().reset(++num_frames_);
    current_frame().begin_frame();
    current_frame().sim_begin();
    current_frame().cpu_begin();
}

void GraphicsDriver::process_command_list(CommandList* list)
{
    list->reset_cursor();

    gdi_->submit(list->buffer);

    list->clear();
}

void GraphicsDriver::set_viewport(sky::Viewport* viewport)
{
    viewport_ = viewport;
    gdi_->set_viewport(viewport);
}

void GraphicsDriver::render_thread_start()
{
    render_thread_active_ = true;
    render_thread_notified_ = false;
    render_thread_ = std::thread(&GraphicsDriver::render_thread_proc, this);
}

void GraphicsDriver::render_thread_proc()
{
    CommandQueueNode node {nullptr, nullptr};

    while (render_thread_active_) {
        if (cmdqueue_allocator_.blocks_initialized() <= 0) {
            std::unique_lock<std::mutex> lock(render_thread_mutex_);
            render_thread_cv_.wait(lock, [&]() {
                return render_thread_notified_;
            });
            render_thread_notified_ = false;
        }


        while (cmdqueue_.pop(node)) {
            node.frame->gdi_begin();

            if (gdi_->begin(&current_frame())) {
                for (int c = 0; c < node.submission->current_list; ++c) {
                    process_command_list(&node.submission->lists[c]);
                    cmdpool_.free(node.submission->lists[c].buffer);
                }
                gdi_->end(&current_frame());
            }

            node.frame->gdi_end();
            cmdlist_sem_.signal();
        }
    }
}

void GraphicsDriver::render_thread_shutdown()
{
    render_thread_notified_ = true;
    render_thread_active_ = false;
    render_thread_cv_.notify_one();

    if (render_thread_.joinable()) {
        render_thread_.join();
    }
}

void GraphicsDriver::set_graphics_backend(sky::GraphicsBackend backend)
{
    render_thread_shutdown();
    gdi_ = GDI::create(backend, gdi_.get());
    gdi_->init(viewport_);
    render_thread_start();
}

GraphicsBackend GraphicsDriver::active_backend()
{
    return gdi_->backend();
}


} // namespace sky
