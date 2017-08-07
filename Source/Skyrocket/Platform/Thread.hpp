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


struct Semaphore {
    explicit Semaphore(const uint16_t max_count)
        : max_count_(max_count),
          count_(max_count)
    {}

    void wait()
    {
        std::unique_lock<std::mutex> lock(mut_);
        cv_.wait(lock, [&]() {
            return count_ > 0;
        });
        --count_;
        if ( count_ < 0 ) {
            count_ = 0;
        }
    }

    void signal()
    {
        std::unique_lock<std::mutex> lock(mut_);
        ++count_;
        if ( count_ > max_count_ ) {
            count_ = max_count_;
        }
        cv_.notify_one();
    }
private:
    uint16_t max_count_;
    std::atomic<uint16_t> count_;
    std::mutex mut_;
    std::condition_variable cv_;
};


} // namespace sky