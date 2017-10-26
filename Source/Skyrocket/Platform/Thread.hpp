//
//  Thread.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 7/08/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>
#include <mutex>

namespace sky {

struct Timespan;

/// @brief A basic semaphore object that allows up to UINT16_MAX resources
struct Semaphore {
    explicit Semaphore(const int16_t max_count)
        : max_count_(max_count),
          count_(max_count)
    {}

    /// @brief Puts the calling thread to sleep until a new resource is available
    void wait();

    /// @brief Releases a resource and notifies the next waiting thread
    void signal();
private:
    int16_t max_count_;
    std::atomic<int16_t> count_;
    std::mutex mut_;
    std::condition_variable cv_;
};

/// @brief Puts the calling thread to sleep for the given timespan
/// @param time
void thread_sleep(const Timespan& time);


} // namespace sky