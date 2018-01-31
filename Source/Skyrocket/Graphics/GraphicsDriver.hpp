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

#include "Skyrocket/Graphics/GDI/CommandList.hpp"

#include <queue>
#include <thread>
#include "Skyrocket/Core/Memory/PoolAllocator.hpp"
#include "Skyrocket/Core/Containers/MPSCQueue.hpp"
#include "Skyrocket/Platform/Thread.hpp"

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

    CommandList* command_list();
    void commit_command_list();

    inline uint32_t frames_queued()
    {
        return cmdqueue_allocator_.blocks_initialized();
    }
private:
    static constexpr size_t cmdpool_size_ = 64;

    std::unique_ptr<GDI> gdi_;

    FixedPoolAllocator cmdlist_allocator_;
    CommandList* cmdlist_;

    FixedPoolAllocator cmdqueue_allocator_;
    MPSCQueue<CommandList*> cmdqueue_;

    // Render thread properties/methods
    Semaphore cmdlist_sem_;
    ThreadSupport threadsupport_;
    std::condition_variable render_thread_cv_;
    std::mutex render_thread_mutex_;
    bool render_thread_active_;
    bool render_thread_notified_;
    std::thread render_thread_;

    void render_thread_proc();
    void process_command_list(CommandList* list);
};


} // namespace sky