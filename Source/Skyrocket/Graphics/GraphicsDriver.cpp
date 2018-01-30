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


GraphicsDriver::GraphicsDriver()
    : cmdlist_allocator_(sizeof(CommandList), cmdpool_size_),
      cmdqueue_allocator_(sizeof(MPSCQueue<CommandList*>::Node), cmdpool_size_),
      cmdqueue_(cmdqueue_allocator_)
{}

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
    threadsupport_ = threading;
    gdi_ = GDI::create();
    gdi_->init(viewport);

    if (threadsupport_ == ThreadSupport::multi_threaded) {
        render_thread_active_ = true;
        render_thread_notified_ = false;
        render_thread_ = std::thread(&GraphicsDriver::render_thread_proc, this);
    }

    cmdlist_ = static_cast<CommandList*>(cmdlist_allocator_.allocate(0));
    cmdlist_->start_recording();

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

void GraphicsDriver::commit_command_list()
{
    if (threadsupport_ == ThreadSupport::single_threaded) {

        cmdlist_->reset_cursor();
        cmdlist_->start_processing();
        gdi_->commit(cmdlist_);
        cmdlist_->end_processing();
        cmdlist_->clear();
        cmdlist_->start_recording();

    } else {

        cmdlist_->queue();
        cmdqueue_.push(cmdlist_);

        cmdlist_ = static_cast<CommandList*>(cmdlist_allocator_.allocate(0));
        if (cmdlist_ != nullptr) {
            cmdlist_->start_recording();
        }

        render_thread_notified_ = true;
        render_thread_cv_.notify_one();

    }
}

void GraphicsDriver::render_thread_proc()
{
    CommandList* cmdlist = nullptr;

    while (render_thread_active_) {
        {
            std::unique_lock<std::mutex> lock(render_thread_mutex_);
            render_thread_cv_.wait(lock, [&]() {
                return render_thread_notified_;
            });
            render_thread_notified_ = false;
        }

        while (render_thread_active_ && cmdqueue_.pop(cmdlist)) {
            cmdlist->reset_cursor();
            cmdlist->start_processing();
            gdi_->commit(cmdlist);
            cmdlist->end_processing();
            cmdlist->clear();
            cmdlist->start_recording();
            cmdlist_allocator_.free(cmdlist);
        }
    }
}


} // namespace sky