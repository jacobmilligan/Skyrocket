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
#include "Skyrocket/Core/Memory/PoolAllocator.hpp"
#include "Skyrocket/Core/Containers/MPSCQueue.hpp"
#include "Skyrocket/Platform/Thread.hpp"
#include "Skyrocket/Graphics/Frame.hpp"
#include "Skyrocket/Core/Diagnostics/Stopwatch.hpp"

#include <queue>
#include <thread>

namespace sky {

class GDI;

enum class GraphicsBackend {
    unknown,
    none,
    Metal,
    OpenGL,
    D3D9,
    D3D11,
    D3D12,
    Vulkan,
    last
};

using graphics_backed_list_t = GraphicsBackend[static_cast<size_t>(GraphicsBackend::last)];

void supported_graphics_backends(graphics_backed_list_t& dest);

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
    void commit_frame();

    inline uint32_t frames_queued() const
    {
        return cmdqueue_allocator_.blocks_initialized();
    }

    inline const Frame& get_frame(const size_t offset) const
    {
        SKY_ASSERT(offset < 16, "Offset is less than the number of available frames for inspection");
        auto frame = (current_frame_ - offset) & (framepool_size_ - 1);
        return frame_pool_[frame];
    }

    void set_viewport(Viewport* viewport);

    void set_vsync_enabled(bool enabled);
private:
    static constexpr size_t cmdpool_size_ = 64;
    static constexpr size_t framepool_size_ = 16;

    struct CommandQueueNode {
        CommandList* cmdlist;
        Frame* frame;
    };

    std::unique_ptr<GDI> gdi_;
    bool vsync_on_;
    Viewport* viewport_;

    FixedPoolAllocator cmdlist_allocator_;
    CommandList* cmdlist_;

    FixedPoolAllocator cmdqueue_allocator_;
    MPSCQueue<CommandQueueNode> cmdqueue_;

    // Frame properties
    uint64_t num_frames_{0};
    size_t current_frame_{0};
    Frame frame_pool_[framepool_size_];

    // Render thread properties/methods
    Semaphore cmdlist_sem_;
    ThreadSupport threadsupport_;
    std::condition_variable render_thread_cv_;
    std::mutex render_thread_mutex_;
    bool render_thread_active_;
    bool render_thread_notified_;
    std::thread render_thread_;

    void notify_render_thread();
    void render_thread_proc();
    void render_thread_frame();
    void process_command_list(CommandList* list);
};


} // namespace sky