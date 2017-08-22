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


struct Semaphore {
    explicit Semaphore(const uint16_t max_count)
        : max_count_(max_count),
          count_(max_count)
    {}

    void wait();
    void signal();
private:
    uint16_t max_count_;
    std::atomic<uint16_t> count_;
    std::mutex mut_;
    std::condition_variable cv_;
};

void thread_sleep(const Timespan& time);


} // namespace sky