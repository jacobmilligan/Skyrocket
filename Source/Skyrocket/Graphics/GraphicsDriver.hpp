//
//  GraphicsDriver.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/GDI/CommandQueue.hpp"

#include <queue>
#include <thread>

namespace sky {

class GDI;

class GraphicsDriver {
public:
    enum class ThreadSupport {
        single_threaded,
        multi_threaded
    };

    GraphicsDriver();
    ~GraphicsDriver();
    bool init(ThreadSupport threading, Viewport* viewport);

    CommandQueue* command_queue();
    void submit_command_queue();
private:
    static constexpr size_t cmdpool_size_ = 64;

    std::unique_ptr<GDI> gdi_;

    CommandQueue cmdpool_[cmdpool_size_];
    size_t current_cmdqueue_{0};

    // Render thread properties/methods
    std::condition_variable render_thread_cv_;
    std::mutex render_thread_mutex_;
    bool render_thread_active_;
    bool render_thread_notified_;
    std::thread render_thread_;

    void render_thread_proc();
};


} // namespace sky