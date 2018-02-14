//
//  Renderer.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/Renderer/CommandList.hpp"
#include "Skyrocket/Core/Memory/PoolAllocator.hpp"
#include "Skyrocket/Core/Containers/MPSCQueue.hpp"
#include "Skyrocket/Platform/Thread.hpp"
#include "Skyrocket/Graphics/Renderer/FrameInfo.hpp"
#include "Skyrocket/Core/Diagnostics/Stopwatch.hpp"
#include "Skyrocket/Graphics/Renderer/GDI.hpp"

#include <queue>
#include <thread>

namespace sky {

class Renderer {
public:
    enum class ThreadSupport {
        single_threaded,
        multi_threaded
    };

    Renderer();
    ~Renderer();
    bool init(ThreadSupport threading, Viewport* viewport,
              RendererBackend backend = RendererBackend::unknown);

    bool destroy();

    CommandList make_command_list();
    void submit(CommandList& cmdlist);
    void commit_frame();

    inline uint32_t frames_queued() const
    {
        return cmdqueue_allocator_.blocks_initialized();
    }

    inline const FrameInfo& get_frame_info(const size_t offset) const
    {
        SKY_ASSERT(offset < 16, "Offset is less than the number of available frames for inspection");
        auto frame = (current_frame_ - offset) & (framepool_size_ - 1);
        return frame_pool_[frame];
    }

    void set_vsync_enabled(bool enabled);

    void set_backend(RendererBackend backend);

    RendererBackend active_backend() const;
private:
    static constexpr size_t cmdpool_size_ = 12;
    static constexpr size_t framepool_size_ = 16;
    static constexpr size_t max_submissions_in_flight_ = GDI::max_frames_in_flight + 1;

    struct Submission {
        size_t current_list{0};
        CommandList lists[cmdpool_size_]{};

        void reset()
        {
            current_list = 0;
            memset(lists, 0, cmdpool_size_);
        }

        void push(const CommandList& cmdlist)
        {
            lists[current_list++] = cmdlist;
        }
    };

    struct CommandQueueNode {
        Submission* submission;
        FrameInfo* frame;
    };

    std::unique_ptr<GDI> gdi_;
    bool vsync_on_;
    Viewport* viewport_;

    Submission submission_[max_submissions_in_flight_];
    size_t current_submit_{0};
    FixedPoolAllocator cmdpool_;

    FixedPoolAllocator cmdqueue_allocator_;
    MPSCQueue<CommandQueueNode> cmdqueue_;

    // Frame properties
    uint64_t num_frames_{0};
    size_t current_frame_{0};
    FrameInfo frame_pool_[framepool_size_];

    std::mutex vsync_mutex_;
    std::condition_variable vsync_cv_;

    // Render thread properties/methods
    std::mutex cmdpool_mut_;
    Semaphore cmdlist_sem_;
    ThreadSupport threadsupport_;
    std::condition_variable render_thread_cv_;
    std::mutex render_thread_mutex_;
    bool render_thread_active_;
    bool render_thread_notified_;
    std::thread render_thread_;

    void render_thread_start();
    void render_thread_proc();
    void render_thread_shutdown();
    void process_command_list(CommandList* list);

    inline void vsync_notify()
    {
        vsync_cv_.notify_one();
    }

    inline void render_thread_notify()
    {
        render_thread_notified_ = true;
        render_thread_cv_.notify_one();
    }

    inline FrameInfo& current_frame()
    {
        return frame_pool_[current_frame_];
    }

    inline void swap_submit_buffers()
    {
        current_submit_ = (current_submit_ + 1) % max_submissions_in_flight_;
    }

    inline void next_frame_info()
    {
        current_frame_ = (current_frame_ + 1) & (framepool_size_ - 1);
    }
};


} // namespace sky