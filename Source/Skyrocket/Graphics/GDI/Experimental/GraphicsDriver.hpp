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

#include "Skyrocket/Core/Memory/PoolAllocator.hpp"
#include "Skyrocket/Graphics/GDI/Experimental/CommandBuffer.hpp"

#include <queue>
#include <thread>

namespace sky {
namespace experimental {

class GDI_EX;

class GraphicsDriver {
public:
    GraphicsDriver();
    ~GraphicsDriver();
    bool init(Viewport* viewport);

    CommandBuffer* make_command_buffer();
    void free_command_buffer(CommandBuffer*& buf);
    void submit_command_buffer(CommandBuffer* buf);

private:
    static constexpr size_t cmdbuf_max_ = 16;

    std::unique_ptr<GDI_EX> gdi_;

    FixedPoolAllocator cmdbuf_pool_;
    std::queue<CommandBuffer*> cmdbuf_queue_;
    std::mutex cmdbuf_queue_mutex_;

    // Render thread properties/methods
    std::condition_variable render_thread_cv_;
    std::mutex render_thread_mutex_;
    bool render_thread_active_;
    bool render_thread_notified_;
    std::thread render_thread_;

    void render_thread_proc();
};


} // namespace experimental
} // namespace sky